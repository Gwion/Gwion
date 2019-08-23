#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "instr.h"
#include "type.h"
#include "emit.h"
#include "compile.h"
#include "gwion.h"

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
  if(c->file)
    fclose(c->file);
  if(c->ast)
    free_ast(p, c->ast);
}

static m_bool _compiler_open(struct Compiler* c) {
  if(c->type == COMPILE_NAME) {
    m_str name = c->name;
    c->name = realpath(name, NULL);
    xfree(name);
    return c->name ? !!(c->file = fopen(c->name, "r")) : -1;
  } else if(c->type == COMPILE_MSTR)
    return (c->file = fmemopen(c->data, strlen(c->data), "r")) ? 1 : - 1;
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

static m_bool check(struct Gwion_* gwion, struct Compiler* c) {
  struct ScannerArg_ arg = { c->name, c->file, gwion->st };
  MUTEX_LOCK(gwion->data->mutex);
  CHECK_OB((c->ast = parse(&arg)))
  gwion->env->name = c->name;
  const m_bool ret = type_engine_check_prog(gwion->env, c->ast);
  MUTEX_UNLOCK(gwion->data->mutex);
  return ret;
}

static m_uint compile(struct Gwion_* gwion, struct Compiler* c) {
  VM_Shred shred = NULL;
  VM_Code code;
  compiler_name(gwion->mp, c);
  MUTEX_LOCK(gwion->data->mutex);
  CHECK_BB(compiler_open(gwion->mp, c))
  if(check(gwion, c) < 0 || !(code = emit_ast(gwion->emit, c->ast)))
    gw_err(_("while compiling file '%s'\n"), c->base);
  else {
    const VM_Shred shred = new_vm_shred(gwion->mp, code);
    shred->info->args = c->args;
    vm_add_shred(gwion->vm, shred);
  }
  MUTEX_UNLOCK(gwion->data->mutex);
  compiler_clean(gwion->mp, c);
  return shred ? shred->tick->xid : 0;
}
/*
m_bool check_filename(struct Gwion_* vm, const m_str filename) {
  struct Compiler c = { .base=filename, .type=COMPILE_NAME };
  CHECK_BB(compiler_open(gwion->mp, c))
  return check(&c, vm);
}

m_bool check_string(struct Gwion_* vm, const m_str filename, const m_str data) {
  struct Compiler c = { .base=filename, .type=COMPILE_MSTR, .data=data };
  CHECK_BB(compiler_open(gwion->mp, c))
  return check(&c, vm);
}

m_bool check_file(struct Gwion_* vm, const m_str filename, FILE* file) {
  struct Compiler c = { .base=filename, .type=COMPILE_FILE, .file = file};
  CHECK_BB(compiler_open(gwion->mp, c))
  return check(&c, vm);
}
*/
//m_uint compile_filename(struct Gwion_* vm, const m_str filename) {
m_uint compile_filename(struct Gwion_* gwion, const m_str filename) {
  struct Compiler c = { .base=filename, .type=COMPILE_NAME };
  return compile(gwion, &c);
}
/*
m_uint compile_string(struct Gwion_* vm, const m_str filename, const m_str data) {
  struct Compiler c = { .base=filename, .type=COMPILE_MSTR, .data=data };
  return compile(vm, &c);
}

m_uint compile_file(struct Gwion_* vm, const m_str filename, FILE* file) {
  struct Compiler c = { .base=filename, .type=COMPILE_MSTR, .file=file };
  return compile(vm, &c);
}
*/
