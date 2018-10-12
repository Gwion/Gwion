#include "lang.h"
#define PUSH_MEM(a, b) a->mem += b;
#define POP_MEM(a, b)  a->mem -= b;
#define PUSH_REG(a, b) a->reg += b;
#define POP_REG(a, b)  a->reg -= b;

#define MEM(a) (shred->mem + (a))
#define REG(a) (shred->reg + (a))
#define INSTR(a) __attribute__((hot))\
ANN2(1) void a(const VM_Shred shred __attribute__((unused)), const Instr instr  __attribute__((unused)))

struct Instr_ {
  void (*execute)(const VM_Shred shred, const Instr instr);
  m_uint m_val, m_val2;
  m_bit ptr[SZ_VEC4];
};

ANEW Instr new_instr(void);
ANN void free_instr(Instr instr);
INSTR(EOC);
INSTR(DTOR_EOC);
INSTR(DtorReturn);

INSTR(RegPushMe);
INSTR(RegPushNow);
INSTR(RegPushMaybe);

/* staking */
INSTR(RegPop);
INSTR(RegPushImm);
INSTR(RegPushMemAddr);
INSTR(MemPushImm);
INSTR(MemSetImm);
INSTR(RegPushMem);
INSTR(RegPushPtr);
INSTR(RegPushCode);
INSTR(RegDup);
INSTR(RegAddRef);

/* branching */
INSTR(BranchSwitch);
INSTR(BranchEqInt);
INSTR(BranchNeqInt);
INSTR(BranchEqFloat);
INSTR(BranchNeqFloat);
INSTR(InitLoopCounter);
INSTR(RegPushDeref);
INSTR(DecIntAddr);
INSTR(Goto);

/* casting */
INSTR(CastI2F);
INSTR(CastF2I);

/* debugging */
INSTR(Gack);

INSTR(RegPushStr);

INSTR(IntNot);
INSTR(FloatTimes);

INSTR(ComplexReal);
INSTR(ComplexImag);

INSTR(AllocWord);

/* function */
INSTR(SporkExp);
INSTR(SporkFunc);
INSTR(FuncUsr);
INSTR(DotFunc);
INSTR(FuncStatic);
INSTR(FuncMember);
INSTR(FuncPtr);
INSTR(FuncReturn);
INSTR(DtorReturn);

/* object */
INSTR(PreCtor);
INSTR(ObjectInstantiate);
INSTR(ObjectAssign);
INSTR(AllocMember);
INSTR(DotStatic);
INSTR(DotImport);
INSTR(DotData);
INSTR(ObjectRelease);

/* array */
INSTR(ArrayTop);
INSTR(ArrayBottom);
INSTR(ArrayPost);
INSTR(ArrayInit);
INSTR(ArrayAlloc);
INSTR(ArrayAccess);
INSTR(ArrayAccessMulti);
INSTR(ArrayAppend);

/* vararg */
INSTR(VarargIni);
INSTR(VarargTop);
INSTR(VarargEnd);
INSTR(VarargMember);

INSTR(MemberFunction);
INSTR(VecMember);
INSTR(PopArrayClass);

INSTR(FuncOp);

INSTR(GcIni);
INSTR(GcEnd);
INSTR(GcAdd);

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
INSTR(JitExec);
#endif

#ifdef JIT_DEV
#undef INSTR
#define INSTR(a) __attribute__((hot))\
ANN2(1) void a(const VM_Shred shred __attribute__((unused)), const Instr instr  __attribute__((unused)));\
__attribute__((constructor(800))) void jit_dev_##a() { jit_dev(a, #a, __FILE__, __LINE__); } \
ANN2(1) void a(const VM_Shred shred __attribute__((unused)), const Instr instr  __attribute__((unused)))
void jit_dev(f_instr f, m_str str, const m_str __file__, const m_uint line);
#endif

//#endif
