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
#include "match.h"
#include "specialid.h"
#include "tmp_resolve.h"
#include "partial.h"
#include "spread.h"

ANN static m_bool check_stmt_list(const Env env, Stmt_List list);
ANN m_bool        check_class_def(const Env env, const Class_Def class_def);

ANN static Type check_internal(const Env env, const Symbol sym, const Exp e,
                               const Type t) {
  struct Implicit  imp = {.e = e, .t = t, .pos = e->pos};
  struct Op_Import opi = {.op   = sym,
                          .lhs  = e->type,
                          .rhs  = t,
                          .data = (uintptr_t)&imp,
                          .pos  = e->pos};
  return op_check(env, &opi);
}

ANN m_bool check_implicit(const Env env, const Exp e, const Type t) {
  if (e->type == t) return GW_OK;
  if (isa(e->type, t) > 0) return GW_OK;
  const Symbol sym = insert_symbol("@implicit");
  return (e->cast_to = check_internal(env, sym, e, t)) ? GW_OK : GW_ERROR;
}

ANN m_bool check_subscripts(Env env, const Array_Sub array,
                            const m_bool is_decl) {
  CHECK_OB(check_exp(env, array->exp));
  m_uint depth = 0;
  Exp    e     = array->exp;
  do
    if (is_decl) CHECK_BB(check_implicit(env, e, env->gwion->type[et_int]));
  while (++depth && (e = e->next));
  if (depth != array->depth)
    ERR_B(array->exp->pos, _("invalid array access expression."))
  return GW_OK;
}

ANN static inline m_bool check_exp_decl_parent(const Env      env,
                                               const Var_Decl *var) {
  const Value value = find_value(env->class_def->info->parent, var->xid);
  if (value) {
    env_err(env, var->pos, _("Value defined in parent class"));
    defined_here(value);
    return GW_ERROR;
  }
  return GW_OK;
}

#define describe_check_decl(a, b, flag)                            \
  ANN static inline void decl_##a(const Env env, const Value v) {  \
    const Nspc nspc = env->curr;                                   \
    flag;                                                          \
    v->from->offset = nspc->b;                                     \
    nspc->b += v->type->size;                                      \
  }

describe_check_decl(member, offset, v->vflag |= vflag_member);
describe_check_decl(static, class_data_size, SET_FLAG(v, static));

ANN static m_bool check_var(const Env env, const Var_Decl *var) {
  if (env->class_def && !env->scope->depth && env->class_def->info->parent)
    CHECK_BB(check_exp_decl_parent(env, var));
  if (var->value->type->effects.ptr) {
    const Vector v = &var->value->type->effects;
    for (m_uint i = 0; i < vector_size(v); i++)
      env_add_effect(env, (Symbol)vector_at(v, i), var->pos);
  }
  return GW_OK;
}

ANN static m_bool check_var_td(const Env env, const Var_Decl *var,
                               Type_Decl *const td) {
  const Value v = var->value;
  if (env->class_def) {
    if (vflag(v, vflag_member)) {
      decl_member(env, v);
      if (env->class_def->info->tuple) tuple_info(env, v);
    } else if (GET_FLAG(td, static))
      decl_static(env, v);
  }
  return GW_OK;
}

ANN static m_bool check_decl(const Env env, const Exp_Decl *decl) {
  const Var_Decl *vd = &decl->vd;
  CHECK_BB(check_var(env, vd));
  CHECK_BB(check_var_td(env, vd, decl->td));
  valid_value(env, vd->xid, vd->value);
  // set_vflag(var->value, vflag_used));
  return GW_OK;
}

ANN /*static inline*/ m_bool ensure_check(const Env env, const Type t) {
  if (tflag(t, tflag_check) || !(tflag(t, tflag_cdef) || tflag(t, tflag_udef)))
    return GW_OK;
  struct EnvSet es = {.env   = env,
                      .data  = env,
                      .func  = (_exp_func)check_cdef,
                      .scope = env->scope->depth,
                      .flag  = tflag_check};
  return envset_run(&es, t);
}

ANN m_bool ensure_traverse(const Env env, const Type t) {
  if (tflag(t, tflag_check) || !(tflag(t, tflag_cdef) || tflag(t, tflag_udef)))
    return GW_OK;
  struct EnvSet es = {.env   = env,
                      .data  = env,
                      .func  = (_exp_func)traverse_cdef,
                      .scope = env->scope->depth,
                      .flag  = tflag_check};
  return envset_run(&es, t);
}

ANN static inline m_bool inferable(const Env env, const Type t,
                                   const loc_t pos) {
  if (!tflag(t, tflag_infer)) return GW_OK;
  ERR_B(pos, _("can't infer type."))
}

ANN Type check_exp_decl(const Env env, const Exp_Decl *decl) {
  if (decl->td->array && decl->td->array->exp)
    CHECK_OO(check_exp(env, decl->td->array->exp));
  if (decl->td->xid == insert_symbol("auto")) { // should be better
    CHECK_BO(scan1_exp(env, exp_self(decl)));
    CHECK_BO(scan2_exp(env, exp_self(decl)));
  }
  if (!decl->type) ERR_O(decl->td->pos, _("can't find type"));
  {
    CHECK_BO(ensure_check(env, decl->type));
    if(inferable(env, decl->type, decl->td->pos) < 0) {
      if(!tflag(decl->type, tflag_check) && decl->type->ref > 1 && env->class_def && !env->scope->depth)
        type_remref(decl->type, env->gwion);
      return NULL;
   }
  }
  const m_bool global = GET_FLAG(decl->td, global);
  const m_uint scope  = !global ? env->scope->depth : env_push_global(env);
  const m_bool ret    = check_decl(env, decl);
  if (global) env_pop(env, scope);
  env_weight(env, 1 + isa(decl->type, env->gwion->type[et_object]) > 0);
  return ret > 0 ? decl->vd.value->type : NULL;
}

ANN static m_bool check_collection(const Env env, Type type, const Exp e,
                                   const loc_t loc) {
  const Type common = find_common_anc(e->type, type);
  if (common) return GW_OK;
  if (check_implicit(env, e, type) > 0) return GW_OK;

  char fst[20 + strlen(type->name)];
  sprintf(fst, "expected `{+/}%s{0}`", type->name);
  gwerr_basic(_("literal contains incompatible types"), fst, "the first element determines the type", env->name,
              loc, 0);
  // suggested fix: rewrite int 2 as float 2.0"
  char sec[16 + strlen(e->type->name)];
  sprintf(sec, "got `{+/}%s{0}`", e->type->name);
  gwerr_secondary(sec, env->name, e->pos);
  return GW_ERROR;
}

ANN static inline Type prim_array_match(const Env env, Exp e) {
  const Type  type = e->type;
  bool        err  = false;
  const loc_t loc  = e->pos;
  do
    if (check_collection(env, type, e, loc) < 0) err = true;
  while ((e = e->next));
  if (!err) return array_type(env, array_base_simple(type), type->array_depth + 1);
  env_set_error(env, true);
  return NULL;
}

ANN static Type check_prim_array(const Env env, const Array_Sub *data) {
  const Array_Sub array = *data;
  const Exp       e     = array->exp;
  if (!e)
    ERR_O(prim_pos(data), _("must provide values/expressions for array [...]"))
  CHECK_OO(check_exp(env, e));
  env_weight(env, 1);
  return array->type = prim_array_match(env, e);
}

ANN static m_bool check_range(const Env env, Range *range) {
  if (range->start) CHECK_OB(check_exp(env, range->start));
  if (range->end) CHECK_OB(check_exp(env, range->end));
  if (range->start && range->end) {
    if (isa(range->end->type, range->start->type) < 0)
      ERR_B(range->start->pos, _("range types do not match"))
  }
  return GW_OK;
}

ANN static Type check_prim_range(const Env env, Range **data) {
  Range *range = *data;
  CHECK_BO(check_range(env, range));
  env_weight(env, 1);
  const Exp e = range->start ?: range->end;
  assert(e);
  const Symbol     sym = insert_symbol("@range");
  struct Op_Import opi = {.op   = sym,
                          .rhs  = e->type,
                          .pos  = e->pos,
                          .data = (uintptr_t)prim_exp(data)};
  return op_check(env, &opi);
}

ANN static Type check_prim_dict(const Env env, Exp *data) {
  const Exp base = *data;
  CHECK_OO(check_exp(env, base));
  const Type  key = base->type;
  const Type  val = base->next->type;
  bool        err = false;
  const loc_t loc = base->pos;
  Exp e = base;
  env_weight(env, 1);
  do {
    if (check_collection(env, key, e, loc) < 0) err = true;
    e = e->next;
    if (check_collection(env, val, e, loc) < 0) err = true;
  } while ((e = e->next));
  if (!err) return dict_type(env->gwion, key, val, base->pos);
  env_set_error(env, true); return NULL;
}

ANN m_bool not_from_owner_class(const Env env, const Type t, const Value v,
                                const loc_t pos) {
  if (!v->from->owner_class || isa(t, v->from->owner_class) < 0) {
    ERR_B(pos, _("'%s' from owner namespace '%s' used in '%s'."), v->name,
          v->from->owner ? v->from->owner->name : "?", t->name)
  }
  return GW_OK;
}

ANN static inline Value get_value(const Env env, const Symbol sym) {
  const Value value = nspc_lookup_value1(env->curr, sym);
  if(value) {
    if (!value->from->owner_class || isa(env->class_def, value->from->owner_class) > 0)
      return value;
  }
  if (env->func && env->func->def->base->values) {
    DECL_OO(const Value, v, = upvalues_lookup(env->func->def->base->values, sym));
    if(fbflag(env->func->def->base, fbflag_lambda))
      CHECK_OO(not_upvalue(env, v));
    return v;
  }
  return NULL;
}

ANN static inline bool is_value_global(const Env env, const Value v) {
  if(GET_FLAG(v, global)) return true;
  return from_global_nspc(env, v->from->owner);
}

ANN static Value check_non_res_value(const Env env, const Symbol *data) {
  const Symbol var   = *data;
  const Value  value = get_value(env, var);
  if (env->class_def) {
    if (value && value->from->owner_class)
      CHECK_BO(
          not_from_owner_class(env, env->class_def, value, prim_pos(data)));
    const Value v = value ?: find_value(env->class_def, var);
    if (v) {
      if (env->func && GET_FLAG(env->func->def->base, static) &&
          vflag(v, vflag_member))
        ERR_O(prim_pos(data),
              _("non-static member '%s' used from static function."),
              s_name(var))
    }
    return v;
  } else if (SAFE_FLAG(env->class_def, global) ||
             (env->func && GET_FLAG(env->func->def->base, global))) {
    if (!value || !(is_value_global(env, value) || vflag(value, vflag_arg)))
      ERR_O(prim_pos(data),
            _("non-global variable '%s' used from global function/class."),
            s_name(var))
  } else if(env->func && fbflag(env->func->def->base, fbflag_locale)) {
    if(!is_func(env->gwion, value->type) && value->from->owner && !from_global_nspc(env, value->from->owner)) // is_callable
      ERR_O(prim_pos(data), _("invalid variable access from locale definition"));
  }
  return value;
}

