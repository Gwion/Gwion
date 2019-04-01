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

#define FAKE_FUNC ((Func)1)

ANN m_bool scan0_class_def(const Env env, const Class_Def class_def);
ANN static m_bool scan1_exp(const Env env, Exp exp);
ANN static m_bool scan1_stmt_list(const Env env, Stmt_List list);
ANN m_bool scan1_class_def(const Env env, const Class_Def class_def);
ANN static m_bool scan1_stmt(const Env env, Stmt stmt);

ANN static Type void_type(const Env env, const Type_Decl* td, const uint pos) {
  const Type t = known_type(env, td);
  CHECK_OO(t)
  if(t->size)
    return t;
  ERR_O(pos, "cannot declare variables of size '0' (i.e. 'void')...")
}

ANN static Type scan1_exp_decl_type(const Env env, Exp_Decl* decl) {
  const Type t = void_type(env, decl->td, decl->self->pos);
  CHECK_OO(t);
  if(GET_FLAG(t, abstract) && !GET_FLAG(decl->td, ref))
    ERR_O(decl->self->pos, "Type '%s' is abstract, declare as ref. (use @)", t->name)
  if(GET_FLAG(t, private) && t->owner != env->curr)
    ERR_O(decl->self->pos, "can't use private type %s", t->name)
  if(GET_FLAG(t, protect) && (!env->class_def || isa(t, env->class_def) < 0))
    ERR_O(decl->self->pos, "can't use protected type %s", t->name)
  if(GET_FLAG(decl->td, global) && env->class_def)
    ERR_O(decl->self->pos, "can't declare variable global inside class.")
  if(env->class_def) {
    if(!env->scope->depth) {
      if(!env->func && !GET_FLAG(decl->td, static))
        SET_FLAG(decl->td, member);
      if(!GET_FLAG(decl->td, ref) && t == env->class_def)
        ERR_O(decl->self->pos, "...(note: object of type '%s' declared inside itself)", t->name)
    }
  }
  decl->base = t->def;
  decl->type = t;
  return t;
}

ANN m_bool scan1_exp_decl(const Env env, Exp_Decl* decl) { GWDEBUG_EXE
  CHECK_BB(env_access(env, decl->td->flag))
  env_storage(env, &decl->td->flag);
  Var_Decl_List list = decl->list;
  m_uint scope;
  ((Exp_Decl*)decl)->type = scan1_exp_decl_type(env, (Exp_Decl*)decl);
  CHECK_OB(decl->type)
  const m_bool global = GET_FLAG(decl->td, global);
  const Nspc nspc = !global ? env->curr : env->global_nspc;
  if(global)
    scope = env_push_global(env);
  do {
    Type t = decl->type;
    const Var_Decl var = list->self;
    const Value former = nspc_lookup_value0(env->curr, var->xid);
    CHECK_BB(isres(var->xid))
    if(former && (!env->class_def ||
        (!GET_FLAG(env->class_def, template) || !GET_FLAG(env->class_def, scan1))))
      ERR_B(var->pos, "variable %s has already been defined in the same scope...",
              s_name(var->xid))
    if(var->array) {
      if(var->array->exp)
        CHECK_BB(scan1_exp(env, var->array->exp))
      t = array_type(env, decl->type, var->array->depth);
    }
    const Value v = var->value = former ? former : new_value(t, s_name(var->xid));
    nspc_add_value(nspc, var->xid, v);
    v->flag = decl->td->flag;
    if(var->array && !var->array->exp)
      SET_FLAG(v, ref);
    if(!env->func && !env->scope->depth && !env->class_def)
      SET_FLAG(v, global);
    v->d.ptr = var->addr;
    v->owner = !env->func ? env->curr : NULL;
    v->owner_class = env->scope->depth ? NULL : env->class_def;
  } while((list = list->next));
  decl->type = decl->list->self->value->type;
  if(global)
    env_pop(env, scope);
  return GW_OK;
}

ANN static inline m_bool scan1_exp_binary(const Env env, const Exp_Binary* bin) { GWDEBUG_EXE
  CHECK_BB(scan1_exp(env, bin->lhs))
  return scan1_exp(env, bin->rhs);
}

