#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "traverse.h"
#include "vm.h"
#include "parse.h"

ANN Map env_label(const Env env) {
  return &env->context->lbls;
}

#define GET(a,b) ((a) & (b)) == (b)
ANN m_bool env_access(const Env env, const ae_flag flag, const loc_t pos) {
  if(env->scope->depth) {
   if(GET(flag, ae_flag_global))
      ERR_B(pos, _("'global' can only be used at %s scope."),
          GET(flag, ae_flag_global) && !env->class_def ?
           "file" : "class")
  }
  if((GET(flag, ae_flag_static) || GET(flag, ae_flag_private) ||
      GET(flag, ae_flag_protect)) && (!env->class_def || env->scope->depth))
      ERR_B(pos, _("static/private/protect can only be used at class scope."))
  return GW_OK;
}

ANN m_bool env_storage(const Env env, ae_flag flag, const loc_t pos) {
  CHECK_BB(env_access(env, flag, pos))
  return !(env->class_def && GET(flag, ae_flag_global)) ? GW_OK :GW_ERROR;
}

ANN Type __find_type(const Type type, const Symbol xid) {
  Type base = type;
  while(base && base->nspc) {
    const Type t = nspc_lookup_type1(base->nspc, xid);
    if(t)
      return t;
    base = base->e->parent;
  }
  return NULL;
}

ANN Type _find_type(const Env env, const Symbol xid) {
  const Type type = nspc_lookup_type1(env->curr, xid);
  if(type || !env->class_def)
    return type;
  return __find_type(env->class_def, xid);
}

ANN Type find_type(const Env env, Type_Decl *path) {
  DECL_OO(Type, type, = _find_type(env, path->xid))
  while((path = path->next) && type && type->nspc) {
    const Nspc nspc = type->nspc;
    type = __find_type(type, path->xid);
    if(!type)
      ERR_O(path->pos, _("...(cannot find class '%s' in nspc '%s')"), s_name(path->xid), nspc->name)
  }
  return type;
}

ANN m_bool already_defined(const Env env, const Symbol s, const loc_t pos) {
  const Value v = nspc_lookup_value0(env->curr, s);
  if(!v || is_class(env->gwion, v->type))
    return GW_OK;
  env_err(env, pos,
      _("'%s' already declared as variable of type '%s'."), s_name(s), v->type->name);
  return GW_ERROR;
}


ANN static Type class_type(const Env env, const Type base) {
  const Type t_class = env->gwion->type[et_class];
  const Type t = type_copy(env->gwion->mp, t_class);
  t->e->parent = t_class;
  t->e->ctx = base->e->ctx;
  t->e->d.base_type = base;
  SET_FLAG(t, infer);
  return t;
}

ANN Value mk_class(const Env env, const Type base) {
  const Type t = class_type(env, base);
  const Symbol sym = insert_symbol(base->name);
  const Value v = new_value(env->gwion->mp, t, s_name(sym));
  valuefrom(env, v->from);
  SET_FLAG(v, const | ae_flag_valid);
  nspc_add_value_front(base->e->owner, sym, v);
  return v;
}

ANN Value global_string(const Env env, const m_str str) {
  char c[strlen(str) + 8];
  sprintf(c, "%s:string", str);
  const Symbol sym = insert_symbol(c);
  const Value v = nspc_lookup_value0(env->global_nspc, sym);
  if(v)
    return v;
  const Value value = new_value(env->gwion->mp, env->gwion->type[et_string], s_name(sym));
  nspc_add_value_front(env->global_nspc, sym, value);
  return value;
}

ANN m_bool isres(const Env env, const Symbol xid, const loc_t pos) {
  if(vector_find(&env->gwion->data->reserved, (vtype)xid) > -1)
    ERR_B(pos, _("%s is reserved."), s_name(xid));
  return GW_OK;
}
