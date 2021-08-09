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
    ERR_B(array->exp->pos, _("invalid array acces expression."))
  return GW_OK;
}

ANN static inline m_bool check_exp_decl_parent(const Env      env,
                                               const Var_Decl var) {
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

ANN static m_bool check_fptr_decl(const Env env, const Var_Decl var) {
  const Value v = var->value;
  Type        t = v->type;
  while (tflag(t, tflag_typedef)) t = t->info->parent;
  if (!t->info->func) return GW_ERROR;
  if (!env->class_def) return GW_OK;
  const Func func = t->info->func;
  const Type type = func->value_ref->from->owner_class;
  if (type && isa(type, env->class_def) < 0 && !GET_FLAG(func, global))
    ERR_B(var->pos, _("can't use non global fptr of other class."))
  if (vflag(func->value_ref, vflag_member) && GET_FLAG(v, static))
    ERR_B(var->pos, _("can't use static variables for member function."))
  return GW_OK;
}

ANN static m_bool check_var(const Env env, const Var_Decl var) {
  if (env->class_def && !env->scope->depth && env->class_def->info->parent)
    CHECK_BB(check_exp_decl_parent(env, var));
  if (var->value->type->effects.ptr) {
    const Vector v = &var->value->type->effects;
    for (m_uint i = 0; i < vector_size(v); i++)
      env_add_effect(env, (Symbol)vector_at(v, i), var->pos);
  }
  if (var->array && var->array->exp)
    return check_subscripts(env, var->array, 1);
  return GW_OK;
}

ANN static m_bool check_var_td(const Env env, const Var_Decl var,
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
  Var_Decl_List list = decl->list;
  do {
    const Var_Decl var = list->self;
    CHECK_BB(check_var(env, var));
    CHECK_BB(check_var_td(env, var, decl->td));
    if (is_fptr(env->gwion, decl->type)) CHECK_BB(check_fptr_decl(env, var));
    set_vflag(var->value, vflag_valid);
    // set_vflag(var->value, vflag_used));
    nspc_add_value(env->curr, var->xid, var->value);
  } while ((list = list->next));
  return GW_OK;
}

ANN static inline m_bool ensure_check(const Env env, const Type t) {
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
    CHECK_BO(inferable(env, decl->type, decl->td->pos));
    CHECK_BO(ensure_check(env, decl->type));
  }
  const m_bool global = GET_FLAG(decl->td, global);
  const m_uint scope  = !global ? env->scope->depth : env_push_global(env);
  const m_bool ret    = check_decl(env, decl);
  if (global) env_pop(env, scope);
  return ret > 0 ? decl->list->self->value->type : NULL;
}

ANN static m_bool prim_array_inner(const Env env, Type type, const Exp e,
                                   const loc_t loc) {
  const Type common = find_common_anc(e->type, type);
  if (common) return GW_OK;
  if (check_implicit(env, e, type) > 0) return GW_OK;

  char fst[20 + strlen(type->name)];
  sprintf(fst, "expected `{+/}%s{0}`", type->name);
  gwerr_basic(_("literal contains incompatible types"), fst, NULL, env->name,
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
    if (prim_array_inner(env, type, e, loc) < 0) err = true;
  while ((e = e->next));
  if (!err) return array_type(env, array_base(type), type->array_depth + 1);
  env->context->error = true;
  return NULL;
}

ANN static Type check_prim_array(const Env env, const Array_Sub *data) {
  const Array_Sub array = *data;
  const Exp       e     = array->exp;
  if (!e)
    ERR_O(prim_pos(data), _("must provide values/expressions for array [...]"))
  CHECK_OO(check_exp(env, e));
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
  const Exp e = range->start ?: range->end;
  assert(e);
  const Symbol     sym = insert_symbol("@range");
  struct Op_Import opi = {.op   = sym,
                          .rhs  = e->type,
                          .pos  = e->pos,
                          .data = (uintptr_t)prim_exp(data)};
  return op_check(env, &opi);
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
  if (value) return value;
  if (env->func && env->func->def->base->values)
    return (Value)scope_lookup1(env->func->def->base->values, (vtype)sym);
  return NULL;
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
    if (!value || !GET_FLAG(value, global))
      ERR_O(prim_pos(data),
            _("non-global variable '%s' used from global function/class."),
            s_name(var))
  }
  return value;
}

ANN static Type check_dot(const Env env, const Exp_Dot *member) {
  struct Op_Import opi = {.op   = insert_symbol("@dot"),
                          .lhs  = member->base->type,
                          .data = (uintptr_t)member,
                          .pos  = exp_self(member)->pos};
  return op_check(env, &opi);
}

static inline Nspc value_owner(const Env env, const Value v) {
  return v ? v->from->owner : env->curr;
}

ANN static void check_upvalue(const Env env, const Exp_Primary *prim) {
  const Value v = prim->value;
  if (GET_FLAG(v, global) ||
      (v->from->owner_class && isa(v->from->owner_class, env->class_def) > 0) ||
      nspc_lookup_value1(env->curr, insert_symbol(v->name)))
    return;
  const Map map = &env->func->upvalues;
  if (!map->ptr) {
    map_init(map);
    map_set(&env->func->upvalues, (vtype)prim, 0);
  } else {
    if (map_get(map, (vtype)v)) return;
    const m_uint offset =
        VVAL(map, VLEN(map) - 1) +
        ((Exp_Primary *)VKEY(map, VLEN(map) - 1))->value->type->size;
    map_set(&env->func->upvalues, (vtype)prim, offset);
  }
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
    if (!isalpha(*name) && *name != '_') /* && *name != '@' ???*/
      return env->gwion->type[et_op];
    gwerr_basic(_("Invalid variable"), _("not legit at this point."), NULL,
                env->name, prim_pos(data), 0);
    did_you_mean_nspc(v ? value_owner(env, v) : env->curr, s_name(sym));
    env->context->error = true;
    return NULL;
  }
  prim_self(data)->value = v;
  if (v->from->owner_class) return prim_owned(env, data);
  if (GET_FLAG(v, const)) exp_setmeta(prim_exp(data), 1);
  if (env->func) {
    if (!GET_FLAG(v, const) && v->from->owner)
      unset_fflag(env->func, fflag_pure);
    if (fbflag(env->func->def->base, fbflag_lambda))
      check_upvalue(env, prim_self(data));
  }
  // set_vflag(v->vflag, vflag_used);
  return v->type;
}

