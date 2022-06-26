//! \file enum.c
//! \brief enumerations utils

#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "instr.h"
#include "emit.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

//! start an enum definition
//! \arg the importer
//! \arg string defining a primitive type
//! why is return type m_int ?

__attribute__ ((visibility ("default")))
ANN m_int gwi_enum_ini(const Gwi gwi, const m_str type) {
  CHECK_BB(ck_ini(gwi, ck_edef));
  CHECK_OB((gwi->ck->xid = gwi_str2sym(gwi, type)));
  vector_init(&gwi->ck->v);
  gwi->ck->tmpl = new_mp_vector(gwi->gwion->mp, Symbol, 0);
  return GW_OK;
}
/*
// adds the id_list to the enum
// change that algo?
ANN static void add2list(struct ImportCK *ck, const ID_List list) {
  if (!ck->tmpl) {
    ck->tmpl = new_mp_vector(list, ;
  } else {
    ck->curr->next = list;
  }
  ck->curr = list;
}
*/
//! add an enum entry
//! \arg the importer
//! \arg name of the entry
//! TODO: change return type to m_bool
ANN m_int gwi_enum_add(const Gwi gwi, const m_str name, const m_uint i) {
  CHECK_BB(ck_ok(gwi, ck_edef));
//  DECL_OB(const ID_List, list, = gwi_str2symlist(gwi, name));

  DECL_OB(const Symbol, xid, = gwi_str2sym(gwi, name));
  mp_vector_add(gwi->gwion->mp, &gwi->ck->tmpl, Symbol, xid);
//  add2list(gwi->ck, list);
  vector_add(&gwi->ck->v, (vtype)i);
  return GW_OK;
}

//! set enum values
//! \arg the importer
//! \arg a vector of values
//! \note [internal]
ANN static void import_enum_end(const Gwi gwi, const Vector v) {
  ImportCK *ck = gwi->ck;
  for (m_uint i = 0; i < vector_size(v); i++) {
    const Value  value = (Value)vector_at(v, i);
    const m_uint addr  = vector_at(&ck->v, i);
    set_vflag(value, vflag_builtin);
    value->d.num = addr ?: i;
  }
  // better clean ?
}

//! finish enum import
//! \arg the importer
//! TODO: check what happens in inside template class
ANN Type gwi_enum_end(const Gwi gwi) {
  CHECK_BO(ck_ok(gwi, ck_edef));
  if (!vector_size(&gwi->ck->v)) GWI_ERR_O("Enum is empty");
  const Gwion    gwion = gwi->gwion;
  const Enum_Def edef =
      new_enum_def(gwion->mp, gwi->ck->tmpl, gwi->ck->xid, gwi->loc);
  gwi->ck->tmpl    = NULL;
  const m_bool ret = traverse_enum_def(gwion->env, edef);
  if (ret > 0) import_enum_end(gwi, &edef->values);
  if (gwi->gwion->data->cdoc) {
    lint_indent(gwi->lint);
    lint_enum_def(gwi->lint, edef);
  }
  const Type t = ret > 0 ? edef->t : NULL;
  if (edef->values.ptr) vector_release(&edef->values);
  free_enum_def(gwion->mp, edef);
  vector_release(&gwi->ck->v);
  gwi->ck->v.ptr = NULL;
  ck_end(gwi);
  return t;
}
