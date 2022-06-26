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

ANN m_int gwi_item_ini(const Gwi gwi, const restrict m_str type,
                       const restrict m_str name) {
  CHECK_BB(ck_ini(gwi, ck_item));
  if ((gwi->ck->exp = make_exp(gwi, type, name))) return GW_OK;
  GWI_ERR_B(_("  ...  during var import '%s.%s'."), gwi->gwion->env->name, name)
}

ANN static m_int gwi_item_tmpl(const Gwi gwi) {
  Stmt_List slist = new_mp_vector(gwi->gwion->mp, struct Stmt_, 1);
  mp_vector_set(slist, struct Stmt_, 0,  ((struct Stmt_) {
      .stmt_type = ae_stmt_exp,
      .d = { .stmt_exp = { .val = gwi->ck->exp } },
      .pos = gwi->loc
    }));
  Section section = (Section) {
    .section_type = ae_section_stmt,
    .d = { .stmt_list = slist }
  };
//  Section *       section = new_section_stmt_list(mp, slist);
//  const Ast       body    = new_ast(mp, section, NULL);
//  gwi_body(gwi, body);
  gwi_body(gwi, &section);
  mp_free2(gwi->gwion->mp, sizeof(ImportCK), gwi->ck);
  gwi->ck = NULL;
  return GW_OK;
}

#undef gwi_item_end
ANN2(1)
m_int gwi_item_end(const Gwi gwi, const ae_flag flag, union value_data addr) {
  CHECK_BB(ck_ok(gwi, ck_item));
  const Env env                     = gwi->gwion->env;
  gwi->ck->exp->d.exp_decl.td->flag = flag;
  if (gwi->gwion->data->cdoc) {
    lint_indent(gwi->lint);
    lint_exp(gwi->lint, gwi->ck->exp);
    lint_sc(gwi->lint);
    lint_nl(gwi->lint);
  }
  if (env->class_def && tflag(env->class_def, tflag_tmpl))
    return gwi_item_tmpl(gwi);
  CHECK_BB(traverse_exp(env, gwi->ck->exp));
  const Value value = gwi->ck->exp->d.exp_decl.vd.value;
  value->d          = addr;
  set_vflag(value, vflag_builtin);
  if (!env->class_def) SET_FLAG(value, global);
  const m_uint offset = value->from->offset;
  free_exp(gwi->gwion->mp, gwi->ck->exp);
  ck_end(gwi);
  return offset;
}

ANN void ck_clean_item(MemPool mp, ImportCK *ck) {
  if (ck->exp) free_exp(mp, ck->exp);
}
