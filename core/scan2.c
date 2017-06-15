#include <string.h>
#include "err_msg.h"
#include "absyn.h"
#include "type.h"
#include "context.h"
#include "func.h"

m_bool scan2_func_def(Env env, Func_Def f);
static m_bool scan2_exp(Env env, Exp exp);
static m_bool scan2_stmt(Env env, Stmt stmt);
static m_bool scan2_stmt_list(Env env, Stmt_List list);

m_bool scan2_exp_decl(Env env, Exp_Decl* decl) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Declaration");
#endif
  Var_Decl_List list = decl->list;
  Type type = decl->m_type;

  if(isa(type, &t_shred) > 0)
    decl->type->ref = 1;
  if(!type->size) {
    err_msg(SCAN2_, decl->pos,
      "cannot declare variables of size '0' (i.e. 'void')...");
    return -1;
  }
  if(!decl->type->ref) {
    if(env->class_def && (type == env->class_def) && !env->class_scope) {
      err_msg(SCAN2_, decl->pos,
              "...(note: object of type '%s' declared inside itself)", type->name);
      return -1;
    }
  } else if((isprim(type) > 0)) {
    err_msg(SCAN2_, decl->pos,
            "cannot declare references (@) of primitive type '%s'...", type->name);
    err_msg(SCAN2_, decl->pos,
            "...(primitive types: 'int', 'float', 'time', 'dur')");
    return -1;
  }

  while(list) {
    if(isres(env, list->self->xid, list->self->pos) > 0) {
      err_msg(SCAN2_, list->self->pos, "... in variable declaration", S_name(list->self->xid));
      return -1;
    }
    if(nspc_lookup_value(env->curr, list->self->xid, 0)) {
      err_msg(SCAN2_, list->self->pos, "variable %s has already been defined in the same scope...", S_name(list->self->xid));
      return -1;
    }
    if(list->self->array != NULL) {
      CHECK_BB(verify_array(list->self->array))
      Type t2 = type;

      if(list->self->array->exp_list)
        CHECK_BB(scan2_exp(env, list->self->array->exp_list))
        type = new_array_type(env, list->self->array->depth, t2, env->curr);
      if(!list->self->array->exp_list)
        decl->type->ref = 1;
      decl->m_type = type;
    }
    list->self->value = new_value(type, S_name(list->self->xid));
    list->self->value->owner = env->curr;
    list->self->value->owner_class = env->func ? NULL : env->class_def;
    if(env->class_def && !env->class_scope && !env->func && !decl->is_static)
      SET_FLAG(list->self->value, ae_value_member);
    if(!env->class_def && !env->func)
      SET_FLAG(list->self->value, ae_value_global);
    list->self->value->ptr = list->self->addr;
    nspc_add_value(env->curr, list->self->xid, list->self->value);
    if(!env->class_def && !env->func) // doc ?
      context_add_value(env->context, list->self->value, &list->self->value->obj);
    if(list->doc)
      list->self->value->doc = list->doc;
    list = list->next;
  }
  return 1;
}

