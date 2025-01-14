#include <gwion_util.h>
#include <gwion_ast.h>
#include <gwion_env.h>

ANN Trait new_trait(MemPool mp, const loc_t loc) {
  const Trait a = mp_calloc(mp, Trait);
  a->loc        = loc;
  return a;
}

ANN void free_trait(MemPool mp, Trait a) {
  if (a->var) free_valuelist(mp, a->var);
  if (a->fun) free_funcdeflist(mp, a->fun);
  mp_free(mp, Trait, a);
}
