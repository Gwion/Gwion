#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"

ANN static void free_context(const Context a, Gwion gwion) {
  REM_REF(a->nspc, gwion)
  free_mstr(gwion->mp, a->name);
  mp_free(gwion->mp, Context, a);
}

ANN2(2) Context new_context(MemPool p, const Ast ast, const m_str str) {
  const Context context = mp_calloc(p, Context);
  context->name = mstrdup(p, str);
  context->nspc = new_nspc(p, context->name);
  context->tree = ast;
  context->ref = new_refcount(p, free_context);
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
  REM_REF(context, env->gwion)
  env->curr = (Nspc)vector_pop(&env->scope->nspc_stack);
}
