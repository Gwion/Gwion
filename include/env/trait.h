#ifndef __TRAIT
#define __TRAIT

typedef struct Trait_ {
  struct Vector_ requested_values;
  struct Vector_ requested_funcs;
  m_str filename;
  loc_t loc;
} *Trait;

ANN Trait new_trait(MemPool, const loc_t);
ANN void free_trait(MemPool, Trait);
#endif
