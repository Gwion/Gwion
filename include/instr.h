#include <stdio.h>
#include <string.h>
#include "lang.h"

#define PUSH_MEM(a, b) a->mem += b;
#define POP_MEM(a, b)  a->mem -= b;
#define PUSH_REG(a, b) a->reg += b;
#define POP_REG(a, b)  a->reg -= b;

#define MEM(a) (shred->mem + (a))
#define REG(a) (shred->reg + (a))
#define INSTR(a) void a(VM* vm, VM_Shred shred, Instr instr)
#define MEM_STEP 16
#define SIZEOF_MEM 0x1 << MEM_STEP
#define SIZEOF_REG 0x1 << 14

struct Instr_ {
  void (*execute)(VM* vm, VM_Shred shred, Instr instr);
  m_uint m_val, m_val2;
  char ptr[SZ_INT];
};

INSTR(EOC);

INSTR(Reg_Push_Me);
INSTR(Reg_Push_Now);
INSTR(Reg_Push_Maybe);

/* staking */
INSTR(Reg_Pop_Word4);
INSTR(Reg_PushImm);
INSTR(Reg_Push_Mem_Addr);
INSTR(Mem_Push_Imm);
INSTR(Mem_Set_Imm);
INSTR(Reg_Push_Mem);
INSTR(Reg_Push_Ptr);
INSTR(Reg_Push_Code);
INSTR(Reg_Dup_Last);
INSTR(Reg_AddRef_Object3);

/* branching */
INSTR(Branch_Switch);
INSTR(Branch_Eq_Int);
INSTR(Branch_Neq_Int);
INSTR(Branch_Eq_Float);
INSTR(Branch_Neq_Float);
INSTR(Init_Loop_Counter);
INSTR(Reg_Push_Deref);
INSTR(Dec_int_Addr);
INSTR(Goto);

/* casting */
INSTR(Cast_i2f);
INSTR(Cast_f2i);

/* debugging */
INSTR(Gack);

INSTR(Reg_Push_Str);
INSTR(Time_Advance);

INSTR(int_not);
INSTR(float_times);

INSTR(complex_real);
INSTR(complex_imag);

INSTR(Alloc_Word);

/* function */
INSTR(Spork);
INSTR(Instr_Exp_Func);
INSTR(Dot_Static_Func);
INSTR(Exp_Dot_Func);
INSTR(Instr_Exp_Func_Static);
INSTR(Instr_Exp_Func_Member);
INSTR(Func_Return);

/* object */
INSTR(Reg_Push_This);
INSTR(Pre_Constructor);
INSTR(Instantiate_Object);
INSTR(Assign_Object);
INSTR(Alloc_Member);
INSTR(Dot_Static_Data);
INSTR(Dot_Static_Import_Data);
INSTR(Exp_Dot_Data);
INSTR(Release_Object2);

/* array */
INSTR(Instr_Pre_Ctor_Array_Top);
INSTR(Instr_Pre_Ctor_Array_Bottom);
INSTR(Instr_Pre_Ctor_Array_Post);
INSTR(Instr_Array_Init);
INSTR(Instr_Array_Alloc);
INSTR(Instr_Array_Access);
INSTR(Instr_Array_Access_Multi);
INSTR(Array_Append);
INSTR(assign_func);

/* vararg */
INSTR(MkVararg);
INSTR(Vararg_start);
INSTR(Vararg_end);
INSTR(Vararg_Member);

INSTR(member_function);
INSTR(vec_member);

INSTR(Instr_Op_Call_Binary);

INSTR(start_gc);
INSTR(stop_gc);
INSTR(add2gc);

INSTR(AutoLoop);
INSTR(Cast2Ptr);

#ifdef GWCOV
INSTR(InstrCoverage);
#endif
