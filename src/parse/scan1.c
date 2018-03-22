#include "defs.h"
#include "err_msg.h"
#include "absyn.h"
#include "type.h"
#include "value.h"
#include "traverse.h"
#ifdef OPTIMIZE
#include <string.h>
#endif
#define FAKE_FUNC ((Func)1)

ANN m_bool scan0_class_def(const Env env, const Class_Def class_def);
ANN static m_bool scan1_exp(const Env env, Exp exp);
ANN static m_bool scan1_stmt_list(const Env env, Stmt_List list);
ANN m_bool scan1_class_def(const Env env, const Class_Def class_def);
ANN static m_bool scan1_stmt(const Env env, Stmt stmt);

ANN static m_bool scan1_exp_decl_template(const Type t, const Exp_Decl* decl) { GWDEBUG_EXE
  if(GET_FLAG(t, ae_flag_template)) {
    Exp_Decl* d = (Exp_Decl*)decl;
    d->base = t->def;
    d->m_type = t;
  }
  return 1;
}

ANN static Type scan1_exp_decl_type(const Env env, const Exp_Decl* decl) { GWDEBUG_EXE
  const Type t = type_decl_resolve(env, decl->type);
  if(!t)
    CHECK_BO(type_unknown(decl->type->xid, "declaration"))
  if(!t->size)
    CHECK_BO(err_msg(SCAN1_, decl->pos,
          "cannot declare variables of size '0' (i.e. 'void')..."))
  if(!GET_FLAG(decl->type, ae_flag_ref)) {
    if(env->class_def && (t == env->class_def) && !env->class_scope)
      CHECK_BO(err_msg(SCAN1_, decl->pos,
            "...(note: object of type '%s' declared inside itself)", t->name))
  } else
    CHECK_BO(prim_ref(decl->type, t))
  if(GET_FLAG(decl->type, ae_flag_private) && !env->class_def)
      CHECK_BO(err_msg(SCAN2_, decl->pos,
            "must declare private variables at class scope..."))
  return t;
}

ANN m_bool scan1_exp_decl(const Env env, const Exp_Decl* decl) { GWDEBUG_EXE
  Var_Decl_List list = decl->list;
  Type t = scan1_exp_decl_type(env, decl);
  CHECK_OB(t)
  CHECK_BB(scan1_exp_decl_template(t, decl))
  if(decl->m_type && !env->func &&
    !(env->class_def && GET_FLAG(env->class_def, ae_flag_template) && GET_FLAG(env->class_def, ae_flag_builtin)))
    t = decl->m_type;
  else
    ((Exp_Decl*)decl)->m_type = t;
  while(list) {
    Value value;
    const Var_Decl v = list->self;
    if(isres(v->xid) > 0)
      CHECK_BB(err_msg(SCAN2_, v->pos,
            "\t... in variable declaration", s_name(v->xid)))
    if((value = nspc_lookup_value0(env->curr, v->xid)) &&
      (!(env->class_def && GET_FLAG(env->class_def, ae_flag_template)) ||
      (env->class_def && GET_FLAG(env->class_def, ae_flag_template) &&
        !GET_FLAG(env->class_def, ae_flag_scan1))))
        CHECK_BB(err_msg(SCAN1_, v->pos,
              "variable %s has already been defined in the same scope...",
              s_name(v->xid)))
    if(v->array) {
      if(v->array->exp_list)
        CHECK_BB(scan1_exp(env, v->array->exp_list))
      CHECK_OB((t = array_type(decl->m_type, v->array->depth)))
    } else
      t = decl->m_type;
    v->value = value ? value : new_value(t, s_name(v->xid));
    nspc_add_value(env->curr, v->xid, v->value);
    v->value->flag = decl->type->flag;
    if(v->array && !v->array->exp_list)
      SET_FLAG(v->value, ae_flag_ref);
    if(env->class_def && !env->class_scope && !env->func && !GET_FLAG(decl->type, ae_flag_static))
      SET_FLAG(v->value, ae_flag_member);
    if(!env->class_def && !env->func && !env->class_scope)
      SET_FLAG(v->value, ae_flag_global);
    v->value->d.ptr = v->addr;
    v->value->owner = env->curr;
    v->value->owner_class = env->func ? NULL : env->class_def;
    list = list->next;
  }
  ((Exp_Decl*)decl)->m_type = decl->list->self->value->m_type;
  return 1;
}

#ifdef OPTIMIZE
ANN static m_bool is_const(Exp e) {
  return e->exp_type == ae_exp_primary && e->d.exp_primary.primary_type == ae_primary_num && e->meta == ae_meta_value;
}

