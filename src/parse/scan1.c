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

ANN static bool scan1_stmt_list(const Env, StmtList *);
ANN static bool scan1_stmt(const Env, Stmt*);

ANN static inline bool type_cyclic(const Env env, const Type t,
                                     const Type_Decl *td) {
  Type owner = env->class_def;
  do {
    Type parent = t;
    do {
      if (parent == owner)
        ERR_B(td->tag.loc, _("%s declared inside %s"), t->name, owner->name);
    } while ((parent = parent->info->parent));
  } while ((owner = owner->info->value->from->owner_class));
  return true;
}

ANN static inline bool ensure_scan1(const Env env, const Type t) {
  if (tflag(t, tflag_scan1) || !(tflag(t, tflag_cdef) || tflag(t, tflag_union)))
    return true;
  struct EnvSet es = {.env   = env,
                      .data  = env,
                      .func  = (_envset_func)scan1_cdef,
                      .scope = env->scope->depth,
                      .flag  = tflag_scan1};
  return envset_run(&es, t);
}

ANN static bool check_global(const Env env, const Type t, const loc_t loc) {
  const ValueFrom *from = t->info->value->from;
  if(from->owner_class && isa(from->owner_class, env->class_def))
    return true;
  if(from_global_nspc(env, from->owner) ||
    (from->owner_class && type_global(env, from->owner_class)))
      return true;
  gwlog_error("can't use non-global type in a global class", NULL, env->name, loc, 0);
  gwlog_related_from("not declared global", from);
  const ValueFrom *ownerFrom = env->class_def->info->value->from;
  gwlog_related_from("is global", ownerFrom);
  env_set_error(env, true);
  return false;
}

ANN static Type scan1_type(const Env env, Type_Decl *td) {
  DECL_O(const Type, t, = known_type(env, td));
  const Type base = array_base(t);
  if (!env->func && env->class_def && !GET_FLAG(td, late))
    CHECK_O(type_cyclic(env, base, td));
  CHECK_O(ensure_scan1(env, t));
  return t;
}

ANN static Type void_type(const Env env, Type_Decl *td) {
  DECL_O(const Type, type, = scan1_type(env, td));
  if (type->size) return type;
  ERR_O(td->tag.loc, _("cannot declare variables of size '0' (i.e. 'void')..."));
}

ANN static Type scan1_exp_decl_type(const Env env, Exp_Decl *decl) {
  if (decl->type) return decl->type;
  DECL_O(const Type, t, = void_type(env, decl->var.td));
  if(env->class_def && type_global(env, env->class_def) && !check_global(env, t, decl->var.td->tag.loc))
      return NULL;
  if (decl->var.td->tag.sym == insert_symbol("auto") && decl->type) return decl->type;
  if (GET_FLAG(t, private) && t->info->value->from->owner != env->curr)
    ERR_O(exp_self(decl)->loc, _("can't use private type %s"), t->name);
  if (GET_FLAG(t, protect) && (!env->class_def || !isa(t, env->class_def)))
    ERR_O(exp_self(decl)->loc, _("can't use protected type %s"), t->name);
  return t;
}

static inline bool scan1_defined(const Env env, const Var_Decl *var) {
  if (var->value) // from an auto declaration
    return true;
  const Value v = ((!env->class_def || !GET_FLAG(env->class_def, final) ||
        env->scope->depth)
           ? nspc_lookup_value1
           : nspc_lookup_value2)(env->curr, var->tag.sym);
  if(v && (!v->from->owner_class || isa(env->class_def, v->from->owner_class))) {
    env_err(env, var->tag.loc,
          _("variable %s has already been defined in the same scope..."),
          s_name(var->tag.sym));
    defined_here(v);
    return false;
  }
  return true;
}

ANN bool abstract_array(const Env env, const Array_Sub array) {
  Exp* e = array->exp;
  bool ok = true;
  while(e) {
    if(!exp_is_zero(e))
      ERR_OK_NODE(ok, e, e->loc, _("arrays of abstract type should use `0` size"));
    e = e->next;
  }
  return ok;
}

