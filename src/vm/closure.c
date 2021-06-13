#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "memoize.h"
#include "gwion.h"
#include "object.h"
#include "array.h"
#include "operator.h"
#include "import.h"

ANN Closure *new_closure(MemPool mp, const m_uint sz) {
  Closure *a = mp_malloc(mp, Closure);
  a->data    = mp_malloc2(mp, sz);
  map_init(&a->m);
  a->sz = sz;
  return a;
}

ANN void free_closure(Closure *a, const Gwion gwion) {
  const Map m = &a->m;
  for (m_uint i = 0; i < map_size(m); ++i)
    compound_release(gwion->vm->cleaner_shred, (Type)VKEY(m, i),
                     a->data + VVAL(m, i));
  map_release(m);
  _mp_free(gwion->mp, a->sz, a->data);
}
