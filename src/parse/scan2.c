#define PARSE
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "absyn.h"
#include "type.h"
#include "value.h"
#include "func.h"
#include "traverse.h"
#include "optim.h"

ANN static m_bool scan2_exp(const Env, const Exp);
ANN static m_bool scan2_stmt(const Env, const Stmt);
ANN static m_bool scan2_stmt_list(const Env, Stmt_List);
ANN m_bool scan2_class_def(const Env, const Class_Def);

ANN static m_bool scan2_exp_decl_template(const Env env, const Exp_Decl* decl) { GWDEBUG_EXE
  CHECK_BB(template_push_types(env, decl->base->tmpl->list.list, decl->td->types));
  CHECK_BB(scan2_class_def(env, decl->type->def))
  nspc_pop_type(env->curr);
  return 1;
}

ANN m_bool scan2_exp_decl(const Env env, const Exp_Decl* decl) { GWDEBUG_EXE
  Var_Decl_List list = decl->list;
  const Type type = decl->type;
  if(GET_FLAG(type, ae_flag_abstract) && !GET_FLAG(decl->td, ae_flag_ref))
    ERR_B(SCAN2_, decl->self->pos, "Type '%s' is abstract, declare as ref"
        ". (use @)", type->name)
  if(GET_FLAG(type, ae_flag_template) && !GET_FLAG(type, ae_flag_scan2))
    CHECK_BB(scan2_exp_decl_template(env, decl))
  m_uint class_scope;
  const m_bool global = GET_FLAG(decl->td, ae_flag_global);
  if(global)
   env_push(env, NULL, env->global_nspc, &class_scope);
  do {
    const Array_Sub array = list->self->array;
    if(array && array->exp)
        CHECK_BB(scan2_exp(env, array->exp))
  } while((list = list->next));
  if(global)
    env_pop(env, class_scope);
  return 1;
}

ANN static m_bool scan2_arg_def_check(Arg_List list) { GWDEBUG_EXE
  if(list->var_decl->value) {
    if(list->var_decl->value->type->array_depth)
      REM_REF(array_base(list->var_decl->value->type))
      list->var_decl->value->type = list->type;
  }
  if(!list->type->size)
    ERR_B(SCAN2_, list->var_decl->pos,
          "cannot declare variables of size '0' (i.e. 'void')...")
  if(isres(list->var_decl->xid) > 0)
    return -1;
  if(GET_FLAG(list->td, ae_flag_ref))
    CHECK_BB(prim_ref(list->td, list->type))
  return 1;
}

ANN2(1) static m_bool scan2_arg_def(const Env env, const Func_Def f) { GWDEBUG_EXE
  Arg_List list = f->arg_list;
//  m_uint count = 1;
  nspc_push_value(env->curr);
  do {
    if(list->var_decl->array && check_array_empty(list->var_decl->array, "argument") < 0) {
      nspc_pop_value(env->curr);
      return -1;
    }
    if(scan2_arg_def_check(list) < 0 ||
        (list->var_decl->array && !(list->type = array_type(list->type, list->var_decl->array->depth)))) {
      nspc_pop_value(env->curr);
      return -1;
    }
    const Value v = list->var_decl->value ? list->var_decl->value : new_value(list->type, s_name(list->var_decl->xid));
    v->owner = env->curr;
    SET_FLAG(v, ae_flag_arg);
    if(GET_FLAG(list->td, ae_flag_const))
      SET_FLAG(v, ae_flag_const);
    if(f) {
      v->offset = f->stack_depth;
      f->stack_depth += list->type->size;
    }
    nspc_add_value(env->curr, list->var_decl->xid, v);
    list->var_decl->value = v;
//    ++count;
  } while((list = list->next));
  nspc_pop_value(env->curr);
  return 1;
}

ANN static Value scan2_func_assign(const Env env, const Func_Def d,
    const Func f, const Value v) {
  v->owner = env->curr;
  if(!(v->owner_class = env->class_def))
    SET_FLAG(v, ae_flag_global);
  else {
    if(GET_FLAG(f, ae_flag_member))
      SET_FLAG(v, ae_flag_member);
    else SET_FLAG(v, ae_flag_static);
    if(GET_FLAG(d, ae_flag_private))
      SET_FLAG(v, ae_flag_private);
  }
  d->func = v->d.func_ref = f;
  return f->value_ref = v;
}

