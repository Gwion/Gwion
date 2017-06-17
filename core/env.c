#include <stdlib.h>
#include "env.h"
#include "context.h"
#include "type.h"

Env new_env() {
  Env env = calloc(1, sizeof(struct Env_));
  env->global_context = new_context(NULL, "global_context");
  env->context = env->global_context;
  env->contexts = new_vector();
  env->class_stack = new_vector();
  env->nspc_stack = new_vector();
  env->class_scope = 0;
  env->global_nspc = new_nspc("global_nspc", "global_nspc");
  env->curr = env->global_nspc;
  env->breaks = new_vector();
  env->conts = new_vector();
//  env->user_nspc = NULL;
  env->known_ctx = new_map();
  env_reset(env);
  env->type_xid = te_last;
  INIT_OO(env, e_env_obj);
  return env;
}

void env_reset(Env env) {
  vector_clear(env->nspc_stack);
  vector_add(env->nspc_stack, (vtype)env->global_nspc);

//  if(env->user_nspc)
//    vector_add(env->nspc_stack, (vtype)env->user_nspc);

  vector_clear(env->class_stack);
  vector_add(env->class_stack, (vtype)NULL);

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
  for(i = 0; i < map_size(a->known_ctx); i++) {
    Context ctx = (Context)map_at(a->known_ctx, i);
    REM_REF(ctx);
  }
  free_vector(a->contexts);
  free_map(a->known_ctx);

  for(i = 0; i < vector_size(a->nspc_stack); i++) {
    Nspc  nspc = (Nspc)vector_pop(a->nspc_stack);
    REM_REF(nspc);
  }
  free_vector(a->nspc_stack);
  free_vector(a->class_stack);
  free_vector(a->breaks);
  free_vector(a->conts);
  free(a);
}

m_bool env_add_type(Env env, Type type) {
  if(type->name[0] != '@')
    CHECK_BB(name_valid(type->name));
  Type v_type = type_copy(env, &t_class);
  v_type->actual_type = type;
  INIT_OO(type, e_type_obj);
  SET_FLAG(type, ae_flag_builtin);
  nspc_add_type(env->curr, insert_symbol(type->name), type);
  Value v = new_value(v_type, type->name);
  SET_FLAG(v, ae_flag_checked | ae_flag_const | ae_flag_global);
  nspc_add_value(env->curr, insert_symbol(type->name), v);
  type->owner = env->curr;
  if(env->do_type_xid) {
    env->type_xid++;
    type->xid = env->type_xid;
  }
  return 1;
}