ANN static inline m_bool scan1_exp_primary(const Env env, const Exp_Primary* prim) { GWDEBUG_EXE
  if(prim->primary_type == ae_primary_hack)
    return scan1_exp(env, prim->d.exp);
  return GW_OK;
}

ANN static inline m_bool scan1_exp_array(const Env env, const Exp_Array* array) { GWDEBUG_EXE
  CHECK_BB(scan1_exp(env, array->base))
  return scan1_exp(env, array->array->exp);
}

ANN static inline m_bool scan1_exp_cast(const Env env, const Exp_Cast* cast) { GWDEBUG_EXE
  return scan1_exp(env, cast->exp);
}

ANN static m_bool scan1_exp_post(const Env env, const Exp_Postfix* post) { GWDEBUG_EXE
  CHECK_BB(scan1_exp(env, post->exp))
  return post->exp->meta == ae_meta_var ? 1 :
    err_msg(post->exp->pos, "post operator '%s' cannot be used"
          " on non-mutable data-type...", op2str(post->op));
}

ANN static m_bool scan1_exp_call(const Env env, const Exp_Call* exp_call) { GWDEBUG_EXE
  if(exp_call->tmpl)
    return GW_OK;
  CHECK_BB(scan1_exp(env, exp_call->func))
  const Exp args = exp_call->args;
  return args ? scan1_exp(env, args) : 1;
}

ANN static inline m_bool scan1_exp_dot(const Env env, const Exp_Dot* member) { GWDEBUG_EXE
  return scan1_exp(env, member->base);
}

ANN static m_bool scan1_exp_if(const Env env, const Exp_If* exp_if) { GWDEBUG_EXE
  CHECK_BB(scan1_exp(env, exp_if->cond))
  CHECK_BB(scan1_exp(env, exp_if->if_exp))
  return scan1_exp(env, exp_if->else_exp);
}

