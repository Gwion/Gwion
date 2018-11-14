#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "map.h"
#include "absyn.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "value.h"
#include "func.h"
#include "traverse.h"
#include "optim.h"
#include "parse.h"
#include "nspc.h"
#include "operator.h"

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
    ERR_B(decl->self->pos, "Type '%s' is abstract, declare as ref"
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
    ERR_B(list->var_decl->pos, "cannot declare variables of size '0' (i.e. 'void')...")
  if(isres(list->var_decl->xid) > 0)
    return -1;
  if(GET_FLAG(list->td, ae_flag_ref))
    CHECK_BB(prim_ref(list->td, list->type))
  return 1;
}

ANN2(1) static m_bool scan2_arg_def(const Env env, const Func_Def f) { GWDEBUG_EXE
  Arg_List list = f->arg_list;
  nspc_push_value(env->curr);
  do {
    if(scan2_arg_def_check(list) < 0 ||
        (list->var_decl->array && !(list->type = array_type(list->type, list->var_decl->array->depth)))) {
      nspc_pop_value(env->curr);
      return -1;
    }
    const Value v = list->var_decl->value ? list->var_decl->value : new_value(list->type, s_name(list->var_decl->xid));
    SET_FLAG(v, ae_flag_arg);
    if(GET_FLAG(list->td, ae_flag_const))
      SET_FLAG(v, ae_flag_const);
    if(f) {
      v->offset = f->stack_depth;
      f->stack_depth += list->type->size;
    }
    nspc_add_value(env->curr, list->var_decl->xid, v);
    list->var_decl->value = v;
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
  if(d.arg_list && scan2_arg_def(env, &d) < 0)
    ERR_B(ptr->td->xid->pos, "in typedef '%s'", s_name(ptr->xid))
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
  return 1;
}

ANN static inline m_bool scan2_stmt_type(const Env env, const Stmt_Type stmt) { GWDEBUG_EXE
  return stmt->type->def ? scan2_class_def(env, stmt->type->def) : 1;
}

ANN static inline Value prim_value(const Env env, const Symbol s) {
  const Value value = nspc_lookup_value1(env->curr, s);
  if(env->class_def) {
    const Value v = value ? value : find_value(env->class_def, s);
    return v;
  }
  return value;
}

ANN static inline m_bool scan2_exp_primary(const Env env, const Exp_Primary* prim) { GWDEBUG_EXE
  if(prim->primary_type == ae_primary_hack)
    CHECK_BB(scan2_exp(env, prim->d.exp))
  else if(prim->primary_type == ae_primary_id) {
    const Value v = prim_value(env, prim->d.var);
    if(v)
      SET_FLAG(v, ae_flag_used);
  }
  return 1;
}

ANN static inline m_bool scan2_exp_array(const Env env, const Exp_Array* array) { GWDEBUG_EXE
  CHECK_BB(scan2_exp(env, array->base))
  return scan2_exp(env, array->array->exp);
}


ANN static m_bool multi_decl(const Exp e, const Operator op) {
  if(e->exp_type == ae_exp_decl) {
    if(e->d.exp_decl.list->next)
      ERR_B(e->pos, "cant '%s' from/to a multi-variable declaration.", op2str(op))
    SET_FLAG(e->d.exp_decl.list->self->value, ae_flag_used);
  }
  return 1;
}

ANN static inline m_bool scan2_exp_binary(const Env env, const Exp_Binary* bin) { GWDEBUG_EXE
  CHECK_BB(scan2_exp(env, bin->lhs))
  CHECK_BB(scan2_exp(env, bin->rhs))
  CHECK_BB(multi_decl(bin->lhs, bin->op))
  return multi_decl(bin->rhs, bin->op);
}

ANN static inline m_bool scan2_exp_cast(const Env env, const Exp_Cast* cast) { GWDEBUG_EXE
  return scan2_exp(env, cast->exp);
}

ANN static inline m_bool scan2_exp_post(const Env env, const Exp_Postfix* post) { GWDEBUG_EXE
  return scan2_exp(env, post->exp);
}

ANN static inline m_bool scan2_exp_dur(const Env env, const Exp_Dur* dur) { GWDEBUG_EXE
  CHECK_BB(scan2_exp(env, dur->base))
  return scan2_exp(env, dur->unit);
}

ANN2(1,2) static inline m_bool scan2_exp_call1(const Env env, const restrict Exp func,
    const restrict Exp args) { GWDEBUG_EXE
  CHECK_BB(scan2_exp(env, func))
  return args ? scan2_exp(env, args) : 1;
}

ANN static inline m_bool scan2_exp_call(const Env env, const Exp_Call* exp_call) { GWDEBUG_EXE
  if(!exp_call->tmpl) // avoid unused var
    return scan2_exp_call1(env, exp_call->func, exp_call->args);
  return scan2_exp(env, exp_call->func);
}

ANN static inline m_bool scan2_exp_dot(const Env env, const Exp_Dot* member) { GWDEBUG_EXE
  return scan2_exp(env, member->base);
}

ANN static inline m_bool scan2_exp_if(const Env env, const Exp_If* exp_if) { GWDEBUG_EXE
  CHECK_BB(scan2_exp(env, exp_if->cond))
  CHECK_BB(scan2_exp(env, exp_if->if_exp))
  return scan2_exp(env, exp_if->else_exp);
}

ANN static m_bool scan2_exp_unary(const Env env, const Exp_Unary * unary) {
  if(unary->op == op_spork && unary->code)
    return scan2_stmt(env, unary->code);
  else if(unary->exp)
    return scan2_exp(env, unary->exp);
  return 1;
}

HANDLE_EXP_FUNC(scan2, m_bool, 1)

#define scan2_stmt_func(name, type, exp) describe_stmt_func(scan2, name, type, exp)
scan2_stmt_func(flow, Stmt_Flow, !(scan2_exp(env, stmt->cond) < 0 ||
    scan2_stmt(env, stmt->body) < 0) ? 1 : -1)
scan2_stmt_func(for, Stmt_For, !(scan2_stmt(env, stmt->c1) < 0 ||
    scan2_stmt(env, stmt->c2) < 0 ||
    (stmt->c3 && scan2_exp(env, stmt->c3) < 0) ||
    scan2_stmt(env, stmt->body) < 0) ? 1 : -1)

scan2_stmt_func(auto, Stmt_Auto, !(scan2_exp(env, stmt->exp) < 0 ||
    scan2_stmt(env, stmt->body) < 0) ? 1 : -1)
scan2_stmt_func(loop, Stmt_Loop, !(scan2_exp(env, stmt->cond) < 0 ||
    scan2_stmt(env, stmt->body) < 0) ? 1 : -1)
scan2_stmt_func(switch, Stmt_Switch, !(scan2_exp(env, stmt->val) < 0 ||
    scan2_stmt(env, stmt->stmt) < 0) ? 1 : -1)
scan2_stmt_func(if, Stmt_If, !(scan2_exp(env, stmt->cond) < 0 ||
    scan2_stmt(env, stmt->if_body) < 0 ||
    (stmt->else_body && scan2_stmt(env, stmt->else_body) < 0)) ? 1 : -1)

ANN static inline m_bool scan2_stmt_code(const Env env, const Stmt_Code stmt) { GWDEBUG_EXE
  if(stmt->stmt_list) {
    RET_NSPC(scan2_stmt_list(env, stmt->stmt_list))
  }
  return 1;
}

ANN static inline m_bool scan2_stmt_exp(const Env env, const Stmt_Exp stmt) { GWDEBUG_EXE
  return stmt->val ? scan2_exp(env, stmt->val) : 1;
}

ANN static inline m_bool scan2_stmt_case(const Env env, const Stmt_Exp stmt) { GWDEBUG_EXE
  return scan2_exp(env, stmt->val);
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
      const Stmt_Jump l = (Stmt_Jump)map_get(m, (vtype)stmt->name);
      vector_release(&l->data.v);
      ERR_B(stmt->self->pos, "label '%s' already defined", s_name(stmt->name))
    }
    map_set(m, (vtype)stmt->name, (vtype)stmt);
    vector_init(&stmt->data.v);
  }
  return 1;
}

