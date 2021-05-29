#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "template.h"
#include "vm.h"
#include "parse.h"
#include "gwion.h"
#include "operator.h"
#include "instr.h"
#include "object.h"
#include "import.h"

ANN static m_bool push_types(const Env env, const Tmpl *tmpl) {
  Specialized_List list = tmpl->list;
  Type_List call = tmpl->call;
  do {
    if(!call)
      break;
    const Type t = known_type(env, call->td);
    if(!t)
      return 1;
    nspc_add_type(env->curr, list->xid, t);
    call = call->next;
  } while((list = list->next));
  return !call;
}

ANN static m_bool _template_push(const Env env, const Type t) {
  if(t->info->value->from->owner_class)
    CHECK_BB(template_push(env, t->info->value->from->owner_class));
  if(tflag(t, tflag_tmpl))
    return push_types(env, t->info->cdef->base.tmpl); // incorrect
  return GW_OK;
}

ANN m_bool template_push(const Env env, const Type t) {
  nspc_push_type(env->gwion->mp, env->curr);
  return _template_push(env, t);
}

ANN m_bool template_push_types(const Env env, const Tmpl *tmpl) {
  nspc_push_type(env->gwion->mp, env->curr);
  if(push_types(env, tmpl))
    return GW_OK;
  POP_RET(GW_ERROR);
}

ANN Tmpl* mk_tmpl(const Env env, const Tmpl *tm, const Type_List types) {
  Tmpl *tmpl = new_tmpl(env->gwion->mp, tm->list, 0);
  tmpl->call = cpy_type_list(env->gwion->mp, types);
  return tmpl;
}

static ANN Type scan_func(const Env env, const Type t, const Type_Decl* td) {
  DECL_OO(const m_str, tl_name, = tl2str(env->gwion, td->types, td->pos));
  const Symbol sym = func_symbol(env, t->info->value->from->owner->name, t->info->func->name, tl_name, 0);
  free_mstr(env->gwion->mp, tl_name);
  const Type base_type = nspc_lookup_type1(t->info->value->from->owner, sym);
  if(base_type)
    return base_type;
  const Type ret = type_copy(env->gwion->mp, t);
  ret->info->parent = t;
  ret->info->value = t->info->func->value_ref;
  ret->name = s_name(sym);
  set_tflag(ret, tflag_ftmpl);
  nspc_add_type_front(t->info->value->from->owner, sym, ret);
  void* func_ptr = t->info->func->def->d.dl_func_ptr;
  if(vflag(t->info->func->value_ref, vflag_builtin))
    t->info->func->def->d.dl_func_ptr = NULL;
  const Func_Def def = cpy_func_def(env->gwion->mp, t->info->func->def);
  const Func func = ret->info->func = new_func(env->gwion->mp, s_name(sym), def);
  const Value value = new_value(env->gwion->mp, ret, s_name(sym));
  func->flag = def->base->flag;
  if(vflag(t->info->func->value_ref, vflag_member))
    set_vflag(value, vflag_member);
  value->d.func_ref = func;
  value->from->owner = t->info->value->from->owner;
//  value->from->owner_class = t->info->owner_class;
  func->value_ref = value;
  func->def->base->tmpl = mk_tmpl(env, t->info->func->def->base->tmpl, td->types);
  def->base->func = func;
  nspc_add_value_front(t->info->value->from->owner, sym, value);
  if(vflag(t->info->func->value_ref, vflag_builtin)) {
    builtin_func(env->gwion->mp, func, func_ptr);
    t->info->func->def->d.dl_func_ptr = func_ptr;
  }
  return ret;
}

static ANN Type maybe_func(const Env env, const Type t, const Type_Decl* td) {
  if(isa(t, env->gwion->type[et_function]) > 0 && t->info->func->def->base->tmpl)
     return scan_func(env, t, td);
  ERR_O(td->pos,
     _("type '%s' is not template. You should not provide template types"), t->name)
}

ANN Type _scan_type(const Env env, const Type t, Type_Decl* td) {
  if(tflag(t, tflag_tmpl) && isa(t, env->gwion->type[et_function]) < 0) {
    if(tflag(t, tflag_ntmpl) && !td->types)
      return t;
    struct TemplateScan ts = { .t=t, .td=td };
    struct Op_Import opi = { .op=insert_symbol("@scan"), .lhs=t, .data=(uintptr_t)&ts, .pos=td->pos };
    return op_check(env, &opi);
  } else if(td->types)
    return maybe_func(env, t, td);
  return find_type(env, td);
}

ANN Type scan_type(const Env env, const Type t, Type_Decl* td) {
  if(td->next) {
    Type_Decl *next = td->next;
    td->next = NULL;
    const Type maybe_array = known_type(env, td);
    const Type owner = array_base(maybe_array);
    td->next = next;
    CHECK_OO(owner);
    if(!owner->nspc)
      ERR_O(td->pos, "type '%s' has no namespace", owner->name)
    struct EnvSet es = { .env=env, .data=env,
      .scope=env->scope->depth, .flag=tflag_none };
    envset_push(&es, owner, owner->nspc);
    (void)env_push(env, owner, owner->nspc);// TODO: is this needed?
//    const Type ret = scan_type(env, t, td->next);
    const Type ret = known_type(env, td->next);
    env_pop(env, es.scope);
    if(es.run)
      envset_pop(&es, owner);
    if(!td->array)
      return ret;
    return array_type(env, ret, td->array->depth);
  }
  return !t->array_depth ? _scan_type(env, t, td) : t;
}