static m_bool scan2_stmt_typedef(Env env, Stmt_Ptr ptr) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Func Ptr");
#endif
  m_uint count = 1;
  Arg_List arg_list = ptr->args;
  Value v;

  nspc_push_value(env->curr);
  while(arg_list) {
    if(arg_list->type->size == 0) {
      err_msg(SCAN2_, arg_list->pos, "cannot declare variables of size '0' (i.e. 'void')...");
      goto error;
    }
    if(isres(env, arg_list->var_decl->xid, arg_list->pos) > 0) {
      err_msg(SCAN2_, arg_list->pos, "in function '%s'", S_name(ptr->xid));
      goto error;
    }
    if((isprim(arg_list->type) > 0) && arg_list->type_decl->ref) {
      err_msg(SCAN2_, arg_list->type_decl->pos,
              "cannot declare references (@) of primitive type '%s'...",
              arg_list->type->name);
      err_msg(SCAN2_, arg_list->type_decl->pos,
              "...(primitive types: 'int', 'float', 'time', 'dur')");
      goto error;
    }
    if(arg_list->var_decl->array) {
      CHECK_BB(verify_array(arg_list->var_decl->array))
      Type t = arg_list->type;
      Type t2 = t;
      if(arg_list->var_decl->array->exp_list) {
        err_msg(SCAN2_, arg_list->pos, "in function '%s':", S_name(ptr->xid));
        err_msg(SCAN2_, arg_list->pos, "argument #%i '%s' must be defined with empty []'s",
                count, S_name(arg_list->var_decl->xid));
        goto error;
      }
      t = new_array_type(env, arg_list->var_decl->array->depth, t2, env->curr);
      arg_list->type_decl->ref = 1;
      arg_list->type = t;
    }

    v = new_value(arg_list->type, S_name(arg_list->var_decl->xid));
    v->owner = env->curr;
    SET_FLAG(v, ae_value_arg);
    nspc_add_value(env->curr, arg_list->var_decl->xid, v);
    arg_list->var_decl->value = v;
    count++;
    arg_list = arg_list->next;
  }
  nspc_pop_value(env->curr);

  SET_FLAG(ptr->value, ae_value_checked);
  nspc_add_value(env->curr, ptr->xid, ptr->value);

  Func_Def def = new_func_def(ae_key_func, !env->class_def ? ae_key_func : !ptr->key ? ae_key_instance : ae_key_static, ptr->type, S_name(ptr->xid), ptr->args, NULL, ptr->pos);
  def->ret_type = ptr->ret_type;
  ptr->func = new_func(S_name(ptr->xid), def);
  ptr->value->func_ref = ptr->func;
  ptr->func->value_ref = ptr->value;
  if(env->class_def) {
    if(!ptr->key) {
      SET_FLAG(ptr->value, ae_value_member);
      ptr->func->is_member   = !ptr->key;
    }
    ptr->value->owner_class = env->class_def;
  }
  nspc_add_func(env->curr, ptr->xid, ptr->func);
  if(!ptr->key)
    ADD_REF(ptr->func);
  return 1;
error:
  nspc_pop_value(env->curr);
  return -1;
}

static m_bool scan2_exp_primary(Env env, Exp_Primary* prim) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Primary");
#endif
  if(prim->type == ae_primary_hack)
    CHECK_BB(scan2_exp(env, prim->d.exp))
  return 1;
}

static m_bool scan2_exp_array(Env env, Exp_Array* array) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Array");
#endif
  CHECK_BB(verify_array(array->indices))
  CHECK_BB(scan2_exp(env, array->base))
  CHECK_BB(scan2_exp(env, array->indices->exp_list))
  return 1;
}

static m_bool scan2_exp_binary(Env env, Exp_Binary* binary) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Binary");
#endif
  CHECK_BB(scan2_exp(env, binary->lhs))
  CHECK_BB(scan2_exp(env, binary->rhs))
  return 1;
}

static m_bool scan2_exp_cast(Env env, Exp_Cast* cast) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Cast");
#endif
  CHECK_BB(scan2_exp(env, cast->exp))
  return 1;
}

static m_bool scan2_exp_postfix(Env env, Exp_Postfix* postfix) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Postfix");
#endif
  CHECK_BB(scan2_exp(env, postfix->exp))
  switch(postfix->op) {
  case op_plusplus:
  case op_minusminus:
    return 1;
    break;
  default: // LCOV_EXCL_START
    err_msg(SCAN2_, postfix->pos,
            "internal compiler error (pre-scan): unrecognized postfix '%i'", postfix->op);
    return -1;
  }        // LCOV_EXCL_STOP
  return 1;
}

static m_bool scan2_exp_dur(Env env, Exp_Dur* dur) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "PostFix");
#endif
  CHECK_BB(scan2_exp(env, dur->base))
  CHECK_BB(scan2_exp(env, dur->unit))
  return 1;
}

static m_bool scan2_exp_call1(Env env, Exp func, Exp args, Func m_func) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Func Call1");
#endif
  CHECK_BB(scan2_exp(env, func))
  CHECK_BB(scan2_exp(env, args))
  return 1;
}