ANN static void optimize_const_folding(const Exp_Binary* bin) {
  Exp l = bin->lhs, r = bin->rhs;
  m_int ret = 0;
  switch(bin->op) {
    case op_plus:
      ret = l->d.exp_primary.d.num + r->d.exp_primary.d.num;
      break;
    case op_minus:
      ret = l->d.exp_primary.d.num - r->d.exp_primary.d.num;
      break;
    case op_times:
       ret = l->d.exp_primary.d.num * r->d.exp_primary.d.num;
       break;
    case op_divide:
       ret = l->d.exp_primary.d.num / r->d.exp_primary.d.num;
       break;
    case op_percent:
      ret = l->d.exp_primary.d.num % r->d.exp_primary.d.num;
      break;
    case op_shift_left:
      ret = l->d.exp_primary.d.num >> r->d.exp_primary.d.num;
      break;
    case op_shift_right:
      ret = l->d.exp_primary.d.num << r->d.exp_primary.d.num;
      break;
    default:
      return;
  }
  Exp n = bin->self->next;
  Exp e = bin->self;
  free_exp(l);
  free_exp(r);
  memset(e, 0, sizeof(struct Exp_));
  e->exp_type = ae_exp_primary;
  e->d.exp_primary.primary_type = ae_primary_num;
  e->d.exp_primary.d.num = ret;
  e->d.exp_primary.self = e;
  e->next = n;
}
#define OPTIMIZE_CONST_FOLD\
  if(is_const(binary->lhs) && is_const(binary->rhs))\
    optimize_const_folding(binary);
#else
#define OPTIMIZE_CONST_FOLD
#endif

ANN static m_bool scan1_exp_binary(const Env env, const Exp_Binary* binary) { GWDEBUG_EXE
  CHECK_BB(scan1_exp(env, binary->lhs))
  CHECK_BB(scan1_exp(env, binary->rhs))
  OPTIMIZE_CONST_FOLD
  return 1;

}

ANN static m_bool scan1_exp_primary(const Env env, const Exp_Primary* prim) { GWDEBUG_EXE
  if(prim->primary_type == ae_primary_hack)
    CHECK_BB(scan1_exp(env, prim->d.exp))
  return 1;
}

ANN static m_bool scan1_exp_array(const Env env, const Exp_Array* array) { GWDEBUG_EXE
  CHECK_BB(scan1_exp(env, array->base))
  return scan1_exp(env, array->indices->exp_list);
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
  Exp args = exp_func->args;
  if(exp_func->tmpl)
    return 1;
  CHECK_BB(scan1_exp(env, exp_func->func))
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
    }
  } while((exp = exp->next));
  return 1;
}

ANN static m_bool scan1_stmt_code(const Env env, const Stmt_Code stmt, const m_bool push) { GWDEBUG_EXE
  m_bool ret;
  if(!stmt->stmt_list)
    return 1;
  env->class_scope++;
  if(push)
    nspc_push_value(env->curr);
  ret = scan1_stmt_list(env, stmt->stmt_list);
  if(push)
    nspc_pop_value(env->curr);
  env->class_scope--;
  return ret;
}

