#include "string.h"
#include "err_msg.h"
#include "absyn.h"
#include "context.h"
#include "type.h"
#include "func.h"

static m_bool isres(S_Symbol xid, m_uint pos) {
  m_str s = s_name(xid);
  if(!strcmp(s, "this") || !strcmp(s, "now") || !name2op(s)) {
    err_msg(TYPE_, 0, "%s is reserved.", s_name(xid));
    return 1;
  }
  return -1;
}

m_bool scan0_class_def(Env env, Class_Def class_def);
static m_bool scan0_Stmt_Typedef(Env env, Stmt_Ptr ptr) {
  Value v;
  m_str name = s_name(ptr->xid);
  Type type;
  Type t = new_type(te_func_ptr, name, &t_func_ptr);
  t->owner = env->curr;
  t->array_depth = 0;
  t->size = SZ_INT;
  t->info = new_nspc(name, env->context->filename);
  nspc_add_type(env->curr, ptr->xid, t);
  type = type_copy(env, &t_class);
  type->d.actual_type = t;
  v = new_value(type, name);
  v->owner = env->curr;
  SET_FLAG(v, ae_flag_const | ae_flag_checked);
  nspc_add_value(env->curr, ptr->xid, v);
  ptr->value = v;
  return 1;
}

static m_bool scan0_Stmt(Env env, Stmt stmt) {
  if(!stmt)
    return 1;
  if(stmt->type == ae_stmt_funcptr)
    CHECK_BB(scan0_Stmt_Typedef(env, &stmt->d.stmt_ptr))
    return 1;
}

static m_bool scan0_Stmt_List(Env env, Stmt_List list) {
  Stmt_List curr = list;
  while(curr) {
    CHECK_BB(scan0_Stmt(env, curr->stmt))
    curr = curr->next;
  }
  return 1;
}

static m_bool scan0_class_def_public(Env env, Class_Def class_def) {
  if(class_def->decl == ae_flag_public) {
    if(env->context->public_class_def) {
      CHECK_BB(err_msg(SCAN0_, class_def->pos,
                       "more than one 'public' class defined..."))
    }
    class_def->home = env->global_nspc; // migth be user
    env->context->public_class_def = class_def;
    vector_add(&env->nspc_stack, (vtype)env->curr);
    env->curr = class_def->home;
  }
  return 1;
}

static m_bool scan0_class_def_pre(Env env, Class_Def class_def) {
  CHECK_BB(scan0_class_def_public(env, class_def))
  if(nspc_lookup_type(env->curr, class_def->name->xid, 1)) {
    CHECK_BB(err_msg(SCAN0_,  class_def->name->pos,
                     "class/type '%s' is already defined in namespace '%s'",
                     s_name(class_def->name->xid), env->curr->name))
  }

  if(isres(class_def->name->xid, class_def->name->pos) > 0) {
    CHECK_BB(err_msg(SCAN0_, class_def->name->pos, "...in class definition: '%s' is reserved",
                     s_name(class_def->name->xid)))
  }
  return 1;
}

static Type scan0_class_def_init(Env env, Class_Def class_def) {
  Type the_class = new_type(env->type_xid++, s_name(class_def->name->xid), &t_object);
  the_class->owner = env->curr;
  the_class->array_depth = 0;
  the_class->size = SZ_INT;
  the_class->info = new_nspc(the_class->name, env->context->filename);
  the_class->info->parent = env->context->public_class_def == class_def ?
    env->context->nspc : env->curr;
  the_class->d.func = NULL;
  the_class->def = class_def;
  the_class->info->pre_ctor = new_vm_code(NULL, 0, 0, the_class->name, "[in code ctor definition]");
  nspc_add_type(env->curr, insert_symbol(the_class->name), the_class);
  if(class_def->types)
    SET_FLAG(the_class, ae_flag_template);
  return the_class;
}

static m_bool scan0_class_def_body(Env env, Class_Body body) {
  while(body) {
    switch(body->section->type) {
      case ae_section_stmt:
        CHECK_BB(scan0_Stmt_List(env, body->section->d.stmt_list))
      case ae_section_func:
        break;
      case ae_section_class:
        CHECK_BB(scan0_class_def(env, body->section->d.class_def))
        break;
    }
    body = body->next;
  }
  return 1;
}

static m_bool scan0_class_def_post(Env env, Class_Def class_def) {
  Value value;
  Type  type;
  type = type_copy(env, &t_class);
  type->d.actual_type = class_def->type;
  value = new_value(type, class_def->type->name);
  value->owner = env->curr;
  SET_FLAG(value, ae_flag_const | ae_flag_checked);
  nspc_add_value(env->curr, insert_symbol(class_def->type->name), value);
  if(class_def->home)
    env->curr = (Nspc)vector_pop(&env->nspc_stack);
  else
    class_def->home = env->curr;
  return 1;
}

m_bool scan0_class_def(Env env, Class_Def class_def) {
  m_bool ret;

  CHECK_BB(scan0_class_def_pre(env, class_def))
  CHECK_OB((class_def->type = scan0_class_def_init(env, class_def)))
  CHECK_BB(env_push_class(env, class_def->type))
  ret = scan0_class_def_body(env, class_def->body);
  CHECK_BB(env_pop_class(env))
  CHECK_BB(scan0_class_def_post(env, class_def))
  return ret;
}

m_bool scan0_Ast(Env env, Ast prog) {
  CHECK_OB(prog)
  while(prog) {
    switch(prog->section->type) {
      case ae_section_stmt:
        CHECK_BB(scan0_Stmt_List(env, prog->section->d.stmt_list))
        break;
      case ae_section_func:
        break;
      case ae_section_class:
        CHECK_BB(scan0_class_def(env, prog->section->d.class_def))
        break;
    }
    prog = prog->next;
  }
  return 1;
}
#include "traverse.h"
static m_bool scan1_exp(Env env, Exp exp);
static m_bool scan1_stmt_list(Env env, Stmt_List list);
m_bool scan1_class_def(Env env, Class_Def class_def);
static m_bool scan1_stmt(Env env, Stmt stmt);

