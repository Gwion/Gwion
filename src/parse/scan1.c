#include "defs.h"
#include "err_msg.h"
#include "absyn.h"
#include "type.h"
#include "value.h"
#include "optim.h"

#define FAKE_FUNC ((Func)1)

ANN m_bool scan0_class_def(const Env env, const Class_Def class_def);
ANN static m_bool scan1_exp(const Env env, Exp exp);
ANN static m_bool scan1_stmt_list(const Env env, Stmt_List list);
ANN m_bool scan1_class_def(const Env env, const Class_Def class_def);
ANN static m_bool scan1_stmt(const Env env, Stmt stmt);

ANN static void scan1_exp_decl_template(const Type t, const Exp_Decl* decl) {
  Exp_Decl* d = (Exp_Decl*)decl;
  d->base = t->def;
  d->type = t;
}

ANN static Type scan1_exp_decl_type(const Env env, const Exp_Decl* decl) {
  const Type t = type_decl_resolve(env, decl->td);
  if(!t)
    CHECK_BO(type_unknown(decl->td->xid, "declaration"))
  if(!t->size)
    CHECK_BO(err_msg(SCAN1_, decl->self->pos,
          "cannot declare variables of size '0' (i.e. 'void')..."))
  if(!GET_FLAG(decl->td, ae_flag_ref)) {
    if(t == env->class_def && !env->class_scope)
      CHECK_BO(err_msg(SCAN1_, decl->self->pos,
            "...(note: object of type '%s' declared inside itself)", t->name))
  } else
    CHECK_BO(prim_ref(decl->td, t))
  if(GET_FLAG(decl->td, ae_flag_private) && !env->class_def)
      CHECK_BO(err_msg(SCAN1_, decl->self->pos,
            "must declare private variables at class scope..."))
  if(GET_FLAG(t, ae_flag_template))
    scan1_exp_decl_template(t, decl);
  return t;
}

ANN m_bool scan1_exp_decl(const Env env, const Exp_Decl* decl) { GWDEBUG_EXE
  Var_Decl_List list = decl->list;
  Type t = scan1_exp_decl_type(env, decl);
  const m_bool is_tmpl_class = SAFE_FLAG(env->class_def, ae_flag_template);
  CHECK_OB(t)
  if(decl->type && !env->func &&
    !(is_tmpl_class && GET_FLAG(env->class_def, ae_flag_builtin)))
    t = decl->type;
  else
    ((Exp_Decl*)decl)->type = t;
  do {
    const Var_Decl v = list->self;
    const Value value = nspc_lookup_value0(env->curr, v->xid);
    if(isres(v->xid) > 0)
      CHECK_BB(err_msg(SCAN1_, v->pos,
            "\t... in variable declaration", s_name(v->xid)))
    if(value && (!is_tmpl_class ||
        (is_tmpl_class && !GET_FLAG(env->class_def, ae_flag_scan1))))
      CHECK_BB(err_msg(SCAN1_, v->pos,
              "variable %s has already been defined in the same scope...",
              s_name(v->xid)))
    if(!v->array)
      t = decl->type;
    else {
      if(v->array->exp)
        CHECK_BB(scan1_exp(env, v->array->exp))
      t = array_type(decl->type, v->array->depth);
    }
    v->value = value ? value : new_value(t, s_name(v->xid));
    nspc_add_value(env->curr, v->xid, v->value);
    v->value->flag = decl->td->flag;
    if(v->array && !v->array->exp)
      SET_FLAG(v->value, ae_flag_ref);
    if(!env->func && !env->class_scope) {
      if(env->class_def) {
        if(!GET_FLAG(decl->td, ae_flag_static))
          SET_FLAG(v->value, ae_flag_member);
      } else
        SET_FLAG(v->value, ae_flag_global);
    }
    v->value->d.ptr = v->addr;
    v->value->owner = env->curr;
    v->value->owner_class = env->func ? NULL : env->class_def;
  } while((list = list->next));
  ((Exp_Decl*)decl)->type = decl->list->self->value->type;
  return 1;
}

ANN static m_bool scan1_exp_binary(const Env env, const Exp_Binary* bin) { GWDEBUG_EXE
  CHECK_BB(scan1_exp(env, bin->lhs))
  return scan1_exp(env, bin->rhs);
}

ANN static m_bool scan1_exp_primary(const Env env, const Exp_Primary* prim) { GWDEBUG_EXE
  if(prim->primary_type == ae_primary_hack)
    CHECK_BB(scan1_exp(env, prim->d.exp))
  return 1;
}

