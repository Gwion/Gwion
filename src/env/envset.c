#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "template.h"

ANN static void check(struct EnvSet *es, const Type t) {
  es->_ctx         = es->env->context;
  es->_filename    = es->env->name;
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

ANN static bool push(struct EnvSet *es, const Type t) {
  es->env->scope->depth  = 0;
  const Type owner_class = t->info->value->from->owner_class;
  if (owner_class)
    CHECK_B(push(es, owner_class));
  else
    env_push_nspc(es->env,
             t->info->value->from->ctx ? t->info->value->from->ctx->nspc
                                       : es->env->curr);
  env_push_type((void *)es->env, t); // do not push if is a function?
  if (tflag(t, tflag_tmpl)) {
    const Tmpl *tmpl = get_tmpl(t);
    CHECK_B(template_push_types(es->env, tmpl));
  }
  return true;
}

ANN2(1, 3)
bool envset_push(struct EnvSet *es, const Type t, const Nspc nspc) {
  if (t) {
    check(es, t);
//    if(es->run && type_global(es->env, t))
//      env_push_global(es->env);
    return es->run ? push(es, t) : true;
  }
  if (nspc != es->env->curr) {
    env_push_nspc(es->env, nspc);
    es->run = 1;
  }
  return true;
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
  if(es->run) _envset_pop(es, t);
//  if(t && type_global(es->env, t))
//    env_pop(es->env, es->scope);
  if (es->_ctx) es->env->context = es->_ctx;
  if (es->_filename)  es->env->name = es->_filename;
}

ANN bool envset_run(struct EnvSet *es, const Type t) {
  check(es, t);
  const Type owner_class = t->info->value->from->owner_class;
  if (es->run) CHECK_B(push(es, owner_class));
  es->env->context = t->info->value->from->ctx;
  es->env->name    = t->info->value->from->filename;
  const bool ret =
      t->info->cdef && !(t->tflag & es->flag) ? es->func(es->data, t) : true;
  envset_pop(es, owner_class);
  return ret;
}
