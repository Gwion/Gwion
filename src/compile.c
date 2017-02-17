#include "err_msg.h"
#include "vm.h"
#include "emit.h"
#include "instr.h"

m_bool compile(VM* vm, const m_str filename)
{
  m_bool ret = 1;
  VM_Shred shred;
  VM_Code  code;
  Ast      ast;
  Vector args = NULL;
  m_str _name, name, d = strdup(filename);
  _name = strsep(&d, ":");
  if(d)
    args = new_Vector();
  while(d)
    vector_append(args, (vtype)S_name(insert_symbol(strsep(&d, ":"))));
  free(d);
  name = realpath(_name, NULL);
  free(_name);
  if(!name) {
    err_msg(COMPILE_, 0, "error while opening file '%s'", filename);
    return -1;
  }
#ifdef DEBUG_COMPILE
  debug_msg("parser", "get full path ok %s", name);
#endif
//    CHECK_OB((ast = parse(name)))
  if(!(ast = parse(name))) {
    ret = -1;
    goto clean;
  }
#ifdef DEBUG_COMPILE
  debug_msg("lexer", "Ast of '%s' ok", name);
#endif
  CHECK_BB(type_engine_check_prog(vm->env, ast, name))
#ifdef DEBUG_COMPILE
  debug_msg("lexer", "type check  of '%s' ok", name);
#endif
  CHECK_BB(emit_Ast(vm->emit, ast, name))
#ifdef DEBUG_COMPILE
  debug_msg("lexer", "emit   of '%s' ok", name);
#endif
  add_instr(vm->emit, EOC);
  vm->emit->code->name = strdup(name);
  vm->emit->code->filename = strdup(name);
  code = emit_to_code(vm->emit);
  free_Ast(ast);
  shred = new_VM_Shred(code);
  shred->args = args;
  shred->me = new_Shred(vm, shred);
  vm_add_shred(vm, shred);
clean:
  free(name);
  return ret;
}