static m_bool scan2_exp_call(Env env, Exp_Func* exp_func) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Func Call");
#endif
  if(exp_func->types) {
    if(exp_func->func->exp_type == ae_exp_primary) {
      Value v = nspc_lookup_value(env->curr, exp_func->func->d.exp_primary.d.var, 1);
      if(!v) {
        err_msg(SCAN2_, exp_func->pos, "template call of non-existant function.");
        return -1;
      }
      if(!v->func_ref) {
        err_msg(SCAN2_, exp_func->pos, "template call of non-function value.");
        return -1;
      }
      Func_Def base = v->func_ref->def;
      if(!base->types) {
        err_msg(SCAN2_, exp_func->pos, "template call of non-template function.");
        return -1;
      }
      Type_List list = exp_func->types;
      while(list) {
        Type t = find_type(env, list->list);
        if(!t) {
          err_msg(SCAN1_, exp_func->pos, "type '%s' unknown in template call", S_name(list->list->xid));
          return -1;
        }
        list = list->next;
      }
      // check num types matches.
      Value value;
      m_uint i;
      m_bool match = -1;
      {
        for(i = 0; i < v->func_num_overloads + 1; i++) {
          char name[256];
          sprintf(name, "%s<template>@%li@%s", v->name, i, env->curr->name);
          value = nspc_lookup_value(env->curr, insert_symbol(name), 1);
          if(!value)continue;
          Type_List tlc = exp_func->types;
          ID_List tld = value->func_ref->def->types;
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
      }
      if(match < 0)
        err_msg(SCAN2_, exp_func->pos, "template type number mismatch.");
      return match;
    } else if(exp_func->func->exp_type == ae_exp_dot) {
      // see type.c
      return 1;
      /*    } else {
            err_msg(SCAN2_, exp_func->pos, "unhandled expression type '%i' in template func call.", exp_func->func->exp_type);
            return -1; */
    }
  }
  return scan2_exp_call1(env, exp_func->func, exp_func->args, exp_func->m_func);
}

static m_bool scan2_exp_dot(Env env, Exp_Dot* member) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Dot Member");
#endif
  CHECK_BB(scan2_exp(env, member->base))
  return 1;
}

static m_bool scan2_exp_if(Env env, Exp_If* exp_if) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "If Exp");
#endif
  CHECK_BB(scan2_exp(env, exp_if->cond))
  CHECK_BB(scan2_exp(env, exp_if->if_exp))
  CHECK_BB(scan2_exp(env, exp_if->else_exp))
  return 1;
}

static m_bool scan2_exp(Env env, Exp exp) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Exp");
#endif
  Exp curr = exp;
  m_bool ret = 1;
  while(curr && ret > 0) {
    switch(exp->exp_type) {
    case ae_exp_primary:
      ret = scan2_exp_primary(env, &exp->d.exp_primary);
      break;
    case ae_exp_decl:
      ret = scan2_exp_decl(env, &exp->d.exp_decl);
      break;
    case ae_exp_unary:
      if(exp->d.exp_unary.code) {
        if(env->func)
          ret = scan2_stmt(env, exp->d.exp_unary.code);
        else {
          env->func = (Func)1; // check me
          ret = scan2_stmt(env, exp->d.exp_unary.code);
          env->func = NULL;
        }
      } else
        ret = 1;
      break;
    case ae_exp_binary:
      ret = scan2_exp_binary(env, &exp->d.exp_binary);
      break;
    case ae_exp_postfix:
      ret = scan2_exp_postfix(env, &exp->d.exp_postfix);
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
    curr = curr->next;
  }
  return ret;
}

static m_bool scan2_stmt_code(Env env, Stmt_Code stmt, m_bool push) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Code");
#endif
  env->class_scope++;
  if(push)
    nspc_push_value(env->curr);
  m_bool t = scan2_stmt_list(env, stmt->stmt_list);
  if(push)
    nspc_pop_value(env->curr);
  env->class_scope--;
  return t;
}

static m_bool scan2_stmt_while(Env env, Stmt_While stmt) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "While");
#endif
  CHECK_BB(scan2_exp(env, stmt->cond))
  CHECK_BB(scan2_stmt(env, stmt->body))
  return 1;
}

static m_bool scan2_stmt_if(Env env, Stmt_If stmt) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "If");
#endif
  CHECK_BB(scan2_exp(env, stmt->cond))
  CHECK_BB(scan2_stmt(env, stmt->if_body))
  if(stmt->else_body)
    CHECK_BB(scan2_stmt(env, stmt->else_body))
  return 1;
}

static m_bool scan2_stmt_until(Env env, Stmt_Until stmt) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Until");
#endif
  CHECK_BB(scan2_exp(env, stmt->cond))
  CHECK_BB(scan2_stmt(env, stmt->body))
  return 1;
}

static m_bool scan2_stmt_for(Env env, Stmt_For stmt) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "For");
#endif
  CHECK_BB(scan2_stmt(env, stmt->c1))
  CHECK_BB(scan2_stmt(env, stmt->c2))
  CHECK_BB(scan2_exp(env, stmt->c3))
  CHECK_BB(scan2_stmt(env, stmt->body))
  return 1;
}