ANN static Type check_dot(const Env env, const Exp_Dot *member) {
  struct Op_Import opi = {.op   = insert_symbol("@dot"),
                          .lhs  = member->base->type,
                          .data = (uintptr_t)member,
                          .pos  = exp_self(member)->pos};
  env_weight(env, 1);
  return op_check(env, &opi);
}

static inline Nspc value_owner(const Env env, const Value v) {
  return v ? v->from->owner : env->curr;
}

ANN static m_bool check_upvalue(const Env env, const Exp_Primary *prim, const Value v) {
  if(not_upvalue(env, v))
    return GW_OK;
  gwerr_basic(_("value not in lambda scope"), NULL, NULL, env->name, exp_self(prim)->pos, 4242);
  gwerr_secondary_from("declared here", v->from);
  gw_err("{-}hint:{0} try adding it to capture list");
  env_set_error(env,  true);
  return GW_ERROR;
}

ANN static Type prim_owned(const Env env, const Symbol *data) {
  const Exp   exp  = exp_self(prim_exp(data));
  const Value v    = exp->d.prim.value;
  const m_str name = !GET_FLAG(v, static) ? "this" : v->from->owner_class->name;
  const Exp   base =
      new_prim_id(env->gwion->mp, insert_symbol(name), prim_pos(data));
  exp_setuse(base, 1);
  exp->exp_type       = ae_exp_dot;
  exp->d.exp_dot.base = base;
  exp->d.exp_dot.xid  = *data;
  return check_exp(env, exp);
}

ANN static Type prim_id_non_res(const Env env, const Symbol *data) {
  const Symbol sym = *data;
  const Value  v   = check_non_res_value(env, data);
  if (!v || !vflag(v, vflag_valid) || (v->from->ctx && v->from->ctx->error)) {
    const m_str name = s_name(*data);
    if (!isalpha(*name) && *name != '_') {
      prim_self(data)->value = env->gwion->type[et_op]->info->value;
      return env->gwion->type[et_op];
    }
    if (env->func && fbflag(env->func->def->base, fbflag_lambda) && env->func->def->base->values) {
      const Value v = upvalues_lookup(env->func->def->base->values, sym);
      if(v) {
        CHECK_BO(check_upvalue(env, prim_self(data), v));
        return v->type;
      }
    }
    gwerr_basic(_("Invalid variable"), _("not legit at this point."), NULL,
                env->name, prim_pos(data), 0);
    did_you_mean_nspc(v ? value_owner(env, v) : env->curr, s_name(sym));
    env_set_error(env, true);
    return NULL;
  }
  prim_self(data)->value = v;
  if (v->from->owner_class) return prim_owned(env, data);
  if (GET_FLAG(v, const)) exp_setmeta(prim_exp(data), 1);
  if (env->func) {
    if (!GET_FLAG(v, const) && v->from->owner)
      unset_fflag(env->func, fflag_pure);
    if (fbflag(env->func->def->base, fbflag_lambda))
      CHECK_BO(check_upvalue(env, prim_self(data), v));
  }
  // set_vflag(v->vflag, vflag_used);
  return v->type;
}

ANN Type check_prim_str(const Env env, const struct AstString *data) {
  if (!prim_self(data)->value)
    prim_self(data)->value = global_string(env, data->data, prim_pos(data));
  return env->gwion->type[et_string]; // prim->value
}

ANN static Type check_prim_id(const Env env, const Symbol *data) {
  struct SpecialId_ *spid = specialid_get(env->gwion, *data);
  if (spid) return specialid_type(env, spid, prim_self(data));
  return prim_id_non_res(env, data);
}

ANN static Type check_prim_perform(const Env env, const Symbol *data) {
  env_add_effect(env, *data, prim_pos(data));
  env_weight(env, 1);
  return env->gwion->type[et_void];
}

ANN static Type check_prim_interp(const Env env, const Exp *exp) {
  CHECK_OO(check_exp(env, *exp));
  env_weight(env, 1);
  return env->gwion->type[et_string];
}

ANN static Type check_prim_hack(const Env env, const Exp *data) {
  if (env->func) unset_fflag(env->func, fflag_pure);
  CHECK_OO(check_prim_interp(env, data));
  env_weight(env, 1);
  return env->gwion->type[et_gack];
}

ANN static Type check_prim_locale(const Env env, const Symbol *data NUSED) {
  return env->gwion->type[et_float];
}

#define describe_prim_xxx(name, type)                                          \
  ANN static Type check_prim_##name(const Env env               NUSED,         \
                                    const union prim_data *data NUSED) {       \
    return type;                                                               \
  }

describe_prim_xxx(num, env->gwion->type[et_int]);
describe_prim_xxx(char, env->gwion->type[et_char]);
describe_prim_xxx(float, env->gwion->type[et_float]);
describe_prim_xxx(nil, env->gwion->type[et_void]);

DECL_PRIM_FUNC(check, Type, Env);

ANN static Type check_prim(const Env env, Exp_Primary *prim) {
  return exp_self(prim)->type = check_prim_func[prim->prim_type](env, &prim->d);
}

ANN Type check_array_access(const Env env, const Array_Sub array) {
  const Symbol     sym = insert_symbol("@array");
  struct Op_Import opi = {.op   = sym,
                          .lhs  = array->exp->type,
                          .rhs  = array->type,
                          .pos  = array->exp->pos,
                          .data = (uintptr_t)array};
  return op_check(env, &opi);
}

static ANN Type check_exp_array(const Env env, const Exp_Array *array) {
  CHECK_OO((array->array->type = check_exp(env, array->base)));
  CHECK_BO(check_subscripts(env, array->array, 0));
  return check_array_access(env, array->array);
}

static ANN Type check_exp_slice(const Env env, const Exp_Slice *range) {
  CHECK_OO(check_exp(env, range->base));
  CHECK_BO(check_range(env, range->range));
  env_weight(env, 1);
  const Symbol sym = insert_symbol("@slice");
  const Exp    e   = range->range->start ?: range->range->end;
  assert(e);
  struct Op_Import opi = {.op   = sym,
                          .lhs  = e->type,
                          .rhs  = range->base->type,
                          .pos  = e->pos,
                          .data = (uintptr_t)exp_self(range)};
  return op_check(env, &opi);
}

// get the type of the function
// without the mangling
ANN static inline Type type_list_base_func(const Type type) {
  const Nspc owner = type->info->value->from->owner;
  const Symbol xid = type->info->func->def->base->xid;
  return nspc_lookup_type0(owner, xid);
}

ANN static inline Type type_list_base(const Gwion gwion, const Type type) {
  return !(is_func(gwion, type)) ? type : type_list_base_func(type);
}

ANN static Type_Decl* mk_td(const Env env, const Arg *arg,
                                  const Type type, const loc_t pos) {
  const Type base = type_list_base(env->gwion, type);
  const Type t    = !arg->td->array ?
          base : array_type(env, base, arg->td->array->depth);
  return type2td(env->gwion, t, pos);
}

ANN2(1, 2)
static Func find_func_match_actual(const Env env, const Func f, const Exp exp,
                                   const bool implicit, const bool specific) {
  Func func = f;
  do {
    Exp e = exp;
    Arg_List args = func->def->base->args;
    m_uint i = 0;
    const m_uint args_len = mp_vector_len(args);
    while (e) {
      e->cast_to = NULL;
      if (!e->type) // investigate
        return NULL;
      Arg *arg = i < args_len ? mp_vector_at(args, Arg, i++) : NULL;
      if (!arg) {
        CHECK_OO(func->next);
        return find_func_match_actual(env, func->next, exp, implicit,
                                      specific);
      }
      if (tflag(e->type, tflag_ref) && isa(e->type, arg->type) > 0) {
        if(!e->cast_to)e->cast_to = arg->type;
      }

      if (arg->type == env->gwion->type[et_auto]) {
        const Type owner = func->value_ref->from->owner_class;
        if (owner) CHECK_BO(template_push(env, owner));
        arg->type = known_type(env, arg->td);
        if (owner) nspc_pop_type(env->gwion->mp, env->curr);
        CHECK_OO(arg->type);
      }

      if (!func_match_inner(env, e, arg->type, implicit, specific)) break;
      e  = e->next;
    }
    if (!e && args_len == i) return func;
  } while ((func = func->next));
  return NULL;
}

ANN static Type check_exp_call(const Env env, Exp_Call *exp);

ANN static Func call2ufcs(const Env env, Exp_Call *call, const Value v) {
  const Exp this               = call->func->d.exp_dot.base;
  this->next                   = call->args;
  call->args                   = this;
  call->func->type             = v->type;
  call->func->d.prim.value     = v;
  call->func->d.prim.d.var     = call->func->d.exp_dot.xid;
  call->func->exp_type         = ae_exp_primary;
  call->func->d.prim.prim_type = ae_prim_id;
  CHECK_OO(check_exp_call(env, call));
  return call->func->type->info->func;
}

ANN Func ufcs(const Env env, const Func up, Exp_Call *const call) {
  const Value v = nspc_lookup_value1(env->curr, up->def->base->xid);
  if (v && is_func(env->gwion, v->type) && !v->from->owner_class) // is_callable
    return call2ufcs(env, call, v);
  return NULL;
}

ANN Func find_func_match(const Env env, const Func up, Exp_Call *const call) {
  Func      func;
  const Exp exp = call->args;
  const Exp args =
      (exp && isa(exp->type, env->gwion->type[et_void]) < 0) ? exp : NULL;
  if ((func = find_func_match_actual(env, up, args, false, true)) ||
      (func = find_func_match_actual(env, up, args, true, true)) ||
      (func = find_func_match_actual(env, up, args, false, true)) ||
      (func = find_func_match_actual(env, up, args, true, false)))
    return func;
  return call->func->exp_type == ae_exp_dot && up->value_ref->from->owner_class
             ? ufcs(env, up, call)
             : NULL;
}

ANN m_bool check_traverse_fdef(const Env env, const Func_Def fdef) {
  struct Vector_ v     = {};
  const m_uint scope   = env->scope->depth;
  env->scope->depth    = 0;
  vector_init(&v);
  while (vector_size((Vector)&env->curr->info->value->ptr) > 1)
    vector_add(&v, vector_pop((Vector)&env->curr->info->value->ptr));
  const m_bool ret = traverse_func_def(env, fdef);
  for (m_uint i = vector_size(&v) + 1; --i;)
    vector_add((Vector)&env->curr->info->value->ptr, vector_at(&v, i - 1));
  vector_release(&v);
  env->scope->depth     = scope;
  return ret;
}

