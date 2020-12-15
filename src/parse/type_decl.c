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

ANN static Type resolve(const Env env, Type_Decl* td) {
  DECL_OO(const Type, base, = find_type(env, td))
  if(base->info->ctx && base->info->ctx->error)
    ERR_O(td_pos(td), _("type '%s' is invalid"), base->name)
  DECL_OO(const Type, t, = scan_type(env, base, td))
  const Type ret = !td->option ? t : option(env, td);
  return !td->array ? ret : array_type(env, ret, td->array->depth);
}

struct td_info {
  Type_List tl;
  GwText text;
};

ANN static void td_fullname(const Env env, GwText *text, const Type t) {
  const Type owner = t->info->owner_class;
  if(owner) {
    td_fullname(env, text, owner);
    text_add(text, ".");
  }
  text_add(text, t->name);
}

ANN static m_bool td_info_run(const Env env, struct td_info* info) {
  Type_List tl = info->tl;
  do {
    DECL_OB(const Type,  t, = known_type(env, tl->td))
    td_fullname(env, &info->text, t);
    if(tl->next)
      text_add(&info->text, ",");
  } while((tl = tl->next));
  return GW_OK;
}

ANEW ANN m_str type2str(const Env env, const Type t) {
  GwText text = { .mp=env->gwion->mp };
  if(t->info->owner_class)
    td_fullname(env, &text, t->info->owner_class);
  text_add(&text, t->name);
  return text.str;
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
