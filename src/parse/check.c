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
ANN m_bool check_class_def(const Env env, const Class_Def class_def);

ANN static Type check_internal(const Env env, const Symbol sym,
      const Exp e, const Type t) {
  struct Implicit imp = { .e=e, .t=t, .pos=e->pos };
  struct Op_Import opi = { .op=sym, .lhs=e->type,
        .rhs=t, .data=(uintptr_t)&imp, .pos=e->pos };
  return op_check(env, &opi);
}

ANN m_bool check_implicit(const Env env, const Exp e, const Type t) {
  if(e->type == t)
    return GW_OK;
  const Symbol sym = insert_symbol("@implicit");
  return (e->cast_to = check_internal(env, sym, e, t)) ? GW_OK : GW_ERROR;
}

ANN m_bool check_subscripts(Env env, const Array_Sub array, const m_bool is_decl) {
  CHECK_OB(check_exp(env, array->exp))
  m_uint depth = 0;
  Exp e = array->exp;
  do if(is_decl)
    CHECK_BB(check_implicit(env, e, env->gwion->type[et_int]))
  while(++depth && (e = e->next));
  if(depth != array->depth)
    ERR_B(array->exp->pos, _("invalid array acces expression."))
  return GW_OK;
}

ANN static inline m_bool check_exp_decl_parent(const Env env, const Var_Decl var) {
  const Value value = find_value(env->class_def->info->parent, var->xid);
  if(value)
    ERR_B(var->pos,
          _("in class '%s': '%s' has already been defined in parent class '%s' ..."),
          env->class_def->name, s_name(var->xid),
          value->from->owner_class ? value->from->owner_class->name : "?")
  return GW_OK;
}

#define describe_check_decl(a, b, flag)                           \
ANN static inline void decl_##a(const Env env, const Value v) {   \
  const Nspc nspc = env->curr;                                    \
  flag;                                                           \
  v->from->offset = nspc->info->b;                                \
  nspc->info->b += v->type->size;                                 \
}
describe_check_decl(member, offset, v->vflag |= vflag_member)
describe_check_decl(static, class_data_size, SET_FLAG(v, static))

ANN static m_bool check_fptr_decl(const Env env, const Var_Decl var) {
  const Value v    = var->value;
  Type t = v->type;
  while(tflag(t, tflag_typedef))
    t = t->info->parent;
  if(!t->info->func)
    return GW_ERROR;
  if(!env->class_def)
    return GW_OK;
  const Func  func = t->info->func;
  const Type type = func->value_ref->from->owner_class;
  if(type && isa(type, env->class_def) < 0 && !GET_FLAG(func, global))
    ERR_B(var->pos, _("can't use non global fptr of other class."))
  if(vflag(func->value_ref, vflag_member) && GET_FLAG(v, static))
      ERR_B(var->pos, _("can't use static variables for member function."))
  return GW_OK;
}

ANN static m_bool check_var(const Env env, const Var_Decl var) {
  if(env->class_def && !env->scope->depth && env->class_def->info->parent)
    CHECK_BB(check_exp_decl_parent(env, var))
  if(var->array && var->array->exp)
    return check_subscripts(env, var->array, 1);
  return GW_OK;
}

ANN static m_bool check_var_td(const Env env, const Var_Decl var, Type_Decl *const td) {
  const Value v = var->value;
  if(env->class_def)  {
    if(vflag(v, vflag_member)) {
      decl_member(env, v);
      if(env->class_def->info->tuple)
        tuple_info(env, v);
    } else if(GET_FLAG(td, static))
      decl_static(env, v);
  }
  return GW_OK;
}

ANN static m_bool check_decl(const Env env, const Exp_Decl *decl) {
  Var_Decl_List list = decl->list;
  do {
    const Var_Decl var = list->self;
    CHECK_BB(check_var(env, var))
    CHECK_BB(check_var_td(env, var, decl->td))
    if(is_fptr(env->gwion, decl->type))
      CHECK_BB(check_fptr_decl(env, var))
    set_vflag(var->value, vflag_valid);
    //set_vflag(var->value, vflag_used));
    nspc_add_value(env->curr, var->xid, var->value);
  } while((list = list->next));
  return GW_OK;
}

ANN static inline m_bool ensure_check(const Env env, const Type t) {
  if(tflag(t, tflag_check) || !(tflag(t, tflag_cdef) || tflag(t, tflag_udef)))
    return GW_OK;
  struct EnvSet es = { .env=env, .data=env, .func=(_exp_func)check_cdef,
    .scope=env->scope->depth, .flag=tflag_check };
  return envset_run(&es, t);
}

ANN m_bool ensure_traverse(const Env env, const Type t) {
  if(tflag(t, tflag_check) || !(tflag(t, tflag_cdef) || tflag(t, tflag_udef)))
    return GW_OK;
  struct EnvSet es = { .env=env, .data=env, .func=(_exp_func)traverse_cdef,
    .scope=env->scope->depth, .flag=tflag_check };
  return envset_run(&es, t);
}

ANN static inline m_bool inferable(const Env env, const Type t, const loc_t pos) {
  if(!tflag(t, tflag_infer))
    return GW_OK;
  ERR_B(pos, _("can't infer type."))
}

ANN Type check_exp_decl(const Env env, const Exp_Decl* decl) {
  if(decl->td->array && decl->td->array->exp)
    CHECK_OO(check_exp(env, decl->td->array->exp))
  if(decl->td->xid == insert_symbol("auto")) { // should be better
    CHECK_BO(scan1_exp(env, exp_self(decl)))
    CHECK_BO(scan2_exp(env, exp_self(decl)))
  }
  if(!decl->type)
    ERR_O(decl->td->pos, _("can't find type"));
  {
    CHECK_BO(inferable(env, decl->type, decl->td->pos))
    CHECK_BO(ensure_check(env, decl->type))
  }
  const m_bool global = GET_FLAG(decl->td, global);
  const m_uint scope = !global ? env->scope->depth : env_push_global(env);
  const m_bool ret = check_decl(env, decl);
  if(global)
    env_pop(env, scope);
  return ret > 0 ? decl->list->self->value->type : NULL;
}

ANN static m_bool prim_array_inner(const Env env, Type type, const Exp e) {
  const Type common = find_common_anc(e->type, type);
  if(common)
    return GW_OK;
  if(check_implicit(env, e, type) < 0)
    ERR_B(e->pos, _("array init [...] contains incompatible types ..."))
  return GW_OK;
}

ANN static inline Type prim_array_match(const Env env, Exp e) {
  const Type type = e->type;
  do CHECK_BO(prim_array_inner(env, type, e))
  while((e = e->next));
  return array_type(env, array_base(type), type->array_depth + 1);
}

ANN static Type check_prim_array(const Env env, const Array_Sub *data) {
  const Array_Sub array = *data;
  const Exp e = array->exp;
  if(!e)
    ERR_O(prim_pos(data), _("must provide values/expressions for array [...]"))
  CHECK_OO(check_exp(env, e))
  return (array->type = prim_array_match(env, e));
}

