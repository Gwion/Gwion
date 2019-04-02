#include <stdlib.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "gwion.h"
#include "operator.h"
#include "value.h"
#include "traverse.h"
#include "type.h"
#include "context.h"
#include "nspc.h"
#include "mpool.h"
#include "switch.h"
#include "vm.h"
#include "parse.h"

ANN static struct Env_Scope_ *new_scope(MemPool p) {
  struct Env_Scope_ *a = mp_alloc(p, Env_Scope);
  vector_init(&a->breaks);
  vector_init(&a->conts);
  vector_init(&a->class_stack);
  vector_init(&a->nspc_stack);
  vector_init(&a->known_ctx);
  vector_init((Vector)&a->swi);
  map_init(&a->swi.map);
  return a;
}

Env new_env(MemPool p) {
  const Env env = (Env)xmalloc(sizeof(struct Env_));
  env->global_nspc = new_nspc(p, "global_nspc");
  env->context = NULL;
  env->scope = new_scope(p);
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
  switch_reset(env);
}

ANN static void free_env_scope(struct Env_Scope_  *a, Gwion gwion) {
  const m_uint size = vector_size(&a->known_ctx);
  for(m_uint i = size + 1; --i;)
    REM_REF((Context)vector_at(&a->known_ctx, i - 1), gwion);
  vector_release(&a->known_ctx);
  vector_release(&a->nspc_stack);
  vector_release(&a->class_stack);
  vector_release(&a->breaks);
  vector_release(&a->conts);
  switch_release(&a->swi);
  mp_free(gwion->p, Env_Scope, a);
}

ANN void free_env(const Env a) {
  switch_reset(a);
  free_env_scope(a->scope, a->gwion);
  REM_REF(a->global_nspc, a->gwion);
  free(a);
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
  const Type v_type = type_copy(env->gwion->p, t_class);
  v_type->d.base_type = type;
  SET_FLAG(type, builtin);
  nspc_add_type(env->curr, insert_symbol(type->name), type);
  const Value v = new_value(env->gwion->p, v_type, type->name);
  SET_FLAG(v, checked | ae_flag_const | ae_flag_global | ae_flag_builtin);
  nspc_add_value(env->curr, insert_symbol(type->name), v);
  type->owner = env->curr;
  type->xid = ++env->scope->type_xid;
}

ANN m_bool type_engine_check_prog(const Env env, const Ast ast) {
  const Context ctx = new_context(env->gwion->p, ast, env->name);
  env_reset(env);
  load_context(ctx, env);
  const m_bool ret = traverse_ast(env, ast);
  if(ret > 0) {
    nspc_commit(env->curr);
    vector_add(&env->scope->known_ctx, (vtype)ctx);
  } else //nspc_rollback(env->global_nspc);
    REM_REF(ctx, env->gwion);
  unload_context(ctx, env);
  return ret;
}

ANN m_bool env_add_op(const Env env, const struct Op_Import* opi) {
  const Nspc nspc = env->curr;
  if(!nspc->info->op_map.ptr)
    map_init(&nspc->info->op_map);
  return add_op(env->gwion, nspc, opi);
}