static m_bool scan1_exp_decl_template(Env env, Type t, Exp_Decl* decl) {
  if(GET_FLAG(t, ae_flag_template)) {
    if(!decl->types)
      CHECK_BB(err_msg(SCAN1_, decl->pos, "you must provide template types"))
    decl->base = t->def;
    Class_Def a = template_class(env, t->def, decl->types);
    CHECK_BB(template_push_types(env, t->def->types, decl->types))
    CHECK_BB(scan0_class_def(env, a))
    SET_FLAG(a->type, ae_flag_template);
    CHECK_BB(scan1_class_def(env, a))
    decl->m_type = a->type;
    a->tref = t->def->types;
    a->base = decl->types;
  } else if(decl->types)
      CHECK_BB(err_msg(SCAN1_, decl->pos, "type '%s' is not template", t->name))
  if(env->class_def && GET_FLAG(env->class_def, ae_flag_template))
    decl->num_decl = 0;
  return 1;
}

static Type scan1_exp_decl_type(Env env, Exp_Decl* decl) {
  Type t = find_type(env, decl->type->xid);
  if(!t)
    CHECK_BO(err_msg(SCAN1_, decl->pos, "type '%s' unknown in declaration ",
          s_name(decl->type->xid->xid)))
  if(!t->size)
    CHECK_BO(err_msg(SCAN2_, decl->pos,
          "cannot declare variables of size '0' (i.e. 'void')..."))
  if(!GET_FLAG(decl->type, ae_flag_ref)) {
    if(env->class_def && (t == env->class_def) && !env->class_scope)
      CHECK_BO(err_msg(SCAN2_, decl->pos,
            "...(note: object of type '%s' declared inside itself)", t->name))
  } else if((isprim(t) > 0))
    CHECK_BO(err_msg(SCAN2_, decl->pos,
          "cannot declare references (@) of primitive type '%s'...\n"
          "\t...(primitive types: 'int', 'float', 'time', 'dur')", t->name))
  return t;
}

m_bool scan1_exp_decl(Env env, Exp_Decl* decl) {
  Var_Decl_List list = decl->list;
  Var_Decl var_decl = NULL;
  Type t = scan1_exp_decl_type(env, decl);

  CHECK_OB(t)
  CHECK_BB(scan1_exp_decl_template(env, t, decl))
  if(decl->m_type)
    t = decl->m_type;
  while(list) {
    Value value;
    var_decl = list->self;
    if(isres(list->self->xid, list->self->pos) > 0)
      CHECK_BB(err_msg(SCAN2_, list->self->pos,
            "\t... in variable declaration", s_name(list->self->xid)))
    if((value = nspc_lookup_value(env->curr, list->self->xid, 0)) &&
      !(env->class_def && GET_FLAG(env->class_def, ae_flag_template)))
        CHECK_BB(err_msg(SCAN2_, list->self->pos,
              "variable %s has already been defined in the same scope...", s_name(list->self->xid)))
    decl->num_decl++;
    if(var_decl->array) {
      Type t2 = t;
      CHECK_BB(verify_array(var_decl->array))
      if(var_decl->array->exp_list)
        CHECK_BB(scan1_exp(env, var_decl->array->exp_list))
      t = new_array_type(env, list->self->array->depth, t2, env->curr);
      if(!list->self->array->exp_list)
        SET_FLAG(decl->type, ae_flag_ref);
    }
    list->self->value = new_value(t, s_name(list->self->xid));
    if(GET_FLAG(decl->type, ae_flag_const)) {
      SET_FLAG(list->self->value, ae_flag_const);
      SET_FLAG(list->self->value, ae_flag_uconst);
    }
    if(env->class_def && !env->class_scope && !env->func && !decl->is_static)
      SET_FLAG(list->self->value, ae_flag_member);
    if(!env->class_def && !env->func && !env->class_scope)
      SET_FLAG(list->self->value, ae_flag_global);
    if(env->func == (Func)1)
      ADD_REF(list->self->value)
    list->self->value->ptr = list->self->addr;
    list->self->value->owner = env->curr;
    list->self->value->owner_class = env->func ? NULL : env->class_def;
    nspc_add_value(env->curr, list->self->xid, list->self->value);
    list = list->next;
  }
  if(decl->types)
    nspc_pop_type(env->curr);
  if(!decl->m_type)
    decl->m_type = t;
  return 1;
}

static m_bool scan1_exp_binary(Env env, Exp_Binary* binary) {
  CHECK_BB(scan1_exp(env, binary->lhs))
  CHECK_BB(scan1_exp(env, binary->rhs))
  return 1;
}

static m_bool scan1_exp_primary(Env env, Exp_Primary* prim) {
  if(prim->type == ae_primary_hack)
    CHECK_BB(scan1_exp(env, prim->d.exp))
    return 1;
}

static m_bool scan1_exp_array(Env env, Exp_Array* array) {
  CHECK_BB(verify_array(array->indices))
  CHECK_BB(scan1_exp(env, array->base))
  CHECK_BB(scan1_exp(env, array->indices->exp_list))
  return 1;
}

static m_bool scan1_exp_cast(Env env, Exp_Cast* cast) {
  CHECK_BB(scan1_exp(env, cast->exp))
  return 1;
}

static m_bool scan1_exp_postfix(Env env, Exp_Postfix* postfix) {
  CHECK_BB(scan1_exp(env, postfix->exp))
  if(postfix->exp->meta != ae_meta_var)
    CHECK_BB(err_msg(SCAN1_, postfix->exp->pos,
          "postfix operator '%s' cannot be used"
          " on non-mutable data-type...", op2str(postfix->op)))
  return 1;
}

static m_bool scan1_exp_dur(Env env, Exp_Dur* dur) {
  CHECK_BB(scan1_exp(env, dur->base))
  CHECK_BB(scan1_exp(env, dur->unit))
  return 1;
}

static m_bool scan1_exp_call(Env env, Exp_Func* exp_func) {
  Exp args = exp_func->args;
  if(exp_func->types)
    return 1;
  CHECK_BB(scan1_exp(env, exp_func->func))
  CHECK_BB(args && scan1_exp(env, args))
  return 1;
}

static m_bool scan1_exp_dot(Env env, Exp_Dot* member) {
  CHECK_BB(scan1_exp(env, member->base));
  return 1;
}

static m_bool scan1_exp_if(Env env, Exp_If* exp_if) {
  CHECK_BB(scan1_exp(env, exp_if->cond))
  CHECK_BB(scan1_exp(env, exp_if->if_exp))
  CHECK_BB(scan1_exp(env, exp_if->else_exp))
  return 1;
}

static m_bool scan1_exp_spork(Env env, Stmt code) {
  Func f = env->func;
  env->func = f ? f: (Func)2;
  CHECK_BB(scan1_stmt(env, code))
  env->func = f;
  return 1;
}