ANN static m_bool check_range(const Env env, Range *range) {
  if(range->start)
    CHECK_OB(check_exp(env, range->start))
  if(range->end)
    CHECK_OB(check_exp(env, range->end))
  if(range->start && range->end) {
    if(isa(range->end->type, range->start->type) < 0)
      ERR_B(range->start->pos, _("range types do not match"))
  }
  return GW_OK;
}

ANN static Type check_prim_range(const Env env, Range **data) {
  Range *range = *data;
  CHECK_BO(check_range(env, range))
  const Exp e = range->start ?: range->end;
  const Symbol sym = insert_symbol("@range");
  struct Op_Import opi = { .op=sym, .rhs=e->type, .pos=e->pos, .data=(uintptr_t)prim_exp(data) };
  return op_check(env, &opi);
}

ANN m_bool not_from_owner_class(const Env env, const Type t,
      const Value v, const loc_t pos) {
  if(!v->from->owner_class || isa(t, v->from->owner_class) < 0) {
    ERR_B(pos,
        _("'%s' from owner namespace '%s' used in '%s'."),
            v->name, v->from->owner ? v->from->owner->name : "?", t->name)
  }
  return GW_OK;
}

ANN static inline Value get_value(const Env env, const Symbol sym) {
  const Value value = nspc_lookup_value1(env->curr, sym);
  if(value)
    return value;
  if(env->func && env->func->def->base->values)
    return  (Value)scope_lookup1(env->func->def->base->values, (vtype)sym);
  return NULL;
}

ANN static Value check_non_res_value(const Env env, const Symbol *data) {
  const Symbol var = *data;
  const Value value = get_value(env, var);
  if(env->class_def) {
    if(value && value->from->owner_class)
      CHECK_BO(not_from_owner_class(env, env->class_def, value, prim_pos(data)))
    const Value v = value ?: find_value(env->class_def, var);
    if(v) {
      if(env->func && GET_FLAG(env->func->def->base, static) && vflag(v, vflag_member))
        ERR_O(prim_pos(data),
              _("non-static member '%s' used from static function."), s_name(var))
    }
    return v;
  } else if(SAFE_FLAG(env->class_def, global) || (env->func && GET_FLAG(env->func->def->base, global))) {
    if(!value || !GET_FLAG(value, global))
      ERR_O(prim_pos(data),
            _("non-global variable '%s' used from global function/class."), s_name(var))
  }
  return value;
}

ANN static Type check_dot(const Env env, const Exp_Dot *member) {
  struct Op_Import opi = { .op=insert_symbol("@dot"), .lhs=member->base->type, .data=(uintptr_t)member,
    .pos=exp_self(member)->pos };
  return op_check(env, &opi);
}

static inline Nspc value_owner(const Value v) {
  return v ? v->from->owner : NULL;
}

ANN static void check_upvalue(const Env env, const Exp_Primary *prim) {
  const Value v = prim->value;
  if(GET_FLAG(v, global) || (v->from->owner_class && isa(v->from->owner_class, env->class_def) > 0) ||
      nspc_lookup_value1(env->curr, insert_symbol(v->name)))
    return;
  const Map map = &env->func->upvalues;
  if(!map->ptr) {
    map_init(map);
    map_set(&env->func->upvalues, (vtype)prim, 0);
  } else {
    if(map_get(map, (vtype)v))return;
    const m_uint offset = VVAL(map, VLEN(map) -1) + ((Exp_Primary*)VKEY(map, VLEN(map) - 1))->value->type->size;
    map_set(&env->func->upvalues, (vtype)prim, offset);
  }
}

ANN static Type prim_owned(const Env env, const Symbol *data) {
  const Exp exp = exp_self(prim_exp(data));
  const Value v = exp->d.prim.value;
  const m_str name = !GET_FLAG(v, static) ? "this" : v->from->owner_class->name;
  const Exp base = new_prim_id(env->gwion->mp, insert_symbol(name), prim_pos(data));
  exp_setuse(base, 1);
  exp->exp_type = ae_exp_dot;
  exp->d.exp_dot.base = base;
  exp->d.exp_dot.xid = *data;
  return check_exp(env, exp);
}

ANN static Type prim_id_non_res(const Env env, const Symbol *data) {
  const Symbol sym = *data;
  const Value v = check_non_res_value(env, data);
  if(!v || !vflag(v, vflag_valid) || (v->from->ctx && v->from->ctx->error)) {
    env_err(env, prim_pos(data),
          _("variable %s not legit at this point."), s_name(sym));
    if(v)
      did_you_mean_nspc(value_owner(v) ?: env->curr, s_name(sym));
    return NULL;
  }
  prim_self(data)->value = v;
  if(v->from->owner_class)
    return prim_owned(env, data);
  if(GET_FLAG(v, const))
    exp_setmeta(prim_exp(data), 1);
  if(env->func) {
    if(!GET_FLAG(v, const) && v->from->owner)
      unset_fflag(env->func, fflag_pure);
    if(fbflag(env->func->def->base, fbflag_lambda))
      check_upvalue(env, prim_self(data));
  }
  //set_vflag(v->vflag, vflag_used);
  return v->type;
}

ANN Type check_prim_str(const Env env, const m_str *data) {
  if(!prim_self(data)->value)
    prim_self(data)->value = global_string(env, *data);
  return env->gwion->type[et_string];// prim->value
}

ANN static Type check_prim_id(const Env env, const Symbol *data) {
  struct SpecialId_ * spid = specialid_get(env->gwion, *data);
  if(spid)
    return specialid_type(env, spid, prim_self(data));
  return prim_id_non_res(env, data);
}

ANN static Type check_prim_interp(const Env env, const Exp* exp) {
  CHECK_OO(check_exp(env, *exp))
  return env->gwion->type[et_string];
}

ANN static Type check_prim_hack(const Env env, const Exp *data) {
  if(env->func)
      unset_fflag(env->func, fflag_pure);
  CHECK_OO(check_prim_interp(env, data))
  return env->gwion->type[et_gack];
}

#define describe_prim_xxx(name, type) \
ANN static Type check##_prim_##name(const Env env NUSED, const union prim_data* data NUSED) {\
  return type; \
}
describe_prim_xxx(num, env->gwion->type[et_int])
describe_prim_xxx(char, env->gwion->type[et_char])
describe_prim_xxx(float, env->gwion->type[et_float])
describe_prim_xxx(nil, env->gwion->type[et_void])

#define check_prim_complex check_prim_vec
#define check_prim_polar check_prim_vec
#define check_prim_char check_prim_char
DECL_PRIM_FUNC(check, Type, Env);

ANN static Type check_prim(const Env env, Exp_Primary *prim) {
  return exp_self(prim)->type = check_prim_func[prim->prim_type](env, &prim->d);
}

ANN Type check_array_access(const Env env, const Array_Sub array) {
  const Symbol sym = insert_symbol("@array");
  struct Op_Import opi = { .op=sym, .lhs=array->exp->type, .rhs=array->type,
    .pos=array->exp->pos, .data=(uintptr_t)array };
  return op_check(env, &opi);
}

static ANN Type check_exp_array(const Env env, const Exp_Array* array) {
  CHECK_OO((array->array->type = check_exp(env, array->base)))
  CHECK_BO(check_subscripts(env, array->array, 0))
  return check_array_access(env, array->array);
}

