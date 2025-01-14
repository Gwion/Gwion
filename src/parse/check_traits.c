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
  if (!isa(a->type, a->type)) {
    gwlog_error_from("invalid variable type", NULL, a->from, 0);
    error = false;
  }
  if (GET_FLAG(a, const) && !GET_FLAG(b, const)) {
    gwlog_error_from("variable differs in {/}constness{0}", NULL, a->from, 0);
    error = false;
  }
  if (GET_FLAG(a, static) && !GET_FLAG(b, static)) {
    gwlog_error_from("variable differs in {/}storage{0}", NULL, a->from, 0);
    error = false;
  }
  if (error) return true;
  gwlog_related_from("from requested variable", b->from);
  return error;
}

ANN static bool request_var(const Env env, const Type t, const Value request) {
  const Value value = nspc_lookup_value0(t->nspc, insert_symbol(request->name));
  if (!value) {
    gwlog_error("missing requested variable", NULL,
                request->from->filename, request->from->loc, 0);
    return false;
  }
  return var_match(value, request);
}

ANN static bool check_trait_variables(const Env env, const Type t,
                                      const Trait trait) {
  bool error = false;
  for (m_uint i = 0; i < trait->var->len; i++) {
    const Value request = valuelist_at(trait->var, i);
    if (!request_var(env, t, request)) error = true;
  }
  return error;
}

ANN static bool trait_inherit(const Env env, const Type t, const Func_Def req) {
  const bool global = type_global(env, t) || !!nspc_lookup_value1(env->curr->parent, req->base->tag.sym);
  nspc_push_type(env->gwion->mp, env->curr);
  nspc_add_type(env->curr, insert_symbol("Self"), t);
  const Func_Def cpy   = cpy_func_def(env->gwion->mp, req);
  if(global) SET_FLAG(cpy->base, global);
  const bool ret   = traverse_func_def(env, cpy);
  nspc_pop_type(env->gwion->mp, env->curr);
  Section section = MK_SECTION(func, func_def, cpy, t->info->value->from->loc);
  
  if(!env->context->extend)
    env->context->extend = new_sectionlist(env->gwion->mp, 0);
  sectionlist_add(env->gwion->mp, &env->context->extend, section);

  // sema_ast, traverse_ast
  // sectionlist_add(env->gwion->mp, &t->info->cdef->body, section);
  return ret;
}

ANN static bool check_trait_args(const Env env, const Func f, const Func_Base *req, const bool m) {
  if (arglist_len(f->def->base->args) + m != arglist_len(req->args)) return false;
  if(m) {
    const Arg r = arglist_at(req->args, 0);
    if(strcmp(s_name(r.var.td->tag.sym), "Self"))
    return false;
  }
  for(m_uint i = m; i < req->args->len; i++) {
    const Arg r = arglist_at(req->args, i + m);
    const Type t = known_type(env, r.var.td);
    const Arg arg = arglist_at(f->def->base->args, i);
    if(arg.type != t) return false;
  }
  return true;
}

ANN static bool request_found(const Env env, const Type t,
                            const Func_Def request) {
  const Value v = nspc_lookup_value0(t->nspc, request->base->tag.sym);
  if (!v) return false;
  if (!is_func(env->gwion, v->type)) {
    gwlog_error_from("is not a function", NULL, v->from, 0);
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
          arglist_len(fbase->args) == arglist_len(req->args) &&
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
  const Value v = nspc_lookup_value1(env->curr, request->base->tag.sym);
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
  const Value parent = nspc_lookup_value1(env->global_nspc, request->base->tag.sym);
  if(parent) {
    const Value v = nspc_lookup_value1(env->curr, request->base->tag.sym);
    gwlog_error_from("is missing {+G}global{0}", NULL, v->from, 0);
    gwlog_related("from requested func", env->name, request->base->tag.loc);
    env_set_error(env, true);
  } else gwlog_error("missing requested function", NULL, env->name,
              request->base->tag.loc, 0);
  return false;
}

ANN static bool check_trait_functions(const Env env, const Type t,
                                      const Trait trait) {
  bool error = false;
  for (m_uint i = 0; i < trait->fun->len; i++) {
    const Func_Def request = funcdeflist_at(trait->fun, i);
    if (!request_fun(env, t, request)) error = true;
  }
  return error;
}

ANN bool trait_nodup(const TagList *list, const uint32_t i) {
  const Tag fst = taglist_at(list, i);
  for(uint32_t j = i + 1; j < list->len; j++) {
    const Tag snd = taglist_at(list, j);
    if (fst.sym== snd.sym) return false;
  }
  return true;
}

ANN static inline bool trait_error(const Env env) {
  env_set_error(env, true);
  return false;
}

ANN bool check_trait_requests(const Env env, const Type t, const TagList *list, const ValueFrom *from) {
  for(uint32_t i = 0; i < list->len; i++) {
    const Tag tag = taglist_at(list, i);
    const Trait trait = nspc_lookup_trait1(env->curr, tag.sym);
    if (!trait_nodup(list, i)) {
      gwlog_warning_from("class has duplicated trait", from);
      return trait_error(env);
    }
    if (trait->var ? check_trait_variables(env, t, trait) : false ||
        trait->fun ? check_trait_functions(env, t, trait) : false) {
      gwlog_warning("in trait", trait->filename, trait->loc);
      gwlog_related_from("requested here", from);
      return trait_error(env);
    }
  }
  return true;
}
