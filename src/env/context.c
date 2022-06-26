#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "clean.h"

ANN void free_context(const Context a, Gwion gwion) {
  const Nspc global = a->nspc->parent;
  nspc_remref(a->nspc, gwion);
  if(a->error) nspc_remref(global, gwion);
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

ANN static void clean(const Nspc nspc, const Env env) {
  env->global_nspc = nspc->parent;
//  nspc_remref(nspc, env->gwion);
}

ANN void unload_context(const Context ctx, const Env env) {
  const Nspc global = ctx->nspc->parent;
  if(global != env->global_nspc) exit(3);
  context_remref(ctx, env->gwion);
  env->curr = (Nspc)vector_pop(&env->scope->nspc_stack);
  const Nspc user = (Nspc)vector_at(&env->scope->nspc_stack, 1);
  user->parent = (Nspc)vector_at(&env->scope->nspc_stack, 0);
  if(ctx->error) clean(global, env);
  else if(!ctx->global) {
    ctx->nspc->parent = global->parent;
    clean(global, env);
    nspc_remref(global, env->gwion);
  } else vector_set(&env->scope->nspc_stack, 2, (m_uint)global);
}