ANN static bool scan1_decl(const Env env, Exp_Decl *const decl) {
  const bool decl_ref = decl->var.td->array && !decl->var.td->array->exp;
  Var_Decl *const vd = &decl->var.vd;
  CHECK_B(not_reserved(env, vd->tag));
  Type t = decl->type;
  CHECK_B(scan1_defined(env, vd));
  const Type base = array_base_simple(t);
  if(decl->var.td->array) {
    if (!GET_FLAG(decl->var.td, late) && !decl->var.td->array->exp)
      ERR_B(decl->var.td->tag.loc, _("arrays with no expressions should be declared `late`"));
    if (GET_FLAG(decl->var.td, late) && decl->var.td->array->exp)
      ERR_B(decl->var.td->array->exp->loc, _("late array should have no size"));
    if (!decl->args && GET_FLAG(base, abstract)) CHECK_B(abstract_array(env, decl->var.td->array));
  }
  const Value v = vd->value =
      vd->value ?: new_value(env, t, vd->tag);
  if (GET_FLAG(t, abstract) && !decl->args && !GET_FLAG(decl->var.td, late)) SET_FLAG(v, late);
  v->type = t;
  if (decl_ref) SET_FLAG(v, late);
  v->flag |= decl->var.td->flag;
  v->from->ctx = env->context;
  if (!env->scope->depth) {
    valuefrom(env, v->from);
    if (env->class_def) {
      if (env->class_def->info->tuple) tuple_contains(env, v);
      if (!GET_FLAG(decl->var.td, static)) {
        set_vflag(v, vflag_member);
        if(tflag(t, tflag_release))
          set_tflag(env->class_def, tflag_release);
        if(is_object(env->gwion, t))
          set_vflag(v, vflag_release);
        if (tflag(env->class_def, tflag_struct)) {
          v->from->offset = env->class_def->size;
          env->class_def->size += t->size;
        }
      }
    } else if (GET_FLAG(decl->var.td, global))
      SET_FLAG(v, global);
    else if(env->context)
    set_vflag(v, vflag_fglobal); // file global
  } else if (GET_FLAG(decl->var.td, global)) // wait we can have globals in a scope ?
    SET_FLAG(v, global);
  nspc_add_value(env->curr, vd->tag.sym, v);
  ((Exp_Decl *)decl)->type = decl->var.vd.value->type;
  return true;
}

ANN bool scan1_exp_decl(const Env env, Exp_Decl *const decl) {
  CHECK_B(env_storage(env, decl->var.td->flag, exp_self(decl)->loc));
  ((Exp_Decl *)decl)->type = scan1_exp_decl_type(env, (Exp_Decl *)decl);
  CHECK_B(decl->type);
  if(decl->args) CHECK_B(scan1_exp(env, decl->args));
  const bool global = GET_FLAG(decl->var.td, global);
  if (global) {
    if (env->context) env->context->global = true;
    if (!type_global(env, decl->type))
      ERR_B(exp_self(decl)->loc, _("type '%s' is not global"), decl->type->name);
  }
  const m_uint scope = !global ? env->scope->depth : env_push_global(env);
  const bool ret   = scan1_decl(env, decl);
  if (global) env_pop(env, scope);
  return ret;
}

ANN static inline bool scan1_exp_binary(const Env         env,
                                          const Exp_Binary *bin) {
  CHECK_B(scan1_exp(env, bin->lhs));
  return scan1_exp(env, bin->rhs);
}

ANN static bool scan1_range(const Env env, Range *range) {
  if (range->start) CHECK_B(scan1_exp(env, range->start));
  if (range->end) CHECK_B(scan1_exp(env, range->end));
  return true;
}

ANN static inline bool scan1_prim(const Env env, const Exp_Primary *prim) {
  if (prim->prim_type == ae_prim_dict || prim->prim_type == ae_prim_interp)
    return scan1_exp(env, prim->d.exp);
  if (prim->prim_type == ae_prim_hack) {
    if(env->func) env->func->weight = 1; // mark function has having gack
    // we should use effects when typechecking for that
    return scan1_exp(env, prim->d.exp);
  }
  if (prim->prim_type == ae_prim_array)
    return scan1_exp(env, prim->d.array->exp);
  if (prim->prim_type == ae_prim_range) return scan1_range(env, prim->d.range);
  if (env->func && prim->prim_type == ae_prim_perform && env->scope->depth <= 2)
    env->func->memoize = 1;
  return true;
}

ANN static inline bool scan1_exp_array(const Env        env,
                                         const Exp_Array *array) {
  CHECK_B(scan1_exp(env, array->base));
  return scan1_exp(env, array->array->exp);
}

ANN static inline bool scan1_exp_slice(const Env        env,
                                         const Exp_Slice *range) {
  CHECK_B(scan1_exp(env, range->base));
  return scan1_range(env, range->range);
}

ANN static inline bool scan1_exp_cast(const Env env, const Exp_Cast *cast) {
  return scan1_exp(env, cast->exp);
}

ANN static bool scan1_exp_post(const Env env, const Exp_Postfix *post) {
  CHECK_B(scan1_exp(env, post->exp));
  const m_str access = exp_access(post->exp);
  if (!access) return true;
  ERR_B(post->exp->loc,
        _("post operator '%s' cannot be used"
          " on %s data-type..."),
        s_name(post->op), access);
}

ANN static bool scan1_exp_call(const Env env, const Exp_Call *exp_call) {
//  if (exp_call->tmpl) return true;
  CHECK_B(scan1_exp(env, exp_call->func));
  Exp* args = exp_call->args;
  return args ? scan1_exp(env, args) : true;
}

ANN static inline bool scan1_exp_dot(const Env env, const Exp_Dot *member) {
  return scan1_exp(env, member->base);
}

ANN static bool scan1_exp_if(const Env env, const Exp_If *exp_if) {
  CHECK_B(scan1_exp(env, exp_if->cond));
  if(exp_if->if_exp) CHECK_B(scan1_exp(env, exp_if->if_exp));
  return scan1_exp(env, exp_if->else_exp);
}

