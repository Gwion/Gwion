#ifndef __MATCH
#define __MATCH
struct Match_ {
  struct Vector_ cond;
  struct Vector_ vec;
};

ANN static inline void match_map(struct Match_ *const match, Exp e) {
  const Vector cond = &match->cond;
  vector_init(cond);
  Exp next;
  do {
    next    = e->next;
    e->next = NULL;
    vector_add(cond, (vtype)e);
  } while ((e = next));
}

ANN static inline void match_unmap(struct Match_ *const match) {
  const Vector cond = &match->cond;
  const vtype  sz   = vector_size(cond);
  for (m_uint i = 0; i < sz - 1; ++i) {
    const Exp e = (Exp)vector_at(cond, i), next = (Exp)vector_at(cond, i + 1);
    e->next = next;
  }
  vector_release(cond);
}

#define MATCH_INI(scope)                                                       \
  struct Match_ *former = scope->match, m = {};                                \
  scope->match = &m;                                                           \
  match_map(&m, stmt->cond);

#define MATCH_END(scope)                                                       \
  match_unmap(scope->match);                                                   \
  scope->match = former;

#endif