ANN static m_bool scan1_exp_array(const Env env, const Exp_Array* array) { GWDEBUG_EXE
  CHECK_BB(scan1_exp(env, array->base))
  return scan1_exp(env, array->array->exp);
}

ANN static m_bool scan1_exp_cast(const Env env, const Exp_Cast* cast) { GWDEBUG_EXE
  return scan1_exp(env, cast->exp);
}

ANN static m_bool scan1_exp_post(const Env env, const Exp_Postfix* post) { GWDEBUG_EXE
  CHECK_BB(scan1_exp(env, post->exp))
  if(post->exp->meta != ae_meta_var)
    CHECK_BB(err_msg(SCAN1_, post->exp->pos,
          "post operator '%s' cannot be used"
          " on non-mutable data-type...", op2str(post->op)))
  return 1;
}

ANN static m_bool scan1_exp_dur(const Env env, const Exp_Dur* dur) { GWDEBUG_EXE
  CHECK_BB(scan1_exp(env, dur->base))
  return scan1_exp(env, dur->unit);
}

ANN static m_bool scan1_exp_call(const Env env, const Exp_Func* exp_func) { GWDEBUG_EXE
  if(exp_func->tmpl)
    return 1;
  CHECK_BB(scan1_exp(env, exp_func->func))
  const Exp args = exp_func->args;
  return args ? scan1_exp(env, args) : 1;
}

ANN static m_bool scan1_exp_dot(const Env env, const Exp_Dot* member) { GWDEBUG_EXE
  return scan1_exp(env, member->base);
}

ANN static m_bool scan1_exp_if(const Env env, const Exp_If* exp_if) { GWDEBUG_EXE
  CHECK_BB(scan1_exp(env, exp_if->cond))
  CHECK_BB(scan1_exp(env, exp_if->if_exp))
  return scan1_exp(env, exp_if->else_exp);
}

ANN static m_bool scan1_exp_spork(const Env env, const Stmt code) { GWDEBUG_EXE
  const Func f = env->func;
  env->func = f ? f: FAKE_FUNC;
  CHECK_BB(scan1_stmt(env, code))
  env->func = f;
  return 1;
}

ANN static m_bool scan1_exp(const Env env, Exp exp) { GWDEBUG_EXE
  do {
    switch(exp->exp_type) {
      case ae_exp_primary:
        CHECK_BB(scan1_exp_primary(env, &exp->d.exp_primary))
        break;
      case ae_exp_decl:
        CHECK_BB(scan1_exp_decl(env, &exp->d.exp_decl))
        break;
      case ae_exp_unary:
        if(exp->d.exp_unary.op == op_spork && exp->d.exp_unary.code)
          CHECK_BB(scan1_exp_spork(env, exp->d.exp_unary.code))
        break;
      case ae_exp_binary:
        CHECK_BB(scan1_exp_binary(env, &exp->d.exp_binary))
        break;
      case ae_exp_post:
        CHECK_BB(scan1_exp_post(env, &exp->d.exp_post))
        break;
      case ae_exp_cast:
        CHECK_BB(scan1_exp_cast(env, &exp->d.exp_cast))
        break;
      case ae_exp_call:
        CHECK_BB(scan1_exp_call(env, &exp->d.exp_func))
        break;
      case ae_exp_array:
        CHECK_BB(scan1_exp_array(env, &exp->d.exp_array))
        break;
      case ae_exp_dot:
        CHECK_BB(scan1_exp_dot(env, &exp->d.exp_dot))
        break;
      case ae_exp_dur:
        CHECK_BB(scan1_exp_dur(env, &exp->d.exp_dur))
        break;
      case ae_exp_if:
        CHECK_BB(scan1_exp_if(env, &exp->d.exp_if))
        break;
      OPTIMIZE_DEFAULT
    }
  } while((exp = exp->next));
  return 1;
}

ANN static inline m_bool scan1_stmt_code(const Env env, const Stmt_Code stmt) { GWDEBUG_EXE
  return stmt->stmt_list ? scan1_stmt_list(env, stmt->stmt_list) : 1;
}

ANN static m_bool scan1_stmt_return(const Env env, const Stmt_Exp stmt) { GWDEBUG_EXE
  return stmt->val ? scan1_exp(env, stmt->val) : 1;
}

ANN static m_bool scan1_stmt_flow(const Env env, const struct Stmt_Flow_* stmt) { GWDEBUG_EXE
  CHECK_BB(scan1_exp(env, stmt->cond))
  return scan1_stmt(env, stmt->body);
}

