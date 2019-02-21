#include <stdlib.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "operator.h"
#include "value.h"
#include "traverse.h"
#include "type.h"
#include "context.h"
#include "nspc.h"
#include "mpool.h"
#include "switch.h"

Env new_env() {
  const Env env = (Env)xmalloc(sizeof(struct Env_));
  env->global_nspc = new_nspc("global_nspc");
  env->context = NULL;
  vector_init(&env->breaks);
  vector_init(&env->conts);
  vector_init(&env->class_stack);
  vector_init(&env->nspc_stack);
  vector_init(&env->known_ctx);
  env->type_xid = 0;
  vector_init((Vector)&env->swi);
  map_init(&env->swi.map);

  env_reset(env);
  return env;
}

ANN void env_reset(const Env env) {
  vector_clear(&env->breaks);
  vector_clear(&env->conts);
  vector_clear(&env->nspc_stack);
  vector_add(&env->nspc_stack, (vtype)env->global_nspc);
  vector_clear(&env->class_stack);
  vector_add(&env->class_stack, (vtype)NULL);
  env->curr = env->global_nspc;
  env->class_def = NULL;
  env->func = NULL;
  env->scope = 0;
  switch_reset(env);
}

ANN void free_env(const Env a) {
  const m_uint size = vector_size(&a->known_ctx);
  for(m_uint i = size + 1; --i;)
    REM_REF((Context)vector_at(&a->known_ctx, i - 1));
  vector_release(&a->known_ctx);
  REM_REF(a->global_nspc);
  vector_release(&a->nspc_stack);
  vector_release(&a->class_stack);
  vector_release(&a->breaks);
  vector_release(&a->conts);
  switch_reset(a);
  switch_release(&a->swi);
  free(a);
}

ANN2(1,3) m_uint env_push(const Env env, const Type type, const Nspc nspc) {
  const m_uint scope = env->scope;
  vector_add(&env->class_stack, (vtype)env->class_def);
  env->class_def = type;
  vector_add(&env->nspc_stack, (vtype)env->curr);
  env->curr = nspc;
  env->scope = 0;
  return scope;
}

ANN void env_pop(const Env env, const m_uint scope) {
  env->class_def = (Type)vector_pop(&env->class_stack);
  env->curr = (Nspc)vector_pop(&env->nspc_stack);
  env->scope = scope;
}

ANN2(1,2) void env_add_value(const Env env, const m_str name, const Type type,
      const m_bool is_const, void* data) {
  const Value v = new_value(env->gwion, type, name);
  ae_flag flag = ae_flag_checked | ae_flag_global | ae_flag_builtin | (is_const ? ae_flag_const : 0);
  v->flag = flag;
  v->d.ptr = data;
  v->owner = env->global_nspc;
  nspc_add_value(env->global_nspc, insert_symbol(name), v);
}

ANN void env_add_type(const Env env, const Type type) {
  const Type v_type = type_copy(t_class);
  v_type->d.base_type = type;
  SET_FLAG(type, builtin);
  nspc_add_type(env->curr, insert_symbol(type->name), type);
  const Value v = new_value(env->gwion, v_type, type->name);
  SET_FLAG(v, checked | ae_flag_const | ae_flag_global | ae_flag_builtin);
  nspc_add_value(env->curr, insert_symbol(type->name), v);
  type->owner = env->curr;
  type->xid = ++env->type_xid;
}

ANN m_bool type_engine_check_prog(const Env env, const Ast ast) {
  const Context ctx = new_context(ast, env->name);
  env_reset(env);
  load_context(ctx, env);
  const m_bool ret = traverse_ast(env, ast);
  if(ret > 0) {
    nspc_commit(env->curr);
    vector_add(&env->known_ctx, (vtype)ctx);
  } else //nspc_rollback(env->global_nspc);
    REM_REF(ctx);
  unload_context(ctx, env);
  return ret;
}

ANN m_bool env_add_op(const Env env, const struct Op_Import* opi) {
  const Nspc nspc = env->curr;
  if(!nspc->op_map.ptr)
    map_init(&nspc->op_map);
  return add_op(nspc, opi);
}