ANN Type check_prim_str(const Env env, const m_str *data) {
  if (!prim_self(data)->value)
    prim_self(data)->value = global_string(env, *data);
  return env->gwion->type[et_string]; // prim->value
}

ANN static Type check_prim_id(const Env env, const Symbol *data) {
  struct SpecialId_ *spid = specialid_get(env->gwion, *data);
  if (spid) return specialid_type(env, spid, prim_self(data));
  return prim_id_non_res(env, data);
}

ANN static Type check_prim_perform(const Env env, const Symbol *data) {
  env_add_effect(env, *data, prim_pos(data));
  return env->gwion->type[et_void];
}

ANN static Type check_prim_interp(const Env env, const Exp *exp) {
  CHECK_OO(check_exp(env, *exp));
  return env->gwion->type[et_string];
}

ANN static Type check_prim_hack(const Env env, const Exp *data) {
  if (env->func) unset_fflag(env->func, fflag_pure);
  CHECK_OO(check_prim_interp(env, data));
  return env->gwion->type[et_gack];
}
/*
ANN static Type check_prim_map(const Env env, const Exp *data) {
  CHECK_OO(check_exp(env, *data));
  if(env->func) // really?
    unset_fflag(env->func, fflag_pure);
  bool err = false;
  Exp key = *data;
  Exp val = key->next;
  const Type type_key = key->type;
  const Type type_val = val->type;
  const loc_t loc_key = (*data)->pos;
  const loc_t loc_val = (*data)->next->pos;
  do {
    val = key->next;
    if(prim_array_inner(env, type_key, key, loc_key) < 0)
      err = true;
    if(prim_array_inner(env, type_val, val, loc_val) < 0)
      err = true;
  } while((key = val->next));
  if(!err) {
    Type_Decl *td_key = type2td(env->gwion, type_key, loc_key);
    Type_Decl *td_val = type2td(env->gwion, type_val, loc_val);
    struct Type_List_ tl_val = { .td=td_val };
    struct Type_List_ tl_key = { .td=td_key, .next=&tl_val };
    Type_Decl td = { .xid=insert_symbol("Map"), .types=&tl_key };
    const Type t = known_type(env, &td);
    free_type_decl(env->gwion->mp, td_key);
    free_type_decl(env->gwion->mp, td_val);
    ensure_traverse(env, t);
    prim_exp(data)->type = t;
    return t;
  }
  env->context->error = true;
  return NULL;
}
*/
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
  return !(is_func(gwion, type) && !is_fptr(gwion, type)) ?
    type : type_list_base_func(type);
}

ANN static Type_List mk_type_list(const Env env, const Arg_List arg,
                                  const Type type, const loc_t pos) {
  const Type base = type_list_base(env->gwion, type);
  const Type t    = !arg->td->array ?
          base : array_type(env, base, arg->td->array->depth);
  Type_Decl *td   = type2td(env->gwion, t, pos);
  return new_type_list(env->gwion->mp, td, NULL);
}

ANN static inline bool func_match_inner(const Env env, const Exp e,
                                        const Type t, const bool implicit,
                                        const bool specific) {
  if (specific ? e->type == t : isa(e->type, t) > 0) // match
    return true;
  return !implicit ? false : check_implicit(env, e, t) > 0;
}

ANN2(1, 2)
static Func find_func_match_actual(const Env env, Func func, const Exp args,
                                   const bool implicit, const bool specific) {
  do {
    Exp      e  = args;
    Arg_List e1 = func->def->base->args;
    while (e) {
      if (!e->type) // investigate
        return NULL;
//      if (!strncmp(e->type->name, "Ref:[", 5)) exp_setvar(e, true);
      if (!e1) {
        if (fbflag(func->def->base, fbflag_variadic)) return func;
        CHECK_OO(func->next);
        return find_func_match_actual(env, func->next, args, implicit,
                                      specific);
      }
      if (e1->type == env->gwion->type[et_auto] ||
          (func->def->base->tmpl &&
           is_fptr(env->gwion, func->value_ref->type) > 0)) {
        const Type owner = func->value_ref->from->owner_class;
        if (owner) CHECK_BO(template_push(env, owner));
        e1->type = known_type(env, e1->td);
        if (owner) nspc_pop_type(env->gwion->mp, env->curr);
        CHECK_OO(e1->type);
      }
      if (!func_match_inner(env, e, e1->type, implicit, specific)) break;
      e  = e->next;
      e1 = e1->next;
    }
    if (!e1) return func;
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
  if (v && is_func(env->gwion, v->type) && !v->from->owner_class)
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
  const m_uint   scope = env->scope->depth;
  env->scope->depth    = 0;
  vector_init(&v);
  while (vector_size((Vector)&env->curr->info->value->ptr) > 1)
    vector_add(&v, vector_pop((Vector)&env->curr->info->value->ptr));
  const m_bool ret = traverse_func_def(env, fdef);
  for (m_uint i = vector_size(&v) + 1; --i;)
    vector_add((Vector)&env->curr->info->value->ptr, vector_at(&v, i - 1));
  vector_release(&v);
  env->scope->depth = scope;
  return ret;
}

ANN static m_bool check_func_args(const Env env, Arg_List arg_list) {
  do {
    const Var_Decl decl = arg_list->var_decl;
    const Value    v    = decl->value;
    CHECK_BB(already_defined(env, decl->xid, decl->pos));
    set_vflag(v, vflag_valid);
    nspc_add_value(env->curr, decl->xid, v);
  } while ((arg_list = arg_list->next));
  return GW_OK;
}

#define next_arg(type)                                                         \
  ANN static inline type next_arg_##type(const type e) {                       \
    const type next = e->next;                                                 \
    if (next) gw_err(", ");                                                    \
    return next;                                                               \
  }
