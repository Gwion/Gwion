#include "err_msg.h"
#include "absyn.h"
#include "type.h"
#include "func.h"

extern void free_expression(Exp exp);
static m_bool scan1_exp(Env env, Exp exp);
static m_bool scan1_stmt_list(Env env, Stmt_List list);
static m_bool scan1_stmt(Env env, Stmt stmt);

m_bool scan1_exp_decl(Env env, Exp_Decl* decl) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "decl ref:%i", decl->type->ref);
#endif
  Var_Decl_List list = decl->list;
  Var_Decl var_decl = NULL;
  Type t = find_type(env, decl->type->xid);
  if(!t) {
    err_msg(SCAN1_, decl->pos, "type '%s' unknown in declaration ", S_name(decl->type->xid->xid));
    return -1;
  }
  while(list) {
    var_decl = list->self;
    decl->num_decl++;
    if(var_decl->array) {
      CHECK_BB(verify_array(var_decl->array))
      if(var_decl->array->exp_list)
        CHECK_BB(scan1_exp(env, var_decl->array->exp_list))
      }
    list = list->next;
  }
  decl->m_type = t;
//  ADD_REF(t);
  return 1;
}

static m_bool scan1_exp_binary(Env env, Exp_Binary* binary) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1",  "binary");
#endif
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
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "array");
#endif
  CHECK_BB(verify_array(array->indices))
  CHECK_BB(scan1_exp(env, array->base))
  CHECK_BB(scan1_exp(env, array->indices->exp_list))
  return 1;
}

static m_bool scan1_exp_cast(Env env, Exp_Cast* cast) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "cast");
#endif
  CHECK_BB(scan1_exp(env, cast->exp))
  return 1;
}

static m_bool scan1_exp_postfix(Env env, Exp_Postfix* postfix) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "postfix");
#endif
  CHECK_BB(scan1_exp(env, postfix->exp))
  switch(postfix->op) {
  case op_plusplus:
  case op_minusminus:
    if(postfix->exp->meta != ae_meta_var) {
      err_msg(SCAN1_, postfix->exp->pos, "postfix operator '%s' cannot be used on non-mutable data-type...", op2str(postfix->op));
      return -1;
    }
    return 1;
    break;
  default: // LCOV_EXCL_START
    err_msg(SCAN1_, postfix->pos,
            "internal compiler error (pre-scan): unrecognized postfix '%i'", op2str(postfix->op));
    return -1;
  }        // LCOV_EXCL_STOP
  return 1;
}

static m_bool scan1_exp_dur(Env env, Exp_Dur* dur) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1",  "dur");
#endif
  CHECK_BB(scan1_exp(env, dur->base))
  CHECK_BB(scan1_exp(env, dur->unit))
  return 1;
}

static m_bool scan1_exp_call1(Env env, Exp exp_func, Exp args, Func func, int pos) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "func call1");
#endif
  CHECK_BB(scan1_exp(env, exp_func))
  CHECK_BB(args && scan1_exp(env, args))
  return 1;
}

static m_bool scan1_exp_call(Env env, Exp_Func* exp_func) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "func call");
#endif
  if(exp_func->types)
    return 1;
  return scan1_exp_call1(env, exp_func->func, exp_func->args, exp_func->m_func, exp_func->pos);
}

static m_bool scan1_exp_dot(Env env, Exp_Dot* member) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "dot member");
#endif
  CHECK_BB(scan1_exp(env, member->base));
  return 1;
}

static m_bool scan1_exp_if(Env env, Exp_If* exp_if) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "exp if");
#endif
  CHECK_BB(scan1_exp(env, exp_if->cond))
  CHECK_BB(scan1_exp(env, exp_if->if_exp))
  CHECK_BB(scan1_exp(env, exp_if->else_exp))
  return 1;
}

static m_bool scan1_exp(Env env, Exp exp) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "exp %p", exp);
#endif
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
      if(exp->d.exp_unary.code)
        CHECK_BB(scan1_stmt(env, exp->d.exp_unary.code))
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
  }
  return 1;
}

static m_bool scan1_stmt_code(Env env, Stmt_Code stmt, m_bool push) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "code");
#endif
  int ret;
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
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "return");
#endif
  return stmt->val ? scan1_exp(env, stmt->val) : 1;
}

static m_bool scan1_stmt_while(Env env, Stmt_While stmt) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "while");
#endif
  CHECK_BB(scan1_exp(env, stmt->cond))
  CHECK_BB(scan1_stmt(env, stmt->body))
  return 1;
}

static m_bool scan1_stmt_until(Env env, Stmt_Until stmt) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "until");
#endif
  CHECK_BB(scan1_exp(env, stmt->cond))
  CHECK_BB(scan1_stmt(env, stmt->body))
  return 1;
}

