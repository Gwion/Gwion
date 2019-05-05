#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "type.h"
#include "nspc.h"
#include "value.h"
#include "optim.h"
#include "vm.h"
#include "parse.h"
#include "traverse.h"

ANN static m_bool scan1_stmt_list(const Env env, Stmt_List list);
ANN static m_bool scan1_stmt(const Env env, Stmt stmt);

ANN static Type void_type(const Env env, const Type_Decl* td) {
  const Type t = known_type(env, td);
  CHECK_OO(t)
  if(t->size)
    return t;
  ERR_O(td_pos(td), "cannot declare variables of size '0' (i.e. 'void')...")
}

ANN static Type scan1_exp_decl_type(const Env env, Exp_Decl* decl) {
  const Type t = void_type(env, decl->td);
  CHECK_OO(t);
  if(decl->td->xid && decl->td->xid->xid == insert_symbol("auto") && decl->type)
    return decl->type;
  if(GET_FLAG(t, abstract) && !GET_FLAG(decl->td, ref))
    ERR_O(exp_self(decl)->pos, "Type '%s' is abstract, declare as ref. (use @)", t->name)
  if(GET_FLAG(t, private) && t->owner != env->curr)
    ERR_O(exp_self(decl)->pos, "can't use private type %s", t->name)
  if(GET_FLAG(t, protect) && (!env->class_def || isa(t, env->class_def) < 0))
    ERR_O(exp_self(decl)->pos, "can't use protected type %s", t->name)
  if(env->class_def) {
    if(!env->scope->depth) {
      if(!env->func && !GET_FLAG(decl->td, static))
        SET_FLAG(decl->td, member);
      if(!GET_FLAG(decl->td, ref) && t == env->class_def)
        ERR_O(exp_self(decl)->pos, "...(note: object of type '%s' declared inside itself)", t->name)
    }
  }
  decl->base = t->def;
  return decl->type = t;
}

ANN m_bool scan1_exp_decl(const Env env, const Exp_Decl* decl) {
  CHECK_BB(env_storage(env, decl->td->flag, exp_self(decl)->pos))
  Var_Decl_List list = decl->list;
  ((Exp_Decl*)decl)->type = scan1_exp_decl_type(env, (Exp_Decl*)decl);
  CHECK_OB(decl->type)
  const m_bool global = GET_FLAG(decl->td, global);
  const m_uint scope = !global ? env->scope->depth : env_push_global(env);
  const Nspc nspc = !global ? env->curr : env->global_nspc;
  do {
    Type t = decl->type;
    const Var_Decl var = list->self;
    const Value former = nspc_lookup_value0(env->curr, var->xid);
    CHECK_BB(isres(env, var->xid, exp_self(decl)->pos))
    if(former && !decl->td->exp &&
        (!env->class_def || !(GET_FLAG(env->class_def, template) || GET_FLAG(env->class_def, scan1))))
      ERR_B(var->pos, "variable %s has already been defined in the same scope...",
              s_name(var->xid))
    if(var->array) {
      if(var->array->exp) {
        if(GET_FLAG(decl->td, ref))
          ERR_B(td_pos(decl->td), "ref array must not have array expression.\n"
            "e.g: int @my_array[];\nnot: int my_array[2];")
        CHECK_BB(scan1_exp(env, var->array->exp))
      }
      t = array_type(env, decl->type, var->array->depth);
    }
    const Value v = var->value = former ? former : new_value(env->gwion->p, t, s_name(var->xid));
    nspc_add_value(nspc, var->xid, v);
    v->flag = decl->td->flag;
    if(var->array && !var->array->exp)
      SET_FLAG(v, ref);
    if(!env->func && !env->scope->depth && !env->class_def)
      SET_FLAG(v, global);
    v->type = t;
    v->d.ptr = var->addr;
    v->owner = !env->func ? env->curr : NULL;
    v->owner_class = env->scope->depth ? NULL : env->class_def;
  } while((list = list->next));
  ((Exp_Decl*)decl)->type = decl->list->self->value->type;
  if(global)
    env_pop(env, scope);
  return GW_OK;
}

