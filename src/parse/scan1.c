#include <ctype.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "template.h"
#include "parse.h"

ANN static m_bool scan1_stmt_list(const Env env, Stmt_List list);
ANN static m_bool scan1_stmt(const Env env, Stmt stmt);

ANN static inline m_bool type_cyclic(const Env env, const Type t, const Type_Decl *td) {
  Type owner = env->class_def;
  do {
    Type parent = t;
    while(parent) {
      if(parent == owner)
        ERR_B(td->pos, _("%s declared inside %s"), t->name, owner->name);
      parent = parent->info->parent;
    }
  } while((owner = owner->info->value->from->owner_class));
  return GW_OK;
}

ANN static inline m_bool ensure_scan1(const Env env, const Type t) {
  if(tflag(t, tflag_scan1) || !(tflag(t, tflag_cdef) || tflag(t, tflag_udef)))
    return GW_OK;
  struct EnvSet es = { .env=env, .data=env, .func=(_exp_func)scan1_cdef,
    .scope=env->scope->depth, .flag=tflag_scan1 };
  return envset_run(&es, t);
}

ANN static Type scan1_type(const Env env, Type_Decl* td) {
  DECL_OO(const Type, t, = known_type(env, td));
  if(!env->func && env->class_def && !GET_FLAG(td, late))
    CHECK_BO(type_cyclic(env, t, td));
  CHECK_BO(ensure_scan1(env, t));
  return t;
}

ANN static Type void_type(const Env env, Type_Decl* td) {
  DECL_OO(const Type, type, = scan1_type(env, td));
  if(type->size)
    return type;
  ERR_O(td->pos, _("cannot declare variables of size '0' (i.e. 'void')..."))
}

ANN static Type scan1_exp_decl_type(const Env env, Exp_Decl* decl) {
  if(decl->type)
    return decl->type;
  DECL_OO(const Type ,t, = void_type(env, decl->td));
  if(decl->td->xid == insert_symbol("auto") && decl->type)
    return decl->type;
  if(GET_FLAG(t, private) && t->info->value->from->owner != env->curr)
    ERR_O(exp_self(decl)->pos, _("can't use private type %s"), t->name)
  if(GET_FLAG(t, protect) && (!env->class_def || isa(t, env->class_def) < 0))
    ERR_O(exp_self(decl)->pos, _("can't use protected type %s"), t->name)
  return decl->type = t;
}

static inline m_bool scan1_defined(const Env env, const Var_Decl var) {
  if(var->value) // from an auto declaration
    return GW_OK;
  if(((!env->class_def || !GET_FLAG(env->class_def, final) || env->scope->depth) ? nspc_lookup_value1 : nspc_lookup_value2)(env->curr, var->xid))
    ERR_B(var->pos, _("variable %s has already been defined in the same scope..."),
              s_name(var->xid))
  return GW_OK;
}

static inline uint array_ref(const Array_Sub array) {
  return array && !array->exp;
}

ANN static m_bool scan1_decl(const Env env, const Exp_Decl* decl) {
  Var_Decl_List list = decl->list;
  const uint decl_ref = array_ref(decl->td->array);
  do {
    const Var_Decl var = list->self;
    CHECK_BB(isres(env, var->xid, exp_self(decl)->pos));
    Type t = decl->type;
    CHECK_BB(scan1_defined(env, var));
    if(var->array) {
      if(var->array->exp)
        CHECK_BB(scan1_exp(env, var->array->exp));
      t = array_type(env, decl->type, var->array->depth);
    }
    if(GET_FLAG(array_base(t), abstract) && ((var->array && var->array->exp)
                                                  || (decl->td->array && decl->td->array->exp)))
      ERR_B(var->pos, _("arrays of abstract type '%s' must be declared empty"),
        array_base(t)->name);
    const Value v = var->value = var->value ?: new_value(env->gwion->mp, t, s_name(var->xid));
    nspc_add_value(env->curr, var->xid, v);
    if(GET_FLAG(t, abstract) && !GET_FLAG(decl->td, late))
     SET_FLAG(v, late);
    v->type = t;
    if(decl_ref || array_ref(var->array))
      SET_FLAG(v, late);
    v->flag |= decl->td->flag;
    if(!env->scope->depth) {
      valuefrom(env, v->from, var->pos);
      if(env->class_def) {
        if(env->class_def->info->tuple)
          tuple_contains(env, v);
        if(!GET_FLAG(decl->td, static)) {
          set_vflag(v, vflag_member);
          if(tflag(env->class_def, tflag_struct)) {
            v->from->offset = env->class_def->size;
            env->class_def->size += t->size;
          }
        }
      }
      set_vflag(v, vflag_fglobal);// file global
    } else
    if(GET_FLAG(decl->td, global))
      SET_FLAG(v, global);
  } while((list = list->next));
  ((Exp_Decl*)decl)->type = decl->list->self->value->type;
  return GW_OK;
}

