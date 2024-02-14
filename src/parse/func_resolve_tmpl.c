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
#include "spread.h"

struct ResolverArgs {
  const Value     v;
  Exp_Call *const e;
  const m_str     tmpl_name;
  const TmplArg_List types;
};

ANN static inline Value template_get_ready(const Env env, const Value v,
                                           const m_str tmpl, const m_uint i) {
  const Symbol sym = func_symbol(env, v->from->owner->name, v->name, tmpl, i);
  return v->from->owner_class ? find_value(v->from->owner_class, sym)
                              : nspc_lookup_value1(v->from->owner, sym);
}

ANN static inline bool
tmpl_valid(const Env env, const Func_Def fdef, const m_str filename) {
  if (safe_fflag(fdef->base->func, fflag_valid)) return true;
  const m_str old_file = env->name;
  env->name = filename;
  const bool ret = check_traverse_fdef(env, fdef);
  env->name = old_file;
  if(!fdef->base->func) free_func_def(env->gwion->mp, fdef);
  return ret;
}

ANN static Func ensure_tmpl(const Env env, const Func_Def fdef,
                            Exp_Call *const exp, const m_str filename) {
  if (!tmpl_valid(env, fdef, filename)) return NULL;
  if(env->context && env->context->error) return NULL;
  if (exp->args && !exp->args->type) return NULL;
  const Func f    = fdef->base->func;
  const Tmpl tmpl = {.list = fdef->base->tmpl->list, .call = exp->tmpl->call};
  CHECK_O(template_push_types(env, &tmpl));
  const Func func = find_func_match(env, f, exp);
  nspc_pop_type(env->gwion->mp, env->curr);
  if (func)
    call_add_effect(env, func, exp->func->loc);
  return func;
}

ANN static inline Func ensure_fptr(const Env env, struct ResolverArgs *ra,
                                   const Fptr_Def fptr) {
  CHECK_O(traverse_fptr_def(env, fptr));
  const Func_Def fdef = mp_vector_at(fptr->cdef->base.type->info->cdef->body, struct Section_ , 0)->d.func_def;
  return find_func_match(env, fdef->base->func, ra->e);
}

ANN static Func fptr_match(const Env env, struct ResolverArgs *ra) {
  const Value  v = ra->v;
  const Symbol sym =
      func_symbol(env, v->from->owner->name, v->name, ra->tmpl_name, 0);
  const Value exists = nspc_lookup_value0(v->from->owner, sym);
  if(exists) {
    const Type t = actual_type(env->gwion, exists->type);
    const Func_Def fdef = mp_vector_at(t->info->cdef->body, struct Section_ , 0)->d.func_def;
    return find_func_match(env, fdef->base->func, ra->e);
  }
  const Func_Def base =
      v->d.func_ref ? v->d.func_ref->def : ra->e->func->type->info->func->def;
  const Tmpl tmpl = {.list = base->base->tmpl->list, .call = ra->types};
  CHECK_O(template_push_types(env, &tmpl));
  Func_Base *const fbase = cpy_func_base(env->gwion->mp, base->base);
  fbase->tag.sym         = sym;
  fbase->tmpl->call      = cpy_tmplarg_list(env->gwion->mp, ra->types);
  const Fptr_Def fptr    = new_fptr_def(env->gwion->mp, fbase);
  const Func     m_func  = ensure_fptr(env, ra, fptr);
  if (m_func)
    nspc_add_type_front(v->from->owner, sym,
                        actual_type(env->gwion, m_func->value_ref->type));
  free_fptr_def(env->gwion->mp, fptr);
  nspc_pop_type(env->gwion->mp, env->curr);
  return m_func;
}

ANN static Func tmpl_exists(const Env env, struct ResolverArgs *ra,
                            const Value exists) {
  if (env->func == exists->d.func_ref)
    return find_func_match(env, env->func, ra->e) ? env->func : NULL;
  return ensure_tmpl(env, exists->d.func_ref->def, ra->e, ra->v->from->filename);
}

ANN static Func create_tmpl(const Env env, struct ResolverArgs *ra,
                            const m_uint i) {
  DECL_O(const Value, value, = template_get_ready(env, ra->v, "template", i));
  if (value->d.func_ref->def->builtin) set_vflag(value, vflag_builtin);
  const Func_Def fdef = cpy_func_def(env->gwion->mp, value->d.func_ref->def);
  fdef->base->tmpl->call = cpy_tmplarg_list(env->gwion->mp, ra->types);

  fdef->vt_index = i;
  if(is_spread_tmpl(value->d.func_ref->def->base->tmpl)) {
    Arg_List args = fdef->base->args ?: new_mp_vector(env->gwion->mp, Arg, 0);
    for(uint32_t idx = 0; idx < ra->types->len; idx++) {
//    for(uint32_t idx = value->d.func_ref->def->base->tmpl->call->len - 1; idx < ra->types->len; idx++) {
      char c[256];
      sprintf(c, "arg%u", idx);
      TmplArg targ = *mp_vector_at(ra->types, TmplArg, idx);
      if(targ.type != tmplarg_td) {
        gwerr_basic("invalid const expression in variadic template", NULL, "can't use expression in spread", env->name, targ.d.exp->loc, 0);
        Specialized *spec = mp_vector_at(value->d.func_ref->def->base->tmpl->list, Specialized, value->d.func_ref->def->base->tmpl->list->len - 1);
        gwerr_secondary("spread starts here", env->name, spec->tag.loc);
        env_set_error(env, true);
        return NULL;
      }
      Arg arg = { .var = MK_VAR(cpy_type_decl(env->gwion->mp, targ.d.td), (Var_Decl){ .tag = MK_TAG(insert_symbol(c), fdef->base->tag.loc)})};
      mp_vector_add(env->gwion->mp, &args, Arg, arg);
    }
    fdef->base->args = args;
  }
  const Func func        = ensure_tmpl(env, fdef, ra->e, ra->v->from->filename);
  if (func && func->def->builtin) {
    builtin_func(env->gwion, func, (void*)ra->v->d.func_ref->code->native_func);
    set_vflag(func->value_ref, vflag_builtin);
    struct Op_Import opi = { .lhs = ra->v->d.func_ref->value_ref->type, .rhs = func->value_ref->type };
    op_cpy(env, &opi);
  }
  return func;
}

