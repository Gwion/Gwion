#include "lang.h"

#define PUSH_MEM(a, b) a->mem += b;
#define POP_MEM(a, b)  a->mem -= b;
#define PUSH_REG(a, b) a->reg += b;
#define POP_REG(a, b)  a->reg -= b;

#define MEM(a) (shred->mem + (a))
#define REG(a) (shred->reg + (a))
#define INSTR(a) __attribute__((hot))\
ANN2(1) void a(const VM_Shred shred __attribute__((unused)), const Instr instr  __attribute__((unused)))
#define MEM_STEP 16
#define SIZEOF_MEM (0x1 << MEM_STEP)
#define SIZEOF_REG (0x1 << 14)

struct Instr_ {
  void (*execute)(const VM_Shred shred, const Instr instr);
  m_uint m_val, m_val2;
  m_bit ptr[SZ_VEC4];
};

ANEW Instr new_instr(void);
ANN void free_instr(Instr instr);
INSTR(EOC);
INSTR(DTOR_EOC);
INSTR(Dtor_Return);


#ifdef GWREPL
INSTR(EOC2); // repl
#endif

INSTR(Reg_Push_Me);
INSTR(Reg_Push_Now);
INSTR(Reg_Push_Maybe);

/* staking */
INSTR(Reg_Pop_Word);
INSTR(Reg_Push_Imm);
INSTR(Reg_Push_Mem_Addr);
INSTR(Mem_Push_Imm);
INSTR(Mem_Set_Imm);
INSTR(Reg_Push_Mem);
INSTR(Reg_Push_Ptr);
INSTR(Reg_Push_Code);
INSTR(Reg_Dup_Last);
INSTR(Reg_Add_Ref);

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

INSTR(int_not);
INSTR(float_times);

INSTR(complex_real);
INSTR(complex_imag);

INSTR(Alloc_Word);

/* function */
INSTR(Spork);
INSTR(Spork_Func);
INSTR(Func_Usr);
INSTR(Exp_Dot_Func);
INSTR(Func_Static);
INSTR(Func_Member);
INSTR(Func_Ptr);
INSTR(Func_Return);
INSTR(Dtor_Return);

/* object */
INSTR(Pre_Constructor);
INSTR(Instantiate_Object);
INSTR(Assign_Object);
INSTR(Alloc_Member);
INSTR(Dot_Static_Data);
INSTR(Dot_Static_Import_Data);
INSTR(Exp_Dot_Data);
INSTR(Release_Object);

/* array */
INSTR(Instr_Pre_Ctor_Array_Top);
INSTR(Instr_Pre_Ctor_Array_Bottom);
INSTR(Instr_Pre_Ctor_Array_Post);
INSTR(Instr_Array_Init);
INSTR(Instr_Array_Alloc);
INSTR(Instr_Array_Access);
INSTR(Instr_Array_Access_Multi);
INSTR(Array_Append);

/* vararg */
INSTR(MkVararg);
INSTR(Vararg_start);
INSTR(Vararg_end);
INSTR(Vararg_Member);

INSTR(member_function);
INSTR(vec_member);
INSTR(pop_array_class);

INSTR(Call_Binary);

INSTR(start_gc);
INSTR(stop_gc);
INSTR(add2gc);

INSTR(AutoLoopStart);
INSTR(AutoLoopEnd);

#ifdef GWCOV
INSTR(InstrCoverage);
#endif

// optimizations
#ifdef OPTIMIZE
INSTR(PutArgsInMem);
INSTR(ConstPropSet);
INSTR(ConstPropGet);
#endif

#ifdef JIT
#ifdef JIT_DEV
#undef INSTR
#define INSTR(a)  \
void a(const VM_Shred, const Instr); \
__attribute__((constructor(400))) void print_##a() { printf("%s %p\n", __func__, (void*)(m_uint)a); } \
__attribute__((hot))\
ANN2(1) void a(const VM_Shred shred __attribute__((unused)), const Instr instr  __attribute__((unused)))
#endif
#endif
