#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "traverse.h"
#include "template.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "parse.h"
#include "emit.h"
#include "specialid.h"
#include "tmp_resolve.h"

struct ResolverArgs {
  const Value v;
  Exp_Call *const e;
  const m_str tmpl_name;
  const Type_List types;
};

ANN static inline Value template_get_ready(const Env env, const Value v, const m_str tmpl, const m_uint i) {
  const Symbol sym = func_symbol(env, v->from->owner->name, v->name, tmpl, i);
  return v->from->owner_class ? find_value(v->from->owner_class, sym) :
      nspc_lookup_value1(v->from->owner, sym);
}

ANN static inline m_bool tmpl_valid(const Env env, const Func_Def fdef) {
  return safe_fflag(fdef->base->func, fflag_valid) ||
      check_traverse_fdef(env, fdef) > 0;
}

ANN static Func ensure_tmpl(const Env env, const Func_Def fdef, Exp_Call *const exp) {
  if(!tmpl_valid(env, fdef))
    return NULL;
  const Func f = fdef->base->func;
  const Func next = f->next;
  f->next = NULL;
  const Func func = find_func_match(env, f, exp);
  f->next = next;
  if(func)
    set_fflag(func, fflag_tmpl | fflag_valid);
  return func;
}

ANN static inline Func ensure_fptr(const Env env, struct ResolverArgs* ra, const Fptr_Def fptr) {
  CHECK_BO(traverse_fptr_def(env, fptr))
  return find_func_match(env, fptr->base->func, ra->e);
}

ANN static Func fptr_match(const Env env, struct ResolverArgs* ra) {
  const Value v = ra->v;
  const Symbol sym = func_symbol(env, v->from->owner->name, v->name, ra->tmpl_name, 0);
  const Type exists = nspc_lookup_type0(v->from->owner, sym);
  if(exists)
    return exists->info->func;
  const Func_Def base = v->d.func_ref ? v->d.func_ref->def : ra->e->func->type->info->func->def;
  const Tmpl tmpl = { .list=base->base->tmpl->list, .call=ra->types };
  CHECK_BO(template_push_types(env, &tmpl));
  Func_Base *const fbase = cpy_func_base(env->gwion->mp, base->base);
  fbase->xid = sym;
  fbase->tmpl->base = 0;
  fbase->tmpl->call = cpy_type_list(env->gwion->mp, ra->types);
  const Fptr_Def fptr = new_fptr_def(env->gwion->mp, fbase);
  const Func m_func = ensure_fptr(env, ra, fptr);
  if(m_func)
    nspc_add_type_front(v->from->owner, sym, actual_type(env->gwion, m_func->value_ref->type));
  if(fptr->type)
    type_remref(fptr->type, env->gwion);
  free_fptr_def(env->gwion->mp, fptr);
  nspc_pop_type(env->gwion->mp, env->curr);
  return m_func;
}

ANN static Func tmpl_exists(const Env env, struct ResolverArgs* ra, const Value exists) {
  if(env->func == exists->d.func_ref)
    return find_func_match(env, env->func, ra->e) ? env->func : NULL;
  return ensure_tmpl(env, exists->d.func_ref->def, ra->e);
}

ANN static Func create_tmpl(const Env env, struct ResolverArgs* ra, const m_uint i) {
  const Value value = template_get_ready(env, ra->v, "template", i);
  if(!value)
    return NULL;
  if(vflag(ra->v, vflag_builtin))
    set_vflag(value, vflag_builtin);
  const Func_Def fdef = (Func_Def)cpy_func_def(env->gwion->mp, value->d.func_ref->def);
  fdef->base->tmpl->call = cpy_type_list(env->gwion->mp, ra->types);
  fdef->base->tmpl->base = i;
  const Func func = ensure_tmpl(env, fdef, ra->e);
  if(!func && !fdef->base->func)
    free_func_def(env->gwion->mp, fdef);
  return func;
}

ANN static Func func_match(const Env env, struct ResolverArgs* ra) {
  for(m_uint i = 0; i < ra->v->from->offset + 1; ++i) {
    const Value exists = template_get_ready(env, ra->v, ra->tmpl_name, i);
    const Func func = exists ?
      tmpl_exists(env, ra, exists) : create_tmpl(env, ra, i);
    if(func)
      return func;
  }
  return NULL;
}

ANN static Func find_tmpl(const Env env, const Value v, Exp_Call *const exp, const m_str tmpl_name) {
  const Type_List types = exp->tmpl->call;
  const Func former = env->func;
  const m_uint scope = env->scope->depth;
  struct EnvSet es = { .env=env, .data=env, .func=(_exp_func)check_cdef,
    .scope=scope, .flag=tflag_check };
  struct ResolverArgs ra = {.v = v, .e = exp, .tmpl_name = tmpl_name, .types = types};
  CHECK_BO(envset_push(&es, v->from->owner_class, v->from->owner))
  (void)env_push(env, v->from->owner_class, v->from->owner);
  const Func m_func = !is_fptr(env->gwion, v->type) ?
      func_match(env, &ra) :fptr_match(env, &ra);
  if(es.run)
    envset_pop(&es, v->from->owner_class);
  env_pop(env, scope);
  env->func = former;
  return m_func;
}

ANN static Func _find_template_match(const Env env, const Value v, Exp_Call *const exp) {
  DECL_OO(const m_str, tmpl_name, = tl2str(env->gwion, exp->tmpl->call, exp->func->pos))
  const Func f = find_tmpl(env, v, exp, tmpl_name);
  free_mstr(env->gwion->mp, tmpl_name);
  return f;
}

ANN static inline m_bool check_call(const Env env, const Exp_Call* exp) {
  const ae_exp_t et = exp->func->exp_type;
  if(et != ae_exp_primary && et != ae_exp_dot && et != ae_exp_cast)
    ERR_B(exp->func->pos, _("invalid expression for function call."))
  return GW_OK;
}

ANN Func find_template_match(const Env env, const Value value, Exp_Call *const exp) {
  CHECK_BO(check_call(env, exp))
  const Func f = _find_template_match(env, value, exp);
  if(f)
    return f;
  Type t = value->from->owner_class;
  while(t && t->nspc) {
    const Func_Def fdef = value->d.func_ref ? value->d.func_ref->def : value->type->info->func->def;
    const Value v = nspc_lookup_value0(t->nspc, fdef->base->xid);
    if(v) {
      const Func f = _find_template_match(env, v, exp);
      if(f)
        return f;
    }
    t = t->info->parent;
  }
  ERR_O(exp_self(exp)->pos, _("arguments do not match for template call"))
}
