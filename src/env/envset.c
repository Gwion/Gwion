#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "template.h"

ANN static void check(struct EnvSet *es, const Type t) {
  const Vector v     = &es->env->scope->class_stack;
  Type         owner = t->info->value->from->owner_class;
  for (vtype i = vector_size(v) + 1; owner && --i;) {
    if (owner != (Type)vector_at(v, i - 1)) {
      es->run = 1;
      return;
    }
    owner = owner->info->value->from->owner_class;
  }
}

ANN static m_bool push(struct EnvSet *es, const Type t) {
  es->env->scope->depth  = 0;
  const Type owner_class = t->info->value->from->owner_class;
  if (owner_class)
    CHECK_BB(push(es, owner_class));
  else
    env_push(es->env, NULL,
             t->info->value->from->ctx ? t->info->value->from->ctx->nspc
                                       : es->env->curr);
  if (es->func && !(t->tflag & es->flag))
    CHECK_BB(es->func((void *)es->data, t));
  env_push_type((void *)es->env, t);
  if (tflag(t, tflag_tmpl))
    CHECK_BB(template_push_types(
        es->env, t->info->cdef->base.tmpl)); // incorrect templates?
  es->_ctx         = es->env->context;
  es->_filename    = es->env->name;
  es->env->context = t->info->value->from->ctx;
  es->env->name    = t->info->value->from->filename;
  return GW_OK;
}

ANN2(1, 3)
m_bool envset_push(struct EnvSet *es, const Type t, const Nspc nspc) {
  if (t) {
    check(es, t);
    return es->run ? push(es, t) : GW_OK;
  }
  if (nspc != es->env->curr) {
    env_push(es->env, NULL, nspc);
    es->run = 1;
  }
  return GW_OK;
}

ANN2(1) static void _envset_pop(struct EnvSet *es, const Type t) {
  if (safe_tflag(t, tflag_tmpl)) // might not be useful
    nspc_pop_type(es->env->gwion->mp, es->env->curr);
  env_pop(es->env, es->scope);
  if (!t) return;
  const Type owner_class = t->info->value->from->owner_class;
  if (owner_class)
    envset_pop(es, owner_class);
  else
    env_pop(es->env, es->scope);
}

ANN2(1) void envset_pop(struct EnvSet *es, const Type t) {
  _envset_pop(es, t);
  if (es->_ctx) es->env->context = es->_ctx;
  if (es->_filename) es->env->name = es->_filename;
}

ANN m_bool envset_run(struct EnvSet *es, const Type t) {
  check(es, t);
  const Type owner_class = t->info->value->from->owner_class;
  if (es->run) CHECK_BB(push(es, owner_class));
  const m_bool ret =
      t->info->cdef && !(t->tflag & es->flag) ? es->func(es->data, t) : GW_OK;
  if (es->run) envset_pop(es, owner_class);
  return ret;
}
