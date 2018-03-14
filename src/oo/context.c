#include <stdlib.h>
#include "env.h"
#include "context.h"
#include "nspc.h"
#include "mpool.h"

POOL_HANDLE(Context, 16)
__attribute__((nonnull(2)))
Context new_context(const Ast prog, const m_str filename) {
  Context context = mp_alloc(Context);
  context->nspc = new_nspc(filename);
  context->tree = prog;
  context->filename = filename;
  INIT_OO(context, e_context_obj);
  return context;
}

ANN void free_context(Context a) {
  REM_REF(a->nspc);
  mp_free(Context, a);
}

ANN m_bool load_context(const Context context, const Env env) {
  vector_add(&env->contexts, (vtype)env->context);
  ADD_REF((env->context = context))
  vector_add(&env->nspc_stack, (vtype)env->curr);
  context->nspc->parent = env->curr;
  env->curr = context->nspc;
  return 1;
}

ANN m_bool unload_context(const Context context, const Env env) {
  if(context->label.ptr) {
    m_uint i;
    for(i = 0; i < map_size(&context->label); i++)
      free_map((Map)map_at(&context->label, i));
    map_release(&context->label);
  }
  env->curr = (Nspc)vector_pop(&env->nspc_stack);
  REM_REF(env->context);
  env->context = (Context)vector_pop(&env->contexts);
  return 1;
}

