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
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "cpy_ast.h"

#define GWI_ERR_B(a,...) { env_err(gwi->gwion->env, gwi->loc, (a), ## __VA_ARGS__); return GW_ERROR; }
#define GWI_ERR_O(a,...) { env_err(gwi->gwion->env, gwi->loc, (a), ## __VA_ARGS__); return NULL; }
#define ENV_ERR_B(pos, a,...) { env_err(env, pos, (a), ## __VA_ARGS__); return GW_ERROR; }
#define ENV_ERR_O(pos, a,...) { env_err(env, pos, (a), ## __VA_ARGS__); return NULL; }

ANN static void dl_var_new_exp_array(MemPool p, DL_Var* v) {
  v->td->array = new_array_sub(p, NULL);
  v->td->array->depth = v->array_depth;
}

ANN static void dl_var_set(MemPool p, DL_Var* v, const ae_flag flag) {
  v->list.self = &v->var;
  v->td->flag = flag;
  v->exp.exp_type = ae_exp_decl;
  v->exp.d.exp_decl.td   = v->td;
  v->exp.d.exp_decl.list = &v->list;
  if(v->array_depth)
    dl_var_new_exp_array(p, v);
}

ANN static void dl_var_release(MemPool p, const DL_Var* v) {
  free_type_decl(p, v->td);
}

ANN m_int gwi_item_ini(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  DL_Var* v = &gwi->var;
  memset(v, 0, sizeof(DL_Var));
  CHECK_BB(name_valid(gwi, name))
  v->var.xid = insert_symbol(gwi->gwion->st, name);
  if((v->td = str2decl(gwi->gwion->env, type, &v->array_depth, gwi->loc)))
    return GW_OK;
  GWI_ERR_B(_("  ...  during var import '%s.%s'."), gwi->gwion->env->name, name)
}

ANN2(1) m_int gwi_item_tmpl(const Gwi gwi) {
  const MemPool mp = gwi->gwion->mp;
  DL_Var* v = &gwi->var;
  Type_Decl *td = cpy_type_decl(mp, v->td);
  const Var_Decl var = new_var_decl(mp, v->var.xid, v->var.array,
    loc_cpy(mp, gwi->loc));
  const Var_Decl_List vlist = new_var_decl_list(mp, var, NULL);
  const Exp exp = new_exp_decl(mp, td, vlist);
  const Stmt stmt = new_stmt_exp(mp, ae_stmt_exp, exp);
  const Stmt_List slist = new_stmt_list(mp, stmt, NULL);
  Section* section = new_section_stmt_list(mp, slist);
  const Class_Body body = new_class_body(mp, section, NULL);
  gwi_body(gwi, body);
  return GW_OK;
}

#undef gwi_item_end
ANN2(1) m_int gwi_item_end(const Gwi gwi, const ae_flag flag, const m_uint* addr) {
  const Env env = gwi->gwion->env;
  DL_Var* v = &gwi->var;
  dl_var_set(gwi->gwion->mp, v, flag | ae_flag_builtin);
  v->var.addr = (void*)addr;
  if(env->class_def && GET_FLAG(env->class_def, template))
    return gwi_item_tmpl(gwi);
  v->exp.pos = gwi->loc;
  CHECK_BB(traverse_decl(env, &v->exp.d.exp_decl))
  SET_FLAG(v->var.value, builtin);
  dl_var_release(gwi->gwion->mp, v);
  return (m_int)v->var.value->from->offset;
}