ANN static m_bool scan1_stmt_for(const Env env, const Stmt_For stmt) { GWDEBUG_EXE
  const Func f = env->func;
  if(env->class_def && !env->func)
   env->func = FAKE_FUNC;
  nspc_push_value(env->curr);
  CHECK_BB(scan1_stmt(env, stmt->c1))
  CHECK_BB(scan1_stmt(env, stmt->c2))
  if(stmt->c3)
    CHECK_BB(scan1_exp(env, stmt->c3))
  CHECK_BB(scan1_stmt(env, stmt->body))
  nspc_pop_value(env->curr);
  env->func = f;
  return 1;
}

ANN static m_bool scan1_stmt_auto(const Env env, const Stmt_Auto stmt) { GWDEBUG_EXE
  CHECK_BB(scan1_exp(env, stmt->exp))
  return scan1_stmt(env, stmt->body);
}

ANN static m_bool scan1_stmt_loop(const Env env, const Stmt_Loop stmt) { GWDEBUG_EXE
  CHECK_BB(scan1_exp(env, stmt->cond))
  return scan1_stmt(env, stmt->body);
  return 1;
}

ANN static m_bool scan1_stmt_switch(const Env env, const Stmt_Switch stmt) { GWDEBUG_EXE
  return scan1_exp(env, stmt->val);
}

ANN static m_bool scan1_stmt_case(const Env env, const Stmt_Exp stmt) { GWDEBUG_EXE
  return stmt->val ? scan1_exp(env, stmt->val) : 1;
}

ANN static m_bool scan1_stmt_if(const Env env, const Stmt_If stmt) { GWDEBUG_EXE
  CHECK_BB(scan1_exp(env, stmt->cond))
  CHECK_BB(scan1_stmt(env, stmt->if_body))
  if(stmt->else_body)
    CHECK_BB(scan1_stmt(env, stmt->else_body))
  return 1;
}

ANN m_bool scan1_stmt_enum(const Env env, const Stmt_Enum stmt) { GWDEBUG_EXE
  ID_List list = stmt->list;
  m_uint count = 1;
  do {
    if(nspc_lookup_value0(env->curr, list->xid))
      CHECK_BB(err_msg(SCAN1_, stmt->self->pos,
            "in enum argument %i '%s' already declared as variable",
            count, s_name(list->xid)))
    const Value v = new_value(stmt->t, s_name(list->xid));
    if(env->class_def) {
      v->owner_class = env->class_def;
      SET_FLAG(v, ae_flag_static);
      if(GET_FLAG(stmt, ae_flag_private))
       SET_FLAG(v, ae_flag_private);
    }
    SET_FLAG(v, ae_flag_const | ae_flag_enum | ae_flag_checked);
    nspc_add_value(env->curr, list->xid, v);
    vector_add(&stmt->values, (vtype)v);
    ++count;
  } while((list = list->next));
  return 1;
}

ANN static m_int scan1_func_def_args(const Env env, Arg_List arg_list) { GWDEBUG_EXE
  do {
    if(!(arg_list->type = type_decl_resolve(env, arg_list->td)))
      CHECK_BB(type_unknown(arg_list->td->xid, "function argument"))
    if(arg_list->td->types)
      ADD_REF(arg_list->type)
  } while((arg_list = arg_list->next));
  return 1;
}

ANN m_bool scan1_stmt_fptr(const Env env, const Stmt_Ptr ptr) { GWDEBUG_EXE
  if(ptr->td->array)
    CHECK_BB(check_array_empty(ptr->td->array, "function pointer"))
  if(!(ptr->ret_type = type_decl_resolve(env, ptr->td)))
    CHECK_BB(type_unknown(ptr->td->xid, "func pointer definition"))
  if(!env->class_def && GET_FLAG(ptr, ae_flag_static))
    CHECK_BB(err_msg(SCAN1_, ptr->td->pos,
          "can't declare func pointer static outside of a class"))
  if(ptr->args && scan1_func_def_args(env, ptr->args) < 0)
    CHECK_BB(err_msg(SCAN1_, ptr->td->pos,
          "\t... in typedef '%s'...", s_name(ptr->xid)))
  return 1;
}

ANN static m_bool scan1_stmt_type(const Env env, const Stmt_Typedef stmt) { GWDEBUG_EXE
  return stmt->type->def ? scan1_class_def(env, stmt->type->def) : 1;
}

