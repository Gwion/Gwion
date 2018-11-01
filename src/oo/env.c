#include <stdlib.h>
#include "defs.h"
#include "err_msg.h"
#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "oo.h"
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
  const m_uint size = vector_size(&a->known_ctx);
  for(m_uint i = 0; i < size; i++)
    REM_REF((Context)vector_at(&a->known_ctx, i));
  REM_REF(a->global_nspc);
  vector_release(&a->known_ctx);
  vector_release(&a->nspc_stack);
  vector_release(&a->class_stack);
  vector_release(&a->breaks);
  vector_release(&a->conts);
  free(a);
}

ANN2(1,3,4) void env_push(const Env env, const Type type,
    const Nspc nspc, m_uint* class_scope) {
  vector_add(&env->class_stack, (vtype)env->class_def);
  env->class_def = type;
  vector_add(&env->nspc_stack, (vtype)env->curr);
  env->curr = nspc;
  *class_scope = env->class_scope;
  env->class_scope = 0;
}

ANN void env_pop(const Env env, const m_uint class_scope) {
  env->class_def = (Type)vector_pop(&env->class_stack);
  env->curr = (Nspc)vector_pop(&env->nspc_stack);
  env->class_scope = class_scope;
}

ANN2(1,2) void env_add_value(const Env env, const m_str name, const Type type,
      const m_bool is_const, void* data) {
  const Value v = new_value(type, name);
  ae_flag flag = ae_flag_checked | ae_flag_global | ae_flag_builtin | (is_const ? ae_flag_const : 0);
  v->flag = flag;
  v->d.ptr = data;
  v->owner = env->global_nspc;
  nspc_add_value(env->global_nspc, insert_symbol(name), v);
}

ANN void env_add_type(const Env env, const Type type) {
  const Type v_type = type_copy(t_class);
  v_type->d.base_type = type;
  SET_FLAG(type, ae_flag_builtin);
  nspc_add_type(env->curr, insert_symbol(type->name), type);
  const Value v = new_value(v_type, type->name);
  SET_FLAG(v, ae_flag_checked | ae_flag_const | ae_flag_global | ae_flag_builtin);
  nspc_add_value(env->curr, insert_symbol(type->name), v);
  type->owner = env->curr;
  type->xid = ++env->type_xid;
}

ANN Map env_label(const Env env) {
  return &env->context->lbls;
}

ANN Nspc env_nspc(const Env env) {
  return env->context->nspc;
}

ANN m_bool type_engine_check_prog(const Env env, const Ast ast, const m_str str) {
  const Context ctx = new_context(ast, str);
  env_reset(env);
  load_context(ctx, env);
  const m_bool ret = traverse_ast(env, ast);
  if(ret > 0) {
    nspc_commit(env->curr);
    vector_add(&env->known_ctx, (vtype)ctx);
  } // else { nspc_rollback(env->global_nspc); }
  unload_context(ctx, env);
  if(ret < 0) {
    gw_err("in file '%s'\n", str);
    free_ast(ast);
    REM_REF(ctx);
    free(str);
  }
  return ret;
}

ANN m_bool env_add_op(const Env env, const struct Op_Import* opi) {
  const Nspc nspc = env->curr;
  if(!nspc->op_map.ptr)
    map_init(&nspc->op_map);
  return add_op(nspc, opi);
}


#define GET(a,b) ((a) & (b)) == (b)
ANN m_bool env_access(const Env env, const ae_flag flag) {
  if(env->class_scope) {
   if(GET(flag, ae_flag_global))
      ERR_B(0, "'global' can only be used at %s scope.",
          GET(flag, ae_flag_global) && !env->class_def ?
           "file" : "class")
  }
  if((GET(flag, ae_flag_static) || GET(flag, ae_flag_private) ||
      GET(flag, ae_flag_protect)) && (!env->class_def || env->class_scope))
      ERR_B(0, "static/private/protect can only be used at class scope.")
  return 1;
}

ANN void env_storage(const Env env, ae_flag* flag) {
  if(env->class_def && GET(*flag, ae_flag_global))
    *flag &= ~ae_flag_global;
}
