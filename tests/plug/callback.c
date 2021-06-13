#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

struct ret_info {
  Instr   instr;
  VM_Code code;
  m_uint  offset;
  m_uint  size;
  size_t  pc;
};

static INSTR(my_ret) {
  struct ret_info *info = (struct ret_info *)instr->m_val;
  POP_MEM(shred, info->offset);
  vector_set(&shred->code->instr, shred->pc, (vtype)info->instr);
  shred->code = info->code;
  //*(VM_Code*)instr->ptr;
  POP_REG(shred, info->size)
  if (shred->mem == (m_bit *)shred + sizeof(struct VM_Shred_) + SIZEOF_REG)
    POP_REG(shred, SZ_INT)
  POP_REG(shred, shred->code->stack_depth);
  //  shred->pc = instr->m_val2;
  shred->pc = info->pc;
  free(info);
  *(m_int *)shred->reg = 2;
  PUSH_REG(shred, SZ_INT);
}

static SFUN(cb_func) {
  m_uint i;
  Func   f = *(Func *)MEM(0);
  if (!f) { handle(shred, "NullCallbackException"); }
  m_uint offset =
      shred->mem - ((m_bit *)shred + sizeof(struct VM_Shred_) + SIZEOF_REG);
  PUSH_MEM(shred, offset);
  Instr            instr = mp_calloc(shred->info->mp, Instr);
  struct ret_info *info  = (struct ret_info *)xmalloc(sizeof(struct ret_info));
  info->offset           = offset;
  info->code             = shred->code;
  info->size             = f->def->base->ret_type->size;
  info->pc               = shred->pc;
  instr->execute         = my_ret;
  //  *(VM_Code*)instr->ptr = shred->code;
  instr->m_val = (m_uint)info;
  //  instr->m_val2 = shred->pc;
  for (i = 0; i < vector_size(&f->code->instr); i++) {
    Instr in = (Instr)vector_at(&f->code->instr, i);
    if (in->execute == FuncReturn || in->execute == my_ret) {
      info->instr = in;
      vector_set(&f->code->instr, i, (vtype)instr);
    }
  }
  *(m_int *)RETURN = 1;
  shred->pc        = 0;
  shred->code      = f->code;
}

GWION_IMPORT(callback) {
  GWI_BB(gwi_fptr_ini(gwi, "Vec4", "PtrType"))
  GWI_OB(gwi_fptr_end(gwi, 0))

  GWI_OB(gwi_class_ini(gwi, "Callback", NULL))
  GWI_BB(gwi_func_ini(gwi, "int", "callback"))
  GWI_BB(gwi_func_arg(gwi, "PtrType", "func"))
  GWI_BB(gwi_func_end(gwi, cb_func, ae_flag_static))
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