ANN static m_bool check_func_args(const Env env, Arg_List args) {
  for(uint32_t i = 0; i < args->len; i++) {
    Arg *arg = mp_vector_at(args, Arg, i);
    const Var_Decl *decl = &arg->var_decl;
    const Value    v    = decl->value;
    if(decl->xid) CHECK_BB(already_defined(env, decl->xid, decl->pos));
    valid_value(env, decl->xid, v);
  }
  return GW_OK;
}

#define next_arg(type)                                                         \
  ANN static inline type next_arg_##type(const type e) {                       \
    const type next = e->next;                                                 \
    if (next) gw_err(", ");                                                    \
    return next;                                                               \
  }
next_arg(Exp)
//next_arg(Arg_List);

ANN static void print_current_args(Exp e) {
  gw_err(_("and not\n  "));
  do gw_err(" {G}%s{0}", e->type ? e->type->name : "<Unknown>");
  while ((e = next_arg_Exp(e)));
  gw_err("\n");
}

ANN2(1)
static void function_alternative(const Env env, const Type t, const Exp args,
                                 const loc_t pos) {
  if (env->context && env->context->error) // needed for ufcs
    return;
  gwerr_basic("Argument type mismatch", "call site",
              "valid alternatives:", env->name, pos, 0);
  Func up = isa(t, env->gwion->type[et_closure]) < 0
          ?  t->info->func : closure_def(t)->base->func;
  do print_signature(up);
  while ((up = up->next));
  if (args)
    print_current_args(args);
  else
    gw_err(_("and not:\n  {G}void{0}\n"));
  env_set_error(env, true);
}

ANN static Func get_template_func(const Env env, Exp_Call *const func,
                                  const Value v) {
  const Func f = find_template_match(env, v, func);
  if (f) {
    Tmpl *tmpl               = new_tmpl_call(env->gwion->mp, func->tmpl->call);
    tmpl->list               = v->d.func_ref
                                   ? v->d.func_ref->def->base->tmpl->list
                                   : func->func->type->info->func->def->base->tmpl->list;
    ((Exp_Call *)func)->tmpl = tmpl;
    func->func->type         = f->value_ref->type;
    return f;
  }
  ((Exp_Call *)func)->tmpl = NULL;
  assert(exp_self(func));
  ERR_O(exp_self(func)->pos,
        _("function is a template for which automatic type inference is not fully implemented"));
}

ANN static Func predefined_func(const Env env, const Value v, Exp_Call *exp,
                                const Tmpl *tm) {
  Tmpl tmpl = {.call = tm->call};
  exp->tmpl = &tmpl;
  DECL_OO(const Func, func, = get_template_func(env, exp, v));
  return v->d.func_ref = func;
}

ANN static Type check_predefined(const Env env, Exp_Call *exp, const Value v,
                                 const Tmpl *tm, const Func_Def fdef) {
  DECL_OO(const Func, func,
          = v->d.func_ref ?: predefined_func(env, v, exp, tm));
  if (!fdef->base->ret_type) { // template fptr
    struct EnvSet es = {.env   = env,
                        .data  = env,
                        .func  = (_exp_func)check_cdef,
                        .scope = env->scope->depth,
                        .flag  = tflag_check};
    CHECK_BO(envset_pushv(&es, v));
    func->def->base->fbflag |= fbflag_internal;
    const m_bool ret = check_traverse_fdef(env, func->def);
    if (es.run) envset_pop(&es, v->from->owner_class);
    CHECK_BO(ret);
  }
  exp->func->type = func->value_ref->type;
  return func->def->base->ret_type;
}

ANN static Type_List check_template_args(const Env env, Exp_Call *exp,
                                         const Tmpl *tm, const Func_Def fdef) {
  Specialized_List sl = tm->list;
  const bool spread = is_spread_tmpl(fdef->base->tmpl);
  const uint32_t len = sl->len - spread;
  Type_List    tl = new_mp_vector(env->gwion->mp, Type_Decl*, len);

  m_uint       args_number = 0;
  for(uint32_t i = 0; i < len; i++) {
    Specialized *spec = mp_vector_at(sl, Specialized, i);
    Arg_List args          = fdef->base->args;
    const uint32_t args_len = mp_vector_len(args);
    Exp      template_arg = exp->args;
    uint32_t count = 0;
    while (count < args_len && template_arg) {
      Arg *arg = mp_vector_at(args, Arg, count);
      if (spec->xid == arg->td->xid) {
        mp_vector_set(tl, Type_Decl*, args_number,
          mk_td(env, arg, template_arg->type, fdef->base->pos));
        ++args_number;
        break;
      }
      count++;
      template_arg = template_arg->next;
    }
  }
  if (args_number < len) //TODO: free type_list
    ERR_O(exp->func->pos, _("not able to infer types for template call."))

  if(spread) {
    Exp e = exp->args;
    if(fdef->base->args)
    for(uint32_t i = 0; i < fdef->base->args->len && e; i++) e = e->next;
    while(e) {
      mp_vector_add(env->gwion->mp, &tl, Type_Decl*, type2td(env->gwion, e->type, e->pos));
      e = e->next;
    }
  }
  return tl;
}

ANN static Type check_exp_call_template(const Env env, Exp_Call *exp) {
  /*const */Type t = exp->func->type;
  if(isa(t, env->gwion->type[et_closure]) > 0) {
    const Func_Def fdef = closure_def(t);
    t = fdef->base->func->value_ref->type;
  }
  DECL_OO(const Value, value, = type_value(env->gwion, t));
  const Func_Def fdef =
      value->d.func_ref ? value->d.func_ref->def : t->info->func->def;
  Tmpl *tm = fdef->base->tmpl;
  if (tm->call) return check_predefined(env, exp, value, tm, fdef);
  DECL_OO(const Type_List, tl, = check_template_args(env, exp, tm, fdef));
  Tmpl tmpl               = {.call = tl};
  ((Exp_Call *)exp)->tmpl = &tmpl;
  DECL_OO(const Func, func, = get_template_func(env, exp, value));
  return func->def->base->ret_type != env->gwion->type[et_auto] ?
    func->def->base->ret_type : exp->func->d.exp_dot.base->type;
}

ANN Type upvalue_type(const Env env, Capture *cap);

ANN static m_bool lambda_args_ref(const Env env, Exp_Call *const call) {
  Exp e = call->args;
  CHECK_OB(check_exp(env, e));
  do if(tflag(e->type, tflag_ref) && !safe_tflag(exp_self(e)->cast_to, tflag_ref))
     exp_setvar(e, true);
  while((e = e->next));
  return GW_OK;
}

ANN2(1) static m_bool lambda_append_args(const Env env, Exp_Call *const call, const Exp add) {
  if(!add) return GW_ERROR;
  if (call->args) {
    Exp e = call->args;
    while(e->next) e = e->next;
    e->next = add;
  } else call->args = add;
  return traverse_exp(env, add);
}

ANN static Exp check_lambda_captures(const Env env, const Func_Def fdef) {
  if(!fdef->base->args)
    fdef->base->args = new_mp_vector(env->gwion->mp, Arg, 0);
  Exp args = NULL, tmp;
  for(uint32_t i = 0; i < fdef->captures->len; i++) {
    Capture *const cap = mp_vector_at(fdef->captures, Capture, i);
    const Type t = upvalue_type(env, cap);
    if(!t) {
      if(args) free_exp(env->gwion->mp, args);
      return NULL;
    }
    Arg arg = {
      .td = type2td(env->gwion, t, cap->pos),
      .var_decl = { .xid = cap->xid }
    };
    mp_vector_add(env->gwion->mp, &fdef->base->args, Arg, arg);
    const Exp exp = new_prim_id(env->gwion->mp, cap->xid, cap->pos);
    if(args) tmp = tmp->next = exp;
    else args = tmp = exp;
  }
  free_mp_vector(env->gwion->mp, Capture, fdef->captures);
  fdef->captures = NULL;
  return args;
}

ANN static Type check_lambda_call(const Env env, Exp_Call *const exp) {
  const Func_Def fdef = exp->func->d.exp_lambda.def;
  const bool captures = !!fdef->captures;
  if (exp->args) CHECK_BO(lambda_args_ref(env, exp));
  const Exp _args = !captures ? NULL : check_lambda_captures(env, fdef);
  if(captures) CHECK_BO(lambda_append_args(env, exp, _args));
  Exp_Lambda *l   = &exp->func->d.exp_lambda;
  Arg_List    args = l->def->base->args;
  Exp         e   = exp->args;
  for(uint32_t i = 0; i < (args ? args->len : 0); i++) {
    if(!e)
      ERR_O(exp_self(exp)->pos, _("argument number does not match for lambda"))
    Arg *arg = mp_vector_at(args, Arg, i);
    arg->type = e->type;
    if(is_class(env->gwion, arg->type))
      type_addref(arg->type);
    e = e->next;
  }
  if(e)
     ERR_O(exp_self(exp)->pos, _("argument number does not match for lambda"))
  Upvalues upvalues = { .values = env->curr->info->value};
  if(env->func && env->func->def->base->values)
    upvalues.parent = env->func->def->base->values;
  l->def->base->values = &upvalues;
  const m_bool ret     = traverse_func_def(env, l->def);
  if (l->def->base->func) {
    free_scope(env->gwion->mp, env->curr->info->value);
    env->curr->info->value = l->def->base->values->values;
    if (env->class_def) set_vflag(l->def->base->func->value_ref, vflag_member);
    exp->func->type = l->def->base->func->value_ref->type;
    if (!l->def->base->ret_type)
      l->def->base->ret_type = env->gwion->type[et_void];
  }
  return ret > 0 ? l->def->base->ret_type : NULL;
}

ANN m_bool func_check(const Env env, Exp_Call *const exp) {
  if(exp->func->exp_type == ae_exp_dot)
    exp->func->d.exp_dot.is_call = exp;
  CHECK_OB(check_exp(env, exp->func));
  if(exp->func->exp_type == ae_exp_dot)
    exp->func->d.exp_dot.is_call = exp;
  if (exp->func->exp_type == ae_exp_decl)
    ERR_B(exp->func->pos, _("Can't call late function pointer at declaration "
                            "site. did you meant to use `@=>`?"))
  const Type t = actual_type(env->gwion, exp->func->type);
  if (is_func(env->gwion, t) && exp->func->exp_type == ae_exp_dot && // is_callable
      !t->info->value->from->owner_class) {
    if (exp->args) CHECK_OB(check_exp(env, exp->args));
    return call2ufcs(env, exp, t->info->func->value_ref) ? GW_OK : GW_ERROR;
  }
  const Exp        e   = exp_self(exp);
  struct Op_Import opi = {.op   = insert_symbol("@func_check"),
                          .rhs  = t,
                          .pos  = e->pos,
                          .data = (uintptr_t)e};
  CHECK_NB(op_check(env, &opi)); // doesn't really return NULL
  if (e->exp_type != ae_exp_call) return 0;
  return e->type != env->gwion->type[et_error] ? GW_OK : GW_ERROR;
}

