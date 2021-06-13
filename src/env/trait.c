#include <gwion_util.h>
#include <gwion_ast.h>
#include <gwion_env.h>

ANN Trait new_trait(MemPool mp, const loc_t loc) {
  const Trait a = mp_calloc(mp, Trait);
  a->loc        = loc;
  return a;
}

ANN void free_trait(MemPool mp, Trait a) {
  if (a->requested_values.ptr) vector_release(&a->requested_values);
  if (a->requested_funcs.ptr) vector_release(&a->requested_funcs);
  mp_free(mp, Trait, a);
}
