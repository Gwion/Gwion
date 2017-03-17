#include <stdlib.h>
#include "env.h"
#include "context.h"
#include "namespace.h"

Env new_Env()
{
  Env env = calloc(1, sizeof(struct Env_));
  env->global_context = new_Context(NULL, "global_context");
  add_ref(env->global_context->obj);
  env->context = env->global_context;
  env->contexts = new_Vector();
  env->class_stack = new_Vector();
  env->nspc_stack = new_Vector();
  env->class_scope = 0;
  env->obj = new_VM_Object(e_env_obj);
  env->global_nspc = new_NameSpace();
  env->global_nspc->name = "global_nspc";
  env->global_nspc->filename = "global_nspc";
  env->curr = env->global_nspc;
  add_ref(env->global_nspc->obj);
  env->breaks = new_Vector();
  env->conts = new_Vector();
//  env->user_nspc = NULL;
  env->known_ctx = new_Map();
  env_reset(env);
  return env;
}

void env_reset(Env env)
{
  // TODO: release stack items?
  free_Vector(env->nspc_stack);
  env->nspc_stack = new_Vector();
  vector_append(env->nspc_stack, (vtype)env->global_nspc);

//  if(env->user_nspc)
//    vector_append(env->nspc_stack, (vtype)env->user_nspc);

  free_Vector(env->class_stack);
  env->class_stack = new_Vector();
  vector_append(env->class_stack, (vtype)NULL);

  // should be at top level
//  if(env->user_nspc)
//    env->curr = env->user_nspc;
//  else
  env->curr = env->global_nspc;
  env->class_def = NULL;
  env->func = NULL;
  env->class_scope = 0;
}
//typedef void (*cleaner)();

void free_Env(Env a)
{
  m_uint i;
  free(a->global_context->tree);
  for(i = 0; i < map_size(a->known_ctx); i++) {
    Context ctx = (Context)map_at(a->known_ctx, i);
    rem_ref(ctx->obj, ctx);
  }
  free_Vector(a->contexts);
  free_Map(a->known_ctx);

  for(i = 0; i < vector_size(a->nspc_stack); i++) {
    NameSpace  nspc = (NameSpace)vector_back(a->nspc_stack);
    vector_pop(a->nspc_stack);
    rem_ref(nspc->obj, nspc);
  }
  free_Vector(a->nspc_stack);
  free_Vector(a->class_stack);
  free_Vector(a->breaks);
  free_Vector(a->conts);
  free(a);
}
