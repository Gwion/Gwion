#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "template.h"
#include "parse.h"
#include "object.h"
#include "operator.h"
#include "instr.h"
#include "import.h"

ANN static m_bool scan1_stmt_list(const Env env, Stmt_List list);
ANN static m_bool scan1_stmt(const Env env, Stmt stmt);

ANN static inline m_bool type_cyclic(const Env env, const Type t,
                                     const Type_Decl *td) {
  Type owner = env->class_def;
  do {
    Type parent = t;
    do {
      if (parent == owner)
        ERR_B(td->pos, _("%s declared inside %s"), t->name, owner->name);
    } while ((parent = parent->info->parent));
  } while ((owner = owner->info->value->from->owner_class));
  return GW_OK;
}

ANN static inline m_bool ensure_scan1(const Env env, const Type t) {
  if (tflag(t, tflag_scan1) || !(tflag(t, tflag_cdef) || tflag(t, tflag_udef)))
    return GW_OK;
  struct EnvSet es = {.env   = env,
                      .data  = env,
                      .func  = (_exp_func)scan1_cdef,
                      .scope = env->scope->depth,
                      .flag  = tflag_scan1};
  return envset_run(&es, t);
}

ANN static m_bool check_global(const Env env, const Type t, const loc_t pos) {
  const ValueFrom *from = t->info->value->from;
  if(from->owner_class && isa(from->owner_class, env->class_def) > 0)
    return true;
  if(from_global_nspc(env, from->owner) ||
    (from->owner_class && type_global(env, from->owner_class)))
      return true;
  gwerr_basic("can't use non-global type in a global class", NULL, NULL, env->name, pos, 0);
  gwerr_secondary_from("not declared global", from);
  const ValueFrom *ownerFrom = env->class_def->info->value->from;
  gwerr_secondary_from("is global", ownerFrom);
  env_set_error(env, true);
  return false;
}

ANN static Type scan1_type(const Env env, Type_Decl *td) {
  DECL_OO(const Type, t, = known_type(env, td));
  const Type base = array_base(t);
  if (!env->func && env->class_def && !GET_FLAG(td, late))
    CHECK_BO(type_cyclic(env, base, td));
  CHECK_BO(ensure_scan1(env, t));
  return t;
}

ANN static Type void_type(const Env env, Type_Decl *td) {
  DECL_OO(const Type, type, = scan1_type(env, td));
  if (type->size) return type;
  ERR_O(td->pos, _("cannot declare variables of size '0' (i.e. 'void')..."))
}

ANN static Type scan1_exp_decl_type(const Env env, Exp_Decl *decl) {
  if (decl->type) return decl->type;
  DECL_OO(const Type, t, = void_type(env, decl->td));
  if(env->class_def && type_global(env, env->class_def) && !check_global(env, t, decl->td->pos))
      return NULL;
  if (decl->td->xid == insert_symbol("auto") && decl->type) return decl->type;
  if (GET_FLAG(t, private) && t->info->value->from->owner != env->curr)
    ERR_O(exp_self(decl)->pos, _("can't use private type %s"), t->name)
  if (GET_FLAG(t, protect) && (!env->class_def || isa(t, env->class_def) < 0))
    ERR_O(exp_self(decl)->pos, _("can't use protected type %s"), t->name)
  return t;
}

static inline m_bool scan1_defined(const Env env, const Var_Decl *var) {
  if (var->value) // from an auto declaration
    return GW_OK;
  const Value v = ((!env->class_def || !GET_FLAG(env->class_def, final) ||
        env->scope->depth)
           ? nspc_lookup_value1
           : nspc_lookup_value2)(env->curr, var->xid);
  if(v && (!v->from->owner_class || isa(env->class_def, v->from->owner_class) > 0))
    ERR_B(var->pos,
          _("variable %s has already been defined in the same scope..."),
          s_name(var->xid))
  return GW_OK;
}

ANN m_bool abstract_array(const Env env, const Array_Sub array) {
  Exp e = array->exp;
  while(e) {
    if(!exp_is_zero(e))
      ERR_B(e->pos, _("arrays of abstract type should use `0` size"));
    e = e->next;
  }
  return GW_OK;
}

