#include "env.h"
#include "lang.h"

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
  env->global_nspc = new_NameSpace(); // done filename
  env->global_nspc->name = "global_nspc";
  env->global_nspc->filename = "global_nspc";
  env->curr = env->global_nspc;
  add_ref(env->global_nspc->obj);
  env->breaks = new_Vector();
  env->conts = new_Vector();
  env->user_nspc = NULL;
//	env->binary_map = new_Unary_Operator_Map();
/*	env->binary_map = new_Binary_Operator_Map();*/
/*  env->clean = new_Vector();*/
  env->known_ctx = new_Map();
  env_reset(env);
  return env;
}

void env_reset(Env env)
{
  // TODO: release stack items?
  free_Vector(env->nspc_stack); env->nspc_stack = new_Vector();
  vector_append(env->nspc_stack, env->global_nspc);

  if(env->user_nspc)
    vector_append(env->nspc_stack, env->user_nspc);
  
  // TODO: release stack items?
  free_Vector(env->class_stack); env->class_stack = new_Vector();
  vector_append(env->class_stack, NULL);

  // should be at top level
//  assert( context == &global_context );
  // assign : TODO: release curr? class_def? func?
  // TODO: need another function, since this is called from constructor
  if(env->user_nspc)
      env->curr = env->user_nspc;
  else
      env->curr = env->global_nspc;
  env->class_def = NULL;
  env->func = NULL;
  env->class_scope = 0;
}
typedef void (*cleaner)();
void free_Env(Env a)
{
  free_NameSpace(a->curr);
  // TODO release content ?
  free_Vector(a->contexts);
  free_Vector(a->class_stack);
  free(a);
  a = NULL;
}
