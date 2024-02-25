#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

ANN bool gwi_item_ini(const Gwi gwi, const restrict m_str type,
                       const restrict m_str name) {
  CHECK_B(ck_ini(gwi, ck_item));
  if ((gwi->ck->exp = make_exp(gwi, type, name))) return true;
  GWI_ERR_B(_("  ...  during var import '%s.%s'."), gwi->gwion->env->name, name)
}

ANN static bool gwi_item_tmpl(const Gwi gwi) {
  Stmt_List slist = new_mp_vector(gwi->gwion->mp, Stmt, 1);
  mp_vector_set(slist, Stmt, 0, MK_STMT_EXP(gwi->loc, gwi->ck->exp));
  Section section = MK_SECTION(stmt, stmt_list, slist, gwi->loc);
  gwi_body(gwi, &section);
  mp_free2(gwi->gwion->mp, sizeof(ImportCK), gwi->ck);
  gwi->ck = NULL;
  return true;
}

#undef gwi_item_end
ANN2(1)
bool gwi_item_end(const Gwi gwi, const ae_flag flag, union value_data addr) {
  CHECK_B(ck_ok(gwi, ck_item));
  const Env env                     = gwi->gwion->env;
  gwi->ck->exp->d.exp_decl.var.td->flag = flag;
  if (gwi->gwion->data->cdoc) {
    gwfmt_indent(gwi->gwfmt);
    gwfmt_exp(gwi->gwfmt, gwi->ck->exp);
    gwfmt_sc(gwi->gwfmt);
    gwfmt_nl(gwi->gwfmt);
  }
  if (env->class_def && tflag(env->class_def, tflag_tmpl))
    return gwi_item_tmpl(gwi);
  CHECK_B(traverse_exp(env, gwi->ck->exp));
  const Value value = gwi->ck->exp->d.exp_decl.var.vd.value;
  value->d          = addr;
  set_vflag(value, vflag_builtin);
  if (!env->class_def) SET_FLAG(value, global);
  free_exp(gwi->gwion->mp, gwi->ck->exp);
  ck_end(gwi);
  return true;
}

ANN void ck_clean_item(MemPool mp, ImportCK *ck) {
  if (ck->exp) free_exp(mp, ck->exp);
}
