#include <stdlib.h>
#include "type.h"
#include "instr.h"
#include "import.h"
#include "emit.h"

#include "func.h"


static struct Type_ t_callback = { "Callback", SZ_INT, &t_object };

struct ret_info {
  Instr instr;
  m_uint offset;
  m_uint size;
};

static INSTR(my_ret) {
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
  
  shred->next_pc = instr->m_val2;
  free(info);
  free(instr);
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
  Instr instr = malloc(sizeof(struct Instr_));
  struct ret_info* info = malloc(sizeof(struct ret_info));
  info->offset = offset; 
  info->size = f->def->ret_type->size; 
  instr->execute = my_ret;
  *(VM_Code*)instr->ptr = shred->code;
  instr->m_val = (m_uint)info;
  instr->m_val2 = shred->pc + 1;
  /*instr->m_val2 = shred->pc;*/
  /*instr->m_val2 = shred->next_pc;*/
  for(i = 0; i < vector_size(f->code->instr); i++) {
    Instr in = (Instr)vector_at(f->code->instr, i);
    if(in->execute == Func_Return ||
      in->execute == my_ret) {
      info->instr = in; 
      vector_set(f->code->instr, i, (vtype)instr);
    }
  }
  *(m_int*)RETURN = 1;
  shred->next_pc = 0;
  shred->code = f->code;
}

IMPORT {
  CHECK_BB(importer_fptr_ini(importer, "Vec4", "PtrType"))
  CHECK_BB(importer_fptr_end(importer, 0))
  
  CHECK_BB(importer_class_ini(importer, &t_callback, NULL, NULL))
    CHECK_BB(importer_func_ini(importer, "int", "callback", (m_uint)cb_func))
      CHECK_BB(importer_func_arg(importer, "PtrType", "func"))
      /*CHECK_BB(importer_func_arg(importer, "int", "unused"))*/
    CHECK_BB(importer_func_end(importer, ae_flag_static))
  CHECK_BB(importer_class_end(importer))
  return 1;
}
