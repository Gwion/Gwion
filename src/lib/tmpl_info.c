#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "traverse.h"
#include "parse.h"
#include "tmpl_info.h"

ANN static inline m_str tmpl_get(struct tmpl_info* info, m_str str) {
  const Type t = (Type)vector_at(&info->type, info->index);
  strcpy(str, t->name);
  return str += vector_at(&info->size, info->index);
}

ANN static void template_name(struct tmpl_info* info, m_str s) {
  m_str str = s;
  const m_uint size = info->index = vector_size(&info->type) -1;
  str = tmpl_get(info, str);
  *str++ = ':';
  *str++ = '[';
  for(info->index = 0; info->index < size; ++info->index) {
    str = tmpl_get(info, str);
    if(info->index < size - 1)
      *str++ = ',';
    else
      *str++ = ']';
  }
  *str = '\0';
}

ANN static inline size_t tmpl_set(struct tmpl_info* info, const Type t) {
  vector_add(&info->type, (vtype)t);
  const size_t len = strlen(t->name);
  vector_add(&info->size, len);
  return len;
}

ANN static ssize_t template_size(const Env env, struct tmpl_info* info) {
  ID_List base = info->list; // ???
  Type_List call = info->td->types;
  size_t size = 0;
  do {
    DECL_OB(const Type, t, = known_type(env, call->td))
    size += tmpl_set(info, t);
  } while((call = call->next) && (base = base->next) && ++size);
  size += tmpl_set(info, info->base);
  return size + 4;
}

ANEW ANN static Symbol template_id(const Env env, struct tmpl_info *const info) {
  vector_init(&info->type);
  vector_init(&info->size);
  ssize_t sz = template_size(env, info);
  char name[sz];
  if(sz > GW_ERROR)
    template_name(info, name);
  vector_release(&info->type);
  vector_release(&info->size);
  return sz > GW_ERROR ? insert_symbol(name) : NULL;
}

ANN static m_bool template_match(ID_List base, Type_List call) {
  while((call = call->next) && (base = base->next));
  return !call ? GW_OK : GW_ERROR;
}

ANN static Type _tmpl_exists(const Env env, const Symbol name) {
  if(env->class_def && name == insert_symbol(env->class_def->name))
     return env->class_def;
  return nspc_lookup_type1(env->curr, name);
}

ANN Type tmpl_exists(const Env env, struct tmpl_info *const info) {
  if(template_match(info->list, info->td->types) < 0) // invalid template
    ERR_N(info->td->pos, _("invalid template types number"))
  DECL_ON(const Symbol, name, = info->name = template_id(env, info))
  return _tmpl_exists(env, name);
}