ANN static m_bool scan1_decl(const Env env, Exp_Decl *const decl) {
  const bool decl_ref = decl->td->array && !decl->td->array->exp;
  Var_Decl *const vd = &decl->vd;
  CHECK_BB(isres(env, vd->xid, exp_self(decl)->pos));
  Type t = decl->type;
  CHECK_BB(scan1_defined(env, vd));
  const Type base = array_base_simple(t);
  if(decl->td->array) {
    if (!GET_FLAG(decl->td, late) && !decl->td->array->exp)
      ERR_B(decl->td->pos, _("arrays with no expressions should be declared `late`"));
    if (GET_FLAG(decl->td, late) && decl->td->array->exp)
      ERR_B(decl->td->array->exp->pos, _("late array should have no size"));
    if (!decl->args && GET_FLAG(base, abstract)) CHECK_BB(abstract_array(env, decl->td->array));
  }
  const Value v = vd->value =
      vd->value ?: new_value(env, t, s_name(vd->xid), vd->pos);
  if (GET_FLAG(t, abstract) && !decl->args && !GET_FLAG(decl->td, late)) SET_FLAG(v, late);
  v->type = t;
  if (decl_ref) SET_FLAG(v, late);
  v->flag |= decl->td->flag;
  if (!env->scope->depth) {
    valuefrom(env, v->from);
    if (env->class_def) {
      if (env->class_def->info->tuple) tuple_contains(env, v);
      if (!GET_FLAG(decl->td, static)) {
        set_vflag(v, vflag_member);
        if(isa(t, env->gwion->type[et_object]) > 0)
          set_vflag(v, vflag_release);
        if (tflag(env->class_def, tflag_struct)) {
          v->from->offset = env->class_def->size;
          env->class_def->size += t->size;
        }
      }
    } else if (GET_FLAG(decl->td, global))
      SET_FLAG(v, global);
    else if(env->context)
      set_vflag(v, vflag_fglobal); // file global
  } else if (GET_FLAG(decl->td, global))
    SET_FLAG(v, global);
  nspc_add_value(env->curr, vd->xid, v);
  ((Exp_Decl *)decl)->type = decl->vd.value->type;
  return GW_OK;
}

ANN m_bool scan1_exp_decl(const Env env, Exp_Decl *const decl) {
  CHECK_BB(env_storage(env, decl->td->flag, exp_self(decl)->pos));
  ((Exp_Decl *)decl)->type = scan1_exp_decl_type(env, (Exp_Decl *)decl);
  CHECK_OB(decl->type);
  if(decl->args) CHECK_BB(scan1_exp(env, decl->args));
  const bool global = GET_FLAG(decl->td, global);
  if (global) {
    if (env->context) env->context->global = true;
    if (!type_global(env, decl->type))
      ERR_B(exp_self(decl)->pos, _("type '%s' is not global"), decl->type->name)
  }
  const m_uint scope = !global ? env->scope->depth : env_push_global(env);
  const m_bool ret   = scan1_decl(env, decl);
  if (global) env_pop(env, scope);
  return ret;
}

ANN static inline m_bool scan1_exp_binary(const Env         env,
                                          const Exp_Binary *bin) {
  CHECK_BB(scan1_exp(env, bin->lhs));
  return scan1_exp(env, bin->rhs);
}

ANN static m_bool scan1_range(const Env env, Range *range) {
  if (range->start) CHECK_BB(scan1_exp(env, range->start));
  if (range->end) CHECK_BB(scan1_exp(env, range->end));
  return GW_OK;
}

ANN static inline m_bool scan1_prim(const Env env, const Exp_Primary *prim) {
  if (prim->prim_type == ae_prim_dict || prim->prim_type == ae_prim_interp)
    return scan1_exp(env, prim->d.exp);
  if (prim->prim_type == ae_prim_hack) {
    if(env->func) env->func->weight = 1; // mark function has having gack
    // we should use effects when typechecking for that
    return scan1_exp(env, prim->d.exp);
  }
  if (prim->prim_type == ae_prim_array && prim->d.array->exp)
    return scan1_exp(env, prim->d.array->exp);
  if (prim->prim_type == ae_prim_range) return scan1_range(env, prim->d.range);
  if (env->func && prim->prim_type == ae_prim_perform && env->scope->depth <= 2)
    env->func->memoize = 1;
  return GW_OK;
}

ANN static inline m_bool scan1_exp_array(const Env        env,
                                         const Exp_Array *array) {
  CHECK_BB(scan1_exp(env, array->base));
  return scan1_exp(env, array->array->exp);
}

ANN static inline m_bool scan1_exp_slice(const Env        env,
                                         const Exp_Slice *range) {
  CHECK_BB(scan1_exp(env, range->base));
  return scan1_range(env, range->range);
}

ANN static inline m_bool scan1_exp_cast(const Env env, const Exp_Cast *cast) {
  return scan1_exp(env, cast->exp);
}