ANN m_bool scan2_stmt_fptr(const Env env, const Stmt_Fptr ptr) { GWDEBUG_EXE
  struct Func_Def_ d;
  d.arg_list = ptr->args;
  if(nspc_lookup_func2(env->curr, ptr->xid))
    ERR_B(SCAN2_, ptr->td->pos,
          "function type '%s' already defined.", s_name(ptr->xid))
  if(d.arg_list && scan2_arg_def(env, &d) < 0)
    ERR_B(SCAN2_, ptr->td->pos, "in typedef '%s'", s_name(ptr->xid))
  SET_FLAG(ptr->value, ae_flag_checked);
  nspc_add_value(env->curr, ptr->xid, ptr->value);

  const Func_Def def = new_func_def(ptr->td, ptr->xid, ptr->args, NULL, ptr->td->flag);
  def->ret_type = ptr->ret_type;
  ptr->func = new_func(s_name(ptr->xid), def);
  ptr->value->d.func_ref = ptr->func;
  ptr->func->value_ref = ptr->value;
  ptr->type->d.func = ptr->func;
  SET_FLAG(ptr->value, ae_flag_func);
  if(env->class_def) {
    if(GET_FLAG(ptr->td, ae_flag_global)) {
      SET_FLAG(ptr->value, ae_flag_global);
      SET_FLAG(ptr->func, ae_flag_global);
    } else if(!GET_FLAG(ptr->td, ae_flag_static)) {
      SET_FLAG(ptr->value, ae_flag_member);
      SET_FLAG(ptr->func, ae_flag_member);
    } else {
      SET_FLAG(ptr->value, ae_flag_static);
      SET_FLAG(ptr->func, ae_flag_static);
    }
    ptr->value->owner_class = env->class_def;
  }
  nspc_add_func(ptr->type->owner, ptr->xid, ptr->func);
  ADD_REF(ptr->func);
  return 1;
}

ANN static m_bool scan2_stmt_type(const Env env, const Stmt_Type stmt) { GWDEBUG_EXE
  return stmt->type->def ? scan2_class_def(env, stmt->type->def) : 1;
}

ANN static m_bool scan2_exp_primary(const Env env, const Exp_Primary* prim) { GWDEBUG_EXE
  if(prim->primary_type == ae_primary_hack)
    CHECK_BB(scan2_exp(env, prim->d.exp))
  return 1;
}

ANN static m_bool scan2_exp_array(const Env env, const Exp_Array* array) { GWDEBUG_EXE
  CHECK_BB(scan2_exp(env, array->base))
  return scan2_exp(env, array->array->exp);
}

ANN static m_bool scan2_exp_binary(const Env env, const Exp_Binary* bin) { GWDEBUG_EXE
  CHECK_BB(scan2_exp(env, bin->lhs))
  return scan2_exp(env, bin->rhs);
}

ANN static m_bool scan2_exp_cast(const Env env, const Exp_Cast* cast) { GWDEBUG_EXE
  return scan2_exp(env, cast->exp);
}

ANN static m_bool scan2_exp_post(const Env env, const Exp_Postfix* post) { GWDEBUG_EXE
  return scan2_exp(env, post->exp);
}

ANN static m_bool scan2_exp_dur(const Env env, const Exp_Dur* dur) { GWDEBUG_EXE
  CHECK_BB(scan2_exp(env, dur->base))
  return scan2_exp(env, dur->unit);
}

ANN2(1,2) static m_bool scan2_exp_call1(const Env env, const Exp func, const Exp args) { GWDEBUG_EXE
  CHECK_BB(scan2_exp(env, func))
  return args ? scan2_exp(env, args) : 1;
}

ANN static m_bool scan2_exp_call(const Env env, const Exp_Func* exp_func) { GWDEBUG_EXE
  if(exp_func->tmpl)
    return 1;
  return scan2_exp_call1(env, exp_func->func, exp_func->args);
}

ANN static m_bool scan2_exp_dot(const Env env, const Exp_Dot* member) { GWDEBUG_EXE
  return scan2_exp(env, member->base);
}