ANN static Func func_match(const Env env, struct ResolverArgs *ra) {
  for (m_uint i = 0; i < ra->v->from->offset + 1; ++i) {
    const Value exists = template_get_ready(env, ra->v, ra->tmpl_name, i);
    const Func  func = exists
           ? tmpl_exists(env, ra, exists)
           : create_tmpl(env, ra, i);
    if (func) return func;
  }
  return NULL;
}

ANN static Func find_tmpl(const Env env, const Value v, Exp_Call *const exp,
                          const m_str tmpl_name) {
  const TmplArg_List     types  = exp->tmpl->call;
  const Func          former = env->func;
  const m_uint        scope  = env->scope->depth;
  struct EnvSet       es     = {.env   = env,
                      .data  = env,
                      .func  = (_envset_func)check_cdef,
                      .scope = scope,
                      .flag  = tflag_check};
  struct ResolverArgs ra     = {
      .v = v, .e = exp, .tmpl_name = tmpl_name, .types = types};
  CHECK_O(envset_pushv(&es, v));
  (void)env_push(env, v->from->owner_class, v->from->owner);
  const Tmpl *tmpl = v->from->owner_class && v->from->owner_class->info->cdef ?
      get_tmpl(v->from->owner_class) : NULL;
  if(tmpl)
    (void)template_push_types(env, tmpl);
  const bool is_clos = isa(exp->func->type, env->gwion->type[et_closure]) > 0;
  const Func m_func = !is_clos ? func_match(env, &ra)
                               : fptr_match(env, &ra);
  if(tmpl)
    nspc_pop_type(env->gwion->mp, env->curr);
  env_pop(env, scope);
  envset_pop(&es, v->from->owner_class);
  env->func = former;
  return m_func;
}

ANN static Func __find_template_match(const Env env, const Value v,
                                      Exp_Call *const exp) {
  DECL_O(const m_str, tmpl_name,
          = tl2str(env->gwion, exp->tmpl->call, exp->func->loc));
  const Func f = find_tmpl(env, v, exp, tmpl_name);
  free_mstr(env->gwion->mp, tmpl_name);
  return f;
}

ANN static Func _find_template_match(const Env env, const Value v,
                                     Exp_Call *const exp) {
  DECL_O(const Func, f, = __find_template_match(env, v, exp));
  TmplArg_List        tl = exp->tmpl->call;
  Specialized_List sl = f->def->base->tmpl->list;
  for(uint32_t i = 0; i < tl->len; i++) {
    Specialized * spec = mp_vector_at(sl, Specialized, i);
    TmplArg arg = *mp_vector_at(tl, TmplArg, i);
    if(unlikely(spec->td)) {
      if(unlikely(arg.type == tmplarg_td))
        ERR_O(exp_self(exp)->loc, "expected contant, not type");
      // check argument in call exp
      continue;

    } else {
      if(unlikely(arg.type == tmplarg_exp)) {
        ERR_O(exp_self(exp)->loc, "expected type, not constant");
        // check argument in call exp?
        continue;
      }
      DECL_O(const Type, t, = known_type(env, arg.d.td));
      if(t->info->traits && miss_traits(t, spec))
        return NULL;
    }
  }
  return f;
}

#undef ERR_B
#define ERR_B(a, b, ...)                                                       \
  {                                                                            \
    env_err(env, (a), (b), ##__VA_ARGS__);                                     \
    return false;                                                              \
  }

ANN static inline bool check_call(const Env env, const Exp_Call *exp) {
  const ae_exp_t et = exp->func->exp_type;
  if (et != ae_exp_primary && et != ae_exp_dot && et != ae_exp_cast)
    ERR_B(exp->func->loc, _("invalid expression for function call."));
  return true;
}

ANN Func find_template_match(const Env env, const Value value,
                             Exp_Call *const exp) {
  CHECK_O(check_call(env, exp));
  const Func f = _find_template_match(env, value, exp);
  if (f) return f;
  Type t = value->from->owner_class;
  while (t && t->nspc) {
    const Func_Def fdef = value->d.func_ref ? value->d.func_ref->def
                                            : value->type->info->func->def;
    const Value    v    = nspc_lookup_value0(t->nspc, fdef->base->tag.sym);
    if (v) {
      const Func f = _find_template_match(env, v, exp);
      if (f) return f;
    }
    t = t->info->parent;
  }
  ERR_O(exp_self(exp)->loc, _("arguments do not match for template call"));
}
