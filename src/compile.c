#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "compile.h"
#include "gwion.h"
#include "pass.h"
#include "shreduler_private.h"

enum compile_type { COMPILE_NAME, COMPILE_MSTR, COMPILE_FILE };

struct Compiler {
  const char       *base;
  m_str             name;
  union {
    const char*     data;
    FILE *          file;
  };
  Ast               ast;
  struct Vector_    args;
  VM_Shred          shred;
  MP_Vector        *values;
  enum compile_type type;
};

ANN static void compiler_name(struct Compiler *c) {
  m_str d = strdup(c->base);
  c->name = strsep(&d, ":");
  if (d) vector_init(&c->args);
  while (d) vector_add(&c->args, (vtype)strdup(strsep(&d, ":")));
  xfree(d);
}

ANN static inline void compiler_error(struct Compiler *const c) {
  if (c->args.ptr) {
    const Vector v = &c->args;
    for (m_uint i = 0; i < vector_size(v); ++i) {
      const m_str str = (m_str)vector_at(v, i);
      if (str) xfree((m_str)vector_at(v, i));
    }
    vector_release(v);
  }
}

ANN static void compiler_clean(const struct Compiler *c) {
  if (c->name) xfree(c->name);
  /* test c->type because COMPILE_FILE does not own file */
  if (c->type != COMPILE_FILE && c->file) fclose(c->file);
}

ANN static bool _compiler_open(struct Compiler *c) {
  if (c->type == COMPILE_NAME) {
    m_str name = c->name;
    c->name    = realpath(name, NULL);
    xfree(name);
    return c->name ? !!(c->file = fopen(c->name, "r")) : false;
  } else if (c->type == COMPILE_MSTR) {
    c->file = c->data ? fmemopen((void*)c->data, strlen(c->data), "r") : NULL;
    return !!c->file;
  }
  return true;
}

#ifndef BUILD_ON_WINDOWS
#include <sys/stat.h>
ANN static bool is_reg(const m_str path) {
  struct stat s = {};
  stat(path, &s);
  return !S_ISDIR(s.st_mode) && (S_ISREG(s.st_mode) || !S_ISFIFO(s.st_mode));
}

#else
ANN static bool is_reg(const m_str path) {
  const DWORD dw = GetFileAttributes(path);
  return !(dw == INVALID_FILE_ATTRIBUTES || dw & FILE_ATTRIBUTE_DIRECTORY);
}
#endif

ANN static inline bool compiler_open(struct Compiler *c) {
  char name[strlen(c->name) + 1];
  strcpy(name, c->name);
#ifndef __FUZZING__
  if ((c->type == COMPILE_FILE || c->type == COMPILE_NAME) && !is_reg(name)) {
    gw_err(_("'%s': is a not a regular file\n"), name);
    return false;
  }
#endif
  if (!_compiler_open(c)) {
    compiler_error(c);
    gw_err(_("can't open '%s'\n"), name);
    return false;
  }
  return true;
}

ANN static inline bool _passes(struct Gwion_ *gwion, struct Compiler *c) {
  for (m_uint i = 0; i < vector_size(&gwion->data->passes->vec); ++i) {
    const compilation_pass pass =
        (compilation_pass)vector_at(&gwion->data->passes->vec, i);
    if(!pass(gwion->env, &c->ast)) {
      gwion->data->errored = true;
      return false;
    }
  }
  return true;
}