static m_bool scan2_stmt_loop(Env env, Stmt_Loop stmt) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Loop");
#endif
  CHECK_BB(scan2_exp(env, stmt->cond))
  CHECK_BB(scan2_stmt(env, stmt->body))
  return 1;
}

static m_bool scan2_stmt_return(Env env, Stmt_Return stmt) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Return");
#endif
  m_bool ret = -1;
  if(stmt->val)
    ret = scan2_exp(env, stmt->val);
  else
    ret = 1;
  return ret;
}

static m_bool scan2_stmt_switch(Env env, Stmt_Switch stmt) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Switch");
#endif
  CHECK_BB(scan2_exp(env, stmt->val))
  return 1;
}

static m_bool scan2_stmt_case(Env env, Stmt_Case stmt) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Case");
#endif
  CHECK_BB(scan2_exp(env, stmt->val))
  return 1;
}

static m_bool scan2_stmt_gotolabel(Env env, Stmt_Goto_Label stmt) {
#ifdef DEBUG_SCAN2
  debug_msg("scan1", "%s '%s'", stmt->is_label ? "label" : "goto", S_name(stmt->name));
#endif
  Map m;
  m_uint* key = env->class_def && !env->func ? (m_uint*)env->class_def : (m_uint*)env->func;
  if(stmt->is_label) {
    m = (Map)map_get(env->curr->label, (vtype)key);
    if(!m) {
      m = new_map();
      map_set(env->curr->label, (vtype)key, (vtype)m);
    }
    if(map_get(m, (vtype)stmt->name)) {
      err_msg(SCAN2_, stmt->pos, "label '%s' already defined", S_name(stmt->name));
      Stmt_Goto_Label l = (Stmt_Goto_Label)map_get(m, (vtype)stmt->name);
      free_vector(l->data.v);
      return -1;
    }
    map_set(m, (vtype)stmt->name, (vtype)stmt);
    stmt->data.v = new_vector();
  }
  return 1;
}

static m_bool scan2_stmt_enum(Env env, Stmt_Enum stmt) {
  Value v = nspc_lookup_value(env->curr, stmt->xid, 1);
  if(v) {
    err_msg(SCAN2_, stmt->pos, "'%s' already declared as variable", S_name(stmt->xid));
    return -1;
  }
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
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Stmt");
#endif
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
    NSPC(ret = scan2_stmt_while(env, &stmt->d.stmt_while))
    break;

  case ae_stmt_until:
    NSPC(ret = scan2_stmt_until(env, &stmt->d.stmt_until))
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
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Stmt list");
#endif
  Stmt_List curr = list;
  while(curr) {
    CHECK_BB(scan2_stmt(env, curr->stmt))
    curr = curr->next;
  }
  return 1;
}

