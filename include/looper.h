#ifndef __LOOPER
#define __LOOPER

typedef struct Looper Looper;

typedef Instr (*f_looper)(const Emitter, const Looper *);

struct Looper {
  Exp*           exp;
  Stmt *const          stmt;
  /*const */ m_uint   offset;
  const m_uint        n;
  const f_looper roll;
  const f_looper unroll;
//  union {
    struct Vector_ unroll_v;
    Instr instr;
//  };
  Var_Decl *idx;
  bool init;
};
#endif