ANN static inline m_bool scan1_exp_unary(const restrict Env env, const Exp_Unary * unary) {
  if((unary->op == op_spork || unary->op == op_fork) && unary->code)
    return scan1_stmt(env, unary->code);
  return unary->exp ? scan1_exp(env, unary->exp) : GW_OK;
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
describe_ret_nspc(switch, Stmt_Switch,, scan1_exp(env, stmt->val))
describe_ret_nspc(if, Stmt_If,, !(scan1_exp(env, stmt->cond) < 0 ||
    scan1_stmt(env, stmt->if_body) < 0 ||
    (stmt->else_body && scan1_stmt(env, stmt->else_body) < 0)) ? 1 : -1)

ANN static inline m_bool scan1_stmt_code(const Env env, const Stmt_Code stmt) { GWDEBUG_EXE
  if(stmt->stmt_list) {
    RET_NSPC(scan1_stmt_list(env, stmt->stmt_list))
  }
  return GW_OK;
}

ANN static inline m_bool scan1_stmt_exp(const Env env, const Stmt_Exp stmt) { GWDEBUG_EXE
  return stmt->val ? scan1_exp(env, stmt->val) : 1;
}

ANN static inline m_bool scan1_stmt_case(const Env env, const Stmt_Exp stmt) { GWDEBUG_EXE
  return scan1_exp(env, stmt->val);
}

ANN m_bool scan1_stmt_enum(const Env env, const Stmt_Enum stmt) { GWDEBUG_EXE
  ID_List list = stmt->list;
  do {
    CHECK_BB(already_defined(env, list->xid, stmt->self->pos))
    const Value v = new_value(stmt->t, s_name(list->xid));
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
ANN static m_bool scan1_args(const Env env, Arg_List list) { GWDEBUG_EXE
  do {
    const Var_Decl var = list->var_decl;
    if(var->xid)
      CHECK_BB(isres(var->xid))
    if(list->td)
      CHECK_OB((list->type = void_type(env, list->td, var->pos)))
  } while((list = list->next));
  return GW_OK;
}

ANN m_bool scan1_stmt_fptr(const Env env, const Stmt_Fptr ptr) { GWDEBUG_EXE
  CHECK_OB((ptr->ret_type = known_type(env, ptr->td)))
  return ptr->args ? scan1_args(env, ptr->args) : GW_OK;
}

ANN static inline m_bool scan1_stmt_type(const Env env, const Stmt_Type stmt) { GWDEBUG_EXE
  return stmt->type->def ? scan1_class_def(env, stmt->type->def) : 1;
}

ANN m_bool scan1_stmt_union(const Env env, const Stmt_Union stmt) { GWDEBUG_EXE
  Decl_List l = stmt->l;
  const m_uint scope = union_push(env, stmt);
  if(stmt->xid || stmt->type_xid) {
    UNSET_FLAG(stmt, private);
    UNSET_FLAG(stmt, protect);
  }
  do {
    const Exp_Decl decl = l->self->d.exp_decl;
    SET_FLAG(decl.td, checked | stmt->flag);
    if(GET_FLAG(stmt, static))
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

ANN static inline m_bool scan1_stmt(const Env env, const Stmt stmt) { GWDEBUG_EXE
  return stmt_func[stmt->stmt_type](env, &stmt->d);
}

ANN static m_bool scan1_stmt_list(const Env env, Stmt_List l) { GWDEBUG_EXE
  do {
    CHECK_BB(scan1_stmt(env, l->stmt))
    if(l->next) {
      if(l->stmt->stmt_type != ae_stmt_return) {
        if(l->next->stmt->stmt_type == ae_stmt_exp &&
          !l->next->stmt->d.stmt_exp.val) {
           Stmt_List next = l->next;
           l->next = l->next->next;
           next->next = NULL;
           free_stmt_list(next);
        }
      } else {
        Stmt_List tmp = l->next;
        l->next = NULL;
        free_stmt_list(tmp);
      }
    }
  } while((l = l->next));
  return GW_OK;
}

ANN m_bool scan1_func_def(const Env env, const Func_Def f) { GWDEBUG_EXE
  CHECK_BB(env_access(env, f->flag))
  env_storage(env, &f->flag);
  if(tmpl_list_base(f->tmpl))
    return GW_OK;
  const Func former = env->func;
  env->func = FAKE_FUNC;
  ++env->scope->depth;
  if(GET_FLAG(f, dtor) && !env->class_def)
    ERR_B(f->td->xid->pos, "dtor must be in class def!!")
  if(f->td)
    CHECK_OB((f->ret_type = known_type(env, f->td)))
  if(f->args)
    CHECK_BB(scan1_args(env, f->args))
  if(!GET_FLAG(f, builtin))
    CHECK_BB(scan1_stmt_code(env, &f->d.code->d.stmt_code))
  if(GET_FLAG(f, op) && env->class_def)
    SET_FLAG(f, static);
  env->func = former;
  --env->scope->depth;
  return GW_OK;
}

DECL_SECTION_FUNC(scan1)

ANN static m_bool scan1_class_parent(const Env env, const Class_Def class_def) {
  if(class_def->ext->array)
    CHECK_BB(scan1_exp(env, class_def->ext->array->exp))
  const Type parent = class_def->type->parent = known_type(env, class_def->ext);
  CHECK_OB(parent)
  if(parent == class_def->type)
    ERR_B(class_def->ext->xid->pos, "class '%s' cannot extend itself",
      class_def->type->name);
  if(isa(class_def->type->parent, t_object) < 0)
    ERR_B(class_def->ext->xid->pos, "cannot extend primitive type '%s'",
            class_def->type->parent->name)
  if(!GET_FLAG(parent, scan1) && parent->def)
    CHECK_BB(scan1_class_def(env, parent->def))
  if(type_ref(parent))
    ERR_B(class_def->ext->xid->pos, "can't use ref type in class extend")
  return GW_OK;
}

ANN static m_bool scan1_class_body(const Env env, const Class_Def class_def) {
  const m_uint scope = env_push_type(env, class_def->type);
  Class_Body body = class_def->body;
  do CHECK_BB(scan1_section(env, body->section))
  while((body = body->next));
  env_pop(env, scope);
  return GW_OK;
}

ANN m_bool scan1_class_def(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  if(tmpl_class_base(class_def->tmpl))
    return GW_OK;
  if(class_def->ext)
    CHECK_BB(scan1_class_parent(env, class_def))
  if(class_def->body)
    CHECK_BB(scan1_class_body(env, class_def))
  SET_FLAG(class_def->type, scan1);
  return GW_OK;
}

ANN m_bool scan1_ast(const Env env, Ast ast) { GWDEBUG_EXE
  do CHECK_BB(scan1_section(env, ast->section))
  while((ast = ast->next));
  return GW_OK;
}