ANN static inline bool scan1_exp_unary(const restrict Env env,
                                         Exp_Unary *const  unary) {
  if(unary->unary_type == unary_exp)
    return scan1_exp(env, unary->exp);
  if (unary->unary_type == unary_code)
    return scan1_stmt_list(env, unary->code);
  return true;
}

#define scan1_exp_lambda dummy_func
#define scan1_exp_td     dummy_func

ANN static bool scan1_exp_named(const Env env, Exp_Named *named) {
  return scan1_exp(env, named->exp);
}

HANDLE_EXP_FUNC(scan1, bool, Env,)

ANN static inline bool _scan1_stmt_match_case(const restrict Env env,
                                                const Stmt_Match   stmt) {
  CHECK_B(scan1_exp(env, stmt->cond));
  if (stmt->when) CHECK_B(scan1_exp(env, stmt->when));
  return scan1_stmt_list(env, stmt->list);
}

ANN static inline bool scan1_stmt_match_case(const restrict Env env,
                                               const Stmt_Match   stmt) {
    RET_NSPC(_scan1_stmt_match_case(env, stmt));
}

ANN static inline bool scan1_stmt_using(const restrict Env env,
                                        const Stmt_Using stmt) {
  if(stmt->tag.sym)
    CHECK_B(scan1_exp(env, stmt->d.exp));
  if(!env->curr->info->gwusing)
    env->curr->info->gwusing = new_usinglist(env->gwion->mp, 0);
  usinglist_add(env->gwion->mp, &env->curr->info->gwusing, stmt);
  return true;
}

#define scan1_stmt_import dummy_func

ANN static inline bool
    _scan1_stmt_match(const restrict Env env, const Stmt_Match stmt) {
  if (stmt->where) CHECK_B(scan1_stmt(env, stmt->where));
  StmtList *l = stmt->list;
  bool ok = true;
  for(m_uint i = 0; i < l->len; i++) {
    Stmt* stmt = stmtlist_ptr_at(l, i);
    if(stmt->poison) continue;
    if(!scan1_stmt_match_case(env, &stmt->d.stmt_match))
      POISON_NODE(ok, env, stmt);
  }
  return ok;
}

ANN static inline bool scan1_stmt_match(const restrict Env env,
                                          const Stmt_Match   stmt) {
  CHECK_B(scan1_exp(env, stmt->cond));
  RET_NSPC(_scan1_stmt_match(env, stmt))
}

ANN static inline bool scan1_handler(const restrict Env env,
                                            const Handler *handler) {
  RET_NSPC(scan1_stmt(env, handler->stmt));
}

ANN static inline bool scan1_handler_list(const restrict Env env,
                                            const HandlerList *handlers) {
  bool ok = true;
  for(uint32_t i = 0; i < handlers->len; i++) {
    const Handler handler = handlerlist_at(handlers, i);
    if(!scan1_handler(env, &handler))
      POISON(ok, env);
  }
  return ok;
}

ANN static inline bool scan1_stmt_try(const restrict Env env,
                                        const Stmt_Try     stmt) {
  CHECK_B(scan1_handler_list(env, stmt->handler));
  RET_NSPC(scan1_stmt(env, stmt->stmt))
}

ANN static inline bool stmt_each_defined(const restrict Env env,
                                           const Stmt_Each    stmt) {
  bool ok = true;
  if (nspc_lookup_value1(env->curr, stmt->var.tag.sym))
    ERR_OK_NODE(ok, stmt_self(stmt), stmt_self(stmt)->loc, _("foreach value '%s' is already defined"),
          s_name(stmt->var.tag.sym));
  if (stmt->idx.tag.sym && nspc_lookup_value1(env->curr, stmt->idx.tag.sym))
    ERR_OK_NODE(ok, stmt_self(stmt), stmt->idx.tag.loc, _("foreach index '%s' is already defined"),
          s_name(stmt->idx.tag.sym));
  return ok;
}

ANN static inline bool shadow_err(const Env env, const Value v,
                                    const loc_t loc) {
  if(env->scope->shadowing) return true;
  gwlog_error(_("shadowing a previously defined variable"), NULL,
              env->name, loc, 0);
  defined_here(v);
  env_set_error(env, true);
  return false;
}

ANN static inline bool shadow_arg(const Env env, const Tag tag) {
  Nspc nspc = env->curr;
  do {
    const Value v = nspc_lookup_value0(nspc, tag.sym);
    if (v && !env->func->def->builtin) {
      const Type owner = v->from->owner_class;
      if (owner && env->class_def && !isa(env->class_def, owner))
        continue;
      return shadow_err(env, v, tag.loc);
    }
  } while ((nspc = nspc->parent));
  return true;
}

ANN static inline bool shadow_var(const Env env, const Tag tag){
  const Value v = nspc_lookup_value1(env->curr, tag.sym);
  return !v ? true : shadow_err(env, v, tag.loc);
}