ANN void call_add_effect(const Env env, const Func func, const loc_t pos) {
  if (func != env->func && func->def->base->effects.ptr) {
    const Vector v = &func->def->base->effects;
    for (m_uint i = 0; i < vector_size(v); i++)
      env_add_effect(env, (Symbol)vector_at(v, i), pos);
  }
}

ANN Type _check_exp_call1(const Env env, Exp_Call *const exp) {
  /* const */Type t = exp->func->type;
  if (!is_func(env->gwion, t)) { // use func flag?
    if(isa(exp->func->type, env->gwion->type[et_closure]) > 0)
      t = closure_def(t)->base->func->value_ref->type;
    else {
      struct Op_Import opi = {.op   = insert_symbol("@ctor"),
                              .rhs  = actual_type(env->gwion, exp->func->type),
                              .data = (uintptr_t)exp,
                              .pos  = exp_self(exp)->pos};
      const Type       t   = op_check(env, &opi);
      return t;
    }
  }
  if (t == env->gwion->type[et_op]) return check_op_call(env, exp);
  if (!t->info->func) // TODO: effects?
    return check_lambda_call(env, exp);
  if (fflag(t->info->func, fflag_ftmpl)) {
    const Value value = t->info->func->value_ref;
    if (value->from->owner_class)
      CHECK_BO(ensure_traverse(env, value->from->owner_class));
  }
  if (exp->args) {
    CHECK_OO(check_exp(env, exp->args));
    Exp e = exp->args;
    do exp_setuse(e, true);
    while((e = e->next));
  }
  if (tflag(t, tflag_ftmpl))
    return check_exp_call_template(env, (Exp_Call *)exp); // TODO: effects?
  const Func func = find_func_match(env, t->info->func, exp);
  if (func) {
    if (func != env->func && func->def && !fflag(func, fflag_valid)) {
      if(func->value_ref->from->owner_class)
        CHECK_BO(ensure_check(env, func->value_ref->from->owner_class));
      else {
        const m_uint scope = env_push(env, NULL, func->value_ref->from->owner);
        const m_bool ret = check_func_def(env, func->def);
        env_pop(env, scope);
        CHECK_BO(ret);
      }
    }
    exp->func->type = func->value_ref->type;
    call_add_effect(env, func, exp->func->pos);
    return func->def->base->ret_type != env->gwion->type[et_auto] ?
      func->def->base->ret_type : exp->func->d.exp_dot.base->type;
  }
  if(exp->func->exp_type == ae_exp_lambda) {
    const Type tt = partial_type(env, exp);
    if(tt) return tt;
  }
  return NULL;
}

ANN Type check_exp_call1(const Env env, Exp_Call *const exp) {
  DECL_BO(const m_bool, ret, = func_check(env, exp));
  if (!ret) return exp_self(exp)->type;
  const Type t = exp->func->type;
  const Type _ret = _check_exp_call1(env, exp);
  if(_ret) return _ret;
  if(isa(exp->func->type, env->gwion->type[et_closure]) > 0) {
    if(exp->func->exp_type == ae_exp_dot && t->info->value->from->owner_class) {
      const Exp args = exp->args;
      const Exp this_arg = cpy_exp(env->gwion->mp, exp->func->d.exp_dot.base);
      this_arg->next = args;
      exp->args = this_arg;
      const Type t = _check_exp_call1(env, exp);
      if(t) return t;
    }
  }
  function_alternative(env, t, exp->args, exp->func->pos);
  return NULL;
}

ANN static Type check_exp_binary(const Env env, const Exp_Binary *bin) {
  CHECK_OO(check_exp(env, bin->lhs));
  const m_bool is_auto = (bin->op == insert_symbol("=>") || bin->op == insert_symbol("@=>")) &&
                         bin->rhs->exp_type == ae_exp_decl &&
                         bin->rhs->d.exp_decl.type == env->gwion->type[et_auto];
  if (is_auto) bin->rhs->d.exp_decl.type = bin->lhs->type;
  CHECK_OO(check_exp(env, bin->rhs));
  if (is_auto) {
    assert(bin->rhs->type == bin->lhs->type);
    set_vflag(bin->rhs->d.exp_decl.vd.value, vflag_assigned);
  }
  struct Op_Import opi = {.op   = bin->op,
                          .lhs  = bin->lhs->type,
                          .rhs  = bin->rhs->type,
                          .data = (uintptr_t)bin,
                          .pos  = exp_self(bin)->pos};
  exp_setuse(bin->lhs, 1);
  exp_setuse(bin->rhs, 1);
  const Type ret = op_check(env, &opi);
  if (!ret && is_auto && exp_self(bin)->exp_type == ae_exp_binary)
    bin->rhs->d.exp_decl.vd.value->type = env->gwion->type[et_auto];
  return ret;
}

ANN static Type check_exp_cast(const Env env, const Exp_Cast *cast) {
  DECL_OO(const Type, t, = check_exp(env, cast->exp));
  CHECK_OO((exp_self(cast)->type = known_type(env, cast->td)));
  struct Op_Import opi = {.op   = insert_symbol("$"),
                          .lhs  = t,
                          .rhs  = exp_self(cast)->type,
                          .data = (uintptr_t)cast,
                          .pos  = exp_self(cast)->pos};
  return op_check(env, &opi);
}

ANN static Type check_exp_post(const Env env, const Exp_Postfix *post) {
  struct Op_Import opi = {.op   = post->op,
                          .lhs  = check_exp(env, post->exp),
                          .data = (uintptr_t)post,
                          .pos  = exp_self(post)->pos};
  CHECK_OO(opi.lhs);
  exp_setuse(post->exp, 1);
  const Type t = op_check(env, &opi);
  if (t && isa(t, env->gwion->type[et_object]) < 0)
    exp_setmeta(exp_self(post), 1);
  return t;
}

ANN static m_bool predefined_call(const Env env, const Type t,
                                  const loc_t pos) {
  const m_str str =
      tl2str(env->gwion, t->info->func->def->base->tmpl->call, pos);
  env_err(env, pos, _("Type '%s' has '%s' as pre-defined types."), t->name,
          str);
  free_mstr(env->gwion->mp, str);
  if (tflag(t, tflag_typedef)) {
    gwerr_secondary("from definition:", env->name,
                    t->info->func->def->base->pos);
  }
  return GW_ERROR;
}

ANN2(1) static inline bool is_partial(const Env env, Exp exp) {
  while (exp) {
    if (is_hole(env, exp) || is_typed_hole(env, exp))
      return true;
    exp = exp->next;
  }
  return false;
}

ANN static bool tl_match(const Env env, const Type_List tl0, const Type_List tl1) {
  if (tl0->len != tl1->len) return false;
  for(uint32_t i = 0; i < tl0->len; i++) {
    Type_Decl *td0 = *mp_vector_at(tl0, Type_Decl*, i);
    Type_Decl *td1 = *mp_vector_at(tl1, Type_Decl*, i);
    if(known_type(env, td0) != known_type(env, td1))
      return false;
  }
  return true;
}

ANN static Type check_exp_call_tmpl(const Env env, Exp_Call *exp, const Type t) {
  if (exp->args) CHECK_OO(check_exp(env, exp->args));
  if (!t->info->func->def->base->tmpl)
    ERR_O(exp_self(exp)->pos, _("template call of non-template function."))
  if (t->info->func->def->base->tmpl->call) {
    if (env->func == t->info->func) {
      exp->func->type = env->func->value_ref->type;
      return env->func->def->base->ret_type;
    } else {
      if(tl_match(env, t->info->func->def->base->tmpl->call, exp->tmpl->call))
        return check_exp_call1(env, exp);
      CHECK_BO(predefined_call(env, t, exp_self(exp)->pos));
    }
  }
  const Value v = type_value(env->gwion, t);
  DECL_OO(const Func, f, = find_template_match(env, v, exp));
  exp->func->type = f->value_ref->type;
  return f->def->base->ret_type;
}

ANN static Type check_exp_call(const Env env, Exp_Call *exp) {
  if (is_partial(env, exp->args)) {
    CHECK_OO(check_exp(env, exp->func));
    struct Op_Import opi = {.op   = insert_symbol("@partial"),
                            .lhs  = exp->func->type,
                            .pos  = exp->func->pos,
                            .data = (uintptr_t)exp};
    return op_check(env, &opi);
  }
  if (exp->tmpl) {
    DECL_BO(const m_bool, ret, = func_check(env, exp));
    if (!ret) return exp_self(exp)->type;
/*    const */Type t = actual_type(env->gwion, exp->func->type);
    if(isa(exp->func->type, env->gwion->type[et_closure]) > 0) {
      t = typedef_base(t);
      t = mp_vector_at(t->info->cdef->body, Section , 0)->d.func_def->base->func->value_ref->type;
    }
// check for closure and b ring it back
    if (!is_func(env->gwion, t)) return check_exp_call1(env, exp);
    if(strcmp("new", s_name(t->info->func->def->base->xid)))
      return check_exp_call_tmpl(env, exp, t);
  }
  return check_exp_call1(env, exp);
}

ANN static Type check_exp_unary(const Env env, const Exp_Unary *unary) {
  const Type rhs =
      unary->unary_type == unary_exp ? check_exp(env, unary->exp) : NULL;
  if (unary->unary_type == unary_exp) {
    CHECK_OO(rhs);
    exp_setuse(unary->exp, 1);
  }
  struct Op_Import opi = {.op   = unary->op,
                          .rhs  = rhs,
                          .data = (uintptr_t)unary,
                          .pos  = exp_self(unary)->pos};
  DECL_OO(const Type, ret, = op_check(env, &opi));
  const Type t = actual_type(env->gwion, ret);
  CHECK_BO(ensure_traverse(env, t));
  return ret;
}

ANN static Type _flow(const Env env, const Exp e, const m_bool b) {
  DECL_OO(const Type, type, = check_exp(env, e));
  struct Op_Import opi = {
      .op   = insert_symbol(b ? "@conditional" : "@unconditional"),
      .rhs  = type,
      .pos  = e->pos,
      .data = (uintptr_t)e};
  return op_check(env, &opi);
}
#define check_flow(emit, b) _flow(emit, b, 1)

