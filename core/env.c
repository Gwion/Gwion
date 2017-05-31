#include <stdlib.h>
#include "env.h"
#include "context.h"
#include "namespace.h"

Env new_env() {
  Env env = calloc(1, sizeof(struct Env_));
  env->global_context = new_context(NULL, "global_context");
  env->context = env->global_context;
  env->contexts = new_vector();
  env->class_stack = new_vector();
  env->nspc_stack = new_vector();
  env->class_scope = 0;
  env->global_nspc = new_NameSpace();
  env->global_nspc->name = "global_nspc";
  env->global_nspc->filename = "global_nspc";
  env->curr = env->global_nspc;
  env->breaks = new_vector();
  env->conts = new_vector();
//  env->user_nspc = NULL;
  env->known_ctx = new_Map();
  env_reset(env);
  INIT_OO(env, e_env_obj);
  return env;
}

void env_reset(Env env) {
  free_vector(env->nspc_stack);
  env->nspc_stack = new_vector();
  vector_append(env->nspc_stack, (vtype)env->global_nspc);

//  if(env->user_nspc)
//    vector_append(env->nspc_stack, (vtype)env->user_nspc);

  free_vector(env->class_stack);
  env->class_stack = new_vector();
  vector_append(env->class_stack, (vtype)NULL);

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
  free_Map(a->known_ctx);

  for(i = 0; i < vector_size(a->nspc_stack); i++) {
    NameSpace  nspc = (NameSpace)vector_pop(a->nspc_stack);
    REM_REF(nspc);
  }
  free_vector(a->nspc_stack);
  free_vector(a->class_stack);
  free_vector(a->breaks);
  free_vector(a->conts);
  free(a);
}