ANN static inline m_bool scan1_exp_binary(const Env env, const Exp_Binary* bin) {
  CHECK_BB(scan1_exp(env, bin->lhs))
  return scan1_exp(env, bin->rhs);
}

ANN static inline m_bool scan1_exp_primary(const Env env, const Exp_Primary* prim) {
  if(prim->primary_type == ae_primary_hack)
    return scan1_exp(env, prim->d.exp);
  if(prim->primary_type == ae_primary_array && prim->d.array->exp)
    return scan1_exp(env, prim->d.array->exp);
  return GW_OK;
}

ANN static inline m_bool scan1_exp_array(const Env env, const Exp_Array* array) {
  CHECK_BB(scan1_exp(env, array->base))
  return scan1_exp(env, array->array->exp);
}

ANN static inline m_bool scan1_exp_cast(const Env env, const Exp_Cast* cast) {
  return scan1_exp(env, cast->exp);
}

ANN static m_bool scan1_exp_post(const Env env, const Exp_Postfix* post) {
  CHECK_BB(scan1_exp(env, post->exp))
  if(post->exp->meta == ae_meta_var)
    return GW_OK;
  env_err(env, post->exp->pos, "post operator '%s' cannot be used"
      " on non-mutable data-type...", op2str(post->op));
  return GW_ERROR;
}

ANN static m_bool scan1_exp_call(const Env env, const Exp_Call* exp_call) {
  if(exp_call->tmpl)
    return GW_OK;
  CHECK_BB(scan1_exp(env, exp_call->func))
  const Exp args = exp_call->args;
  return args ? scan1_exp(env, args) : 1;
}

ANN static inline m_bool scan1_exp_dot(const Env env, const Exp_Dot* member) {
  return scan1_exp(env, member->base);
}

ANN static m_bool scan1_exp_if(const Env env, const Exp_If* exp_if) {
  CHECK_BB(scan1_exp(env, exp_if->cond))
  CHECK_BB(scan1_exp(env, exp_if->if_exp))
  return scan1_exp(env, exp_if->else_exp);
}

ANN static inline m_bool scan1_exp_unary(const restrict Env env, const Exp_Unary *unary) {
  if((unary->op == op_spork || unary->op == op_fork) && unary->code) {
    RET_NSPC(scan1_stmt(env, unary->code))
  }
  return unary->exp ? scan1_exp(env, unary->exp) : GW_OK;
}

ANN static inline m_bool scan1_exp_typeof(const restrict Env env, const Exp_Typeof *exp) {
  return scan1_exp(env, exp->exp);
}

#define scan1_exp_lambda dummy_func
HANDLE_EXP_FUNC(scan1, m_bool, 1)

#define describe_ret_nspc(name, type, prolog, exp) describe_stmt_func(scan1, name, type, prolog, exp)
describe_ret_nspc(flow, Stmt_Flow,, !(scan1_exp(env, stmt->cond) < 0 ||
    scan1_stmt(env, stmt->body) < 0) ? 1 : -1)
describe_ret_nspc(for, Stmt_For,, !(scan1_stmt(env, stmt->c1) < 0 ||
    scan1_stmt(env, stmt->c2) < 0 ||
    (stmt->c3 && scan1_exp(env, stmt->c3) < 0) ||
    scan1_stmt(env, stmt->body) < 0) ? 1 : -1)
describe_ret_nspc(auto, Stmt_Auto,, !(scan1_exp(env, stmt->exp) < 0 ||
    scan1_stmt(env, stmt->body) < 0) ? 1 : -1)
describe_ret_nspc(loop, Stmt_Loop,, !(scan1_exp(env, stmt->cond) < 0 ||
    scan1_stmt(env, stmt->body) < 0) ? 1 : -1)
describe_ret_nspc(switch, Stmt_Switch,, !(scan1_exp(env, stmt->val) < 0 ||
    scan1_stmt(env, stmt->stmt) < 0) ? 1 : -1)
describe_ret_nspc(if, Stmt_If,, !(scan1_exp(env, stmt->cond) < 0 ||
    scan1_stmt(env, stmt->if_body) < 0 ||
    (stmt->else_body && scan1_stmt(env, stmt->else_body) < 0)) ? 1 : -1)

