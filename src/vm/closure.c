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
  Closure *a = mp_malloc2(mp, sizeof(Closure) + sz);
  map_init(&a->m);
  a->sz = sz;
  return a;
}

ANN void free_closure(Closure *a, const Gwion gwion) {
  const Map m = &a->m;
  for (m_uint i = 0; i < map_size(m); ++i) {
    const Type t = (Type)VKEY(m, i);
    const m_bit *data = tflag(t, tflag_ref) ?
      (a->data + VVAL(m, i)) : *(m_bit**)(a->data + VVAL(m, i));
    compound_release(gwion->vm->cleaner_shred, t, data);
  }
  map_release(m);
  _mp_free(gwion->mp, sizeof(Closure) + a->sz, a);
}