ANN int is_global(const Nspc nspc, Nspc global) {
  do if(nspc == global)
    return 1;
  while((global = global->parent));
  return 0;
}

ANN m_bool scan1_exp_decl(const Env env, const Exp_Decl* decl) {
  CHECK_BB(env_storage(env, decl->td->flag, exp_self(decl)->pos));
  ((Exp_Decl*)decl)->type = scan1_exp_decl_type(env, (Exp_Decl*)decl);
  CHECK_OB(decl->type);
  const bool global = GET_FLAG(decl->td, global);
  if(global) {
    if(env->context)
      env->context->global = true;
    if(!is_global(decl->type->info->value->from->owner, env->global_nspc))
      ERR_B(exp_self(decl)->pos, _("type '%s' is not global"), decl->type->name)
  }
  const m_uint scope = !global ? env->scope->depth : env_push_global(env);
  const m_bool ret = scan1_decl(env, decl);
  if(global)
    env_pop(env, scope);
  return ret;
}

ANN static inline int opiscall(const Symbol sym) {
  const m_str opname = s_name(sym);
  return (opname[0] == '@' || opname[0] == '&') &&
    (isalpha(opname[1]) || opname[1] == '_');
}

ANN static inline Exp sym2func(const Env env, const Symbol sym, const loc_t pos) {
  MemPool mp = env->gwion->mp;
  const m_str name = s_name(sym);
  return new_prim_id(mp, insert_symbol(name + 1), pos);
}

ANN static void binary_args(const Exp_Binary* bin) {
  Exp arg = bin->lhs;
  while(arg->next)
    arg = arg->next;
  arg->next = bin->rhs;
}

ANN static m_bool exp2call(const Env env, const Exp e, const Symbol sym, const Exp args) {
  e->exp_type = ae_exp_call;
  e->d.exp_call.func = sym2func(env, sym, e->pos);
  e->d.exp_call.args = args;
  e->d.exp_call.tmpl = NULL;
  return scan1_exp(env, e);
}

ANN static m_bool binary2call(const Env env, const Exp_Binary* bin) {
  binary_args(bin);
  return exp2call(env, exp_self(bin), bin->op, bin->lhs);
}

ANN static inline m_bool scan1_exp_binary(const Env env, const Exp_Binary* bin) {
  if(opiscall(bin->op))
    return binary2call(env, bin);
  CHECK_BB(scan1_exp(env, bin->lhs));
  return scan1_exp(env, bin->rhs);
}

ANN static m_bool scan1_range(const Env env, Range *range) {
  if(range->start)
    CHECK_BB(scan1_exp(env, range->start));
  if(range->end)
    CHECK_BB(scan1_exp(env, range->end));
  return GW_OK;
}

ANN static inline m_bool scan1_prim(const Env env, const Exp_Primary* prim) {
  if(prim->prim_type == ae_prim_hack || prim->prim_type == ae_prim_interp)
    return scan1_exp(env, prim->d.exp);
  if(prim->prim_type == ae_prim_array && prim->d.array->exp)
    return scan1_exp(env, prim->d.array->exp);
  if(prim->prim_type == ae_prim_range)
    return scan1_range(env, prim->d.range);
  return GW_OK;
}

ANN static inline m_bool scan1_exp_array(const Env env, const Exp_Array* array) {
  CHECK_BB(scan1_exp(env, array->base));
  return scan1_exp(env, array->array->exp);
}

