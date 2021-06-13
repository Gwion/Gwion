#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "parse.h"

ANN static Type _option(const Env env, Type_Decl *td, const uint8_t n) {
  struct Type_List_ tl  = {.td = td};
  Type_Decl         tmp = {
      .xid = insert_symbol("Option"), .types = &tl, .pos = td->pos};
  return !(n - 1) ? known_type(env, &tmp) : _option(env, &tmp, n - 1);
}

ANN static Type option(const Env env, Type_Decl *td) {
  const uint8_t option = td->option;
  td->option           = 0;
  const Type t         = _option(env, td, option);
  td->option           = option;
  return t;
}

ANN static Type _ref(const Env env, Type_Decl *td) {
  struct Type_List_ tl = {.td = td};
  Type_Decl tmp = {.xid = insert_symbol("Ref"), .types = &tl, .pos = td->pos};
  return known_type(env, &tmp);
}

ANN static inline Type ref(const Env env, Type_Decl *td) {
  td->ref      = false;
  const Type t = _ref(env, td);
  td->ref      = true;
  return t;
}

ANN static inline Type find(const Env env, Type_Decl *td) {
  if (!td->fptr) return find_type(env, td);
  if (!td->fptr->type) CHECK_BO(traverse_fptr_def(env, td->fptr));
  return td->fptr->type;
}

ANN static inline Type find1(const Env env, const Type base, Type_Decl *td) {
  if (!td->fptr) return scan_type(env, base, td);
  if (!td->fptr->type) {
    CHECK_BO(scan0_fptr_def(env, td->fptr));
    CHECK_BO(traverse_fptr_def(env, td->fptr));
  }
  return td->fptr->type;
}

ANN static Type resolve(const Env env, Type_Decl *td) {
  Type_Decl *last = td;
  while (last->next) last = last->next;
  DECL_OO(const Type, base, = find(env, td));
  const Context ctx = base->info->value->from->ctx;
  if (ctx && ctx->error) ERR_O(td->pos, _("type '%s' is invalid"), base->name)
  DECL_OO(const Type, type, = find1(env, base, td));
  const Type      t     = !td->ref ? type : ref(env, td);
  const Type      ret   = !td->option ? t : option(env, td);
  const Array_Sub array = last->array;
  return !array ? ret : array_type(env, ret, array->depth);
}

ANN static inline void *type_unknown(const Env env, const Type_Decl *td) {
  env_err(env, td->pos, _("unknown type '%s'"), s_name(td->xid));
  return NULL;
}

ANN Type known_type(const Env env, Type_Decl *td) {
  return resolve(env, td) ?: type_unknown(env, td);
}