#define describe_ret_nspc(name, type, prolog, exp)                             \
  describe_stmt_func(scan1, name, type, prolog, exp)
describe_ret_nspc(flow, Stmt_Flow,, !(!scan1_exp(env, stmt->cond) ||
    !scan1_stmt(env, stmt->body)) ? true : false)
describe_ret_nspc(for, Stmt_For,, !(!scan1_stmt(env, stmt->c1) ||
    !scan1_stmt(env, stmt->c2) ||
    (stmt->c3 && !scan1_exp(env, stmt->c3)) ||
    !scan1_stmt(env, stmt->body)) ? true : false)
describe_ret_nspc(each, Stmt_Each,, !(!stmt_each_defined(env, stmt) || !scan1_exp(env, stmt->exp) ||
    !scan1_stmt(env, stmt->body)) ? true : false)
describe_ret_nspc(loop, Stmt_Loop,, !( (!stmt->idx.tag.sym ? true : !shadow_var(env, stmt->idx.tag)) ||
    !scan1_exp(env, stmt->cond) ||
    !scan1_stmt(env, stmt->body)) ? true : false)

ANN static inline bool if_stmt_is_return(Stmt* stmt) {
  if (stmt->stmt_type == ae_stmt_return) return true;
  if (stmt->stmt_type == ae_stmt_code) {
    if (stmtlist_len(stmt->d.stmt_code.stmt_list)) {
      Stmt s = stmtlist_back(stmt->d.stmt_code.stmt_list);
      if (s.stmt_type == ae_stmt_return) return true;
    }
  }
  return false;
}

ANN static inline bool _scan1_stmt_if(const Env env, const Stmt_If stmt) {
  CHECK_B(scan1_exp(env, stmt->cond));
  CHECK_B(scan1_stmt(env, stmt->if_body));
  if(stmt->else_body) {
    const bool is_ret = if_stmt_is_return(stmt->if_body);
    if(is_ret) env->scope->depth--;
    CHECK_B(scan1_stmt(env, stmt->else_body));
    if(is_ret) env->scope->depth++;
  }
  return true;
}

ANN static inline bool scan1_stmt_if(const Env env, const Stmt_If stmt) {
  RET_NSPC(_scan1_stmt_if(env, stmt));
  return true;
}

ANN static inline bool scan1_stmt_code(const Env env, const Stmt_Code stmt) {
  if (stmt->stmt_list) { RET_NSPC(scan1_stmt_list(env, stmt->stmt_list)) }
  return true;
}

ANN static inline bool scan1_stmt_exp(const Env env, const Stmt_Exp stmt) {
  return stmt->val ? scan1_exp(env, stmt->val) : 1;
}

ANN static void enum_value(const Env env, const Enum_Def edef,
                           const Tag tag, const m_int value, m_int *last) {
  const Value v = new_value(env, edef->type, tag);
  v->d.num = value;
  *last = v->d.num + 1;
  valuefrom(env, v->from);
  nspc_add_value(env->curr, tag.sym, v);
  SET_FLAG(v, static | ae_flag_const);
  SET_ACCESS(edef, v)
  SET_ACCESS(edef, edef->type)
  set_vflag(v, vflag_builtin);
}

ANN bool scan1_enum_def(const Env env, const Enum_Def edef) {
  const Type t = edef->type;
  t->nspc = new_nspc(env->gwion->mp, t->name);
  const m_uint scope = env_push_type(env, t);
  EnumValueList *list = edef->list;
  m_int last = 0;
  for(uint32_t i = 0; i < list->len; i++) {
    EnumValue ev = enumvaluelist_at(list, i);
    CHECK_B(can_define(env, ev.tag.sym, ev.tag.loc));
    const m_int value = !ev.set
      ? last
      : ev.gwint.num;
    enum_value(env, edef, ev.tag, value, &last);
  }
  env_pop(env, scope);
  return true;
}

ANN static Value arg_value(const Env env, Arg *const arg) {
  const Var_Decl *vd = &arg->var.vd;
  const Symbol sym = vd->tag.sym ?: insert_symbol((m_str)__func__);
  const Value    v   = new_value(env, arg->type, MK_TAG(sym, arg->var.vd.tag.loc));
  v->from->ctx = env->context;
  if (arg->var.td)
    v->flag = arg->var.td->flag;
  return v;
}

ANN static bool scan1_args(const Env env, ArgList *args) {
  bool ok = true;
  for(uint32_t i = 0; i < args->len; i++) {
    Arg *arg = arglist_ptr_at(args, i);
    Var_Decl *const vd = &arg->var.vd;
    if (vd->tag.sym) {
      if(!not_reserved(env, vd->tag))
        POISON(ok, env);
    }
    if (arg->var.td) {
      if(!(arg->type = void_type(env, arg->var.td))) {
        POISON(ok, env);
        continue;
      }
      if (GET_FLAG(env->func->def->base, global) && !type_global(env, arg->type))
        ERR_OK(ok, arg->var.td->tag.loc, "is not global");
    }
    vd->value = arg_value(env, arg);
    if (ok && vd->tag.sym) nspc_add_value(env->curr, vd->tag.sym, vd->value);
  }
  return ok;
}

