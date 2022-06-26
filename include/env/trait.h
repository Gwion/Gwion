#ifndef __TRAIT
#define __TRAIT

typedef struct Trait_ {
  MP_Vector *var;
  MP_Vector *fun;
  m_str      name;
  m_str      filename;
  loc_t      loc;
} * Trait;

ANN Trait new_trait(MemPool, const loc_t);
ANN void  free_trait(MemPool, Trait);
#endif
