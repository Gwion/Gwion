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
#include "gwion_parse.h"

ANN2(1, 2, 3)
static bool dl_func_init(const Gwi gwi, const restrict m_str t,
                           const restrict m_str n) {
  CHECK_B(ck_ini(gwi, ck_fdef));
  gwi->ck->name = n;
  CHECK_B(check_typename_def(gwi, gwi->ck));
  CHECK_B((gwi->ck->td = gwi_str2td(gwi, t)));
  gwi->ck->arg_list = new_arglist(gwi->gwion->mp, 0);
  return true;
}

ANN bool gwi_func_ini(const Gwi gwi, const restrict m_str t,
                       const restrict m_str n) {
  return dl_func_init(gwi, t, n);
}

ANEW ANN static Func_Base *gwi_func_base(const Gwi gwi, ImportCK *ck) {
  ArgList *args = gwi->ck->arg_list->len ? cpy_arg_list(gwi->gwion->mp, gwi->ck->arg_list) : NULL;
  Func_Base *    base = new_func_base(gwi->gwion->mp, ck->td, ck->sym, args,
                                  ck->flag, gwi->loc);
  ck->td = NULL;
  if (ck->sl) {
    base->tmpl = gwi_tmpl(gwi);
    ck->sl   = NULL;
  }
  return base;
}

ANEW ANN static Func_Def import_fdef(const Gwi gwi, ImportCK *ck) {
  Func_Base *    base = gwi_func_base(gwi, ck);
  const Func_Def fdef = new_func_def(gwi->gwion->mp, base, NULL);
  if (gwi->gwion->data->cdoc) {
    gwfmt_indent(gwi->gwfmt);
    gwfmt_func_def(gwi->gwfmt, fdef);
  }
  if (gwi->effects.ptr) {
    vector_init(&fdef->base->effects);
    vector_copy2(&gwi->effects, &fdef->base->effects);
    vector_release(&gwi->effects);
  }
  return fdef;
}

ANN static bool section_fdef(const Gwi gwi, const Func_Def fdef) {
  Section section = MK_SECTION(func, func_def, fdef, gwi->loc);
  gwi_body(gwi, &section);
  return true;
}

ANN static bool error_fdef(const Gwi gwi, const Func_Def fdef) {
  func_def_cleaner(gwi->gwion, fdef);
  return false;
}

ANN static bool gwi_func_valid(const Gwi gwi, const f_xfun addr, ImportCK *ck) {
  const Func_Def fdef = import_fdef(gwi, ck);
  fdef->builtin = true;
  if (safe_tflag(gwi->gwion->env->class_def, tflag_tmpl)) {
    section_fdef(gwi, fdef);
    fdef->d.dl_func_ptr = addr;
    return true;
  }
  if (!traverse_func_def(gwi->gwion->env, fdef))
    return error_fdef(gwi, fdef);
  builtin_func(gwi->gwion, fdef->base->func, addr);
  return true;
}

ANN bool gwi_func_end(const Gwi gwi, const f_xfun addr, const ae_flag flag) {
  CHECK_B(ck_ok(gwi, ck_fdef));
  gwi->ck->flag    = flag;
  const bool ret = gwi_func_valid(gwi, addr, gwi->ck);
  ck_end(gwi);
  return ret;
}

ANN bool gwi_func_code(const Gwi gwi, const char *data) {
  Func_Def fdef = gwion_parse_func_def(gwi->gwion, data, gwi->loc);
  if (safe_tflag(gwi->gwion->env->class_def, tflag_tmpl))
    return section_fdef(gwi, fdef);
  if (!traverse_func_def(gwi->gwion->env, fdef))
    return error_fdef(gwi, fdef);
  return true;
}

ANN bool gwi_func_arg(const Gwi gwi, const restrict m_str t,
                       const restrict m_str n) {
  CHECK_B(ck_ok(gwi, ck_fdef));
  DECL_B(Type_Decl *, td, = gwi_str2td(gwi, t));
  struct Var_Decl_ var;
  if(gwi_str2var(gwi, &var, n)) {
    Arg arg = { .var = MK_VAR(td, var), .loc = td->tag.loc};
    arglist_add(gwi->gwion->mp, &gwi->ck->arg_list, arg);
    return true;
  }
  free_type_decl(gwi->gwion->mp, td);
  return false;
}

ANN bool gwi_fptr_ini(const Gwi gwi, const restrict m_str type,
                       const restrict m_str name) {
  return dl_func_init(gwi, type, name);
}

ANN static Fptr_Def import_fptr(const Gwi gwi) {
  Func_Base *base = gwi_func_base(gwi, gwi->ck);
  return new_fptr_def(gwi->gwion->mp, base);
}

ANN static bool section_fptr(const Gwi gwi, const Fptr_Def fdef) {
  Section section = MK_SECTION(fptr, fptr_def, fdef, gwi->loc);
  gwi_body(gwi, &section);
  return true;
}

ANN Type gwi_fptr_end(const Gwi gwi, const ae_flag flag) {
  CHECK_O(ck_ok(gwi, ck_fdef));
  DECL_O(const Fptr_Def, fptr, = import_fptr(gwi));
  fptr->base->flag |= flag;
  if (gwi->gwion->data->cdoc) {
    gwfmt_indent(gwi->gwfmt);
    gwfmt_fptr_def(gwi->gwfmt, fptr);
  }
  if (safe_tflag(gwi->gwion->env->class_def,
                 tflag_tmpl) /*&& !fptr->base->tmpl*/) {
    section_fptr(gwi, fptr);
    ck_end(gwi);
    return (Type)true;
  }
  const bool ret = traverse_fptr_def(gwi->gwion->env, fptr);
//  if (fptr->base->func) // is it needed ?
//    set_vflag(fptr->base->func->value_ref, vflag_builtin);
  const Type t = ret ? fptr->cdef->base.type : NULL;
  free_fptr_def(gwi->gwion->mp, fptr);
  ck_end(gwi);
  return t;
}

ANN void ck_clean_fdef(MemPool mp, ImportCK *ck) {
  if (ck->td) free_type_decl(mp, ck->td);
  free_arg_list(mp, ck->arg_list);
  if (ck->sl) free_specializedlist(mp, ck->sl);
}