ANN static inline m_bool scan1_stmt_code(const Env env, const Stmt_Code stmt) {
  if(stmt->stmt_list) {
    RET_NSPC(scan1_stmt_list(env, stmt->stmt_list))
  }
  return GW_OK;
}

ANN static inline m_bool scan1_stmt_exp(const Env env, const Stmt_Exp stmt) {
  return stmt->val ? scan1_exp(env, stmt->val) : 1;
}

ANN static inline m_bool scan1_stmt_case(const Env env, const Stmt_Exp stmt) {
  return scan1_exp(env, stmt->val);
}

ANN m_bool scan1_stmt_enum(const Env env, const Stmt_Enum stmt) {
  if(!stmt->t)
    CHECK_BB(scan0_stmt_enum(env, stmt))
  ID_List list = stmt->list;
  do {
    CHECK_BB(already_defined(env, list->xid, stmt_self(stmt)->pos))
    const Value v = new_value(env->gwion->p, stmt->t, s_name(list->xid));
    if(env->class_def) {
      v->owner_class = env->class_def;
      v->owner = env->curr;
      SET_FLAG(v, static);
      SET_ACCESS(stmt, v)
    }
    SET_FLAG(v, const | ae_flag_enum | ae_flag_checked);
    nspc_add_value(stmt->t->owner, list->xid, v);
    vector_add(&stmt->values, (vtype)v);
  } while((list = list->next));
  return GW_OK;
}
#include "func.h"
ANN static m_bool scan1_args(const Env env, Arg_List list) {
  do {
    const Var_Decl var = list->var_decl;
    if(var->xid)
      CHECK_BB(isres(env, var->xid, var->pos))
    if(list->td)
      CHECK_OB((list->type = void_type(env, list->td)))
  } while((list = list->next));
  return GW_OK;
}

ANN m_bool scan1_stmt_fptr(const Env env, const Stmt_Fptr stmt) {
  if(!stmt->type)
    CHECK_BB(scan0_stmt_fptr(env, stmt))
  CHECK_OB((stmt->base->ret_type = known_type(env, stmt->base->td)))
  return stmt->base->args ? scan1_args(env, stmt->base->args) : GW_OK;
}

ANN static inline m_bool scan1_stmt_type(const Env env, const Stmt_Type stmt) {
  if(!stmt->type)
    CHECK_BB(scan0_stmt_type(env, stmt))
  return stmt->type->def ? scan1_class_def(env, stmt->type->def) : 1;
}

ANN m_bool scan1_stmt_union(const Env env, const Stmt_Union stmt) {
  if(!stmt->value)
    CHECK_BB(scan0_stmt_union(env, stmt))
  Decl_List l = stmt->l;
  const m_uint scope = union_push(env, stmt);
  if(stmt->xid || stmt->type_xid) {
    UNSET_FLAG(stmt, private);
    UNSET_FLAG(stmt, protect);
  }
  do {
    const Exp_Decl decl = l->self->d.exp_decl;
    SET_FLAG(decl.td, checked | stmt->flag);
    if(GET_FLAG(stmt, member))
      SET_FLAG(decl.td, member);
    else if(GET_FLAG(stmt, static))
      SET_FLAG(decl.td, static);
    CHECK_BB(scan1_exp(env, l->self))
  } while((l = l->next));
  union_pop(env, stmt, scope);
  return GW_OK;
}

static const _exp_func stmt_func[] = {
  (_exp_func)scan1_stmt_exp,  (_exp_func)scan1_stmt_flow, (_exp_func)scan1_stmt_flow,
  (_exp_func)scan1_stmt_for,  (_exp_func)scan1_stmt_auto, (_exp_func)scan1_stmt_loop,
  (_exp_func)scan1_stmt_if,   (_exp_func)scan1_stmt_code, (_exp_func)scan1_stmt_switch,
  (_exp_func)dummy_func,      (_exp_func)dummy_func,      (_exp_func)scan1_stmt_exp,
  (_exp_func)scan1_stmt_case, (_exp_func)dummy_func,      (_exp_func)scan1_stmt_enum,
  (_exp_func)scan1_stmt_fptr, (_exp_func)scan1_stmt_type, (_exp_func)scan1_stmt_union,
};