ANN m_bool scan2_stmt_union(const Env env, const Stmt_Union stmt) { GWDEBUG_EXE
  Decl_List l = stmt->l;
  m_uint class_scope;
  const m_bool global = GET_FLAG(stmt, ae_flag_global);
  if(stmt->xid)
    env_push(env, stmt->value->type, stmt->value->type->nspc, &class_scope);
  else if(stmt->type_xid)
    env_push(env, stmt->type, stmt->type->nspc, &class_scope);
  else if(global)
    env_push(env, NULL, env->global_nspc, &class_scope);
  do CHECK_BB(scan2_exp(env, l->self))
  while((l = l->next));
  if(stmt->xid || stmt->type_xid || global)
    env_pop(env, class_scope);
  return 1;
}

static const _exp_func stmt_func[] = {
  (_exp_func)scan2_stmt_exp,  (_exp_func)scan2_stmt_flow, (_exp_func)scan2_stmt_flow,
  (_exp_func)scan2_stmt_for,  (_exp_func)scan2_stmt_auto, (_exp_func)scan2_stmt_loop,
  (_exp_func)scan2_stmt_if,   (_exp_func)scan2_stmt_code, (_exp_func)scan2_stmt_switch,
  (_exp_func)dummy_func,      (_exp_func)dummy_func,      (_exp_func)scan2_stmt_exp,
  (_exp_func)scan2_stmt_case, (_exp_func)scan2_stmt_jump, (_exp_func)dummy_func,
  (_exp_func)scan2_stmt_fptr, (_exp_func)scan2_stmt_type, (_exp_func)scan2_stmt_union,
};

