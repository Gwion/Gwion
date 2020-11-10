#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "traverse.h"
#include "template.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "parse.h"
#include "match.h"
#include "emit.h"
#include "specialid.h"
#include "tmp_resolve.h"

struct ResolverArgs {
  Value v;
  const Exp_Call *e;
  m_str tmpl_name;
  Func m_func;
  Type_List types;
};

ANN static inline Value template_get_ready(const Env env, const Value v, const m_str tmpl, const m_uint i) {
  const Symbol sym = func_symbol(env, v->from->owner->name, v->name, tmpl, i);
  return v->from->owner_class ? find_value(v->from->owner_class, sym) :
      nspc_lookup_value1(v->from->owner, sym);
}

ANN static m_bool check_call(const Env env, const Exp_Call* exp) {
  ae_exp_t et = exp->func->exp_type;
  if(et != ae_exp_primary && et != ae_exp_dot && et != ae_exp_cast)
    ERR_B(exp->func->pos, _("invalid expression for function call."))
  CHECK_OB(check_exp(env, exp->func))
  if(exp->args)
    CHECK_OB(check_exp(env, exp->args))
  return GW_OK;
}

ANN static Func ensure_tmpl(const Env env, const Func_Def fdef, const Exp_Call *exp) {
  const m_bool ret = (fdef->base->func && fflag(fdef->base->func, fflag_valid)) || check_traverse_fdef(env, fdef) > 0;
  if(ret) {
    const Func f = fdef->base->func;
    const Func next = f->next;
    f->next = NULL;
    const Func func = find_func_match(env, f, exp->args);
    f->next = next;
    if(func) {
      set_fflag(func, fflag_tmpl);
      set_fflag(func, fflag_valid);
      return func;
    }
  }
  return NULL;
}

ANN static Func fptr_match(const Env env, struct ResolverArgs* f_ptr_args) {
  const Value v = f_ptr_args->v;
  const m_str tmpl_name = f_ptr_args->tmpl_name;
  const Exp_Call *exp = f_ptr_args->e;
  Type_List types = f_ptr_args->types;
  const Symbol sym = func_symbol(env, v->from->owner->name, v->name, tmpl_name, 0);
  const Type exists = nspc_lookup_type0(v->from->owner, sym);
  if(exists)
    return exists->info->func;

  Func m_func = f_ptr_args->m_func;
  Func_Def base = v->d.func_ref ? v->d.func_ref->def : exp->func->info->type->info->func->def;
  Func_Base *fbase = cpy_func_base(env->gwion->mp, base->base);
  fbase->xid = sym;
  fbase->tmpl->base = 0;
  fbase->tmpl->call = cpy_type_list(env->gwion->mp, types);
  if(template_push_types(env, fbase->tmpl) > 0) {
    const Fptr_Def fptr = new_fptr_def(env->gwion->mp, fbase);
    if(traverse_fptr_def(env, fptr) > 0 &&
        (base->base->ret_type = known_type(env, base->base->td)) &&
        (!exp->args || !!check_exp(env, exp->args))) {
      m_func = find_func_match(env, fbase->func, exp->args);
      nspc_pop_type(env->gwion->mp, env->curr);
      if(m_func)
        nspc_add_type_front(v->from->owner, sym, actual_type(env->gwion, m_func->value_ref->type));
    }
    if(fptr->type)
      type_remref(fptr->type, env->gwion);
    free_fptr_def(env->gwion->mp, fptr);
  }
  return m_func;
}

ANN static Func func_match(const Env env, struct ResolverArgs* f_ptr_args) {
  const Value v = f_ptr_args->v;
  const m_str tmpl_name = f_ptr_args->tmpl_name;
  const Exp_Call *exp = f_ptr_args->e;
  Func m_func = f_ptr_args->m_func;
  Type_List types = f_ptr_args->types;
  for(m_uint i = 0; i < v->from->offset + 1; ++i) {
    const Value exists = template_get_ready(env, v, tmpl_name, i);
    if(exists) {
      if(env->func == exists->d.func_ref) {
        if(check_call(env, exp) < 0 ||
           !find_func_match(env, env->func, exp->args))
          continue;
        m_func = env->func;
        break;
      }
      if((m_func = ensure_tmpl(env, exists->d.func_ref->def, exp)))
        break;
    } else {
      const Value value = template_get_ready(env, v, "template", i);
      if(!value)
        continue;
      if(vflag(v, vflag_builtin))
        set_vflag(value, vflag_builtin);
      const Func_Def fdef = (Func_Def)cpy_func_def(env->gwion->mp, value->d.func_ref->def);
      fdef->base->tmpl->call = cpy_type_list(env->gwion->mp, types);
      fdef->base->tmpl->base = i;
      if((m_func = ensure_tmpl(env, fdef, exp))) {
        break;
      }
      if(!fdef->base->func) {
        free_func_def(env->gwion->mp, fdef);
      }
    }
  }
  return m_func;
}

ANN static Func _find_template_match(const Env env, const Value v, const Exp_Call* exp) {
  CHECK_BO(check_call(env, exp))
  const Type_List types = exp->tmpl->call;
  Func m_func = NULL, former = env->func;
  DECL_OO(const m_str, tmpl_name, = tl2str(env, types))
  const m_uint scope = env->scope->depth;
  struct EnvSet es = { .env=env, .data=env, .func=(_exp_func)check_cdef,
    .scope=scope, .flag=tflag_check };
  struct ResolverArgs f_ptr_args = {.v = v, .e = exp, .tmpl_name = tmpl_name, m_func =  m_func, .types = types};
  CHECK_BO(envset_push(&es, v->from->owner_class, v->from->owner))
  (void)env_push(env, v->from->owner_class, v->from->owner);
  if(is_fptr(env->gwion, v->type)) {
    m_func = fptr_match(env, &f_ptr_args);
  } else {
    m_func = func_match(env, &f_ptr_args);
  }
  free_mstr(env->gwion->mp, tmpl_name);
  if(es.run)
    envset_pop(&es, v->from->owner_class);
  env_pop(env, scope);
  env->func = former;
  return m_func;
}

ANN Func find_template_match(const Env env, const Value value, const Exp_Call* exp) {
  const Func f = _find_template_match(env, value, exp);
  if(f)
    return f;
  Type t = value->from->owner_class;
  while(t && t->nspc) {
    Func_Def fdef = value->d.func_ref ? value->d.func_ref->def : value->type->info->func->def;
    const Value v = nspc_lookup_value0(t->nspc, fdef->base->xid);
    if(!v) {
      const Func f = _find_template_match(env, v, exp);
      if(f)
        return f;
    }
    t = t->info->parent;
  }
  ERR_O(exp_self(exp)->pos, _("arguments do not match for template call"))
}