static m_bool scan1_exp(Env env, Exp exp) {
  Exp curr = exp;
  while(curr) {
    switch(curr->exp_type) {
      case ae_exp_primary:
        CHECK_BB(scan1_exp_primary(env, &curr->d.exp_primary))
        break;
      case ae_exp_decl:
        CHECK_BB(scan1_exp_decl(env, &curr->d.exp_decl))
        break;
      case ae_exp_unary:
        if(curr->d.exp_unary.op == op_spork && curr->d.exp_unary.code)
          CHECK_BB(scan1_exp_spork(env, curr->d.exp_unary.code))
        break;
      case ae_exp_binary:
        CHECK_BB(scan1_exp_binary(env, &curr->d.exp_binary))
        break;
      case ae_exp_postfix:
        CHECK_BB(scan1_exp_postfix(env, &curr->d.exp_postfix))
        break;
      case ae_exp_cast:
        CHECK_BB(scan1_exp_cast(env, &curr->d.exp_cast))
        break;
      case ae_exp_call:
        CHECK_BB(scan1_exp_call(env, &curr->d.exp_func))
        break;
      case ae_exp_array:
        CHECK_BB(scan1_exp_array(env, &curr->d.exp_array))
        break;
      case ae_exp_dot:
        CHECK_BB(scan1_exp_dot(env, &curr->d.exp_dot))
        break;
      case ae_exp_dur:
        CHECK_BB(scan1_exp_dur(env, &curr->d.exp_dur))
        break;
      case ae_exp_if:
        CHECK_BB(scan1_exp_if(env, &curr->d.exp_if))
        break;
    }
    curr = curr->next;
    if(curr&& curr->exp_type == ae_exp_decl)
      CHECK_BB(err_msg(SCAN1_, curr->pos, "can't declare after expression"))
  }
  return 1;
}

static m_bool scan1_stmt_code(Env env, Stmt_Code stmt, m_bool push) {
  m_bool ret;
  env->class_scope++;
  if(push)
    nspc_push_value(env->curr);
  ret = scan1_stmt_list(env, stmt->stmt_list);
  if(push)
    nspc_pop_value(env->curr);
  env->class_scope--;
  return ret;
}

static m_bool scan1_stmt_return(Env env, Stmt_Return stmt) {
  return stmt->val ? scan1_exp(env, stmt->val) : 1;
}

static m_bool scan1_stmt_flow(Env env, struct Stmt_Flow_* stmt) {
  CHECK_BB(scan1_exp(env, stmt->cond))
  CHECK_BB(scan1_stmt(env, stmt->body))
  return 1;
}

static m_bool scan1_stmt_for(Env env, Stmt_For stmt) {
  CHECK_BB(scan1_stmt(env, stmt->c1))
  CHECK_BB(scan1_stmt(env, stmt->c2))
  CHECK_BB(scan1_exp(env, stmt->c3))
  CHECK_BB(scan1_stmt(env, stmt->body))
  return 1;
}

static m_bool scan1_stmt_loop(Env env, Stmt_Loop stmt) {
  CHECK_BB(scan1_exp(env, stmt->cond))
  CHECK_BB(scan1_stmt(env, stmt->body))
  return 1;
}

static m_bool scan1_stmt_switch(Env env, Stmt_Switch stmt) {
  return scan1_exp(env, stmt->val) < 0 ? -1 : 1;
}

static m_bool scan1_stmt_case(Env env, Stmt_Case stmt) {
  return scan1_exp(env, stmt->val) < 0 ? -1 : 1;
}

static m_bool scan1_stmt_if(Env env, Stmt_If stmt) {
  CHECK_BB(scan1_exp(env, stmt->cond))
  CHECK_BB(scan1_stmt(env, stmt->if_body))
  if(stmt->else_body)
    CHECK_BB(scan1_stmt(env, stmt->else_body))
    return 1;
}

static m_bool scan1_stmt_enum(Env env, Stmt_Enum stmt) {
  Nspc nspc = env->class_def ? env->class_def->info : env->curr;
  Type t = NULL;
  ID_List list = stmt->list;
  m_uint count = list ? 1 : 0;
  if(stmt->xid) {
    if(nspc_lookup_type(nspc, stmt->xid, 1)) {
      CHECK_BB(err_msg(SCAN1_, stmt->pos, "type '%s' already declared", s_name(stmt->xid)))
    }
    if(nspc_lookup_value(env->curr, stmt->xid, 1)) {
      CHECK_BB(err_msg(SCAN1_, stmt->pos, "'%s' already declared as variable", s_name(stmt->xid)))
    }
  }
  t = type_copy(env, &t_int);
  t->name = stmt->xid ? s_name(stmt->xid) : "int";
  t->parent = &t_int;
  nspc_add_type(nspc, stmt->xid, t);
  while(list) {
    Value v;
    if(nspc_lookup_value(nspc, list->xid, 0)) {
      CHECK_BB(err_msg(SCAN1_, stmt->pos, "in enum argument %i '%s' already declared as variable", count, s_name(list->xid)))
    }
    v = new_value(t, s_name(list->xid));
    if(env->class_def) {
      v->owner_class = env->class_def;
      SET_FLAG(v, ae_flag_static);
    }
    SET_FLAG(v, ae_flag_const | ae_flag_enum | ae_flag_checked);
    nspc_add_value(nspc, list->xid, v);
    vector_add(&stmt->values, (vtype)v);
    list = list->next;
    count++;
  }
  return 1;
}

static m_int scan1_func_def_args(Env env, Arg_List arg_list) {
  m_int count = 0;
  while(arg_list) {
    count++;
    if(!(arg_list->type = find_type(env, arg_list->type_decl->xid))) {
      char path[id_list_len(arg_list->type_decl->xid)];
      type_path(path, arg_list->type_decl->xid);
      CHECK_BB(err_msg(SCAN1_, arg_list->pos,
              "'%s' unknown type in argument %i",
              path, count))
    }
    arg_list = arg_list->next;
  }
  return count;
}

static m_bool scan1_stmt_typedef(Env env, Stmt_Ptr ptr) {
  ptr->ret_type = find_type(env, ptr->type->xid);
  if(!ptr->ret_type)
    CHECK_BB(err_msg(SCAN1_, ptr->pos, 
          "unknown type '%s' in func ptr declaration",  s_name(ptr->xid)))
  if(!env->class_def && GET_FLAG(ptr, ae_flag_static))
    CHECK_BB(err_msg(SCAN1_, ptr->pos,
          "can't declare func pointer static outside of a class"))
  if(scan1_func_def_args(env, ptr->args) < 0)
    CHECK_BB(err_msg(SCAN1_, ptr->pos,
          "\t... in typedef '%s'...", s_name(ptr->xid)))
  return 1;
}