ANN static inline m_bool scan1_exp_slice(const Env env, const Exp_Slice* range) {
  CHECK_BB(scan1_exp(env, range->base));
  return scan1_range(env, range->range);
}

ANN static inline m_bool scan1_exp_cast(const Env env, const Exp_Cast* cast) {
  return scan1_exp(env, cast->exp);
}

ANN static m_bool scan1_exp_post(const Env env, const Exp_Postfix* post) {
  if(opiscall(post->op)) {
    return exp2call(env, exp_self(post), post->op, post->exp);
  }
  CHECK_BB(scan1_exp(env, post->exp));
  const m_str access = exp_access(post->exp);
  if(!access)
    return GW_OK;
  ERR_B(post->exp->pos, _("post operator '%s' cannot be used"
      " on %s data-type..."), s_name(post->op), access);
}

ANN static m_bool scan1_exp_call(const Env env, const Exp_Call* exp_call) {
  if(exp_call->tmpl)
    return GW_OK;
  CHECK_BB(scan1_exp(env, exp_call->func));
  const Exp args = exp_call->args;
  return args ? scan1_exp(env, args) : GW_OK;
}

ANN static inline m_bool scan1_exp_dot(const Env env, const Exp_Dot* member) {
  return scan1_exp(env, member->base);
}

ANN static m_bool scan1_exp_if(const Env env, const Exp_If* exp_if) {
  CHECK_BB(scan1_exp(env, exp_if->cond));
  CHECK_BB(scan1_exp(env, exp_if->if_exp ?: exp_if->cond));
  return scan1_exp(env, exp_if->else_exp);
}

ANN static inline m_bool scan1_exp_unary(const restrict Env env, const Exp_Unary *unary) {
  if(unary->unary_type == unary_code)
    { RET_NSPC(scan1_stmt(env, unary->code)) }
  else if(opiscall(unary->op)) {
    return exp2call(env, exp_self(unary), unary->op, unary->exp);
  }
  return unary->unary_type == unary_exp ? scan1_exp(env, unary->exp) : GW_OK;
}

#define scan1_exp_lambda dummy_func
#define scan1_exp_td dummy_func
HANDLE_EXP_FUNC(scan1, m_bool, Env)

ANN static inline m_bool _scan1_stmt_match_case(const restrict Env env, const Stmt_Match stmt) {
  CHECK_BB(scan1_exp(env, stmt->cond));
  if(stmt->when)
    CHECK_BB(scan1_exp(env, stmt->when));
  return scan1_stmt_list(env, stmt->list);
}

ANN static inline m_bool scan1_stmt_match_case(const restrict Env env, const Stmt_Match stmt) {
  RET_NSPC(_scan1_stmt_match_case(env, stmt))
}

ANN static inline m_bool _scan1_stmt_match(const restrict Env env, const Stmt_Match stmt) {
  if(stmt->where)
    CHECK_BB(scan1_stmt(env, stmt->where));
  Stmt_List list = stmt->list;
  do CHECK_BB(scan1_stmt_match_case(env, &list->stmt->d.stmt_match));
  while((list = list->next));
  return GW_OK;
}

ANN static inline m_bool scan1_stmt_match(const restrict Env env, const Stmt_Match stmt) {
  CHECK_BB(scan1_exp(env, stmt->cond));
  RET_NSPC(_scan1_stmt_match(env, stmt))
}


ANN static inline m_bool scan1_handler_list(const restrict Env env, const Handler_List handler) {
  if(handler->next)
    CHECK_BB(scan1_handler_list(env, handler->next));
  RET_NSPC(scan1_stmt(env, handler->stmt))
}

ANN static inline m_bool scan1_stmt_try(const restrict Env env, const Stmt_Try stmt) {
  CHECK_BB(scan1_handler_list(env, stmt->handler));
  RET_NSPC(scan1_stmt(env, stmt->stmt))
}

