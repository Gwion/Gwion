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

// move me
ANN Exp make_exp(const Gwi gwi, const m_str type, const m_str name) {
  DECL_OO(Type_Decl *, td, = gwi_str2td(gwi, type));
  const Var_Decl_List vlist = gwi_str2varlist(gwi, name);
  if (vlist) return new_exp_decl(gwi->gwion->mp, td, vlist, gwi->loc);
  free_type_decl(gwi->gwion->mp, td);
  return NULL;
}

ANN m_int gwi_union_ini(const Gwi gwi, const m_str name) {
  CHECK_BB(ck_ini(gwi, ck_udef));
  gwi->ck->name = name;
  CHECK_BB(check_typename_def(gwi, gwi->ck));
  return GW_OK;
}

ANN m_int gwi_union_add(const Gwi gwi, const restrict m_str type,
                        const restrict m_str name) {
  CHECK_BB(ck_ok(gwi, ck_udef));
  DECL_OB(Type_Decl *, td, = str2td(gwi->gwion, type, gwi->loc));
  DECL_OB(const Symbol, xid, = str2sym(gwi->gwion, name, gwi->loc));
  const Union_List l = new_union_list(gwi->gwion->mp, td, xid, gwi->loc);
  l->next            = gwi->ck->list;
  gwi->ck->list      = l;
  return GW_OK;
}

ANN static Type union_type(const Gwi gwi, const Union_Def udef) {
  CHECK_BO(scan0_union_def(gwi->gwion->env, udef));
  CHECK_BO(traverse_union_def(gwi->gwion->env, udef));
  //  if(!udef->tmpl)
  //    emit_union_offset(udef->l, udef->o);
  //  if(gwi->gwion->env->class_def && !GET_FLAG(udef, static))
  //      gwi->gwion->env->class_def->nspc->offset =
  //       udef->o + udef->s;
  //  set_vflag(udef->value, vflag_builtin);
  //  const M_Object o = new_object(gwi->gwion->mp, NULL, udef->value->type);
  //  udef->value->d.ptr = (m_uint*)o;
  if (gwi->gwion->data->cdoc) {
    lint_indent(gwi->lint);
    lint_union_def(gwi->lint, udef);
  }
  return udef->type;
}

ANN Type gwi_union_end(const Gwi gwi, const ae_flag flag) {
  CHECK_BO(ck_ok(gwi, ck_udef));
  if (!gwi->ck->list) GWI_ERR_O(_("union is empty"));
  const Union_Def udef = new_union_def(gwi->gwion->mp, gwi->ck->list, gwi->loc);
  gwi->ck->list        = NULL;
  udef->flag           = flag;
  udef->xid            = gwi->ck->sym;
  if (gwi->ck->tmpl) {
    udef->tmpl    = gwi_tmpl(gwi);
    gwi->ck->tmpl = NULL;
  }
  const Type t = union_type(gwi, udef);
  if (!safe_tflag(t, tflag_tmpl)) free_union_def(gwi->gwion->mp, udef);
  ck_end(gwi);
  return t;
}

ANN void ck_clean_udef(MemPool mp, ImportCK *ck) {
  if (ck->list) free_union_list(mp, ck->list);
  if (ck->tmpl) free_id_list(mp, ck->tmpl);
}