ANN static m_bool scan1_exp_post(const Env env, const Exp_Postfix *post) {
  CHECK_BB(scan1_exp(env, post->exp));
  const m_str access = exp_access(post->exp);
  if (!access) return GW_OK;
  ERR_B(post->exp->pos,
        _("post operator '%s' cannot be used"
          " on %s data-type..."),
        s_name(post->op), access);
}

ANN static m_bool scan1_exp_call(const Env env, const Exp_Call *exp_call) {
  if (exp_call->tmpl) return GW_OK;
  CHECK_BB(scan1_exp(env, exp_call->func));
  const Exp args = exp_call->args;
  return args ? scan1_exp(env, args) : GW_OK;
}

ANN static inline m_bool scan1_exp_dot(const Env env, const Exp_Dot *member) {
  return scan1_exp(env, member->base);
}

ANN static m_bool scan1_exp_if(const Env env, const Exp_If *exp_if) {
  CHECK_BB(scan1_exp(env, exp_if->cond));
  if(exp_if->if_exp) CHECK_BB(scan1_exp(env, exp_if->if_exp));
  return scan1_exp(env, exp_if->else_exp);
}

ANN static inline m_bool scan1_exp_unary(const restrict Env env,
                                         Exp_Unary *const  unary) {
  if(unary->unary_type == unary_exp)
    return scan1_exp(env, unary->exp);
  if (unary->unary_type == unary_code)
    return scan1_stmt_list(env, unary->code);
  return GW_OK;
}

#define scan1_exp_lambda dummy_func
#define scan1_exp_td     dummy_func
HANDLE_EXP_FUNC(scan1, m_bool, Env)

ANN static inline m_bool _scan1_stmt_match_case(const restrict Env env,
                                                const Stmt_Match   stmt) {
  CHECK_BB(scan1_exp(env, stmt->cond));
  if (stmt->when) CHECK_BB(scan1_exp(env, stmt->when));
  return scan1_stmt_list(env, stmt->list);
}

ANN static inline m_bool scan1_stmt_match_case(const restrict Env env,
                                               const Stmt_Match   stmt) {
    RET_NSPC(_scan1_stmt_match_case(env, stmt))}

ANN static inline m_bool
    _scan1_stmt_match(const restrict Env env, const Stmt_Match stmt) {
  if (stmt->where) CHECK_BB(scan1_stmt(env, stmt->where));
  Stmt_List l = stmt->list;
  for(m_uint i = 0; i < l->len; i++) {
    const Stmt s = mp_vector_at(l, struct Stmt_, i);
    CHECK_BB(scan1_stmt_match_case(env, &s->d.stmt_match));
  }
  return GW_OK;
}

ANN static inline m_bool scan1_stmt_match(const restrict Env env,
                                          const Stmt_Match   stmt) {
  CHECK_BB(scan1_exp(env, stmt->cond));
  RET_NSPC(_scan1_stmt_match(env, stmt))
}

ANN static inline m_bool scan1_handler(const restrict Env env,
                                            const Handler *handler) {
  RET_NSPC(scan1_stmt(env, handler->stmt));
}

ANN static inline m_bool scan1_handler_list(const restrict Env env,
                                            const Handler_List handlers) {
  for(uint32_t i = 0; i < handlers->len; i++) {
    Handler * handler = mp_vector_at(handlers, Handler, i);
    CHECK_BB(scan1_handler(env, handler));
  }
  return GW_OK;
}

ANN static inline m_bool scan1_stmt_try(const restrict Env env,
                                        const Stmt_Try     stmt) {
  CHECK_BB(scan1_handler_list(env, stmt->handler));
  RET_NSPC(scan1_stmt(env, stmt->stmt))
}

ANN static inline m_bool stmt_each_defined(const restrict Env env,
                                           const Stmt_Each    stmt) {
  if (nspc_lookup_value1(env->curr, stmt->sym))
    ERR_B(stmt_self(stmt)->pos, _("foreach value '%s' is already defined"),
          s_name(stmt->sym))
  if (stmt->idx && nspc_lookup_value1(env->curr, stmt->idx->sym))
    ERR_B(stmt->idx->pos, _("foreach index '%s' is already defined"),
          s_name(stmt->idx->sym))
  return GW_OK;
}

ANN static inline m_bool shadow_err(const Env env, const Value v,
                                    const loc_t loc) {
  if(env->scope->shadowing) return GW_OK;
  gwerr_basic(_("shadowing a previously defined variable"), NULL, NULL,
              env->name, loc, 0);
  defined_here(v);
  env_set_error(env, true);
  return GW_ERROR;
}