ANN static m_bool scan2_exp_if(const Env env, const Exp_If* exp_if) { GWDEBUG_EXE
  CHECK_BB(scan2_exp(env, exp_if->cond))
  CHECK_BB(scan2_exp(env, exp_if->if_exp))
  return scan2_exp(env, exp_if->else_exp);
}

ANN static m_bool scan2_exp_spork(const Env env, const Stmt code) { GWDEBUG_EXE
  return scan2_stmt(env, code);
}

static m_bool scan2_exp(const Env env, Exp exp) { GWDEBUG_EXE
  do {
    switch(exp->exp_type) {
      case ae_exp_primary:
        CHECK_BB(scan2_exp_primary(env, &exp->d.exp_primary))
        break;
      case ae_exp_decl:
        CHECK_BB(scan2_exp_decl(env, &exp->d.exp_decl))
        break;
      case ae_exp_unary:
        if(exp->d.exp_unary.op == op_spork && exp->d.exp_unary.code)
          CHECK_BB(scan2_exp_spork(env, exp->d.exp_unary.code))
        break;
      case ae_exp_binary:
        CHECK_BB(scan2_exp_binary(env, &exp->d.exp_binary))
        break;
      case ae_exp_post:
        CHECK_BB(scan2_exp_post(env, &exp->d.exp_post))
        break;
      case ae_exp_cast:
        CHECK_BB(scan2_exp_cast(env, &exp->d.exp_cast))
        break;
      case ae_exp_call:
        CHECK_BB(scan2_exp_call(env, &exp->d.exp_func))
        break;
      case ae_exp_array:
        CHECK_BB(scan2_exp_array(env, &exp->d.exp_array))
        break;
      case ae_exp_dot:
        CHECK_BB(scan2_exp_dot(env, &exp->d.exp_dot))
        break;
      case ae_exp_if:
        CHECK_BB(scan2_exp_if(env, &exp->d.exp_if))
        break;
      case ae_exp_dur:
        CHECK_BB(scan2_exp_dur(env, &exp->d.exp_dur))
        break;
      OPTIMIZE_DEFAULT
    }
  } while((exp = exp->next));
  return 1;
}

ANN static m_bool scan2_stmt_code(const Env env, const Stmt_Code stmt) { GWDEBUG_EXE
  return stmt->stmt_list ? scan2_stmt_list(env, stmt->stmt_list) : 1;
}

ANN static m_bool scan2_stmt_if(const Env env, const Stmt_If stmt) { GWDEBUG_EXE
  CHECK_BB(scan2_exp(env, stmt->cond))
  CHECK_BB(scan2_stmt(env, stmt->if_body))
  if(stmt->else_body)
    CHECK_BB(scan2_stmt(env, stmt->else_body))
  return 1;
}

ANN static m_bool scan2_stmt_flow(const Env env, const struct Stmt_Flow_* stmt) { GWDEBUG_EXE
  CHECK_BB(scan2_exp(env, stmt->cond))
  return scan2_stmt(env, stmt->body);
}

ANN static m_bool scan2_stmt_for(const Env env, const Stmt_For stmt) { GWDEBUG_EXE
  CHECK_BB(scan2_stmt(env, stmt->c1))
  CHECK_BB(scan2_stmt(env, stmt->c2))
  if(stmt->c3)
    CHECK_BB(scan2_exp(env, stmt->c3))
  return scan2_stmt(env, stmt->body);
}

ANN static m_bool scan2_stmt_auto(const Env env, const Stmt_Auto stmt) { GWDEBUG_EXE
  CHECK_BB(scan2_exp(env, stmt->exp))
  return scan2_stmt(env, stmt->body);
}

ANN static m_bool scan2_stmt_loop(const Env env, const Stmt_Loop stmt) { GWDEBUG_EXE
  CHECK_BB(scan2_exp(env, stmt->cond))
  return scan2_stmt(env, stmt->body);
}

ANN static m_bool scan2_stmt_return(const Env env, const Stmt_Exp stmt) { GWDEBUG_EXE
  return stmt->val ? scan2_exp(env, stmt->val) : 1;
}

ANN static m_bool scan2_stmt_switch(const Env env, const Stmt_Switch stmt) { GWDEBUG_EXE
  return scan2_exp(env, stmt->val);
}

ANN static m_bool scan2_stmt_case(const Env env, const Stmt_Exp stmt) { GWDEBUG_EXE
  return stmt->val ? scan2_exp(env, stmt->val) : 1;
}