ANN static m_bool scan1_stmt_union_array(const Array_Sub array) { GWDEBUG_EXE
  if(array->exp)
    CHECK_BB(err_msg(SCAN1_, array->pos,
      "array declaration must be empty in union."))
  return 1;
}

ANN m_bool scan1_stmt_union(const Env env, const Stmt_Union stmt) { GWDEBUG_EXE
  Decl_List l = stmt->l;

  if(stmt->xid) {
    UNSET_FLAG(stmt, ae_flag_private);
    env_push_class(env, stmt->value->type);
  }
  do {
    Var_Decl_List list = l->self->d.exp_decl.list;

    if(l->self->exp_type != ae_exp_decl)
      CHECK_BB(err_msg(SCAN1_, stmt->self->pos,
            "invalid expression type '%i' in union declaration."))
    SET_FLAG(l->self->d.exp_decl.td, ae_flag_checked | stmt->flag);
    if(GET_FLAG(stmt, ae_flag_static))
      SET_FLAG(l->self->d.exp_decl.td, ae_flag_static);
    do {
      const Var_Decl var_decl = list->self;
      if(var_decl->array)
        CHECK_BB(scan1_stmt_union_array(var_decl->array))
    } while((list = list->next));
    CHECK_BB(scan1_exp_decl(env, &l->self->d.exp_decl))
  } while((l = l->next));
  if(stmt->xid)
    env_pop_class(env);
  return 1;
}

ANN static m_bool scan1_stmt(const Env env, const Stmt stmt) { GWDEBUG_EXE
  switch(stmt->stmt_type) {
    case ae_stmt_exp:
      if(stmt->d.stmt_exp.val)
        CHECK_BB(scan1_exp(env, stmt->d.stmt_exp.val))
      break;
    case ae_stmt_code:
      NSPC(CHECK_BB(scan1_stmt_code(env, &stmt->d.stmt_code)))
      break;
    case ae_stmt_return:
      CHECK_BB(scan1_stmt_return(env, &stmt->d.stmt_exp))
      break;
    case ae_stmt_if:
      NSPC(CHECK_BB(scan1_stmt_if(env, &stmt->d.stmt_if)))
      break;
    case ae_stmt_for:
      NSPC(CHECK_BB(scan1_stmt_for(env, &stmt->d.stmt_for)))
      break;
    case ae_stmt_auto:
      NSPC(CHECK_BB(scan1_stmt_auto(env, &stmt->d.stmt_auto)))
      break;
    case ae_stmt_while:
    case ae_stmt_until:
      NSPC(CHECK_BB(scan1_stmt_flow(env, &stmt->d.stmt_flow)))
      break;
    case ae_stmt_loop:
      NSPC(CHECK_BB(scan1_stmt_loop(env, &stmt->d.stmt_loop)))
      break;
    case ae_stmt_switch:
      NSPC(CHECK_BB(scan1_stmt_switch(env, &stmt->d.stmt_switch)))
      break;
    case ae_stmt_case:
      CHECK_BB(scan1_stmt_case(env, &stmt->d.stmt_exp))
      break;
    case ae_stmt_enum:
      CHECK_BB(scan1_stmt_enum(env, &stmt->d.stmt_enum))
      break;
    case ae_stmt_continue:
    case ae_stmt_break:
    case ae_stmt_gotolabel:
      break;
    case ae_stmt_funcptr:
      CHECK_BB(scan1_stmt_fptr(env, &stmt->d.stmt_ptr))
      break;
    case ae_stmt_typedef:
      CHECK_BB(scan1_stmt_type(env, &stmt->d.stmt_type))
      break;
    case ae_stmt_union:
      CHECK_BB(scan1_stmt_union(env, &stmt->d.stmt_union))
      break;
  }
  return 1;
}

ANN static m_bool scan1_stmt_list(const Env env, Stmt_List l) { GWDEBUG_EXE
  do {
    CHECK_BB(scan1_stmt(env, l->stmt))
    if(l->next) {
      if(l->stmt->stmt_type != ae_stmt_return) {
        if(l->next->stmt->stmt_type == ae_stmt_exp &&
          !l->next->stmt->d.stmt_exp.val)
        l->next = l->next->next;
      } else {
        Stmt_List tmp = l->next;
        l->next = NULL;
        free_stmt_list(tmp);
      }
    }
  }while((l = l->next));
  return 1;
}

