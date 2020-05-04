#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "template.h"

ANN static void check(struct EnvSet *es, const Type t) {
  const Vector v = &es->env->scope->class_stack;
  Type owner = t->e->owner_class;
  for(vtype i = vector_size(v); owner && --i;) {
    if(owner != (Type)vector_at(v, i - 1)) {
      es->run = 1;
      return;
    }
    owner = owner->e->owner_class;
  }
}

ANN m_bool push(struct EnvSet *es, const Type t) {
  if(t->e->owner_class)
    CHECK_BB(push(es, t->e->owner_class))
  if(!(t->flag & es->flag))
    CHECK_BB(es->func((void*)es->data, t->e->def))
  if(GET_FLAG(t, template))
    CHECK_BB(template_push_types(es->env, t->e->def->base.tmpl))
  env_push_type((void*)es->env, t);
  return GW_OK;
}

ANN m_bool envset_push(struct EnvSet *es, const Type t) {
  check(es, t);
  if(es->run)
    CHECK_BB(envset_push(es, t->e->owner_class))
  return GW_OK;
}

ANN void envset_pop(struct EnvSet *es, const Type t) {
  env_pop(es->env, es->scope);
  if(GET_FLAG(t, template))
    nspc_pop_type(es->env->gwion->mp, es->env->curr);
  if(t->e->owner_class)
    envset_pop(es, t->e->owner_class);
}

ANN m_bool envset_run(struct EnvSet *es, const Type t) {
  check(es, t);
  if(es->run)
    CHECK_BB(push(es, t->e->owner_class))
  const m_bool ret = es->func(es->data, t->e->def);
  if(es->run)
    envset_pop(es, t->e->owner_class);
  return ret;
}