ANN static m_bool scan1_stmt_return(const Env env, const Stmt_Return stmt) { GWDEBUG_EXE
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

ANN static m_bool scan1_stmt_case(const Env env, const Stmt_Case stmt) { GWDEBUG_EXE
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
  while(list) {
    Value v;
    if(nspc_lookup_value0(env->curr, list->xid))
      CHECK_BB(err_msg(SCAN1_, stmt->pos,
            "in enum argument %i '%s' already declared as variable",
            count, s_name(list->xid)))
    v = new_value(stmt->t, s_name(list->xid));
    if(env->class_def) {
      v->owner_class = env->class_def;
      SET_FLAG(v, ae_flag_static);
      if(GET_FLAG(stmt, ae_flag_private))
       SET_FLAG(v, ae_flag_private);
    }
    SET_FLAG(v, ae_flag_const | ae_flag_enum | ae_flag_checked);
    nspc_add_value(env->curr, list->xid, v);
    vector_add(&stmt->values, (vtype)v);
    list = list->next;
    count++;
  }
  return 1;
}

ANN static m_int scan1_func_def_args(const Env env, Arg_List arg_list) { GWDEBUG_EXE
  do {
    if(!(arg_list->type = type_decl_resolve(env, arg_list->type_decl)))
      CHECK_BB(type_unknown(arg_list->type_decl->xid, "function argument"))
    if(arg_list->type_decl->types)
      ADD_REF(arg_list->type)
  } while((arg_list = arg_list->next));
  return 1;
}

ANN m_bool scan1_stmt_fptr(const Env env, const Stmt_Ptr ptr) { GWDEBUG_EXE
  if(ptr->type->array)
    CHECK_BB(check_array_empty(ptr->type->array, "function pointer"))
  if(!(ptr->ret_type = type_decl_resolve(env, ptr->type)))
    CHECK_BB(type_unknown(ptr->type->xid, "func pointer definition"))
  if(!env->class_def && GET_FLAG(ptr, ae_flag_static))
    CHECK_BB(err_msg(SCAN1_, ptr->pos,
          "can't declare func pointer static outside of a class"))
  if(ptr->args && scan1_func_def_args(env, ptr->args) < 0)
    CHECK_BB(err_msg(SCAN1_, ptr->pos,
          "\t... in typedef '%s'...", s_name(ptr->xid)))
  return 1;
}

ANN static m_bool scan1_stmt_type(const Env env, const Stmt_Typedef stmt) { GWDEBUG_EXE
  return stmt->m_type->def ? scan1_class_def(env, stmt->m_type->def) : 1;
}

ANN static m_bool scan1_stmt_union_array(const Array_Sub array) { GWDEBUG_EXE
  if(array->exp_list)
    CHECK_BB(err_msg(SCAN1_, array->pos,
      "array declaration must be empty in union."))
  return 1;
}

ANN m_bool scan1_stmt_union(const Env env, const Stmt_Union stmt) { GWDEBUG_EXE
  Decl_List l = stmt->l;

  if(stmt->xid) {
    UNSET_FLAG(stmt, ae_flag_private);
    env_push_class(env, stmt->value->m_type);
  }
  while(l) {
    Var_Decl_List list = l->self->d.exp_decl.list;

    if(l->self->exp_type != ae_exp_decl)
      CHECK_BB(err_msg(SCAN1_, stmt->pos,
            "invalid expression type '%i' in union declaration."))
    SET_FLAG(l->self->d.exp_decl.type, ae_flag_checked | stmt->flag);
    if(GET_FLAG(stmt, ae_flag_static))
      SET_FLAG(l->self->d.exp_decl.type, ae_flag_static);
    while(list) {
      const Var_Decl var_decl = list->self;
      if(var_decl->array)
        CHECK_BB(scan1_stmt_union_array(var_decl->array))
      list = list->next;
    }
    CHECK_BB(scan1_exp_decl(env, &l->self->d.exp_decl))
    l = l->next;
  }
  if(stmt->xid)
    env_pop_class(env);
  return 1;
}

ANN static m_bool scan1_stmt(const Env env, const Stmt stmt) { GWDEBUG_EXE
  m_bool ret = -1;

  switch(stmt->stmt_type) {
    case ae_stmt_exp:
      ret = stmt->d.stmt_exp.val ? scan1_exp(env, stmt->d.stmt_exp.val) : 1;
      break;
    case ae_stmt_code:
      SCOPE(ret = scan1_stmt_code(env, &stmt->d.stmt_code, 1))
      break;
    case ae_stmt_return:
      ret = scan1_stmt_return(env, &stmt->d.stmt_return);
      break;
    case ae_stmt_if:
      NSPC(ret = scan1_stmt_if(env, &stmt->d.stmt_if))
      break;
    case ae_stmt_while:
      NSPC(ret = scan1_stmt_flow(env, &stmt->d.stmt_while))
      break;
    case ae_stmt_for:
      NSPC(ret = scan1_stmt_for(env, &stmt->d.stmt_for))
      break;
    case ae_stmt_auto:
      NSPC(ret = scan1_stmt_auto(env, &stmt->d.stmt_auto))
      break;
    case ae_stmt_until:
      NSPC(ret = scan1_stmt_flow(env, &stmt->d.stmt_until))
      break;
    case ae_stmt_loop:
      NSPC(ret = scan1_stmt_loop(env, &stmt->d.stmt_loop))
      break;
    case ae_stmt_switch:
      NSPC(ret = scan1_stmt_switch(env, &stmt->d.stmt_switch))
      break;
    case ae_stmt_case:
      ret = scan1_stmt_case(env, &stmt->d.stmt_case);
      break;
    case ae_stmt_enum:
      ret = scan1_stmt_enum(env, &stmt->d.stmt_enum);
      break;
    case ae_stmt_continue:
    case ae_stmt_break:
    case ae_stmt_gotolabel:
      ret = 1;
      break;
    case ae_stmt_funcptr:
      ret = scan1_stmt_fptr(env, &stmt->d.stmt_ptr);
      break;
    case ae_stmt_typedef:
      ret = scan1_stmt_type(env, &stmt->d.stmt_type);
      break;
    case ae_stmt_union:
      ret = scan1_stmt_union(env, &stmt->d.stmt_union);
      break;
  }
  return ret;
}

ANN static m_bool scan1_stmt_list(const Env env, Stmt_List l) { GWDEBUG_EXE
  do CHECK_BB(scan1_stmt(env, l->stmt))
  while((l = l->next));
  return 1;
}

ANN static m_bool scan1_func_def_type(const Env env, const Func_Def f) { GWDEBUG_EXE
  if(f->type_decl->array)
    CHECK_BB(check_array_empty(f->type_decl->array, "function return"))
  if(!(f->ret_type = type_decl_resolve(env, f->type_decl)))
      CHECK_BB(type_unknown(f->type_decl->xid, "function return"))
  return 1;
}

ANN static m_bool scan1_func_def_op(const Func_Def f) { GWDEBUG_EXE
  m_int count = 0;
  Arg_List list = f->arg_list;
  while(list) {
    count++;
    list = list->next;
  }
  if(count > (GET_FLAG(f, ae_flag_unary) ? 1 : 2) || !count)
    CHECK_BB(err_msg(SCAN1_, f->pos,
          "operators can only have one or two arguments"))
  return 1;
}

ANN static m_bool scan1_func_def_flag(const Env env, const Func_Def f) { GWDEBUG_EXE
  if(GET_FLAG(f, ae_flag_dtor) && !env->class_def)
    CHECK_BB(err_msg(SCAN1_, f->pos, "dtor must be in class def!!"))
  if(GET_FLAG(f, ae_flag_op))
    CHECK_BB(scan1_func_def_op(f))
  return 1;
}

ANN static m_bool scan1_func_def_code(const Env env, const Func_Def f) { GWDEBUG_EXE
  m_bool ret;
  nspc_push_value(env->curr);
  ret = scan1_stmt_code(env, &f->code->d.stmt_code, 0);
  nspc_pop_value(env->curr);
  return ret;
}

ANN m_bool scan1_func_def(const Env env, const Func_Def f) { GWDEBUG_EXE
  if(GET_FLAG(f, ae_flag_private) && !env->class_def)
    CHECK_BB(err_msg(SCAN1_, f->pos, "can't declare func '%s' private outside of class", s_name(f->name)))
  if(tmpl_list_base(f->tmpl))
    return 1;
  env->func = FAKE_FUNC;
  f->stack_depth = 0;
  if(scan1_func_def_flag(env, f) < 0 ||
     scan1_func_def_type(env, f) < 0 ||
    (f->arg_list && scan1_func_def_args(env, f->arg_list) < 0) ||
    (f->code && scan1_func_def_code(env, f) < 0))
    CHECK_BB(err_msg(SCAN1_, f->pos, "\t...in function '%s'", s_name(f->name)))
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
  Class_Body body = class_def->body;

  if(tmpl_class_base(class_def->tmpl))
    return 1;
  if(class_def->ext) {
    if(!(class_def->type->parent = type_decl_resolve(env, class_def->ext)))
      CHECK_BB(type_unknown(class_def->ext->xid, "child class definition"))
    if(!GET_FLAG(class_def->type->parent, ae_flag_scan1) && class_def->type->parent->def)
      CHECK_BB(scan1_class_def(env, class_def->type->parent->def))
    if(class_def->ext->array) {
      if(!class_def->ext->array->exp_list)
        CHECK_BB(err_msg(SCAN1_, class_def->pos, "can't use empty []'s in class extend"))
      CHECK_BB(scan1_exp(env, class_def->ext->array->exp_list))
    }
    if(type_ref(class_def->type->parent))
        CHECK_BB(err_msg(SCAN1_, class_def->pos, "can't use ref type in class extend"))
  }
  CHECK_BB(env_push_class(env, class_def->type))
  while(body) {
    CHECK_BB(scan1_section(env, body->section))
    body = body->next;
  }
  SET_FLAG(class_def->type, ae_flag_scan1);
  return env_pop_class(env);
}

ANN m_bool scan1_ast(const Env env, Ast ast) { GWDEBUG_EXE
  do CHECK_BB(scan1_section(env, ast->section))
  while((ast = ast->next));
  return 1;
}
