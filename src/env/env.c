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
  return a;
}

Env new_env(MemPool p) {
  const Env env = (Env)xmalloc(sizeof(struct Env_));
  env->global_nspc = new_nspc(p, "global_nspc");
  env->context = NULL;
  env->scope = new_envscope(p);
  env_reset(env);
  return env;
}

ANN void env_reset(const Env env) {
  vector_clear(&env->scope->breaks);
  vector_clear(&env->scope->conts);
  vector_clear(&env->scope->nspc_stack);
  vector_add(&env->scope->nspc_stack, (vtype)env->global_nspc);
  vector_clear(&env->scope->class_stack);
  vector_add(&env->scope->class_stack, (vtype)NULL);
  env->curr = env->global_nspc;
  env->class_def = NULL;
  env->func = NULL;
  env->scope->depth = 0;
}

ANN void release_ctx(struct Env_Scope_ *a, struct Gwion_ *gwion) {
  const m_uint size = vector_size(&a->known_ctx);
  for(m_uint i = size + 1; --i;)
    context_remref((Context)vector_at(&a->known_ctx, i - 1), gwion);
}

ANN static void free_env_scope(struct Env_Scope_  *a, Gwion gwion) {
  release_ctx(a, gwion);
  vector_release(&a->known_ctx);
  vector_release(&a->nspc_stack);
  vector_release(&a->class_stack);
  vector_release(&a->breaks);
  vector_release(&a->conts);
  mp_free(gwion->mp, Env_Scope, a);
}

ANN void free_env(const Env a) {
  free_env_scope(a->scope, a->gwion);
  while(pop_global(a->gwion));
  xfree(a);
}

ANN2(1,3) m_uint env_push(const Env env, const Type type, const Nspc nspc) {
  const m_uint scope = env->scope->depth;
  vector_add(&env->scope->class_stack, (vtype)env->class_def);
  env->class_def = type;
  vector_add(&env->scope->nspc_stack, (vtype)env->curr);
  env->curr = nspc;
  env->scope->depth = 0;
  return scope;
}

ANN void env_pop(const Env env, const m_uint scope) {
  env->class_def = (Type)vector_pop(&env->scope->class_stack);
  env->curr = (Nspc)vector_pop(&env->scope->nspc_stack);
  env->scope->depth = scope;
}

ANN void env_add_type(const Env env, const Type type) {
  type->info->owner = env->curr;
  type->info->owner_class = env->class_def; // t owner_class ?
  const Symbol sym = insert_symbol(type->name);
  nspc_add_type_front(env->curr, sym, type);
  const Value v = mk_class(env, type);
  SET_FLAG(v, global);
  set_vflag(v, vflag_builtin);
  set_tflag(type, tflag_scan0 | tflag_scan1 | tflag_scan2 | tflag_check | tflag_emit);
  type->xid = ++env->scope->type_xid;
}

ANN m_bool type_engine_check_prog(const Env env, const Ast ast) {
  const Context ctx = new_context(env->gwion->mp, ast, env->name);
  env_reset(env);
  load_context(ctx, env);
  return traverse_ast(env, ast);
}

ANN m_bool type_engine_clean_prog(const Env env, m_bool *r) {
  const m_bool ret = (m_bool)*r;
  const Context ctx = env->context;
  if(ret > 0) //{
    nspc_commit(env->curr);
  if(ret > 0 || env->context->global)
    vector_add(&env->scope->known_ctx, (vtype)ctx);
  else //nspc_rollback(env->global_nspc);
    context_remref(ctx, env->gwion);
  unload_context(ctx, env);
  return ret;
}
