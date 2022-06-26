#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "parse.h"
#include "object.h"
#include "operator.h"
#include "instr.h"
#include "import.h"

ANN static Type _option(const Env env, Type_Decl *td, const uint8_t n) {
  Type_List tl  = new_mp_vector(env->gwion->mp, Type_Decl*, 1);
  mp_vector_set(tl, Type_Decl*, 0, td);
  Type_Decl         tmp = {
      .xid = insert_symbol("Option"), .types = tl, .pos = td->pos};
  const Type t = !(n - 1) ? known_type(env, &tmp) : _option(env, &tmp, n - 1);
  free_mp_vector(env->gwion->mp, Type_Decl*, tl);
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
  Type_List tl  = new_mp_vector(env->gwion->mp, Type_Decl*, 1);
  mp_vector_set(tl, Type_Decl*, 0, td);
  Type_Decl tmp = {.xid = insert_symbol("Ref"), .types = tl, .pos = td->pos};
  const Type t = known_type(env, &tmp);
  free_mp_vector(env->gwion->mp, Type_Decl*, tl);
  return t;
}

ANN static inline Type ref(const Env env, Type_Decl *td) {
  td->ref      = false;
  const Type t = _ref(env, td);
  td->ref      = true;
  return t;
}

ANN static Symbol symname(const Env env, Func_Base *const base, bool *global) {
  GwText text = { .mp = env->gwion->mp };
  text_add(&text, "(");
  DECL_OO(const Type, t, = known_type(env, base->td));
  DECL_OO(const m_str, name, = type2str(env->gwion, t, base->td->pos));
  text_add(&text, name);
  free_mstr(env->gwion->mp, name);
	text_add(&text, "(");
  *global = type_global(env, t);
  if(base->args) {
    for(uint32_t i = 0; i < base->args->len; i++) {
      if(i) text_add(&text, ",");
      Arg *arg = mp_vector_at(base->args, Arg, i);
      DECL_OO(const Type, t, = known_type(env, arg->td));
      DECL_OO(const m_str, name, = type2str(env->gwion, t, arg->td->pos));
      text_add(&text, name);
      free_mstr(env->gwion->mp, name);
      if(*global)
        *global = type_global(env, t);
    }
  }
  text_add(&text, ")");
  text_add(&text, ")");
  base->xid = insert_symbol(text.str);
  text_release(&text);
  return base->xid;
}

ANN static inline Type find(const Env env, Type_Decl *td) {
  if (!td->fptr) return find_type(env, td);
  bool global = false;
  CHECK_OO((td->xid = symname(env, td->fptr->base, &global)));
  const Fptr_Def fptr = td->fptr;
  td->fptr = NULL;
  const Type exists = find_type(env, td);
  if(exists) return exists;
  const m_uint scope = env->context
      ? env_push(env, NULL, env->context->nspc)
      : env_push_global(env);
  const m_bool ret = traverse_fptr_def(env, fptr);
  env_pop(env, scope);
  const Type t = fptr->cdef->base.type;
  free_fptr_def(env->gwion->mp, fptr);
  return ret > 0 ? t : NULL;
}

ANN static inline Type find1(const Env env, const Type base, Type_Decl *td) {
  if (!td->fptr) return scan_type(env, base, td);
  if (!td->fptr->cdef->base.type) {
    CHECK_BO(scan0_fptr_def(env, td->fptr));
    CHECK_BO(traverse_fptr_def(env, td->fptr));
  }
  return td->fptr->cdef->base.type;
}

ANN static Type resolve(const Env env, Type_Decl *td) {
  Type_Decl *last = td;
  while (last->next) last = last->next;
  DECL_OO(const Type, base, = find(env, td));
  const Context ctx = base->info->value->from->ctx;
  if (ctx && ctx->error) ERR_O(td->pos, _("type '%s' is invalid"), base->name)
  DECL_OO(const Type, type, = find1(env, base, td));
  DECL_OO(const Type, t,    = !td->ref ? type : ref(env, td));
  DECL_OO(const Type, ret,  = !td->option ? t : option(env, td));
  const Array_Sub array = last->array;
  return !array ? ret : array_type(env, ret, array->depth);
}

ANN static inline void *type_unknown(const Env env, const Type_Decl *td) {
  env_err(env, td->pos, _("unknown type '%s'"), s_name(td->xid));
  return NULL;
}

ANN Type known_type(const Env env, Type_Decl *td) {
  return resolve(env, td) ?: type_unknown(env, td);
}