__attribute__((returns_nonnull))
ANN static Map scan2_label_map(const Env env) { GWDEBUG_EXE
  Map m, label = env_label(env);
  const m_uint* key = env->class_def && !env->func ?
    (m_uint*)env->class_def : (m_uint*)env->func;
  if(!label->ptr)
    map_init(label);
  if(!(m = (Map)map_get(label, (vtype)key))) {
    m = new_map();
    map_set(label, (vtype)key, (vtype)m);
  }
  return m;
}

ANN static m_bool scan2_stmt_jump(const Env env, const Stmt_Jump stmt) { GWDEBUG_EXE
  if(stmt->is_label) {
    const Map m = scan2_label_map(env);
    if(map_get(m, (vtype)stmt->name)) {
      Stmt_Jump l = (Stmt_Jump)map_get(m, (vtype)stmt->name);
      vector_release(&l->data.v);
      ERR_B(SCAN2_, stmt->self->pos,
            "label '%s' already defined", s_name(stmt->name))
    }
    map_set(m, (vtype)stmt->name, (vtype)stmt);
    vector_init(&stmt->data.v);
  }
  return 1;
}

ANN m_bool scan2_stmt_enum(const Env env, const Stmt_Enum stmt) { GWDEBUG_EXE
  const Value v = nspc_lookup_value1(env->curr, stmt->xid);
  if(v)
    ERR_B(SCAN2_, stmt->self->pos,
          "'%s' already declared as variable", s_name(stmt->xid))
  return 1;
}

ANN m_bool scan2_stmt_union(const Env env, const Stmt_Union stmt) { GWDEBUG_EXE
  Decl_List l = stmt->l;
  m_uint class_scope;
  const m_bool global = GET_FLAG(stmt, ae_flag_global);
  if(stmt->xid)
    env_push(env, stmt->value->type, stmt->value->type->nspc, &class_scope);
  else if(global)
    env_push(env, NULL, env->global_nspc, &class_scope);
  do CHECK_BB(scan2_exp(env, l->self))
  while((l = l->next));
  if(stmt->xid || global)
    env_pop(env, class_scope);
  return 1;
}

ANN static m_bool scan2_stmt(const Env env, const Stmt stmt) { GWDEBUG_EXE
  switch(stmt->stmt_type) {
    case ae_stmt_exp:
      if(stmt->d.stmt_exp.val)
        CHECK_BB(scan2_exp(env, stmt->d.stmt_exp.val));
      break;
    case ae_stmt_return:
      CHECK_BB(scan2_stmt_return(env, &stmt->d.stmt_exp))
      break;
    case ae_stmt_code:
        NSPC(CHECK_BB(scan2_stmt_code(env, &stmt->d.stmt_code)))
      break;
    case ae_stmt_if:
      NSPC(CHECK_BB(scan2_stmt_if(env, &stmt->d.stmt_if)))
      break;
    case ae_stmt_while:
      NSPC(CHECK_BB(scan2_stmt_flow(env, &stmt->d.stmt_flow)))
      break;
    case ae_stmt_until:
      NSPC(CHECK_BB(scan2_stmt_flow(env, &stmt->d.stmt_flow)))
      break;
    case ae_stmt_for:
      NSPC(CHECK_BB(scan2_stmt_for(env, &stmt->d.stmt_for)))
      break;
    case ae_stmt_auto:
      NSPC(CHECK_BB(scan2_stmt_auto(env, &stmt->d.stmt_auto)))
      break;
    case ae_stmt_loop:
      NSPC(CHECK_BB(scan2_stmt_loop(env, &stmt->d.stmt_loop)))
      break;
    case ae_stmt_switch:
      NSPC(CHECK_BB(scan2_stmt_switch(env, &stmt->d.stmt_switch)))
      break;
    case ae_stmt_case:
      CHECK_BB(scan2_stmt_case(env, &stmt->d.stmt_exp))
      break;
    case ae_stmt_jump:
      CHECK_BB(scan2_stmt_jump(env, &stmt->d.stmt_jump))
      break;
    case ae_stmt_continue:
    case ae_stmt_break:
      break;
    case ae_stmt_enum:
      CHECK_BB(scan2_stmt_enum(env, &stmt->d.stmt_enum))
      break;
    case ae_stmt_fptr:
      CHECK_BB(scan2_stmt_fptr(env, &stmt->d.stmt_fptr))
      break;
    case ae_stmt_type:
      CHECK_BB(scan2_stmt_type(env, &stmt->d.stmt_type))
      break;
    case ae_stmt_union:
      CHECK_BB(scan2_stmt_union(env, &stmt->d.stmt_union))
      break;
  }
  return 1;
}

