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
ANN m_int gwi_enum_ini(const Gwi gwi, const m_str type) {
  CHECK_BB(ck_ini(gwi, ck_edef));
  CHECK_OB((gwi->ck->xid = gwi_str2sym(gwi, type)));
  gwi->ck->tmpl = new_mp_vector(gwi->gwion->mp, EnumValue, 0);
  return GW_OK;
}

//! add an enum entry
//! \arg the importer
//! \arg name of the entry
//! TODO: change return type to m_bool
ANN m_int gwi_enum_add(const Gwi gwi, const m_str name, const m_uint i) {
  CHECK_BB(ck_ok(gwi, ck_edef));
  DECL_OB(const Symbol, xid, = gwi_str2sym(gwi, name));
  const EnumValue ev = { .tag = MK_TAG(xid, gwi->loc), .gwint = { .num = i }, .set = true};
  mp_vector_add(gwi->gwion->mp, &gwi->ck->tmpl, EnumValue, ev);
  return GW_OK;
}

//! finish enum import
//! \arg the importer
ANN Type gwi_enum_end(const Gwi gwi) {
  CHECK_BO(ck_ok(gwi, ck_edef));
  if (!gwi->ck->tmpl->len) GWI_ERR_O("Enum is empty");
  const Gwion    gwion = gwi->gwion;
  const Enum_Def edef =
      new_enum_def(gwion->mp, gwi->ck->tmpl, gwi->ck->xid, gwi->loc);
// clean the vector
  gwi->ck->tmpl    = NULL;
  const m_bool ret = traverse_enum_def(gwion->env, edef);
  if (gwi->gwion->data->cdoc) gwfmt_enum_def(gwi->gwfmt, edef);
  const Type t = ret > 0 ? edef->type : NULL;
  free_enum_def(gwion->mp, edef);
  ck_end(gwi);
  return t;
}
