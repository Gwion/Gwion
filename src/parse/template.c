#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "func.h"
#include "value.h"
#include "nspc.h"
#include "traverse.h"
#include "template.h"
#include "vm.h"
#include "parse.h"
#include "gwion.h"
#include "cpy_ast.h"
#include "tuple.h"

struct tmpl_info {
  const  Class_Def cdef;
  Type_List        call;
  struct Vector_   type;
  struct Vector_   size;
  uint8_t index;
};

ANN static inline size_t tmpl_set(struct tmpl_info* info, const Type t) {
  vector_add(&info->type, (vtype)t);
  const size_t len = strlen(t->name);
  vector_add(&info->size, len);
  return len;
}

ANN static ssize_t template_size(const Env env, struct tmpl_info* info) {
  ID_List base = info->cdef->base.tmpl->list;
  Type_List call = info->call;
  size_t size = tmpl_set(info, info->cdef->base.type);
  do {
    DECL_OB(const Type, t, = known_type(env, call->td))
    size += tmpl_set(info, t);
  } while((call = call->next) && (base = base->next) && ++size);
  return size + 16 + 3;
}

ANN static inline m_str tmpl_get(struct tmpl_info* info, m_str str) {
  const Type t = (Type)vector_at(&info->type, info->index);
  strcpy(str, t->name);
  return str += vector_at(&info->size, info->index);
}

ANN static void template_name(struct tmpl_info* info, m_str s) {
  m_str str = s;
  str = tmpl_get(info, str);
  *str++ = '<';
  *str++ = '~';
  const m_uint size = vector_size(&info->type);
  for(info->index = 1; info->index < size; ++info->index) {
    str = tmpl_get(info, str);
    if(info->index < size - 1)
      *str++ = ',';
    else {
      *str++ = '~';
      *str++ = '>';
    }
  }
  *str = '\0';
}

ANEW ANN static Symbol template_id(const Env env, const Class_Def c, const Type_List call) {
  struct tmpl_info info = { .cdef=c, .call=call };
  vector_init(&info.type);
  vector_init(&info.size);
  ssize_t sz = template_size(env, &info);
  char name[sz];
  if(sz > GW_ERROR)
    template_name(&info, name);
  vector_release(&info.type);
  vector_release(&info.size);
  return sz > GW_ERROR ? insert_symbol(name) : NULL;
}

ANN m_bool template_match(ID_List base, Type_List call) {
  while((call = call->next) && (base = base->next));
  return !call ? GW_OK : GW_ERROR;
}

ANN static Class_Def template_class(const Env env, const Class_Def def, const Type_List call) {
  DECL_OO(const Symbol, name, = template_id(env, def, call))
  if(env->class_def && name == insert_symbol(env->class_def->name))
     return env->class_def->e->def;
  const Type t = nspc_lookup_type1(env->curr, name);
  if(t)
    return t->e->def;
  const Class_Def c = cpy_class_def(env->gwion->mp, def);
  c->base.xid = name;
  SET_FLAG(c, template | ae_flag_ref);
  return c;

}

ANN m_bool template_push_types(const Env env, const Tmpl *tmpl) {
  ID_List list = tmpl->list;
  Type_List call = tmpl->call;
  nspc_push_type(env->gwion->mp, env->curr);
  do {
    if(!call)
      break;
    const Type t = known_type(env, call->td);
    assert(t);
    nspc_add_type(env->curr, list->xid, t);
    call = call->next;
  } while((list = list->next));
  if(!call)
    return GW_OK;
  POP_RET(-1);
}

extern ANN m_bool scan0_class_def(const Env, const Class_Def);
extern ANN m_bool scan1_class_def(const Env, const Class_Def);
extern ANN m_bool traverse_func_def(const Env, const Func_Def);
extern ANN m_bool traverse_class_def(const Env, const Class_Def);

ANN Type scan_tuple(const Env env, const Type_Decl *td) {
  struct Vector_ v;
  vector_init(&v);
  Type_List tl = td->types;
  do {
    const Type t = tl->td->xid->xid != insert_symbol("_") ?
       known_type(env, tl->td) : (Type)1;
    if(t)
      vector_add(&v, (m_uint)t);
    else {
      vector_release(&v);
      return NULL;
    }
  } while((tl = tl->next));
  const Type ret = tuple_type(env, &v, td_pos(td));
  vector_release(&v);
  return ret;
}