static m_bool scan1_stmt_union_array(Array_Sub array) {
  CHECK_BB(verify_array(array))
  if(array->exp_list)
    CHECK_BB(err_msg(SCAN1_, array->pos,
      "array declaration must be empty in union."))
  return 1;
}

static m_bool scan1_stmt_union(Env env, Stmt_Union stmt) {
  Decl_List l = stmt->l;

  while(l) {
    Var_Decl_List list = l->self->d.exp_decl.list;

    if(l->self->exp_type != ae_exp_decl)
      CHECK_BB(err_msg(SCAN1_, stmt->pos,
            "invalid expression type '%i' in union declaration."))
    while(list) {
      Var_Decl var_decl = list->self;
      if(var_decl->array)
        CHECK_BB(scan1_stmt_union_array(var_decl->array))
      list = list->next;
    }
    CHECK_BB(scan1_exp_decl(env, &l->self->d.exp_decl))
    l = l->next;
  }
  return 1;
}

static m_bool scan1_stmt(Env env, Stmt stmt) {
  m_bool ret = -1;

  switch(stmt->type) {
    case ae_stmt_exp:
      ret = scan1_exp(env, stmt->d.stmt_exp.val);
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
      ret = scan1_stmt_typedef(env, &stmt->d.stmt_ptr);
      break;
    case ae_stmt_union:
      ret = scan1_stmt_union(env, &stmt->d.stmt_union);
      break;
  }
  return ret;
}

static m_bool scan1_stmt_list(Env env, Stmt_List list) {
  Stmt_List curr = list;
  while(curr) {
    CHECK_BB(scan1_stmt(env, curr->stmt))
    curr = curr->next;
  }
  return 1;
}

static m_int scan1_func_def_array(Env env, Func_Def f) {
  Type t = NULL;
  Type t2 = f->ret_type;

  CHECK_BB(verify_array(f->type_decl->array))
  if(f->type_decl->array->exp_list) {
    free_expression(f->type_decl->array->exp_list);
    CHECK_BB(err_msg(SCAN1_, f->type_decl->array->pos,
      "in function '%s':\n\treturn array type must be defined with empty []'s", s_name(f->name)))
  }
  t = new_array_type(env, f->type_decl->array->depth, t2, env->curr);
  SET_FLAG(f->type_decl, ae_flag_ref);
  f->ret_type = t;
  return 1;
}

static m_bool scan1_func_def_type(Env env, Func_Def f) {
  f->ret_type = find_type(env, f->type_decl->xid);
  if(!f->ret_type)
    CHECK_BB(err_msg(SCAN1_, f->pos, "unknown return type '%s'",
                     s_name(f->type_decl->xid->xid)))
  if(f->type_decl->array)
    CHECK_BB(scan1_func_def_array(env, f))
  return 1;
}

static m_bool scan1_func_def_op(Env env, Func_Def f) {
  m_int count = 0;
  Arg_List list = f->arg_list;
  while(list) {
    count++;
    list = list->next;
  }
  if(count > 2 || !count)
    CHECK_BB(err_msg(SCAN1_, f->pos,
          "operators can only have one or two arguments"))
  if(name2op(s_name(f->name)) < 0)
    CHECK_BB(err_msg(SCAN1_, f->pos,
          "%s is not a valid operator name", s_name(f->name)))
  return 1;
}

static m_bool scan1_func_def_flag(Env env, Func_Def f) {
  if(GET_FLAG(f, ae_flag_dtor) && !env->class_def)
    CHECK_BB(err_msg(SCAN1_, f->pos, "dtor must be in class def!!"))
  if(GET_FLAG(f, ae_flag_op))
    CHECK_BB(scan1_func_def_op(env, f))
  else if(name2op(s_name(f->name)) > 0)
      CHECK_BB(err_msg(SCAN1_, f->pos,
                "'%s' is a reserved operator name", s_name(f->name)))
  return 1;
}

m_bool scan1_func_def(Env env, Func_Def f) {
  if(f->types)
    return 1;
  env->func = (Func)1;
  if( scan1_func_def_flag(env, f) < 0 ||
      scan1_func_def_type(env, f) < 0 ||
    (f->arg_list && scan1_func_def_args(env, f->arg_list) < 0) ||
    (f->code && scan1_stmt_code(env, &f->code->d.stmt_code, 0) < 0))
    CHECK_BB(err_msg(SCAN1_, f->pos, "\t...in function '%s'", s_name(f->name)))
  env->func = NULL;
  return 1;
}

static m_bool scan1_section(Env env, Section* section) {
  ae_Section_Type t = section->type;
  if(t == ae_section_stmt)
    CHECK_BB(scan1_stmt_list(env, section->d.stmt_list))
  else if(t == ae_section_func)
    CHECK_BB(scan1_func_def(env, section->d.func_def))
  else if(t == ae_section_class)
    CHECK_BB(scan1_class_def(env, section->d.class_def))
  return 1;
}

m_bool scan1_class_def(Env env, Class_Def class_def) {
  m_bool ret = 1;
  Class_Body body = class_def->body;

  if(class_def->types)
    return 1;
  CHECK_BB(env_push_class(env, class_def->type))
  while(body && ret > 0) {
    ret = scan1_section(env, body->section);
    body = body->next;
  }
  CHECK_BB(env_pop_class(env))
  return ret;
}

m_bool scan1_ast(Env env, Ast ast) {
  Ast prog = ast;
  while(prog) {
    CHECK_BB(scan1_section(env, ast->section))
    prog = prog->next;
  }
  return 1;
}

static m_bool scan2_exp(Env env, Exp exp);
static m_bool scan2_stmt(Env env, Stmt stmt);
static m_bool scan2_stmt_list(Env env, Stmt_List list);
m_bool scan2_class_def(Env env, Class_Def class_def);

static m_bool scan2_exp_decl_template(Env env, Exp_Decl* decl) {
  Type type = decl->m_type;
  if(env->class_def && GET_FLAG(env->class_def, ae_flag_template))
    template_push_types(env, env->class_def->def->types, decl->types);
  if(GET_FLAG(type, ae_flag_template)) {
    template_push_types(env, decl->base->types, decl->types);
    CHECK_BB(scan1_class_def(env, type->def))
    CHECK_BB(scan2_class_def(env, type->def))
  }
  return 1;
}