ANN static Type scan1_noret(const Env env, const Func_Base *base) {
  assert(base->td);
  DECL_O(const Type, t, = known_type(env, base->td));
  if (!tflag(t, tflag_noret)) return t;
  ERR_O(base->tag.loc, _("Can't use type `{+G}%s{0}` for return"), t->name);
}

ANN static bool _scan1_fbase_tmpl(const Env env, Func_Base *base) {
  const SpecializedList *sl = base->tmpl->list;
  for(uint32_t i = 0; i < sl->len; i++) {
    const Specialized spec = specializedlist_at(sl, i);
    nspc_add_type(env->curr, spec.tag.sym, env->gwion->type[et_auto]);
  }
  CHECK_B((base->ret_type = scan1_noret(env, base)));
  return true;
}

ANN static bool scan1_fbase_tmpl(const Env env, Func_Base *const base) {
  nspc_push_type(env->gwion->mp, env->curr);
  const bool ret = _scan1_fbase_tmpl(env, base);
  nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}

ANN static bool spec_acc_has(const MemPool mp, TagList **acc, const Tag tag) {
  for(uint32_t i = 0; i < (*acc)->len; i++) {
    const Tag t = taglist_at(*acc, i);
    if (t.sym == tag.sym) return true;
  }
  taglist_add(mp, acc, tag);
  return false;
}

ANN static bool find_op_template_type(const MemPool mp, const Symbol xid, const SpecializedList *sl, TagList **acc) {
  for(uint32_t i = 0; i < sl->len; i++) {
    const Specialized spec = specializedlist_at(sl, i);
    if (xid == spec.tag.sym) {
      spec_acc_has(mp, acc, spec.tag);
      return true;
    }
  }
  return false;
}

ANN static bool scan1_fdef_base_tmpl(const Env env, const Func_Def fdef) {
  Func_Base *const base = fdef->base;
  if (!fbflag(base, fbflag_op)) return scan1_fbase_tmpl(env, base);
  ArgList         *args = fdef->base->args;
  const SpecializedList *sl   = fdef->base->tmpl->list;
  TagList *acc = new_taglist(env->gwion->mp, 0);
  for(uint32_t i = 0; i < args->len; i++) {
    const Arg arg = arglist_at(args, i);
    if (!arg.var.td->next) {
      find_op_template_type(env->gwion->mp, arg.var.td->tag.sym, sl, &acc);
    }
  }
  const uint32_t len = acc->len;
  free_taglist(env->gwion->mp, acc);
  if (len < sl->len) ERR_B(base->tag.loc, "too many template types for operator");
  if (!env->curr->operators)
    env->curr->operators = mp_calloc(env->gwion->mp, NspcOp);
  const Vector v = &env->curr->operators->tmpl;
  if (!v->ptr) vector_init(v);
  vector_add(v, (m_uint)cpy_func_def(env->gwion->mp, fdef));
  return true;
}

ANN bool scan1_fptr_def(const Env env, const Fptr_Def fptr) {
  const bool global = GET_FLAG(fptr->cdef, global);
  if(global) env_push_global(env);
  const bool ret = scan1_class_def(env, fptr->cdef);
  if(global) env_pop(env, 0);
  return ret;
}

ANN bool scan1_type_def(const Env env, const Type_Def tdef) {
  if (tdef->when) CHECK_B(scan1_exp(env, tdef->when));
  if(tflag(tdef->type->info->parent, tflag_ref))
    ERR_B(tdef->tag.loc, "can't typedef a reference type");
  if (tflag(tdef->type, tflag_cdef))
    return scan1_class_def(env, tdef->type->info->cdef);
  return tdef->type->info->cdef ? scan1_cdef(env, tdef->type) : true;
}

ANN static inline bool scan1_union_def_inner_loop(const Env env,
                                                    Union_Def udef) {
  m_uint      sz = 0;
  bool ok = true;
  VariableList *l  = udef->l;
  for(uint32_t i = 0; i < l->len; i++) {
    const Variable um = variablelist_at(l, i);
    if (nspc_lookup_value0(env->curr, um.vd.tag.sym)) {
      // TODO: use already_declared
      ERR_OK(ok, um.vd.tag.loc, _("'%s' already declared in union"), s_name(um.vd.tag.sym));
      continue;
    }
    const Type t = known_type(env, um.td);
    if(t) {
      if(tflag(t, tflag_ref)) {
        ERR_OK(ok, um.vd.tag.loc, _("can't declare ref type in union"));
        continue;
      }
      const Value v = new_value(env, t, um.vd.tag);
      tuple_contains(env, v);
      valuefrom(env, v->from);
      nspc_add_value_front(env->curr, um.vd.tag.sym, v);
      if (t->size > sz) sz = t->size;
    } else POISON(ok, env);
  }
  udef->type->nspc->offset = SZ_INT + sz;
  udef->type->size = SZ_INT + sz;
  return ok;
}

