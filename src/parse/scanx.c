#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "template.h"
#include "traverse.h"
#include "parse.h"

ANN static inline m_bool _body(const Env e, Ast b, const _exp_func f) {
  for(m_uint i = 0; i < b->len; i++) {
    Section *section = mp_vector_at(b, Section, i);
    CHECK_BB(f(e, section));
  }
  return GW_OK;
}

ANN static inline int actual(const Tmpl *tmpl) {
  return tmpl->call && tmpl->call != (Type_List)1 && tmpl->list;
}

ANN static inline m_bool tmpl_push(const Env env, const Tmpl *tmpl) {
  return actual(tmpl) ? template_push_types(env, tmpl) : GW_ERROR;
}

ANN static inline m_int _push(const Env env, const Class_Def c) {
  DECL_BB(const m_int, scope, = env_push_type(env, c->base.type));
  return (!c->base.tmpl || tmpl_push(env, c->base.tmpl)) ? scope : GW_ERROR;
}

ANN static inline void _pop(const Env e, const Class_Def c, const m_uint s) {
  if (c->base.tmpl && actual(c->base.tmpl))
    nspc_pop_type(e->gwion->mp, e->curr);
  env_pop(e, s);
}

// TODO: 'v' should be 2° argument
ANN m_bool scanx_body(const Env e, const Class_Def c, const _exp_func f,
                      void *d) {
  DECL_BB(const m_int, scope, = _push(e, c));
  const m_bool ret = _body(d, c->body, f);
  _pop(e, c, scope);
  return ret;
}

ANN static m_bool _scanx_cdef(const Env env, void *opt, const Type t,
                              const _exp_func f_cdef, const _exp_func f_udef) {
  if (t->info->parent != env->gwion->type[et_union])
    return f_cdef(opt, t->info->cdef);
  const m_bool ret = f_udef(opt, t->info->udef);
  return ret;
}

ANN m_bool scanx_cdef(const Env env, void *opt, const Type t,
                      const _exp_func f_cdef, const _exp_func f_udef) {
  const bool   in_try = env->scope->in_try;
  const m_bool ret    = _scanx_cdef(env, opt, t, f_cdef, f_udef);
  env->scope->in_try  = in_try;
  return ret;
}

ANN m_bool scanx_fdef(const Env env, void *data, const Func_Def fdef,
                      const _exp_func func) {
  if (fdef->base->tmpl) CHECK_BB(template_push_types(env, fdef->base->tmpl));
  const bool   in_try = env->scope->in_try;
  const m_bool ret    = func(data, fdef);
  if (fdef->base->tmpl) nspc_pop_type(env->gwion->mp, env->curr);
  env->scope->in_try = in_try;
  return ret;
}
