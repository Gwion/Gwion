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
#include "clean.h"

ANN2(1, 2, 3)
static m_bool dl_func_init(const Gwi gwi, const restrict m_str t,
                           const restrict m_str n) {
  CHECK_BB(ck_ini(gwi, ck_fdef));
  gwi->ck->name = n;
  CHECK_BB(check_typename_def(gwi, gwi->ck));
  CHECK_OB((gwi->ck->td = gwi_str2td(gwi, t)));
  vector_init(&gwi->ck->v);
  return GW_OK;
}

ANN m_int gwi_func_ini(const Gwi gwi, const restrict m_str t,
                       const restrict m_str n) {
  return dl_func_init(gwi, t, n);
}

ANN Arg_List make_dll_arg_list(const Vector v) {
  Arg_List base = (Arg_List)vector_front(v), arg_list = base;
  for (m_uint i = 1; i < vector_size(v); ++i)
    arg_list = (arg_list->next = (Arg_List)vector_at(v, i));
  vector_release(v);
  v->ptr = NULL;
  return base;
}

ANEW ANN static Func_Base *gwi_func_base(const Gwi gwi, ImportCK *ck) {
  const Arg_List arg_list = make_dll_arg_list(&gwi->ck->v);
  Func_Base *    base = new_func_base(gwi->gwion->mp, ck->td, ck->sym, arg_list,
                                  ck->flag, gwi->loc);
  if (ck->variadic) base->fbflag |= fbflag_variadic;
  ck->td = NULL;
  if (ck->tmpl) {
    base->tmpl = gwi_tmpl(gwi);
    ck->tmpl   = NULL;
  }
  return base;
}

ANEW ANN static Func_Def import_fdef(const Gwi gwi, ImportCK *ck) {
  Func_Base *    base = gwi_func_base(gwi, ck);
  const Func_Def fdef = new_func_def(gwi->gwion->mp, base, NULL);
  if (gwi->gwion->data->cdoc) {
    lint_indent(gwi->lint);
    lint_func_def(gwi->lint, fdef);
  }
  if (gwi->effects.ptr) {
    vector_init(&fdef->base->effects);
    vector_copy2(&gwi->effects, &fdef->base->effects);
    vector_release(&gwi->effects);
  }
  return fdef;
}

ANN static m_bool section_fdef(const Gwi gwi, const Func_Def fdef) {
  Section * section = new_section_func_def(gwi->gwion->mp, fdef);
  const Ast body    = new_ast(gwi->gwion->mp, section, NULL);
  gwi_body(gwi, body);
  return GW_OK;
}

ANN static m_bool error_fdef(const Gwi gwi, const Func_Def fdef) {
  func_def_cleaner(gwi->gwion, fdef);
  return GW_ERROR;
}

ANN m_int gwi_func_valid(const Gwi gwi, ImportCK *ck) {
  const Func_Def fdef = import_fdef(gwi, ck);
  if (safe_tflag(gwi->gwion->env->class_def, tflag_tmpl))
    return section_fdef(gwi, fdef);
  if (traverse_func_def(gwi->gwion->env, fdef) < 0)
    return error_fdef(gwi, fdef);
  builtin_func(gwi->gwion->mp, fdef->base->func, ck->addr);
  return GW_OK;
}

ANN m_int gwi_func_end(const Gwi gwi, const f_xfun addr, const ae_flag flag) {
  CHECK_BB(ck_ok(gwi, ck_fdef));
  gwi->ck->addr    = addr;
  gwi->ck->flag    = flag;
  const m_bool ret = gwi_func_valid(gwi, gwi->ck);
  ck_end(gwi);
  return ret;
}

ANN m_int gwi_func_arg(const Gwi gwi, const restrict m_str t,
                       const restrict m_str n) {
  CHECK_BB(ck_ok(gwi, ck_fdef));
  if (gwi->ck->variadic) GWI_ERR_B(_("already declared as variadic"));
  if (!strcmp(n, "...")) {
    gwi->ck->variadic = 1;
    return GW_OK;
  }
  DECL_OB(Type_Decl *, td, = gwi_str2td(gwi, t));
  const Var_Decl var = gwi_str2var(gwi, n);
  if (var) {
    const Arg_List arg = new_arg_list(gwi->gwion->mp, td, var, NULL);
    vector_add(&gwi->ck->v, (vtype)arg);
    return GW_OK;
  }
  free_type_decl(gwi->gwion->mp, td); // ???
  return GW_ERROR;
}

ANN m_int gwi_fptr_ini(const Gwi gwi, const restrict m_str type,
                       const restrict m_str name) {
  return dl_func_init(gwi, type, name);
}

ANN static Fptr_Def import_fptr(const Gwi gwi) {
  Func_Base *base = gwi_func_base(gwi, gwi->ck);
  return new_fptr_def(gwi->gwion->mp, base);
}

ANN static m_bool section_fptr(const Gwi gwi, const Fptr_Def fdef) {
  Section * section = new_section_fptr_def(gwi->gwion->mp, fdef);
  const Ast body    = new_ast(gwi->gwion->mp, section, NULL);
  gwi_body(gwi, body);
  return GW_OK;
}

ANN Type gwi_fptr_end(const Gwi gwi, const ae_flag flag) {
  CHECK_BO(ck_ok(gwi, ck_fdef));
  DECL_OO(const Fptr_Def, fptr, = import_fptr(gwi));
  fptr->base->flag |= flag;
  if (gwi->gwion->data->cdoc) {
    lint_indent(gwi->lint);
    lint_fptr_def(gwi->lint, fptr);
  }
  if (safe_tflag(gwi->gwion->env->class_def,
                 tflag_tmpl) /* && !fptr->base->tmpl*/) {
    section_fptr(gwi, fptr);
    ck_end(gwi);
    return (Type)GW_OK;
  }
  const m_bool ret = traverse_fptr_def(gwi->gwion->env, fptr);
  if (fptr->base->func) // is it needed ?
    set_vflag(fptr->base->func->value_ref, vflag_builtin);
  const Type t = ret > 0 ? fptr->type : NULL;
  free_fptr_def(gwi->gwion->mp, fptr);
  if (fptr->type) type_remref(fptr->type, gwi->gwion);
  ck_end(gwi);
  return t;
}

ANN void ck_clean_fdef(MemPool mp, ImportCK *ck) {
  if (ck->td) free_type_decl(mp, ck->td);
  if (ck->v.ptr) {
    for (m_uint i = 0; i < vector_size(&ck->v); ++i) {
      Arg_List list = (Arg_List)vector_at(&ck->v, i);
      list->next    = NULL;
      free_arg_list(mp, list);
    }
    vector_release(&ck->v);
  }
  if (ck->tmpl) free_id_list(mp, ck->tmpl);
}
