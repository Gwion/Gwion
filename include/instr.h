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

void Reg_Push_Me(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Now(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Maybe(VM* vm, VM_Shred shred, Instr instr);

/* stacking */
void Reg_Pop_Word4(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Imm(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Mem_Addr(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Imm2(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_ImmX(VM* vm, VM_Shred shred, Instr instr);
void Mem_Push_Imm(VM* vm, VM_Shred shred, Instr instr);
void Mem_Set_Imm(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Mem(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Mem2(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Mem_Complex(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Mem_Vec3(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Mem_Vec4(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Ptr(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Code(VM* vm, VM_Shred shred, Instr instr);
void Reg_Dup_Last(VM* vm, VM_Shred shred, Instr instr);
void Reg_AddRef_Object3(VM* vm, VM_Shred shred, Instr instr);

/* branching */
void Branch_Switch(VM* vm, VM_Shred shred, Instr instr);
void Branch_Eq_Int(VM* vm, VM_Shred shred, Instr instr);
void Branch_Neq_Int(VM* vm, VM_Shred shred, Instr instr);
void Branch_Eq_Float(VM* vm, VM_Shred shred, Instr instr);
void Branch_Neq_Float(VM* vm, VM_Shred shred, Instr instr);
INSTR(Init_Loop_Counter);
INSTR(Reg_Push_Deref);
INSTR(Dec_int_Addr);
INSTR(Goto);

/* casting */
INSTR(Cast_i2f);
INSTR(Cast_f2i);

/* debugging */
void Gack(VM* vm, VM_Shred shred, Instr instr);

void Reg_Push_Str(VM* vm, VM_Shred shred, Instr instr);
void Time_Advance(VM* vm, VM_Shred shred, Instr instr);

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
void Alloc_Member_Word_Vec3(VM* vm, VM_Shred shred, Instr instr);
void Alloc_Member_Word_Vec4(VM* vm, VM_Shred shred, Instr instr);
//INSTR(Alloc_Dot_Static_Data);
INSTR(Dot_Static_Data);
INSTR(Dot_Static_Import_Data);
INSTR(Exp_Dot_Data);
void Release_Object2(VM* vm, VM_Shred shred, Instr instr);

/* array */
INSTR(Instr_Pre_Ctor_Array_Top);
INSTR(Instr_Pre_Ctor_Array_Bottom);
INSTR(Instr_Pre_Ctor_Array_Post);
void Instr_Array_Init(VM* vm, VM_Shred shred, Instr instr);
void Instr_Array_Alloc(VM* vm, VM_Shred shred, Instr instr);
void Instr_Array_Access(VM* vm, VM_Shred shred, Instr instr);
void Instr_Array_Access_Multi(VM* vm, VM_Shred shred, Instr instr);
extern INSTR(Array_Append);
//void member_function(VM* vm, VM_Shred shred, Instr instr);
void assign_func(VM* vm, VM_Shred shred, Instr instr);
INSTR(MkVararg);
INSTR(Vararg_start);
INSTR(Vararg_end);
INSTR(Vararg_int);
INSTR(Vararg_float);
INSTR(Vararg_complex);
INSTR(Vararg_object);
INSTR(Vararg_Vec3);
INSTR(Vararg_Vec4);
INSTR(Free_Func); // for template

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
