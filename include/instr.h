#include <stdio.h>
#include <string.h>

#include "dl.h"
#include "func.h"
#include "lang.h"
#include "shreduler.h"
#include "bbq.h"
#include "object.h"


struct Instr_ {
  void (*execute)(VM* vm, VM_Shred shred, Instr instr);
  m_uint m_val, m_val2;
  m_float f_val;
  m_complex c_val;
  void* ptr;
};

Instr new_Instr();
void EOC(VM * vm, VM_Shred shred, Instr instr);

void Reg_Push_Me(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Now(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Maybe(VM* vm, VM_Shred shred, Instr instr);

/* stacking */
void Reg_Pop_Word4( VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Imm(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Mem_Addr(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_Imm2(VM* vm, VM_Shred shred, Instr instr);
void Reg_Push_ImmC(VM* vm, VM_Shred shred, Instr instr);
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
void Init_Loop_Counter(VM * vm, VM_Shred shred, Instr instr);
void Free_Loop_Counter(VM * vm, VM_Shred shred, Instr instr);
void Reg_Push_Deref(VM * vm, VM_Shred shred, Instr instr);
void Dec_int_Addr(VM * vm, VM_Shred shred, Instr instr);
void Goto(VM * vm, VM_Shred shred, Instr instr);

/* casting */
void Cast_i2f(VM * vm, VM_Shred shred, Instr instr);
void Cast_f2i(VM * vm, VM_Shred shred, Instr instr);

/* debugging */
void Gack(VM* vm, VM_Shred shred, Instr instr);

void Reg_Push_Str(VM* vm, VM_Shred shred, Instr instr);
void Time_Advance(VM* vm, VM_Shred shred, Instr instr);
void Event_Wait(VM* vm, VM_Shred shred, Instr instr);

void negate(VM * vm, VM_Shred shred, Instr instr);
void not(VM * vm, VM_Shred shred, Instr instr);
//void PostInc_Int(VM * vm, VM_Shred shred, Instr instr);
//void PostDec_Int(VM * vm, VM_Shred shred, Instr instr);
//void PreInc_Int(VM * vm, VM_Shred shred, Instr instr);
//void PreDec_Int(VM * vm, VM_Shred shred, Instr instr);
void inc(VM * vm, VM_Shred shred, Instr instr);
void dec(VM * vm, VM_Shred shred, Instr instr);

void timesf(VM * vm, VM_Shred shred, Instr instr);
void notf(VM * vm, VM_Shred shred, Instr instr);
void negatef(VM * vm, VM_Shred shred, Instr instr);

void complex_real(VM * vm, VM_Shred shred, Instr instr);
void complex_imag(VM * vm, VM_Shred shred, Instr instr);

/* alloc (instr.c)*/
void Alloc_Word(VM* vm, VM_Shred shred, Instr instr);
void Alloc_Word_Float(VM* vm, VM_Shred shred, Instr instr);
void Alloc_Word_Complex(VM* vm, VM_Shred shred, Instr instr);
void Alloc_Word_Vec3(VM* vm, VM_Shred shred, Instr instr);
void Alloc_Word_Vec4(VM* vm, VM_Shred shred, Instr instr);

/* function */
void Spork(VM * vm, VM_Shred shred, Instr instr);
void Instr_Func_Call(VM * vm, VM_Shred shred, Instr instr);
void Dot_Static_Func(VM * vm, VM_Shred shred, Instr instr);
void Dot_Member_Func(VM * vm, VM_Shred shred, Instr instr);
void Instr_Func_Call_Static(VM * vm, VM_Shred shred, Instr instr);
void Instr_Func_Call_Member(VM * vm, VM_Shred shred, Instr instr);
void Func_Return(VM * vm, VM_Shred shred, Instr instr);

/* object */
void Reg_Push_This(VM * vm, VM_Shred shred, Instr instr);
void Pre_Constructor(VM * vm, VM_Shred shred, Instr instr);
void Instantiate_Object(VM * vm, VM_Shred shred, Instr instr);
void Assign_Object(VM * vm, VM_Shred shred, Instr instr);
void Alloc_Member_Word(VM * vm, VM_Shred shred, Instr instr);
void Alloc_Member_Word_Float(VM * vm, VM_Shred shred, Instr instr);
void Alloc_Member_Word_Complex(VM * vm, VM_Shred shred, Instr instr);
void Alloc_Member_Word_Vec3(VM* vm, VM_Shred shred, Instr instr);
void Alloc_Member_Word_Vec4(VM* vm, VM_Shred shred, Instr instr);
//void Alloc_Dot_Static_Data(VM * vm, VM_Shred shred, Instr instr);
void Dot_Static_Data(VM * vm, VM_Shred shred, Instr instr);
void Dot_Static_Import_Data(VM * vm, VM_Shred shred, Instr instr);
void Dot_Member_Data(VM * vm, VM_Shred shred, Instr instr);
void Release_Object2(VM* vm, VM_Shred shred, Instr instr);

/* array */
void Instr_Pre_Ctor_Array_Top(VM * vm, VM_Shred shred, Instr instr);
void Instr_Pre_Ctor_Array_Bottom(VM * vm, VM_Shred shred, Instr instr);
void Instr_Pre_Ctor_Array_Post(VM * vm, VM_Shred shred, Instr instr);
void Instr_Array_Init(VM* vm, VM_Shred shred, Instr instr);
void Instr_Array_Alloc(VM* vm, VM_Shred shred, Instr instr);
void Instr_Array_Access(VM* vm, VM_Shred shred, Instr instr);
void Instr_Array_Access_Multi(VM* vm, VM_Shred shred, Instr instr);

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

INSTR(Mem_Push_Ret); // test on 13/01/17

INSTR(Instr_Op_Call_Binary);
