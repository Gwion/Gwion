#ifndef __INSTR
#define __INSTR
#define PUSH_MEM(a, b) a->mem += b;
#define POP_MEM(a, b)  a->mem -= b;
#define PUSH_REG(a, b) a->reg += b;
#define POP_REG(a, b)  a->reg -= b;

#define MEM(a) (shred->mem + (a))
#define REG(a) (shred->reg + (a))
#define INSTR(a)                                                               \
  __attribute__((hot)) ANN2(1) void a(const VM_Shred shred NUSED,              \
                                      const Instr instr    NUSED)

// enum Kind { KIND_INT, KIND_FLOAT, KIND_OTHER, KIND_ADDR };

typedef struct Instr_ *Instr;
typedef void (*f_instr)(const VM_Shred, const Instr);

struct InstrVal {
  uint16_t one;
  uint16_t two;
};

struct Instr_ {
  m_uint opcode;
  union {
    m_float f;
    m_uint  m_val;
  };
  union {
    m_uint          m_val2;
    struct InstrVal udata;
  };
  void (*execute)(const VM_Shred shred, const Instr instr);
};
#define BYTECODE_SZ                                                            \
  ((2 * sizeof(unsigned)) + sizeof(struct Instr_) - SZ_INT * 2)

INSTR(EOC);
INSTR(DTOR_EOC);
INSTR(DtorReturn);

INSTR(ComplexReal);
INSTR(ComplexImag);

/* function */
INSTR(DtorReturn);

/* array */
INSTR(ArrayBottom);
INSTR(ArrayPost);
INSTR(ArrayInit);
INSTR(ArrayAlloc);
INSTR(ArrayStruct);

/* vararg */
INSTR(VarargIni);

INSTR(DotTmpl);
INSTR(GTmpl);

struct dottmpl_ {
  size_t    len;
  m_str     name;
  Func_Def  base, def;
  Type      owner_class;
  Nspc      owner;
  Type_List tl;
  void *    xfun; // (type is f_xfun)
};
ANN void   free_dottmpl(struct dottmpl_ *);
ANN m_bool traverse_dot_tmpl(const Emitter emit, const struct dottmpl_ *dt);

INSTR(SetFunc);
INSTR(SetRecurs);
INSTR(SetCtor);
// optimizations
#ifdef OPTIMIZE
INSTR(PutArgsInMem);
#endif
#include "opcode.h"
#endif
