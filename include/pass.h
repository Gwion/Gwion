#ifndef __GWIONPASS
#define __GWIONPASS

struct Passes_ {
  struct Map_    map;
  struct Vector_ vec;
};
// change this to gwion ?
typedef m_bool (*compilation_pass)(Env, Ast);

ANEW ANN struct Passes_* new_passes(MemPool mp);
ANN void free_passes(struct Passes_*);
ANN void pass_register(const Gwion, const m_str, const compilation_pass);
ANN void pass_default(const Gwion);
ANN m_bool pass_set(const Gwion, const Vector);
#endif