m_bool scan2_exp_decl(Env env, Exp_Decl* decl) {
  Var_Decl_List list = decl->list;
  Type type = decl->m_type;

  if(isa(type, &t_shred) > 0)
    SET_FLAG(decl->type, ae_flag_ref);
  CHECK_BB(scan2_exp_decl_template(env, decl))
  while(list) {
    if(list->self->array && list->self->array->exp_list)
        CHECK_BB(scan2_exp(env, list->self->array->exp_list))
    list = list->next;
  }
  if(GET_FLAG(type, ae_flag_template)  || (env->class_def && GET_FLAG(env->class_def, ae_flag_template)))
    nspc_pop_type(env->curr);
  return 1;
}

static m_bool scan2_arg_def_check(Arg_List list) {
  if(list->var_decl->value) {
    if(list->var_decl->value->m_type->array_depth)
      REM_REF(list->var_decl->value->m_type->d.array_type)
      list->var_decl->value->m_type = list->type;
  }
  if(!list->type->size)
    CHECK_BB(err_msg(SCAN2_, list->pos, "cannot declare variables of size '0' (i.e. 'void')..."))
  if(isres(list->var_decl->xid, list->pos) > 0)
    return -1;
  if((isprim(list->type) > 0) && GET_FLAG(list->type_decl, ae_flag_ref))
    CHECK_BB(err_msg(SCAN2_, list->type_decl->pos,
                  "cannot declare references (@) of primitive type '%s'...\n"
                  "\t...(primitive types: 'int', 'float', 'time', 'dur')", list->type->name))
  return 1;
}

static m_bool scan2_arg_def_array(Env env, Arg_List list) {
  CHECK_BB(verify_array(list->var_decl->array))
  if(list->var_decl->array->exp_list)
    CHECK_BB(err_msg(SCAN2_, list->pos,
                  "\t'%s': function arguments must be defined with empty []'s",
                  s_name(list->var_decl->xid)))
  list->type  = new_array_type(env, list->var_decl->array->depth, 
      list->type, env->curr);
  SET_FLAG(list->type, ae_flag_ref);
  return 1;
}

static m_bool scan2_arg_def(Env env, Func_Def f, Arg_List list) {
  m_uint count = 1;
  nspc_push_value(env->curr);
  while(list) {
    Value v;
    if(scan2_arg_def_check(list) < 0 || 
        (list->var_decl->array && scan2_arg_def_array(env, list) < 0)) {
      nspc_pop_value(env->curr);
      return -1;
    }
    v = list->var_decl->value ? list->var_decl->value : new_value(list->type, s_name(list->var_decl->xid));
    v->owner = env->curr;
    SET_FLAG(v, ae_flag_arg);
    if(GET_FLAG(list->type_decl, ae_flag_const))
      SET_FLAG(v, ae_flag_const | ae_flag_uconst);
    if(f) {
      v->offset = f->stack_depth;
      f->stack_depth += list->type->size;
    }
    nspc_add_value(env->curr, list->var_decl->xid, v);
    list->var_decl->value = v;
    count++;
    list = list->next;
  }
  nspc_pop_value(env->curr);
  return 1;
}

static Value scan2_func_assign(Env env, Func_Def d, Func f, Value v) {
  v->owner = env->curr;
  if(GET_FLAG(f, ae_flag_member))
    SET_FLAG(v, ae_flag_member);
  if(!(v->owner_class = env->class_def))
    SET_FLAG(v, ae_flag_global);
  d->d.func = v->func_ref = f;
  return f->value_ref = v;
}

static m_bool scan2_stmt_typedef(Env env, Stmt_Ptr ptr) {
  if(nspc_lookup_func(env->curr, ptr->xid, -1))
    CHECK_BB(err_msg(SCAN2_, ptr->pos, "function type '%s' already defined.", s_name(ptr->xid)))
    if(scan2_arg_def(env, NULL, ptr->args) < 0)
      CHECK_BB(err_msg(SCAN2_, ptr->pos, "in typedef '%s'", s_name(ptr->xid)))
      SET_FLAG(ptr->value, ae_flag_checked);
  nspc_add_value(env->curr, ptr->xid, ptr->value);

  Func_Def def = new_func_def(ae_flag_func | (!env->class_def ? ae_flag_func :
                              !GET_FLAG(ptr, ae_flag_static) ? ae_flag_instance : ae_flag_static), ptr->type, s_name(ptr->xid), ptr->args, NULL, ptr->pos);
  def->ret_type = ptr->ret_type;
  ptr->func = new_func(s_name(ptr->xid), def);
  ptr->value->func_ref = ptr->func;
  ptr->func->value_ref = ptr->value;
  if(env->class_def) {
    if(!GET_FLAG(ptr, ae_flag_static)) {
      SET_FLAG(ptr->value, ae_flag_member);
      SET_FLAG(ptr->func, ae_flag_member);
    }
    ptr->value->owner_class = env->class_def;
  }
  nspc_add_func(env->curr, ptr->xid, ptr->func);
  if(!GET_FLAG(ptr, ae_flag_static))
    ADD_REF(ptr->func);
  return 1;
}

static m_bool scan2_exp_primary(Env env, Exp_Primary* prim) {
  if(prim->type == ae_primary_hack)
    CHECK_BB(scan2_exp(env, prim->d.exp))
    return 1;
}

static m_bool scan2_exp_array(Env env, Exp_Array* array) {
  CHECK_BB(verify_array(array->indices))
  CHECK_BB(scan2_exp(env, array->base))
  CHECK_BB(scan2_exp(env, array->indices->exp_list))
  return 1;
}

static m_bool scan2_exp_binary(Env env, Exp_Binary* binary) {
  CHECK_BB(scan2_exp(env, binary->lhs))
  CHECK_BB(scan2_exp(env, binary->rhs))
  return 1;
}

static m_bool scan2_exp_cast(Env env, Exp_Cast* cast) {
  CHECK_BB(scan2_exp(env, cast->exp))
  return 1;
}

static m_bool scan2_exp_postfix(Env env, Exp_Postfix* postfix) {
  CHECK_BB(scan2_exp(env, postfix->exp))
  return 1;
}