ANN static Type check_exp_if(const Env env, Exp_If *const exp_if) {
  if (!exp_if->if_exp) {
    const Exp e = exp_if->if_exp = cpy_exp(env->gwion->mp, exp_if->cond);
    scan1_exp(env, e);
    scan2_exp(env, e);
  }
  DECL_OO(const Type, cond, = check_flow(env, exp_if->cond));
  DECL_OO(const Type, if_exp, = check_exp(env, exp_if->if_exp));
  DECL_OO(const Type, else_exp, = check_exp(env, exp_if->else_exp));

  const uint meta =
      exp_getmeta(exp_if->if_exp) || exp_getmeta(exp_if->else_exp);
  exp_setmeta(exp_self(exp_if), meta);
  const Type ret = find_common_anc(if_exp, else_exp);
  if (!ret)
    ERR_O(exp_self(exp_if)->pos,
          _("incompatible types '%s' and '%s' in if expression..."),
          if_exp->name, else_exp->name)
  if (isa(if_exp, else_exp) < 0)
    ERR_O(exp_self(exp_if)->pos, _("condition type '%s' does not match '%s'"),
          cond->name, ret->name)
  exp_setuse(exp_if->cond, true);
  exp_setuse(exp_if->if_exp, true);
  exp_setuse(exp_if->else_exp, true);
  return ret;
}

ANN static Type check_exp_dot(const Env env, Exp_Dot *member) {
  CHECK_OO(check_exp(env, member->base));
  return check_dot(env, member);
}

ANN m_bool check_type_def(const Env env, const Type_Def tdef) {
  if (tdef->when) {
    set_tflag(tdef->type, tflag_contract);
    struct Var_Decl_ decl = { .xid = insert_symbol("self"), .pos = tdef->when->pos };
    Type_Decl *td = cpy_type_decl(env->gwion->mp, tdef->ext);
    Arg_List args = new_mp_vector(env->gwion->mp, Arg, 1);
    mp_vector_set(args, Arg, 0, ((Arg) { .td = td, .var_decl = decl }));
    Func_Base *fb = new_func_base(
        env->gwion->mp, type2td(env->gwion, tdef->type, tdef->pos),
        insert_symbol("@implicit"), args, ae_flag_none, tdef->pos);
    set_fbflag(fb, fbflag_op);
    const Exp helper = new_prim_id(env->gwion->mp, insert_symbol("@predicate"),
                                   tdef->when->pos);
    const Exp when   = tdef->when;
    tdef->when = NULL;
    when->next       = helper;
    Stmt_List body = new_mp_vector(env->gwion->mp, struct Stmt_, 2);
    mp_vector_set(body, struct Stmt_, 0,
      ((struct Stmt_) {
      .stmt_type = ae_stmt_exp, .d = { .stmt_exp = { .val = when }},
      .pos = when->pos
    }));
    mp_vector_set(body, struct Stmt_, 1,
      ((struct Stmt_) {
      .stmt_type = ae_stmt_exp,
      .pos = when->pos
    }));
    const Stmt     code = new_stmt_code(env->gwion->mp, body, when->pos);
    const Func_Def fdef = new_func_def(env->gwion->mp, fb, code);
    tdef->when_def           = fdef;
    CHECK_BB(traverse_func_def(env, fdef));
    if (isa(when->type, env->gwion->type[et_bool]) < 0) {
      char explain[strlen(when->type->name) + 20];
      sprintf(explain, "found `{/+}%s{0}`", when->type->name);
      gwerr_basic("Invalid `{/+}when{0}` predicate expression type", explain,
                  "use `{/+}bool{0}`", env->name, when->pos, 0);
      char from[strlen(tdef->type->name) + 39];
      sprintf(from, "in `{/+}%s{0}` definition", tdef->type->name);
      gwerr_secondary(from, env->name, tdef->pos);
      env_set_error(env, true);
      return GW_ERROR;
    }
    // we handle the return after, so that we don't get *cant' use implicit
    // casting while defining it*
    const Exp ret_id = new_prim_id(env->gwion->mp, insert_symbol("self"), when->pos);
    ret_id->d.prim.value = new_value(env, tdef->type, "self", tdef->pos);
    struct Stmt_ ret = {
      .stmt_type = ae_stmt_return, .d = { .stmt_exp = { .val = ret_id }},
      .pos = when->pos
    };
    mp_vector_set(fdef->d.code->d.stmt_code.stmt_list, struct Stmt_, 1, ret);
    ret_id->type = tdef->type;
  }
  return GW_OK;
}
ANN static Type check_exp_lambda(const Env env, const Exp_If *exp_if NUSED) {
  return env->gwion->type[et_function];
}

ANN static Type check_exp_td(const Env env, Type_Decl **td) {
  DECL_OO(const Type, t, = known_type(env, *td));
  if(t == env->gwion->type[et_class])
    ERR_O(exp_self(td)->pos, "can't use {G+}Class{0} in type decl expression");
  if (!is_func(env->gwion, t)) return type_class(env->gwion, t);
  return t;
}

DECL_EXP_FUNC(check, Type, Env)

ANN Type check_exp(const Env env, const Exp exp) {
  Exp curr = exp;
  if (!exp->type) {
    env_weight(env, 1);
    do {
      CHECK_OO((curr->type = check_exp_func[curr->exp_type](env, &curr->d)));
      if (env->func && !is_class(env->gwion, curr->type) &&
          is_func(env->gwion, curr->type) &&
          !safe_fflag(curr->type->info->func, fflag_pure))
        unset_fflag(env->func, fflag_pure);
    } while ((curr = curr->next));
  }
  return exp->type;
}

ANN m_bool check_enum_def(const Env env, const Enum_Def edef) {
  const m_uint scope = env_push_type(env, edef->t);
  ID_List list = edef->list;
  for(uint32_t i = 0; i < list->len; i++) {
    Symbol xid = *mp_vector_at(list, Symbol, i);
    decl_static(env, nspc_lookup_value0(env->curr, xid));
  }
  env_pop(env, scope);
  nspc_allocdata(env->gwion->mp, edef->t->nspc);
  return GW_OK;
}

ANN static m_bool check_stmt_code(const Env env, const Stmt_Code stmt) {
  if (stmt->stmt_list) { RET_NSPC(check_stmt_list(env, stmt->stmt_list)) }
  return GW_OK;
}

ANN static inline m_bool _check_breaks(const Env env, const Stmt b) {
    RET_NSPC(check_stmt(env, b))}

ANN static m_bool check_breaks(const Env env, const Stmt a, const Stmt b) {
  vector_add(&env->scope->breaks, (vtype)a);
  const m_bool ret = _check_breaks(env, b);
  vector_pop(&env->scope->breaks);
  return ret;
}

ANN static m_bool check_conts(const Env env, const Stmt a, const Stmt b) {
  vector_add(&env->scope->conts, (vtype)a);
  CHECK_BB(check_breaks(env, a, b));
  vector_pop(&env->scope->conts);
  return GW_OK;
}

ANN static inline m_bool for_empty(const Env env, const Stmt_For stmt) {
  if (!stmt->c2 || !stmt->c2->d.stmt_exp.val)
    ERR_B(stmt_self(stmt)->pos,
          _("empty for loop condition..."
            "...(note: explicitly use 'true' if it's the intent)"
            "...(e.g., 'for(; true;){{ /*...*/ }')"))
  return GW_OK;
}

ANN static void check_idx(const Env env, const Type base, struct EachIdx_ *const idx) {
  idx->v = new_value(env, base, s_name(idx->sym), idx->pos);
  valid_value(env, idx->sym, idx->v);
  SET_FLAG(idx->v, const);
}

/** sets for the key expression value
    with eg  type *int* for an array or the *Key* type of a Dict **/
ANN static m_bool check_each_idx(const Env env, const Exp exp, struct EachIdx_ *const idx) {
  struct Op_Import opi = {
    .lhs = exp->type,
    .op  = insert_symbol("@each_idx"),
    .data = (m_uint)exp,
    .pos = idx->pos
  };
  DECL_OB(const Type, t, = op_check(env, &opi));
  check_idx(env, t, idx);
  return GW_OK;
}

/** return the base type for the foreach expression
    eg the base type of an array or the *Val* type of a Dict **/
ANN static Type check_each_val(const Env env, const Exp exp) {
  struct Op_Import opi = {
    .lhs  = exp->type,
    .op   = insert_symbol("@each_val"),
    .data = (m_uint)exp,
    .pos = exp->pos
  };
  return op_check(env, &opi);
}

ANN static m_bool do_stmt_each(const Env env, const Stmt_Each stmt) {
  CHECK_OB(check_exp(env, stmt->exp));
  if (stmt->idx)
    CHECK_BB(check_each_idx(env, stmt->exp, stmt->idx));
  DECL_OB(const Type, ret, = check_each_val(env, stmt->exp));
  stmt->v = new_value(env, ret, s_name(stmt->sym), stmt->vpos);
  valid_value(env, stmt->sym, stmt->v);
  return check_conts(env, stmt_self(stmt), stmt->body);
}

ANN static m_bool do_stmt_repeat(const Env env, const Stmt_Loop stmt) {
  if (stmt->idx) check_idx(env, env->gwion->type[et_int], stmt->idx);
  return check_conts(env, stmt_self(stmt), stmt->body);
}

ANN static inline m_bool repeat_type(const Env env, const Exp e) {
  const Type t_int = env->gwion->type[et_int];
  if (check_implicit(env, e, t_int) < 0) {
    char explain[40 + strlen(e->type->name)];
    sprintf(explain, "expected `{/+}int{0}`, got `{/+}%s{0}`", e->type->name);
    gwerr_basic(_("invalid repeat condition type"), explain,
                _("use an integer or cast to int if possible"), env->name,
                e->pos, 0);
    env_set_error(env, true);
    return GW_ERROR;
  }
  return GW_OK;
}

#define stmt_func_xxx(name, type, prolog, exp)                                 \
  describe_stmt_func(check, name, type, prolog, exp)
stmt_func_xxx(if, Stmt_If,, !(!check_flow(env, stmt->cond)   ||
  check_stmt(env, stmt->if_body) < 0 ||
  (stmt->else_body && check_stmt(env, stmt->else_body) < 0)) ? 1 : -1)
stmt_func_xxx(flow, Stmt_Flow, env_inline_mult(env, 1.5),
  !(!check_exp(env, stmt->cond) ||
    !_flow(env, stmt->cond, !stmt->is_do ?
       stmt_self(stmt)->stmt_type == ae_stmt_while :
       stmt_self(stmt)->stmt_type != ae_stmt_while) ||
    check_conts(env, stmt_self(stmt), stmt->body) < 0) ? 1 : -1)
stmt_func_xxx(for, Stmt_For, env_inline_mult(env, 1.5), !(
  for_empty(env, stmt) < 0 ||
  check_stmt(env, stmt->c1) < 0 ||
  !check_flow(env, stmt->c2->d.stmt_exp.val) ||
  (stmt->c3 && !check_exp(env, stmt->c3)) ||
  check_conts(env, stmt_self(stmt), stmt->body) < 0) ? 1 : -1)
stmt_func_xxx(loop, Stmt_Loop, env_inline_mult(env, 1.5); check_idx(env, stmt->idx), !(!check_exp(env, stmt->cond) ||
  repeat_type(env, stmt->cond) < 0 ||
  do_stmt_repeat(env, stmt) < 0) ? 1 : -1)