ANN static m_bool scan2_stmt_list(const Env env, Stmt_List list) { GWDEBUG_EXE
  do CHECK_BB(scan2_stmt(env, list->stmt))
  while((list = list->next));
  return 1;
}

ANN static m_bool scan2_func_def_overload(const Func_Def f, const Value overload) { GWDEBUG_EXE
  const m_bool base = tmpl_list_base(f->tmpl);
  const m_bool tmpl = GET_FLAG(overload, ae_flag_template);
  if(isa(overload->type, t_function) < 0)
    ERR_B(SCAN2_, f->td->pos,
          "function name '%s' is already used by another value",
          s_name(f->name))
  if((!tmpl && base) ||
      (tmpl && !base && !GET_FLAG(f, ae_flag_template)))
    ERR_B(SCAN2_, f->td->pos,
          "must override template function with template")
  return 1;
}

ANN2(1, 2) static m_bool scan2_func_def_template (const Env env, const Func_Def f, const Value overload) { GWDEBUG_EXE
  const m_str func_name = s_name(f->name);
  const Func func = new_func(func_name, f);
  const m_uint len = strlen(func_name) +
    num_digit(overload ? overload->offset + 1 : 0) +
    strlen(env->curr->name) + 13;

  char name[len];
  if(overload)
    func->next = overload->d.func_ref->next;
  if(env->class_def) {
    if(GET_FLAG(env->class_def, ae_flag_template))
      SET_FLAG(func, ae_flag_ref);
    if(!GET_FLAG(f, ae_flag_static))
      SET_FLAG(func, ae_flag_member);
  }
  const Type type = type_copy(t_function);
  type->name = func_name;
  type->owner = env->curr;
  const Value value = new_value(type, func_name);
  SET_FLAG(value, ae_flag_func);
  CHECK_OB(scan2_func_assign(env, f, func, value))
  SET_FLAG(value, ae_flag_const | ae_flag_checked | ae_flag_template);
  if(overload)
    ++overload->offset;
  else {
    ADD_REF(type);
    ADD_REF(value);
    ADD_REF(func);
    nspc_add_value(env->curr, f->name, value);
  }
  snprintf(name, len, "%s<template>@%" INT_F "@%s", func_name,
           overload ? overload->offset : 0, env->curr->name);
  nspc_add_value(env->curr, insert_symbol(name), value);
  return 1;
}

ANN static m_bool scan2_func_def_builtin(const Func func, const m_str name) { GWDEBUG_EXE
  SET_FLAG(func, ae_flag_builtin);
  func->code = new_vm_code(NULL, func->def->stack_depth, 1, name);
  if(GET_FLAG(func, ae_flag_member))
    SET_FLAG(func->code, _NEED_THIS_);
  UNSET_FLAG(func->code, NATIVE_NOT);
  func->code->native_func = (m_uint)func->def->d.dl_func_ptr;
  return 1;
}

ANN static m_bool scan2_func_def_op(const Env env, const Func_Def f) { GWDEBUG_EXE
  const Operator ret = name2op(s_name(f->name));
  const Type l = GET_FLAG(f, ae_flag_unary) ? NULL :
    f->arg_list->var_decl->value->type;
  const Type r = GET_FLAG(f, ae_flag_unary) ? f->arg_list->var_decl->value->type :
    f->arg_list->next ? f->arg_list->next->var_decl->value->type : NULL;
  struct Op_Import opi = { ret, l, r, f->ret_type, NULL, NULL, 0};
  CHECK_BB(env_add_op(env, &opi))
  if(env->class_def) {
    if(env->class_def == l)
      REM_REF(l)
    if(env->class_def == r)
      REM_REF(r)
    if(env->class_def == f->ret_type)
      REM_REF(f->ret_type)
  }
  return 1;
}

