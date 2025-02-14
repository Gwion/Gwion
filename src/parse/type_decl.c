#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "symbol.h"
#include "vm.h"
#include "traverse.h"
#include "parse.h"
#include "object.h"
#include "operator.h"
#include "instr.h"
#include "import.h"
#include "gwfmt.h"

ANN static Type _option(const Env env, Type_Decl *td, const uint8_t n) {
  const Array_Sub array = td->array;
  td->array = NULL;
  TmplArgList *tl  = new_tmplarglist(env->gwion->mp, 1);
  TmplArg arg = { .type = tmplarg_td, .d = { .td = td } };
  tmplarglist_set(tl, 0, arg);
  Type_Decl         tmp = { .tag = MK_TAG(insert_symbol("Option"), td->tag.loc), .types = tl };
  const Type t = !(n - 1) ? known_type(env, &tmp) : _option(env, &tmp, n - 1);
  free_tmplarglist(env->gwion->mp, tl);
  td->array = array;
  return t;
}

ANN static Type option(const Env env, Type_Decl *td) {
  const uint8_t option = td->option;
  td->option           = 0;
  const Type t         = _option(env, td, option);
  td->option           = option;
  return t;
}

ANN static Type _ref(const Env env, Type_Decl *td) {
  TmplArgList *tl  = new_tmplarglist(env->gwion->mp, 1);
  TmplArg arg = { .type = tmplarg_td, .d = { .td = td } };
  tmplarglist_set(tl, 0, arg);
  Type_Decl tmp = {.tag = MK_TAG(insert_symbol("Ref"), td->tag.loc), .types = tl};
  const Type t = known_type(env, &tmp);
  free_tmplarglist(env->gwion->mp, tl);
  return t;
}

ANN static inline Type ref(const Env env, Type_Decl *td) {
  td->ref      = false;
  const Type t = _ref(env, td);
  td->ref      = true;
  return t;
}

ANN static Symbol symname(const Env env, Func_Base *const base, bool *global) {
  GwText text;
  text_init(&text, env->gwion->mp);
  text_add(&text, "(");
  DECL_O(const Type, t, = known_type(env, base->td));
  DECL_O(const m_str, name, = type2str(env->gwion, t, base->td->tag.loc));
  text_add(&text, name);
  free_mstr(env->gwion->mp, name);
	text_add(&text, "(");
  *global = type_global(env, t);
  if(base->args) {
    for(uint32_t i = 0; i < base->args->len; i++) {
      if(i) text_add(&text, ",");
      const Arg arg = arglist_at(base->args, i);
      DECL_O(const Type, t, = known_type(env, arg.var.td));
      DECL_O(const m_str, name, = type2str(env->gwion, t, arg.var.td->tag.loc));
      text_add(&text, name);
      free_mstr(env->gwion->mp, name);
      if(*global)
        *global = type_global(env, t);
    }
  }
  text_add(&text, ")");
  text_add(&text, ")");
  base->tag.sym = insert_symbol(text.str);
  text_release(&text);
  return base->tag.sym;
}

ANN static inline Type find(const Env env, Type_Decl *td) {
  if (!td->fptr) return find_type(env, td);
  bool global = false;
  CHECK_O((td->tag.sym = symname(env, td->fptr->base, &global)));
  const Fptr_Def fptr = td->fptr;
  td->fptr = NULL;
  const Type exists = find_type(env, td);
  if(exists) return exists;
  const m_uint scope = env->context
      ? env_push_nspc(env, env->context->nspc)
      : env_push_global(env);
  const bool ret = traverse_fptr_def(env, fptr);
  env_pop(env, scope);
  const Type t = fptr->cdef->base.type;
  free_fptr_def(env->gwion->mp, fptr);
  return ret ? t : NULL;
}

ANN static inline Type find1(const Env env, const Type base, Type_Decl *td) {
  if (!td->fptr) return scan_type(env, base, td);
  if (!td->fptr->cdef->base.type) {
    CHECK_O(scan0_fptr_def(env, td->fptr));
    CHECK_O(traverse_fptr_def(env, td->fptr));
  }
  return td->fptr->cdef->base.type;
}

ANN static Type resolve(const Env env, Type_Decl *td) {
  Type_Decl *last = td;
  while (last->next) last = last->next;
  DECL_O(const Type, base, = find(env, td));
  if (base->error) ERR_O(td->tag.loc, _("type '%s' is invalid"), base->name);
  DECL_O(const Type, type, = find1(env, base, td));
  DECL_O(const Type, t,    = !td->ref ? type : ref(env, td));
  DECL_O(const Type, ret,  = !td->option ? t : option(env, td));
  const Array_Sub array = last->array;
  return !array ? ret : array_type(env, ret, array->depth, td->tag.loc);
}

ANN m_str tdpp(MemPool mp, SymTable *st, const Type_Decl *td,
               const bool no_color, const bool minimize) {
  struct GwfmtState ls     = { .minimize = minimize,};
  gwfmt_state_init(&ls);
  text_init(&ls.text, mp);
  Gwfmt gwfmter = {.mp = mp, .ls = &ls, .st = st };
  bool has_color = tcol_has_color();
  if(no_color) tcol_override_color_checks(false);
  gwfmt_type_decl(&gwfmter, td);
  tcol_override_color_checks(has_color);
  return ls.text.str;
}

ANN static inline void *type_unknown(const Env env, const Type_Decl *td) {
  m_str str = tdpp(env->gwion->mp, env->gwion->st, td, tcol_has_color(), false);
  env_err(env, td->tag.loc, _("unknown type '%s'"), str);
  free_mstr(env->gwion->mp, str);
  return NULL;
}

ANN Type known_type(const Env env, Type_Decl *td) {
  return resolve(env, td) ?: type_unknown(env, td);
}
