#ifndef __MATCH
#define __MATCH
struct Match_ {
  struct Map_ map;
  struct Vector_ vec;
};

ANN static inline void match_map(struct Match_ * const match, Exp e) {
  const Map map = &match->map;
  map_init(map);
  Exp next;
  do {
    next = e->next;
    e->next = NULL;
    map_set(map, (vtype)e, 0);
  } while((e = next));
}

ANN static inline void match_unmap(struct Match_ * const match) {
  const Map map = &match->map;
  for(m_uint i = 0; i < map_size(map) - 1; ++i) {
    const Exp e = (Exp)VKEY(map, i), next = (Exp)VKEY(map, i + 1);
    e->next = next;
  }
  map_release(map);
}

#define MATCH_INI(scope)                        \
  struct Match_ *former = scope->match, m = {}; \
  scope->match = &m;                             \
  match_map(&m, stmt->cond);

#define MATCH_END(scope)     \
  match_unmap(scope->match); \
  scope->match = former;

#endif