static m_bool scan2_exp_dur(Env env, Exp_Dur* dur) {
  CHECK_BB(scan2_exp(env, dur->base))
  CHECK_BB(scan2_exp(env, dur->unit))
  return 1;
}

static m_bool scan2_exp_call1(Env env, Exp func, Exp args) {
  CHECK_BB(scan2_exp(env, func))
  CHECK_BB(scan2_exp(env, args))
  return 1;
}

static m_bool scan2_template_match(Env env, Value v, Type_List types) {
  m_uint i, match = -1;
  m_uint digit = num_digit(v->func_num_overloads);
  for(i = 0; i < v->func_num_overloads + 1; i++) {
    Value value;
    Type_List tlc = types;
    ID_List tld;
    char name[strlen(v->name) + strlen(env->curr->name) + digit + 13];

    sprintf(name, "%s<template>@%li@%s", v->name, i, env->curr->name);
    value = nspc_lookup_value(env->curr, insert_symbol(name), 1);
    if(!value)continue;
    tld = value->func_ref->def->types;
    while(tld) {
      if(!tlc)
        break;
      tld = tld->next;
      if(!tld && tlc->next)
        break;
      tlc = tlc->next;
    }
    if(!tlc && !tld)
      match = 1;
  }
  return match;
}

static m_bool scan2_exp_call(Env env, Exp_Func* exp_func) {
  if(exp_func->types) {
    if(exp_func->func->exp_type == ae_exp_primary) {
      Value v = nspc_lookup_value(env->curr, exp_func->func->d.exp_primary.d.var, 1);
      if(!v)
        CHECK_BB(err_msg(SCAN2_, exp_func->pos, "template call of non-existant function."))
      if(!v->func_ref)
        CHECK_BB(err_msg(SCAN2_, exp_func->pos, "template call of non-function value."))
      Func_Def base = v->func_ref->def;
      if(!base->types)
        CHECK_BB(err_msg(SCAN2_, exp_func->pos, "template call of non-template function."))
      Type_List list = exp_func->types;
      while(list) {
        Type t = find_type(env, list->list);
        if(!t)
          CHECK_BB(err_msg(SCAN1_, exp_func->pos, "type '%s' unknown in template call", s_name(list->list->xid)))
          list = list->next;
      }
      if(scan2_template_match(env, v, exp_func->types) < 0)
        CHECK_BB(err_msg(SCAN2_, exp_func->pos, "template type number mismatch."))
      return 1;
    } else if(exp_func->func->exp_type == ae_exp_dot) {
      return 1;      // see type.c
    } else {
      err_msg(SCAN2_, exp_func->pos, "unhandled expression type '%i' in template func call.", exp_func->func->exp_type);
      return -1;
    }
  }
  return scan2_exp_call1(env, exp_func->func, exp_func->args);
}

static m_bool scan2_exp_dot(Env env, Exp_Dot* member) {
  CHECK_BB(scan2_exp(env, member->base))
  return 1;
}

static m_bool scan2_exp_if(Env env, Exp_If* exp_if) {
  CHECK_BB(scan2_exp(env, exp_if->cond))
  CHECK_BB(scan2_exp(env, exp_if->if_exp))
  CHECK_BB(scan2_exp(env, exp_if->else_exp))
  return 1;
}

static m_bool scan2_exp_spork(Env env, Stmt code) {
  Func f = env->func;
  env->func = f ? f : (Func)1;
  CHECK_BB(scan2_stmt(env, code))
  env->func = f;
  return 1;
}

static m_bool scan2_exp(Env env, Exp exp) {
  Exp curr = exp;
  m_bool ret = 1;
  while(curr && ret > 0) {
    switch(curr->exp_type) {
      case ae_exp_primary:
        ret = scan2_exp_primary(env, &curr->d.exp_primary);
        break;
      case ae_exp_decl:
        ret = scan2_exp_decl(env, &curr->d.exp_decl);
        break;
      case ae_exp_unary:
        ret = (curr->d.exp_unary.op == op_spork && curr->d.exp_unary.code) ?
          scan2_exp_spork(env, curr->d.exp_unary.code) : 1;
        break;
      case ae_exp_binary:
        ret = scan2_exp_binary(env, &curr->d.exp_binary);
        break;
      case ae_exp_postfix:
        ret = scan2_exp_postfix(env, &curr->d.exp_postfix);
        break;
      case ae_exp_cast:
        ret = scan2_exp_cast(env, &curr->d.exp_cast);
        break;
      case ae_exp_call:
        ret = scan2_exp_call(env, &curr->d.exp_func);
        break;
      case ae_exp_array:
        ret = scan2_exp_array(env, &curr->d.exp_array);
        break;
      case ae_exp_dot:
        ret = scan2_exp_dot(env, &curr->d.exp_dot);
        break;
      case ae_exp_if:
        ret = scan2_exp_if(env, &curr->d.exp_if);
        break;
      case ae_exp_dur:
        ret = scan2_exp_dur(env, &curr->d.exp_dur);
        break;
    }
    curr = curr->next;
  }
  return ret;
}

static m_bool scan2_stmt_code(Env env, Stmt_Code stmt, m_bool push) {
  env->class_scope++;
  if(push)
    nspc_push_value(env->curr);
  m_bool t = scan2_stmt_list(env, stmt->stmt_list);
  if(push)
    nspc_pop_value(env->curr);
  env->class_scope--;
  return t;
}

static m_bool scan2_stmt_if(Env env, Stmt_If stmt) {
  CHECK_BB(scan2_exp(env, stmt->cond))
  CHECK_BB(scan2_stmt(env, stmt->if_body))
  if(stmt->else_body)
    CHECK_BB(scan2_stmt(env, stmt->else_body))
    return 1;
}

static m_bool scan2_stmt_flow(Env env, struct Stmt_Flow_* stmt) {
  CHECK_BB(scan2_exp(env, stmt->cond))
  CHECK_BB(scan2_stmt(env, stmt->body))
  return 1;
}

static m_bool scan2_stmt_for(Env env, Stmt_For stmt) {
  CHECK_BB(scan2_stmt(env, stmt->c1))
  CHECK_BB(scan2_stmt(env, stmt->c2))
  CHECK_BB(scan2_exp(env, stmt->c3))
  CHECK_BB(scan2_stmt(env, stmt->body))
  return 1;
}

static m_bool scan2_stmt_loop(Env env, Stmt_Loop stmt) {
  CHECK_BB(scan2_exp(env, stmt->cond))
  CHECK_BB(scan2_stmt(env, stmt->body))
  return 1;
}

