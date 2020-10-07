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
  const m_bool ret = GET_FLAG(fdef->base, valid) || check_traverse_fdef(env, fdef) > 0;
  if(ret) {
    const Func f = fdef->base->func;
    const Func next = f->next;
    f->next = NULL;
    const Func func = find_func_match(env, f, exp->args);
    f->next = next;
    if(func) {
      SET_FLAG(func, valid | ae_flag_template);
      return func;
    }
  }
  return NULL;
}

ANN static Func _find_template_match(const Env env, const Value v, const Exp_Call* exp) {
  CHECK_BO(check_call(env, exp))
  const Type_List types = exp->tmpl->call;
  Func m_func = NULL, former = env->func;
  DECL_OO(const m_str, tmpl_name, = tl2str(env, types))
  const m_uint scope = env->scope->depth;
  struct EnvSet es = { .env=env, .data=env, .func=(_exp_func)check_cdef,
    .scope=scope, .flag=ae_flag_check };
  CHECK_BO(envset_push(&es, v->from->owner_class, v->from->owner))
  (void)env_push(env, v->from->owner_class, v->from->owner);
  if(is_fptr(env->gwion, v->type)) {
    const Symbol sym = func_symbol(env, v->from->owner->name, v->name, tmpl_name, 0);
    const Type exists = nspc_lookup_type0(v->from->owner, sym);
    if(exists)
      m_func = exists->e->d.func;
    else {
      Func_Def base = v->d.func_ref ? v->d.func_ref->def : exp->func->info->type->e->d.func->def;
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
        free_fptr_def(env->gwion->mp, fptr);
        if(fptr->type)
          REM_REF(fptr->type, env->gwion)
      }
    }
  } else {
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
        if(GET_FLAG(v, builtin)) {
          SET_FLAG(value, builtin);
          SET_FLAG(value->d.func_ref, builtin);
        }
        const Func_Def fdef = (Func_Def)cpy_func_def(env->gwion->mp, value->d.func_ref->def);
        SET_FLAG(fdef->base, template);
        fdef->base->tmpl->call = cpy_type_list(env->gwion->mp, types);
        fdef->base->tmpl->base = i;
        if((m_func = ensure_tmpl(env, fdef, exp)))
          break;
      }
    }
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
    Func_Def fdef = value->d.func_ref ? value->d.func_ref->def : value->type->e->d.func->def;
    const Value v = nspc_lookup_value0(t->nspc, fdef->base->xid);
    if(!v)
      goto next;
     const Func f = _find_template_match(env, v, exp);
     if(f)
       return f;
   next:
     t = t->e->parent;
  }
  ERR_O(exp_self(exp)->pos, _("arguments do not match for template call"))
}
