#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "instr.h"
#include "object.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

ANN m_int gwi_item_ini(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  CHECK_BB(ck_ini(gwi, ck_item))
  if((gwi->ck->exp = make_exp(gwi, type, name)))
    return GW_OK;
  GWI_ERR_B(_("  ...  during var import '%s.%s'."), gwi->gwion->env->name, name)
}

ANN static m_int gwi_item_tmpl(const Gwi gwi) {
  const MemPool mp = gwi->gwion->mp;
  const Stmt stmt = new_stmt_exp(mp, ae_stmt_exp, gwi->ck->exp);
  const Stmt_List slist = new_stmt_list(mp, stmt, NULL);
  Section* section = new_section_stmt_list(mp, slist);
  const Ast body = new_ast(mp, section, NULL);
  gwi_body(gwi, body);
  mp_free2(gwi->gwion->mp, sizeof(ImportCK), gwi->ck);
  gwi->ck = NULL;
  return GW_OK;
}

#undef gwi_item_end
ANN2(1) m_int gwi_item_end(const Gwi gwi, const ae_flag flag, const m_uint* addr) {
  CHECK_BB(ck_ok(gwi, ck_item))
  const Env env = gwi->gwion->env;
  gwi->ck->exp->d.exp_decl.list->self->addr = (m_uint*)addr;
  gwi->ck->exp->d.exp_decl.td->flag = flag;
  if(env->class_def && GET_FLAG(env->class_def, template))
    return gwi_item_tmpl(gwi);
  CHECK_BB(traverse_decl(env, &gwi->ck->exp->d.exp_decl))
  const Value value = gwi->ck->exp->d.exp_decl.list->self->value;
  SET_FLAG(value, builtin);
  const m_uint offset = value->from->offset;
  free_exp(gwi->gwion->mp, gwi->ck->exp);
  ck_end(gwi);
  return offset;
}

ANN void ck_clean_item(MemPool mp, ImportCK* ck) {
  if(ck->exp)
    free_exp(mp, ck->exp);
}