static ANN Type check_exp_slice(const Env env, const Exp_Slice* range) {
  CHECK_OO(check_exp(env, range->base))
  CHECK_BO(check_range(env, range->range))
  const Symbol sym = insert_symbol("@slice");
  const Exp e = range->range->start ?: range->range->end;
  struct Op_Import opi = { .op=sym, .lhs=e->type, .rhs=range->base->type,
    .pos=e->pos, .data=(uintptr_t)exp_self(range) };
  return op_check(env, &opi);
}

ANN2(1,2) static Type_Decl* prepend_type_decl(MemPool mp, const Symbol xid, Type_Decl* td, const struct loc_t_ pos) {
  Type_Decl *a = new_type_decl(mp, xid, pos);
  a->next = td;
  return a;
}

ANN static Type_List mk_type_list(const Env env, const Type type, const loc_t pos) {
  struct Vector_ v;
  vector_init(&v);
  vector_add(&v, (vtype)insert_symbol(type->name));
  Type owner = type->info->owner_class;
  while(owner) {
    vector_add(&v, (vtype)insert_symbol(owner->name));
    owner = owner->info->owner_class;
  }
  Type_Decl *td = NULL;
  for(m_uint i = 0 ; i < vector_size(&v); ++i)
    td = prepend_type_decl(env->gwion->mp, (Symbol)vector_at(&v, i), td, pos);
  vector_release(&v);
  return new_type_list(env->gwion->mp, td, NULL);
}

ANN static m_bool func_match_inner(const Env env, const Exp e, const Type t,
  const m_bool implicit, const m_bool specific) {
  const m_bool match = (specific ? e->type == t : isa(e->type, t) > 0);
  if(!match) {
    if(e->type == env->gwion->type[et_lambda] && is_fptr(env->gwion, t)) {
      exp_setvar(e, 1);
      return check_lambda(env, t, &e->d.exp_lambda);
    }
    if(implicit) {
      const m_bool ret = check_implicit(env, e, t);
      if(ret == GW_OK)
        return ret;
    }
  }
  return match ? 1 : -1;
}

ANN2(1,2) static Func find_func_match_actual(const Env env, Func func, const Exp args,
  const m_bool implicit, const m_bool specific) {
  do {
    Exp e = args;
    Arg_List e1 = func->def->base->args;
    while(e) {
      if(!e1) {
        if(fbflag(func->def->base, fbflag_variadic))
          return func;
        CHECK_OO(func->next);
        return find_func_match_actual(env, func->next, args, implicit, specific);
      } else if(!e->type) //fix bug found with Cytosol
          return NULL;
      if(e1->type == env->gwion->type[et_auto] ||
            (func->def->base->tmpl && is_fptr(env->gwion, func->value_ref->type) > 0)) {
        const Type owner = func->value_ref->from->owner_class;
        if(owner)
          CHECK_BO(template_push(env, owner))
        e1->type = known_type(env, e1->td);
        if(owner)
          nspc_pop_type(env->gwion->mp, env->curr);
        CHECK_OO(e1->type)
      }
//      if(!func->def->base->tmpl && func->next)
//        env->context->error = 1;
      const m_bool ret = func_match_inner(env, e, e1->type, implicit, specific);
//      if(func->next)
//        env->context->error = ret < 0;
      if(ret < 0)
        break;
      e = e->next;
      e1 = e1->next;
    }
    if(!e1)
      return func;
  } while((func = func->next));
  return NULL;
}

ANN static Type check_exp_call(const Env env, Exp_Call* exp);

ANN static Func call2ufcs(const Env env, Exp_Call* call, const Value v) {
  const Exp this = call->func->d.exp_dot.base;
  this->next = call->args;
  call->args = this;
  call->func->type = v->type;
  call->func->d.prim.value = v;
  call->func->d.prim.d.var = call->func->d.exp_dot.xid;
  call->func->exp_type = ae_exp_primary;
  call->func->d.prim.prim_type = ae_prim_id;
  call->args = this;
  CHECK_OO(check_exp_call(env, call))
  return call->func->type->info->func;
}

ANN Func ufcs(const Env env, const Func up, Exp_Call *const call) {
  const Value v = nspc_lookup_value1(env->curr, up->def->base->xid);
  if(v && isa(v->type, env->gwion->type[et_function]) > 0 && !vflag(v, vflag_member))
    return call2ufcs(env, call, v);
  return NULL;
}

ANN Func find_func_match(const Env env, const Func up, Exp_Call *const call) {
  Func func;
  const Exp exp = call->args;
  const Exp args = (exp && isa(exp->type, env->gwion->type[et_void]) < 0) ? exp : NULL;
  if((func = find_func_match_actual(env, up, args, 0, 1)) ||
     (func = find_func_match_actual(env, up, args, 1, 1)) ||
     (func = find_func_match_actual(env, up, args, 0, 0)) ||
     (func = find_func_match_actual(env, up, args, 1, 0)))
    return func;
  return call->func->exp_type == ae_exp_dot && up->value_ref->from->owner_class ?
    ufcs(env, up, call) : NULL;
}

ANN m_bool check_traverse_fdef(const Env env, const Func_Def fdef) {
  struct Vector_ v = {};
  const m_uint scope = env->scope->depth;
  env->scope->depth = 0;
  vector_init(&v);
  while(vector_size((Vector)&env->curr->info->value->ptr) > 1)
    vector_add(&v, vector_pop((Vector)&env->curr->info->value->ptr));
  const m_bool ret = traverse_func_def(env, fdef);
  for(m_uint i = vector_size(&v) + 1; --i;)
    vector_add((Vector)&env->curr->info->value->ptr, vector_at(&v, i-1));
  vector_release(&v);
  env->scope->depth = scope;
  return ret;
}

ANN static m_bool check_func_args(const Env env, Arg_List arg_list) {
  do {
    const Var_Decl decl = arg_list->var_decl;
    const Value v = decl->value;
// TODO: use coumpound instead of object?
    if(isa(v->type, env->gwion->type[et_object]) > 0 || isa(v->type, env->gwion->type[et_function]) > 0)
      unset_fflag(env->func, fflag_pure);
    CHECK_BB(already_defined(env, decl->xid, decl->pos))
    set_vflag(v, vflag_valid);
    nspc_add_value(env->curr, decl->xid, v);
  } while((arg_list = arg_list->next));
  return GW_OK;
}

#define next_arg(type) \
ANN static inline type next_arg_##type(const type e) { \
  const type next = e->next;                           \
  if(next)                                             \
    gw_err(",");                                       \
  return next;                                         \
}
next_arg(Exp)
next_arg(Arg_List)

ANN static void print_current_args(Exp e) {
  gw_err(_("and not\n  "));
  do gw_err(" \033[32m%s\033[0m", e->type ? e->type->name : "Unknown");
  while((e = next_arg_Exp(e)));
  gw_err("\n");
}

ANN static void print_arg(Arg_List e) {
  do gw_err(" \033[32m%s\033[0m \033[1m%s\033[0m", e->type ? e->type->name : NULL,
       e->var_decl->xid ? s_name(e->var_decl->xid)  : "");
  while((e = next_arg_Arg_List(e)));
}