static m_bool scan2_stmt_return(Env env, Stmt_Return stmt) {
    return stmt->val ? scan2_exp(env, stmt->val) : 1;
}

static m_bool scan2_stmt_switch(Env env, Stmt_Switch stmt) {
  CHECK_BB(scan2_exp(env, stmt->val))
  return 1;
}

static m_bool scan2_stmt_case(Env env, Stmt_Case stmt) {
  CHECK_BB(scan2_exp(env, stmt->val))
  return 1;
}

static Map scan2_label_map(Env env, Stmt_Goto_Label stmt) {
  Map m;
  m_uint* key = env->class_def && !env->func ?
    (m_uint*)env->class_def : (m_uint*)env->func;
  if(!env->context->label.ptr)
    map_init(&env->context->label);
  m = (Map)map_get(&env->context->label, (vtype)key);
   if(!m) {
    m = new_map();
    map_set(&env->context->label, (vtype)key, (vtype)m);
  }
  return m;
}

static m_bool scan2_stmt_gotolabel(Env env, Stmt_Goto_Label stmt) {
  if(stmt->is_label) {
    Map m = scan2_label_map(env, stmt);
    CHECK_OB(m)
    if(map_get(m, (vtype)stmt->name)) {
      Stmt_Goto_Label l = (Stmt_Goto_Label)map_get(m, (vtype)stmt->name);
      vector_release(&l->data.v);
      CHECK_BB(err_msg(SCAN2_, stmt->pos,
            "label '%s' already defined", s_name(stmt->name)))
    }
    map_set(m, (vtype)stmt->name, (vtype)stmt);
    vector_init(&stmt->data.v);
  }
  return 1;
}

static m_bool scan2_stmt_enum(Env env, Stmt_Enum stmt) {
  Value v = nspc_lookup_value(env->curr, stmt->xid, 1);
  if(v)
    CHECK_BB(err_msg(SCAN2_, stmt->pos,
          "'%s' already declared as variable", s_name(stmt->xid)))
  return 1;
}

static m_bool scan2_stmt_union(Env env, Stmt_Union stmt) {
  Decl_List l = stmt->l;
  while(l) {
    CHECK_BB(scan2_exp(env, l->self))
    l = l->next;
  }
  return 1;
}

static m_bool scan2_stmt(Env env, Stmt stmt) {
  m_bool ret = 1;
  if(!stmt)
    return 1;
  switch(stmt->type) {
    case ae_stmt_exp:
      ret = scan2_exp(env, stmt->d.stmt_exp.val);
      break;
    case ae_stmt_return:
      ret = scan2_stmt_return(env, &stmt->d.stmt_return);
      break;
    case ae_stmt_code:
      SCOPE(ret = scan2_stmt_code(env, &stmt->d.stmt_code, 1))
      break;
    case ae_stmt_if:
      NSPC(ret = scan2_stmt_if(env, &stmt->d.stmt_if))
      break;
    case ae_stmt_while:
      NSPC(ret = scan2_stmt_flow(env, &stmt->d.stmt_while))
      break;
    case ae_stmt_until:
      NSPC(ret = scan2_stmt_flow(env, &stmt->d.stmt_until))
      break;
    case ae_stmt_for:
      NSPC(ret = scan2_stmt_for(env, &stmt->d.stmt_for))
      break;
    case ae_stmt_loop:
      NSPC(ret = scan2_stmt_loop(env, &stmt->d.stmt_loop))
      break;
    case ae_stmt_switch:
      NSPC(ret = scan2_stmt_switch(env, &stmt->d.stmt_switch))
      break;
    case ae_stmt_case:
      ret = scan2_stmt_case(env, &stmt->d.stmt_case);
      break;
    case ae_stmt_gotolabel:
      ret = scan2_stmt_gotolabel(env, &stmt->d.stmt_gotolabel);
      break;
    case ae_stmt_continue:
    case ae_stmt_break:
      break;
    case ae_stmt_enum:
      ret = scan2_stmt_enum(env, &stmt->d.stmt_enum);
      break;
    case ae_stmt_funcptr:
      ret = scan2_stmt_typedef(env, &stmt->d.stmt_ptr);
      break;
    case ae_stmt_union:
      ret = scan2_stmt_union(env, &stmt->d.stmt_union);
      break;
  }
  return ret;
}

static m_bool scan2_stmt_list(Env env, Stmt_List list) {
  Stmt_List curr = list;
  while(curr) {
    CHECK_BB(scan2_stmt(env, curr->stmt))
    curr = curr->next;
  }
  return 1;
}

static m_bool scan2_func_def_overload(Func_Def f, Value overload) {
  if(isa(overload->m_type, &t_function) < 0)
    CHECK_BB(err_msg(SCAN2_, f->pos, "function name '%s' is already used by another value", s_name(f->name)))
  else if(!overload->func_ref)
    CHECK_BB(err_msg(SCAN2_, f->pos, "internal error: missing function '%s'", overload->name)) // LCOV_EXCL_LINE
  if((!GET_FLAG(overload, ae_flag_template) && f->types) ||
      (GET_FLAG(overload, ae_flag_template) && !f->types &&
       !GET_FLAG(f, ae_flag_template)))
    CHECK_BB(err_msg(SCAN2_, f->pos, "must override template function with template"))
  return 1;
}

 static m_bool scan2_func_def_template (Env env, Func_Def f, Value overload) {
  m_str func_name = s_name(f->name);
  Func func = new_func(func_name, f);
  Value value;
  m_uint len = strlen(func_name) + num_digit(overload ? overload->func_num_overloads + 1 : 0) +
               strlen(env->curr->name) + 13;

  char name[len];
  if(overload)
    func->next = overload->func_ref->next;
  if(env->class_def && !GET_FLAG(f, ae_flag_static))
    SET_FLAG(func, ae_flag_member);
  value = new_value(&t_function, func_name);
  CHECK_OB(scan2_func_assign(env, f, func, value))
  SET_FLAG(value, ae_flag_const | ae_flag_checked | ae_flag_template);
  if(overload)
    overload->func_num_overloads++;
  else {
    ADD_REF(value);
    ADD_REF(func);
    nspc_add_value(env->curr, f->name, value);
  }
  snprintf(name, len, "%s<template>@%li@%s", func_name,
           overload ? overload->func_num_overloads : 0, env->curr->name);
  nspc_add_value(env->curr, insert_symbol(name), value);
  return 1;
}

