#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "clean.h"

ANN void free_context(const Context a, Gwion gwion) {
  nspc_remref(a->nspc, gwion);
  free_mstr(gwion->mp, a->name);
  ast_cleaner(gwion, a->tree);
  mp_free(gwion->mp, Context, a);
}

ANN2(1,3) Context new_context(MemPool p, const Ast ast, const m_str str) {
  const Context context = mp_calloc(p, Context);
  context->name         = mstrdup(p, str);
  context->nspc         = new_nspc(p, context->name);
  context->tree         = ast;
  context->ref          = 1;
  return context;
}

ANN void load_context(const Context context, const Env env) {
  const Nspc global = new_nspc(env->gwion->mp, context->name);
  global->parent = env->global_nspc;
  env->global_nspc = global;
  context_addref((env->context = context));
  vector_add(&env->scope->nspc_stack, (vtype)env->curr);
  env->name             = context->name;
  context->nspc->parent = global;
  env->curr = context->nspc;
}

ANN void unload_context(const Context ctx, const Env env) {
  const Nspc global = env->global_nspc;
  context_remref(ctx, env->gwion);
  env->curr = (Nspc)vector_pop(&env->scope->nspc_stack);
  if(ctx->error) {
    nspc_remref(global, env->gwion);
    env->global_nspc = global->parent;
  }
  else if(!ctx->global)
    env->global_nspc = global->parent;
}
