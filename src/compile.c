#include "err_msg.h"
#include "vm.h"
#include "emit.h"
#include "instr.h"

m_bool compile(VM* vm, const m_str filename)
{
    VM_Shred shred;
    VM_Code  code;
    Ast      ast;
    Vector args = NULL;
    m_str name, d = strdup(filename);
    name = strsep(&d, ":");
    if(d)
        args = new_Vector();
    while(d)
        vector_append(args, (vtype)strsep(&d, ":"));
    free(d);
    name = realpath(name, NULL);
    if(!name)
    {
        err_msg(COMPILE_, 0, "error while opening file '%s'", filename);
        return -1;
    }
#ifdef DEBUG_COMPILE
    debug_msg("parser", "get full path ok %s", name);
#endif
    CHECK_OB((ast = parse(name)))
#ifdef DEBUG_COMPILE
    debug_msg("lexer", "Ast of '%s' ok", name);
#endif
    CHECK_BB(type_engine_check_prog(vm->env, ast, name))
#ifdef DEBUG_COMPILE
    debug_msg("lexer", "type check  of '%s' ok", name);
#endif
    CHECK_BB(emit_Ast(vm->emit, ast))
#ifdef DEBUG_COMPILE
    debug_msg("lexer", "emit   of '%s' ok", name);
#endif
    free_Ast(ast);
    add_instr(vm->emit, EOC);
    vm->emit->code->name = name;
    code = emit_to_code(vm->emit);
    shred = new_VM_Shred(code);
    shred->args = args;
    shred->me = new_Shred(vm, shred);
    shred->code->filename = name;
    vm_add_shred(vm, shred);
#ifdef DEBUG_COMPILE
    debug_msg("lexer", "lauching shred [%p] : %i (%p)", shred, shred->xid, shred->args);
#endif
    return 1;
}