stmt_func_xxx(each, Stmt_Each, env_inline_mult(env, 1.5), do_stmt_each(env, stmt))

ANN static m_bool check_stmt_return(const Env env, const Stmt_Exp stmt) {
  if (!strcmp(s_name(env->func->def->base->xid), "new")) {
    if(stmt->val)
      ERR_B(stmt_self(stmt)->pos,
            _("'return' statement inside constructor function should have no expression"))
    return GW_OK;
  }
  DECL_OB(const Type, ret_type,
          = stmt->val ? check_exp(env, stmt->val) : env->gwion->type[et_void]);
  if (!env->func->def->base->ret_type) {
    env->func->def->base->ret_type = ret_type;
    return GW_OK;
  }
  if (isa(ret_type, env->func->def->base->ret_type) > 0) return GW_OK;
  if (tflag(ret_type, tflag_noret))
    ERR_B(stmt->val->pos, _("Can't use type `{+G}%s{+G}` for return"),
          ret_type->name);
  if (stmt->val) {
    Arg *arg = mp_vector_at(env->func->def->base->args, Arg, 0);
    if (env->func->def->base->xid == insert_symbol("@implicit") &&
        ret_type == arg->type)
      ERR_B(stmt_self(stmt)->pos,
            _("can't use implicit casting while defining it"))
    if (check_implicit(env, stmt->val, env->func->def->base->ret_type) > 0)
      return GW_OK;
    ERR_B(stmt_self(stmt)->pos,
          _("invalid return type: got '%s', expected '%s'"), ret_type->name,
          env->func->def->base->ret_type->name)
  }
  if (isa(env->func->def->base->ret_type, env->gwion->type[et_void]) > 0)
    return GW_OK;
  ERR_B(stmt_self(stmt)->pos, _("missing value for return statement"))
}

