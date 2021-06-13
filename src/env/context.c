#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"

ANN void free_context(const Context a, Gwion gwion) {
  nspc_remref(a->nspc, gwion);
  free_mstr(gwion->mp, a->name);
  mp_free(gwion->mp, Context, a);
}

ANN2(2) Context new_context(MemPool p, const Ast ast, const m_str str) {
  const Context context = mp_calloc(p, Context);
  context->name         = mstrdup(p, str);
  context->nspc         = new_nspc(p, context->name);
  context->tree         = ast;
  context->ref          = 1;
  return context;
}

ANN void load_context(const Context context, const Env env) {
  context_addref((env->context = context));
  vector_add(&env->scope->nspc_stack, (vtype)env->curr);
  env->name             = context->name;
  context->nspc->parent = env->curr;
  env->curr             = context->nspc;
}

ANN void unload_context(const Context context, const Env env) {
  context_remref(context, env->gwion);
  env->curr = (Nspc)vector_pop(&env->scope->nspc_stack);
}
