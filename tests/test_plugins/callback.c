#include <stdlib.h>
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "import.h"
#include "func.h"

struct ret_info {
  Instr instr;
  m_uint offset;
  m_uint size;
};

static INSTR(my_ret) { GWDEBUG_EXE
  struct ret_info* info = (struct ret_info*)instr->m_val;
  POP_MEM(shred, info->offset);
        vector_set(shred->code->instr, shred->pc, (vtype)info->instr);
  shred->code = *(VM_Code*)instr->ptr;
  /*if(shred->mem == shred->_mem)*/
  POP_REG(shred, info->size)
  /*POP_REG(shred, shred->code->stack_depth)*/
  /*else*/
    if(shred->mem == shred->_mem)
    POP_REG(shred, SZ_INT)
  POP_REG(shred, shred->code->stack_depth);
  
  shred->pc = instr->m_val2;
  free(info);
  free_instr(instr);
  *(m_int*)shred->reg = 2;
  PUSH_REG(shred, SZ_INT);
  /*POP_REG(shred, shred->code->stack_depth);*/
}

static SFUN(cb_func) {
  m_uint i;
  Func f = *(Func*)(shred->mem + SZ_INT);
  if(!f){
    Except(shred, "NullCallbackException");
  }
  m_uint offset = shred->mem -shred->_mem;
  PUSH_MEM(shred, offset);
  Instr instr = new_instr();
  struct ret_info* info = (struct ret_info*)xmalloc(sizeof(struct ret_info));
  info->offset = offset;
  info->size = f->def->ret_type->size;
  instr->execute = my_ret;
  *(VM_Code*)instr->ptr = shred->code;
  instr->m_val = (m_uint)info;
//  instr->m_val2 = shred->pc + 1;
  instr->m_val2 = shred->pc;
  for(i = 0; i < vector_size(f->code->instr); i++) {
    Instr in = (Instr)vector_at(f->code->instr, i);
    if(in->execute == Func_Return ||
      in->execute == my_ret) {
      info->instr = in;
      vector_set(f->code->instr, i, (vtype)instr);
    }
  }
  *(m_int*)RETURN = 1;
  shred->pc = 0;
  shred->code = f->code;
}

IMPORT {
  Type t_callback;
  CHECK_BB(gwi_fptr_ini(gwi, "Vec4", "PtrType"))
  CHECK_BB(gwi_fptr_end(gwi, 0))

  CHECK_OB((t_callback = gwi_mk_type(gwi, "Callback", SZ_INT, t_object)))
  CHECK_BB(gwi_class_ini(gwi, t_callback, NULL, NULL))
    CHECK_BB(gwi_func_ini(gwi, "int", "callback", cb_func))
      CHECK_BB(gwi_func_arg(gwi, "PtrType", "func"))
      /*CHECK_BB(gwi_func_arg(gwi, "int", "unused"))*/
    CHECK_BB(gwi_func_end(gwi, ae_flag_static))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
