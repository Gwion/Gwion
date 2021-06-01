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
  if(isa(a->type, a->type) < 0) {
    gwerr_basic("invalid variable type", NULL, NULL,
        a->from->filename, a->from->loc, 0);
    // can we point to the type decl?
    error = false;
  }
  if(GET_FLAG(a, const) && !GET_FLAG(b, const)) {
    gwerr_basic("variable differs in {/}constness{0}", NULL, NULL,
        a->from->filename, a->from->loc, 0);
    // can we point to the flag?
    error = false;
  }
  if(GET_FLAG(a, static) && !GET_FLAG(b, static)) {
    gwerr_basic("variable differs in {/}storage{0}", NULL, NULL,
        a->from->filename, a->from->loc, 0);
    // can we point to the flag?
    error = false;
  }
  if(error)
    return true;
  gwerr_secondary("from requested variable", b->from->filename, b->from->loc);
  return error;
}

ANN static bool request_var(const Env env, const Type t, const Value request) {
  const Value value = nspc_lookup_value0(t->nspc, insert_symbol(request->name));
  if(!value) {
    gwerr_basic("missing requested variable", NULL, NULL,
        request->from->filename, request->from->loc, 0);
    return false;
  }
  return var_match(value, request);
}

ANN static bool check_trait_variables(const Env env, const Type t, const Trait trait) {
  bool error = false;
  for(m_uint i = 0; i < vector_size(&trait->requested_values); i++) {
    const Value request = (Value)vector_at(&trait->requested_values, i);
    if(!request_var(env, t, request))
      error = true;
  }
  return error;
}

ANN static bool request_fun(const Env env, const Type t, const Func_Def request) {
  const Value v = nspc_lookup_value0(t->nspc, request->base->xid);
if(v) {
  if(isa(v->type, env->gwion->type[et_function]) < 0 || is_fptr(env->gwion, v->type)) {
    gwerr_basic("is not a function", NULL, NULL,
      v->from->filename, v->from->loc, 0);
    return false;
  }
  Func f = v->d.func_ref;
  do {
    if(compat_func(f->def, request) > 0) {
      if(!GET_FLAG(f->def->base, abstract))
        return true;
    }
  } while ((f = f->next));
}
  if(!GET_FLAG(request->base, abstract)){
    const m_uint scope = env_push_type(env, t);
    const Func_Def cpy = cpy_func_def(env->gwion->mp, request);
    const m_bool ret = traverse_func_def(env, cpy);
    env_pop(env, scope);
    if(ret > 0){
      Section *section = new_section_func_def(env->gwion->mp, cpy);
      const Ast ast = new_ast(env->gwion->mp, section, NULL);
      Ast last = t->info->cdef->body;
      while(last->next)
        last = last->next;
      last->next = ast;
      return true;
    } else
    free_func_def(env->gwion->mp, cpy);
  }
  if(!v) {
    gwerr_basic("missing requested function", NULL, NULL,
      env->name, request->base->pos, 0);
    return false;
  }
  return true;
}

ANN static bool check_trait_functions(const Env env, const Type t, const Trait trait) {
  bool error = false;
  for(m_uint i = 0; i < vector_size(&trait->requested_funcs); i++) {
    const Func_Def request = (Func_Def)vector_at(&trait->requested_funcs, i);
    if(!request_fun(env, t, request))
      error = true;
  }
  return error;
}

ANN2(1,2) static inline bool trait_nodup(Type t, const Symbol trait, ID_List list) {
  bool nodup = true;
  do {
    while(list) {
      if(trait == list->xid)
        nodup = false;
      list = list->next;
    }
  } while ((t = t->info->parent));
  return nodup;
}

ANN bool check_trait_requests(const Env env, const Type t, const ID_List list) {
  const Trait trait = nspc_lookup_trait1(env->curr, list->xid);
  if(!trait_nodup(t, list->xid, list->next)) {
    gwerr_secondary("duplicated trait", trait->filename, trait->loc);
    return false;
  }
  const bool value_error = check_trait_variables(env, t, trait);
  const bool funcs_error = check_trait_functions(env, t, trait);
  if(!value_error && !funcs_error)
    return true;
  const Value request = (Value)vector_front(&trait->requested_values);
  gwerr_secondary("from trait", request->from->filename, trait->loc);
  return false;
}

