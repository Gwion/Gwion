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
ANN Exp* make_exp(const Gwi gwi, const m_str type, const m_str name) {
  DECL_O(Type_Decl *, td, = gwi_str2td(gwi, type));
  struct Var_Decl_ vd;
  if(!gwi_str2var(gwi, &vd, name)) {
    free_type_decl(gwi->gwion->mp, td);
    return NULL;
  }
  return new_exp_decl(gwi->gwion->mp, td, &vd, gwi->loc);
}

ANN bool gwi_union_ini(const Gwi gwi, const m_str name) {
  CHECK_B(ck_ini(gwi, ck_udef));
  gwi->ck->name = name;
  CHECK_B(check_typename_def(gwi, gwi->ck));
  gwi->ck->list = new_variablelist(gwi->gwion->mp, 0);
  return true;
}

ANN bool gwi_union_add(const Gwi gwi, const restrict m_str type,
                        const restrict m_str name) {
  CHECK_B(ck_ok(gwi, ck_udef));
  DECL_B(Type_Decl *, td, = str2td(gwi->gwion, type, gwi->loc));
  DECL_B(const Symbol, xid, = str2sym(gwi->gwion, name, gwi->loc));
  Variable um = { .td = td, .vd = { .tag = MK_TAG(xid, gwi->loc) } };
  variablelist_add(gwi->gwion->mp, &gwi->ck->list, um);
//  const Variable_List l = new_variable_list(gwi->gwion->mp, td, xid, gwi->loc);
//  l->next            = gwi->ck->list;
//  gwi->ck->list      = l;
  return true;
}

ANN static Type union_type(const Gwi gwi, const Union_Def udef) {
  CHECK_O(scan0_union_def(gwi->gwion->env, udef));
  CHECK_O(traverse_union_def(gwi->gwion->env, udef));
  //  if(!udef->tmpl)
  //    emit_union_offset(udef->l, udef->o);
  //  if(gwi->gwion->env->class_def && !GET_FLAG(udef, static))
  //      gwi->gwion->env->class_def->nspc->offset =
  //       udef->o + udef->s;
  //  set_vflag(udef->value, vflag_builtin);
  //  const M_Object o = new_object(gwi->gwion->mp, udef->value->type);
  //  udef->value->d.ptr = (m_uint*)o;
  if (gwi->gwion->data->cdoc) {
    gwfmt_indent(gwi->gwfmt);
    gwfmt_union_def(gwi->gwfmt, udef);
  }
  return udef->type;
}

ANN bool gwi_union_end(const Gwi gwi, const ae_flag flag) {
  CHECK_B(ck_ok(gwi, ck_udef));
  if (!gwi->ck->list->len) GWI_ERR_O(_("union is empty"));
  const Union_Def udef = new_union_def(gwi->gwion->mp, gwi->ck->list, gwi->loc);
  gwi->ck->list        = NULL;
  udef->flag           = flag;
  udef->tag.sym        = gwi->ck->sym;
  if (gwi->ck->sl) {
    udef->tmpl    = gwi_tmpl(gwi);
    gwi->ck->sl = NULL;
  }
  const Type t = union_type(gwi, udef);
  if (!safe_tflag(t, tflag_tmpl)) free_union_def(gwi->gwion->mp, udef);
  ck_end(gwi);
  return !!t;
}

ANN void ck_clean_udef(MemPool mp, ImportCK *ck) {
  if (ck->list) free_variablelist(mp, ck->list);
  if (ck->sl) free_specializedlist(mp, ck->sl);
}