ANN static inline m_bool stmt_each_defined(const restrict Env env, const Stmt_Each stmt) {
  if(nspc_lookup_value1(env->curr, stmt->sym))
    ERR_B(stmt_self(stmt)->pos, _("foreach value '%s' is already defined"), s_name(stmt->sym))
  if(stmt->idx && nspc_lookup_value1(env->curr, stmt->idx->sym))
    ERR_B(stmt_self(stmt)->pos, _("foreach index '%s' is already defined"), s_name(stmt->idx->sym))
  return GW_OK;
}

ANN static inline m_bool shadow_err(const Env env, const Value v, const loc_t loc) {
  gwerr_basic(_("shadowing a previously defined variable"), NULL, NULL, env->name, loc, 0);
  defined_here(v);
  env->context->error = true;
  return GW_ERROR;
}

ANN static inline m_bool shadow_arg(const Env env, const Symbol sym, const loc_t  loc) {
  Nspc nspc = env->curr;
  do {
    const Value v = nspc_lookup_value0(nspc, sym);
    if(v)
      return shadow_err(env, v, loc);
  } while((nspc = nspc->parent));
  return GW_OK;
}

ANN static inline m_bool shadow_var(const Env env, const Symbol sym, const loc_t  loc) {
  const Value v = nspc_lookup_value1(env->curr, sym);
  return !v ? GW_OK : shadow_err(env, v, loc);
}

#define describe_ret_nspc(name, type, prolog, exp) describe_stmt_func(scan1, name, type, prolog, exp)
describe_ret_nspc(flow, Stmt_Flow,, !(scan1_exp(env, stmt->cond) < 0 ||
    scan1_stmt(env, stmt->body) < 0) ? 1 : -1)
