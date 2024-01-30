#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "traverse.h"
#include "parse.h"
#include "object.h"
#include "instr.h"
#include "import.h"
#include "tmpl_info.h"

ANN static inline m_str tmpl_get(struct tmpl_info *info, m_str str) {
  const m_str tmp = (m_str)vector_at(&info->type, info->index);
  strcpy(str, tmp);
  return str + vector_at(&info->size, info->index);
}

ANN static void template_name(struct tmpl_info *info, m_str s) {
  m_str        str  = s;
  const m_uint size = info->index = vector_size(&info->type) - 1;
  str                             = tmpl_get(info, str);
  *str++                          = ':';
  *str++                          = '[';
  for (info->index = 0; info->index < size; ++info->index) {
    str = tmpl_get(info, str);
    if (info->index < size - 1)
      *str++ = ',';
    else
      *str++ = ']';
  }
  *str = '\0';
}

ANN static inline size_t tmpl_set(struct tmpl_info *info, const m_str str) {
  vector_add(&info->type, (vtype)str);
  const size_t len = strlen(str);
  vector_add(&info->size, len);
  return len;
}

ANN static ssize_t template_size(const Env env, struct tmpl_info *info) {
  DECL_OB(const m_str, str,
          = tl2str(env->gwion, info->td->types, info->td->tag.loc));
  const size_t tmpl_sz = tmpl_set(info, str);
  const m_str  base    = !is_func(env->gwion, info->base)
   ? mstrdup(env->gwion->mp, info->base->name)
   : type2str(env->gwion, info->base, info->td->tag.loc);
  return tmpl_sz + tmpl_set(info, base) + 4;
}

ANEW ANN static Symbol _template_id(const Env env, struct tmpl_info *const info,
                                    const size_t sz) {
  char name[sz];
  template_name(info, name);
  return insert_symbol(name);
}

ANEW ANN static Symbol template_id(const Env               env,
                                   struct tmpl_info *const info) {
  vector_init(&info->type);
  vector_init(&info->size);
  ssize_t      sz  = template_size(env, info);
  const Symbol sym = sz > GW_ERROR ? _template_id(env, info, sz) : NULL;
  for (m_uint i = 0; i < vector_size(&info->type); ++i)
    mp_free2(env->gwion->mp, vector_at(&info->size, i),
             (m_str)vector_at(&info->type, i));
  vector_release(&info->type);
  vector_release(&info->size);
  return sym;
}

ANN static m_bool template_match(Specialized_List sl, TmplArg_List tl) {
//  uint32_t i = 0;
//  while ((call = call->next)) i++;
//&& (base = base->next))
//  while ((call = call->next) && (base = base->next))
//    ;
//  return i = base->len ? GW_OK : GW_ERROR;
//  return !call ? GW_OK : GW_ERROR;
  return tl->len >= sl->len;
}

ANN static Type _tmpl_exists(const Env env, const Symbol name) {
  if (env->class_def && name == insert_symbol(env->class_def->name))
    return env->class_def;
  return nspc_lookup_type1(env->curr, name) ?: env->context ?  nspc_lookup_type1(env->context->nspc, name) : NULL;
}

ANN Type tmpl_exists(const Env env, struct tmpl_info *const info) {
  if (template_match(info->list, info->td->types) < 0) // invalid template
    ERR_N(info->td->tag.loc, _("invalid template types number"));
  if (!info->name)
    info->name = template_id(env, info);
  return _tmpl_exists(env, info->name);
}

ANN bool tmpl_global(const Env env, TmplArg_List tl) {
  for(uint32_t i = 0; i < tl->len; i++) {
    TmplArg arg = *mp_vector_at(tl, TmplArg, i);
    if(unlikely(arg.type == tmplarg_exp)) continue;
    const Type t = known_type(env, arg.d.td);
    if(!t || !type_global(env, t))
      return false;
  }
  return true;
}
