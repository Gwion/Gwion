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
#define BYTECODE_SZ (SZ_INT * 4)

ANN void free_instr(const Gwion, const Instr);
INSTR(EOC);
INSTR(DTOR_EOC);

struct FastExceptInfo {
  m_str file;
  loc_t loc;
  m_str file2;
  loc_t loc2;
};

INSTR(fast_except);

/* array */
INSTR(ArrayBottom);
INSTR(ArrayPost);
INSTR(ArrayInit);
INSTR(ArrayAlloc);
INSTR(ArrayStruct);

INSTR(DotTmpl);
INSTR(GTmpl);

struct dottmpl_ {
  m_str     name;
  Func_Def  base, def;
  Type_List tl;
};
ANN m_bool traverse_dot_tmpl(const Emitter emit, const Func_Def fdef, const Value v);

INSTR(SetFunc);
INSTR(SetCtor);
#include "opcode.h"

INSTR(dict_ctor_alt);
INSTR(dict_lit_ctor);
#endif