ANN static inline m_bool shadow_arg(const Env env, const Symbol sym,
                                    const loc_t loc) {
  Nspc nspc = env->curr;
  do {
    const Value v = nspc_lookup_value0(nspc, sym);
    if (v && !env->func->def->builtin) {
      const Type owner = v->from->owner_class;
      if (owner && env->class_def && isa(env->class_def, owner) < 0)
        continue;
      return shadow_err(env, v, loc);
    }
  } while ((nspc = nspc->parent));
  return GW_OK;
}

ANN static inline m_bool shadow_var(const Env env, const Symbol sym,
                                    const loc_t loc) {
  const Value v = nspc_lookup_value1(env->curr, sym);
  return !v ? GW_OK : shadow_err(env, v, loc);
}

#define describe_ret_nspc(name, type, prolog, exp)                             \
  describe_stmt_func(scan1, name, type, prolog, exp)
describe_ret_nspc(flow, Stmt_Flow,, !(scan1_exp(env, stmt->cond) < 0 ||
    scan1_stmt(env, stmt->body) < 0) ? 1 : -1)
describe_ret_nspc(for, Stmt_For,, !(scan1_stmt(env, stmt->c1) < 0 ||
    scan1_stmt(env, stmt->c2) < 0 ||
    (stmt->c3 && scan1_exp(env, stmt->c3) < 0) ||
    scan1_stmt(env, stmt->body) < 0) ? 1 : -1)
describe_ret_nspc(each, Stmt_Each,, !(stmt_each_defined(env, stmt) < 0 || scan1_exp(env, stmt->exp) < 0 ||
    scan1_stmt(env, stmt->body) < 0) ? 1 : -1)
describe_ret_nspc(loop, Stmt_Loop,, !( (!stmt->idx ? GW_OK : shadow_var(env, stmt->idx->sym, stmt->idx->pos)) < 0 ||
    scan1_exp(env, stmt->cond) < 0 ||
    scan1_stmt(env, stmt->body) < 0) ? 1 : -1)
describe_ret_nspc(if, Stmt_If,, !(scan1_exp(env, stmt->cond) < 0 ||
    scan1_stmt(env, stmt->if_body) < 0 ||
    (stmt->else_body && scan1_stmt(env, stmt->else_body) < 0)) ? 1 : -1)

ANN static inline m_bool scan1_stmt_code(const Env env, const Stmt_Code stmt) {
  if (stmt->stmt_list) { RET_NSPC(scan1_stmt_list(env, stmt->stmt_list)) }
  return GW_OK;
}

ANN static inline m_bool scan1_stmt_exp(const Env env, const Stmt_Exp stmt) {
  return stmt->val ? scan1_exp(env, stmt->val) : 1;
}

ANN m_bool scan1_enum_def(const Env env, const Enum_Def edef) {
  const Type t = edef->type;
  t->nspc = new_nspc(env->gwion->mp, t->name);
  const m_uint scope = env_push_type(env, t);
  ID_List list = edef->list;
  for(uint32_t i = 0; i < list->len; i++) {
    Symbol xid = *mp_vector_at(list, Symbol, i);
    const Value v = new_value(env, t, s_name(xid), edef->pos);
    valuefrom(env, v->from);
    nspc_add_value(env->curr, xid, v);
    if (env->class_def) {
      SET_FLAG(v, static);
      SET_ACCESS(edef, v)
      SET_ACCESS(edef, t)
    } else
      set_vflag(v, vflag_builtin);
    SET_FLAG(v, const);
    vector_add(&edef->values, (vtype)v);
  }
  env_pop(env, scope);
  return GW_OK;
}

ANN static Value arg_value(const Env env, Arg *const arg) {
  const Var_Decl *vd = &arg->var_decl;
  const Value    v   = new_value(env, arg->type,
                            vd->xid ? s_name(vd->xid) : (m_str) __func__, arg->var_decl.pos);
  if (arg->td)
    v->flag = arg->td->flag;
  return v;
}

ANN static m_bool scan1_args(const Env env, Arg_List args) {
  for(uint32_t i = 0; i < args->len; i++) {
    Arg *arg = mp_vector_at(args, Arg, i);
    Var_Decl *const vd = &arg->var_decl;
    if (vd->xid) CHECK_BB(isres(env, vd->xid, vd->pos));
    if (arg->td) {
      SET_FLAG(arg->td, late);
      CHECK_OB((arg->type = void_type(env, arg->td)));
      if (GET_FLAG(env->func->def->base, global) && !type_global(env, arg->type))
        ERR_B(arg->td->pos, "is not global");
      UNSET_FLAG(arg->td, late);
    }
    vd->value = arg_value(env, arg);
    if (vd->xid) nspc_add_value(env->curr, vd->xid, vd->value);
  }
  return GW_OK;
}

