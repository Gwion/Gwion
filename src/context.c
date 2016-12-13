#include <stdlib.h>
#include <string.h>
#include "env.h"

void context_add_type(Context context, Type type, VM_Object obj)
{
  obj->ref = context->new_types;
  vector_append(context->new_types, (vtype)type);
}

void context_add_value(Context context, Value value, VM_Object obj)
{
  obj->ref = context->new_values;
  vector_append(context->new_values, (vtype)value);
}

void context_add_func(Context context, Func func, VM_Object obj)
{
  obj->ref = context->new_funcs;
  vector_append(context->new_funcs, (vtype)func);
}

Context new_Context(Ast prog, m_str filename)
{
  Context context = malloc(sizeof(struct Context_));
  context->nspc = new_NameSpace();
  context->nspc->filename = filename;
  context->tree = prog;
  context->filename = filename;
  context->nspc->name = filename;
  context->obj = new_VM_Object(e_context_obj);
  context->new_funcs = new_Vector();
  context->new_values = new_Vector();
  context->new_types = new_Vector();
  context->public_class_def = NULL;
  return context;
}

void free_Context(Context a)
{
  m_uint i;
//  free_NameSpace(a->nspc);
  rem_ref(a->nspc->obj, a->nspc);
  free_Vector(a->new_funcs);
  for(i = 0; i < vector_size(a->new_values); i++) {
    printf("lol\n");
  }
  free_Vector(a->new_values);
  // delete user class
//  for(i = 0; i < vector_size(a->new_types); i++) {
//    if(strcmp(a->filename, "global_context")) {
//      Type t = (Type)vector_at(a->new_types, i);
//      printf("%s\n", ((Type)vector_at(a->new_types, i))->name);
//free(t);
//rem_ref(t->obj, t);
//    }
//  }
  free_Vector(a->new_types);
  free(a);
}

int load_context(Context context, Env env)
{
  vector_append(env->contexts, (vtype)env->context);
  env->context = context;
  add_ref(env->context->obj);
  namespace_push_value(context->nspc);
  vector_append(env->nspc_stack, (vtype)env->curr);
  context->nspc->parent = env->curr;
  env->curr = context->nspc;
  return 1;
}

int unload_context(Context context, Env env)
{
  namespace_pop_value(env->context->nspc);
  env->curr = (NameSpace)vector_back(env->nspc_stack);
  vector_pop(env->nspc_stack);
//    rem_ref(env->context->obj, env->context);
  env->context = (Context)vector_back(env->contexts);
  vector_pop(env->contexts);
  return 1;
}