next_arg(Exp) next_arg(Arg_List);

ANN static void print_current_args(Exp e) {
  gw_err(_("and not\n  "));
  do gw_err(" {G}%s{0}", e->type ? e->type->name : "{-/}Unknown");
  while ((e = next_arg_Exp(e)));
  gw_err("\n");
}

ANN static void print_arg(Arg_List e) {
  do
    gw_err("{G}%s{0} {/}%s{0}", e->type ? e->type->name : NULL,
           e->var_decl->xid ? s_name(e->var_decl->xid) : "");
  while ((e = next_arg_Arg_List(e)));
}

ANN2(1)
static void function_alternative(const Env env, const Type f, const Exp args,
                                 const loc_t pos) {
  if (env->context && env->context->error) // needed for ufcs
    return;
  gwerr_basic("Argument type mismatch", "call site",
              "valid alternatives:", env->name, pos, 0);
  Func up = f->info->func;
  do {
    gw_err("  {-}(%s){0}  ", up->name);
    const Arg_List e = up->def->base->args;
    if (e)
      print_arg(e);
    else
      gw_err("{G}void{0}");
    gw_err("\n");
  } while ((up = up->next));
  if (args)
    print_current_args(args);
  else
    gw_err(_("and not:\n  {G}void{0}\n"));
}

ANN static m_uint get_type_number(Specialized_List list) {
  m_uint type_number = 0;
  do ++type_number;
  while ((list = list->next));
  return type_number;
}

ANN static Func get_template_func(const Env env, Exp_Call *const func,
                                  const Value v) {
  const Func f = find_template_match(env, v, func);
  if (f) {
    // copy that tmpl->call?
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
        _("function is template. automatic type guess not fully implemented "
          "yet.\n"
          "  please provide template types. eg: ':[type1, type2, ...]'"))
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
  m_uint       args_number = 0;
  const m_uint type_number = get_type_number(tm->list);
  Type_List    tl[type_number];
  tl[0]                 = NULL;
  Specialized_List list = tm->list;
  while (list) {
    Arg_List arg          = fdef->base->args;
    Exp      template_arg = exp->args;
    while (arg && template_arg) {
      if (list->xid == arg->td->xid) {
        if  (isa(template_arg->type, env->gwion->type[et_lambda]) > 0 &&
             !template_arg->type->info->func)
          break;
        tl[args_number] =
            mk_type_list(env, arg, template_arg->type, fdef->base->pos);
        if (args_number) tl[args_number - 1]->next = tl[args_number];
        ++args_number;
        break;
      }
      arg          = arg->next;
      template_arg = template_arg->next;
    }
    list = list->next;
  }
  if (args_number < type_number) // TODO: free type_list
    ERR_O(exp->func->pos, _("not able to guess types for template call."))
  return tl[0];
}

