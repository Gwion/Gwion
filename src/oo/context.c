#include <stdlib.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "nspc.h"
#include "vm.h"
#include "gwion.h"
#include "context.h"

ANN static void free_context(const Context a, Gwion gwion) {
  REM_REF(a->nspc, gwion)
  mp_free(gwion->mp, Context, a);
}

ANN2(2) Context new_context(MemPool p, const Ast ast, const m_str str) {
  const Context context = mp_alloc(p, Context);
  context->nspc = new_nspc(p, str);
  context->tree = ast;
  context->name = str;
  INIT_OO(p, context, free_context);
  return context;
}

ANN void load_context(const Context context, const Env env) {
  ADD_REF((env->context = context))
  vector_add(&env->scope->nspc_stack, (vtype)env->curr);
  context->nspc->parent = env->curr;
  env->curr = context->nspc;
}

ANN void unload_context(const Context context, const Env env) {
  if(context->lbls.ptr) {
    LOOP_OPTIM
    for(m_uint i = 0; i < map_size(&context->lbls); i++)
      free_map(env->gwion->mp, (Map)map_at(&context->lbls, i));
    map_release(&context->lbls);
  }
  REM_REF(context, env->gwion);
  env->curr = (Nspc)vector_pop(&env->scope->nspc_stack);
}