static m_bool scan1_stmt_for(Env env, Stmt_For stmt) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "for");
#endif
  CHECK_BB(scan1_stmt(env, stmt->c1))
  CHECK_BB(scan1_stmt(env, stmt->c2))
  CHECK_BB(scan1_exp(env, stmt->c3))
  CHECK_BB(scan1_stmt(env, stmt->body))
  return 1;
}

static m_bool scan1_stmt_loop(Env env, Stmt_Loop stmt) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "loop");
#endif
  CHECK_BB(scan1_exp(env, stmt->cond))
  CHECK_BB(scan1_stmt(env, stmt->body))
  return 1;
}

static m_bool scan1_stmt_switch(Env env, Stmt_Switch stmt) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "switch");
#endif
  return scan1_exp(env, stmt->val) < 0 ? -1 : 1;
}

static m_bool scan1_stmt_case(Env env, Stmt_Case stmt) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "case");
#endif
  return scan1_exp(env, stmt->val) < 0 ? -1 : 1;
}

static m_bool scan1_stmt_if(Env env, Stmt_If stmt) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "if");
#endif
  CHECK_BB(scan1_exp(env, stmt->cond))
  CHECK_BB(scan1_stmt(env, stmt->if_body))
  if(stmt->else_body)
    CHECK_BB(scan1_stmt(env, stmt->else_body))
    return 1;
}

static m_bool scan1_stmt_enum(Env env, Stmt_Enum stmt) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "enum");
#endif
  Nspc nspc = env->class_def ? env->class_def->info : env->curr;
  Type t = NULL;
  ID_List list = stmt->list;
  m_uint count = list ? 1 : 0;
  if(stmt->xid) {
    if(nspc_lookup_type(nspc, stmt->xid, 1)) {
      err_msg(SCAN1_, stmt->pos, "type '%s' already declared", S_name(stmt->xid));
      return -1;
    }
    if(nspc_lookup_value(env->curr, stmt->xid, 1)) {
      err_msg(SCAN1_, stmt->pos, "'%s' already declared as variable", S_name(stmt->xid));
      return -1;
    }
  }
  t = type_copy(env, &t_int);
  t->name = stmt->xid ? S_name(stmt->xid) : "int";
  t->parent = &t_int;
  nspc_add_type(nspc, stmt->xid, t);
  while(list) {
    Value v;
    if(nspc_lookup_value(nspc, list->xid, 0)) {
      err_msg(SCAN1_, stmt->pos, "in enum argument %i '%s' already declared as variable", count, S_name(list->xid));
      return -1;
    }
    v = new_value(t, S_name(list->xid));
    if(env->class_def) {
      v->owner_class = env->class_def;
      SET_FLAG(v, ae_value_static);
    }
    SET_FLAG(v, ae_value_const | ae_value_enum | ae_value_checked);
    nspc_add_value(nspc, list->xid, v);
    vector_add(stmt->values, (vtype)v);
    list = list->next;
    count++;
  }
  return 1;
}

static m_bool scan1_stmt_typedef(Env env, Stmt_Ptr ptr) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "func ptr");
#endif
  Arg_List arg_list;
  int count = 1;
  arg_list = ptr->args;
  ptr->ret_type = find_type(env, ptr->type->xid);

  if(!ptr->ret_type) {
    err_msg(SCAN1_, ptr->pos, "unknown type '%s' in func ptr declaration",  S_name(ptr->xid));
    return -1;
  }

  if(!env->class_def && ptr->key == ae_key_static) {
    err_msg(SCAN1_, ptr->pos, "can't declare func pointer static outside of a class");
    return -1;
  }

  while(arg_list) {
    arg_list->type = find_type(env, arg_list->type_decl->xid);
    if(!arg_list->type) {
      m_str path = type_path(arg_list->type_decl->xid);
      err_msg(SCAN1_, arg_list->pos, "'%s' unknown type in argument %i of func %s", path,  count, S_name(ptr->xid));
      free(path);
      return -1;
    }
    count++;
    arg_list = arg_list->next;
  }
  return 1;
}

static m_bool scan1_stmt_union(Env env, Stmt_Union stmt) {
  Decl_List l = stmt->l;

  while(l) {
    Var_Decl_List list = l->self->d.exp_decl.list;
    Var_Decl var_decl = NULL;

    if(l->self->exp_type != ae_exp_decl) {
      err_msg(SCAN1_, stmt->pos, "invalid expression type '%i' in union declaration.");
	  return -1;
	}
    Type t = find_type(env, l->self->d.exp_decl.type->xid);
    if(!t) {
      err_msg(SCAN1_, l->self->pos, "unknown type '%s' in union declaration ", S_name(l->self->d.exp_decl.type->xid->xid));
      return -1;
    }
    while(list) {
      var_decl = list->self;
      l->self->d.exp_decl.num_decl++;
      if(var_decl->array) {
        CHECK_BB(verify_array(var_decl->array))
        if(var_decl->array->exp_list) {
          err_msg(SCAN1_, l->self->pos, "array declaration must be empty in union.");
          return -1;
        }
      }
      list = list->next;
    }
    l->self->d.exp_decl.m_type = t;
    l = l->next;
  }
  return 1;
}

