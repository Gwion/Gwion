#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "type.h"
#include "nspc.h"
#include "vm.h"
#include "parse.h"
#include "template.h"

ANN static inline m_bool _body(const Env e, Class_Body b, const _exp_func f) {
  do CHECK_BB(f(e, b->section))
  while((b = b->next));
  return GW_OK;
}

ANN static inline int actual(const Tmpl *tmpl) {
  return tmpl->call && tmpl->call != (Type_List)1;
}

ANN static inline m_bool tmpl_push(const Env env, const Tmpl* tmpl) {
  if(actual(tmpl))
    return template_push_types(env, tmpl);
  return GW_ERROR;
}

ANN static inline m_int _push(const Env env, const Class_Def c) {
  const m_uint scope = env_push_type(env, c->base.type);
  if(c->tmpl && !tmpl_push(env, c->tmpl))
    return GW_ERROR;
  return scope;
}

ANN static inline void _pop(const Env e, const Class_Def c, const m_uint s) {
  if(c->tmpl && actual(c->tmpl))
    nspc_pop_type(e->gwion->mp, e->curr);
  env_pop(e, s);
}

ANN m_bool
scanx_body(const Env e, const Class_Def c, const _exp_func f, void* d) {
  const m_int scope = _push(e, c);
  CHECK_BB(scope)
  const m_bool ret =  _body(d, c->body, f);
  _pop(e, c, scope);
  return ret;
}