ANN static Type scan1_noret(const Env env, const Func_Base *base) {
  assert(base->td);
  DECL_OO(const Type, t, = known_type(env, base->td));
  if (!tflag(t, tflag_noret)) return t;
  ERR_O(base->pos, _("Can't use type `{+G}%s{0}` for return"), t->name);
}

ANN static m_bool _scan1_fbase_tmpl(const Env env, Func_Base *base) {
  Specialized_List sl = base->tmpl->list;
  for(uint32_t i = 0; i < sl->len; i++) {
    Specialized *spec = mp_vector_at(sl, Specialized, i);
    nspc_add_type(env->curr, spec->xid, env->gwion->type[et_auto]);
  }
  CHECK_OB((base->ret_type = scan1_noret(env, base)));
  return GW_OK;
}

ANN static m_bool scan1_fbase_tmpl(const Env env, Func_Base *const base) {
  nspc_push_type(env->gwion->mp, env->curr);
  const m_bool ret = _scan1_fbase_tmpl(env, base);
  nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}

ANN static m_bool scan1_fdef_base_tmpl(const Env env, const Func_Def fdef) {
  Func_Base *const base = fdef->base;
  if (!fbflag(base, fbflag_op)) return scan1_fbase_tmpl(env, base);
  Arg_List         args = fdef->base->args;
  Specialized_List sl   = fdef->base->tmpl->list;
  uint32_t j = 0;
  for(uint32_t i = 0; i < args->len; i++) {
    Arg *arg = mp_vector_at(args, Arg, i);
    Specialized *spec = mp_vector_at(sl, Specialized, j);
    if (!arg->td->next && spec && arg->td->xid == spec->xid) { j++; }
  }
  if (j < sl->len) ERR_B(base->pos, "too many template types for operator");
  const Vector v = &env->curr->info->op_tmpl;
  if (!v->ptr) vector_init(v);
  vector_add(v, (m_uint)cpy_func_def(env->gwion->mp, fdef));
  return GW_OK;
}

ANN m_bool scan1_fptr_def(const Env env, const Fptr_Def fptr) {
  const bool global = GET_FLAG(fptr->cdef, global);
  if(global) env_push_global(env);
  const m_bool ret = scan1_class_def(env, fptr->cdef);
  if(global) env_pop(env, 0);
  return ret;
}

ANN m_bool scan1_type_def(const Env env, const Type_Def tdef) {
  if (tdef->when) CHECK_BB(scan1_exp(env, tdef->when));
  if(tflag(tdef->type->info->parent, tflag_ref))
    ERR_B(tdef->pos, "can't typedef a reference type");
  if (tflag(tdef->type, tflag_cdef))
    return scan1_class_def(env, tdef->type->info->cdef);
  return tdef->type->info->cdef ? scan1_cdef(env, tdef->type) : GW_OK;
}

ANN static inline m_bool scan1_union_def_inner_loop(const Env env,
                                                    Union_Def udef) {
  nspc_allocdata(env->gwion->mp, udef->type->nspc);
  Union_List  l  = udef->l;
  m_uint      sz = 0;
  const Value v = new_value(env, env->gwion->type[et_int], "@index", udef->pos);
  nspc_add_value_front(env->curr, insert_symbol("@index"), v);
  valuefrom(env, v->from);
  for(uint32_t i = 0; i < l->len; i++) {
    Union_Member *um = mp_vector_at(l, Union_Member, i);
    DECL_OB(const Type, t, = known_type(env, um->td));
    if (nspc_lookup_value0(env->curr, um->vd.xid))
      ERR_B(um->vd.pos, _("'%s' already declared in union"), s_name(um->vd.xid))
    if(tflag(t, tflag_ref))
      ERR_B(um->vd.pos, _("can't declare ref type in union"));
    const Value v = new_value(env, t, s_name(um->vd.xid), um->vd.pos);
    tuple_contains(env, v);
    valuefrom(env, v->from);
    nspc_add_value_front(env->curr, um->vd.xid, v);
    if (t->size > sz) sz = t->size;
  }
  udef->type->nspc->offset = SZ_INT + sz;
  return GW_OK;
}

ANN static m_bool scan1_union_def_inner(const Env env, const Union_Def udef) {
  if (udef->tmpl && udef->tmpl->call)
    CHECK_BB(template_push_types(env, udef->tmpl));
  const m_bool ret = scan1_union_def_inner_loop(env, udef);
  if (udef->tmpl && udef->tmpl->call) nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}

