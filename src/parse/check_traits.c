#include <ctype.h>
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

ANN static bool var_match(const Value a, const Value b) {
  bool error = true;
  if (isa(a->type, a->type) < 0) {
    gwerr_basic_from("invalid variable type", NULL, NULL, a->from, 0);
    error = false;
  }
  if (GET_FLAG(a, const) && !GET_FLAG(b, const)) {
    gwerr_basic_from("variable differs in {/}constness{0}", NULL, NULL, a->from, 0);
    error = false;
  }
  if (GET_FLAG(a, static) && !GET_FLAG(b, static)) {
    gwerr_basic_from("variable differs in {/}storage{0}", NULL, NULL, a->from, 0);
    error = false;
  }
  if (error) return true;
  gwerr_secondary_from("from requested variable", b->from);
  return error;
}

ANN static bool request_var(const Env env, const Type t, const Value request) {
  const Value value = nspc_lookup_value0(t->nspc, insert_symbol(request->name));
  if (!value) {
    gwerr_basic("missing requested variable", NULL, NULL,
                request->from->filename, request->from->loc, 0);
    return false;
  }
  return var_match(value, request);
}

ANN static bool check_trait_variables(const Env env, const Type t,
                                      const Trait trait) {
  bool error = false;
  for (m_uint i = 0; i < trait->var->len; i++) {
    const Value request = *mp_vector_at(trait->var, Value, i);
    if (!request_var(env, t, request)) error = true;
  }
  return error;
}

ANN static bool trait_inherit(const Env env, const Type t, const Func_Def req) {
  const bool global = type_global(env, t) || !!nspc_lookup_value1(env->curr->parent, req->base->xid);
  nspc_push_type(env->gwion->mp, env->curr);
  nspc_add_type(env->curr, insert_symbol("Self"), t);
  const Func_Def cpy   = cpy_func_def(env->gwion->mp, req);
  if(global) SET_FLAG(cpy->base, global);
  const m_bool ret   = traverse_func_def(env, cpy);
  nspc_pop_type(env->gwion->mp, env->curr);
  Section section = MK_SECTION(func, func_def, cpy);
  if(!env->context->extend)
    env->context->extend = new_mp_vector(env->gwion->mp, Section, 0);
  mp_vector_add(env->gwion->mp, &env->context->extend, Section, section);
  return ret;
}

ANN static bool check_trait_args(const Env env, const Func f, const Func_Base *req, const bool m) {
  if (mp_vector_len(f->def->base->args) + m != mp_vector_len(req->args)) return false;
  if(m) {
    const Arg *r = mp_vector_at(req->args, Arg, 0);
    if(strcmp(s_name(r->td->xid), "Self"))
    return false;
  }
  for(m_uint i = m; i < req->args->len; i++) {
    const Arg *r = mp_vector_at(req->args, Arg, i + m);
    const Type t = known_type(env, r->td);
    const Arg *arg = mp_vector_at(f->def->base->args, Arg, i);
    if(arg->type != t) return false;
  }
  return true;
}

ANN static bool request_found(const Env env, const Type t,
                            const Func_Def request) {
  const Value v = nspc_lookup_value0(t->nspc, request->base->xid);
  if (!v) return false;
  if (!is_func(env->gwion, v->type)) {
    gwerr_basic_from("is not a function", NULL, NULL, v->from, 0);
    return false;
  }
  Func f = v->d.func_ref;
  do {
    const Func_Base *req = request->base;
    const Func_Base *fbase = f->def->base;
    if (!GET_FLAG(fbase, abstract) &&
        fbase->ret_type == known_type(env, req->td) &&
        (!req->args || check_trait_args(env, f, req, vflag(f->value_ref, vflag_member))))
    return true;
  } while ((f = f->next));
  return false;
}

ANN static bool ufcs_match(const Env env, const Value v,
                           const Func_Def request, const bool global) {
  if (!is_func(env->gwion, v->type)) return false; // mayb enot ignore;
    Func f = v->d.func_ref;
    do {
      const Func_Base *req = request->base;
      const Func_Base *fbase = f->def->base;
      if (!GET_FLAG(fbase, abstract) &&
          !vflag(f->value_ref, vflag_member) &&
          fbase->ret_type == known_type(env, req->td) &&
          mp_vector_len(fbase->args) == mp_vector_len(req->args) &&
          (!req->args || check_trait_args(env, f, req, 0))) {
      if(global && !GET_FLAG(fbase, global))
        return false;
    return true;
}
  } while ((f = f->next));
  return false;
}

ANN static bool trait_ufcs(const Env env, const Type t,
                            const Func_Def request) {
  const Value v = nspc_lookup_value1(env->curr, request->base->xid);
  if(v) {
    const bool global = type_global(env, t) || from_global_nspc(env, v->from->owner);
    nspc_push_type(env->gwion->mp, env->curr);
    nspc_add_type(env->curr, insert_symbol("Self"), t);
    const bool ret = ufcs_match(env, v, request, global);
    nspc_pop_type(env->gwion->mp, env->curr);
    if (ret) return true;
  }
  return false;
}

ANN static bool request_fun(const Env env, const Type t,
                            const Func_Def request) {
  if (t->nspc && request_found(env, t, request)) return true;
  if (trait_ufcs(env, t, request)) return true;
  if (!GET_FLAG(request->base, abstract))
    return trait_inherit(env, t, request);
  const Value parent = nspc_lookup_value1(env->global_nspc, request->base->xid);
  if(parent) {
    const Value v = nspc_lookup_value1(env->curr, request->base->xid);
    if(!env->context->error) {
      gwerr_basic_from("is missing {+G}global{0}", NULL, NULL, v->from, 0);
      gwerr_secondary("from requested func", env->name, request->base->pos);
      env->context->error = true;
    }
  } else gwerr_basic("missing requested function", NULL, NULL, env->name,
              request->base->pos, 0);
  return false;
}

ANN static bool check_trait_functions(const Env env, const Type t,
                                      const Trait trait) {
  bool error = false;
  for (m_uint i = 0; i < trait->fun->len; i++) {
    const Func_Def request = *mp_vector_at(trait->fun, Func_Def, i);
    if (!request_fun(env, t, request)) error = true;
  }
  return error;
}

ANN bool trait_nodup(const ID_List list, const uint32_t i) {
  const Symbol fst = *mp_vector_at(list, Symbol, i);
  for(uint32_t j = i + 1; j < list->len; j++) {
    const Symbol snd = *mp_vector_at(list, Symbol, j);
    if (fst == snd) return false;
  }
  return true;
}

ANN static inline bool trait_error(const Env env) {
  env_set_error(env, true);
  return false;
}

ANN bool check_trait_requests(const Env env, const Type t, const ID_List list, const ValueFrom *from) {
  for(uint32_t i = 0; i < list->len; i++) {
    const Symbol xid = *mp_vector_at(list, Symbol, i);
    const Trait trait = nspc_lookup_trait1(env->curr, xid);
    if (!trait_nodup(list, i)) {
      gwerr_secondary_from("class has duplicated trait", from);
      return trait_error(env);
    }
    if (trait->var ? check_trait_variables(env, t, trait) : false ||
        trait->fun ? check_trait_functions(env, t, trait) : false) {
      gwerr_secondary("in trait", trait->filename, trait->loc);
      gwerr_secondary("requested here", from->filename, from->loc);
      return trait_error(env);
    }
  }
  return true;
}
