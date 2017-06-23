#include <stdlib.h>
#include "env.h"
#include "context.h"
#include "type.h"

Env new_env() {
  Env env = malloc(sizeof(struct Env_));
  env->global_context = new_context(NULL, "global_context");
  env->context = env->global_context;
  env->global_nspc = new_nspc("global_nspc", "global_nspc");
//  env->user_nspc = NULL;
  vector_init(&env->breaks);
  vector_init(&env->conts);
  vector_init(&env->contexts);
  vector_init(&env->class_stack);
  vector_init(&env->nspc_stack);
  map_init(&env->known_ctx);
  env->type_xid = te_last; // ????????
  env->do_type_xid = 0;
  INIT_OO(env, e_env_obj);
  env_reset(env);
  return env;
}

void env_reset(Env env) {
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

void free_env(Env a) {
  m_uint i;
  free(a->global_context->tree);
  for(i = 0; i < map_size(&a->known_ctx); i++) {
    Context ctx = (Context)map_at(&a->known_ctx, i);
    REM_REF(ctx);
  }
  vector_release(&a->contexts);
  map_release(&a->known_ctx);

  for(i = 0; i < vector_size(&a->nspc_stack); i++) {
    Nspc  nspc = (Nspc)vector_pop(&a->nspc_stack);
    REM_REF(nspc);
  }
  vector_release(&a->nspc_stack);
  vector_release(&a->class_stack);
  vector_release(&a->breaks);
  vector_release(&a->conts);
  free(a);
}

m_bool env_add_type(Env env, Type type) {
  if(type->name[0] != '@')
    CHECK_BB(name_valid(type->name));
  Type v_type = type_copy(env, &t_class);
  v_type->d.actual_type = type;
  INIT_OO(type, e_type_obj);
  SET_FLAG(type, ae_flag_builtin);
  nspc_add_type(env->curr, insert_symbol(type->name), type);
  Value v = new_value(v_type, type->name);
  SET_FLAG(v, ae_flag_checked | ae_flag_const | ae_flag_global | ae_flag_builtin);
  nspc_add_value(env->curr, insert_symbol(type->name), v);
  type->owner = env->curr;
  if(env->do_type_xid) {
    env->type_xid++;
    type->xid = env->type_xid;
  }
  return 1;
}

