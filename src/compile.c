#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "compile.h"
#include "gwion.h"
#include "pass.h"

enum compile_type {
  COMPILE_NAME,
  COMPILE_MSTR,
  COMPILE_FILE
};

struct Compiler {
  const m_str base;
  m_str  name;
  m_str data;
  FILE*  file;
  Ast    ast;
  Vector args;
  enum compile_type type;
};

static void compiler_name(MemPool p, struct Compiler* c) {
  m_str d = strdup(c->base);
  c->name = strsep(&d, ":");
  if(d)
    c->args = new_vector(p);
  while(d)
    vector_add(c->args, (vtype)strdup(strsep(&d, ":")));
  free(d);
}

static inline void compiler_error(MemPool p, const struct Compiler* c) {
  if(c->args) {
    for(m_uint i = 0; i < vector_size(c->args); ++i) {
      const m_str str = (m_str)vector_at(c->args, i);
      if(str)
        xfree((m_str)vector_at(c->args, i));
    }
    free_vector(p, c->args);
  }
}

static void compiler_clean(MemPool p, const struct Compiler* c) {
  if(c->name)
    xfree(c->name);
  /* test c->type because COMPILE_FILE does not own file */
  if(c->type != COMPILE_FILE && c->file)
    fclose(c->file);
  if(c->ast)
    free_ast(p, c->ast);
}

static m_bool _compiler_open(struct Compiler* c) {
  if(c->type == COMPILE_NAME) {
    m_str name = c->name;
    c->name = realpath(name, NULL);
    xfree(name);
    return c->name ? !!(c->file = fopen(c->name, "r")) : GW_ERROR;
  } else if(c->type == COMPILE_MSTR) {
    c->file = fmemopen(c->data, strlen(c->data), "r");
    return c->file ? GW_OK : GW_ERROR;
  }
  return GW_OK;
}

static inline m_bool compiler_open(MemPool p, struct Compiler* c) {
  char name[strlen(c->name) + 1];
  strcpy(name, c->name);
  if(_compiler_open(c) < 0) {
    compiler_error(p, c);
    gw_err(_("'%s': no such file\n"), name);
    return GW_ERROR;
  }
  return GW_OK;
}

static inline m_bool _check(struct Gwion_* gwion, struct Compiler* c) {
  struct ScannerArg_ arg = { c->name, c->file, gwion->st };
  CHECK_OB((c->ast = parse(&arg)))
  gwion->env->name = c->name;
  for(m_uint i = 0; i < vector_size(&gwion->data->pass); ++i) {
    const compilation_pass pass = (compilation_pass)vector_at(&gwion->data->pass, i);
    CHECK_BB(pass(gwion->env, c->ast))
  }
  return GW_OK;
}

static m_uint _compile(struct Gwion_* gwion, struct Compiler* c) {
  CHECK_BB(compiler_open(gwion->mp, c))
  if(_check(gwion, c) < 0) {
    gw_err(_("while compiling file '%s'\n"), c->base);
    return 0;
  }
  if(gwion->emit->info->code) {
    const VM_Shred shred = new_vm_shred(gwion->mp, gwion->emit->info->code);
    shred->info->args = c->args;
    vm_add_shred(gwion->vm, shred);
    gwion->emit->info->code = NULL;
    return shred->tick->xid;
  }
  return GW_OK;
}

static m_uint compile(struct Gwion_* gwion, struct Compiler* c) {
  compiler_name(gwion->mp, c);
  MUTEX_LOCK(gwion->data->mutex);
  const m_uint ret = _compile(gwion, c);
  MUTEX_UNLOCK(gwion->data->mutex);
  compiler_clean(gwion->mp, c);
  return ret;
}

m_uint compile_filename(struct Gwion_* gwion, const m_str filename) {
  struct Compiler c = { .base=filename, .type=COMPILE_NAME };
  return compile(gwion, &c);
}

m_uint compile_string(struct Gwion_* vm, const m_str filename, const m_str data) {
  struct Compiler c = { .base=filename, .type=COMPILE_MSTR, .data=data };
  return compile(vm, &c);
}

m_uint compile_file(struct Gwion_* vm, const m_str filename, FILE* file) {
  struct Compiler c = { .base=filename, .type=COMPILE_FILE, .file=file };
  return compile(vm, &c);
}