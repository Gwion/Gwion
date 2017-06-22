#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "context.h"
#include "type.h"
#include "func.h"

Context new_context(Ast prog, m_str filename) {
  Context context = malloc(sizeof(struct Context_));
  context->nspc = new_nspc(filename, filename);
  context->tree = prog;
  context->filename = filename;
  vector_init(&context->new_funcs);
  vector_init(&context->new_values);
  context->public_class_def = NULL;
  INIT_OO(context, e_context_obj);
  return context;
}

void free_context(Context a) {
  vtype i;
/*
  for(i = 0; i < vector_size(&a->new_funcs); i++) {
    Func f = (Func)vector_at(&a->new_funcs, i);
    if(GET_FLAG(f->def, ae_flag_op)) {
      REM_REF(f->value_ref->m_type);
      REM_REF(f->value_ref);
      REM_REF(f);
    }
  }
*/
  vector_release(&a->new_funcs);
/*
  for(i = vector_size(&a->new_values) + 1; --i;) { // only stores new class_def
    Value v = (Value)vector_at(&a->new_values, i - 1);
    REM_REF(v->m_type);
    REM_REF(v);
  }
*/
  vector_release(&a->new_values);
  REM_REF(a->nspc);
  free(a);
}

m_bool load_context(Context context, Env env) {
  vector_add(&env->contexts, (vtype)env->context);
  env->context = context;
  ADD_REF(env->context);
//  nspc_push_value(context->nspc);
  vector_add(&env->nspc_stack, (vtype)env->curr);
  context->nspc->parent = env->curr;
  env->curr = context->nspc;
  return 1;
}

m_bool unload_context(Context context, Env env) {
//  nspc_pop_value(env->context->nspc);
  env->curr = (Nspc)vector_pop(&env->nspc_stack);
  REM_REF(env->context);
  env->context = (Context)vector_pop(&env->contexts);
  return 1;
}
