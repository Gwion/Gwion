#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "type.h"
#include "nspc.h"
#include "vm.h"
#include "template.h"
#include "traverse.h"
#include "parse.h"

ANN static inline m_bool _body(const Env e, Class_Body b, const _exp_func f) {
  do CHECK_BB(f(e, b->section))
  while((b = b->next));
  return GW_OK;
}

ANN static inline int actual(const Tmpl *tmpl) {
  return tmpl->call && tmpl->call != (Type_List)1;
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
  if(c->base.tmpl && actual(c->base.tmpl))
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

#undef scanx_ext
ANN m_bool
scanx_ext(const Env e, const Class_Def c, const _exp_func f, void* d) {
  DECL_BB(const m_int, scope, = _push(e, c))
  const m_bool ret =  f(d, c);
  _pop(e, c, scope);
  return ret;
}
#undef scanx_parent
__attribute__((returns_nonnull))
static inline Type get_type(const Type t) {
  return !t->array_depth ? t : array_base(t);
}

__attribute__((returns_nonnull))
static inline Class_Def get_type_def(const Type t) {
  return get_type(t)->e->def;
}

ANN m_bool
scanx_parent(const Type t, const _exp_func f, void* d) {
  const Class_Def def = get_type_def(t);
  return def ? f(d, def) : GW_OK;
}

ANN m_bool scanx_cdef(const Env env, void* opt, const Class_Def cdef,
    const _exp_func f_cdef, const _exp_func f_union) {
  if(cdef->base.type->e->parent !=  t_union)
     return f_cdef(opt, cdef);
  CHECK_BB(template_push_types(env, cdef->base.tmpl))
  const m_bool ret = f_union(opt, cdef->union_def);
  nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}