ANN Tmpl* mk_tmpl(const Env env, const Tmpl *tm, const Type_List types) {
  Tmpl *tmpl = new_tmpl(env->gwion->mp, tm->list, 0);
  tmpl->call = cpy_type_list(env->gwion->mp, types);
  return tmpl;
}

ANN static m_bool class2udef(const Env env, const Class_Def a, const Type t) {
  a->union_def = new_union_def(env->gwion->mp, a->list,
    loc_cpy(env->gwion->mp, t->e->def->pos));
  a->union_def->type_xid = a->base.xid;
  CHECK_BB(scan0_union_def(env, a->union_def))
  a->base.type = a->union_def->type;
  a->base.type->e->def = a;
  return GW_OK;
}

ANN static Type scan_class(const Env env, const Type t, const Type_Decl* td) {
  if(template_match(t->e->def->base.tmpl->list, td->types) < 0)
   ERR_O(td->xid->pos, _("invalid template types number"))
  DECL_OO(const Class_Def, a, = template_class(env, t->e->def, td->types))
  if(a->base.type)
    return a->base.type;
  a->base.tmpl = mk_tmpl(env, t->e->def->base.tmpl, td->types);
  if(t->e->parent !=  env->gwion->type[et_union])
    CHECK_BO(scan0_class_def(env, a))
  else
    CHECK_BO(class2udef(env, a, t))
  SET_FLAG(a->base.type, template);
  if(GET_FLAG(t, builtin))
    SET_FLAG(a->base.type, builtin);
  CHECK_BO(scan1_cdef(env, a))
  return a->base.type;
}

ANN Type scan_func(const Env env, const Type t, const Type_Decl* td) {
  DECL_OO(const m_str, tl_name, = tl2str(env, td->types))
  const Symbol sym = func_symbol(env, t->e->owner->name, t->e->d.func->name, tl_name, 0);
  free_mstr(env->gwion->mp, tl_name);
  const Type base_type = nspc_lookup_type1(t->e->owner, sym);
  if(base_type)
    return base_type;
  const Type ret = type_copy(env->gwion->mp, t);
  ADD_REF(ret->nspc)
  ret->e->parent = t;
  ret->name = s_name(sym);
  SET_FLAG(ret, func);
  nspc_add_type_front(t->e->owner, sym, ret);
  const Func_Def def = cpy_func_def(env->gwion->mp, t->e->d.func->def);
  const Func func = ret->e->d.func = new_func(env->gwion->mp, s_name(sym), def);
  const Value value = new_value(env->gwion->mp, ret, s_name(sym));
  func->flag = def->flag;
  value->d.func_ref = func;
  value->from->owner = t->e->owner;
  value->from->owner_class = t->e->d.func->value_ref->from->owner_class;
  func->value_ref = value;
  func->def->base->tmpl = mk_tmpl(env, t->e->d.func->def->base->tmpl, td->types);
  def->base->func = func;
  nspc_add_value_front(t->e->owner, sym, value);
  return ret;
}

ANN Type no_types(const Env env, const Type t) {
  if(t != env->gwion->type[et_tuple])
    ERR_O(t->e->def->pos,
        _("you must provide template types for type '%s'"), t->name)
  return t;
}

ANN Type maybe_func(const Env env, const Type t, const Type_Decl* td) {
  if(isa(t, env->gwion->type[et_function]) > 0 && t->e->d.func->def->base->tmpl)
     return scan_func(env, t, td);
   ERR_O(td->xid->pos,
      _("type '%s' is not template. You should not provide template types"), t->name)
}

ANN Type scan_type(const Env env, const Type t, const Type_Decl* td) {
  if(GET_FLAG(t, template)) {
    if(GET_FLAG(t, ref))
      return t;
    if(!td->types)
      return no_types(env, t);
     if(t->e->def)
       return scan_class(env, t, td);
     else
       return scan_tuple(env, td);
   } else if(td->types)
     return maybe_func(env, t, td);
  return t;
}