ANN static m_bool scan2_func_def_code(const Env env, const Func_Def f) { GWDEBUG_EXE
  const Func former = env->func;
  env->func = f->func;
  nspc_push_value(env->curr);
  const m_bool ret = scan2_stmt_code(env, &f->d.code->d.stmt_code);
  if(ret < 0)
    err_msg(SCAN2_, f->td->pos, "... in function '%s'", s_name(f->name));
  nspc_pop_value(env->curr);
  env->func = former;
  return ret;
}

ANN2(1,2) static m_bool scan2_func_def_add(const Env env, const Value value, const Value overload) { GWDEBUG_EXE
  const m_str name = s_name(value->d.func_ref->def->name);
  const Func func = value->d.func_ref;

  if(overload) {
    if(overload->d.func_ref->def->ret_type)
      if(!GET_FLAG(func->def, ae_flag_template))
        if(func->def->ret_type->xid != overload->d.func_ref->def->ret_type->xid) {
          err_msg(SCAN2_,  func->def->td->pos,
              "function signatures differ in return type... '%s' and '%s'",
                  func->def->ret_type->name,
                  overload->d.func_ref->def->ret_type->name);
          if(env->class_def)
            ERR_B(SCAN2_, func->def->td->pos,
                    "function '%s.%s' matches '%s.%s' but cannot overload...",
                    env->class_def->name, name,
                    value->owner_class->name, name)
        }
  }
  SET_FLAG(value, ae_flag_checked);
  return 1;
}

ANN static void scan2_func_def_flag(const Func_Def f) { GWDEBUG_EXE
  if(GET_FLAG(f, ae_flag_builtin))
    SET_FLAG(f->func->value_ref, ae_flag_builtin);
  if(GET_FLAG(f, ae_flag_dtor))
    SET_FLAG(f->func, ae_flag_dtor);
  if(GET_FLAG(f, ae_flag_dtor))
    SET_FLAG(f->func->value_ref->owner_class, ae_flag_dtor);
  else if(GET_FLAG(f, ae_flag_variadic))
    f->stack_depth += SZ_INT;
  SET_FLAG(f->func->value_ref, ae_flag_const);
}

ANN m_str func_tmpl_name(const Env env, const Func_Def f, const m_uint len) {
  const m_str func_name = s_name(f->name);
  struct Vector_ v;
  ID_List id = f->tmpl->list;
  m_uint tlen = 0;
  vector_init(&v);
  do {
    const Type t = nspc_lookup_type0(env->curr, id->xid);
    vector_add(&v, (vtype)t);
    tlen += strlen (t->name);
    if(id->next )
      ++tlen;
  }
  while((id = id->next));
  char name[len + tlen + 3];
  char tmpl_name[tlen + 1];
  memset(name, 0, len + tlen + 3);
  memset(tmpl_name, 0, tlen + 1);
  tmpl_name[0] = '\0';
  m_str str = tmpl_name;
  for(m_uint i = 0; i < vector_size(&v); ++i) {
    const m_str name = ((Type)vector_at(&v, i))->name;
    strcpy(str, name);
    str += strlen(name);
    if(i + 1 < vector_size(&v)) {
      strcpy(str, ",");
      ++str;
    }
  }
  tmpl_name[len+1] = '\0';
  vector_release(&v);
  snprintf(name, len + tlen + 3, "%s<%s>@%" INT_F "@%s",
  func_name, tmpl_name, f->tmpl->base, env->curr->name);
  return s_name(insert_symbol(name));
}

ANN2(1,2,4) static Value func_create(const Env env, const Func_Def f,
    const Value overload, const m_str func_name) {
  const Func func = new_func(func_name, f);
  nspc_add_func(env->curr, insert_symbol(func->name), func);
  if(env->class_def) {
    if(GET_FLAG(env->class_def, ae_flag_template))
      SET_FLAG(func, ae_flag_ref);
    if(!GET_FLAG(f, ae_flag_static))
      SET_FLAG(func, ae_flag_member);
  }
  if(GET_FLAG(f, ae_flag_builtin))
    CHECK_BO(scan2_func_def_builtin(func, func->name))
  const Type type = new_type(t_function->xid, func_name, t_function);
  type->size = SZ_INT;
  if(GET_FLAG(func, ae_flag_member))
    type->size += SZ_INT;
  type->d.func = func;
  const Value value = new_value(type, func_name);
  SET_FLAG(value, ae_flag_func);
  nspc_add_value(env->curr, !overload ?
      f->name : insert_symbol(func->name), value);
  CHECK_OO(scan2_func_assign(env, f, func, value))
  scan2_func_def_flag(f);
  if(overload) {
    func->next = overload->d.func_ref->next;
    overload->d.func_ref->next = func;
  }
  if(GET_FLAG(f->td, ae_flag_ref))
    CHECK_BO(prim_ref(f->td, f->ret_type))
  if(GET_FLAG(func, ae_flag_member))
    f->stack_depth += SZ_INT;
  return value;
}

