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
  Type parent = t->e->parent;
  while(parent) {
    if(parent == env->class_def)
      ERR_B(td_pos(td), _("%s declared inside %s\n."), t->name, t == env->class_def ? "itself" : env->class_def->name);
    parent = parent->e->parent;
  }
  return GW_OK;
}

ANN static inline m_bool ensure_scan1(const Env env, const Type t) {
  struct EnvSet es = { .env=env, .data=env, .func=(_exp_func)scan1_cdef,
    .scope=env->scope->depth, .flag=ae_flag_scan1 };
  return envset_run(&es, t);
}

ANN static m_bool type_recursive(const Env env, const Type_Decl *td, const Type t) {
  if(!GET_FLAG(td, ref) && env->class_def && !env->scope->depth &&
          t == env->class_def) {
    ERR_B(td_pos(td), _("%s declared inside %s\n. (make it a ref ?)"),
       t->name, t == env->class_def ? "itself" : env->class_def->name);
  }
  return GW_OK;
}

ANN static Type scan1_type(const Env env, Type_Decl* td) {
  DECL_OO(const Type, type, = known_type(env, td))
  const Type t = get_type(type);
  CHECK_BO(type_cyclic(env, t, td))
  if(!GET_FLAG(t, scan1) && t->e->def)
    CHECK_BO(ensure_scan1(env, t))
  return type;
}

ANN static Type void_type(const Env env, Type_Decl* td) {
  DECL_OO(const Type, type, = scan1_type(env, td))
  if(isa(type, env->gwion->type[et_compound]) > 0)
    CHECK_BO(type_recursive(env, td, type))
  if(type->size)
    return type;
  ERR_O(td_pos(td), _("cannot declare variables of size '0' (i.e. 'void')..."))
}

ANN static Type scan1_exp_decl_type(const Env env, Exp_Decl* decl) {
  if(decl->type)
    return decl->type;
  DECL_OO(const Type ,t, = void_type(env, decl->td))
  if(decl->td->xid == insert_symbol("auto") && decl->type)
    return decl->type;
  if(!env->scope->depth && env->class_def && !GET_FLAG(decl->td, static))
    SET_FLAG(decl->td, member);
  if(GET_FLAG(t, private) && t->e->owner != env->curr)
    ERR_O(exp_self(decl)->pos, _("can't use private type %s"), t->name)
  if(GET_FLAG(t, protect) && (!env->class_def || isa(t, env->class_def) < 0))
    ERR_O(exp_self(decl)->pos, _("can't use protected type %s"), t->name)
  return decl->type = t;
}

