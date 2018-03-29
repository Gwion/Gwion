#include <stdlib.h>
#include "defs.h"
#include "err_msg.h"
#include "absyn.h"
#include "context.h"
#include "value.h"
#include "traverse.h"
#include "type.h"
#include "env.h"

Env new_env() {
  const Env env = (Env)xmalloc(sizeof(struct Env_));
  env->global_nspc = new_nspc("global_nspc");
//  env->user_nspc = NULL;
  env->context = NULL;
  vector_init(&env->breaks);
  vector_init(&env->conts);
  vector_init(&env->contexts);
  vector_init(&env->class_stack);
  vector_init(&env->nspc_stack);
  vector_init(&env->known_ctx);
  env->type_xid = 0;
  env_reset(env);
  return env;
}

ANN void env_reset(const Env env) {
  vector_clear(&env->breaks);
  vector_clear(&env->conts);
  vector_clear(&env->nspc_stack);
  vector_add(&env->nspc_stack, (vtype)env->global_nspc);

//  if(env->user_nspc)
//    vector_add(env->nspc_stack, (vtype)env->user_nspc);

  vector_clear(&env->class_stack);
  vector_add(&env->class_stack, (vtype)NULL);
//  if(env->user_nspc)
//    env->curr = env->user_nspc;
//  else
  env->curr = env->global_nspc;
  env->class_def = NULL;
  env->func = NULL;
  env->class_scope = 0;
}

ANN void free_env(const Env a) {
  Context ctx;
  while((ctx = (Context)vector_pop(&a->known_ctx)))
    REM_REF(ctx)
  REM_REF(a->global_nspc);
  vector_release(&a->contexts);
  vector_release(&a->known_ctx);
  vector_release(&a->nspc_stack);
  vector_release(&a->class_stack);
  vector_release(&a->breaks);
  vector_release(&a->conts);
  free(a);
}

ANN m_bool env_push_class(const Env env, const Type type) {
  vector_add(&env->nspc_stack, (vtype)env->curr);
  env->curr = type->info;
  vector_add(&env->class_stack, (vtype)env->class_def);
  env->class_def = type;
  env->class_scope = 0;
  return 1;
}

ANN m_bool env_pop_class(const Env env) {
  env->class_def = (Type)vector_pop(&env->class_stack);
  env->curr = (Nspc)vector_pop(&env->nspc_stack);
  return 1;
}


ANN2(1,2) m_bool env_add_value(const Env env, const m_str name, const Type type,
      const m_bool is_const, void* data) {
  const Value v = new_value(type, name);
  ae_flag flag = ae_flag_checked | ae_flag_global | ae_flag_builtin | (is_const ? ae_flag_const : 0);
  v->flag = flag;
  v->d.ptr = data;
  v->owner = env->global_nspc;
  nspc_add_value(env->global_nspc, insert_symbol(name), v);
  return 1;
}

ANN m_bool env_add_type(const Env env, const Type type) {
  const Type v_type = type_copy(t_class);
  v_type->d.base_type = type;
  INIT_OO(type, e_type_obj);
  SET_FLAG(type, ae_flag_builtin);
  nspc_add_type(env->curr, insert_symbol(type->name), type);
  Value v = new_value(v_type, type->name);
  SET_FLAG(v, ae_flag_checked | ae_flag_const | ae_flag_global | ae_flag_builtin);
  nspc_add_value(env->curr, insert_symbol(type->name), v);
  type->owner = env->curr;
  type->xid = ++env->type_xid;
  return 1;
}

ANN Map env_label(const Env env) {
  return &env->context->label;
}

ANN Nspc env_nspc(const Env env) {
  return env->context->nspc;
}

ANN2(1) Class_Def env_class_def(const Env env, const Class_Def def) {
  if(def)
    env->context->public_class_def = def;
  return env->context ? env->context->public_class_def : NULL;
}

ANN m_bool type_engine_check_prog(const Env env, const Ast ast, const m_str filename) {
  const Context context = new_context(ast, filename);
  env_reset(env);
  CHECK_BB(load_context(context, env))
  const m_bool ret = traverse_ast(env, ast);
  if(ret > 0) {
    nspc_commit(env->curr);
    vector_add(&env->known_ctx, (vtype)context);
  } // else { nspc_rollback(env->global_nspc); }
  CHECK_BB(unload_context(context, env)) // no real need to check that
  if(ret < 0) {
    gw_err("in file '%s'\n", context->filename);
    free_ast(ast);
    REM_REF(context);
    free(filename);
  }
  return ret;
}

ANN m_bool env_add_op(const Env env, const struct Op_Import* opi) {
  const Nspc nspc = env->curr;
  if(!nspc->op_map.ptr)
    map_init(&nspc->op_map);
  CHECK_BB(add_op(nspc, opi))
  return 1;
}