ANN static inline bool passes(struct Gwion_ *gwion, struct Compiler *c) {
  const Env     env = gwion->env;
  const Context ctx = new_context(env->gwion->mp, c->ast, env->name);
  env_reset(env);
  load_context(ctx, env);
  for(uint32_t i = 0; i < mp_vector_len(c->values); i++) {
    const Value v = *mp_vector_at(c->values, Value, i);
    set_vflag(v, vflag_builtin); // TODO: we should copy the values, maybe
    if(isa(v->type, gwion->type[et_class])) {
      const Type t = (Type)v->d.ptr;
      type_addref(t);
      mk_class(gwion->env, t, t->info->value->from->loc);
      nspc_add_type(gwion->env->curr, insert_symbol(gwion->st, v->name), t);
    } else
      valid_value(gwion->env, insert_symbol(gwion->st, v->name), v);
  }
  const bool ret = _passes(gwion, c);
  ctx->tree = c->ast;
  if (ret) //{
    nspc_commit(env->curr);
  if (ret || env->context->global)
    vector_add(&env->scope->known_ctx, (vtype)ctx);
  else { // nspc_rollback(env->global_nspc);
    context_remref(ctx, env->gwion);
  }
  unload_context(ctx, env);
  return ret;
}

ANN static inline bool _check(struct Gwion_ *gwion, struct Compiler *c) {
  struct AstGetter_ arg = {c->name, c->file, gwion->st, .ppa = gwion->ppa};
  CHECK_B((c->ast = parse(&arg)));
  gwion->env->name = c->name;
  return passes(gwion, c);
}

ANN static m_uint _compile(struct Gwion_ *gwion, struct Compiler *c) {
  if (!compiler_open(c)) return 0;
  if (!_check(gwion, c)) return 0;
  if (gwion->emit->info->code) {
    c->shred                 = new_vm_shred(gwion->mp, gwion->emit->info->code);
    c->shred->info->args.ptr = c->args.ptr;
    vm_add_shred(gwion->vm, c->shred);
    gwion->emit->info->code = NULL;
    return c->shred->tick->xid;
  }
  return true;
}

ANN static m_uint compile(struct Gwion_ *gwion, struct Compiler *c) {
  compiler_name(c);
  gwt_lock(&gwion->data->mutex);
  const m_uint ret = _compile(gwion, c);
  gwt_unlock(&gwion->data->mutex);
  compiler_clean(c);
  return ret;
}

ANN m_uint compile_filename_values(struct Gwion_ *gwion, const char *filename, MP_Vector *values) {
  struct Compiler c = {.base = filename, .type = COMPILE_NAME, .values=values};
  return compile(gwion, &c);
}

ANN m_uint compile_string_values(struct Gwion_ *gwion, const char *filename,
                          const char *data, MP_Vector *values) {
  struct Compiler c = {.base = filename, .type = COMPILE_MSTR, .data = data, .values=values};
  return compile(gwion, &c);
}

ANN m_uint compile_file_values(struct Gwion_ *gwion, const char *filename,
                        FILE *file, MP_Vector *values) {
  struct Compiler c = {.base = filename, .type = COMPILE_FILE, .file = file, .values=values};
  return compile(gwion, &c);
}

ANN m_uint compile_filename_xid_values(struct Gwion_ *gwion, const char* filename,
                                const m_uint xid, MP_Vector *values) {
  struct Compiler c = {.base = filename, .type = COMPILE_NAME, .values=values};
  if (!compile(gwion, &c)) return 0;
  assert(c.shred);
  return c.shred->tick->xid = xid;
}

ANN m_uint compile_string_xid_values(struct Gwion_ *gwion, const char *filename,
                              const char *data, const m_uint xid, MP_Vector *values) {
  struct Compiler c = {.base = filename, .type = COMPILE_MSTR, .data = data, .values=values};
  if (!compile(gwion, &c)) return 0;
  assert(c.shred);
  gwion->vm->shreduler->shred_ids--;
  return c.shred->tick->xid = xid;
}

ANN m_uint compile_file_xid_values(struct Gwion_ *gwion, const char * filename,
                            FILE *file, const m_uint xid, MP_Vector *values) {
  struct Compiler c = {.base = filename, .type = COMPILE_FILE, .file = file, .values=values};
  if (!compile(gwion, &c)) return 0;
  assert(c.shred);
  return c.shred->tick->xid = xid;
}