ANN static m_bool scan1_decl(const Env env, const Exp_Decl* decl) {
  Var_Decl_List list = decl->list;
  do {
    const Var_Decl var = list->self;
    CHECK_BB(isres(env, var->xid, exp_self(decl)->pos))
    Type t = decl->type;
    const Value former = nspc_lookup_value0(env->curr, var->xid);
    if(former && t != env->gwion->type[et_auto])
      ERR_B(var->pos, _("variable %s has already been defined in the same scope..."),
              s_name(var->xid))
    if(var->array) {
      if(var->array->exp) {
        if(GET_FLAG(decl->td, ref))
          ERR_B(var->array->exp->pos, _("ref array must not have array expression.\n"
            "e.g: int @my_array[];\nnot: int @my_array[2];"))
        CHECK_BB(scan1_exp(env, var->array->exp))
      }
      t = array_type(env, decl->type, var->array->depth);
    } else if(GET_FLAG(t, abstract) && !GET_FLAG(decl->td, ref)) {
      if(decl->td->xid == insert_symbol("auto"))
        SET_FLAG(decl->td, ref);
      else
        ERR_B(exp_self(decl)->pos, _("Type '%s' is abstract, declare as ref. (use @)"), t->name)
    }
    const Value v = var->value = former ?: new_value(env->gwion->mp, t, s_name(var->xid));
    if(SAFE_FLAG(env->class_def, struct) && !GET_FLAG(decl->td, static)) {
      v->from->offset = env->class_def->size;
      env->class_def->size += t->size;
    }
    nspc_add_value(env->curr, var->xid, v);
    v->flag = decl->td->flag;
    v->type = t;
    if(var->array && !var->array->exp)
      SET_FLAG(v, ref);
    if(env->class_def) {
      if(env->class_def->e->tuple)
        tuple_contains(env, v);
    } else if(!env->scope->depth)
      SET_FLAG(v, global);
    v->d.ptr = var->addr;
    if(!env->scope->depth)
      valuefrom(env, v->from);
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
  CHECK_BB(env_storage(env, decl->td->flag, exp_self(decl)->pos))
  ((Exp_Decl*)decl)->type = scan1_exp_decl_type(env, (Exp_Decl*)decl);
  CHECK_OB(decl->type)
  if(GET_FLAG(decl->type, const))
    exp_setmeta(exp_self(decl), 1);
//    SET_FLAG(decl->td, const);
  const m_bool global = GET_FLAG(decl->td, global);
  if(global && !is_global(decl->type->e->owner, env->global_nspc))
    ERR_B(exp_self(decl)->pos, _("type '%s' is not global"), decl->type->name)
  if(env->context)
    env->context->global = 1;
  const m_uint scope = !global ? env->scope->depth : env_push_global(env);
  const m_bool ret = scan1_decl(env, decl);
  if(global)
    env_pop(env, scope);
  return ret;
}

ANN static inline m_bool scan1_exp_binary(const Env env, const Exp_Binary* bin) {
  CHECK_BB(scan1_exp(env, bin->lhs))
  return scan1_exp(env, bin->rhs);
}

ANN static m_bool scan1_range(const Env env, Range *range) {
  if(range->start)
    CHECK_BB(scan1_exp(env, range->start))
  if(range->end)
    CHECK_BB(scan1_exp(env, range->end))
  return GW_OK;
}

ANN static inline m_bool scan1_prim(const Env env, const Exp_Primary* prim) {
  if(prim->prim_type == ae_prim_hack || prim->prim_type == ae_prim_typeof ||
        prim->prim_type == ae_prim_interp)
    return scan1_exp(env, prim->d.exp);
  if(prim->prim_type == ae_prim_array && prim->d.array->exp)
    return scan1_exp(env, prim->d.array->exp);
  if(prim->prim_type == ae_prim_range)
    return scan1_range(env, prim->d.range);
  return GW_OK;
}

ANN static inline m_bool scan1_exp_array(const Env env, const Exp_Array* array) {
  CHECK_BB(scan1_exp(env, array->base))
  return scan1_exp(env, array->array->exp);
}

ANN static inline m_bool scan1_exp_slice(const Env env, const Exp_Slice* range) {
  CHECK_BB(scan1_exp(env, range->base))
  return scan1_range(env, range->range);
}

ANN static inline m_bool scan1_exp_cast(const Env env, const Exp_Cast* cast) {
  return scan1_exp(env, cast->exp);
}

ANN static m_bool scan1_exp_post(const Env env, const Exp_Postfix* post) {
  CHECK_BB(scan1_exp(env, post->exp))
  const m_str access = exp_access(post->exp);
  if(!access)
    return GW_OK;
  ERR_B(post->exp->pos, _("post operator '%s' cannot be used"
      " on %s data-type..."), s_name(post->op), access);
}

ANN static m_bool scan1_exp_call(const Env env, const Exp_Call* exp_call) {
  if(exp_call->tmpl)
    return GW_OK;
  CHECK_BB(scan1_exp(env, exp_call->func))
  const Exp args = exp_call->args;
  return args ? scan1_exp(env, args) : GW_OK;
}

ANN static inline m_bool scan1_exp_dot(const Env env, const Exp_Dot* member) {
  if(member->base->next)
    ERR_B(member->base->pos, _("can't use multiple expression"
      " in dot member base expression"))
  return scan1_exp(env, member->base);
}

ANN static m_bool scan1_exp_if(const Env env, const Exp_If* exp_if) {
  CHECK_BB(scan1_exp(env, exp_if->cond))
  CHECK_BB(scan1_exp(env, exp_if->if_exp ?: exp_if->cond))
  return scan1_exp(env, exp_if->else_exp);
}

ANN static inline m_bool scan1_exp_unary(const restrict Env env, const Exp_Unary *unary) {
  if((unary->op == insert_symbol("spork") || unary->op == insert_symbol("fork")) && unary->code)
    { RET_NSPC(scan1_stmt(env, unary->code)) }
  return unary->exp ? scan1_exp(env, unary->exp) : GW_OK;
}

#define scan1_exp_lambda dummy_func
HANDLE_EXP_FUNC(scan1, m_bool, Env)

ANN static inline m_bool _scan1_stmt_match_case(const restrict Env env, const Stmt_Match stmt) {
  CHECK_BB(scan1_exp(env, stmt->cond))
  if(stmt->when)
    CHECK_BB(scan1_exp(env, stmt->when))
  return scan1_stmt_list(env, stmt->list);
}

ANN static inline m_bool scan1_stmt_match_case(const restrict Env env, const Stmt_Match stmt) {
  RET_NSPC(_scan1_stmt_match_case(env, stmt))
}

ANN static inline m_bool _scan1_stmt_match(const restrict Env env, const Stmt_Match stmt) {
  if(stmt->where)
    CHECK_BB(scan1_stmt(env, stmt->where))
  Stmt_List list = stmt->list;
  do CHECK_BB(scan1_stmt_match_case(env, &list->stmt->d.stmt_match))
  while((list = list->next));
  return GW_OK;
}

ANN static inline m_bool scan1_stmt_match(const restrict Env env, const Stmt_Match stmt) {
  CHECK_BB(scan1_exp(env, stmt->cond))
  RET_NSPC(_scan1_stmt_match(env, stmt))
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
describe_ret_nspc(auto, Stmt_Auto,, !(scan1_exp(env, stmt->exp) < 0 ||
    scan1_stmt(env, stmt->body) < 0) ? 1 : -1)
describe_ret_nspc(loop, Stmt_Loop,, !(scan1_exp(env, stmt->cond) < 0 ||
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
    CHECK_BB(already_defined(env, list->xid, edef->pos))
    const Value v = new_value(env->gwion->mp, edef->t, s_name(list->xid));
    valuefrom(env, v->from);
    if(env->class_def) {
      SET_FLAG(v, static);
      SET_ACCESS(edef, v)
      SET_ACCESS(edef, edef->t)
    }
    SET_FLAG(v, const | ae_flag_enum | ae_flag_valid);
    nspc_add_value(edef->t->e->owner, list->xid, v);
    vector_add(&edef->values, (vtype)v);
  } while((list = list->next));
  return GW_OK;
}

ANN static Value arg_value(const Env env, const Arg_List list) {
  const Var_Decl var = list->var_decl;
  const Value v = new_value(env->gwion->mp, list->type, var->xid ? s_name(var->xid) : (m_str)__func__);
  if(var->array)
    v->type = list->type = array_type(env, list->type, var->array->depth);
  if(list->td)
    v->flag = list->td->flag | ae_flag_abstract;
  return v;
}

ANN static m_bool scan1_args(const Env env, Arg_List list) {
  do {
    const Var_Decl var = list->var_decl;
    if(var->xid)
      CHECK_BB(isres(env, var->xid, var->pos))
    if(list->td)
      CHECK_OB((list->type = void_type(env, list->td)))
    var->value = arg_value(env, list);
    nspc_add_value(env->curr, var->xid, var->value);
  } while((list = list->next));
  return GW_OK;
}

ANN static m_bool _scan1_fdef_base_tmpl(const Env env, Func_Base *base) {
  ID_List id = base->tmpl->list;
  do nspc_add_type(env->curr, id->xid, env->gwion->type[et_undefined]);
  while((id = id->next));
  CHECK_OB((base->ret_type = known_type(env, base->td)))
  if(base->args) {
    Arg_List arg = base->args;
    do CHECK_OB(known_type(env, arg->td))
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
  CHECK_OB((fdef->base->ret_type = scan1_type(env, fdef->base->td)))
  if(!fdef->base->args)
    return GW_OK;
  RET_NSPC(scan1_args(env, fdef->base->args))
}

ANN m_bool scan1_type_def(const Env env, const Type_Def tdef) {
  if(!tdef->type)
    tdef->type = nspc_lookup_type0(env->curr, tdef->xid);
  if(!tdef->type->e->def)return GW_OK;
  return !is_fptr(env->gwion, tdef->type) ? scan1_cdef(env, tdef->type->e->def) : GW_OK;
}

ANN m_bool scan1_union_def_action(const Env env, const Union_Def udef,
    const Decl_List l) {
  const Exp_Decl decl = l->self->d.exp_decl;
  SET_FLAG(decl.td, valid | udef->flag);
  const m_bool global = GET_FLAG(udef, global);
  if(global)
    UNSET_FLAG(decl.td, global);
  if(GET_FLAG(udef, member))
    SET_FLAG(decl.td, member);
  else if(GET_FLAG(udef, static))
    SET_FLAG(decl.td, static);
  if(udef->tmpl && udef->tmpl->call)
    CHECK_BB(template_push_types(env, udef->tmpl))
  const m_bool ret = scan1_exp(env, l->self);
  if(udef->tmpl && udef->tmpl->call)
    nspc_pop_type(env->gwion->mp, env->curr);
  CHECK_BB(ret)

  Var_Decl_List list = decl.list;
  do ADD_REF(list->self->value)
  while((list = list->next));

  if(global)
    SET_FLAG(decl.td, global);
  return GW_OK;
}

ANN m_bool scan1_union_def_inner(const Env env, const Union_Def udef) {
  Decl_List l = udef->l;
  do CHECK_BB(scan1_union_def_action(env, udef, l))
  while((l = l->next));
  return GW_OK;
}

ANN m_bool scan1_union_def(const Env env, const Union_Def udef) {
  if(tmpl_base(udef->tmpl))
    return GW_OK;
  const m_uint scope = union_push(env, udef);
  if(udef->xid || udef->type_xid) {
    UNSET_FLAG(udef, private);
    UNSET_FLAG(udef, protect);
  }
  const m_bool ret = scan1_union_def_inner(env, udef);
  union_pop(env, udef, scope);
  union_flag(udef, ae_flag_scan1);
  return ret;
}

#define scan1_stmt_while    scan1_stmt_flow
#define scan1_stmt_until    scan1_stmt_flow
#define scan1_stmt_continue (void*)dummy_func
#define scan1_stmt_break    (void*)dummy_func
#define scan1_stmt_jump     (void*)dummy_func
#define scan1_stmt_return   scan1_stmt_exp
#define scan1_stmt_pp       (void*)dummy_func
DECL_STMT_FUNC(scan1, m_bool, Env)

ANN static inline m_bool scan1_stmt(const Env env, const Stmt stmt) {
  return scan1_stmt_func[stmt->stmt_type](env, &stmt->d);
}

ANN static m_bool scan1_stmt_list(const Env env, Stmt_List l) {
  do {
    CHECK_BB(scan1_stmt(env, l->stmt))
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
    ERR_B(td_pos(base->td), _("'%s' must be in class def!!"), s_name(base->xid))
  if(base->args)
    ERR_B(td_pos(base->td), _("'%s' must not have args"), s_name(base->xid))
  if(base->ret_type != env->gwion->type[et_void])
    ERR_B(td_pos(base->td), _("'%s' must return 'void'"), s_name(base->xid))
  return GW_OK;
}

ANN static inline m_bool scan_internal_arg(const Env env, const Func_Base *base) {
  if(base->args && !base->args->next)
    return GW_OK;
  ERR_B(td_pos(base->td), _("'%s' must have one (and only one) argument"), s_name(base->xid))
}

ANN static inline m_bool scan_internal_int(const Env env, const Func_Base *base) {
    CHECK_BB(scan_internal_arg(env, base))
    if(isa(base->ret_type, env->gwion->type[et_int]) > 0)
      return GW_OK;
    ERR_B(td_pos(base->td), _("'%s' must return 'int'"), s_name(base->xid))
}

ANN static m_bool scan_internal(const Env env, const Func_Base *base) {
  const Symbol op = base->xid;
  if(op == insert_symbol("@dtor") || op == insert_symbol("@gack"))
    return class_internal(env, base);
  if(op == insert_symbol("@implicit"))
    return scan_internal_arg(env, base);
  if(op == insert_symbol("@conditionnal") ||
     op == insert_symbol("@unconditionnal"))
    return scan_internal_int(env, base);
  return GW_OK;
}

ANN m_bool scan1_fbody(const Env env, const Func_Def fdef) {
  if(fdef->base->args)
    CHECK_BB(scan1_args(env, fdef->base->args))
  if(!GET_FLAG(fdef, builtin) && fdef->d.code && fdef->d.code->d.stmt_code.stmt_list)
    CHECK_BB(scan1_stmt_list(env, fdef->d.code->d.stmt_code.stmt_list))
  return GW_OK;
}

ANN m_bool scan1_fdef(const Env env, const Func_Def fdef) {
  if(fdef->base->td)
    CHECK_OB((fdef->base->ret_type = known_type(env, fdef->base->td)))
  if(GET_FLAG(fdef, typedef))
    CHECK_BB(scan_internal(env, fdef->base))
  else if(GET_FLAG(fdef, op) && env->class_def)
    SET_FLAG(fdef, static);
  RET_NSPC(scan1_fbody(env, fdef))
  return GW_OK;
}

ANN m_bool scan1_func_def(const Env env, const Func_Def fdef) {
  if(fdef->base->td)
    CHECK_BB(env_storage(env, fdef->flag, td_pos(fdef->base->td)))
  if(tmpl_base(fdef->base->tmpl))
    return scan1_fdef_base_tmpl(env, fdef->base);
  struct Func_ fake = { .name=s_name(fdef->base->xid) }, *const former = env->func;
  env->func = &fake;
  ++env->scope->depth;
  const m_bool ret = scanx_fdef(env, env, fdef, (_exp_func)scan1_fdef);
  --env->scope->depth;
  env->func = former;
  return ret;
}

HANDLE_SECTION_FUNC(scan1, m_bool, Env)

ANN static Type scan1_get_parent(const Env env, const Type_Def tdef) {
  const Type parent = known_type(env, tdef->ext);
  CHECK_OO((tdef->type->e->parent = parent));
  Type t = parent;
  do if(tdef->type == t)
      ERR_O(td_pos(tdef->ext), _("recursive (%s <= %s) class declaration."), tdef->type->name, t->name)
  while((t = t->e->parent));
  return parent;
}

ANN static m_bool scan1_parent(const Env env, const Class_Def cdef) {
  const loc_t pos = td_pos(cdef->base.ext);
  if(cdef->base.ext->array)
    CHECK_BB(scan1_exp(env, cdef->base.ext->array->exp))
  DECL_OB(const Type , parent, = scan1_get_parent(env, &cdef->base))
  if(isa(parent, env->gwion->type[et_object]) < 0)
    ERR_B(pos, _("cannot extend primitive type '%s'"), parent->name)
  if(parent->e->def && !GET_FLAG(parent, scan1))
    CHECK_BB(scanx_parent(parent, scan1_cdef, env))
  if(type_ref(parent))
    ERR_B(pos, _("can't use ref type in class extend"))
  if(GET_FLAG(parent, nonnull))
    ERR_B(pos, _("can't use nonnull type in class extend"))
  return GW_OK;
}

ANN static m_bool cdef_parent(const Env env, const Class_Def cdef) {
  if(cdef->base.tmpl && cdef->base.tmpl->list)
    CHECK_BB(template_push_types(env, cdef->base.tmpl))
  const m_bool ret = scanx_parent(cdef->base.type, scan1_parent, env);
  if(cdef->base.tmpl && cdef->base.tmpl->list)
    nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}

ANN m_bool scan1_class_def(const Env env, const Class_Def cdef) {
  if(tmpl_base(cdef->base.tmpl))
    return GW_OK;
  const Type t = cdef->base.type;
  if(GET_FLAG(t, scan1))
    return GW_OK;
  SET_FLAG(t, scan1);
  if(t->e->owner_class && !GET_FLAG(t->e->owner_class, scan1))
    CHECK_BB(ensure_scan1(env, t->e->owner_class))
  SET_FLAG(cdef->base.type, scan1);
  if(cdef->base.ext)
    CHECK_BB(cdef_parent(env, cdef))
  if(cdef->body)
    CHECK_BB(env_body(env, cdef, scan1_section))
  return GW_OK;
}

ANN m_bool scan1_ast(const Env env, Ast ast) {
  do CHECK_BB(scan1_section(env, ast->section))
  while((ast = ast->next));
  return GW_OK;
}
