#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"

ANN m_uint union_push(const Env env, const Union_Def udef) {
  const Type type = udef->xid ? udef->value->type : udef->type_xid ?
    udef->type : NULL;
  return type ? env_push_type(env, type) : GET_FLAG(udef, global) ?
    env_push_global(env) : env->scope->depth;
}

ANN void union_pop(const Env env, const Union_Def udef, const m_uint scope) {
  if(udef->xid || udef->type_xid || GET_FLAG(udef, global))
    env_pop(env, scope);
}