ANN static m_bool scan2_stmt(const Env env, const Stmt stmt) { GWDEBUG_EXE
  return stmt_func[stmt->stmt_type](env, &stmt->d);
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
    ERR_B(f->td->xid->pos, "function name '%s' is already used by another value",
          s_name(f->name))
  if((!tmpl && base) ||
      (tmpl && !base && !GET_FLAG(f, ae_flag_template)))
    ERR_B(f->td->xid->pos, "must override template function with template")
  return 1;
}

ANN static Func scan_new_func(const Env env, const Func_Def f, const m_str name) {
  const Func func = new_func(name, f);
  if(env->class_def) {
    if(GET_FLAG(env->class_def, ae_flag_template))
      SET_FLAG(func, ae_flag_ref);
    if(!GET_FLAG(f, ae_flag_static))
      SET_FLAG(func, ae_flag_member);
  }
  return func;
}

ANN2(1, 2) static m_bool scan2_func_def_template (const Env env, const Func_Def f, const Value overload) { GWDEBUG_EXE
  const m_str func_name = s_name(f->name);
  const Func func = scan_new_func(env, f, func_name);
  if(overload)
    func->next = overload->d.func_ref->next;
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
    nspc_add_value(env->curr, f->name, value);
  }
  const m_uint len = strlen(func_name) +
    num_digit(overload ? overload->offset + 1 : 0) +
    strlen(env->curr->name) + 13;
  char name[len];
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
  const m_bool ret = scan2_stmt_code(env, &f->d.code->d.stmt_code);
  if(ret < 0)
    err_msg(f->td->xid->pos, "... in function '%s'", s_name(f->name));
  env->func = former;
  return ret;
}

