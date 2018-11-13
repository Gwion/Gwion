#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "map.h"
#include "defs.h"
#include "absyn.h"
#include "vm.h"
#include "env.h"
#include "instr.h"
#include "type.h"
#include "emit.h"
#include "hash.h"
#include "scanner.h"
#include "compile.h"

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

static m_uint finalize(const VM* vm, const Vector args) {
  emitter_add_instr(vm->emit, EOC);
  const VM_Code code = emit_code(vm->emit);
  const VM_Shred shred = new_vm_shred(code);
  shred->args = args;
  return vm_add_shred(vm, shred);
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
  return 1;
}

static m_bool check(struct Compiler* c, VM* vm) {
  CHECK_BB(compiler_open(c))
  CHECK_OB((c->ast = parse(vm->scan, c->name, c->file)))
  vm->emit->env->name = c->name;
  return type_engine_check_prog(vm->emit->env, c->ast);
}

static m_uint compile(VM* vm, struct Compiler* c) {
  m_uint xid = 0;
  compiler_name(c);
  if(check(c, vm) < 0 ||
     emit_ast(vm->emit, c->ast) < 0)
     gw_err("while compiling file '%s'\n", c->base);
  else
    xid = finalize(vm, c->args);
  compiler_clean(c);
  return xid;
}
/*
m_bool check_filename(VM* vm, const m_str filename) {
  struct Compiler c = { .base=filename, .type=COMPILE_NAME };
  return check(&c, vm);
}

m_bool check_string(VM* vm, const m_str filename, const m_str data) {
  struct Compiler c = { .base=filename, .type=COMPILE_MSTR, .data=data };
  return check(&c, vm);
}

m_bool check_file(VM* vm, const m_str filename, FILE* file) {
  struct Compiler c = { .base=filename, .type=COMPILE_FILE, .file = file};
  return check(&c, vm);
}
*/
m_uint compile_filename(VM* vm, const m_str filename) {
  struct Compiler c = { .base=filename, .type=COMPILE_NAME };
  return compile(vm, &c);
}
/*
m_uint compile_string(VM* vm, const m_str filename, const m_str data) {
  struct Compiler c = { .base=filename, .type=COMPILE_MSTR, .data=data };
  return compile(vm, &c);
}

m_uint compile_file(VM* vm, const m_str filename, FILE* file) {
  struct Compiler c = { .base=filename, .type=COMPILE_MSTR, .file=file };
  return compile(vm, &c);
}
*/