ANN m_bool scan1_union_def(const Env env, const Union_Def udef) {
  if (tmpl_base(udef->tmpl)) return GW_OK;
  const m_uint scope = env_push_type(env, udef->type);
  const m_bool ret   = scan1_union_def_inner(env, udef);
  env_pop(env, scope);
  set_tflag(udef->type, tflag_scan1);
  return ret;
}

#define scan1_stmt_while    scan1_stmt_flow
#define scan1_stmt_until    scan1_stmt_flow
#define scan1_stmt_continue dummy_func
#define scan1_stmt_break    dummy_func
#define scan1_stmt_retry    dummy_func

ANN static m_bool scan1_stmt_return(const Env env, const Stmt_Exp stmt) {
  if (!env->func)
    ERR_B(stmt_self(stmt)->pos,
          _("'return' statement found outside function definition"))
  if (env->scope->depth == 1) env->func->memoize = 1;
  if(stmt->val) CHECK_BB(scan1_exp(env, stmt->val));
  return GW_OK;
}

ANN static m_bool scan1_stmt_pp(const Env env, const Stmt_PP stmt) {
  if (stmt->pp_type == ae_pp_include) env->name = stmt->data;
  if (stmt->pp_type == ae_pp_pragma && !strcmp(stmt->data, "packed")) {
    if(env->class_def && !tflag(env->class_def, tflag_union)) set_tflag(env->class_def, tflag_packed);
    else ERR_B(stmt_self(stmt)->pos, "`packed` pragma outside of {G+}class{0} or {G+}struct{0} declaration");
  }
  return GW_OK;
}

ANN static m_bool scan1_stmt_defer(const Env env, const Stmt_Defer stmt) {
  return scan1_stmt(env, stmt->stmt);
}

ANN static m_bool scan1_stmt_spread(const Env env, const Spread_Def spread) {
  ERR_B(stmt_self(spread)->pos, "spread statement outside of variadic environment");
}

DECL_STMT_FUNC(scan1, m_bool, Env)

ANN static inline m_bool scan1_stmt(const Env env, const Stmt stmt) {
  return scan1_stmt_func[stmt->stmt_type](env, &stmt->d);
}

ANN static inline bool end_flow(Stmt s) {
  const ae_stmt_t t = s->stmt_type;
    return t == ae_stmt_continue ||
           t == ae_stmt_break    ||
           t == ae_stmt_return;
}

ANN static void dead_code(const Env env, Stmt_List l, uint32_t len) {
  for(uint32_t i = len; i < l->len; i++) {
    const Stmt s = mp_vector_at(l, struct Stmt_, i);
    free_stmt(env->gwion->mp, s);
  }
  l->len = len;
}

ANN static m_bool scan1_stmt_list(const Env env, Stmt_List l) {
  uint32_t i;
  for(i = 0; i < l->len; i++) {
    const Stmt s = mp_vector_at(l, struct Stmt_, i);
    CHECK_BB(scan1_stmt(env, s));
    if(end_flow(s)) break;
  }
  if(++i < l->len) dead_code(env, l, i);
  return GW_OK;
}

ANN static m_bool class_internal(const Env env, const Func_Base *base) {
  assert(base->td);
  if (!env->class_def)
    ERR_B(base->td->pos, _("'%s' must be in class def!!"), s_name(base->xid))
  if (base->args)
    ERR_B(base->td->pos, _("'%s' must not have args"), s_name(base->xid))
  if (base->ret_type != env->gwion->type[et_void])
    ERR_B(base->td->pos, _("'%s' must return 'void'"), s_name(base->xid))
  return GW_OK;
}

ANN static inline m_bool scan_internal_arg(const Env        env,
                                           const Func_Base *base) {
  if (mp_vector_len(base->args) == 1) return GW_OK;
  assert(base->td);
  ERR_B(base->td->pos, _("'%s' must have one (and only one) argument"),
        s_name(base->xid))
}

ANN static inline m_bool scan_internal_int(const Env        env,
                                           const Func_Base *base) {
  assert(base->td);
  CHECK_BB(scan_internal_arg(env, base));
  if (isa(base->ret_type, env->gwion->type[et_int]) > 0) return GW_OK;
  ERR_B(base->td->pos, _("'%s' must return 'int'"), s_name(base->xid))
}

