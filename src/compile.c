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
#include "type.h"
#include "emit.h"
#include "hash.h"
#include "scanner.h"
#include "compile.h"
#include "instr.h"

enum compile_type {
  COMPILE_FILE,
  COMPILE_MSTR
};

struct Compiler {
  m_str  name;
  FILE*  file;
  Ast    ast;
  Vector args;
  enum compile_type type;
};

static void compiler_name(struct Compiler* compiler, const m_str filename) {
  m_str _name, d = strdup(filename);
  _name = strsep(&d, ":");
  if(d)
    compiler->args = new_vector();
  while(d)
    vector_add(compiler->args, (vtype)strdup(strsep(&d, ":")));
  free(d);
  if(compiler->type == COMPILE_FILE) {
    compiler->name = realpath(_name, NULL);
    free(_name);
  } else
    compiler->name = _name;
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

m_uint compile(VM* vm, const m_str filename) {
  struct Compiler compiler = { .type=COMPILE_FILE };
  compiler_name(&compiler, filename);
  if(!compiler.name) {
    err_msg(0, "error while opening file '%s'", filename);
    return 0;
  }
  m_uint xid = 0;
  if(!(compiler.file = fopen(compiler.name, "r")) ||
     !(compiler.ast = parse(vm->scan, compiler.name, compiler.file)) ||
     type_engine_check_prog(vm->emit->env, compiler.ast, compiler.name) < 0 ||
     emit_ast(vm->emit, compiler.ast, compiler.name) < 0)
     gw_err("in file '%s'\n", filename);
  else
    xid = finalize(vm, compiler.args);
  compiler_clean(&compiler);
  return xid;
}
