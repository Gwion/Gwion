#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "value.h"
#include "type.h"

ANN m_uint union_push(const Env env, const Stmt_Union stmt) {
  const Type type = stmt->xid ? stmt->value->type : stmt->type_xid ?
    stmt->type : NULL;
  return type ? env_push_type(env, type) : GET_FLAG(stmt, global) ?
    env_push_global(env) : env->scope;
}

ANN void union_pop(const Env env, const Stmt_Union stmt, const m_uint scope) {
  if(stmt->xid || stmt->type_xid || GET_FLAG(stmt, global))
    env_pop(env, scope);
}