ANN m_bool scan2_func_def(const Env env, const Func_Def f) { GWDEBUG_EXE
  Value value    = NULL;

  f->stack_depth = 0;

  const Value overload = nspc_lookup_value0(env->curr, f->name);
  m_str func_name = s_name(f->name);
  const m_uint len = strlen(func_name) +
    num_digit(overload ? overload->offset + 1 : 0) +
    strlen(env->curr->name) + 3;

  if(overload)
    CHECK_BB(scan2_func_def_overload(f, overload))

  if(tmpl_list_base(f->tmpl))
    return scan2_func_def_template(env, f, overload);

  if(!f->tmpl) {
    char name[len];
    snprintf(name, len, "%s@%" INT_F "@%s", func_name,
           overload ? ++overload->offset : 0, env->curr->name);
    func_name = s_name(insert_symbol(name));
  } else {
    func_name = func_tmpl_name(env, f, len);
    const Func func = nspc_lookup_func1(env->curr, insert_symbol(func_name));
    if(func && f->arg_list)
      return scan2_arg_def(env, f);
  }
  const Func base = get_func(env, f);
  if(!base) {
    m_uint class_scope;
    if(GET_FLAG(f, ae_flag_global))
    env_push(env, NULL, env->global_nspc, &class_scope);
      CHECK_OB((value = func_create(env, f, overload, func_name)))
    if(GET_FLAG(f, ae_flag_global))
    env_pop(env, class_scope);

  } else
    f->func = base;
  if(f->arg_list && scan2_arg_def(env, f) < 0)
    ERR_B(SCAN2_, f->td->pos,
          "\t... in function '%s'\n", s_name(f->name))
  if(!base) {
    if(GET_FLAG(f, ae_flag_op))
      return scan2_func_def_op(env, f);
    CHECK_BB(scan2_func_def_add(env, value, overload))
  }
  return (!GET_FLAG(f, ae_flag_builtin) && f->d.code->d.stmt_code.stmt_list) ?
      scan2_func_def_code(env, f) : 1;
}

ANN static m_bool scan2_section(const Env env, const Section* section) { GWDEBUG_EXE
  const ae_Section_Type t = section->section_type;
  if(t == ae_section_stmt)
    CHECK_BB(scan2_stmt_list(env, section->d.stmt_list))
  else if(t == ae_section_func)
    CHECK_BB(scan2_func_def(env, section->d.func_def))
  else if(t == ae_section_class)
    CHECK_BB(scan2_class_def(env, section->d.class_def))
  return 1;
}

ANN m_bool scan2_class_def(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  if(tmpl_class_base(class_def->tmpl))
    return 1;
  if(class_def->ext) {
    const Type t = class_def->type->parent->array_depth ?
      array_base(class_def->type->parent) : class_def->type->parent;
    if(!GET_FLAG(t, ae_flag_scan2) && GET_FLAG(class_def->ext, ae_flag_typedef))
      CHECK_BB(scan2_class_def(env, t->def))
    if(class_def->ext->array)
      CHECK_BB(scan2_exp(env, class_def->ext->array->exp))
  }
  if(class_def->body) {
    Class_Body body = class_def->body;
    m_uint class_scope;
    env_push(env, class_def->type, class_def->type->nspc, &class_scope);
    do CHECK_BB(scan2_section(env, body->section))
    while((body = body->next));
    env_pop(env, class_scope);
  }
  SET_FLAG(class_def->type, ae_flag_scan2);
  return 1;
}

ANN m_bool scan2_ast(const Env env, Ast ast) { GWDEBUG_EXE
  do CHECK_BB(scan2_section(env, ast->section))
  while((ast = ast->next));
  return 1;
}