ANN2(1) static void function_alternative(const Env env, const Type f, const Exp args, const loc_t pos){
  if(env->context->error) // needed for ufcs
    return;
  env_err(env, pos, _("argument type(s) do not match for function. should be :"));
  Func up = f->info->func;
  do {
    gw_err("(%s)  ", up->name);
    const Arg_List e = up->def->base->args;
    if(e)
      print_arg(e);
    else
      gw_err("\033[32mvoid\033[0m");
    gw_err("\n");
  } while((up = up->next));
  if(args)
    print_current_args(args);
  else
    gw_err(_("and not:\n  \033[32mvoid\033[0m\n"));
}

ANN static m_uint get_type_number(ID_List list) {
  m_uint type_number = 0;
  do ++type_number;
  while((list = list->next));
  return type_number;
}

ANN static Func get_template_func(const Env env, Exp_Call *const func, const Value v) {
  const Func f = find_template_match(env, v, func);
  if(f) {
// copy that tmpl->call?
    Tmpl* tmpl = new_tmpl_call(env->gwion->mp, func->tmpl->call);
    tmpl->list = v->d.func_ref ? v->d.func_ref->def->base->tmpl->list : func->func->type->info->func->def->base->tmpl->list;
    ((Exp_Call*)func)->tmpl = tmpl;
    func->func->type = f->value_ref->type;
    return f;
  }
  ((Exp_Call*)func)->tmpl = NULL;
  assert(exp_self(func));
  ERR_O(exp_self(func)->pos,
        _("function is template. automatic type guess not fully implemented yet.\n"
        "  please provide template types. eg: ':[type1, type2, ...]'"))
}

ANN static Func predefined_func(const Env env, const Value v,
    Exp_Call *exp, const Tmpl *tm) {
  Tmpl tmpl = { .call=tm->call };
  exp->tmpl = &tmpl;
  DECL_OO(const Func, func, = get_template_func(env, exp, v))
  return v->d.func_ref = func;
}

ANN static Type check_predefined(const Env env, Exp_Call *exp, const Value v, const Tmpl *tm, const Func_Def fdef) {
  DECL_OO(const Func, func, = v->d.func_ref ?: predefined_func(env, v, exp, tm))
  if(!fdef->base->ret_type) { // template fptr
    struct EnvSet es = { .env=env, .data=env, .func=(_exp_func)check_cdef,
      .scope=env->scope->depth, .flag=tflag_check };
    CHECK_BO(envset_push(&es, v->from->owner_class, v->from->owner))
    func->def->base->fbflag |= fbflag_internal;
    const m_bool ret = check_traverse_fdef(env, func->def);
    if(es.run)
      envset_pop(&es, v->from->owner_class);
    CHECK_BO(ret)
  }
  exp->func->type = func->value_ref->type;
  return func->def->base->ret_type;
}

ANN static Type_List check_template_args(const Env env, Exp_Call *exp, const Tmpl *tm, const Func_Def fdef) {
  m_uint args_number = 0;
  const m_uint type_number = get_type_number(tm->list);
  Type_List tl[type_number];
  tl[0] = NULL;
  ID_List list = tm->list;
  while(list) {
    Arg_List arg = fdef->base->args;
    Exp template_arg = exp->args;
    while(arg && template_arg) {
      if(list->xid == arg->td->xid) {
        tl[args_number] = mk_type_list(env, template_arg->type, fdef->pos);
        if(args_number)
          tl[args_number - 1]->next = tl[args_number];
        ++args_number;
        break;
      }
      arg = arg->next;
      template_arg = template_arg->next;
    }
    list = list->next;
  }
  if(args_number < type_number)
    ERR_O(exp->func->pos, _("not able to guess types for template call."))
  return tl[0];
}

ANN static Type check_exp_call_template(const Env env, Exp_Call *exp) {
  const Type t = exp->func->type;
  DECL_OO(const Value, value, = type_value(env->gwion, t))
  const Func_Def fdef = value->d.func_ref ? value->d.func_ref->def : t->info->func->def;
  Tmpl *tm = fdef->base->tmpl;
  if(tm->call)
    return check_predefined(env, exp, value, tm, fdef);
  DECL_OO(const Type_List, tl, = check_template_args(env, exp, tm, fdef));
  Tmpl tmpl = { .call=tl };
  ((Exp_Call*)exp)->tmpl = &tmpl;
  DECL_OO(const Func,func, = get_template_func(env, exp, value))
  return func->def->base->ret_type;
}

ANN static Type check_lambda_call(const Env env, const Exp_Call *exp) {
  if(exp->args)
    CHECK_OO(check_exp(env, exp->args))
  Exp_Lambda *l = &exp->func->d.exp_lambda;
  Arg_List arg = l->def->base->args;
  Exp e = exp->args;
  while(arg && e) {
    arg->type = e->type;
    arg = arg->next;
    e = e->next;
  }
  if(arg || e)
    ERR_O(exp_self(exp)->pos, _("argument number does not match for lambda"))
  l->def->base->values = env->curr->info->value;
  const m_bool ret = traverse_func_def(env, l->def);
  if(l->def->base->func) {
    free_scope(env->gwion->mp, env->curr->info->value);
    env->curr->info->value = l->def->base->values;
    if(env->class_def)
      set_vflag(l->def->base->func->value_ref, vflag_member);
  }
  exp->func->type = l->def->base->func->value_ref->type;
  if(!l->def->base->ret_type)
    l->def->base->ret_type = env->gwion->type[et_void];
  return ret > 0 ? l->def->base->ret_type : NULL;
}

ANN m_bool func_check(const Env env, Exp_Call *const exp) {
  CHECK_OB(check_exp(env, exp->func))
  if(exp->func->exp_type == ae_exp_decl)
    ERR_B(exp->func->pos, _("Can't call late function pointer at declaration site"))
  const Type t = actual_type(env->gwion, exp->func->type);
  if(isa(t, env->gwion->type[et_function]) > 0 &&
        exp->func->exp_type == ae_exp_dot && !t->info->owner_class) {
    if(exp->args)
      CHECK_OB(check_exp(env, exp->args))
    return call2ufcs(env, exp, t->info->func->value_ref) ?
          GW_OK: GW_ERROR;
  }
  const Exp e = exp_self(exp);
  struct Op_Import opi = { .op=insert_symbol("@func_check"),
    .rhs=t, .pos=e->pos, .data=(uintptr_t)e };
  CHECK_NB(op_check(env, &opi)) // doesn't really return NULL
  if(e->exp_type != ae_exp_call)
    return 0;
  return e->type != env->gwion->type[et_error] ?
    GW_OK : GW_ERROR;
}

