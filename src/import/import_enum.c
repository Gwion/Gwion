//! \file enum.c
//! \brief enumerations utils

#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "instr.h"
#include "object.h"
#include "emit.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

//! start an enum definition
//! \arg the importer
//! \arg string defining a primitive type
//! why is return type m_int ?
ANN2(1) m_int gwi_enum_ini(const Gwi gwi, const m_str type) {
  CHECK_BB(ck_ini(gwi, ck_edef))
  if(type)
    CHECK_OB((gwi->ck->xid = gwi_str2sym(gwi, type)))
  vector_init(&gwi->ck->v);
  return GW_OK;
}

// adds the id_list to the enum
// change that algo?
ANN static void add2list(struct ImportCK* ck, const ID_List list) {
  if(!ck->tmpl)
    ck->tmpl = list;
  else
    ck->curr->next = list;
  ck->curr = list;
}
/*
void Append(DL_Enum* d, const ID_List list) {
  List* next = &d->base;
  while (*next != NULL) next = &(*next)->Next;
  *next = list;
  next->next = NULL;
}
*/
//! add an enum entry
//! \arg the importer
//! \arg name of the entry
//! TODO: change return type to m_bool
ANN m_int gwi_enum_add(const Gwi gwi, const m_str name, const m_uint i) {
  CHECK_BB(ck_ok(gwi, ck_edef))
  DECL_OB(const ID_List, list, = gwi_str2symlist(gwi, name))
  add2list(gwi->ck, list);
  ALLOC_PTR(gwi->gwion->mp, addr, m_int, i);
  vector_add(&gwi->ck->v, (vtype)addr);
  return GW_OK;
}

//! set enum values
//! \arg the importer
//! \arg a vector of values
//! \note [internal]
ANN static void import_enum_end(const Gwi gwi, const Vector v) {
  ImportCK *ck = gwi->ck;
  for(m_uint i = 0; i < vector_size(v); i++) {
    const Value value = (Value)vector_at(v, i);
    const m_uint addr = vector_at(&ck->v, i);
    SET_FLAG(value, builtin);
//    ADD_REF(value->type); // what ?
    if(!gwi->gwion->env->class_def)
      value->d.ptr = (m_uint*)(addr ? addr : i);
    else
      value->d.ptr = (m_uint*)(addr ? *(m_uint*)addr : i);
  }
  // better clean ?
}

//! finish enum import
//! \arg the importer
//! TODO: check what happens in inside template class
ANN Type gwi_enum_end(const Gwi gwi) {
  CHECK_BO(ck_ok(gwi, ck_edef))
  const Gwion gwion = gwi->gwion;
  const Enum_Def edef  = new_enum_def(gwion->mp, gwi->ck->tmpl,
      gwi->ck->xid, loc(gwi));
  gwi->ck->tmpl = NULL;
  const m_bool ret = traverse_enum_def(gwion->env, edef);
  import_enum_end(gwi, &edef->values);
  const Type t = ret > 0 ? edef->t : NULL;
  free_enum_def(gwion->mp, edef);
  vector_release(&gwi->ck->v);
  gwi->ck->v.ptr = NULL;
  ck_end(gwi);
  return t;
}

ANN void ck_clean_edef(MemPool mp, ImportCK *ck) {
  if(ck->tmpl)
    free_id_list(mp, ck->tmpl);
  if(ck->v.ptr) {
    for(m_uint i = 0; i < vector_size(&ck->v); ++i)
      mp_free2(mp, SZ_INT, (m_uint*)vector_at(&ck->v, i));
    vector_release(&ck->v);
  }
}

