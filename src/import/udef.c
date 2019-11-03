#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
#include "emit.h"
#include "func.h"
#include "nspc.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

// move me
ANN Exp make_exp(const Gwi gwi, const m_str type, const m_str name) {
  DECL_OO(Type_Decl*, td, = str2decl(gwi, type))
  const Var_Decl_List vlist = str2varlist(gwi, name);
  if(vlist)
    return new_exp_decl(gwi->gwion->mp, td, vlist);
  free_type_decl(gwi->gwion->mp, td);
  return NULL;
}

ANN2(1) m_int gwi_union_ini(const Gwi gwi, const m_str type, const m_str name) {
  CHECK_BB(ck_ini(gwi, ck_udef))
  if(name)
    CHECK_OB((gwi->ck->sym = str2sym(gwi, name)))
//  gwi->ck->name = name;
  gwi->ck->name = type;
  if(type)
    CHECK_BB(check_typename_def(gwi, gwi->ck))
  return GW_OK;
}

ANN m_int gwi_union_add(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  CHECK_BB(ck_ok(gwi, ck_udef))
  DECL_OB(const Exp, exp, = make_exp(gwi, type, name))
  SET_FLAG(exp->d.exp_decl.td, ref); // might not be needed
  gwi->ck->list = new_decl_list(gwi->gwion->mp, exp, gwi->ck->list);
  return GW_OK;
}

ANN static Type union_type(const Gwi gwi, const Union_Def udef) {
  CHECK_BO(scan0_union_def(gwi->gwion->env, udef))
  CHECK_BO(traverse_union_def(gwi->gwion->env, udef))
  if(!udef->tmpl)
    emit_union_offset(udef->l, udef->o);
  if(gwi->gwion->env->class_def && !GET_FLAG(udef, static))
      gwi->gwion->env->class_def->nspc->info->offset =
       udef->o + udef->s;
  if(udef->xid || !udef->type_xid) {
    SET_FLAG(udef->value, builtin);
    const M_Object o = new_object(gwi->gwion->mp, NULL, udef->value->type);
    udef->value->d.ptr = (m_uint*)o;
   return udef->value->type;
  }
  return udef->type;
}

ANN Type gwi_union_end(const Gwi gwi, const ae_flag flag) {
  CHECK_BO(ck_ok(gwi, ck_udef))
  if(!gwi->ck->list)
    GWI_ERR_O(_("union is empty"));
  const Union_Def udef = new_union_def(gwi->gwion->mp, gwi->ck->list, loc(gwi));
  gwi->ck->list = NULL;
  udef->flag = flag;
  udef->xid = gwi->ck->xid;
  udef->type_xid = gwi->ck->sym;
  if(gwi->ck->tmpl) {
    if(udef->xid)
      GWI_ERR_O(_("Template union type can't declare instance at declaration"));
    udef->tmpl = new_tmpl(gwi->gwion->mp, gwi->ck->tmpl, -1);
    gwi->ck->tmpl = NULL;
  }
  const Type t = union_type(gwi, udef);
  if(!SAFE_FLAG(t, template))
    free_union_def(gwi->gwion->mp, udef);
  ck_end(gwi);
  return t;
}

ANN void ck_clean_udef(MemPool mp, ImportCK* ck) {
  if(ck->list)
    free_decl_list(mp, ck->list);
  if(ck->tmpl)
    free_id_list(mp, ck->tmpl);
  if(ck->td)
    free_type_decl(mp, ck->td);
}