ANN Type check_exp_call1(const Env env, Exp_Call *const exp) {
  DECL_BO(const m_bool, ret, = func_check(env, exp))
  if(!ret)
    return exp_self(exp)->type;
  const Type t = actual_type(env->gwion, exp->func->type);
  if(isa(t, env->gwion->type[et_function]) < 0) { // use func flag?
    struct Op_Import opi = { .op=insert_symbol("@ctor"), .rhs=actual_type(env->gwion, exp->func->type),
      .data=(uintptr_t)exp, .pos=exp_self(exp)->pos };
    const Type t = op_check(env, &opi);
    return t;
  }
  if(t == env->gwion->type[et_lambda])
    return check_lambda_call(env, exp);
  if(fflag(t->info->func, fflag_ftmpl)) {
    const Value value = t->info->func->value_ref;
    if(value->from->owner_class)
      CHECK_BO(ensure_traverse(env, value->from->owner_class))
  }
  if(exp->args)
    CHECK_OO(check_exp(env, exp->args))
  if(tflag(t, tflag_ftmpl))
    return check_exp_call_template(env, (Exp_Call*)exp);
  const Func func = find_func_match(env, t->info->func, exp);
  if(func) {
    exp->func->type = func->value_ref->type;
    return func->def->base->ret_type;
  }
  function_alternative(env, exp->func->type, exp->args, exp_self(exp)->pos);
  return NULL;
}

ANN static Type check_exp_binary(const Env env, const Exp_Binary* bin) {
  CHECK_OO(check_exp(env, bin->lhs))
  const m_bool is_auto = bin->rhs->exp_type == ae_exp_decl && bin->rhs->d.exp_decl.type == env->gwion->type[et_auto];
  if(is_auto)
    bin->rhs->d.exp_decl.type = bin->lhs->type;
  CHECK_OO(check_exp(env, bin->rhs))
  if(is_auto)
    bin->rhs->type = bin->lhs->type;
  struct Op_Import opi = { .op=bin->op, .lhs=bin->lhs->type,
    .rhs=bin->rhs->type, .data=(uintptr_t)bin, .pos=exp_self(bin)->pos };
  exp_setuse(bin->lhs, 1);
  exp_setuse(bin->rhs, 1);
  const Type ret = op_check(env, &opi);
  if(!ret && is_auto && exp_self(bin)->exp_type == ae_exp_binary)
    bin->rhs->d.exp_decl.list->self->value->type = env->gwion->type[et_auto];
  return ret;
}

ANN static Type check_exp_cast(const Env env, const Exp_Cast* cast) {
  DECL_OO(const Type, t, = check_exp(env, cast->exp))
  CHECK_OO((exp_self(cast)->type = known_type(env, cast->td)))
  struct Op_Import opi = { .op=insert_symbol("$"), .lhs=t, .rhs=exp_self(cast)->type,
    .data=(uintptr_t)cast, .pos=exp_self(cast)->pos };
  return op_check(env, &opi);
}

ANN static Type check_exp_post(const Env env, const Exp_Postfix* post) {
  struct Op_Import opi = { .op=post->op, .lhs=check_exp(env, post->exp),
    .data=(uintptr_t)post, .pos=exp_self(post)->pos };
  CHECK_OO(opi.lhs)
  exp_setuse(post->exp, 1);
  const Type t = op_check(env, &opi);
  if(t && isa(t, env->gwion->type[et_object]) < 0)
    exp_setmeta(exp_self(post), 1);
  return t;
}

ANN static m_bool predefined_call(const Env env, const Type t, const loc_t pos) {
  const m_str str = tl2str(env->gwion, t->info->func->def->base->tmpl->call, pos);
  env_err(env, pos, _("Type '%s' has '%s' as pre-defined types."),
      t->name, str);
  free_mstr(env->gwion->mp, str);
  if(tflag(t, tflag_typedef)) {
    loc_header(t->info->func->def->pos, env->name);
    gw_err(_("from definition:\n"));
    loc_err(t->info->func->def->pos, env->name);
  }
  return GW_ERROR;
}

ANN static Type check_exp_call(const Env env, Exp_Call* exp) {
  if(exp->tmpl) {
//    CHECK_BO(func_check(env, exp))
    DECL_BO(const m_bool, ret, = func_check(env, exp))
    if(!ret)
      return exp_self(exp)->type;
    const Type t = actual_type(env->gwion, exp->func->type);
    if(isa(t, env->gwion->type[et_function]) < 0)
       return check_exp_call1(env, exp);
    if(exp->args)
      CHECK_OO(check_exp(env, exp->args))
    if(!t->info->func->def->base->tmpl)
      ERR_O(exp_self(exp)->pos, _("template call of non-template function."))
    if(t->info->func->def->base->tmpl->call) {
      if(env->func == t->info->func) {
        exp->func->type = env->func->value_ref->type;
        return env->func->def->base->ret_type;
      } else
        CHECK_BO(predefined_call(env, t, exp_self(exp)->pos))
    }
    const Value v = type_value(env->gwion, t);
    DECL_OO(const Func, f, = find_template_match(env, v, exp))
    exp->func->type = f->value_ref->type;
    return f->def->base->ret_type;
  }
  return check_exp_call1(env, exp);
}

ANN static Type check_exp_unary(const Env env, const Exp_Unary* unary) {
  const Type rhs = unary->unary_type == unary_exp ? check_exp(env, unary->exp) : NULL;
  if(unary->unary_type == unary_exp) {
    CHECK_OO(rhs)
    exp_setuse(unary->exp, 1);
  }
  struct Op_Import opi = { .op=unary->op, .rhs=rhs,
    .data=(uintptr_t)unary, .pos=exp_self(unary)->pos };
  DECL_OO(const Type, ret, = op_check(env, &opi))
  const Type t = actual_type(env->gwion, ret);
  CHECK_BO(ensure_traverse(env, t))
  return ret;
}

ANN static Type _flow(const Env env, const Exp e, const m_bool b) {
  DECL_OO(const Type, type, = check_exp(env, e))
  struct Op_Import opi = { .op=insert_symbol(b ? "@conditionnal" : "@unconditionnal"),
    .rhs=type, .pos=e->pos, .data=(uintptr_t)e };
  return op_check(env, &opi);
}
#define check_flow(emit,b) _flow(emit, b, 1)

ANN static Type check_exp_if(const Env env, const Exp_If* exp_if) {
  const Exp e = exp_if->if_exp ?: exp_if->cond;
  DECL_OO(const Type, cond, = check_flow(env, exp_if->cond))
  DECL_OO(const Type, if_exp, = (exp_if->if_exp ? check_exp(env, exp_if->if_exp) : cond))
  DECL_OO(const Type, else_exp, = check_exp(env, exp_if->else_exp))

  const uint meta = exp_getmeta(e) || exp_getmeta(exp_if->else_exp);
  exp_setmeta(exp_self(exp_if), meta);
  const Type ret = find_common_anc(if_exp, else_exp);
  if(!ret)
    ERR_O(exp_self(exp_if)->pos,
          _("incompatible types '%s' and '%s' in if expression..."),
          if_exp->name, else_exp->name)
  if(isa(if_exp, else_exp) < 0)
    ERR_O(exp_self(exp_if)->pos,
        _("condition type '%s' does not match '%s'"),
         cond->name, ret->name)
  return ret;
}

ANN static Type check_exp_dot(const Env env, Exp_Dot* member) {
  CHECK_OO(check_exp(env, member->base))
  return check_dot(env, member);
}

ANN m_bool check_type_def(const Env env, const Type_Def tdef) {
  return (!is_fptr(env->gwion, tdef->type) && tdef->type->info->cdef) ?
     check_class_def(env, tdef->type->info->cdef) : GW_OK;
}
ANN static Type check_exp_lambda(const Env env,
    const Exp_If* exp_if NUSED) { return env->gwion->type[et_lambda]; }