ANN static bool scan1_union_def_inner(const Env env, const Union_Def udef) {
  if (udef->tmpl && udef->tmpl->call)
    CHECK_B(template_push_types(env, udef->tmpl));
  const bool ret = scan1_union_def_inner_loop(env, udef);
  if (udef->tmpl && udef->tmpl->call) nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}

ANN bool scan1_union_def(const Env env, const Union_Def udef) {
  if (tmpl_base(udef->tmpl)) return true;
  const m_uint scope = env_push_type(env, udef->type);
  const bool ret   = scan1_union_def_inner(env, udef);
  env_pop(env, scope);
  set_tflag(udef->type, tflag_scan1);
  return ret;
}

#define scan1_stmt_while    scan1_stmt_flow
#define scan1_stmt_until    scan1_stmt_flow
#define scan1_stmt_continue dummy_func
#define scan1_stmt_break    dummy_func
#define scan1_stmt_retry    dummy_func

ANN static bool scan1_stmt_return(const Env env, const Stmt_Exp stmt) {
  if (env->scope->depth == 1) env->func->memoize = 1;
  if(stmt->val) CHECK_B(scan1_exp(env, stmt->val));
  return true;
}

ANN static bool scan1_stmt_pp(const Env env, const Stmt_PP stmt) {
  if (stmt->pp_type == ae_pp_include)
    env->name = stmt->data;
  if (stmt->pp_type == ae_pp_pragma && !strcmp(stmt->data, "packed")) {
    if(env->class_def && !tflag(env->class_def, tflag_union)) set_tflag(env->class_def, tflag_packed);
    else ERR_B(stmt_self(stmt)->loc, "`packed` pragma outside of {G+}class{0} or {G+}struct{0} declaration");
  }
  return true;
}

ANN static bool scan1_stmt_defer(const Env env, const Stmt_Defer stmt) {
  return scan1_stmt(env, stmt->stmt);
}

// TODO: get rid of this function
ANN static bool scan1_stmt_spread(const Env env, const Spread_Def spread) {
  ERR_B(stmt_self(spread)->loc, "spread statement outside of variadic environment");
}

DECL_STMT_FUNC(scan1, bool, Env,)

ANN static inline bool scan1_stmt(const Env env, Stmt* stmt) {
  return scan1_stmt_func[stmt->stmt_type](env, &stmt->d);
}

ANN static inline bool end_flow(Stmt* s) {
  const ae_stmt_t t = s->stmt_type;
  return t == ae_stmt_continue ||
         t == ae_stmt_break    ||
         t == ae_stmt_return;
}

ANN static void dead_code(const Env env, StmtList *l, uint32_t len) {
  for(uint32_t i = len; i < l->len; i++) {
    Stmt* s = stmtlist_ptr_at(l, i);
    free_stmt(env->gwion->mp, s);
  }
  l->len = len;
}

ANN static bool scan1_stmt_list(const Env env, StmtList *l) {
  uint32_t i;
  bool ok = true;
  const uint32_t nusing = usinglist_len(env->curr->info->gwusing);
  for(i = 0; i < l->len; i++) {
    Stmt* stmt = stmtlist_ptr_at(l, i);
    if(stmt->poison) { ok = false; continue;}
    if(!scan1_stmt(env, stmt)) {
      POISON_NODE(ok, env, stmt);
      continue;
    }
    if(end_flow(stmt)) break;
  }
  if(env->curr->info->gwusing)
    env->curr->info->gwusing->len = nusing;
  if(++i < l->len) dead_code(env, l, i);
  return ok;
}

ANN static bool class_internal(const Env env, const Func_Base *base) {
  assert(base->td);
  if (!env->class_def)
    ERR_B(base->td->tag.loc, _("'%s' must be in class def!!"), s_name(base->tag.sym));
  if (base->args)
    ERR_B(base->td->tag.loc, _("'%s' must not have args"), s_name(base->tag.sym));
  if (base->ret_type != env->gwion->type[et_void])
    ERR_B(base->td->tag.loc, _("'%s' must return 'void'"), s_name(base->tag.sym));
  return true;
}

ANN static inline bool scan_internal_arg(const Env        env,
                                           const Func_Base *base) {
  if (arglist_len(base->args) == 1) return true;
  assert(base->td);
  ERR_B(base->td->tag.loc, _("'%s' must have one (and only one) argument"),
        s_name(base->tag.sym));
}

ANN static inline bool scan_internal_int(const Env        env,
                                           const Func_Base *base) {
  assert(base->td);
  CHECK_B(scan_internal_arg(env, base));
  if (isa(base->ret_type, env->gwion->type[et_int])) return true;
  ERR_B(base->td->tag.loc, _("'%s' must return 'int'"), s_name(base->tag.sym));
}

