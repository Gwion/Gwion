#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "absyn.h"
#include "type.h"
#include "func.h"

#include "traverse.h"

static m_bool scan2_exp(Env env, Exp exp);
static m_bool scan2_stmt(Env env, Stmt stmt);
static m_bool scan2_stmt_list(Env env, Stmt_List list);
m_bool scan2_class_def(Env env, Class_Def class_def);

static m_bool scan2_exp_decl_template(Env env, Exp_Decl* decl) {
  Type type = decl->m_type;
  if(GET_FLAG(type, ae_flag_template)) {
    CHECK_BB(template_push_types(env, decl->base->types, decl->type->types));
    CHECK_BB(scan2_class_def(env, type->def))
    nspc_pop_type(env->curr);
  }
  return 1;
}

m_bool scan2_exp_decl(Env env, Exp_Decl* decl) {
  Var_Decl_List list = decl->list;
  Type type = decl->m_type;

  if(GET_FLAG(type, ae_flag_abstract) && !GET_FLAG(decl->type, ae_flag_ref))
    CHECK_BB(err_msg(SCAN2_, decl->pos, "Type '%s' is abstract, declare as ref"
        ". (use @)", type->name))
  CHECK_BB(scan2_exp_decl_template(env, decl))
  while(list) {
    if(list->self->array && list->self->array->exp_list)
        CHECK_BB(scan2_exp(env, list->self->array->exp_list))
    list = list->next;
  }
  return 1;
}

static m_bool scan2_arg_def_check(Arg_List list) {
  if(list->var_decl->value) {
    if(list->var_decl->value->m_type->array_depth)
//      REM_REF(list->var_decl->value->m_type->d.array_type)
      REM_REF(array_base(list->var_decl->value->m_type))
      list->var_decl->value->m_type = list->type;
  }
  if(!list->type->size)
    CHECK_BB(err_msg(SCAN2_, list->pos,
          "cannot declare variables of size '0' (i.e. 'void')..."))
  if(isres(list->var_decl->xid, list->pos) > 0)
    return -1;
  if((isprim(list->type) > 0) && GET_FLAG(list->type_decl, ae_flag_ref))
    CHECK_BB(err_msg(SCAN2_, list->type_decl->pos,
          "cannot declare references (@) of primitive type '%s'...\n"
          "\t...(primitive types: 'int', 'float', 'time', 'dur')",
          list->type->name))
  return 1;
}

