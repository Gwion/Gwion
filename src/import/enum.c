#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "value.h"
#include "traverse.h"
#include "instr.h"
#include "object.h"
#include "emit.h"
#include "func.h"
#include "nspc.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

ANN2(1) m_int gwi_enum_ini(const Gwi gwi, const m_str type) {
  gwi->enum_data.t = type;
  vector_init(&gwi->enum_data.addr);
  return GW_OK;
}

ANN m_int gwi_enum_add(const Gwi gwi, const m_str name, const m_uint i) {
  CHECK_BB(name_valid(gwi, name))
  const Gwion gwion = gwi->gwion;
  const ID_List list = new_id_list(gwion->mp, insert_symbol(gwion->st, name),
      loc_cpy(gwion->mp, gwi->loc));
  DL_Enum* d = &gwi->enum_data;
  ALLOC_PTR(gwion->mp, addr, m_int, i);
  vector_add(&gwi->enum_data.addr, (vtype)addr);
  if(!d->base)
    d->base = list;
  else
    d->curr->next = list;
  d->curr = list;
  return list ? 1 : -1;
}

ANN static void import_enum_end(const Gwi gwi, const Vector v) {
  DL_Enum* d = &gwi->enum_data;
  for(m_uint i = 0; i < vector_size(v); i++) {
    Value value = (Value)vector_at(v, i);
    const m_uint addr = vector_at(&d->addr, i);
    SET_FLAG(value, builtin);
    ADD_REF(value->type);
    if(!gwi->gwion->env->class_def)
      value->d.ptr = (m_uint*)(addr ? addr : i);
    else
      value->d.ptr = (m_uint*)(addr ? *(m_uint*)addr : i);
  }
  d->t = NULL;
  d->base = NULL;
  vector_release(&d->addr);
}

ANN Type gwi_enum_end(const Gwi gwi) {
  const Gwion gwion = gwi->gwion;
  DL_Enum* d = &gwi->enum_data;
  const Enum_Def edef  = new_enum_def(gwion->mp, d->base,
        d->t ? insert_symbol(gwion->st, d->t) : NULL, loc_cpy(gwion->mp, gwi->loc));
  const m_bool ret = traverse_enum_def(gwion->env, edef);
  import_enum_end(gwi, &edef->values);
  const Type t = ret > 0 ? edef->t : NULL;
  free_enum_def(gwion->mp, edef);
  return t;
}