ANN static inline m_bool scan1_stmt(const Env env, const Stmt stmt) {
  return stmt_func[stmt->stmt_type](env, &stmt->d);
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
           free_stmt_list(env->gwion->p, next);
        }
      } else {
        Stmt_List tmp = l->next;
        l->next = NULL;
        free_stmt_list(env->gwion->p, tmp);
      }
    }
  } while((l = l->next));
  return GW_OK;
}

ANN m_bool scan1_func_def(const Env env, const Func_Def fdef) {
  if(fdef->base->td)
    CHECK_BB(env_storage(env, fdef->flag, td_pos(fdef->base->td)))
  if(tmpl_list_base(fdef->tmpl))
    return GW_OK;
  struct Func_ fake = { .name=s_name(fdef->base->xid) }, *const former = env->func;
  env->func = &fake;
  ++env->scope->depth;
  if(GET_FLAG(fdef, dtor) && !env->class_def)
    ERR_B(td_pos(fdef->base->td), "dtor must be in class def!!")
  if(fdef->base->td)
    CHECK_OB((fdef->base->ret_type = known_type(env, fdef->base->td)))
  if(fdef->base->args)
    CHECK_BB(scan1_args(env, fdef->base->args))
  if(!GET_FLAG(fdef, builtin))
    CHECK_BB(scan1_stmt_code(env, &fdef->d.code->d.stmt_code))
  if(GET_FLAG(fdef, op) && env->class_def)
    SET_FLAG(fdef, static);
  env->func = former;
  --env->scope->depth;
  return GW_OK;
}

DECL_SECTION_FUNC(scan1)

ANN static m_bool scan1_class_parent(const Env env, const Class_Def cdef) {
  const loc_t pos = td_pos(cdef->base.ext);
  if(cdef->base.ext->array)
    CHECK_BB(scan1_exp(env, cdef->base.ext->array->exp))
  const Type parent = cdef->base.type->parent = known_type(env, cdef->base.ext);
  CHECK_OB(parent)
  if(parent == t_undefined)
    return GW_OK;
  Type t = parent;
  while(t) {
    if(cdef->base.type == t)
      ERR_B(pos, "recursive (%s <= %s) class declaration.", cdef->base.type->name, t->name);
    t = t->parent;
  }
  if(parent == cdef->base.type)
    ERR_B(pos, "class '%s' cannot extend itself", cdef->base.type->name);
  if(isa(cdef->base.type->parent, t_object) < 0)
    ERR_B(pos, "cannot extend primitive type '%s'",
            cdef->base.type->parent->name)
  if(!GET_FLAG(parent, scan1) && parent->def)
    CHECK_BB(scan1_class_def(env, parent->def))
  if(type_ref(parent))
    ERR_B(pos, "can't use ref type in class extend")
  return GW_OK;
}

ANN static m_bool scan1_class_body(const Env env, const Class_Def cdef) {
  const m_uint scope = env_push_type(env, cdef->base.type);
  Class_Body body = cdef->body;
  do CHECK_BB(scan1_section(env, body->section))
  while((body = body->next));
  env_pop(env, scope);
  return GW_OK;
}

ANN m_bool scan1_class_def(const Env env, const Class_Def cdef) {
  if(!cdef->base.type)
    CHECK_BB(scan0_class_def(env, cdef))
  if(tmpl_class_base(cdef->tmpl))
    return GW_OK;
  if(cdef->base.ext)
    CHECK_BB(scan1_class_parent(env, cdef))
  if(cdef->body)
    CHECK_BB(scan1_class_body(env, cdef))
  SET_FLAG(cdef->base.type, scan1);
  return GW_OK;
}

ANN m_bool scan1_ast(const Env env, Ast ast) {
  do CHECK_BB(scan1_section(env, ast->section))
  while((ast = ast->next));
  return GW_OK;
}