m_bool scan2_func_def(Env env, Func_Def f) {
  Type     type     = NULL;
  Value    value    = NULL;
  Func     func     = NULL;
  Value    overload = NULL;
  Arg_List arg_list = NULL;
  Value    v;
  m_str func_name = S_name(f->name);
  m_str orig_name = func_name;
  m_uint count = 0;

  if(f->types) {
    func = new_func(func_name, f);
    overload = nspc_lookup_value(env->curr,  f->name, 0);
    if(overload)
      func->next = overload->func_ref->next;
    func->is_member = (env->class_def && (f->static_decl != ae_key_static));
    value = new_value(&t_function, func_name);
    value->owner = env->curr;
    value->owner_class = env->class_def;
    if(func->is_member)
      SET_FLAG(value, ae_value_member);
    if(!env->class_def)
      SET_FLAG(value, ae_value_global);
    value->func_ref = func;
//    ADD_REF(value->func_ref);
    func->value_ref = value;
//    ADD_REF(value);
    f->func = func;
    SET_FLAG(value, ae_value_const | ae_value_checked);
    if(overload)
      overload->func_num_overloads++;
    else
      nspc_add_value(env->curr, insert_symbol(orig_name), value);
    char name[256];
    sprintf(name, "%s<template>@%li@%s", S_name(f->name),
            overload ? overload->func_num_overloads : 0, env->curr->name);
    nspc_add_value(env->curr, insert_symbol(name), value);
    return 1;
  }

  // overload
  overload = nspc_lookup_value(env->curr,  f->name, 0);
  if(overload) {
    if(isa(overload->m_type, &t_function) < 0) {
      err_msg(SCAN2_, f->pos, "function name '%s' is already used by another value", S_name(f->name));
      goto error;
    } else {
      if(!overload->func_ref) {
        err_msg(SCAN2_, f->pos, "internal error: missing function '%s'", overload->name); // LCOV_EXCL_LINE
        return -1;                                                                         // LCOV_EXCL_LINE
      }
    }
  }
  char tmp[256];
  memset(tmp, 0, 256);
  m_uint len = strlen(func_name) + strlen("0") + strlen(env->curr->name) + 3;

  if(overload && !f->is_template) {
    len = strlen(func_name) + ((overload->func_num_overloads + 1) % 10) + strlen(env->curr->name) + 3;
    snprintf(tmp, len + 1, "%s@%li@%s", func_name, ++overload->func_num_overloads, env->curr->name);
  } else
    snprintf(tmp, len + 1, "%s@0@%s", func_name, env->curr->name);
  func_name = strdup(tmp);
  func = new_func(func_name, f);
  func->is_member = (env->class_def && (f->static_decl != ae_key_static));

  if(f->s_type == ae_func_builtin) { // actual builtin func import
    func->code = new_vm_code(NULL, func->def->stack_depth, 1, S_name(f->name), "builtin func code");
    func->code->need_this = func->is_member;
    func->code->native_func = (m_uint)func->def->dl_func_ptr;
  }

  type = new_type(te_function, strdup(tmp));
  type->parent = &t_function;
  type->size = SZ_INT;
  type->func = func;
  value = new_value(type, func_name);
  SET_FLAG(value, ae_value_const);
  value->owner = env->curr;
  value->owner_class = env->class_def;
  if(func->is_member)
    SET_FLAG(value, ae_value_member);
  if(!env->class_def)
    SET_FLAG(value, ae_value_global);
  value->func_ref = func;
//  ADD_REF(value->func_ref);
  func->value_ref = value;
  f->func = func;
//  ADD_REF(f->func);

  if(overload) {
    func->next = overload->func_ref->next;
    overload->func_ref->next = func;
  }
  if(isprim(f->ret_type) > 0 && f->type_decl->ref) {
    err_msg(SCAN2_,  f->type_decl->pos,
            "FUNC cannot declare references (@) of primitive type '%s'...\n", f->ret_type->name);
    err_msg(SCAN2_, f->type_decl->pos, "...(primitive types: 'int', 'float', 'time', 'dur')");
    goto error;
  }

  arg_list = f->arg_list;
  count = 1;
  f->stack_depth = func->is_member ? SZ_INT : 0;

  nspc_push_value(env->curr);

  while(arg_list) {
    if(!arg_list->type->size) {
      err_msg(SCAN2_, arg_list->pos, "cannot declare variables of size '0' (i.e. 'void')...");
      nspc_pop_value(env->curr);
      goto error;
    }
    if(isres(env, arg_list->var_decl->xid, arg_list->pos) > 0) {
      err_msg(SCAN2_,  arg_list->pos, "in function '%s'", S_name(f->name));
      nspc_pop_value(env->curr);
      goto error;
    }
    if((isprim(arg_list->type) > 0)
        && arg_list->type_decl->ref) {
      err_msg(SCAN2_, arg_list->type_decl->pos,
              "cannot declare references (@) of primitive type '%s'...",
              arg_list->type->name);
      err_msg(SCAN2_, arg_list->type_decl->pos,
              "...(primitive types: 'int', 'float', 'time', 'dur')");
      nspc_pop_value(env->curr);
      goto error;
    }

    if(arg_list->var_decl->array) {
      CHECK_BB(verify_array(arg_list->var_decl->array))

      Type t = arg_list->type;
      Type t2 = t;
      if(arg_list->var_decl->array->exp_list) {
        err_msg(SCAN2_, arg_list->pos, "in function '%s':", S_name(f->name));
        err_msg(SCAN2_, arg_list->pos, "argument %i '%s' must be defined with empty []'s",
                count, S_name(arg_list->var_decl->xid));
        nspc_pop_value(env->curr);
        goto error;
      }
      t = new_array_type(env, arg_list->var_decl->array->depth, t2, env->curr);
      arg_list->type_decl->ref = 1;
      arg_list->type = t;
    }
    v = new_value(arg_list->type, S_name(arg_list->var_decl->xid));
    v->owner = env->curr;
    SET_FLAG(v, ae_value_arg);
    nspc_add_value(env->curr, arg_list->var_decl->xid, v);
    v->offset = f->stack_depth;
    f->stack_depth += arg_list->type->size;

    if(arg_list->var_decl->value)
      REM_REF(arg_list->var_decl->value);
    arg_list->var_decl->value = v;
    count++;
    arg_list = arg_list->next;
  }

  nspc_pop_value(env->curr);

  if(f->is_variadic)
    f->stack_depth += SZ_INT;
  else if(f->spec == ae_func_spec_op) {
    m_bool ret;
    m_str str = strdup(S_name(f->name));
    str = strsep(&str, "@");
    ret = name2op(str);
    free(str);
    if(env->class_def)f->func->is_member = 1; // 04/05/17
    CHECK_BB(add_binary_op(env, ret, f->arg_list->var_decl->value->m_type,
                           f->arg_list->next ? f->arg_list->next->var_decl->value->m_type : NULL, f->ret_type, NULL, 1))
    if(!env->class_def)
      context_add_func(env->context, func, &func->obj);
    return 1;
  }

  SET_FLAG(value, ae_value_checked);
  // while making doc
  if(!env->class_def)
    context_add_func(env->context, func, &func->obj);

  nspc_add_func(env->curr, insert_symbol(func->name), func); // template. is it necessary ?
  if(!overload)
    nspc_add_value(env->curr, insert_symbol(orig_name), value);
  else {
    nspc_add_value(env->curr, insert_symbol(func->name), value);
    if(overload->func_ref->def->ret_type) // template func don't check ret_type case
      if(!f->is_template)
        if(f->ret_type->xid != overload->func_ref->def->ret_type->xid) {
          err_msg(SCAN2_,  f->pos, "function signatures differ in return type... '%s' and '%s'",
                  f->ret_type->name, overload->func_ref->def->ret_type->name);
          if(env->class_def)
            err_msg(SCAN2_, f->pos,
                    "function '%s.%s' matches '%s.%s' but cannot overload...",
                    env->class_def->name, S_name(f->name),
                    value->owner_class->name, S_name(f->name));
          return -1;
        }
  }

  env->func = func;
  nspc_push_value(env->curr);

  if(f->code && scan2_stmt_code(env, &f->code->d.stmt_code, 0) < 0) {
    err_msg(SCAN2_, f->pos, "...in function '%s'", S_name(f->name));
// should be in free_context, at least.
    free(value->m_type->name);
    REM_REF(value->m_type);
    f->func->value_ref->m_type = NULL;
    nspc_pop_value(env->curr);
    return -1;
  }
  nspc_pop_value(env->curr);
  env->func = NULL;

  if(f->spec == ae_func_spec_dtor) {
    f->func->is_dtor = 1;
//    ADD_REF(f->func->value_ref);
  }
  if(f->type_decl->doc)
    func->doc = f->type_decl->doc;

  return 1;
error:
  if(func) {
    env->func = NULL;
    scope_rem(env->curr->func, f->name);
    func->def = NULL;
    f->func = NULL;
    REM_REF(func);
  }
  if(value) {
    free(value->m_type->name);
    free(value->name);
    REM_REF(value->m_type);
    REM_REF(value);
  }
  return -1;
}

static m_bool scan2_class_def(Env env, Class_Def class_def) {
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
      ret = scan2_stmt_list(env, body->section->d.stmt_list);
      break;
    case ae_section_class:
      ret = scan2_class_def(env, body->section->d.class_def);
      break;
    case ae_section_func:
      ret = scan2_func_def(env, body->section->d.func_def);
      break;
    }
    body = body->next;
  }

  env->class_def = (Type)vector_pop(env->class_stack);
  env->curr = (Nspc)vector_pop(env->nspc_stack);

  if(class_def->doc)
    the_class->doc = class_def->doc;

  return ret;
}

m_bool scan2_ast(Env env, Ast ast) {
#ifdef DEBUG_SCAN2
  debug_msg("scan2", "Ast");
#endif
  Ast prog = ast;
  while(prog) {
    switch(prog->section->type) {
    case ae_section_stmt:
      CHECK_BB(scan2_stmt_list(env, prog->section->d.stmt_list))
      break;
    case ae_section_func:
      CHECK_BB(scan2_func_def(env, prog->section->d.func_def))
      break;
    case ae_section_class:
      CHECK_BB(scan2_class_def(env, prog->section->d.class_def))
      break;
    }
    prog = prog->next;
  }
  return 1;
}