ANN static m_bool scan_internal(const Env env, const Func_Base *base) {
  const Symbol op = base->xid;
  if (op == insert_symbol("@dtor")) {
    if(safe_tflag(env->class_def, tflag_struct))
      ERR_B(base->pos, "can't use '@dtor' for structures");
    return class_internal(env, base);
  }
  if (op == insert_symbol("@gack"))
    return class_internal(env, base);
  if (op == insert_symbol("@implicit")) return scan_internal_arg(env, base);
  if (op == insert_symbol("@conditional") ||
      op == insert_symbol("@unconditional"))
    return scan_internal_int(env, base);
  if(op == insert_symbol("@array_init") ||
     op == insert_symbol("@each")       ||
     op == insert_symbol("@each_idx")   ||
     op == insert_symbol("@each_init")  ||
     op == insert_symbol("@each_val")   ||
     op == insert_symbol("@partial"))
      ERR_B(base->pos, "operator '%s' not allowed", s_name(op));
  return GW_OK;
}

ANN static m_bool scan1_fdef_args(const Env env, Arg_List args) {
  for(uint32_t i = 0; i < args->len; i++) {
    Arg *arg = mp_vector_at(args, Arg, i);
    CHECK_BB(shadow_arg(env, arg->var_decl.xid, arg->var_decl.pos));
  }
  return GW_OK;
}

ANN m_bool scan1_fbody(const Env env, const Func_Def fdef) {
  if (fdef->base->args) {
    CHECK_BB(scan1_fdef_args(env, fdef->base->args));
    CHECK_BB(scan1_args(env, fdef->base->args));
  }
  if (!fdef->builtin && fdef->d.code)
    CHECK_BB(scan1_stmt_list(env, fdef->d.code));
  return GW_OK;
}

ANN m_bool scan1_fdef(const Env env, const Func_Def fdef) {
  if (fdef->base->td)
    CHECK_OB((fdef->base->ret_type = scan1_noret(env, fdef->base)));
  if (fbflag(fdef->base, fbflag_internal))
    CHECK_BB(scan_internal(env, fdef->base));
  else if (fbflag(fdef->base, fbflag_op) && env->class_def)
    SET_FLAG(fdef->base, static);
  if(!is_ctor(fdef)) {
    RET_NSPC(scan1_fbody(env, fdef))
  } else if(!fdef->builtin)
      CHECK_BB(scan1_stmt_list(env, fdef->d.code));
  return GW_OK;
}

ANN static inline m_bool scan1_fdef_defined(const Env      env,
                                            const Func_Def fdef) {
  const Value v = nspc_lookup_value1(env->curr, fdef->base->xid);
  if (!v) return GW_OK;
  if (is_func(env->gwion, actual_type(env->gwion, v->type))) return GW_OK; // is_callable
  if(fdef->builtin) return GW_OK;
  if ((!env->class_def || !GET_FLAG(env->class_def, final)) &&
      !nspc_lookup_value0(env->curr, fdef->base->xid))
    ERR_B(fdef->base->pos,
          _("function '%s' has already been defined in the same scope..."),
          s_name(fdef->base->xid))
  return GW_OK;
}

ANN static m_bool _scan1_func_def(const Env env, const Func_Def fdef) {
  if(GET_FLAG(fdef->base, abstract) && !env->class_def)
    ERR_B(fdef->base->pos, "file scope function can't be abstract");
  CHECK_BB(env_storage(env, fdef->base->flag, fdef->base->pos));
  CHECK_BB(scan1_fdef_defined(env, fdef));
  const bool   global = GET_FLAG(fdef->base, global);
  const m_uint scope  = !global ? env->scope->depth : env_push_global(env);
  if (tmpl_base(fdef->base->tmpl)) return scan1_fdef_base_tmpl(env, fdef);
  struct Func_ fake = {.name = s_name(fdef->base->xid), .def = fdef }, *const former =
                                                             env->func;
  env->func = &fake;
  const m_bool ret = scanx_fdef(env, env, fdef, (_exp_func)scan1_fdef);
  env->func = former;
  if (global) env_pop(env, scope);
  if ((strcmp(s_name(fdef->base->xid), "@implicit") || fbflag(fdef->base, fbflag_internal)) && !fdef->builtin && fdef->base->ret_type &&
       fdef->base->ret_type != env->gwion->type[et_void] && fdef->d.code &&
       !fake.memoize)
     ERR_B(fdef->base->td->pos,
           _("missing return statement in a non void function"));
  if (fdef->base->xid == insert_symbol("@gack") && !fake.weight) {
    gwerr_basic(_("`@gack` operator does not print anything"), NULL,
      _("use `<<<` `>>>` in the function"), env->name, fdef->base->pos, 0);
    env_set_error(env,  true);
    return GW_ERROR;
  }
  return ret;
}

ANN m_bool scan1_func_def(const Env env, const Func_Def fdef) {
  const uint16_t depth = env->scope->depth;
  env->scope->depth = 0;
  const m_bool ret = _scan1_func_def(env, fdef);
  env->scope->depth = depth;
  return ret;
}

