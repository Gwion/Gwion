#ifndef __INSTR
#define __INSTR
#define PUSH_MEM(a, b) a->mem += b;
#define POP_MEM(a, b)  a->mem -= b;
#define PUSH_REG(a, b) a->reg += b;
#define POP_REG(a, b)  a->reg -= b;

#define MEM(a) (shred->mem + (a))
#define REG(a) (shred->reg + (a))
#define INSTR(a) __attribute__((hot))\
ANN2(1) void a(const VM_Shred shred __attribute__((unused)), const Instr instr  __attribute__((unused)))

enum Kind {
  KIND_INT,
  KIND_FLOAT,
  KIND_OTHER,
  KIND_ADDR
};

typedef struct Instr_     * Instr;
typedef void (*f_instr)(const VM_Shred, const Instr);
struct Instr_ {
  m_bit opcode;
  union {
    m_float f;
    m_uint m_val;
  };
  m_uint m_val2;
  void (*execute)(const VM_Shred shred, const Instr instr);
};

INSTR(EOC);
INSTR(DTOR_EOC);
INSTR(DtorReturn);

/* branching */
INSTR(BranchSwitch);
INSTR(SwitchIni);
INSTR(SwitchEnd);

INSTR(ComplexReal);
INSTR(ComplexImag);

/* function */
INSTR(DtorReturn);

/* array */
INSTR(ArrayBottom);
INSTR(ArrayPost);
INSTR(ArrayInit);
INSTR(ArrayAlloc);
INSTR(ArrayAccess);
INSTR(ArrayAccessMulti);

/* vararg */
INSTR(VarargIni);
INSTR(VarargTop);
INSTR(VarargEnd);
INSTR(VarargMember);

INSTR(VecCpy);
INSTR(VecMember);
INSTR(PopArrayClass);

INSTR(AutoLoopStart);
INSTR(AutoLoopEnd);
INSTR(DotTmpl);

struct dottmpl_ {
  size_t len;
  m_str name;
  Func_Def base, def;
  Type owner;
  size_t overload; // => vtindex ?
  Type_List tl;
};
ANN void free_dottmpl(struct dottmpl_*);
ANN m_bool traverse_dot_tmpl(const Emitter emit, const struct dottmpl_ *dt);
// optimizations
#ifdef OPTIMIZE
INSTR(PutArgsInMem);
#endif
#include "opcode.h"
#endif
