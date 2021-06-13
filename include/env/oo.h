#ifndef __OO
#define __OO

typedef struct Gwion_ *Gwion;

typedef struct Type_ * Type;
typedef struct Nspc_ * Nspc;
typedef struct Value_ *Value;
typedef struct Func_ * Func;

#define REF_FUNC(A, b)                                                         \
  ANN void               free_##b(const A, struct Gwion_ *const);              \
  ANN static inline void b##_addref(const A a) { ++a->ref; }                   \
  ANN static inline void b##_remref(const A a, struct Gwion_ *const gwion) {   \
    if (!--a->ref) free_##b(a, gwion);                                         \
  }
#endif