ANN static Type check_exp_td(const Env env, Type_Decl **td) {
  DECL_OO(const Type, t, = known_type(env, *td))
  return type_class(env->gwion, t);
}

DECL_EXP_FUNC(check, Type, Env)

ANN Type check_exp(const Env env, const Exp exp) {
  Exp curr = exp;
  if(!exp->type) {
    do {
      CHECK_OO((curr->type = check_exp_func[curr->exp_type](env, &curr->d)))
      if(env->func && isa(curr->type, env->gwion->type[et_lambda]) < 0 && isa(curr->type, env->gwion->type[et_function]) > 0 &&
          !fflag(curr->type->info->func, fflag_pure))
        unset_fflag(env->func, fflag_pure);
    } while((curr = curr->next));
  }
  return exp->type;
}

ANN m_bool check_enum_def(const Env env, const Enum_Def edef) {
  if(env->class_def) {
    ID_List list = edef->list;
    do decl_static(env, nspc_lookup_value0(env->curr, list->xid));
    while((list = list->next));
  }
  return GW_OK;
}

ANN static m_bool check_stmt_code(const Env env, const Stmt_Code stmt) {
  if(stmt->stmt_list) { RET_NSPC(check_stmt_list(env, stmt->stmt_list)) }
  return GW_OK;
}

ANN static m_bool check_stmt_varloop(const Env env, const Stmt_VarLoop stmt) {
  CHECK_OB(check_exp(env, stmt->exp))
  if(isa(stmt->exp->type, env->gwion->type[et_vararg]) < 0)
    ERR_B(stmt->exp->pos, "varloop expression type must be '%s', not '%s'",
      env->gwion->type[et_vararg]->name, stmt->exp->type->name)
  return check_stmt(env, stmt->body);
}

ANN static inline m_bool _check_breaks(const Env env, const Stmt b) {
  RET_NSPC(check_stmt(env, b))
}

ANN static m_bool check_breaks(const Env env, const Stmt a, const Stmt b) {
  vector_add(&env->scope->breaks, (vtype)a);
  const m_bool ret = _check_breaks(env, b);
  vector_pop(&env->scope->breaks);
  return ret;
}

ANN static m_bool check_conts(const Env env, const Stmt a, const Stmt b) {
  vector_add(&env->scope->conts, (vtype)a);
  CHECK_BB(check_breaks(env, a, b))
  vector_pop(&env->scope->conts);
  return GW_OK;
}

ANN static inline m_bool for_empty(const Env env, const Stmt_For stmt) {
  if(!stmt->c2 || !stmt->c2->d.stmt_exp.val)
    ERR_B(stmt_self(stmt)->pos, _("empty for loop condition..."
          "...(note: explicitly use 'true' if it's the intent)"
          "...(e.g., 'for(; true;){ /*...*/ }')"))
  return GW_OK;
}

ANN static inline Type foreach_type(const Env env, const Exp exp) {
  DECL_OO(Type, et, = check_exp(env, exp))
  if(isa(et, env->gwion->type[et_array]) < 0)
    ERR_O(exp->pos, _("type '%s' is not array.\n"
          " This is not allowed in foreach loop"), et->name)
  DECL_OO(Type, base, = typedef_base(et))
  DECL_OO(const Type, t, = array_base(base))
  const m_uint depth = base->array_depth - 1;
  return depth ? array_type(env, t, depth) : t;

}

ANN static m_bool do_stmt_each(const Env env, const Stmt_Each stmt) {
  DECL_OB(const Type, base, = foreach_type(env, stmt->exp))
  CHECK_BB(ensure_traverse(env, base))
  const m_str basename = type2str(env->gwion, base, stmt->exp->pos);
  char c[15 + strlen(basename)];
  sprintf(c, "Ref:[%s]", basename);
  const Type ret = str2type(env->gwion, c, stmt->exp->pos);
  if(base->array_depth)
    set_tflag(ret, tflag_typedef);
  stmt->v = new_value(env->gwion->mp, ret, s_name(stmt->sym));
  set_vflag(stmt->v, vflag_valid);
  nspc_add_value(env->curr, stmt->sym, stmt->v);
  if(stmt->idx) {
    stmt->vidx = new_value(env->gwion->mp, env->gwion->type[et_int], s_name(stmt->idx));
    set_vflag(stmt->vidx, vflag_valid);
    nspc_add_value(env->curr, stmt->idx, stmt->vidx);
  }
  return check_conts(env, stmt_self(stmt), stmt->body);
}

ANN static inline m_bool cond_type(const Env env, const Exp e) {
  const Type t_int = env->gwion->type[et_int];
  if(check_implicit(env, e, t_int) < 0)
    ERR_B(e->pos, _("invalid condition type"))
  return GW_OK;
}

#define stmt_func_xxx(name, type, prolog, exp) describe_stmt_func(check, name, type, prolog, exp)
stmt_func_xxx(if, Stmt_If,, !(!check_flow(env, stmt->cond)   ||
  check_stmt(env, stmt->if_body) < 0 ||
  (stmt->else_body && check_stmt(env, stmt->else_body) < 0)) ? 1 : -1)
stmt_func_xxx(flow, Stmt_Flow,,
  !(!check_exp(env, stmt->cond) ||
    !_flow(env, stmt->cond, !stmt->is_do ?
       stmt_self(stmt)->stmt_type == ae_stmt_while :
       stmt_self(stmt)->stmt_type != ae_stmt_while) ||
    check_conts(env, stmt_self(stmt), stmt->body) < 0) ? 1 : -1)
stmt_func_xxx(for, Stmt_For,, !(
  for_empty(env, stmt) < 0 ||
  check_stmt(env, stmt->c1) < 0 ||
  !check_flow(env, stmt->c2->d.stmt_exp.val) ||
  (stmt->c3 && !check_exp(env, stmt->c3)) ||
  check_conts(env, stmt_self(stmt), stmt->body) < 0) ? 1 : -1)
stmt_func_xxx(loop, Stmt_Loop,, !(!check_exp(env, stmt->cond) ||
  cond_type(env, stmt->cond) < 0 ||
  check_conts(env, stmt_self(stmt), stmt->body) < 0) ? 1 : -1)
stmt_func_xxx(each, Stmt_Each,, do_stmt_each(env, stmt))

ANN static m_bool check_stmt_return(const Env env, const Stmt_Exp stmt) {
  if(!env->func)
    ERR_B(stmt_self(stmt)->pos, _("'return' statement found outside function definition"))
  DECL_OB(const Type, ret_type, = stmt->val ? check_exp(env, stmt->val) : env->gwion->type[et_void])
  if(!env->func->def->base->ret_type) {
    assert(isa(env->func->value_ref->type, env->gwion->type[et_lambda]) > 0);
    env->func->def->base->ret_type = ret_type;
    return GW_OK;
  }
  if(isa(ret_type, env->func->def->base->ret_type) > 0)
    return GW_OK;
  if(stmt->val) {
    if(env->func->def->base->xid == insert_symbol("@implicit") && ret_type == env->func->def->base->args->type)
      ERR_B(stmt_self(stmt)->pos, _("can't use implicit casting while defining it"))
    if(check_implicit(env, stmt->val, env->func->def->base->ret_type) > 0)
      return GW_OK;
    ERR_B(stmt_self(stmt)->pos, _("invalid return type: got '%s', expected '%s'"),
        ret_type->name, env->func->def->base->ret_type->name)
  }
  if(isa(env->func->def->base->ret_type, env->gwion->type[et_void]) > 0)
    return GW_OK;
  ERR_B(stmt_self(stmt)->pos, _("missing value for return statement"))
}