static m_bool scan1_stmt(Env env, Stmt stmt) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "stmt");
#endif
  m_bool ret = -1;
  if(!stmt)
    return 1;

  // DIRTY!!! happens when '1, new Object', for instance
  if(stmt->type == 3 && !stmt->d.stmt_for.c1) // bad thing in parser, continue
    return 1;

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
    NSPC(ret = scan1_stmt_while(env, &stmt->d.stmt_while))
    break;
  case ae_stmt_for:
    NSPC(ret = scan1_stmt_for(env, &stmt->d.stmt_for))
    break;
  case ae_stmt_until:
    NSPC(ret = scan1_stmt_until(env, &stmt->d.stmt_until))
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
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "stmt list");
#endif
  Stmt_List curr = list;
  while(curr) {
    CHECK_BB(scan1_stmt(env, curr->stmt))
    curr = curr->next;
  }
  return 1;
}

m_bool scan1_func_def(Env env, Func_Def f) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "func def");
#endif

  if(f->spec == ae_func_spec_dtor && !env->class_def) {
    err_msg(SCAN1_, f->pos, "dtor must be in class def!!");
    return -1;
  }

  if(f->types)
    return 1;
  Arg_List arg_list = NULL;
  m_uint count = 0;

  f->ret_type = find_type(env, f->type_decl->xid);

  if(!f->ret_type) {
    err_msg(SCAN1_, f->pos, "scan1: unknown return type '%s' of func '%s'",
            S_name(f->type_decl->xid->xid), S_name(f->name));
    return -1;
  }

  if(f->type_decl->array) {
    CHECK_BB(verify_array(f->type_decl->array))

    Type t = NULL;
    Type t2 = f->ret_type;
    if(f->type_decl->array->exp_list) {
      err_msg(SCAN1_, f->type_decl->array->pos, "in function '%s':", S_name(f->name));
      err_msg(SCAN1_, f->type_decl->array->pos, "return array type must be defined with empty []'s");
      free_expression(f->type_decl->array->exp_list);
      return -1;
    }
    t = new_array_type(env, f->type_decl->array->depth, t2, env->curr);
    f->type_decl->ref = 1;
    f->ret_type = t;
  }
  arg_list = f->arg_list;
  count = 1;

  while(arg_list) {
    if(!(arg_list->type = find_type(env, arg_list->type_decl->xid))) {
      m_str path = type_path(arg_list->type_decl->xid);
      err_msg(SCAN1_, arg_list->pos, "'%s' unknown type in argument %i of func %s", path, count, S_name(f->name));
      free(path);
      return -1;
    }
    count++;
    arg_list = arg_list->next;
  }
  if(f->spec == ae_func_spec_op) {
    if(count > 3 || count == 1) {
      err_msg(SCAN1_, f->pos, "operators can only have one or two arguments\n");
      return -1;
    }
    if(name2op(S_name(f->name)) < 0) {
      err_msg(SCAN1_, f->pos, "%s is not a valid operator name\n", S_name(f->name));
      return -1;
    }
  }
  if(f->code && scan1_stmt_code(env, &f->code->d.stmt_code, 0) < 0) {
    err_msg(SCAN1_, f->pos, "...in function '%s'\n", S_name(f->name));
    return -1;
  }
  return 1;
}

static m_bool scan1_class_def(Env env, Class_Def class_def) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "class def");
#endif
  m_bool ret = 1;
  Class_Body body = class_def->body;
  Type the_class = class_def->type;
  vector_add(env->nspc_stack, (vtype)env->curr);
  env->curr = the_class->info;
  vector_add(env->class_stack, (vtype)env->class_def);
  env->class_def = the_class;
  env->class_scope = 0;

  while(body && ret > 0) {
    switch(body->section->type) {
    case ae_section_stmt:
      ret = scan1_stmt_list(env, body->section->d.stmt_list);
      break;
    case ae_section_func:
      ret = scan1_func_def(env, body->section->d.func_def);
      break;
    case ae_section_class:
      ret = scan1_class_def(env, body->section->d.class_def);
      break;
    }
    body = body->next;
  }
  env->class_def = (Type)vector_pop(env->class_stack);
  env->curr = (Nspc)vector_pop(env->nspc_stack);
  return ret;
}

m_bool scan1_ast(Env env, Ast ast) {
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "Ast");
#endif
  Ast prog = ast;
  while(prog) {
    switch(prog->section->type) {
    case ae_section_stmt:
      CHECK_BB(scan1_stmt_list(env, prog->section->d.stmt_list))
      break;
    case ae_section_func:
      CHECK_BB(scan1_func_def(env, prog->section->d.func_def))
      break;
    case ae_section_class:
      CHECK_BB(scan1_class_def(env, prog->section->d.class_def))
      break;
    }
    prog = prog->next;
  }
  return 1;
}
