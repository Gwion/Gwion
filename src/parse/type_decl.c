#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "nspc.h"
#include "type.h"
#include "vm.h"
#include "traverse.h"
#include "parse.h"

ANN Type type_decl_resolve(const Env env, const Type_Decl* td) {
  DECL_OO(const Type, base, = find_type(env, td->xid))
  DECL_OO(const Type, t, = scan_type(env, base, td))
  return !td->array ? t : array_type(env, t, td->array->depth);
}

struct td_info {
  Type_List tl;
  GwText text;
};

ANEW ANN static m_str td2str(const Env env, const Type_Decl* td);
ANN static void td_info_run(const Env env, struct td_info* info) {
  Type_List tl = info->tl;
  do {
    m_str name = td2str(env, tl->td);
    text_add(&info->text, name);
    free_mstr(env->gwion->mp, name);
    if(tl->next)
      text_add(&info->text, ",");
  } while((tl = tl->next));
}

ANEW ANN static m_str td2str(const Env env, const Type_Decl* td) {
  m_uint depth = td->array ? td->array->depth : 0;
  const size_t len = id_list_len(td->xid)  + depth * 2;
  const size_t cap = round2szint(len);
  struct td_info info = { td->types,
    { (m_str)mp_malloc2(env->gwion->mp, cap), cap, len, env->gwion->mp }
  };
  type_path(info.text.str, td->xid);
  while(depth--) { text_add(&info.text, "[]"); }
  Type_List tl = td->types;
  if(tl) {
    text_add(&info.text, "<");
    td_info_run(env, &info);
    text_add(&info.text, ">");
  }
  return info.text.str;
}

ANEW ANN m_str tl2str(const Env env, Type_List tl) {
  struct td_info info = { .tl=tl, { .mp=env->gwion->mp} };
  td_info_run(env, &info);
  return info.text.str;
}

ANN static inline void* type_unknown(const Env env, const ID_List id) {
  char path[id_list_len(id)];
  type_path(path, id);
  env_err(env, id->pos, _("unknown type '%s'"), path);
  did_you_mean_nspc(env->curr, s_name(id->xid));
  return NULL;
}

ANN static inline Type prim_ref(const Env env, const Type t, const Type_Decl* td) {
  if(GET_FLAG(td, ref) && isa(t, t_object) < 0 && isa(t, t_class) < 0)
    ERR_O(td->xid->pos, _("primitive types cannot be used as reference (@)...\n"))
  return t;
}

ANN Type known_type(const Env env, const Type_Decl* td) {
  if(!td->xid)
    return t_undefined;
  const Type t = type_decl_resolve(env, td);
  return t ? prim_ref(env, t, td) : type_unknown(env, td->xid);
}
