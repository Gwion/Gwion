#include <stdlib.h>
#include "env.h"

void context_add_type(Context context, Type type, VM_Object obj)
{
  obj->ref = context->new_types;
  vector_append(context->new_types, type);
}

void context_add_value(Context context, Value value, VM_Object obj)
{
  obj->ref = context->new_values;
  vector_append(context->new_values, value);
}

void context_add_func(Context context, Func func, VM_Object obj)
{
  obj->ref = context->new_funcs;
  vector_append(context->new_funcs, func);
}

Context new_Context(Ast prog, m_str filename)
{
  Context context = calloc(1, sizeof(struct Context_));
  context->nspc = new_NameSpace();
	context->nspc->filename = filename;
  context->tree = prog;
  context->filename = filename;
  context->nspc->name = filename;
  context->obj = new_VM_Object(e_context_obj);
  context->new_funcs = new_Vector();
  context->new_values = new_Vector();
  context->new_types = new_Vector();
/*	context->binary_map = new_Operator_Map();*/
//	context->binary_map = NULL;
  return context;
}

void free_Context(Context context)
{
  free_NameSpace(context->nspc);
  free_Vector(context->new_funcs);
  free_Vector(context->new_values);
  free_Vector(context->new_types);
  free(context);
  context = NULL;
}

int load_context(Context context, Env env)
{
  vector_append(env->contexts, env->context);
  env->context = context;
  add_ref(env->context->obj);
  namespace_push_value(context->nspc);
  vector_append(env->nspc_stack, env->curr);
  context->nspc->parent = env->curr;
  env->curr = context->nspc;
  return 1;
}

int unload_context(Context context, Env env)
{
  namespace_pop_value(env->context->nspc);
  env->curr = vector_back(env->nspc_stack);
  vector_pop(env->nspc_stack);
  rem_ref(env->context->obj, env->context);
  env->context = vector_back(env->contexts);
  vector_pop(env->contexts);
  return 1;
}
