#include <stdio.h>
#include <string.h>
#include "dl.h"
#include "bbq.h"
#include "lang.h"

#ifdef DEBUG_STACK
#define PUSH_MEM(a, b) \
{\
  a->mem_index += b; \
  a->mem += b;\
}
#define POP_MEM(a, b) \
{\
  a->mem_index -= b; \
  a->mem -= b;\
}
#define PUSH_REG(a, b) \
{\
  a->reg_index += b; \
  a->reg += b;\
}
#define POP_REG(a, b) \
{\
  a->reg_index -= b; \
  a->reg -= b;\
}

#else
#define PUSH_MEM(a, b) a->mem += b;
#define POP_MEM(a, b)  a->mem -= b;
#define PUSH_REG(a, b) a->reg += b;
#define POP_REG(a, b)  a->reg -= b;
#endif

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

Instr add_instr(Emitter emit, f_instr f);

INSTR(EOC);

INSTR(Reg_Push_Me);
INSTR(Reg_Push_Now);
INSTR(Reg_Push_Maybe);

/* staking */
INSTR(Reg_Pop_Word4);
INSTR(Reg_Push_Imm);
INSTR(Reg_Push_Mem_Addr);
INSTR(Reg_Push_Imm2);
INSTR(Reg_Push_ImmX);
INSTR(Mem_Push_Imm);
INSTR(Mem_Set_Imm);
INSTR(Reg_Push_Mem);
INSTR(Reg_Push_Mem2);
INSTR(Reg_Push_Mem_Complex);
INSTR(Reg_Push_Mem_Vec3);
INSTR(Reg_Push_Mem_Vec4);
INSTR(Reg_Push_Ptr);
INSTR(Reg_Push_Code);
INSTR(Reg_Dup_Last);
INSTR(Reg_AddRef_Object3);

/* braching */
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
INSTR(Alloc_Word_Float);
INSTR(Alloc_Word_Complex);
INSTR(Alloc_Word_Vec3);
INSTR(Alloc_Word_Vec4);

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
INSTR(Alloc_Member_Word);
INSTR(Alloc_Member_Word_Float);
INSTR(Alloc_Member_Word_Complex);
INSTR(Alloc_Member_Word_Vec3);
INSTR(Alloc_Member_Word_Vec4);
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
INSTR(MkVararg);
INSTR(Vararg_start);
INSTR(Vararg_end);
INSTR(Vararg_int);
INSTR(Vararg_float);
INSTR(Vararg_complex);
INSTR(Vararg_object);
INSTR(Vararg_Vec3);
INSTR(Vararg_Vec4);

INSTR(member_function);
INSTR(vec3_x);
INSTR(vec3_y);
INSTR(vec3_z);
INSTR(vec4_x);
INSTR(vec4_y);
INSTR(vec4_z);
INSTR(vec4_w);
INSTR(Reg_Dup_Last_Vec3);
INSTR(Reg_Dup_Last_Vec4);

//INSTR(Mem_Push_Ret); // test on 13/01/17 // removed 09/04/17

INSTR(Instr_Op_Call_Binary);

INSTR(start_gc);
INSTR(stop_gc);
//INSTR(add_gc);
