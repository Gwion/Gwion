#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "template.h"
#include "traverse.h"
#include "parse.h"

ANN static inline bool _body(const Env env, Ast ast, const _envset_func f) {
  bool ok = true;
  for(m_uint i = 0; i < ast->len; i++) {
    Section *section = sectionlist_ptr_at(ast, i);
    if(section->poison) { ok = false; continue; }
    if(!f(env, section))
      POISON_SECTION(ok, env, section);
  }
  return ok;
}

ANN static inline void _push(const Env env, const Class_Def c) {
  env_push_type(env, c->base.type);
  if (c->base.tmpl)
    template_push_types(env, c->base.tmpl);
}

ANN static inline void _pop(const Env env, const Class_Def c, const m_uint s) {
  if (c->base.tmpl)
    nspc_pop_type(env->gwion->mp, env->curr);
  env_pop(env, s);
}

ANN bool scanx_body(const Env env, const Class_Def c, const _envset_func f,
                      void *d) {
  const m_int scope = env->scope->depth; 
  _push(env, c);
  const bool ret = _body(d, c->body, f);
  _pop(env, c, scope);
  return ret;
}

ANN static bool _scanx_cdef(const Env env, void *opt, const Type t,
                              const _envset_func f_cdef, const _envset_func f_udef) {
  if (t->info->parent != env->gwion->type[et_union])
    return f_cdef(opt, t->info->cdef);
  return f_udef(opt, t->info->udef);
}

ANN bool scanx_cdef(const Env env, void *opt, const Type t,
                      const _envset_func f_cdef, const _envset_func f_udef) {
  const bool in_try = env->scope->in_try;
  const bool ret    = _scanx_cdef(env, opt, t, f_cdef, f_udef);
  env->scope->in_try  = in_try;
  return ret;
}

ANN bool scanx_fdef(const Env env, void *data, const Func_Def fdef,
                      const _envset_func func) {
  if (fdef->base->tmpl) CHECK_B(template_push_types(env, fdef->base->tmpl));
  const bool in_try = env->scope->in_try;
  const bool ret    = func(data, fdef);
  if (fdef->base->tmpl) nspc_pop_type(env->gwion->mp, env->curr);
  env->scope->in_try = in_try;
  return ret;
}