static m_bool scan2_arg_def(Env env, Func_Def f) {
  Arg_List list = f->arg_list;
  m_uint count = 1;
  nspc_push_value(env->curr);
  while(list) {
    Value v;
    if(scan2_arg_def_check(list) < 0 ||
        (list->var_decl->array && !(list->type = get_array(list->type, list->var_decl->array, "argument")))) {
      nspc_pop_value(env->curr);
      return -1;
    }
    v = list->var_decl->value ? list->var_decl->value :
      new_value(list->type, s_name(list->var_decl->xid));
    v->owner = env->curr;
    SET_FLAG(v, ae_flag_arg);
    if(GET_FLAG(list->type_decl, ae_flag_const))
      SET_FLAG(v, ae_flag_const);
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

m_bool scan2_stmt_fptr(Env env, Stmt_Ptr ptr) {
  struct Func_Def_ d;
  d.arg_list = ptr->args;
  if(nspc_lookup_func2(env->curr, ptr->xid))
    CHECK_BB(err_msg(SCAN2_, ptr->pos,
          "function type '%s' already defined.", s_name(ptr->xid)))
    if(scan2_arg_def(env, &d) < 0)
      CHECK_BB(err_msg(SCAN2_, ptr->pos, "in typedef '%s'", s_name(ptr->xid)))
      SET_FLAG(ptr->value, ae_flag_checked);
  nspc_add_value(env->curr, ptr->xid, ptr->value);

  Func_Def def = new_func_def(GET_FLAG(ptr, ae_flag_static) ? ae_flag_static : 0,
      ptr->type, ptr->xid, ptr->args, NULL, ptr->pos);
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
  //if(!GET_FLAG(ptr, ae_flag_static) && !GET_FLAG(ptr, ae_flag_builtin))
  if(!GET_FLAG(ptr, ae_flag_builtin))
    ADD_REF(ptr->func);
  return 1;
}

static m_bool scan2_stmt_type(Env env, Stmt_Typedef stmt) {
  return scan2_class_def(env, stmt->m_type->def);
}

static m_bool scan2_exp_primary(Env env, Exp_Primary* prim) {
  if(prim->primary_type == ae_primary_hack)
    CHECK_BB(scan2_exp(env, prim->d.exp))
    return 1;
}

static m_bool scan2_exp_array(Env env, Exp_Array* array) {
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

static m_bool scan2_exp_post(Env env, Exp_Postfix* post) {
  CHECK_BB(scan2_exp(env, post->exp))
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

    sprintf(name, "%s<template>@%" INT_F "@%s", v->name, i, env->curr->name);
    value = nspc_lookup_value1(env->curr, insert_symbol(name));
    if(!value)continue;
    tld = value->func_ref->def->tmpl->list;
    UNSET_FLAG(value->func_ref->def, ae_flag_template);
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
  if(match)
    SET_FLAG(v, ae_flag_template);
  return match;
}

static m_bool scan2_exp_call(Env env, Exp_Func* exp_func) {
  if(exp_func->types) {
    if(exp_func->func->exp_type == ae_exp_primary) {
      Value v = nspc_lookup_value1(env->curr,
          exp_func->func->d.exp_primary.d.var);
      if(!v)
        CHECK_BB(err_msg(SCAN2_, exp_func->pos,
              "template call of non-existant function."))
      if(!v->func_ref)
        CHECK_BB(err_msg(SCAN2_, exp_func->pos,
              "template call of non-function value."))
      Func_Def base = v->func_ref->def;
      UNSET_FLAG(base, ae_flag_template);
      if(!base->tmpl || !base->tmpl->base)
        CHECK_BB(err_msg(SCAN2_, exp_func->pos,
              "template call of non-template function."))
      Type_List list = exp_func->types;
      while(list) {
        if(!find_type(env, list->list->xid))
          CHECK_BB(type_unknown(list->list->xid, "template call"))
        list = list->next; // TODO: check me since type_list change
      }
      if(scan2_template_match(env, v, exp_func->types) < 0)
        CHECK_BB(err_msg(SCAN2_, exp_func->pos,
              "template type number mismatch."))
      SET_FLAG(base, ae_flag_template);
      return 1;
    } else if(exp_func->func->exp_type == ae_exp_dot) {
      return 1;      // see type.c
    } else {
      CHECK_BB(err_msg(SCAN2_, exp_func->pos,
            "unhandled expression type '%i' in template func call.",
            exp_func->func->exp_type))
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
  m_bool ret = 1;
  while(exp && ret > 0) {
    switch(exp->exp_type) {
      case ae_exp_primary:
        ret = scan2_exp_primary(env, &exp->d.exp_primary);
        break;
      case ae_exp_decl:
        ret = scan2_exp_decl(env, &exp->d.exp_decl);
        break;
      case ae_exp_unary:
        ret = (exp->d.exp_unary.op == op_spork && exp->d.exp_unary.code) ?
          scan2_exp_spork(env, exp->d.exp_unary.code) : 1;
        break;
      case ae_exp_binary:
        ret = scan2_exp_binary(env, &exp->d.exp_binary);
        break;
      case ae_exp_post:
        ret = scan2_exp_post(env, &exp->d.exp_post);
        break;
      case ae_exp_cast:
        ret = scan2_exp_cast(env, &exp->d.exp_cast);
        break;
      case ae_exp_call:
        ret = scan2_exp_call(env, &exp->d.exp_func);
        break;
      case ae_exp_array:
        ret = scan2_exp_array(env, &exp->d.exp_array);
        break;
      case ae_exp_dot:
        ret = scan2_exp_dot(env, &exp->d.exp_dot);
        break;
      case ae_exp_if:
        ret = scan2_exp_if(env, &exp->d.exp_if);
        break;
      case ae_exp_dur:
        ret = scan2_exp_dur(env, &exp->d.exp_dur);
        break;
    }
    exp = exp->next;
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

static m_bool scan2_stmt_auto(Env env, Stmt_Auto stmt) {
  CHECK_BB(scan2_exp(env, stmt->exp))
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
  Map m, label = env_label(env);
  m_uint* key = env->class_def && !env->func ?
    (m_uint*)env->class_def : (m_uint*)env->func;
  if(!label->ptr)
    map_init(label);
  if(!(m = (Map)map_get(label, (vtype)key))) {
    m = new_map();
    map_set(label, (vtype)key, (vtype)m);
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

m_bool scan2_stmt_enum(Env env, Stmt_Enum stmt) {
  Value v = nspc_lookup_value1(env->curr, stmt->xid);
  if(v)
    CHECK_BB(err_msg(SCAN2_, stmt->pos,
          "'%s' already declared as variable", s_name(stmt->xid)))
  return 1;
}

m_bool scan2_stmt_union(Env env, Stmt_Union stmt) {
  Decl_List l = stmt->l;
  if(stmt->xid)
    env_push_class(env, stmt->value->m_type);
  while(l) {
    CHECK_BB(scan2_exp(env, l->self))
    l = l->next;
  }
  if(stmt->xid)
    env_pop_class(env);
  return 1;
}

static m_bool scan2_stmt(Env env, Stmt stmt) {
  m_bool ret = 1;
  if(!stmt)
    return 1;
  switch(stmt->stmt_type) {
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
    case ae_stmt_auto:
      NSPC(ret = scan2_stmt_auto(env, &stmt->d.stmt_auto))
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
      ret = scan2_stmt_fptr(env, &stmt->d.stmt_ptr);
      break;
    case ae_stmt_typedef:
      ret = scan2_stmt_type(env, &stmt->d.stmt_type);
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
    CHECK_BB(err_msg(SCAN2_, f->pos,
          "function name '%s' is already used by another value",
          s_name(f->name)))
  else if(!overload->func_ref)
    CHECK_BB(err_msg(SCAN2_, f->pos,
          "internal error: missing function '%s'",
          overload->name))
  if((!GET_FLAG(overload, ae_flag_template) && f->tmpl && f->tmpl->base)||
      (GET_FLAG(overload, ae_flag_template) && (!f->tmpl || !f->tmpl->base) &&
       !GET_FLAG(f, ae_flag_template)))
    CHECK_BB(err_msg(SCAN2_, f->pos,
          "must override template function with template"))
  return 1;
}

 static m_bool scan2_func_def_template (Env env, Func_Def f, Value overload) {
  m_str func_name = s_name(f->name);
  Func func = new_func(func_name, f);
  Value value;
  Type type;
  m_uint len = strlen(func_name) +
    num_digit(overload ? overload->func_num_overloads + 1 : 0) +
    strlen(env->curr->name) + 13;

  char name[len];
  if(overload)
    func->next = overload->func_ref->next;
  if(env->class_def && !GET_FLAG(f, ae_flag_static))
    SET_FLAG(func, ae_flag_member);
  type = type_copy(&t_function);
  type->name = func_name;
  type->owner = env->curr;
  value = new_value(type, func_name);
  CHECK_OB(scan2_func_assign(env, f, func, value))
  SET_FLAG(value, ae_flag_const | ae_flag_checked | ae_flag_template);
  if(GET_FLAG(f, ae_flag_private))
    SET_FLAG(value, ae_flag_private);
  if(overload)
    overload->func_num_overloads++;
  else {
    ADD_REF(type);
    ADD_REF(value);
    ADD_REF(func);
    nspc_add_value(env->curr, f->name, value);
  }
  snprintf(name, len, "%s<template>@%" INT_F "@%s", func_name,
           overload ? overload->func_num_overloads : 0, env->curr->name);
  nspc_add_value(env->curr, insert_symbol(name), value);
  return 1;
}

static m_bool scan2_func_def_builtin(Func func, m_str name) {
  SET_FLAG(func, ae_flag_builtin);
  func->code = new_vm_code(NULL, func->def->stack_depth, 1,
      name, "builtin func code");
  func->code->need_this = GET_FLAG(func, ae_flag_member);
  func->code->native_func = (m_uint)func->def->d.dl_func_ptr;
  return 1;
}

static m_bool scan2_func_def_op(Env env, Func_Def f) {
  Operator ret = name2op(s_name(f->name));
  Type l = GET_FLAG(f, ae_flag_unary) ? NULL :
    f->arg_list->var_decl->value->m_type;
  Type r = GET_FLAG(f, ae_flag_unary) ? f->arg_list->var_decl->value->m_type :
    f->arg_list->next ? f->arg_list->next->var_decl->value->m_type : NULL;
  struct Op_Import opi = { ret, l, r, f->ret_type, NULL, NULL, 0};
  CHECK_BB(env_add_op(env, &opi))
  return 1;
}

static m_bool scan2_func_def_code(Env env, Func_Def f) {
  m_bool ret = 1;
  env->func = f->d.func;
  nspc_push_value(env->curr);
  if(scan2_stmt_code(env, &f->code->d.stmt_code, 0) < 0)
    ret = err_msg(SCAN2_, f->pos, "... in function '%s'", s_name(f->name));
  nspc_pop_value(env->curr);
  env->func = NULL;
  return ret;
}

static m_bool scan2_func_def_add(Env env, Value value, Value overload) {
  m_str name = s_name(value->func_ref->def->name);
  Func func = value->func_ref;

  SET_FLAG(value, ae_flag_checked);
  if(overload) {
    if(overload->func_ref->def->ret_type)
      if(!GET_FLAG(func->def, ae_flag_template))
        if(func->def->ret_type->xid != overload->func_ref->def->ret_type->xid) {
          err_msg(SCAN2_,  func->def->pos,
              "function signatures differ in return type... '%s' and '%s'",
                  func->def->ret_type->name,
                  overload->func_ref->def->ret_type->name);
          if(env->class_def)
            CHECK_BB(err_msg(SCAN2_, func->def->pos,
                    "function '%s.%s' matches '%s.%s' but cannot overload...",
                    env->class_def->name, name,
                    value->owner_class->name, name))
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
  if(GET_FLAG(f, ae_flag_dtor))
    SET_FLAG(f->d.func->value_ref->owner_class, ae_flag_dtor);
  else if(GET_FLAG(f, ae_flag_variadic))
    f->stack_depth += SZ_INT;
}

m_bool scan2_func_def(Env env, Func_Def f) {
  Type     type     = NULL;
  Value    value    = NULL;
  Func     func     = NULL;

  Value overload = nspc_lookup_value0(env->curr,  f->name);
  m_str func_name = s_name(f->name);
  m_uint len = strlen(func_name) +
    num_digit(overload ? overload->func_num_overloads + 1 : 0) +
    strlen(env->curr->name) + 3;

  char name[len];

  if(overload)
    CHECK_BB(scan2_func_def_overload(f, overload))

  if(f->tmpl && f->tmpl->base)
    return scan2_func_def_template(env, f, overload);

  snprintf(name, len, "%s@%" INT_F "@%s", func_name,
           overload ? ++overload->func_num_overloads : 0, env->curr->name);

  func_name = s_name(insert_symbol(name));
  func = new_func(func_name, f);
  nspc_add_func(env->curr, insert_symbol(func->name), func);
  if(env->class_def && !GET_FLAG(f, ae_flag_static))
    SET_FLAG(func, ae_flag_member);
  if(GET_FLAG(f, ae_flag_builtin)) // actual builtin func import
    CHECK_BB(scan2_func_def_builtin(func, name))
  type = new_type(te_function, func_name, &t_function);
  type->size = SZ_INT;
  if(GET_FLAG(func, ae_flag_member))
    type->size += SZ_INT;
  type->d.func = func;
  value = new_value(type, func_name);
  if(GET_FLAG(f, ae_flag_private))
    SET_FLAG(value, ae_flag_private);
  nspc_add_value(env->curr, !overload ?
      f->name : insert_symbol(func->name), value);
  CHECK_OB(scan2_func_assign(env, f, func, value))
  scan2_func_def_flag(env, f);
  if(overload) {
    func->next = overload->func_ref->next;
    overload->func_ref->next = func;
  }
  if(isprim(f->ret_type) > 0 && GET_FLAG(f->type_decl, ae_flag_ref))
    CHECK_BB(err_msg(SCAN2_,  f->type_decl->pos,
          "FUNC cannot declare references (@) of primitive type '%s'...\n"
        "...(primitive types: 'int', 'float', 'time', 'dur')",
        f->ret_type->name))
  f->stack_depth = GET_FLAG(func, ae_flag_member) ? SZ_INT : 0;

  if(scan2_arg_def(env, f) < 0)
    CHECK_BB(err_msg(SCAN2_, f->pos,
          "\t... in function '%s'\n", s_name(f->name)))
  if(GET_FLAG(f, ae_flag_op))
    return scan2_func_def_op(env, f);

  CHECK_BB(scan2_func_def_add(env, value, overload))
  return f->code ? scan2_func_def_code(env, f) : 1;
}

static m_bool scan2_section(Env env, Section* section) {
  ae_Section_Type t = section->section_type;
  if(t == ae_section_stmt)
    CHECK_BB(scan2_stmt_list(env, section->d.stmt_list))
  else if(t == ae_section_func)
    CHECK_BB(scan2_func_def(env, section->d.func_def))
  else if(t == ae_section_class)
    CHECK_BB(scan2_class_def(env, section->d.class_def))
  return 1;
}

m_bool scan2_class_def(Env env, Class_Def class_def) {
  Class_Body body = class_def->body;

  if(class_def->types)
    return 1;
  if(class_def->ext) {
    if(!GET_FLAG(class_def->type->parent, ae_flag_scan2) && GET_FLAG(class_def->ext, ae_flag_typedef))
      CHECK_BB(scan2_class_def(env, class_def->type->parent->def))
    if(class_def->ext->array)
      CHECK_BB(scan2_exp(env, class_def->ext->array->exp_list))
  }
  CHECK_BB(env_push_class(env, class_def->type))
  while(body) {
    CHECK_BB(scan2_section(env, body->section))
    body = body->next;
  }
  CHECK_BB(env_pop_class(env))
  SET_FLAG(class_def->type, ae_flag_scan2);
  return 1;
}

m_bool scan2_ast(Env env, Ast ast) {
  while(ast) {
    CHECK_BB(scan2_section(env, ast->section))
    ast = ast->next;
  }
  return 1;
}
