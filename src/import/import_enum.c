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
ANN bool gwi_enum_ini(const Gwi gwi, const m_str type) {
  CHECK_B(ck_ini(gwi, ck_edef));
  CHECK_B((gwi->ck->xid = gwi_str2sym(gwi, type)));
  gwi->ck->enums = new_enumvaluelist(gwi->gwion->mp, 0);
  return true;
}

//! add an enum entry
//! \arg the importer
//! \arg name of the entry
ANN bool gwi_enum_add(const Gwi gwi, const m_str name, const m_uint i) {
  CHECK_B(ck_ok(gwi, ck_edef));
  DECL_B(const Symbol, xid, = gwi_str2sym(gwi, name));
  const EnumValue ev = { .tag = MK_TAG(xid, gwi->loc) , .gwint = { .num = i }, .set = true};
  enumvaluelist_add(gwi->gwion->mp, &gwi->ck->enums, ev);
  return true;
}

//! finish enum import
//! \arg the importer
ANN bool gwi_enum_end(const Gwi gwi) {
  CHECK_B(ck_ok(gwi, ck_edef));
  if (!gwi->ck->enums->len) GWI_ERR_B("Enum is empty");
  const Gwion    gwion = gwi->gwion;
  const Enum_Def edef =
      new_enum_def(gwion->mp, gwi->ck->enums, gwi->ck->xid, gwi->loc);
// clean the vector
  gwi->ck->enums    = NULL;
  const bool ret = traverse_enum_def(gwion->env, edef);
  if (gwi->gwion->data->cdoc) gwfmt_enum_def(gwi->gwfmt, edef);
  free_enum_def(gwion->mp, edef);
  ck_end(gwi);
  return ret;
}
