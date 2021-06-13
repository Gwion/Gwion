#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
#include "mpool.h"
#include "specialid.h"

ANN m_int gwi_typedef_ini(const Gwi gwi, const restrict m_str type,
                          const restrict m_str name) {
  CHECK_BB(ck_ini(gwi, ck_tdef));
  gwi->ck->name = name;
  CHECK_BB(check_typename_def(gwi, gwi->ck));
  return (gwi->ck->td = gwi_str2td(gwi, type)) ? GW_OK : GW_ERROR;
}

ANN Type gwi_typedef_end(const Gwi gwi, const ae_flag flag) {
  CHECK_BO(ck_ok(gwi, ck_tdef));
  Type_Decl *td = gwi->ck->td;
  td->flag |= flag;
  const Type_Def tdef =
      new_type_def(gwi->gwion->mp, td, gwi->ck->sym, gwi->loc);
  if (gwi->ck->tmpl) tdef->tmpl = gwi_tmpl(gwi);
  gwi->ck->td      = NULL;
  gwi->ck->tmpl    = NULL;
  const m_bool ret = traverse_type_def(gwi->gwion->env, tdef);
  if (gwi->gwion->data->cdoc) {
    lint_indent(gwi->lint);
    lint_type_def(gwi->lint, tdef);
  }
  const Type t = tdef->type;
  if (ret > 0)
    set_tflag(t, tflag_scan0 | tflag_scan1 | tflag_scan2 | tflag_check |
                     tflag_emit);
  free_type_def(gwi->gwion->mp, tdef);
  ck_end(gwi);
  return ret > 0 ? t : NULL;
}

ANN void ck_clean_tdef(MemPool mp, ImportCK *ck) {
  if (ck->td) free_type_decl(mp, ck->td);
  if (ck->tmpl) free_id_list(mp, ck->tmpl);
}