ANN2(1,2) static m_bool scan2_func_def_add(const Env env,
    const restrict Value value, const restrict Value overload) { GWDEBUG_EXE
  const m_str name = s_name(value->d.func_ref->def->name);
  const Func func = value->d.func_ref;

  if(overload) {
    if(overload->d.func_ref->def->ret_type)
      if(!GET_FLAG(func->def, ae_flag_template))
        if(func->def->ret_type->xid != overload->d.func_ref->def->ret_type->xid) {
          err_msg(func->def->td->xid->pos,
              "function signatures differ in return type... '%s' and '%s'",
                  func->def->ret_type->name,
                  overload->d.func_ref->def->ret_type->name);
          if(env->class_def)
            ERR_B(func->def->td->xid->pos,
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
  else
    SET_FLAG(f->func, ae_flag_pure);
  if(GET_FLAG(f, ae_flag_dtor)) {
    SET_FLAG(f->func, ae_flag_dtor);
    SET_FLAG(f->func->value_ref->owner_class, ae_flag_dtor);
  }
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
    tlen += strlen(t->name);
    if(id->next)
      ++tlen;
  } while((id = id->next));
  char name[len + tlen + 3];
  char tmpl_name[tlen + 1];
  memset(name, 0, len + tlen + 3);
  memset(tmpl_name, 0, tlen + 1);
  tmpl_name[0] = '\0';
  m_str str = tmpl_name;
  for(m_uint i = 0; i < vector_size(&v); ++i) {
    const m_str s = ((Type)vector_at(&v, i))->name;
    strcpy(str, s);
    str += strlen(s);
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
  const Func func = scan_new_func(env, f, func_name);
  nspc_add_func(env->curr, insert_symbol(func->name), func);
  if(GET_FLAG(f, ae_flag_builtin))
    CHECK_BO(scan2_func_def_builtin(func, func->name))
  const Type type = new_type(t_function->xid, func_name, t_function);
  type->size = SZ_INT;
  type->owner = env->curr;
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
    if(func)
      return f->arg_list ? scan2_arg_def(env, f) : 1;
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
    ERR_B(f->td->xid->pos, "\t... in function '%s'\n", s_name(f->name))
  if(!GET_FLAG(f, ae_flag_builtin) && f->d.code->d.stmt_code.stmt_list)
      CHECK_BB(scan2_func_def_code(env, f))
  if(!base) {
    CHECK_BB(scan2_func_def_add(env, value, overload))
    if(GET_FLAG(f, ae_flag_op))
      CHECK_BB(scan2_func_def_op(env, f))
  }
  return 1;
}

DECL_SECTION_FUNC(scan2)

ANN static m_bool scan2_class_parent(const Env env, const Class_Def class_def) {
  const Type t = class_def->type->parent->array_depth ?
    array_base(class_def->type->parent) : class_def->type->parent;
  if(!GET_FLAG(t, ae_flag_scan2) && GET_FLAG(class_def->ext, ae_flag_typedef))
    CHECK_BB(scan2_class_def(env, t->def))
  if(class_def->ext->array)
    CHECK_BB(scan2_exp(env, class_def->ext->array->exp))
  return 1;
}

ANN static m_bool scan2_class_body(const Env env, const Class_Def class_def) {
  m_uint class_scope;
  env_push(env, class_def->type, class_def->type->nspc, &class_scope);
  Class_Body body = class_def->body;
  do CHECK_BB(scan2_section(env, body->section))
  while((body = body->next));
  env_pop(env, class_scope);
  return 1;
}

ANN m_bool scan2_class_def(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  if(tmpl_class_base(class_def->tmpl))
    return 1;
  if(class_def->ext)
    CHECK_BB(scan2_class_parent(env, class_def))
  if(class_def->body)
    CHECK_BB(scan2_class_body(env, class_def))
  SET_FLAG(class_def->type, ae_flag_scan2);
  return 1;
}

ANN m_bool scan2_ast(const Env env, Ast ast) { GWDEBUG_EXE
  do CHECK_BB(scan2_section(env, ast->section))
  while((ast = ast->next));
  return 1;
}
