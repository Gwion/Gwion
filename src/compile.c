#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "compile.h"
#include "gwion.h"
#include "pass.h"
#include "clean.h"

enum compile_type { COMPILE_NAME, COMPILE_MSTR, COMPILE_FILE };

struct Compiler {
  const m_str       base;
  m_str             name;
  m_str             data;
  FILE *            file;
  Ast               ast;
  struct Vector_    args;
  VM_Shred          shred;
  enum compile_type type;
};

ANN static void compiler_name(struct Compiler *c) {
  m_str d = strdup(c->base);
  c->name = strsep(&d, ":");
  if (d) vector_init(&c->args);
  while (d) vector_add(&c->args, (vtype)strdup(strsep(&d, ":")));
  free(d);
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

ANN static m_bool _compiler_open(struct Compiler *c) {
  if (c->type == COMPILE_NAME) {
    m_str name = c->name;
    c->name    = realpath(name, NULL);
    xfree(name);
    return c->name ? !!(c->file = fopen(c->name, "r")) : GW_ERROR;
  } else if (c->type == COMPILE_MSTR) {
    c->file = c->data ? fmemopen(c->data, strlen(c->data), "r") : NULL;
    return c->file ? GW_OK : GW_ERROR;
  }
  return GW_OK;
}

#ifndef BUILD_ON_WINDOWS
#include <sys/stat.h>
ANN static int is_reg(const m_str path) {
  struct stat s = {};
  stat(path, &s);
  return !S_ISDIR(s.st_mode) && (S_ISREG(s.st_mode) || !S_ISFIFO(s.st_mode));
}

#else
ANN static m_bool is_reg(const m_str path) {
  const DWORD dw = GetFileAttributes(path);
  return !(dw == INVALID_FILE_ATTRIBUTES || dw & FILE_ATTRIBUTE_DIRECTORY);
}
#endif

ANN static inline m_bool compiler_open(struct Compiler *c) {
  char name[strlen(c->name) + 1];
  strcpy(name, c->name);
#ifndef __FUZZING__
  if ((c->type == COMPILE_FILE || c->type == COMPILE_NAME) && !is_reg(name)) {
    gw_err(_("'%s': is a not a regular file\n"), name);
    return GW_ERROR;
  }
#endif
  if (_compiler_open(c) < 0) {
    compiler_error(c);
    gw_err(_("can't open '%s'\n"), name);
    return GW_ERROR;
  }
  return GW_OK;
}

ANN static inline m_bool _passes(struct Gwion_ *gwion, struct Compiler *c) {
  for (m_uint i = 0; i < vector_size(&gwion->data->passes->vec); ++i) {
    const compilation_pass pass =
        (compilation_pass)vector_at(&gwion->data->passes->vec, i);
    CHECK_BB(pass(gwion->env, c->ast));
  }
  return GW_OK;
}

ANN static inline m_bool passes(struct Gwion_ *gwion, struct Compiler *c) {
  const Env     env = gwion->env;
  const Context ctx = new_context(env->gwion->mp, c->ast, env->name);
  env_reset(env);
  load_context(ctx, env);
  const m_bool ret = _passes(gwion, c);
  if (ret > 0) //{
    nspc_commit(env->curr);
  if (ret > 0 || env->context->global)
    vector_add(&env->scope->known_ctx, (vtype)ctx);
  else { // nspc_rollback(env->global_nspc);
    if (!ctx->error) {
      gw_err(_("{-}while compiling file `{0}{/}%s{-}`{0}\n"), c->base);
      ctx->error = 1;
    }
    context_remref(ctx, env->gwion);
  }
  unload_context(ctx, env);
  return ret;
}

ANN static inline m_bool _check(struct Gwion_ *gwion, struct Compiler *c) {
  struct AstGetter_ arg = {c->name, c->file, gwion->st, .ppa = gwion->ppa};
  CHECK_OB((c->ast = parse(&arg)));
  gwion->env->name = c->name;
  const m_bool ret = passes(gwion, c);
  if (!arg.global) ast_cleaner(gwion, c->ast);
  return ret;
}

ANN static m_uint _compile(struct Gwion_ *gwion, struct Compiler *c) {
  if (compiler_open(c) < 0) return 0;
  if (_check(gwion, c) < 0) return 0;
  if (gwion->emit->info->code) {
    c->shred                 = new_vm_shred(gwion->mp, gwion->emit->info->code);
    c->shred->info->args.ptr = c->args.ptr;
    vm_add_shred(gwion->vm, c->shred);
    gwion->emit->info->code = NULL;
    return c->shred->tick->xid;
  }
  return GW_OK;
}

ANN static m_uint compile(struct Gwion_ *gwion, struct Compiler *c) {
  compiler_name(c);
  MUTEX_LOCK(gwion->data->mutex);
  const m_uint ret = _compile(gwion, c);
  MUTEX_UNLOCK(gwion->data->mutex);
  compiler_clean(c);
  return ret;
}

ANN m_uint compile_filename(struct Gwion_ *gwion, const m_str filename) {
  struct Compiler c = {.base = filename, .type = COMPILE_NAME};
  return compile(gwion, &c);
}

ANN m_uint compile_string(struct Gwion_ *gwion, const m_str filename,
                          const m_str data) {
  struct Compiler c = {.base = filename, .type = COMPILE_MSTR, .data = data};
  return compile(gwion, &c);
}

ANN m_uint compile_file(struct Gwion_ *gwion, const m_str filename,
                        FILE *file) {
  struct Compiler c = {.base = filename, .type = COMPILE_FILE, .file = file};
  return compile(gwion, &c);
}

ANN m_uint compile_filename_xid(struct Gwion_ *gwion, const m_str filename,
                                const m_uint xid) {
  struct Compiler c = {.base = filename, .type = COMPILE_NAME};
  if (!compile(gwion, &c)) return 0;
  assert(c.shred);
  return c.shred->tick->xid = xid;
}

ANN m_uint compile_string_xid(struct Gwion_ *gwion, const m_str filename,
                              const m_str data, const m_uint xid) {
  struct Compiler c = {.base = filename, .type = COMPILE_MSTR, .data = data};
  if (!compile(gwion, &c)) return 0;
  assert(c.shred);
  return c.shred->tick->xid = xid;
}

ANN m_uint compile_file_xid(struct Gwion_ *gwion, const m_str filename,
                            FILE *file, const m_uint xid) {
  struct Compiler c = {.base = filename, .type = COMPILE_FILE, .file = file};
  if (!compile(gwion, &c)) return 0;
  assert(c.shred);
  return c.shred->tick->xid = xid;
}