ANN static bool scan_internal(const Env env, const Func_Base *base) {
  const Symbol op = base->tag.sym;
  if (op == insert_symbol("@dtor")) {
    if(safe_tflag(env->class_def, tflag_struct))
      ERR_B(base->tag.loc, "can't use '@dtor' for structures");
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
      ERR_B(base->tag.loc, "operator '%s' not allowed", s_name(op));
  return true;
}

ANN static bool scan1_fdef_args(const Env env, ArgList *args) {
  bool ok = true;
  for(uint32_t i = 0; i < args->len; i++) {
    const Arg arg = arglist_at(args, i);
    if(!shadow_arg(env, arg.var.vd.tag))
      POISON(ok, env);
  }
  return ok;
}

ANN bool scan1_fbody(const Env env, const Func_Def fdef) {
  if (fdef->base->args) {
    CHECK_B(scan1_fdef_args(env, fdef->base->args));
    CHECK_B(scan1_args(env, fdef->base->args));
  }
  if (!fdef->builtin && fdef->d.code)
    (void)scan1_stmt_list(env, fdef->d.code);
  return true;
}

ANN bool scan1_fdef_body(const Env env, const Func_Def fdef) {
  RET_NSPC(scan1_fbody(env, fdef))
}

ANN bool scan1_fdef(const Env env, const Func_Def fdef) {
  if (fdef->base->td)
    CHECK_B((fdef->base->ret_type = scan1_noret(env, fdef->base)));
  if (fbflag(fdef->base, fbflag_internal))
    CHECK_B(scan_internal(env, fdef->base));
  else if (fbflag(fdef->base, fbflag_op) && env->class_def)
    SET_FLAG(fdef->base, static);
  if(!is_ctor(fdef)) CHECK_B(scan1_fdef_body(env, fdef));
  else if(!fdef->builtin)
      (void)scan1_stmt_list(env, fdef->d.code);
  return true;
}

ANN static inline bool scan1_fdef_defined(const Env      env,
                                            const Func_Def fdef) {
  const Value v = nspc_lookup_value1(env->curr, fdef->base->tag.sym);
  if (!v) return true;
  if (is_func(env->gwion, actual_type(env->gwion, v->type))) return true; // is_callable
  if(fdef->builtin) return true;
  if ((!env->class_def || !GET_FLAG(env->class_def, final)) &&
      !nspc_lookup_value0(env->curr, fdef->base->tag.sym))
    ERR_B(fdef->base->tag.loc,
          _("function '%s' has already been defined in the same scope..."),
          s_name(fdef->base->tag.sym));
  return true;
}

ANN static bool _scan1_func_def(const Env env, const Func_Def fdef) {
  if(GET_FLAG(fdef->base, abstract) && !env->class_def)
    ERR_B(fdef->base->tag.loc, "file scope function can't be abstract");
  CHECK_B(env_storage(env, fdef->base->flag, fdef->base->tag.loc));
  CHECK_B(scan1_fdef_defined(env, fdef));
  const bool   global = GET_FLAG(fdef->base, global);
  const m_uint scope  = !global ? env->scope->depth : env_push_global(env);
  if (tmpl_base(fdef->base->tmpl)) return scan1_fdef_base_tmpl(env, fdef);
  struct Func_ fake = {.name = s_name(fdef->base->tag.sym), .def = fdef }, *const former =
                                                             env->func;
  env->func = &fake;
  const bool ret = scanx_fdef(env, env, fdef, (_envset_func)scan1_fdef);
  env->func = former;
  if (global) env_pop(env, scope);
  if ((strcmp(s_name(fdef->base->tag.sym), "@implicit") || fbflag(fdef->base, fbflag_internal)) && !fdef->builtin && fdef->base->ret_type &&
       fdef->base->ret_type != env->gwion->type[et_void] && fdef->d.code &&
       !fake.memoize)
     ERR_B(fdef->base->td->tag.loc,
           _("missing return statement in a non void function"));
  // check gack is called.
  // there **may** be a better way using effects
  if (fdef->base->tag.sym == insert_symbol("@gack") && !fake.weight) {
    gwlog_error(_("`@gack` operator does not print anything"), NULL,
      env->name, fdef->base->tag.loc, 0);
    gwlog_hint(_("use `<<<` `>>>` in the function"), env->name, fdef->base->tag.loc);
    env_set_error(env,  true);
    return false;
  }
  return ret;
}

ANN bool scan1_func_def(const Env env, const Func_Def fdef) {
  const uint16_t depth = env->scope->depth;
  env->scope->depth = 0;
  const bool ret = _scan1_func_def(env, fdef);
  env->scope->depth = depth;
  return ret;
}

#define scan1_trait_def dummy_func
#define scan1_extend_def dummy_func
#define scan1_prim_def dummy_func
HANDLE_SECTION_FUNC(scan1, bool, Env,)

ANN static Type scan1_get_parent(const Env env, const Type_Def tdef) {
  const Type parent = tdef->type->info->parent;
  Type t = parent;
  do
    if (tdef->type == t)
      ERR_O(tdef->ext->tag.loc, _("recursive (%s <= %s) class declaration."),
            tdef->type->name, t->name);
  while ((t = t->info->parent));
  return parent;
}

ANN static bool scan1_parent(const Env env, const Class_Def cdef) {
  const loc_t loc = cdef->base.ext->tag.loc;
  if (cdef->base.ext->array && cdef->base.ext->array->exp)
    CHECK_B(scan1_exp(env, cdef->base.ext->array->exp));
  DECL_B(const Type, parent, = scan1_get_parent(env, &cdef->base));
  if (!is_object(env->gwion, parent) &&
//      !(tflag(cdef->base.type, tflag_cdef) || tflag(cdef->base.type, tflag_udef)))
      !(tflag(cdef->base.type, tflag_cdef) || tflag(cdef->base.type, tflag_union)))
    ERR_B(loc, _("cannot extend primitive type '%s'"), parent->name);
  if (type_ref(parent)) ERR_B(loc, _("can't use ref type in class extend"));
  return true;
}

ANN static inline Type scan1_final(const Env env, Type_Decl *td, const bool tdef) {
  DECL_O(const Type, t, = known_type(env, td));
  if (!GET_FLAG(t, final) || tdef) return t;
  ERR_O(td->tag.loc, _("can't inherit from final parent class '%s'\n."), t->name);
}

ANN static bool cdef_parent(const Env env, const Class_Def cdef) {
  CHECK_B((cdef->base.type->info->parent = scan1_final(env, cdef->base.ext, tflag(cdef->base.type, tflag_typedef))));
  const bool tmpl = !!cdef->base.tmpl;
  if (tmpl) CHECK_B(template_push_types(env, cdef->base.tmpl));
  const bool ret = scan1_parent(env, cdef);
  if (tmpl) nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}

ANN static bool scan1_class_def_body(const Env env, const Class_Def cdef) {
  if(!tmpl_base(cdef->base.tmpl) && !isa(cdef->base.type, env->gwion->type[et_closure]) &&
   !isa(cdef->base.type, env->gwion->type[et_dict])) {
    MemPool mp = env->gwion->mp;
    Ast base = cdef->body;
    StmtList *ctor = new_stmtlist(mp, 0);
    Ast body = new_sectionlist(mp, 1); // room for ctor
    for(uint32_t i = 0; i < base->len; i++) {
      const Section section = sectionlist_at(base, i);
      if(section.poison) continue;
      if(section.section_type == ae_section_stmt) {
        StmtList *list = section.d.stmt_list;
        for(uint32_t j = 0; j < list->len; j++) {
          Stmt stmt = stmtlist_at(list, j);
          if(stmt.poison) continue;
          stmtlist_add(mp, &ctor, stmt);
        }
      } else sectionlist_add(mp, &body, section);
    }
    Type_Decl *td = type2td(env->gwion, env->gwion->type[et_void], cdef->base.tag.loc);
    Symbol sym = insert_symbol("@ctor");
    Func_Base *fb = new_func_base(mp, td, sym, NULL, ae_flag_none, cdef->base.tag.loc);
    Func_Def  fdef = new_func_def(mp, fb, ctor);
    sectionlist_set(body, 0, MK_SECTION(func, func_def, fdef, cdef->base.tag.loc));
    free_sectionlist(mp, base);
    cdef->body = body;
  }
//  return 
// check for previous errors?
  cdef->base.type->error = !env_body(env, cdef, scan1_section);
  return true;
}

ANN static bool scan1_class_tmpl(const Env env, const Class_Def c) {
  SpecializedList *sl = c->base.tmpl->list;
  TmplArgList *tl = c->base.tmpl->call;
  env_push_type(env, c->base.type);
  bool ret = true;
  for(uint32_t i = 0; i < sl->len; i++) {
    const TmplArg targ = tmplarglist_at(tl, i);
    if (targ.type == tmplarg_td) continue;
    if(!scan1_exp(env, targ.d.exp)) {
      ret = false;
      break;
    }
  }
  env_pop(env, 0);
  return ret;
}

ANN bool scan1_class_def(const Env env, const Class_Def cdef) {
  if (tmpl_base(cdef->base.tmpl)) return true;
  const Type      t = cdef->base.type;
  if (tflag(t, tflag_scan1)) return true;
  set_tflag(t, tflag_scan1);
  const Class_Def c = t->info->cdef;
  if (c->base.ext) CHECK_B(cdef_parent(env, c));
  if (c->body) CHECK_B(scan1_class_def_body(env, c));
  if (c->base.tmpl) CHECK_B(scan1_class_tmpl(env, c));
  return true;
}

ANN bool scan1_ast(const Env env, Ast *ast) {
  Ast a = *ast;
  bool ok = true;
  for(m_uint i = 0; i < a->len; i++) {
    Section *section = sectionlist_ptr_at(a, i);
    if(section->poison) { ok = false; continue;}
    if(!scan1_section(env, section))
      POISON_SECTION(ok, env, section);
  }
  return ok;
}
