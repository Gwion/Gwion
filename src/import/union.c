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

ANN static Exp make_exp(const Gwi gwi, const m_str type, const m_str name) {
  const Env env = gwi->gwion->env;
  m_uint array_depth;
  Array_Sub array = NULL;
  DECL_OO(const ID_List, id_list, = str2list(env, type, &array_depth, gwi->loc))
  if(array_depth) {
    array = new_array_sub(env->gwion->mp, NULL);
    array->depth = array_depth;
  }
  Type_Decl *type_decl = new_type_decl(env->gwion->mp, id_list);
  const Var_Decl var_decl = new_var_decl(env->gwion->mp,
      insert_symbol(env->gwion->st, name), array, loc_cpy(env->gwion->mp, gwi->loc));
  const Var_Decl_List var_decl_list = new_var_decl_list(env->gwion->mp, var_decl, NULL);
  return new_exp_decl(env->gwion->mp, type_decl, var_decl_list);
}

ANN2(1) m_int gwi_union_ini(const Gwi gwi, const m_str type, const m_str name) {
  gwi->union_data.type_name = type;
  gwi->union_data.name = name;
  return GW_OK;
}

ANN m_int gwi_union_add(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  DECL_OB(const Exp, exp, = make_exp(gwi, type, name))
  SET_FLAG(exp->d.exp_decl.td, ref);
  gwi->union_data.list = new_decl_list(gwi->gwion->mp, exp, gwi->union_data.list);
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
  if(!gwi->union_data.list)
    GWI_ERR_O(_("union is empty"));
  if(gwi->union_data.name)
    CHECK_BO(name_valid(gwi, gwi->union_data.name))
  struct func_checker ck = { .name=gwi->union_data.type_name, .flag=flag };
  if(gwi->union_data.type_name)
    CHECK_BO(check_typename_def(gwi, &ck))
  const Symbol xid = gwi->union_data.name ? insert_symbol(gwi->gwion->st, gwi->union_data.name) : NULL;
  const Symbol type_xid = gwi->union_data.type_name ? insert_symbol(gwi->gwion->st, ck.name) : NULL;
  const Union_Def udef = new_union_def(gwi->gwion->mp, gwi->union_data.list, loc_cpy(gwi->gwion->mp, gwi->loc));
  udef->flag = flag;
  udef->xid = xid;
  udef->type_xid = type_xid;
  if(ck.tmpl) {
    if(udef->xid)
      GWI_ERR_O(_("Template union type can't declare union"));
    udef->tmpl = new_tmpl(gwi->gwion->mp, ck.tmpl, -1);
  }
  const Type t = union_type(gwi, udef);
  if(!SAFE_FLAG(t, template))
    free_union_def(gwi->gwion->mp, udef);
  gwi->union_data.list = NULL;
  gwi->union_data.name  = NULL;
  gwi->union_data.type_name  = NULL;
  return t;
}