#define describe_check_stmt_stack(stack, name)                                     \
ANN static m_bool check_stmt_##name(const Env env, const Stmt stmt) {\
  if(!vector_size(&env->scope->stack))                                                    \
    ERR_B(stmt->pos, _("'"#name"' found outside of for/while/until..."))             \
  return GW_OK;                                                                        \
}
describe_check_stmt_stack(conts,  continue)
describe_check_stmt_stack(breaks, break)

ANN m_bool check_union_def(const Env env NUSED, const Union_Def udef) {
  if(tmpl_base(udef->tmpl)) // there's a func for this
    return GW_OK;
  set_tflag(udef->type, tflag_check);
  return GW_OK;
}

ANN static m_bool check_stmt_exp(const Env env, const Stmt_Exp stmt) {
  return stmt->val ? check_exp(env, stmt->val) ? 1 : -1 : 1;
}

ANN static Value match_value(const Env env, const Type base, const Exp_Primary* prim) {
  const Symbol sym = prim->d.var;
  const Value v = new_value(env->gwion->mp, base, s_name(sym));
  set_vflag(v, vflag_valid);
  nspc_add_value(env->curr, sym, v);
  return v;
}

ANN static Symbol case_op(const Env env, const Type base, const Exp e) {
  if(e->exp_type == ae_exp_primary) {
    if(e->d.prim.prim_type == ae_prim_id) {
      if(e->d.prim.d.var == insert_symbol("_"))
        return NULL;
      if(!nspc_lookup_value1(env->curr, e->d.prim.d.var)) {
        e->d.prim.value = match_value(env, base, &e->d.prim);
        return NULL;
      }
    }
  } else if(isa(base, env->gwion->type[et_union]) > 0 && e->exp_type == ae_exp_call) {
    const Exp func = e->d.exp_call.func;
    if(func->d.prim.prim_type == ae_prim_id) {
      const Value v= find_value(base, func->d.prim.d.var);
      if(v) {
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
  for(m_uint i = 0; i < vector_size(&env->scope->match->cond); e = e->next, ++i) {
    if(!e)
      ERR_B(last->pos, _("no enough to match"))
    last = e;
    const Exp base = (Exp)vector_at(&env->scope->match->cond, i);
    const Symbol op = case_op(env, base->type, e);
    if(op) {
      const Exp next = e->next;
      e->next = NULL;
      const Type t = check_exp(env, e);
      e->next = next;
      CHECK_OB(t)
      Exp_Binary bin = { .lhs=base, .rhs=e, .op=op };
      struct Exp_ ebin = { .d={.exp_binary=bin} };
      struct Op_Import opi = { .op=op, .lhs=base->type, .rhs=e->type,
        .data=(uintptr_t)&ebin.d.exp_binary, .pos=e->pos };
      CHECK_OB(op_check(env, &opi))
    }
  }
  if(e)
    ERR_B(e->pos, _("too many expression to match"))
  return GW_OK;
}

ANN static m_bool _check_stmt_case(const Env env, const Stmt_Match stmt) {
  CHECK_BB(match_case_exp(env, stmt->cond))
  if(stmt->when)
    CHECK_OB(check_flow(env, stmt->when))
  return check_stmt_list(env, stmt->list);
}

ANN static m_bool check_stmt_case(const Env env, const Stmt_Match stmt) {
  RET_NSPC(_check_stmt_case(env, stmt))
}

ANN static m_bool case_loop(const Env env, const Stmt_Match stmt) {
  Stmt_List list = stmt->list;
  do CHECK_BB(check_stmt_case(env, &list->stmt->d.stmt_match))
  while((list = list->next));
  return GW_OK;
}

ANN static m_bool _check_stmt_match(const Env env, const Stmt_Match stmt) {
  CHECK_OB(check_exp(env, stmt->cond))
  MATCH_INI(env->scope)
  const m_bool ret = case_loop(env, stmt);
  MATCH_END(env->scope)
  return ret;
}

ANN static inline m_bool handle_where(const Env env, const Stmt_Match stmt) {
  if(stmt->where)
    CHECK_BB(check_stmt(env, stmt->where))
  RET_NSPC(_check_stmt_match(env, stmt))
}

ANN static m_bool check_stmt_match(const Env env, const Stmt_Match stmt) {
  RET_NSPC(handle_where(env, stmt))
}

#define check_stmt_while check_stmt_flow
#define check_stmt_until check_stmt_flow

ANN static m_bool check_stmt_pp(const Env env, const Stmt_PP stmt) {
  if(stmt->pp_type == ae_pp_include)
    env->name = stmt->data;
  return GW_OK;
}

ANN static m_bool check_stmt_defer(const Env env, const Stmt_Defer stmt) {
  return check_stmt(env, stmt->stmt);
}

DECL_STMT_FUNC(check, m_bool , Env)

ANN m_bool check_stmt(const Env env, const Stmt stmt) {
  return check_stmt_func[stmt->stmt_type](env, &stmt->d);
}

ANN static m_bool check_stmt_list(const Env env, Stmt_List l) {
  do CHECK_BB(check_stmt(env, l->stmt))
  while((l = l->next));
  return GW_OK;
}

ANN static m_bool check_signature_match(const Env env, const Func_Def fdef, const Func parent) {
  if(GET_FLAG(parent->def->base, final))
    ERR_B(fdef->base->td->pos, _("can't override final function '%s'\n"), parent->name)
  if(GET_FLAG(parent->def->base, static) != GET_FLAG(fdef->base, static)) {
    const m_str c_name  = fdef->base->func->value_ref->from->owner_class->name;
    const m_str p_name = parent->value_ref->from->owner_class->name;
    const m_str f_name = s_name(fdef->base->xid);
    ERR_B(fdef->base->td->pos,
          _("function '%s.%s' ressembles '%s.%s' but cannot override...\n"
          "  ...(reason: '%s.%s' is declared as 'static')"),
          c_name, f_name, p_name, c_name,
          GET_FLAG(fdef->base, static) ? c_name : p_name, f_name)
  }
  return !fdef->base->tmpl ? isa(fdef->base->ret_type, parent->def->base->ret_type) : GW_OK;
}

ANN static m_bool parent_match_actual(const Env env, const restrict Func_Def fdef,
    const restrict Func func) {
  Func parent_func = func;
  do {
    if(parent_func->def->base && compat_func(fdef, parent_func->def) > 0) {
      CHECK_BB(check_signature_match(env, fdef, parent_func))
      if(!fdef->base->tmpl) {
        fdef->base->func->vt_index = parent_func->vt_index;
        vector_set(&env->curr->info->vtable, fdef->base->func->vt_index, (vtype)fdef->base->func);
      }
      return GW_OK;
    }
  } while((parent_func = parent_func->next));
  return 0;
}

ANN static m_bool check_parent_match(const Env env, const Func_Def fdef) {
  const Func func = fdef->base->func;
  const Type parent = env->class_def->info->parent;
  if(!env->curr->info->vtable.ptr)
    vector_init(&env->curr->info->vtable);
  if(parent) {
    const Value v = find_value(parent, fdef->base->xid);
    if(v && isa(v->type, env->gwion->type[et_function]) > 0) {
      const m_bool match = parent_match_actual(env, fdef, v->d.func_ref);
      if(match)
        return match;
    }
  }
  func->vt_index = vector_size(&env->curr->info->vtable);
  vector_add(&env->curr->info->vtable, (vtype)func);
  return GW_OK;
}

ANN static inline Func get_overload(const Env env, const Func_Def fdef, const m_uint i) {
  const Symbol sym = func_symbol(env, env->curr->name, s_name(fdef->base->xid), NULL, i);
  return nspc_lookup_func1(env->curr, sym);
}

ANN static m_bool check_func_overload(const Env env, const Func_Def fdef) {
  const Value v = fdef->base->func->value_ref;
  for(m_uint i = 0; i <= v->from->offset; ++i) {
    const Func f1 = get_overload(env, fdef, i);
    for(m_uint j = i + 1; f1 && j <= v->from->offset; ++j) {
      const Func f2 = get_overload(env, fdef, j);
      if(f2 && compat_func(f1->def, f2->def) > 0 &&
          fbflag(f1->def->base, fbflag_op) == fbflag(f2->def->base, fbflag_op) &&
          fbflag(f1->def->base, fbflag_unary) == fbflag(f2->def->base, fbflag_unary) &&
          fbflag(f1->def->base, fbflag_postfix) == fbflag(f2->def->base, fbflag_postfix))
        ERR_B(f2->def->base->td->pos, _("global function '%s' already defined"
          " for those arguments"), s_name(fdef->base->xid))
    }
  }
  return GW_OK;
}

ANN static m_bool check_func_def_override(const Env env, const Func_Def fdef) {
  const Func func = fdef->base->func;
  if(env->class_def && env->class_def->info->parent) {
    const Value override = find_value(env->class_def->info->parent, fdef->base->xid);
    if(override && override->from->owner_class && isa(override->type, env->gwion->type[et_function]) < 0)
      ERR_B(fdef->pos,
            _("function name '%s' conflicts with previously defined value...\n"
            "  from super class '%s'..."),
            s_name(fdef->base->xid), override->from->owner_class->name)
  }
  if(func->value_ref->from->offset && (!fdef->base->tmpl || !fdef->base->tmpl->base))
    CHECK_BB(check_func_overload(env, fdef))
  return GW_OK;
}

ANN m_bool check_fdef(const Env env, const Func_Def fdef) {
  if(fdef->base->args)
    CHECK_BB(check_func_args(env, fdef->base->args))
  if(fdef->d.code)
    CHECK_BB(check_stmt_code(env, &fdef->d.code->d.stmt_code))
  return GW_OK;
}

ANN m_bool check_func_def(const Env env, const Func_Def f) {
  const Func func = f->base->func;
  const Func_Def fdef = func->def;
  assert(func == fdef->base->func);
  if(env->class_def) // tmpl ?
    CHECK_BB(check_parent_match(env, fdef))
  if(tmpl_base(fdef->base->tmpl))
    return GW_OK;
  CHECK_BB(check_func_def_override(env, fdef))
  DECL_BB(const m_int, scope, = GET_FLAG(fdef->base, global) ? env_push_global(env) : env->scope->depth)
  const Func former = env->func;
  env->func = func;
  ++env->scope->depth;
  nspc_push_value(env->gwion->mp, env->curr);
  struct Op_Import opi = { };
  if(fbflag(fdef->base, fbflag_op)) {
    func_operator(f, &opi);
    operator_suspend(env->curr, &opi);
  }
  const m_bool ret = scanx_fdef(env, env, fdef, (_exp_func)check_fdef);
  if(fbflag(fdef->base, fbflag_op))
    operator_resume(&opi);
  nspc_pop_value(env->gwion->mp, env->curr);
  --env->scope->depth;
  env->func = former;
  if(ret > 0)
    set_fflag(fdef->base->func, fflag_valid);
  if(GET_FLAG(fdef->base, global))
    env_pop(env,scope);
  return ret;
}

#define check_fptr_def dummy_func
HANDLE_SECTION_FUNC(check, m_bool, Env)

ANN static m_bool check_parent(const Env env, const Class_Def cdef) {
  const Type parent = cdef->base.type->info->parent;
  const Type_Decl *td = cdef->base.ext;
//  if(td->array)
  if(td->array && td->array->exp)
    CHECK_BB(check_subscripts(env, td->array, 1))
  CHECK_BB(ensure_check(env, parent))
//  if(tflag(parent, tflag_typedef)) {
//    set_tflag(cdef->base.type, tflag_typedef);
//  }
  return GW_OK;
}

ANN static m_bool cdef_parent(const Env env, const Class_Def cdef) {
  if(cdef->base.tmpl && cdef->base.tmpl->list)
    CHECK_BB(template_push_types(env, cdef->base.tmpl))
  const m_bool ret = check_parent(env, cdef);
  if(cdef->base.tmpl && cdef->base.tmpl->list)
    nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}

ANN m_bool check_abstract(const Env env, const Class_Def cdef) {
  if(!cdef->base.type->nspc->info->vtable.ptr)
    return GW_OK;
  for(m_uint i = 0; i < vector_size(&cdef->base.type->nspc->info->vtable); ++i) {
    Func f = (Func)vector_at(&cdef->base.type->nspc->info->vtable, i);
    if(GET_FLAG(f->def->base, abstract))
      ERR_B(cdef->pos, _("'%s' must be declared 'abstract'"), s_name(cdef->base.xid))
  }
  return GW_OK;
}

ANN m_bool check_class_def(const Env env, const Class_Def cdef) {
  if(tmpl_base(cdef->base.tmpl))
    return GW_OK;
  const Type t = cdef->base.type;
  struct Op_Import opi = { .op=insert_symbol("@class_check"), .lhs=t,
        .data=(uintptr_t)cdef, .pos=cdef->pos };
  CHECK_OB(op_check(env, &opi))
  if(t->info->owner_class)
    CHECK_BB(ensure_check(env, t->info->owner_class))
  if(tflag(t, tflag_check))
    return GW_OK;
  set_tflag(t, tflag_check);
  if(cdef->base.ext)
    CHECK_BB(cdef_parent(env, cdef))
  if(!tflag(t, tflag_struct))
    inherit(t);
  if(cdef->body) {
    CHECK_BB(env_body(env, cdef, check_section))
    set_tflag(t, tflag_ctor);
  }
  if(!GET_FLAG(cdef, abstract))
    CHECK_BB(check_abstract(env, cdef))
  return GW_OK;
}

ANN m_bool check_ast(const Env env, Ast ast) {
  do CHECK_BB(check_section(env, ast->section))
  while((ast = ast->next));
  return GW_OK;
}
