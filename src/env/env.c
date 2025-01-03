#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "operator.h"
#include "traverse.h"
#include "vm.h"
#include "parse.h"

ANN static struct Env_Scope_ *new_envscope(MemPool p) {
  struct Env_Scope_ *a = mp_calloc(p, Env_Scope);
  vector_init(&a->breaks);
  vector_init(&a->conts);
  vector_init(&a->class_stack);
  vector_init(&a->nspc_stack);
  vector_init(&a->known_ctx);
  vector_init(&a->effects);
  return a;
}

Env new_env(MemPool p) {
  const Env env    = (Env)mp_malloc(p, Env);
  env->global_nspc = new_nspc(p, "global_nspc");
  env->context     = NULL;
  env->scope       = new_envscope(p);
  vector_add(&env->scope->nspc_stack, (vtype)env->global_nspc);
  env_reset(env);
  return env;
}

ANN void env_reset(const Env env) {
const Nspc nspc = (Nspc)vector_front(&env->scope->nspc_stack);
  vector_clear(&env->scope->breaks);
  vector_clear(&env->scope->conts);
  vector_clear(&env->scope->nspc_stack);
  vector_add(&env->scope->nspc_stack, (vtype)nspc);
  vector_add(&env->scope->nspc_stack, (vtype)env->global_nspc);
  vector_clear(&env->scope->class_stack);
  vector_add(&env->scope->class_stack, (vtype)NULL);
  vector_add(&env->scope->effects, 0);
  env->curr          = env->global_nspc;
  env->class_def     = NULL;
  env->func          = NULL;
  env->scope->depth  = 0;
  env->scope->in_try = false;
}

ANN void release_ctx(struct Env_Scope_ *a, struct Gwion_ *gwion) {
  const m_uint size = vector_size(&a->known_ctx);
  for (m_uint i = size + 1; --i;) {
    const Context ctx = (Context)vector_at(&a->known_ctx, i - 1);
    context_remref(ctx, gwion);
  }
}

ANN static void free_env_scope(struct Env_Scope_ *a, Gwion gwion) {
  vector_release(&a->known_ctx);
  vector_release(&a->nspc_stack);
  vector_release(&a->class_stack);
  vector_release(&a->breaks);
  vector_release(&a->conts);
  const Vector v = &a->effects;
  for (m_uint i = 0; i < vector_size(v); i++) {
    ScopeEffectList *eff = (ScopeEffectList*)vector_at(v, i);
    if (eff) free_scopeeffectlist(gwion->mp, eff);
  }
  vector_release(&a->effects);
  mp_free(gwion->mp, Env_Scope, a);
}

ANN void env_add_effect(const Env a, const Symbol effect, const loc_t loc) {
  const Vector   v = &a->scope->effects;
  ScopeEffectList *w = (ScopeEffectList*)vector_back(v);
  if (!w) {
    w = new_scopeeffectlist(a->gwion->mp, 0);
    VPTR(v, VLEN(v) - 1) = (vtype)w;
  }
  struct ScopeEffect eff = {effect, loc};
  // TODO: vector_at_ptr
  scopeeffectlist_add(a->gwion->mp, (ScopeEffectList**)&(VPTR(v, VLEN(v) - 1)) , eff);
}

ANN void free_env(const Env a) {
  pop_global(a->gwion);
  free_env_scope(a->scope, a->gwion);
  mp_free(a->gwion->mp, Env, a);
}

ANN2(1, 3) m_uint env_push(const Env env, const Type type, const Nspc nspc) {
  const m_uint scope = env->scope->depth;
  vector_add(&env->scope->class_stack, (vtype)env->class_def);
  env->class_def = type;
  vector_add(&env->scope->nspc_stack, (vtype)env->curr);
  vector_add(&env->scope->effects, 0);
  env->curr         = nspc;
  env->scope->depth = 0;
  return scope;
}

ANN void env_pop(const Env env, const m_uint scope) {
  env->class_def  = (Type)vector_pop(&env->scope->class_stack);
  env->curr       = (Nspc)vector_pop(&env->scope->nspc_stack);
  ScopeEffectList *const v = (ScopeEffectList*)vector_pop(&env->scope->effects);
  if (v) free_scopeeffectlist(env->gwion->mp, v);
  env->scope->depth = scope;
}

ANN void env_add_type(const Env env, const Type type, const loc_t loc) {
  const Symbol sym = insert_symbol(type->name);
  nspc_add_type_front(env->curr, sym, type);
  const Value v = mk_class(env, type, loc);
  SET_FLAG(v, global);
  set_vflag(v, vflag_builtin);
  set_tflag(type,
            tflag_scan0 | tflag_scan1 | tflag_scan2 | tflag_check | tflag_emit);
}