#define describe_check_stmt_stack(stack, name)                                 \
  ANN static m_bool check_stmt_##name(const Env env, const Stmt stmt) {        \
    if (!vector_size(&env->scope->stack))                                      \
      ERR_B(stmt->pos, _("'" #name "' found outside of for/while/until..."))   \
    return GW_OK;                                                              \
  }
describe_check_stmt_stack(conts, continue);
describe_check_stmt_stack(breaks, break);

ANN m_bool check_union_def(const Env env NUSED, const Union_Def udef) {
  if (tmpl_base(udef->tmpl)) // there's a func for this
    return GW_OK;
  set_tflag(udef->type, tflag_check);
  return GW_OK;
}

ANN static m_bool check_stmt_exp(const Env env, const Stmt_Exp stmt) {
  if(stmt->val) {
    CHECK_OB(check_exp(env, stmt->val));
    if(stmt->val->exp_type == ae_exp_lambda) {
     const loc_t loc = stmt->val->d.exp_lambda.def->base->pos;
     env_warn(env, loc, _("Partial application not used"));
    }
  }
  return stmt->val ? check_exp(env, stmt->val) ? 1 : -1 : 1;
}

ANN static Value match_value(const Env env, const Type base,
                             const Exp_Primary *prim) {
  const Symbol sym = prim->d.var;
  const Value  v   = new_value(env, base, s_name(sym), prim_pos(prim));
  // valuefrom?
  valid_value(env, sym, v);
  return v;
}

ANN static Symbol case_op(const Env env, const Type base, const Exp e) {
  if (e->exp_type == ae_exp_primary) {
    if (e->d.prim.prim_type == ae_prim_id) {
      if (e->d.prim.d.var == insert_symbol("_")) return NULL;
      if (!nspc_lookup_value1(env->curr, e->d.prim.d.var)) {
        e->d.prim.value = match_value(env, base, &e->d.prim);
        return NULL;
      }
    }
  } else if (isa(base, env->gwion->type[et_union]) > 0 &&
             e->exp_type == ae_exp_call) {
    const Exp func = e->d.exp_call.func;
    if (func->d.prim.prim_type == ae_prim_id) {
      const Value v = find_value(base, func->d.prim.d.var);
      if (v) {
        e->type = v->type;
        case_op(env, v->type, e->d.exp_call.args);
        e->d.exp_call.args->type = v->type;
        return NULL;
      }
    }
  }
  return insert_symbol("?=");
}

ANN static m_bool match_case_exp(const Env env, Exp e) {
  Exp last = e;
  for (m_uint i = 0; i < vector_size(&env->scope->match->cond); e = e->next, ++i) {
    if (!e) ERR_B(last->pos, _("no enough to match"))
    last              = e;
    const Exp    base = (Exp)vector_at(&env->scope->match->cond, i);
    const Symbol op   = case_op(env, base->type, e);
    if (op) {
      const Exp next = e->next;
      e->next        = NULL;
      const Type t   = check_exp(env, e);
      e->next        = next;
      CHECK_OB(t);
      Exp_Binary       bin  = {.lhs = cpy_exp(env->gwion->mp, base), .rhs = cpy_exp(env->gwion->mp, e), .op = op};
      struct Exp_      ebin = {.d = {.exp_binary = bin}, .exp_type = ae_exp_binary, .pos = e->pos };
      struct Op_Import opi  = {.op   = op,
                              .lhs  = base->type,
                              .rhs  = e->type,
                              .data = (uintptr_t)&ebin.d.exp_binary,
                              .pos  = e->pos};
      traverse_exp(env, &ebin);
      const Type ret = op_check(env, &opi);
      if(ebin.exp_type == ae_exp_binary) {
        free_exp(env->gwion->mp, bin.lhs);
        free_exp(env->gwion->mp, bin.rhs);
      }
      CHECK_OB(ret);
    }
  }
  if (e) ERR_B(e->pos, _("too many expression to match"))
  return GW_OK;
}

ANN static m_bool _check_stmt_case(const Env env, const Stmt_Match stmt) {
  CHECK_BB(match_case_exp(env, stmt->cond));
  if (stmt->when) CHECK_OB(check_flow(env, stmt->when));
  return check_stmt_list(env, stmt->list);
}

ANN static m_bool check_stmt_case(const Env env, const Stmt_Match stmt) {
    RET_NSPC(_check_stmt_case(env, stmt))}

ANN static m_bool case_loop(const Env env, const Stmt_Match stmt) {
  for(m_uint i = 0; i < stmt->list->len; i++) {
    const Stmt s = mp_vector_at(stmt->list, struct Stmt_, i);
    CHECK_BB(check_stmt_case(env, &s->d.stmt_match));
  }
  return GW_OK;
}

ANN static inline m_bool check_handler(const restrict Env env,
                                            const Handler *handler) {
  RET_NSPC(check_stmt(env, handler->stmt));
}

ANN static inline m_bool check_handler_list(const restrict Env env,
                                            const Handler_List handlers) {
  for(uint32_t i = 0; i < handlers->len; i++) {
    Handler *handler = mp_vector_at(handlers, Handler, i);
    CHECK_BB(check_handler(env, handler));
  }
  return GW_OK;
}

ANN static inline bool find_handler(const Handler_List handlers, const Symbol xid) {
  for(uint32_t i = 0; i < handlers->len; i++) {
    Handler *handler = mp_vector_at(handlers, Handler, i);
    if(xid == handler->xid) return true;
  }
  return false;
}

ANN static inline m_bool check_stmt_try_start(const restrict Env env,
                                              const Stmt_Try     stmt) {
    RET_NSPC(check_stmt(env, stmt->stmt))
}

ANN static inline m_bool _check_stmt_try(const restrict Env env, const Stmt_Try stmt) {
  CHECK_BB(check_handler_list(env, stmt->handler));
  vector_add(&env->scope->effects, 0);
  const m_bool ret = check_stmt_try_start(env, stmt);
  MP_Vector *const v = (MP_Vector*)vector_pop(&env->scope->effects);
  if (v) {
    for (m_uint i = 0; i < v->len; i++) {
      struct ScopeEffect *eff = mp_vector_at(v, struct ScopeEffect, i);
      bool found   = find_handler(stmt->handler, eff->sym);
      if (!found) env_add_effect(env, eff->sym, eff->pos);
    }
    free_mp_vector(env->gwion->mp, struct ScopeEffect, v);
  }
  return ret;
}

ANN static inline m_bool check_stmt_try(const restrict Env env,
                                        const Stmt_Try     stmt) {
  const bool   in_try = env->scope->in_try;
  const m_bool ret    = _check_stmt_try(env, stmt);
  env->scope->in_try  = in_try;
  return ret;
}

ANN static m_bool _check_stmt_match(const Env env, const Stmt_Match stmt) {
  CHECK_OB(check_exp(env, stmt->cond));
  MATCH_INI(env->scope)
  const m_bool ret = case_loop(env, stmt);
  MATCH_END(env->scope)
  return ret;
}

ANN static inline m_bool handle_where(const Env env, const Stmt_Match stmt) {
  if (stmt->where) CHECK_BB(check_stmt(env, stmt->where));
  RET_NSPC(_check_stmt_match(env, stmt))
}

ANN static m_bool check_stmt_match(const Env env, const Stmt_Match stmt) {
  RET_NSPC(handle_where(env, stmt))
}

#define check_stmt_while check_stmt_flow
#define check_stmt_until check_stmt_flow

ANN static m_bool check_stmt_pp(const Env env, const Stmt_PP stmt) {
  if (stmt->pp_type == ae_pp_include) env->name = stmt->data;
  // check for memoization
  else if (env->func && stmt->pp_type == ae_pp_pragma &&
      !strncmp(stmt->data, "memoize", strlen("memoize")))
    env->func->memoize = strtol(stmt->data + 7, NULL, 10);
  else if(stmt->pp_type == ae_pp_locale) {
    const loc_t loc = stmt_self(stmt)->pos;
    const Exp id   = new_prim_id(env->gwion->mp, stmt->xid, loc);
    const Exp arg   = new_prim_id(env->gwion->mp, insert_symbol("_"), loc);
    arg->next = stmt->exp;
    const Exp call = new_exp_call(env->gwion->mp, id, arg, loc);
    stmt->exp = call;
    CHECK_BB(traverse_exp(env, id));
    CHECK_OB(partial_type(env, &call->d.exp_call));
  }
  return GW_OK;
}

ANN static m_bool check_stmt_defer(const Env env, const Stmt_Defer stmt) {
  return check_stmt(env, stmt->stmt);
}

#define check_stmt_retry dummy_func
#define check_stmt_spread dummy_func
DECL_STMT_FUNC(check, m_bool, Env)

ANN m_bool check_stmt(const Env env, const Stmt stmt) {
  return check_stmt_func[stmt->stmt_type](env, &stmt->d);
}

ANN static m_bool check_stmt_list(const Env env, Stmt_List l) {
  for(m_uint i = 0; i < l->len; i++) {
    const Stmt s = mp_vector_at(l, struct Stmt_, i);
    CHECK_BB(check_stmt(env, s));
  }
  return GW_OK;
}

ANN static m_bool check_signature_match(const Env env, const Func_Def fdef,
                                        const Func parent) {
  if (GET_FLAG(parent->def->base, final))
    ERR_B(fdef->base->td->pos, _("can't override final function '%s'\n"),
          parent->name)
  if (GET_FLAG(parent->def->base, static) != GET_FLAG(fdef->base, static)) {
    const m_str c_name = fdef->base->func->value_ref->from->owner_class->name;
    const m_str p_name = parent->value_ref->from->owner_class->name;
    const m_str f_name = s_name(fdef->base->xid);
    ERR_B(fdef->base->td->pos,
          _("function '%s.%s' ressembles '%s.%s' but cannot override...\n"
            "  ...(reason: '%s.%s' is declared as 'static')"),
          c_name, f_name, p_name, c_name,
          GET_FLAG(fdef->base, static) ? c_name : p_name, f_name)
  }
  if(fdef->base->tmpl || isa(fdef->base->ret_type, parent->def->base->ret_type) > 0)
    return GW_OK;
  gwerr_basic_from("invalid overriding", NULL, NULL, fdef->base->func->value_ref->from, 0);
  gwerr_secondary_from("does not match", parent->value_ref->from);
  env_set_error(env,  true);
  return GW_ERROR;
}

ANN static m_bool parent_match_actual(const Env               env,
                                      const restrict Func_Def fdef,
                                      const restrict Func     func) {
  Func parent_func = func;
  do {
    if (parent_func->def->base && compat_func(fdef, parent_func->def) > 0) {
      CHECK_BB(check_signature_match(env, fdef, parent_func));
      if (!fdef->base->tmpl) {
        fdef->vt_index = parent_func->def->vt_index;
        vector_set(&env->curr->vtable, fdef->vt_index,
                   (vtype)fdef->base->func);
      }
      return GW_OK;
    }
  } while ((parent_func = parent_func->next));
  return 0;
}

ANN static m_bool check_parent_match(const Env env, const Func_Def fdef) {
  const Func func   = fdef->base->func;
  const Type parent = env->class_def->info->parent;
  if (!env->curr->vtable.ptr) vector_init(&env->curr->vtable);
  if (parent) {
    const Value v = find_value(parent, fdef->base->xid);
    if (v && is_func(env->gwion, v->type)) {
      const m_bool match = parent_match_actual(env, fdef, v->d.func_ref);
      if (match) return match;
    }
  }
  func->def->vt_index = vector_size(&env->curr->vtable);
  vector_add(&env->curr->vtable, (vtype)func);
  return GW_OK;
}

ANN static inline Func get_overload(const Env env, const Func_Def fdef,
                                    const m_uint i) {
  const Symbol sym =
      func_symbol(env, env->curr->name, s_name(fdef->base->xid), NULL, i);
  return nspc_lookup_func1(env->curr, sym);
}

ANN static m_bool check_func_overload(const Env env, const Func_Def fdef) {
  const Value v = fdef->base->func->value_ref;
  for (m_uint i = 0; i <= v->from->offset; ++i) {
    const Func f1 = get_overload(env, fdef, i);
    for (m_uint j = i + 1; f1 && j <= v->from->offset; ++j) {
      const Func f2 = get_overload(env, fdef, j);
      if (f2 && compat_func(f1->def, f2->def) > 0 &&
          fbflag(f1->def->base, fbflag_op) ==
              fbflag(f2->def->base, fbflag_op) &&
          fbflag(f1->def->base, fbflag_unary) ==
              fbflag(f2->def->base, fbflag_unary) &&
          fbflag(f1->def->base, fbflag_postfix) ==
              fbflag(f2->def->base, fbflag_postfix))
        ERR_B(f2->def->base->pos,
              _("global function '%s' already defined"
                " for those arguments"),
              s_name(fdef->base->xid))
    }
  }
  return GW_OK;
}

ANN static bool check_effect_overload(const Vector base, const Func override) {
  if (!base->ptr) return true;
  if (!override->def->base->effects.ptr) return false;
  const Vector v = &override->def->base->effects;
  for (m_uint i = 0; i < vector_size(v); i++) {
    if (vector_find(base, vector_at(v, i)) == -1) return false;
  }
  return true;
}

ANN static m_bool check_func_def_override(const Env env, const Func_Def fdef,
                                          Value *ov) {
  const Func func = fdef->base->func;
  if (env->class_def && env->class_def->info->parent) {
    const Value override =
        find_value(env->class_def->info->parent, fdef->base->xid);
    if (override && override->from->owner_class &&
        !is_func(env->gwion, override->type))
      ERR_B(fdef->base->pos,
            _("function name '%s' conflicts with previously defined value...\n"
              "  from super class '%s'..."),
            s_name(fdef->base->xid), override->from->owner_class->name)
    *ov = override;
  }
  if (func->value_ref->from->offset &&
      (!fdef->base->tmpl /*|| !fdef->base->tmpl->base*/))
    CHECK_BB(check_func_overload(env, fdef));
  return GW_OK;
}

ANN m_bool check_fdef(const Env env, const Func_Def fdef) {
  if (fdef->base->args) CHECK_BB(check_func_args(env, fdef->base->args));
  if(fdef->builtin) return GW_OK;
  if (fdef->d.code && fdef->d.code->d.stmt_code.stmt_list)
    CHECK_BB(check_stmt_code(env, &fdef->d.code->d.stmt_code));
  return GW_OK;
}

ANN static bool effect_find(const MP_Vector *v, const Symbol sym) {
  for(m_uint i = 0; i < v->len; i++) {
    struct ScopeEffect *eff = mp_vector_at(v, struct ScopeEffect, i);
    if(eff->sym == sym) return true;
  }
  return false;
}

ANN m_bool _check_func_def(const Env env, const Func_Def f) {
  if (tmpl_base(f->base->tmpl) && fbflag(f->base, fbflag_op)) return GW_OK;
  const Func     func = f->base->func;
  const Func_Def fdef = func->def;
  if(fflag(func, fflag_valid))return GW_OK;
  set_fflag(func, fflag_valid);
  assert(func == fdef->base->func);
  if (env->class_def && !fbflag(func->def->base, fbflag_lambda))
    CHECK_BB(check_parent_match(env, fdef));
  if (tmpl_base(fdef->base->tmpl)) return GW_OK;
  Value override = NULL;
  CHECK_BB(check_func_def_override(env, fdef, &override));
  DECL_BB(const m_int, scope, = GET_FLAG(fdef->base, global)
                                    ? env_push_global(env)
                                    : env->scope->depth);
  const Func former = env->func;
  env->func         = func;
  nspc_push_value(env->gwion->mp, env->curr);
  struct Op_Import opi = {};
  if (fbflag(fdef->base, fbflag_op)) {
    func_operator(f, &opi);
    operator_suspend(env->curr, &opi);
  }
  if(fdef->captures) {
    uint32_t offset = fdef->stack_depth;
    for(uint32_t i = 0; i < fdef->captures->len; i++) {
      Capture *cap = mp_vector_at(fdef->captures, Capture, i);
      valid_value(env, cap->xid, cap->temp);
      cap->temp->from->offset = offset;
      offset += cap->temp->type->size;
    }
  }
  vector_add(&env->scope->effects, 0);
  const m_bool ret = scanx_fdef(env, env, fdef, (_exp_func)check_fdef);
  MP_Vector *v = (MP_Vector*)vector_back(&env->scope->effects);
  if (v) {
    if (fdef->base->xid == insert_symbol("@dtor"))
      ERR_B(fdef->base->pos, _("can't use effects in destructors"));
    const Vector base = &fdef->base->effects;
    if (!base->ptr) vector_init(base);
    for (uint32_t i = 0; i < v->len; i++) {
      struct ScopeEffect *eff = mp_vector_at(v, struct ScopeEffect, i);
      if(!effect_find(v, eff->sym))
        vector_add(base, (m_uint)eff->sym);
    }
    free_mp_vector(env->gwion->mp, struct ScopeEffect, v);
  }
  vector_pop(&env->scope->effects);
  if (fbflag(fdef->base, fbflag_op)) operator_resume(&opi);
  nspc_pop_value(env->gwion->mp, env->curr);
  env->func = former;
  if (ret > 0) {
    if (env->class_def && fdef->base->effects.ptr &&
        (override &&
         !check_effect_overload(&fdef->base->effects, override->d.func_ref)))
      ERR_B(fdef->base->pos, _("too much effects in override."),
            s_name(fdef->base->xid))
  }
  if (GET_FLAG(fdef->base, global)) env_pop(env, scope);
  if (func->value_ref->from->owner_class)
    func->inline_mult += 3;
  else
    func->inline_mult += 4;
  return ret;
}

ANN m_bool check_func_def(const Env env, const Func_Def fdef) {
  const uint16_t depth = env->scope->depth;
  env->scope->depth = 0;
  const m_bool ret = _check_func_def(env, fdef);
  env->scope->depth = depth;
  return ret;
}

ANN bool check_trait_requests(const Env env, const Type t, const ID_List list, const ValueFrom *from);
ANN static m_bool check_extend_def(const Env env, const Extend_Def xdef) {
  ValueFrom from = { .filename = env->name, .loc=xdef->td->pos, .ctx=env->context,
      .owner = env->curr, .owner_class = env->class_def
  };
  const bool ret = check_trait_requests(env, xdef->t, xdef->traits, &from);
  if(ret) {
    if(!xdef->t->info->traits) {
      xdef->t->info->traits = new_mp_vector(env->gwion->mp, Symbol, xdef->traits->len);
      for(uint32_t i = 0; i < xdef->traits->len; i++) {
        const Symbol sym = *mp_vector_at(xdef->t->info->traits, Symbol, i);
        mp_vector_set(xdef->t->info->traits, Symbol, i, sym);
      }
    } else {
      for(uint32_t i = 0; i < xdef->traits->len; i++) {
        const Symbol sym = *mp_vector_at(xdef->t->info->traits, Symbol, i);
        mp_vector_add(env->gwion->mp, &xdef->t->info->traits, Symbol, sym);
      }
    }
    return GW_OK;
  }
  return GW_ERROR;
}

ANN static m_bool _check_trait_def(const Env env, const Trait_Def pdef) {
  const Trait trait = nspc_lookup_trait1(env->curr, pdef->xid);
  Ast         ast   = pdef->body;
  if(!ast) return GW_OK;
  for(m_uint i = 0; i < ast->len; i++) {
    Section * section = mp_vector_at(ast, Section, i);
    // we should traverse the funcs here also
    if (section->section_type == ae_section_stmt) {
  Stmt_List l = section->d.stmt_list;
  for(m_uint i = 0; i < l->len; i++) {
    const Stmt stmt = mp_vector_at(l, struct Stmt_, i);
        if (stmt->stmt_type == ae_stmt_exp) {
          CHECK_BB(traverse_exp(env, stmt->d.stmt_exp.val));
          Var_Decl vd = stmt->d.stmt_exp.val->d.exp_decl.vd;
          const Value value = vd.value;
          valuefrom(env, value->from);
          if (!trait->var)
            trait->var = new_mp_vector(env->gwion->mp, Value, 0);
          mp_vector_add(env->gwion->mp, &trait->var, Value, value);
        }
      }
    }
  }
  return GW_OK;
}

ANN static m_bool check_trait_def(const Env env, const Trait_Def pdef) {
  RET_NSPC(_check_trait_def(env, pdef));
}

ANN m_bool check_fptr_def(const Env env, const Fptr_Def fptr) {
  return check_class_def(env, fptr->cdef);
}

//#define check_fptr_def dummy_func
HANDLE_SECTION_FUNC(check, m_bool, Env)

ANN static m_bool check_parent(const Env env, const Class_Def cdef) {
  const Type       parent = cdef->base.type->info->parent;
  const Type_Decl *td     = cdef->base.ext;
  if (td->array && td->array->exp)
    CHECK_BB(check_subscripts(env, td->array, 1));
  CHECK_BB(ensure_check(env, parent));
  //  if(tflag(parent, tflag_typedef)) {
  //    set_tflag(cdef->base.type, tflag_typedef);
  //  }
  return GW_OK;
}

ANN static m_bool cdef_parent(const Env env, const Class_Def cdef) {
  const bool tmpl = !!cdef->base.tmpl;
  if (tmpl) CHECK_BB(template_push_types(env, cdef->base.tmpl));
  const m_bool ret = check_parent(env, cdef);
  if (tmpl) nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}

ANN m_bool check_abstract(const Env env, const Class_Def cdef) {
  if (!cdef->base.type->nspc->vtable.ptr) return GW_OK;
  bool err = false;
  for (m_uint i = 0; i < vector_size(&cdef->base.type->nspc->vtable); ++i) {
    const Func f = (Func)vector_at(&cdef->base.type->nspc->vtable, i);
   if (f && f->def->base && GET_FLAG(f->def->base, abstract)) {
      if (!err) {
        err = true;
        gwerr_basic(_("missing function definition"),
                    _("must be declared 'abstract'"),
                    _("provide an implementation for the following:"),
                    env->name, cdef->pos, 0);
      }
      ValueFrom *from = f->value_ref->from;
      gwerr_secondary_from("implementation missing", from);
      env_set_error(env, true);
    }
  }
  return !err ? GW_OK : GW_ERROR;
}

ANN static inline void ctor_effects(const Env env) {
  const Vector v  = &env->scope->effects;
  MP_Vector *const w = (MP_Vector*)vector_back(v);
  if (!w) return;
  vector_init(&env->class_def->effects);
  for (uint32_t j = 0; j < w->len; j++) {
    struct ScopeEffect *eff = mp_vector_at(w, struct ScopeEffect, j);
    vector_add(&env->class_def->effects, (m_uint)eff->sym);
  }
  free_mp_vector(env->gwion->mp, struct ScopeEffect, w);
  vector_pop(v);
}

ANN static m_bool check_body(const Env env, Section *const section) {
  const m_bool ret = check_section(env, section);
  ctor_effects(env);
  return ret;
}

ANN static bool class_def_has_body(const Env env, Ast ast) {
//  do {
  for(m_uint i = 0; i < ast->len; i++) {
    const Section *section = mp_vector_at(ast, Section, i);
    if (section->section_type == ae_section_stmt) {
      Stmt_List l = section->d.stmt_list;
      for(m_uint i = 0; i < l->len; i++) {
        const Stmt stmt = mp_vector_at(l, struct Stmt_, i);
        if (stmt->stmt_type == ae_stmt_pp) continue;
        if (stmt->stmt_type == ae_stmt_exp) {
          const Exp exp = stmt->d.stmt_exp.val;
          if (!exp) continue;
          if (exp->exp_type != ae_exp_decl) return true;
          if (GET_FLAG(exp->d.exp_decl.td, late)) continue;
          Var_Decl vd = exp->d.exp_decl.vd;
          if (GET_FLAG(vd.value, late)) continue;
          if (isa(vd.value->type, env->gwion->type[et_compound]) > 0)
              return true;
        } else
          return true;
      } //while ((list = list->next));
    }
  } //while ((ast = ast->next));
  return false;
}

ANN static inline bool type_is_recurs(const Type t, const Type tgt) {
//  if(tflag(tgt, tflag_union)) return false;
  return isa(tgt, t) > 0 || isa(t, tgt) > 0 || (tgt->info->tuple && vector_find(&tgt->info->tuple->contains, (m_uint)t) > -1);
}

ANN static m_bool recursive_type_base(const Env env, const Type t);
ANN static bool recursive_type(const Env env, const Type t, const Type tgt);
ANN static bool recursive_value(const Env env, const Type t, const Value v) {
  const Type tgt = array_base(v->type);
  if(type_is_recurs(t, tgt)) {
    env_err(env, v->from->loc, _("recursive type"));
    env_set_error(env,  false);
    gwerr_secondary("in class", t->name, t->info->cdef->base.pos);

    const Type first = tgt->info->value->from->loc.first.line < t->info->value->from->loc.first.line ?
//      tgt : t;
      v->type : t;
    const Type second = tgt->info->value->from->loc.first.line > t->info->value->from->loc.first.line ?
//      tgt : t;
      v->type : t;

    if(first != second) {
      const Map m1 = &first->info->value->from->owner->info->type->map;
      map_remove(m1, (m_uint)insert_symbol(first->name));
      const Map m2 = &second->info->value->from->owner->info->type->map;
      map_remove(m2, (m_uint)insert_symbol(second->name));
      if(first->ref > 2)
        type_remref(first, env->gwion);
      if(second->ref > 2)
        type_remref(second, env->gwion);
      if(v->from->owner_class->ref > 2)
        type_remref(v->from->owner_class, env->gwion);
      if(t->ref > 2)
        type_remref(t, env->gwion);
    }
    set_tflag(t, tflag_error);
    set_tflag(t, tflag_infer);
    set_tflag(tgt, tflag_infer);
    unset_tflag(t, tflag_check);
    unset_tflag(tgt, tflag_check);
    return true;
  }

  if(t != tgt && v->type->nspc && (!GET_FLAG(v, late) ||  vflag(v, vflag_assigned)) && strncmp(tgt->name, "Option:[", 8) &&
      isa(tgt, env->gwion->type[et_compound]) > 0)
    return recursive_type(env, t, tgt);

  return false;
}

ANN static bool recursive_type(const Env env, const Type t, const Type tgt) {
  Value             v;
  struct scope_iter inner = {tgt->nspc->info->value, 0, 0};
  bool error = false;
  while (scope_iter(&inner, &v) > 0) {
    if((!GET_FLAG(v, late) || vflag(v, vflag_assigned)) && v->type != tgt && recursive_value(env, t, v)) {
      error = true;
    }
  }
  return error;
}

ANN static m_bool recursive_type_base(const Env env, const Type t) {
  Value             value;
  bool error = false;
  struct scope_iter iter = {t->nspc->info->value, 0, 0};
  while (scope_iter(&iter, &value) > 0) {
    if (isa(value->type, env->gwion->type[et_compound]) < 0) continue;
    if (value->type->nspc && (!GET_FLAG(value, late) || vflag(value, vflag_assigned))) {
      if(value->type == t || recursive_type(env, t, value->type)) {
        env_err(env, value->from->loc, _("recursive type"));
        gw_err("use {+G}late{0} on one (or more) of the variables?\n");
        error = true;
      }
    }
  }
  return error;
}

ANN bool check_trait_requests(const Env env, const Type t, const ID_List list, const ValueFrom *from);
ANN static m_bool _check_class_def(const Env env, const Class_Def cdef) {
  const Type t = cdef->base.type;
  if (cdef->base.ext) CHECK_BB(cdef_parent(env, cdef));
  if (!tflag(t, tflag_struct)) inherit(t);
  if (cdef->body) {
    CHECK_BB(env_body(env, cdef, check_body));
    if (cflag(cdef, cflag_struct) || class_def_has_body(env, cdef->body))
      set_tflag(t, tflag_ctor);
  }
/*
  // enforce new to be defined in every child class
  if(t->info->parent->nspc && nspc_lookup_value0(t->info->parent->nspc, insert_symbol("new")) && !nspc_lookup_value0(t->nspc, insert_symbol("new"))) {
    env_err(env, cdef->pos, "must define 'new' operator");
    env_warn(env, t->info->parent->info->value->from->loc, "defined here");
    return GW_ERROR;
  }
*/
  if (!GET_FLAG(cdef, abstract)) CHECK_BB(check_abstract(env, cdef));
  if (cdef->traits) {
    ID_List list        = cdef->traits;
    if (!check_trait_requests(env, t, list, t->info->value->from)) {
      env->class_def = t;
      env_error_footer(env);
      env_set_error(env, true);
      return GW_ERROR;
    }
  }
  CHECK_BB(recursive_type_base(env, t));
  nspc_allocdata(env->gwion->mp, t->nspc);
  return GW_OK;
}

ANN m_bool check_class_def(const Env env, const Class_Def cdef) {
  if (tmpl_base(cdef->base.tmpl)) return GW_OK;
  const Type       t   = cdef->base.type;
  if (tflag(t, tflag_check)) return GW_OK;
  set_tflag(t, tflag_check);
  return _check_class_def(env, t->info->cdef);
}

ANN static inline void check_unhandled(const Env env) {
  const Vector v  = &env->scope->effects;
  MP_Vector *const w = (MP_Vector*)vector_back(v);
  if(!w) return;
  for (m_uint j = 0; j < w->len; j++) {
    struct ScopeEffect *eff = mp_vector_at(w, struct ScopeEffect, j);
    if(s_name(eff->sym)[0] == '!')
      continue;
    gwerr_secondary("Unhandled effect", env->name, eff->pos);
    env_set_error(env,  false);
  }
  free_mp_vector(env->gwion->mp, struct ScopeEffect, w);
  vector_pop(v);
}

ANN static void check_extend(const Env env, Ast ast) {
  for(m_uint i = 0; i < ast->len; i++) {
    Section * section = mp_vector_at(ast, Section, i);
    (void)check_section(env, section);
    mp_vector_add(env->gwion->mp, &env->context->extend, Section, *section);
  }
  free_mp_vector(env->gwion->mp, Section, env->context->extend);
  env->context->extend = NULL;
}

ANN m_bool check_ast(const Env env, Ast *ast) {
  Ast a = *ast;
  for(m_uint i = 0; i < a->len; i++) {
    Section * section = mp_vector_at(a, Section, i);
    CHECK_BB(check_section(env, section));
  }
  check_unhandled(env);
  if(env->context->extend) check_extend(env, env->context->extend);
  return GW_OK;
}
