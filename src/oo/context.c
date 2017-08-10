#include <stdlib.h>
#include "absyn.h"
#include "map.h"
#include "env.h"
#include "context.h"
#include "nspc.h"

Context new_context(Ast prog, m_str filename) {
  Context context = malloc(sizeof(struct Context_));
  context->nspc = new_nspc(filename, filename);
  context->tree = prog;
  context->filename = filename;
  context->public_class_def = NULL;
  context->label.ptr = NULL;
  INIT_OO(context, e_context_obj);
  return context;
}

void free_context(Context a) {
  REM_REF(a->nspc);
  free(a);
}

m_bool load_context(Context context, Env env) {
  vector_add(&env->contexts, (vtype)env->context);
  env->context = context;
  ADD_REF(env->context);
  vector_add(&env->nspc_stack, (vtype)env->curr);
  context->nspc->parent = env->curr;
  env->curr = context->nspc;
  return 1;
}

m_bool unload_context(Context context, Env env) {
  env->curr = (Nspc)vector_pop(&env->nspc_stack);
  REM_REF(env->context);
  env->context = (Context)vector_pop(&env->contexts);
  return 1;
}