ANN static Type check_exp_call_template(const Env env, Exp_Call *exp) {
  const Type t = exp->func->type;
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

ANN static Type check_lambda_call(const Env env, const Exp_Call *exp) {
  if (exp->args) CHECK_OO(check_exp(env, exp->args));
  Exp_Lambda *l   = &exp->func->d.exp_lambda;
  Arg_List    arg = l->def->base->args;
  Exp         e   = exp->args;
  while (arg && e) {
    arg->type = e->type;
    if(is_class(env->gwion, arg->type))
      type_addref(arg->type);
    arg       = arg->next;
    e         = e->next;
  }
  if (arg || e)
    ERR_O(exp_self(exp)->pos, _("argument number does not match for lambda"))
  l->def->base->values = env->curr->info->value;
  const m_bool ret     = traverse_func_def(env, l->def);
  if (l->def->base->func) {
    free_scope(env->gwion->mp, env->curr->info->value);
    env->curr->info->value = l->def->base->values;
    if (env->class_def) set_vflag(l->def->base->func->value_ref, vflag_member);
    exp->func->type = l->def->base->func->value_ref->type;
    if (!l->def->base->ret_type)
      l->def->base->ret_type = env->gwion->type[et_void];
  }
  return ret > 0 ? l->def->base->ret_type : NULL;
}

ANN m_bool func_check(const Env env, Exp_Call *const exp) {
  if(exp->func->exp_type == ae_exp_dot)
    exp->func->d.exp_dot.is_call = true;
  CHECK_OB(check_exp(env, exp->func));
  if (exp->func->exp_type == ae_exp_decl)
    ERR_B(exp->func->pos, _("Can't call late function pointer at declaration "
                            "site. did you meant to use `@=>`?"))
  const Type t = actual_type(env->gwion, exp->func->type);
  if (is_func(env->gwion, t) && exp->func->exp_type == ae_exp_dot &&
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

ANN Type check_exp_call1(const Env env, Exp_Call *const exp) {
  DECL_BO(const m_bool, ret, = func_check(env, exp));
  if (!ret) return exp_self(exp)->type;
  const bool _class = is_class(env->gwion, exp->func->type);
  const Type t =
      _class ? actual_type(env->gwion, exp->func->type) : exp->func->type;
  if (!is_func(env->gwion, t)) { // use func flag?
    struct Op_Import opi = {.op   = insert_symbol("@ctor"),
                            .rhs  = actual_type(env->gwion, exp->func->type),
                            .data = (uintptr_t)exp,
                            .pos  = exp_self(exp)->pos};
    const Type       t   = op_check(env, &opi);
    return t;
  }
  if (_class) // need an instance
    ERR_O(exp->func->pos, "can't call a function pointer type");
  if (t == env->gwion->type[et_op]) return check_op_call(env, exp);
  if (t == env->gwion->type[et_lambda]) // TODO: effects?
    return check_lambda_call(env, exp);
  if (fflag(t->info->func, fflag_ftmpl)) {
    const Value value = t->info->func->value_ref;
    if (value->from->owner_class)
      CHECK_BO(ensure_traverse(env, value->from->owner_class));
  }
  if (exp->args) CHECK_OO(check_exp(env, exp->args));
  if (tflag(t, tflag_ftmpl))
    return check_exp_call_template(env, (Exp_Call *)exp); // TODO: effects?
  const Func func = find_func_match(env, t->info->func, exp);
  if (func) {
    if (!is_fptr(env->gwion, func->value_ref->type)) // skip function pointers
      if (func != env->func && func->def && !fflag(func, fflag_valid)) {
        struct EnvSet es = {.env   = env,
                            .data  = env,
                            .func  = (_exp_func)check_cdef,
                            .scope = env->scope->depth,
                            .flag  = tflag_check};
        CHECK_BO(envset_pushv(&es, func->value_ref));
        CHECK_BO(check_func_def(env, func->def));
        if (es.run) envset_pop(&es, func->value_ref->from->owner_class);
      }

    exp->func->type = func->value_ref->type;
    call_add_effect(env, func, exp->func->pos);
    if (func == env->func) set_fflag(env->func, fflag_recurs);
    return func->def->base->ret_type != env->gwion->type[et_auto] ?
      func->def->base->ret_type : exp->func->d.exp_dot.base->type;
  }
  const loc_t pos = exp->args ? exp->args->pos : exp->func->pos;
  function_alternative(env, exp->func->type, exp->args, pos);
  return NULL;
}

ANN static Type check_exp_binary(const Env env, const Exp_Binary *bin) {
  CHECK_OO(check_exp(env, bin->lhs));
  const m_bool is_auto = bin->rhs->exp_type == ae_exp_decl &&
                         bin->rhs->d.exp_decl.type == env->gwion->type[et_auto];
  if (is_auto) bin->rhs->d.exp_decl.type = bin->lhs->type;
  CHECK_OO(check_exp(env, bin->rhs));
  if (is_auto) bin->rhs->type = bin->lhs->type;
  struct Op_Import opi = {.op   = bin->op,
                          .lhs  = bin->lhs->type,
                          .rhs  = bin->rhs->type,
                          .data = (uintptr_t)bin,
                          .pos  = exp_self(bin)->pos};
  exp_setuse(bin->lhs, 1);
  exp_setuse(bin->rhs, 1);
  const Type ret = op_check(env, &opi);
  if (!ret && is_auto && exp_self(bin)->exp_type == ae_exp_binary)
    bin->rhs->d.exp_decl.list->self->value->type = env->gwion->type[et_auto];
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

ANN2(1) static inline bool curried(const Env env, Exp exp) {
  while (exp) {
    if (is_hole(env, exp)) return true;
    exp = exp->next;
  }
  return false;
}

ANN static Type check_exp_call(const Env env, Exp_Call *exp) {
  if (exp->allow_curry && curried(env, exp->args))
    return env->gwion->type[et_curry];
  if (exp->tmpl) {
    DECL_BO(const m_bool, ret, = func_check(env, exp));
    if (!ret) return exp_self(exp)->type;
    const Type t = actual_type(env->gwion, exp->func->type);
    if (!is_func(env->gwion, t)) return check_exp_call1(env, exp);
    if(isa(t, env->gwion->type[et_lambda]) > 0)
      if  (!t->info->func)
        ERR_O(exp->func->pos, _("invalid lambda use."))
    if (exp->args) CHECK_OO(check_exp(env, exp->args));
    if (!t->info->func->def->base->tmpl)
      ERR_O(exp_self(exp)->pos, _("template call of non-template function."))
    if (t->info->func->def->base->tmpl->call) {
      if (env->func == t->info->func) {
        exp->func->type = env->func->value_ref->type;
        return env->func->def->base->ret_type;
      } else
        CHECK_BO(predefined_call(env, t, exp_self(exp)->pos));
    }
    const Value v = type_value(env->gwion, t);
    DECL_OO(const Func, f, = find_template_match(env, v, exp));
    exp->func->type = f->value_ref->type;
    return f->def->base->ret_type;
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
  return ret;
}

ANN static Type check_exp_dot(const Env env, Exp_Dot *member) {
  CHECK_OO(check_exp(env, member->base));
  return check_dot(env, member);
}

/*
// enable static checking
ANN static OP_CHECK(opck_predicate) {
  const Exp_Call *call = (Exp_Call*)data;
  const Exp predicate = call->args;
const Func f = exp_self(call)->type->info->func;
//f->def->d.code->d.stmt_code.stmt_list->stmt->d.stmt_exp.val;
  if(predicate->exp_type != ae_exp_primary ||
     predicate->d.prim.prim_type != ae_prim_num ||
     !predicate->d.prim.d.num) exit(12);
}
*/

ANN m_bool check_type_def(const Env env, const Type_Def tdef) {
  if (tdef->when) {
    set_tflag(tdef->type, tflag_contract);
    const Var_Decl decl = new_var_decl(env->gwion->mp, insert_symbol("self"),
                                       NULL, tdef->when->pos);
    const Arg_List args = new_arg_list(
        env->gwion->mp, cpy_type_decl(env->gwion->mp, tdef->ext), decl, NULL);
    Func_Base *fb = new_func_base(
        env->gwion->mp, type2td(env->gwion, tdef->type, tdef->pos),
        insert_symbol("@implicit"), args, ae_flag_none, tdef->pos);
    set_fbflag(fb, fbflag_op);
    const Exp helper = new_prim_id(env->gwion->mp, insert_symbol("@predicate"),
                                   tdef->when->pos);
    const Exp when   = cpy_exp(env->gwion->mp, tdef->when);
    when->next       = helper;
    const Stmt stmt =
        new_stmt_exp(env->gwion->mp, ae_stmt_exp, when, when->pos);
    const Stmt_List body =
        new_stmt_list(env->gwion->mp, stmt, NULL); // ret_list);
    const Stmt     code = new_stmt_code(env->gwion->mp, body, when->pos);
    const Func_Def fdef = new_func_def(env->gwion->mp, fb, code);
    CHECK_BB(traverse_func_def(env, fdef));
    const Exp predicate = stmt->d.stmt_exp.val;
    if (isa(predicate->type, env->gwion->type[et_bool]) < 0) {
      char explain[strlen(predicate->type->name) + 20];
      sprintf(explain, "found `{/+}%s{0}`", predicate->type->name);
      gwerr_basic("Invalid `{/+}when{0}` predicate expression type", explain,
                  "use `{/+}bool{0}`", env->name, when->pos, 0);
      char from[strlen(tdef->type->name) + 39];
      sprintf(from, "in `{/+}%s{0}` definition", tdef->type->name);
      gwerr_secondary(from, env->name, tdef->pos);
      if (env->context) env->context->error = true;
      return GW_ERROR;
    }
    /*
        // enable static checking
        const Func f = fdef->base->func;
        const struct Op_Func opfunc = { .ck=opck_predicate };
        const struct Op_Import opi = { .rhs=f->value_ref->type,
           .func=&opfunc, .data=(uintptr_t)f, .pos=tdef->pos,
       .op=insert_symbol("@func_check") }; CHECK_BB(add_op(env->gwion, &opi));
    */
    // we handle the return after, so that we don't get *cant' use implicit
    // casting while defining it*
    const Exp ret_id =
        new_prim_id(env->gwion->mp, insert_symbol("self"), when->pos);
    ret_id->d.prim.value = new_value(env->gwion->mp, tdef->type, "self");
    const Stmt ret =
        new_stmt_exp(env->gwion->mp, ae_stmt_return, ret_id, when->pos);
    const Stmt_List ret_list = new_stmt_list(env->gwion->mp, ret, NULL);
    ret_id->type             = tdef->type;
    body->next               = ret_list;
    tdef->when_def           = fdef;
  }
  return (!is_fptr(env->gwion, tdef->type) && tdef->type->info->cdef)
             ? check_class_def(env, tdef->type->info->cdef)
             : GW_OK;
}
ANN static Type check_exp_lambda(const Env env, const Exp_If *exp_if NUSED) {
  return env->gwion->type[et_lambda];
}

ANN static Type check_exp_td(const Env env, Type_Decl **td) {
  DECL_OO(const Type, t, = known_type(env, *td));
  if (!is_func(env->gwion, t) || is_fptr(env->gwion, t))
    return type_class(env->gwion, t);
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
          isa(curr->type, env->gwion->type[et_lambda]) < 0 &&
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
  do decl_static(env, nspc_lookup_value0(env->curr, list->xid));
  while ((list = list->next));
  env_pop(env, scope);
  nspc_allocdata(env->gwion->mp, edef->t->nspc);
  return GW_OK;
}

ANN static m_bool check_stmt_code(const Env env, const Stmt_Code stmt) {
  if (stmt->stmt_list) { RET_NSPC(check_stmt_list(env, stmt->stmt_list)) }
  return GW_OK;
}

ANN static m_bool check_stmt_varloop(const Env env, const Stmt_VarLoop stmt) {
  CHECK_OB(check_exp(env, stmt->exp));
  if (isa(stmt->exp->type, env->gwion->type[et_vararg]) < 0)
    ERR_B(stmt->exp->pos, "varloop expression type must be '%s', not '%s'",
          env->gwion->type[et_vararg]->name, stmt->exp->type->name)
  return check_stmt(env, stmt->body);
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

ANN static inline Type foreach_type(const Env env, const Exp exp) {
  DECL_OO(Type, et, = check_exp(env, exp));
  if (isa(et, env->gwion->type[et_array]) < 0)
    ERR_O(exp->pos,
          _("type '%s' is not array.\n"
            " This is not allowed in foreach loop"),
          et->name)
  DECL_OO(Type, base, = typedef_base(et));
  DECL_OO(const Type, t, = array_base(base));
  const m_uint depth = base->array_depth - 1;
  return depth ? array_type(env, t, depth) : t;
}

ANN static void check_idx(const Env env, struct EachIdx_ *const idx) {
  idx->v =
      new_value(env->gwion->mp, env->gwion->type[et_int], s_name(idx->sym));
  valuefrom(env, idx->v->from, idx->pos);
  set_vflag(idx->v, vflag_valid);
  nspc_add_value(env->curr, idx->sym, idx->v);
}

ANN static m_bool do_stmt_each(const Env env, const Stmt_Each stmt) {
  DECL_OB(const Type, base, = foreach_type(env, stmt->exp));
  CHECK_BB(ensure_traverse(env, base));
  const m_str basename = type2str(env->gwion, base, stmt->exp->pos);
  char        c[15 + strlen(basename)];
  sprintf(c, "Ref:[%s]", basename);
  const Type ret = str2type(env->gwion, c, stmt->exp->pos);
  if (base->array_depth) set_tflag(ret, tflag_typedef);
  stmt->v = new_value(env->gwion->mp, ret, s_name(stmt->sym));
  set_vflag(stmt->v, vflag_valid);
  nspc_add_value(env->curr, stmt->sym, stmt->v);
  if (stmt->idx) check_idx(env, stmt->idx);
  return check_conts(env, stmt_self(stmt), stmt->body);
}

ANN static m_bool do_stmt_repeat(const Env env, const Stmt_Loop stmt) {
  if (stmt->idx) check_idx(env, stmt->idx);
  return check_conts(env, stmt_self(stmt), stmt->body);
}

ANN static inline m_bool cond_type(const Env env, const Exp e) {
  const Type t_int = env->gwion->type[et_int];
  if (check_implicit(env, e, t_int) < 0) {
    char explain[40 + strlen(e->type->name)];
    sprintf(explain, "expected `{/+}int{0}`, got `{/+}%s{0}`", e->type->name);
    gwerr_basic(_("invalid repeat condition type"), explain,
                _("use an integer or cast to int if possible"), env->name,
                e->pos, 0);
    env->context->error = true;
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
  cond_type(env, stmt->cond) < 0 ||
  do_stmt_repeat(env, stmt) < 0) ? 1 : -1)
stmt_func_xxx(each, Stmt_Each, env_inline_mult(env, 1.5), do_stmt_each(env, stmt))

ANN static m_bool check_stmt_return(const Env env, const Stmt_Exp stmt) {
  if (!env->func)
    ERR_B(stmt_self(stmt)->pos,
          _("'return' statement found outside function definition"))
  if (env->scope->depth == 1) // so ops no dot set scope->depth ?
    set_fflag(env->func, fflag_return);
  if (!strcmp(s_name(env->func->def->base->xid), "new")) {
    if(stmt->val)
      ERR_B(stmt_self(stmt)->pos,
            _("'return' statement inside constructor function should have no expression"))
    return GW_OK;
  }
  DECL_OB(const Type, ret_type,
          = stmt->val ? check_exp(env, stmt->val) : env->gwion->type[et_void]);
  if (!env->func->def->base->ret_type) {
    assert(isa(env->func->value_ref->type, env->gwion->type[et_lambda]) > 0);
    env->func->def->base->ret_type = ret_type;
    return GW_OK;
  }
  if (isa(ret_type, env->func->def->base->ret_type) > 0) return GW_OK;
  if (tflag(ret_type, tflag_noret))
    ERR_B(stmt->val->pos, _("Can't use type `{+G}%s{+G}` for return"),
          ret_type->name);
  if (stmt->val) {
    if (env->func->def->base->xid == insert_symbol("@implicit") &&
        ret_type == env->func->def->base->args->type)
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
  return stmt->val ? check_exp(env, stmt->val) ? 1 : -1 : 1;
}

ANN static Value match_value(const Env env, const Type base,
                             const Exp_Primary *prim) {
  const Symbol sym = prim->d.var;
  const Value  v   = new_value(env->gwion->mp, base, s_name(sym));
  set_vflag(v, vflag_valid);
  nspc_add_value(env->curr, sym, v);
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
  return insert_symbol("==");
}

ANN static m_bool match_case_exp(const Env env, Exp e) {
  Exp last = e;
  for (m_uint i = 0; i < vector_size(&env->scope->match->cond);
       e        = e->next, ++i) {
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
      Exp_Binary       bin  = {.lhs = base, .rhs = e, .op = op};
      struct Exp_      ebin = {.d = {.exp_binary = bin}};
      struct Op_Import opi  = {.op   = op,
                              .lhs  = base->type,
                              .rhs  = e->type,
                              .data = (uintptr_t)&ebin.d.exp_binary,
                              .pos  = e->pos};
      CHECK_OB(op_check(env, &opi));
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
  Stmt_List list = stmt->list;
  do CHECK_BB(check_stmt_case(env, &list->stmt->d.stmt_match));
  while ((list = list->next));
  return GW_OK;
}

ANN static inline m_bool check_handler_list(const restrict Env env,
                                            const Handler_List handler) {
  if (handler->next) CHECK_BB(check_handler_list(env, handler->next));
  RET_NSPC(check_stmt(env, handler->stmt))
}

ANN static inline m_bool check_stmt_try_start(const restrict Env env,
                                              const Stmt_Try     stmt) {
    RET_NSPC(check_stmt(env, stmt->stmt))} ANN static inline m_bool
    _check_stmt_try(const restrict Env env, const Stmt_Try stmt) {
  CHECK_BB(check_handler_list(env, stmt->handler));
  vector_add(&env->scope->effects, 0);
  const m_bool ret = check_stmt_try_start(env, stmt);
  const m_uint _v  = vector_pop(&env->scope->effects);
  if (_v) {
    const M_Vector v = (M_Vector)&_v;
    for (m_uint i = 0; i < m_vector_size(v); i++) {
      struct ScopeEffect eff;
      m_vector_get(v, i, &eff);
      Handler_List handler = stmt->handler;
      bool         found   = false;
      do { // check there is no duplicate handler
        if (eff.sym == handler->xid) {
          found = true;
          break;
        }
      } while ((handler = handler->next));
      if (!found) env_add_effect(env, eff.sym, eff.pos);
    }
    m_vector_release(v);
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
  if (env->func && stmt->pp_type == ae_pp_pragma &&
      !strncmp(stmt->data, "memoize", strlen("memoize")))
    env->func->memoize = strtol(stmt->data + 7, NULL, 10);
  return GW_OK;
}

ANN static m_bool check_stmt_defer(const Env env, const Stmt_Defer stmt) {
  return check_stmt(env, stmt->stmt);
}

#define check_stmt_retry dummy_func
DECL_STMT_FUNC(check, m_bool, Env)

ANN m_bool check_stmt(const Env env, const Stmt stmt) {
  return check_stmt_func[stmt->stmt_type](env, &stmt->d);
}

ANN static m_bool check_stmt_list(const Env env, Stmt_List l) {
  do CHECK_BB(check_stmt(env, l->stmt));
  while ((l = l->next));
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
  return !fdef->base->tmpl
             ? isa(fdef->base->ret_type, parent->def->base->ret_type)
             : GW_OK;
}

ANN static m_bool parent_match_actual(const Env               env,
                                      const restrict Func_Def fdef,
                                      const restrict Func     func) {
  Func parent_func = func;
  do {
    if (parent_func->def->base && compat_func(fdef, parent_func->def) > 0) {
      CHECK_BB(check_signature_match(env, fdef, parent_func));
      if (!fdef->base->tmpl) {
        fdef->base->func->vt_index = parent_func->vt_index;
        vector_set(&env->curr->vtable, fdef->base->func->vt_index,
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
  func->vt_index = vector_size(&env->curr->vtable);
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

ANN bool check_effect_overload(const Vector base, const Func override) {
  if (!base->ptr) return true;
  if (!override->def->base->effects.ptr) return false; // TODO: error message
  const Vector v = &override->def->base->effects;
  for (m_uint i = 0; i < vector_size(v); i++) {
    if (vector_find((Vector)base, vector_at(v, i)) == -1) return false;
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
      (!fdef->base->tmpl || !fdef->base->tmpl->base))
    CHECK_BB(check_func_overload(env, fdef));
  return GW_OK;
}

ANN m_bool check_fdef(const Env env, const Func_Def fdef) {
  if (fdef->base->args) CHECK_BB(check_func_args(env, fdef->base->args));
  if (fdef->d.code) {
    env->scope->depth--;
    CHECK_BB(check_stmt_code(env, &fdef->d.code->d.stmt_code));
    env->scope->depth++;
  }
  // check fdef->base->td for `new`
  if (fdef->base->td && fdef->base->ret_type &&
      fdef->base->ret_type != env->gwion->type[et_void] && fdef->d.code &&
      !fflag(fdef->base->func, fflag_return))
    ERR_B(fdef->base->td->pos,
          _("missing return statement in a non void function"));
  return GW_OK;
}

ANN m_bool _check_func_def(const Env env, const Func_Def f) {
  if (tmpl_base(f->base->tmpl) && fbflag(f->base, fbflag_op)) return GW_OK;
  const Func     func = f->base->func;
  const Func_Def fdef = func->def;
  if(fflag(func, fflag_valid))return GW_OK;
  set_fflag(fdef->base->func, fflag_valid);
  assert(func == fdef->base->func);
  if (env->class_def) // tmpl ?
    CHECK_BB(check_parent_match(env, fdef));
  if (tmpl_base(fdef->base->tmpl)) return GW_OK;
  Value override = NULL;
  CHECK_BB(check_func_def_override(env, fdef, &override));
  DECL_BB(const m_int, scope, = GET_FLAG(fdef->base, global)
                                    ? env_push_global(env)
                                    : env->scope->depth);
  const Func former = env->func;
  env->func         = func;
  ++env->scope->depth;
  nspc_push_value(env->gwion->mp, env->curr);
  struct Op_Import opi = {};
  if (fbflag(fdef->base, fbflag_op)) {
    func_operator(f, &opi);
    operator_suspend(env->curr, &opi);
  }
  vector_add(&env->scope->effects, 0);
  const m_bool ret = scanx_fdef(env, env, fdef, (_exp_func)check_fdef);
  const m_uint _v  = vector_back(&env->scope->effects);
  if (_v) {
    if (fdef->base->xid == insert_symbol("@dtor"))
      ERR_B(fdef->base->pos, _("can't use effects in destructors"));
    const Vector v    = (Vector)&_v;
    const Vector base = &fdef->base->effects;
    if (!base->ptr) vector_init(base);
    for (m_uint i = 0; i < vector_size(v); i += 2) {
      const Symbol effect = (Symbol)vector_at(v, i);
      if (vector_find(base, (m_uint)effect) == -1)
        vector_add(base, (m_uint)effect);
    }
    vector_release(v);
  }
  vector_pop(&env->scope->effects);
  if (fbflag(fdef->base, fbflag_op)) operator_resume(&opi);
  nspc_pop_value(env->gwion->mp, env->curr);
  --env->scope->depth;
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

ANN static m_bool check_extend_def(const Env env, const Extend_Def xdef) {
  CHECK_BB(ensure_check(env, xdef->t));
  CHECK_BB(extend_push(env, xdef->t));
  const m_bool ret = check_ast(env, xdef->body);
  extend_pop(env, xdef->t);
  return ret;
}

ANN static m_bool _check_trait_def(const Env env, const Trait_Def pdef) {
  const Trait trait = nspc_lookup_trait1(env->curr, pdef->xid);
  Ast         ast   = pdef->body;
  while (ast) {
    Section *section = ast->section;
    if (section->section_type == ae_section_stmt) {
      Stmt_List list = section->d.stmt_list;
      while (list) {
        const Stmt stmt = list->stmt;
        if (stmt->stmt_type == ae_stmt_exp) {
          CHECK_BB(traverse_exp(env, stmt->d.stmt_exp.val));
          Var_Decl_List list = stmt->d.stmt_exp.val->d.exp_decl.list;
          while (list) {
            const Value value = list->self->value;
            valuefrom(env, value->from,
                      list->self->pos); // we do not need owner
            if (!trait->requested_values.ptr)
              vector_init(&trait->requested_values);
            vector_add(&trait->requested_values, (m_uint)value);
            list = list->next;
          }
        }
        list = list->next;
      }
    }
    ast = ast->next;
  }
  return GW_OK;
}

ANN static m_bool check_trait_def(const Env env, const Trait_Def pdef) {
  RET_NSPC(_check_trait_def(env, pdef));
}

#define check_fptr_def dummy_func
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
    Func f = (Func)vector_at(&cdef->base.type->nspc->vtable, i);
    if (f && GET_FLAG(f->def->base, abstract)) {
      if (!err) {
        err = true;
        gwerr_basic(_("missing function definition"),
                    _("must be declared 'abstract'"),
                    _("provide an implementation for the following:"),
                    env->name, cdef->pos, 0);
      }
      struct ValueFrom_ *from = f->value_ref->from;
      gwerr_secondary("implementation missing", from->filename, from->loc);
      env->context->error = true;
    }
  }
  return !err ? GW_OK : GW_ERROR;
}

ANN static inline void ctor_effects(const Env env) {
  const Vector v  = &env->scope->effects;
  const m_uint _w = vector_back(v);
  if (!_w) return;
  vector_init(&env->class_def->effects);
  const M_Vector w = (M_Vector)&_w;
  for (m_uint j = 0; j < m_vector_size(w); j++) {
    struct ScopeEffect eff;
    m_vector_get(w, j, &eff);
    vector_add(&env->class_def->effects, (m_uint)eff.sym);
  }
  m_vector_release(w);
  vector_pop(v);
}

ANN static m_bool check_body(const Env env, Section *const section) {
  const m_bool ret = check_section(env, section);
  ctor_effects(env);
  return ret;
}

ANN static bool class_def_has_body(const Env env, Ast ast) {
  do {
    const Section *section = ast->section;
    if (section->section_type == ae_section_stmt) {
      Stmt_List list = section->d.stmt_list;
      do {
        const Stmt stmt = list->stmt;
        if (stmt->stmt_type == ae_stmt_pp) continue;
        if (stmt->stmt_type == ae_stmt_exp) {
          const Exp exp = stmt->d.stmt_exp.val;
          if (!exp) continue;
          if (exp->exp_type != ae_exp_decl) return true;
          if (GET_FLAG(exp->d.exp_decl.td, late)) continue;
          Var_Decl_List dlist = exp->d.exp_decl.list;
          do {
            if (GET_FLAG(dlist->self->value, late)) continue;
            if (isa(dlist->self->value->type, env->gwion->type[et_compound]) >
                0)
              //            if(tflag(dlist->self->value->type, tflag_ctor) ||
              //                     dlist->self->value->type->array_depth)
              return true;
          } while ((dlist = dlist->next));
        } else
          return true;
      } while ((list = list->next));
    }
  } while ((ast = ast->next));
  return false;
}

ANN bool check_trait_requests(const Env env, const Type t, const ID_List list);
ANN static m_bool _check_class_def(const Env env, const Class_Def cdef) {
  const Type t = cdef->base.type;
  if (cdef->base.ext) CHECK_BB(cdef_parent(env, cdef));
  if (!tflag(t, tflag_struct)) inherit(t);
  if (cdef->body) {
    CHECK_BB(env_body(env, cdef, check_body));
    if (cflag(cdef, cflag_struct) || class_def_has_body(env, cdef->body))
      set_tflag(t, tflag_ctor);
  }
  if (!GET_FLAG(cdef, abstract)) CHECK_BB(check_abstract(env, cdef));
  if (cdef->traits) {
    ID_List list        = cdef->traits;
    bool    value_error = false;
    do
      if (!check_trait_requests(env, t, list)) value_error = true;
    while ((list = list->next));

    if (value_error) {
      env->class_def = t;
      env_error_footer(env);
      env->context->error = true;
      return GW_ERROR;
    }
  }

  Value             value;
  struct scope_iter iter = {t->nspc->info->value, 0, 0};
  while (scope_iter(&iter, &value) > 0) {
    if (isa(value->type, env->gwion->type[et_compound]) < 0) continue;
    if (value->type->nspc && !GET_FLAG(value, late)) {
      Value             v;
      struct scope_iter inner = {value->type->nspc->info->value, 0, 0};
      while (scope_iter(&inner, &v) > 0) {
        //      if(isa(v->type, t) > 0 || isa(t, v->type) > 0) {
        if (v->type == t) {
          env_err(env, v->from->loc, _("recursive type"));
          env->context->error = false;
          env_err(env, value->from->loc, _("recursive type"));
          env->context->error = true;
          type_remref(t, env->gwion);
          return GW_ERROR;
        }
      }
    }
  }
  nspc_allocdata(env->gwion->mp, t->nspc);
  return GW_OK;
}

ANN m_bool check_class_def(const Env env, const Class_Def cdef) {
  if (tmpl_base(cdef->base.tmpl)) return GW_OK;
  const Type       t   = cdef->base.type;
  const Class_Def  c   = t->info->cdef;
  struct Op_Import opi = {.op   = insert_symbol("@class_check"),
                          .lhs  = t,
                          .data = (uintptr_t)c,
                          .pos  = c->pos};
  CHECK_OB(op_check(env, &opi));
  if (t->info->value->from->owner_class)
    CHECK_BB(ensure_check(env, t->info->value->from->owner_class));
  if (tflag(t, tflag_check)) return GW_OK;
  set_tflag(t, tflag_check);
  return _check_class_def(env, c);
}

ANN static inline void check_unhandled(const Env env) {
  const Vector v  = &env->scope->effects;
  const m_uint _w = vector_back(v);
  if (!_w) return;
  const M_Vector w = (M_Vector)&_w;
  for (m_uint j = 0; j < m_vector_size(w); j++) {
    struct ScopeEffect eff;
    m_vector_get(w, j, &eff);
    if(s_name(eff.sym)[0] == '!')
      continue;
    gwerr_secondary("Unhandled effect", env->name, eff.pos);
    env->context->error = false;
  }
  m_vector_release(w);
  vector_pop(v);
}

ANN m_bool check_ast(const Env env, Ast ast) {
  do CHECK_BB(check_section(env, ast->section));
  while ((ast = ast->next));
  check_unhandled(env);
  return GW_OK;
}
