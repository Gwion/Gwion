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
#include "gwion_parse.h"

ANN bool gwi_typedef_ini(const Gwi gwi, const restrict m_str type,
                          const restrict m_str name) {
  CHECK_B(ck_ini(gwi, ck_tdef));
  gwi->ck->name = name;
  CHECK_B(check_typename_def(gwi, gwi->ck));
  return !!(gwi->ck->td = gwi_str2td(gwi, type));
}

ANN bool gwi_typedef_exp(const Gwi gwi, const restrict m_str data) {
  CHECK_B((gwi->ck->when = gwion_parse_expression(gwi->gwion, data, gwi->loc)));
  return true;
}

ANN bool gwi_typedef_end(const Gwi gwi, const ae_flag flag) {
  CHECK_O(ck_ok(gwi, ck_tdef));
  Type_Decl *td = gwi->ck->td;
  td->flag |= flag;
  const Type_Def tdef =
      new_type_def(gwi->gwion->mp, td, gwi->ck->sym, gwi->loc);
  if (gwi->ck->when) tdef->when = gwi->ck->when;
  if (gwi->ck->sl) tdef->tmpl = gwi_tmpl(gwi);

  if (safe_tflag(gwi->gwion->env->class_def, tflag_tmpl)) {
    Section section = MK_SECTION(
      type, type_def, tdef, gwi->loc 
    );
    gwi_body(gwi, &section);
    ck_end(gwi);
    return true;
  }
  
  gwi->ck->td      = NULL;
  gwi->ck->sl    = NULL;
  const bool ret = traverse_type_def(gwi->gwion->env, tdef);
  if (gwi->gwion->data->cdoc) {
    gwfmt_indent(gwi->gwfmt);
    gwfmt_type_def(gwi->gwfmt, tdef);
  }
  const Type t = tdef->type;
  if (ret)
    set_tflag(t, tflag_scan0 | tflag_scan1 | tflag_scan2 | tflag_check |
                     tflag_emit);
  free_type_def(gwi->gwion->mp, tdef);
  ck_end(gwi);
  return ret;
}

ANN void ck_clean_tdef(MemPool mp, ImportCK *ck) {
  if (ck->td) free_type_decl(mp, ck->td);
  if (ck->when) free_exp(mp, ck->when);
  if (ck->sl) free_specializedlist(mp, ck->sl);
}
