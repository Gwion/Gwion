#include <stdlib.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "value.h"
#include "traverse.h"
#include "type.h"
#include "context.h"
#include "nspc.h"
#include "vm.h"
#include "parse.h"

ANN Map env_label(const Env env) {
  return &env->context->lbls;
}

ANN Nspc env_nspc(const Env env) {
  return env->context->nspc;
}

#define GET(a,b) ((a) & (b)) == (b)
ANN m_bool env_access(const Env env, const ae_flag flag) {
  if(env->scope->depth) {
   if(GET(flag, ae_flag_global))
      ERR_B(0, "'global' can only be used at %s scope.",
          GET(flag, ae_flag_global) && !env->class_def ?
           "file" : "class")
  }
  if((GET(flag, ae_flag_static) || GET(flag, ae_flag_private) ||
      GET(flag, ae_flag_protect)) && (!env->class_def || env->scope->depth))
      ERR_B(0, "static/private/protect can only be used at class scope.")
  return GW_OK;
}

ANN void env_storage(const Env env, ae_flag* flag) {
  if(env->class_def && GET(*flag, ae_flag_global))
    *flag &= (uint)~ae_flag_global;
}

ANN static Type find_typeof(const Env env, ID_List path) {
  Value v = nspc_lookup_value1(env->curr, path->xid); // was lookup2
  Type t = actual_type(v->type);
  path = path->next;
  while(path) {
    CHECK_OO((v = find_value(t, path->xid)))
    t = v->type;
    path = path->next;
  }
  return v->type;
}

ANN Type find_type(const Env env, ID_List path) {
  if(path->ref)
    return find_typeof(env, path);
  Type type = nspc_lookup_type1(env->curr, path->xid);
  CHECK_OO(type)
  Nspc nspc = type->nspc;
  path = path->next;
  while(path) {
    const Symbol xid = path->xid;
    Type t = nspc_lookup_type1(nspc, xid);
    while(!t && type && type->parent) {
      t = nspc_lookup_type1(type->parent->nspc, xid); // was lookup2
      type = type->parent;
    }
    if(!t)
      ERR_O(path->pos, "...(cannot find class '%s' in nspc '%s')", s_name(xid), nspc->name)
    type = t;
    nspc = type->nspc;
    path = path->next;
  }
  return type;
}

ANN m_bool already_defined(const Env env, const Symbol s, const uint pos) {
  const Value v = nspc_lookup_value0(env->curr, s);
  return v ? err_msg(pos,
    "'%s' already declared as variable of type '%s'.", s_name(s), v->type->name) : GW_OK;
}

