#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "template.h"
#include "traverse.h"
#include "parse.h"

ANN static inline m_bool _body(const Env e, Ast b, const _exp_func f) {
  do CHECK_BB(f(e, b->section))
  while((b = b->next));
  return GW_OK;
}

ANN static inline int actual(const Tmpl *tmpl) {
  return tmpl->call && tmpl->call != (Type_List)1 && tmpl->list;
}

ANN static inline m_bool tmpl_push(const Env env, const Tmpl* tmpl) {
  return actual(tmpl) ? template_push_types(env, tmpl) : GW_ERROR;
}

ANN static inline m_int _push(const Env env, const Class_Def c) {
  DECL_BB(const m_int, scope, = env_push_type(env, c->base.type))
  return (!c->base.tmpl || tmpl_push(env, c->base.tmpl)) ?
    scope : GW_ERROR;
}

ANN static inline void _pop(const Env e, const Class_Def c, const m_uint s) {
  if(c->base.tmpl && actual(c->base.tmpl) && c->base.tmpl->list)
    nspc_pop_type(e->gwion->mp, e->curr);
  env_pop(e, s);
}

// TODO: 'v' should be 2° argument
ANN m_bool
scanx_body(const Env e, const Class_Def c, const _exp_func f, void* d) {
  DECL_BB(const m_int, scope, = _push(e, c))
  const m_bool ret =  _body(d, c->body, f);
  _pop(e, c, scope);
  return ret;
}

__attribute__((returns_nonnull))
ANN Type get_type(const Type t) {
  const Type type = !t->array_depth ? t : array_base(t);
  return type;
}

ANN m_bool scanx_cdef(const Env env, void* opt, const Type base,
    const _exp_func f_cdef, const _exp_func f_union) {
  const Type t = get_type(base);
  if(t->info->parent !=  env->gwion->type[et_union])
     return f_cdef(opt, t->info->cdef);
  const m_bool ret = f_union(opt, t->info->udef);
  return ret;
}

ANN m_bool scanx_fdef(const Env env, void *data,
    const Func_Def fdef, const _exp_func func) {
  if(fdef->base->tmpl)
    CHECK_BB(template_push_types(env, fdef->base->tmpl))
  const m_bool ret = func(data, fdef);
  if(fdef->base->tmpl)
    nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}
