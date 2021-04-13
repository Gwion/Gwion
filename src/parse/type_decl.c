#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "parse.h"

ANN static Type _option(const Env env, Type_Decl* td, const m_uint n) {
  struct Type_List_ tl = { .td=td };
  Type_Decl option_td = { .xid=insert_symbol("Option"), .types=&tl, .pos=td->pos };
  return !(n-1) ? known_type(env, &option_td) : _option(env, &option_td, n-1);
}

ANN static Type option(const Env env, Type_Decl* td) {
  const m_uint option = td->option;
  td->option = 0;
  const Type ret = _option(env, td, option);
  td->option = option;
  return ret;
}

ANN static inline Type ref(const Env env, Type_Decl* td) {
  struct Type_List_ tl = { .td=td };
  td->ref = 0;
  Type_Decl option_td = { .xid=insert_symbol("Ref"), .types=&tl, .pos=td->pos };
  const Type t = known_type(env, &option_td);
  td->ref = 1;
  return t;
}

ANN static Type resolve(const Env env, Type_Decl* td) {
  Type_Decl *last = td;
  while(last->next)
    last = last->next;
  Array_Sub array = last->array;
  DECL_OO(const Type, base, = find_type(env, td))
  const Context ctx = base->info->value->from->ctx;
  if(ctx && ctx->error)
    ERR_O(td->pos, _("type '%s' is invalid"), base->name)
  DECL_OO(const Type, type, = scan_type(env, base, td))
  const Type t = !td->ref ? type : ref(env, td);
  const Type ret = !td->option ? t : option(env, td);
  return !array ? ret: array_type(env, ret, array->depth);
}

ANN static inline void* type_unknown(const Env env, const Type_Decl* td) {
  env_err(env, td->pos, _("unknown type '%s'"), s_name(td->xid));
  return NULL;
}

ANN Type known_type(const Env env, Type_Decl* td) {
  return resolve(env, td) ?:type_unknown(env, td);
}