ANN static m_bool scan1_func_def_type(const Env env, const Func_Def f) { GWDEBUG_EXE
  if(f->td->array)
    CHECK_BB(check_array_empty(f->td->array, "function return"))
  if(!(f->ret_type = type_decl_resolve(env, f->td)))
      CHECK_BB(type_unknown(f->td->xid, "function return"))
  return 1;
}

ANN static m_bool scan1_func_def_op(const Func_Def f) { GWDEBUG_EXE
  m_int count = 0;
  Arg_List list = f->arg_list;
  while(list) {
    ++count;
    list = list->next;
  }
  if(count > (GET_FLAG(f, ae_flag_unary) ? 1 : 2) || !count)
    CHECK_BB(err_msg(SCAN1_, f->td->pos,
          "operators can only have one or two arguments"))
  return 1;
}

ANN static m_bool scan1_func_def_flag(const Env env, const Func_Def f) { GWDEBUG_EXE
  if(GET_FLAG(f, ae_flag_dtor) && !env->class_def)
    CHECK_BB(err_msg(SCAN1_, f->td->pos, "dtor must be in class def!!"))
  if(GET_FLAG(f, ae_flag_op))
    CHECK_BB(scan1_func_def_op(f))
  return 1;
}

ANN static m_bool scan1_func_def_code(const Env env, const Func_Def f) { GWDEBUG_EXE
  nspc_push_value(env->curr);
  const m_bool ret = scan1_stmt_code(env, &f->d.code->d.stmt_code);
  nspc_pop_value(env->curr);
  return ret;
}

ANN m_bool scan1_func_def(const Env env, const Func_Def f) { GWDEBUG_EXE
  if(GET_FLAG(f, ae_flag_private) && !env->class_def)
    CHECK_BB(err_msg(SCAN1_, f->td->pos, "can't declare func '%s' private outside of class", s_name(f->name)))
  if(tmpl_list_base(f->tmpl))
    return 1;
  env->func = FAKE_FUNC;
  if(scan1_func_def_flag(env, f) < 0 ||
     scan1_func_def_type(env, f) < 0 ||
    (f->arg_list && scan1_func_def_args(env, f->arg_list) < 0) ||
    (!GET_FLAG(f, ae_flag_builtin) && f->d.code->d.stmt_code.stmt_list &&
        scan1_func_def_code(env, f) < 0))
    CHECK_BB(err_msg(SCAN1_, f->td->pos, "\t...in function '%s'", s_name(f->name)))
  env->func = NULL;
  return 1;
}

ANN static m_bool scan1_section(const Env env, const Section* section) { GWDEBUG_EXE
  const ae_Section_Type t = section->section_type;
  if(t == ae_section_stmt)
    CHECK_BB(scan1_stmt_list(env, section->d.stmt_list))
  else if(t == ae_section_func)
    CHECK_BB(scan1_func_def(env, section->d.func_def))
  else if(t == ae_section_class)
    CHECK_BB(scan1_class_def(env, section->d.class_def))
  return 1;
}

ANN m_bool scan1_class_def(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  if(tmpl_class_base(class_def->tmpl))
    return 1;
  if(class_def->ext) {
    const Type parent = type_decl_resolve(env, class_def->ext);
    if(!parent)
      CHECK_BB(type_unknown(class_def->ext->xid, "child class definition"))
    if(!GET_FLAG(parent, ae_flag_scan1) && parent->def)
      CHECK_BB(scan1_class_def(env, parent->def))
    if(class_def->ext->array) {
      if(!class_def->ext->array->exp) {
        REM_REF(t_array->nspc)
        CHECK_BB(err_msg(SCAN1_, class_def->ext->pos, "can't use empty []'s in class extend"))
      }
      CHECK_BB(scan1_exp(env, class_def->ext->array->exp))
    }
    if(type_ref(parent)) {
//      REM_REF(class_def->type->nspc)
      REM_REF(t_array->nspc)
      CHECK_BB(err_msg(SCAN1_, class_def->ext->pos, "can't use ref type in class extend"))
    }
    class_def->type->parent = parent;
  }
  if(class_def->body) {
    Class_Body body = class_def->body;
    env_push_class(env, class_def->type);
    do CHECK_BB(scan1_section(env, body->section))
    while((body = body->next));
    env_pop_class(env);
  }
  SET_FLAG(class_def->type, ae_flag_scan1);
  return 1;
}

ANN m_bool scan1_ast(const Env env, Ast ast) { GWDEBUG_EXE
  do CHECK_BB(scan1_section(env, ast->section))
  while((ast = ast->next));
  return 1;
}