describe_ret_nspc(varloop, Stmt_VarLoop,, !(scan1_exp(env, stmt->exp) < 0 ||
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
  if(stmt->stmt_list)
    { RET_NSPC(scan1_stmt_list(env, stmt->stmt_list)) }
  return GW_OK;
}

ANN static inline m_bool scan1_stmt_exp(const Env env, const Stmt_Exp stmt) {
  return stmt->val ? scan1_exp(env, stmt->val) : 1;
}

ANN m_bool scan1_enum_def(const Env env, const Enum_Def edef) {
  ID_List list = edef->list;
  do {
    CHECK_BB(already_defined(env, list->xid, edef->pos));
    const Value v = new_value(env->gwion->mp, edef->t, s_name(list->xid));
    valuefrom(env, v->from, edef->pos);
    if(env->class_def) {
      SET_FLAG(v, static);
      SET_ACCESS(edef, v)
      SET_ACCESS(edef, edef->t)
    } else
      set_vflag(v, vflag_builtin);
    SET_FLAG(v, const);
    set_vflag(v, vflag_valid);
    nspc_add_value(edef->t->info->value->from->owner, list->xid, v);
    vector_add(&edef->values, (vtype)v);
  } while((list = list->next));
  return GW_OK;
}

ANN static Value arg_value(const Env env, const Arg_List list) {
  const Var_Decl var = list->var_decl;
  const Value v = new_value(env->gwion->mp, list->type, var->xid ? s_name(var->xid) : (m_str)__func__);
  if(var->array)
    v->type = list->type = array_type(env, list->type, var->array->depth);
  if(list->td) {
    v->flag = list->td->flag;
//    SET_FLAG(v, global); ???
  }
  return v;
}

ANN static m_bool scan1_args(const Env env, Arg_List list) {
  do {
    const Var_Decl var = list->var_decl;
    if(var->xid)
      CHECK_BB(isres(env, var->xid, var->pos));
    if(list->td) {
      SET_FLAG(list->td, late);
      CHECK_OB((list->type = void_type(env, list->td)));
      UNSET_FLAG(list->td, late);
    }
    var->value = arg_value(env, list);
    nspc_add_value(env->curr, var->xid, var->value);
  } while((list = list->next));
  return GW_OK;
}

ANN static Type scan1_noret(const Env env, const Func_Base *base) {
  DECL_OO(const Type, t, = known_type(env, base->td));
  if(!tflag(t, tflag_noret))
    return t;
  ERR_O(base->pos, _("Can't use type `{+G}%s{0}` for return"),
        t->name);
}

ANN static m_bool _scan1_fdef_base_tmpl(const Env env, Func_Base *base) {
  Specialized_List id = base->tmpl->list;
  do nspc_add_type(env->curr, id->xid, env->gwion->type[et_auto]);
  while((id = id->next));
  CHECK_OB((base->ret_type = scan1_noret(env, base)));
  if(base->args) {
    Arg_List arg = base->args;
    do CHECK_OB(known_type(env, arg->td));
    while((arg = arg->next));
  }
  return GW_OK;
}

ANN static m_bool scan1_fdef_base_tmpl(const Env env, Func_Base *base) {
  nspc_push_type(env->gwion->mp, env->curr);
  const m_bool ret = _scan1_fdef_base_tmpl(env, base);
  nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}

ANN m_bool scan1_fptr_def(const Env env, const Fptr_Def fptr) {
  if(tmpl_base(fptr->base->tmpl))
    return scan1_fdef_base_tmpl(env, fptr->base);
  if(!fptr->base->func) {
    fptr->base->func = nspc_lookup_value0(env->curr, fptr->base->xid)->d.func_ref;
    fptr->type = nspc_lookup_type0(env->curr, fptr->base->xid);
  }
  const Func_Def fdef = fptr->base->func->def;
  CHECK_OB((fdef->base->ret_type = scan1_noret(env, fdef->base)));
  if(!fdef->base->args)
    return GW_OK;
  RET_NSPC(scan1_args(env, fdef->base->args))
}

ANN m_bool scan1_type_def(const Env env, const Type_Def tdef) {
  if(!tdef->type)
    tdef->type = nspc_lookup_type0(env->curr, tdef->xid);
  if(tdef->when)
    CHECK_BB(scan1_exp(env, tdef->when));
  return (!is_fptr(env->gwion, tdef->type) && tdef->type->info->cdef) ?
    scan1_cdef(env, tdef->type) : GW_OK;
}

ANN static inline m_bool scan1_union_def_inner_loop(const Env env, Union_Def udef) {
  nspc_allocdata(env->gwion->mp, udef->type->nspc);
  Union_List l = udef->l;
  m_uint sz = 0;
  const Value v = new_value(env->gwion->mp, env->gwion->type[et_int], "@index");
  nspc_add_value_front(env->curr, insert_symbol("@index"), v);
  valuefrom(env, v->from, udef->pos);
  do {
    DECL_OB(const Type, t, = known_type(env, l->td));
    if(nspc_lookup_value0(env->curr, l->xid))
      ERR_B(l->pos, _("'%s' already declared in union"), s_name(l->xid))
    const Value v = new_value(env->gwion->mp, t, s_name(l->xid));
    if(!tflag(t, tflag_scan1)) // ???
      tuple_contains(env, v);  // ???
    v->from->offset = SZ_INT;
    valuefrom(env ,v->from, udef->pos);
    nspc_add_value_front(env->curr, l->xid, v);
    if(t->size > sz)
      sz = t->size;
  } while((l = l->next));
  udef->type->nspc->info->offset = SZ_INT +sz;
  return GW_OK;
}

ANN static m_bool scan1_union_def_inner(const Env env, const Union_Def udef) {
  if(udef->tmpl && udef->tmpl->call)
    CHECK_BB(template_push_types(env, udef->tmpl));
  const m_bool ret = scan1_union_def_inner_loop(env, udef);
  if(udef->tmpl && udef->tmpl->call)
    nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}

ANN m_bool scan1_union_def(const Env env, const Union_Def udef) {
  if(tmpl_base(udef->tmpl))
    return GW_OK;
  const m_uint scope = env_push_type(env, udef->type);
  const m_bool ret = scan1_union_def_inner(env, udef);
  env_pop(env, scope);
  set_tflag(udef->type, tflag_scan1);
  return ret;
}

#define scan1_stmt_while    scan1_stmt_flow
#define scan1_stmt_until    scan1_stmt_flow
#define scan1_stmt_continue dummy_func
#define scan1_stmt_break    dummy_func
#define scan1_stmt_return   scan1_stmt_exp
#define scan1_stmt_retry    dummy_func

ANN static m_bool scan1_stmt_pp(const Env env, const Stmt_PP stmt) {
  if(stmt->pp_type == ae_pp_include)
    env->name = stmt->data;
  return GW_OK;
}

ANN static m_bool scan1_stmt_defer(const Env env, const Stmt_Defer stmt) {
  return scan1_stmt(env, stmt->stmt);
}

DECL_STMT_FUNC(scan1, m_bool, Env)

ANN static inline m_bool scan1_stmt(const Env env, const Stmt stmt) {
  return scan1_stmt_func[stmt->stmt_type](env, &stmt->d);
}

ANN static m_bool scan1_stmt_list(const Env env, Stmt_List l) {
  do {
    CHECK_BB(scan1_stmt(env, l->stmt));
    if(l->next) {
      if(l->stmt->stmt_type != ae_stmt_return) {
        if(l->next->stmt->stmt_type == ae_stmt_exp &&
          !l->next->stmt->d.stmt_exp.val) {
           Stmt_List next = l->next;
           l->next = l->next->next;
           next->next = NULL;
           free_stmt_list(env->gwion->mp, next);
        }
      } else {
        Stmt_List tmp = l->next;
        l->next = NULL;
        free_stmt_list(env->gwion->mp, tmp);
      }
    }
  } while((l = l->next));
  return GW_OK;
}

ANN static m_bool class_internal(const Env env, const Func_Base *base) {
  if(!env->class_def)
    ERR_B(base->td->pos, _("'%s' must be in class def!!"), s_name(base->xid))
  if(base->args)
    ERR_B(base->td->pos, _("'%s' must not have args"), s_name(base->xid))
  if(base->ret_type != env->gwion->type[et_void])
    ERR_B(base->td->pos, _("'%s' must return 'void'"), s_name(base->xid))
  return GW_OK;
}

ANN static inline m_bool scan_internal_arg(const Env env, const Func_Base *base) {
  if(base->args && !base->args->next)
    return GW_OK;
  ERR_B(base->td->pos, _("'%s' must have one (and only one) argument"), s_name(base->xid))
}

ANN static inline m_bool scan_internal_int(const Env env, const Func_Base *base) {
    CHECK_BB(scan_internal_arg(env, base));
    if(isa(base->ret_type, env->gwion->type[et_int]) > 0)
      return GW_OK;
    ERR_B(base->td->pos, _("'%s' must return 'int'"), s_name(base->xid))
}

ANN static m_bool scan_internal(const Env env, const Func_Base *base) {
  const Symbol op = base->xid;
  if(op == insert_symbol("@dtor") || op == insert_symbol("@gack"))
    return class_internal(env, base);
  if(op == insert_symbol("@implicit"))
    return scan_internal_arg(env, base);
  if(op == insert_symbol("@conditional") ||
     op == insert_symbol("@unconditional"))
    return scan_internal_int(env, base);
  return GW_OK;
}

ANN static m_bool scan1_fdef_args(const Env env, Arg_List list) {
  do CHECK_BB(shadow_arg(env, list->var_decl->xid, list->var_decl->pos));
  while((list = list->next));
  return GW_OK;
}

ANN m_bool scan1_fbody(const Env env, const Func_Def fdef) {
  if(fdef->base->args) {
    CHECK_BB(scan1_fdef_args(env, fdef->base->args));
    CHECK_BB(scan1_args(env, fdef->base->args));
  }
  if(fdef->d.code && fdef->d.code->d.stmt_code.stmt_list)
    CHECK_BB(scan1_stmt_list(env, fdef->d.code->d.stmt_code.stmt_list));
  return GW_OK;
}

ANN m_bool scan1_fdef(const Env env, const Func_Def fdef) {
  if(fdef->base->td)
    CHECK_OB((fdef->base->ret_type = scan1_noret(env, fdef->base)));
  if(fbflag(fdef->base, fbflag_internal))
    CHECK_BB(scan_internal(env, fdef->base));
  else if(fbflag(fdef->base, fbflag_op) && env->class_def)
    SET_FLAG(fdef->base, static);
  RET_NSPC(scan1_fbody(env, fdef))
  return GW_OK;
}

ANN static inline m_bool scan1_fdef_defined(const Env env, const Func_Def fdef) {
  const Value v = nspc_lookup_value1(env->curr, fdef->base->xid);
  if(!v)
    return GW_OK;
  if(isa(actual_type(env->gwion, v->type), env->gwion->type[et_function]) > 0)
    return GW_OK;
  if((!env->class_def || !GET_FLAG(env->class_def, final)) && !nspc_lookup_value0(env->curr, fdef->base->xid))
    ERR_B(fdef->base->pos, _("function '%s' has already been defined in the same scope..."),
         s_name(fdef->base->xid))
  return GW_OK;
}

ANN m_bool scan1_func_def(const Env env, const Func_Def fdef) {
  const bool global = GET_FLAG(fdef->base, global);
  const m_uint scope = !global ? env->scope->depth : env_push_global(env);
  if(fdef->base->td)
    CHECK_BB(env_storage(env, fdef->base->flag, fdef->base->td->pos));
  CHECK_BB(scan1_fdef_defined(env, fdef));
  if(tmpl_base(fdef->base->tmpl))
    return scan1_fdef_base_tmpl(env, fdef->base);
  struct Func_ fake = { .name=s_name(fdef->base->xid) }, *const former = env->func;
  env->func = &fake;
  ++env->scope->depth;
  const m_bool ret = scanx_fdef(env, env, fdef, (_exp_func)scan1_fdef);
  --env->scope->depth;
  env->func = former;
  if(global)
    env_pop(env, scope);
  return ret;
}

ANN static m_bool scan1_extend_def(const Env env, const Extend_Def xdef) {
  CHECK_BB(ensure_scan1(env, xdef->t));
  CHECK_BB(extend_push(env, xdef->t));
  const m_bool ret = scan1_ast(env, xdef->body);
  extend_pop(env, xdef->t);
  return ret;
}

#define scan1_trait_def dummy_func
HANDLE_SECTION_FUNC(scan1, m_bool, Env)

ANN static Type scan1_get_parent(const Env env, const Type_Def tdef) {
  const Type parent = known_type(env, tdef->ext);
  CHECK_OO((tdef->type->info->parent = parent));;
  Type t = parent;
  do if(tdef->type == t)
      ERR_O(tdef->ext->pos, _("recursive (%s <= %s) class declaration."), tdef->type->name, t->name)
  while((t = t->info->parent));
  return parent;
}

ANN static m_bool scan1_parent(const Env env, const Class_Def cdef) {
  const loc_t pos = cdef->base.ext->pos;
  if(cdef->base.ext->array && cdef->base.ext->array->exp)
    CHECK_BB(scan1_exp(env, cdef->base.ext->array->exp));
  DECL_OB(const Type , parent, = scan1_get_parent(env, &cdef->base));
  if(isa(parent, env->gwion->type[et_object]) < 0)
    ERR_B(pos, _("cannot extend primitive type '%s'"), parent->name)
  CHECK_BB(ensure_scan1(env, parent));
  if(type_ref(parent))
    ERR_B(pos, _("can't use ref type in class extend"))
  return GW_OK;
}

ANN static m_bool cdef_parent(const Env env, const Class_Def cdef) {
  const bool tmpl = !!cdef->base.tmpl;
  if(tmpl)
    CHECK_BB(template_push_types(env, cdef->base.tmpl));
  const m_bool ret = scan1_parent(env, cdef);
  if(tmpl)
    nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}

ANN m_bool scan1_class_def(const Env env, const Class_Def cdef) {
  if(tmpl_base(cdef->base.tmpl))
    return GW_OK;
  const Type t = cdef->base.type;
  if(tflag(t, tflag_scan1))
    return GW_OK;
  set_tflag(t, tflag_scan1);
  if(t->info->value->from->owner_class)
    CHECK_BB(ensure_scan1(env, t->info->value->from->owner_class));
  if(cdef->base.ext)
    CHECK_BB(cdef_parent(env, cdef));
  if(cdef->body)
    CHECK_BB(env_body(env, cdef, scan1_section));
  return GW_OK;
}

ANN m_bool scan1_ast(const Env env, Ast ast) {
  do CHECK_BB(scan1_section(env, ast->section));
  while((ast = ast->next));
  return GW_OK;
}
