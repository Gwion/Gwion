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

static void compiler_name(struct Compiler* c) {
  m_str d = strdup(c->base);
  c->name = strsep(&d, ":");
  if(d)
    c->args = new_vector();
  while(d)
    vector_add(c->args, (vtype)strdup(strsep(&d, ":")));
  free(d);
}

static void compiler_clean(const struct Compiler* c) {
  if(c->name)
    free(c->name);
  if(c->file)
    fclose(c->file);
  if(c->ast)
    free_ast(c->ast);
}

static m_bool compiler_open(struct Compiler* c) {
  if(c->type == COMPILE_NAME) {
    m_str name = c->name;
    c->name = realpath(name, NULL);
    free(name);
    return c->name ? !!(c->file = fopen(c->name, "r")) : -1;
  } else if(c->type == COMPILE_MSTR)
    return (c->file = fmemopen(c->data, strlen(c->data), "r")) ? 1 : - 1;
  return GW_OK;
}

static m_bool check(struct Gwion_* gwion, struct Compiler* c) {
  CHECK_BB(compiler_open(c))
  CHECK_OB((c->ast = parse(c->name, c->file)))
  gwion->env->name = c->name;
  return type_engine_check_prog(gwion->env, c->ast);
}

static m_uint compile(struct Gwion_* gwion, struct Compiler* c) {
  m_uint xid = 0;
  VM_Code code;
  compiler_name(c);
  if(check(gwion, c) < 0 ||
     !(code = emit_ast(gwion->emit, c->ast)))
     gw_err("while compiling file '%s'\n", c->base);
  else {
    const VM_Shred shred = new_vm_shred(code);
    shred->info->args = c->args;
    xid = vm_add_shred(gwion->vm, shred);
  }
  compiler_clean(c);
  return xid;
}
/*
m_bool check_filename(struct Gwion_* vm, const m_str filename) {
  struct Compiler c = { .base=filename, .type=COMPILE_NAME };
  return check(&c, vm);
}

m_bool check_string(struct Gwion_* vm, const m_str filename, const m_str data) {
  struct Compiler c = { .base=filename, .type=COMPILE_MSTR, .data=data };
  return check(&c, vm);
}

m_bool check_file(struct Gwion_* vm, const m_str filename, FILE* file) {
  struct Compiler c = { .base=filename, .type=COMPILE_FILE, .file = file};
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
