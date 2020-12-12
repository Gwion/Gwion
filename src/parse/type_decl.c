#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "parse.h"

ANN static Type option(const Env env, Type_Decl* td) {
  struct Type_List_ tl = { .td=td };
  Type_Decl option_td = { .xid=insert_symbol("Option"), .types=&tl };
  UNSET_FLAG(td, optionnal);
  const Type ret = known_type(env, &option_td);
  SET_FLAG(td, optionnal);
  return ret;
}

ANN static Type resolve(const Env env, Type_Decl* td) {
  DECL_OO(const Type, base, = find_type(env, td))
  if(base->info->ctx && base->info->ctx->error)
    ERR_O(td_pos(td), _("type '%s' is invalid"), base->name)
  DECL_OO(const Type, t, = scan_type(env, base, td))
  const Type ret = !td->array ? t : array_type(env, t, td->array->depth);
  return !GET_FLAG(td, optionnal) ? ret : option(env, td);
}

struct td_info {
  Type_List tl;
  GwText text;
};

ANN static m_bool td_info_run(const Env env, struct td_info* info) {
  Type_List tl = info->tl;
  do {
    DECL_OB(const Type,  t, = known_type(env, tl->td))
    text_add(&info->text, t->name);
    if(tl->next)
      text_add(&info->text, ",");
  } while((tl = tl->next));
  return GW_OK;
}

ANEW ANN m_str tl2str(const Env env, Type_List tl) {
  struct td_info info = { .tl=tl, { .mp=env->gwion->mp} };
  CHECK_BO(td_info_run(env, &info))
  return info.text.str;
}

ANN static inline void* type_unknown(const Env env, const Type_Decl* td) {
  env_err(env, td->pos, _("unknown type '%s'"), s_name(td->xid));
  return NULL;
}

ANN Type known_type(const Env env, Type_Decl* td) {
  if(!td->xid)
    return env->gwion->type[et_undefined];
  return resolve(env, td) ?:type_unknown(env, td);
}