#define scan1_trait_def dummy_func
#define scan1_extend_def dummy_func
#define scan1_prim_def dummy_func
HANDLE_SECTION_FUNC(scan1, m_bool, Env)

ANN static Type scan1_get_parent(const Env env, const Type_Def tdef) {
  const Type parent = tdef->type->info->parent;
  Type t = parent;
  do
    if (tdef->type == t)
      ERR_O(tdef->ext->pos, _("recursive (%s <= %s) class declaration."),
            tdef->type->name, t->name)
  while ((t = t->info->parent));
  return parent;
}

ANN static m_bool scan1_parent(const Env env, const Class_Def cdef) {
  const loc_t pos = cdef->base.ext->pos;
  if (cdef->base.ext->array && cdef->base.ext->array->exp)
    CHECK_BB(scan1_exp(env, cdef->base.ext->array->exp));
  DECL_OB(const Type, parent, = scan1_get_parent(env, &cdef->base));
  if (isa(parent, env->gwion->type[et_object]) < 0 &&
      !(tflag(cdef->base.type, tflag_cdef) || tflag(cdef->base.type, tflag_udef)))
    ERR_B(pos, _("cannot extend primitive type '%s'"), parent->name)
  if (type_ref(parent)) ERR_B(pos, _("can't use ref type in class extend"))
  if (GET_FLAG(parent, abstract)) SET_FLAG(cdef->base.type, abstract);
  return GW_OK;
}

ANN static inline Type scan1_final(const Env env, Type_Decl *td, const bool tdef) {
  DECL_OO(const Type, t, = known_type(env, td));
  if (!GET_FLAG(t, final) || tdef) return t;
  ERR_O(td->pos, _("can't inherit from final parent class '%s'\n."), t->name);
}

ANN static m_bool cdef_parent(const Env env, const Class_Def cdef) {
  CHECK_OB((cdef->base.type->info->parent = scan1_final(env, cdef->base.ext, tflag(cdef->base.type, tflag_typedef))));
  const bool tmpl = !!cdef->base.tmpl;
  if (tmpl) CHECK_BB(template_push_types(env, cdef->base.tmpl));
  const m_bool ret = scan1_parent(env, cdef);
  if (tmpl) nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}

ANN static m_bool scan1_class_def_body(const Env env, const Class_Def cdef) {
  if(!tmpl_base(cdef->base.tmpl) && isa(cdef->base.type, env->gwion->type[et_closure]) < 0 &&
   isa(cdef->base.type, env->gwion->type[et_dict]) < 0) {
    MemPool mp = env->gwion->mp;
    Ast base = cdef->body;
    Stmt_List ctor = new_mp_vector(mp, struct Stmt_, 0);
    Ast body = new_mp_vector(mp, Section, 1); // room for ctor
    for(uint32_t i = 0; i < base->len; i++) {
      Section section = *mp_vector_at(base, Section, i);
      if(section.section_type == ae_section_stmt) {
        Stmt_List list = section.d.stmt_list;
        for(uint32_t j = 0; j < list->len; j++) {
          Stmt stmt = mp_vector_at(list, struct Stmt_, j);
          mp_vector_add(mp, &ctor, struct Stmt_, *stmt);
        }
      } else mp_vector_add(mp, &body, Section, section);
    }
    Type_Decl *td = type2td(env->gwion, env->gwion->type[et_void], cdef->base.pos);
    Symbol sym = insert_symbol("@ctor");
    Func_Base *fb = new_func_base(mp, td, sym, NULL, ae_flag_none, cdef->base.pos);
    Func_Def  fdef = new_func_def(mp, fb, ctor);
    mp_vector_set(body, Section, 0, MK_SECTION(func, func_def, fdef));
    free_mp_vector(mp, Section, base);
    cdef->body = body;
  }
  return env_body(env, cdef, scan1_section);
}

ANN m_bool scan1_class_def(const Env env, const Class_Def cdef) {
  if (tmpl_base(cdef->base.tmpl)) return GW_OK;
  const Type      t = cdef->base.type;
  if (tflag(t, tflag_scan1)) return GW_OK;
  set_tflag(t, tflag_scan1);
  const Class_Def c = t->info->cdef;
  if (c->base.ext) CHECK_BB(cdef_parent(env, c));
  if (c->body) CHECK_BB(scan1_class_def_body(env, c));
  return GW_OK;
}

ANN m_bool scan1_ast(const Env env, Ast *ast) {
  Ast a = *ast;
  for(m_uint i = 0; i < a->len; i++) {
    Section *section = mp_vector_at(a, Section, i);
    CHECK_BB(scan1_section(env, section));
  }
  return GW_OK;
}
