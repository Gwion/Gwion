#include <stdlib.h>
#include "defs.h"
#include "absyn.h"
#include "nspc.h"
#include "context.h"
#include "env.h"
#include "mpool.h"

ANN2(2) Context new_context(const Ast ast, const m_str str) {
  const Context context = mp_alloc(Context);
  context->nspc = new_nspc(str);
  context->tree = ast;
  context->name = str;
  INIT_OO(context, e_context_obj);
  return context;
}

ANN void free_context(const Context a) {
//if(a->nspc->obj.ref_count > 1)exit(2);
//  REM_REF(a->nspc);
  free_nspc(a->nspc);
  mp_free(Context, a);
}

ANN void load_context(const Context context, const Env env) {
  ADD_REF((env->context = context))
  vector_add(&env->nspc_stack, (vtype)env->curr);
  context->nspc->parent = env->curr;
  env->curr = context->nspc;
}

ANN void unload_context(const Context context, const Env env) {
  if(context->lbls.ptr) {
    LOOP_OPTIM
    for(m_uint i = 0; i < map_size(&context->lbls); i++)
      free_map((Map)map_at(&context->lbls, i));
    map_release(&context->lbls);
  }
  REM_REF(context);
  env->curr = (Nspc)vector_pop(&env->nspc_stack);
}
