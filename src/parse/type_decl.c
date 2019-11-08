#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "parse.h"

#define STR_NONNULL ":nonnull"
#define STRLEN_NONNULL strlen(STR_NONNULL)

ANN Type type_nonnull(const Env env, const Type base) {
  char c[strlen(base->name) + 9];
  sprintf(c, "%s%s", base->name, STR_NONNULL);
  const Symbol sym = insert_symbol(c);
  const Type exist = nspc_lookup_type1(base->e->owner, sym);
  if(exist)
    return exist;
  const Type t = type_copy(env->gwion->mp, base);
  t->e->parent = base;
  if(t->nspc)
    ADD_REF(t->nspc)
  t->name = s_name(sym);
  t->flag = base->flag;
  SET_FLAG(t, nonnull);
  nspc_add_type_front(t->e->owner, sym, t);
  return t;
}

ANN Type type_decl_resolve(const Env env, const Type_Decl* td) {
  DECL_OO(const Type, base, = find_type(env, td->xid))
  if(base->e->ctx && base->e->ctx->error)
    ERR_O(td_pos(td), _("type '%s' is invalid"), base->name)
  DECL_OO(const Type, t, = scan_type(env, base, td))
  const Type ret = !td->array ? t : array_type(env, t, td->array->depth);
  if(GET_FLAG(td, nonnull)) {
    if(isa(ret, env->gwion->type[et_void]) > 0)
      ERR_O(td_pos(td), _("void types can't be nonnull."))
    if(isa(ret, env->gwion->type[et_object]) < 0 && isa(ret, env->gwion->type[et_fptr]) < 0)
      return ret;
    return type_nonnull(env, ret);
  }
  return ret;
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

ANN static inline void* type_unknown(const Env env, const ID_List id) {
  char path[id_list_len(id)];
  type_path(path, id);
  env_err(env, id->pos, _("unknown type '%s'"), path);
  did_you_mean_nspc(env->curr, s_name(id->xid));
  return NULL;
}

ANN Type known_type(const Env env, const Type_Decl* td) {
  if(!td->xid)
    return env->gwion->type[et_undefined];
  return type_decl_resolve(env, td) ?:type_unknown(env, td->xid);
}