static m_bool scan2_func_def_builtin(Func func, m_str name) {
  SET_FLAG(func, ae_flag_builtin);
  func->code = new_vm_code(NULL, func->def->stack_depth, 1, name, "builtin func code");
  func->code->need_this = GET_FLAG(func, ae_flag_member);
  func->code->native_func = (m_uint)func->def->d.dl_func_ptr;
  return 1;
}

static m_bool scan2_func_def_op(Env env, Func_Def f) {
  m_bool ret = name2op(strtok(s_name(f->name), "@"));
  if(env->class_def)SET_FLAG(f->d.func, ae_flag_member); // 04/05/17
    CHECK_BB(env_add_op(env, ret, f->arg_list->var_decl->value->m_type,
        f->arg_list->next ? f->arg_list->next->var_decl->value->m_type : NULL, f->ret_type, NULL, 1))
  return 1;
}

static m_bool scan2_func_def_code(Env env, Func_Def f) {
  m_bool ret = 1;
  env->func = f->d.func;
  nspc_push_value(env->curr);
  if(scan2_stmt_code(env, &f->code->d.stmt_code, 0) < 0)
    ret = err_msg(SCAN2_, f->pos, "...in function '%s'", s_name(f->name));
  nspc_pop_value(env->curr);
  env->func = NULL;
  return ret;
}

static m_bool scan2_func_def_add(Env env, Value value, Value overload) {
  m_str name = s_name(value->func_ref->def->name);
  Func func = value->func_ref;

  SET_FLAG(value, ae_flag_checked);
  nspc_add_func(env->curr, insert_symbol(func->name), func); // template. is it necessary ?
  if(!overload)
    nspc_add_value(env->curr, value->func_ref->def->name, value);
  else {
    nspc_add_value(env->curr, insert_symbol(func->name), value);
    if(overload->func_ref->def->ret_type) // template func don't check ret_type case
      if(!GET_FLAG(func->def, ae_flag_template))
        if(func->def->ret_type->xid != overload->func_ref->def->ret_type->xid) {
          err_msg(SCAN2_,  func->def->pos, "function signatures differ in return type... '%s' and '%s'",
                  func->def->ret_type->name, overload->func_ref->def->ret_type->name);
          if(env->class_def)
            err_msg(SCAN2_, func->def->pos,
                    "function '%s.%s' matches '%s.%s' but cannot overload...",
                    env->class_def->name, name,
                    value->owner_class->name, name);
          return -1;
        }
  }
  return 1;
}

static void scan2_func_def_flag(Env env, Func_Def f) {
  SET_FLAG(f->d.func->value_ref, ae_flag_const);
  if(GET_FLAG(f, ae_flag_builtin))
    SET_FLAG(f->d.func->value_ref, ae_flag_builtin);
  if(GET_FLAG(f, ae_flag_dtor))
    SET_FLAG(f->d.func, ae_flag_dtor);
  else if(GET_FLAG(f, ae_flag_variadic))
    f->stack_depth += SZ_INT;
}

m_bool scan2_func_def(Env env, Func_Def f) {
  Type     type     = NULL;
  Value    value    = NULL;
  Func     func     = NULL;

  Value overload = nspc_lookup_value(env->curr,  f->name, 0);
  m_str func_name = s_name(f->name);
  m_uint len = strlen(func_name) + num_digit(overload ? overload->func_num_overloads + 1 : 0) +
               strlen(env->curr->name) + 3;

  char name[len];

  if(overload)
    CHECK_BB(scan2_func_def_overload(f, overload))

  if(f->types)
    return scan2_func_def_template(env, f, overload);

  snprintf(name, len, "%s@%li@%s", func_name,
           overload ? ++overload->func_num_overloads : 0, env->curr->name);

  func_name = s_name(insert_symbol(name));
  func = new_func(func_name, f);
  if(env->class_def && !GET_FLAG(f, ae_flag_static))
    SET_FLAG(func, ae_flag_member);
  if(GET_FLAG(f, ae_flag_builtin)) // actual builtin func import
    CHECK_BB(scan2_func_def_builtin(func, name))
  type = new_type(te_function, func_name, &t_function);
  type->size = SZ_INT;
  type->d.func = func;
  value = new_value(type, func_name);
  CHECK_OB(scan2_func_assign(env, f, func, value))
  scan2_func_def_flag(env, f);
  if(overload) {
    func->next = overload->func_ref->next;
    overload->func_ref->next = func;
  }
  if(isprim(f->ret_type) > 0 && GET_FLAG(f->type_decl, ae_flag_ref)) {
    err_msg(SCAN2_,  f->type_decl->pos, "FUNC cannot declare references (@) of primitive type '%s'...\n"
            "...(primitive types: 'int', 'float', 'time', 'dur')", f->ret_type->name);
    return -1;
  }

  f->stack_depth = GET_FLAG(func, ae_flag_member) ? SZ_INT : 0;

  if(scan2_arg_def(env, f, f->arg_list) < 0)
    CHECK_BB(err_msg(SCAN2_, f->pos, "\t... in function '%s'\n", s_name(f->name)))
  if(GET_FLAG(f, ae_flag_op))
    return scan2_func_def_op(env, f);

  CHECK_BB(scan2_func_def_add(env, value, overload))
  return f->code ? scan2_func_def_code(env, f) : 1;
}

static m_bool scan2_section(Env env, Section* section) {
  ae_Section_Type t = section->type;
  if(t == ae_section_stmt)
    CHECK_BB(scan2_stmt_list(env, section->d.stmt_list))
  else if(t == ae_section_func)
    CHECK_BB(scan2_func_def(env, section->d.func_def))
  else if(t == ae_section_class)
    CHECK_BB(scan2_class_def(env, section->d.class_def))
  return 1;
}

static m_bool scan2_class_def_body(Env env, Class_Body body) {
  while(body) {
    CHECK_BB(scan2_section(env, body->section))
    body = body->next;
  }
  return 1;
}

m_bool scan2_class_def(Env env, Class_Def class_def) {
  if(class_def->types)
    return 1;
  CHECK_BB(env_push_class(env, class_def->type))
  CHECK_BB(scan2_class_def_body(env, class_def->body))
  CHECK_BB(env_pop_class(env))
  return 1;
}

m_bool scan2_ast(Env env, Ast ast) {
  while(ast) {
    CHECK_BB(scan2_section(env, ast->section))
    ast = ast->next;
  }
  return 1;
}
