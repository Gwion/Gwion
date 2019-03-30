#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "value.h"
#include "func.h"
#include "template.h"
#include "optim.h"
#include "parse.h"
#include "nspc.h"
#include "operator.h"

ANN static m_bool scan2_exp(const Env, const Exp);
ANN static m_bool scan2_stmt(const Env, const Stmt);
ANN static m_bool scan2_stmt_list(const Env, Stmt_List);
extern ANN m_bool scan1_class_def(const Env, const Class_Def);
ANN m_bool scan2_class_def(const Env, const Class_Def);

ANN static m_bool scan2_exp_decl_template(const Env env, const Exp_Decl* decl) { GWDEBUG_EXE
  CHECK_BB(template_push_types(env, decl->base->tmpl->list.list, decl->td->types));
  CHECK_BB(scan1_class_def(env, decl->type->def))
  CHECK_BB(scan2_class_def(env, decl->type->def))
  nspc_pop_type(env->curr);
  return GW_OK;
}

ANN m_bool scan2_exp_decl(const Env env, const Exp_Decl* decl) { GWDEBUG_EXE
  Var_Decl_List list = decl->list;
  const Type type = decl->type;
  if(GET_FLAG(type, template) && !GET_FLAG(type, scan2))
    CHECK_BB(scan2_exp_decl_template(env, decl))
  m_uint scope;
  const m_bool global = GET_FLAG(decl->td, global);
  if(global)
   scope = env_push_global(env);
  do {
    const Var_Decl var = list->self;
    const Array_Sub array = var->array;
    if(array && array->exp)
      CHECK_BB(scan2_exp(env, array->exp))
    nspc_add_value(env->curr, var->xid, var->value);
  } while((list = list->next));
  if(global)
    env_pop(env, scope);
  return GW_OK;
}

ANN static Value arg_value(const Env env, const Arg_List list) {
  const Var_Decl var = list->var_decl;
  if(!var->value) {
    const Value v = new_value(env->gwion, list->type,
      var->xid ? s_name(var->xid) : s_name(insert_symbol((m_str)var)));
    if(list->td)
      v->flag = list->td->flag | ae_flag_arg;
    return v;
  }
  var->value->type = list->type;
  return var->value;
}

ANN static m_bool scan2_args(const Env env, const Func_Def f) { GWDEBUG_EXE
  Arg_List list = f->arg_list;
  do {
    const Var_Decl var = list->var_decl;
    if(var->array)
      list->type = array_type(list->type, var->array->depth);
    var->value = arg_value(env, list);
    var->value->offset = f->stack_depth;
    f->stack_depth += list->type->size;
  } while((list = list->next));
  return GW_OK;
}

ANN static Value scan2_func_assign(const Env env, const Func_Def d,
    const Func f, const Value v) {
  v->owner = env->curr;
  SET_FLAG(v, func | ae_flag_const);
  if(!(v->owner_class = env->class_def))
    SET_FLAG(v, global);
  else {
    if(GET_FLAG(f, member))
      SET_FLAG(v, member);
    else SET_FLAG(v, static);
    SET_ACCESS(d, v)
  }
  d->func = v->d.func_ref = f;
  return f->value_ref = v;
}

ANN m_bool scan2_stmt_fptr(const Env env, const Stmt_Fptr ptr) { GWDEBUG_EXE
  struct Func_Def_ d = { .stack_depth=0 };
  d.arg_list = ptr->args;
  if(d.arg_list)
    CHECK_BB(scan2_args(env, &d))
  const Func_Def def = new_func_def(ptr->td, ptr->xid, ptr->args, NULL, ptr->td->flag);
  def->ret_type = ptr->ret_type;
  def->stack_depth = d.stack_depth;
  ptr->func = new_func(s_name(ptr->xid), def);
  ptr->value->d.func_ref = ptr->func;
  ptr->func->value_ref = ptr->value;
  ptr->type->d.func = ptr->func;
  SET_FLAG(ptr->value, func | ae_flag_checked);
  if(env->class_def) {
    if(GET_FLAG(ptr->td, global)) {
      SET_FLAG(ptr->value, global);
      SET_FLAG(ptr->func, global);
    } else if(!GET_FLAG(ptr->td, static)) {
      SET_FLAG(ptr->value, member);
      SET_FLAG(ptr->func, member);
      def->stack_depth += SZ_INT;
    } else {
      SET_FLAG(ptr->value, static);
      SET_FLAG(ptr->func, static);
    }
    ptr->value->owner_class = env->class_def;
  }
  nspc_add_value(env->curr, ptr->xid, ptr->value);
  nspc_add_func(ptr->type->owner, ptr->xid, ptr->func);
  return GW_OK;
}

ANN static inline m_bool scan2_stmt_type(const Env env, const Stmt_Type stmt) { GWDEBUG_EXE
  return stmt->type->def ? scan2_class_def(env, stmt->type->def) : 1;
}

ANN static inline Value prim_value(const Env env, const Symbol s) {
  const Value value = nspc_lookup_value1(env->curr, s);
  if(env->class_def)
    return value ?: find_value(env->class_def, s);
  return value;
}

ANN static inline m_bool scan2_exp_primary(const Env env, const Exp_Primary* prim) { GWDEBUG_EXE
  if(prim->primary_type == ae_primary_hack) {
    CHECK_BB(scan2_exp(env, prim->d.exp))
    Exp e = prim->d.exp;
    do {
      if(e->exp_type == ae_exp_decl) {
        Var_Decl_List l = e->d.exp_decl.list;
        do {
           const Value v = l->self->value;
           SET_FLAG(v, used);
        }while ((l = l->next));
      }
    } while((e = e->next));
  } else if(prim->primary_type == ae_primary_id) {
    const Value v = prim_value(env, prim->d.var);
    if(v)
      SET_FLAG(v, used);
  }
  return GW_OK;
}

ANN static inline m_bool scan2_exp_array(const Env env, const Exp_Array* array) { GWDEBUG_EXE
  CHECK_BB(scan2_exp(env, array->base))
  return scan2_exp(env, array->array->exp);
}


ANN static m_bool multi_decl(const Exp e, const Operator op) {
  if(e->exp_type == ae_exp_decl) {
    if(e->d.exp_decl.list->next)
      ERR_B(e->pos, "cant '%s' from/to a multi-variable declaration.", op2str(op))
    SET_FLAG(e->d.exp_decl.list->self->value, used);
  }
  return GW_OK;
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

ANN2(1,2) static inline m_bool scan2_exp_call1(const Env env, const restrict Exp func,
    const restrict Exp args) { GWDEBUG_EXE
  CHECK_BB(scan2_exp(env, func))
  return args ? scan2_exp(env, args) : 1;
}

ANN static inline m_bool scan2_exp_call(const Env env, const Exp_Call* exp_call) { GWDEBUG_EXE
    return scan2_exp_call1(env, exp_call->func, exp_call->args);
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
  if((unary->op == op_spork || unary->op == op_fork) && unary->code)
    return scan2_stmt(env, unary->code);
  else if(unary->exp)
    return scan2_exp(env, unary->exp);
  return GW_OK;
}

#define scan2_exp_lambda dummy_func
HANDLE_EXP_FUNC(scan2, m_bool, 1)

#define scan2_stmt_func(name, type, prolog, exp) describe_stmt_func(scan2, name, type, prolog, exp)
scan2_stmt_func(flow, Stmt_Flow,, !(scan2_exp(env, stmt->cond) < 0 ||
    scan2_stmt(env, stmt->body) < 0) ? 1 : -1)
scan2_stmt_func(for, Stmt_For,, !(scan2_stmt(env, stmt->c1) < 0 ||
    scan2_stmt(env, stmt->c2) < 0 ||
    (stmt->c3 && scan2_exp(env, stmt->c3) < 0) ||
    scan2_stmt(env, stmt->body) < 0) ? 1 : -1)
scan2_stmt_func(auto, Stmt_Auto,, !(scan2_exp(env, stmt->exp) < 0 ||
    scan2_stmt(env, stmt->body) < 0) ? 1 : -1)
scan2_stmt_func(loop, Stmt_Loop,, !(scan2_exp(env, stmt->cond) < 0 ||
    scan2_stmt(env, stmt->body) < 0) ? 1 : -1)
scan2_stmt_func(switch, Stmt_Switch,, !(scan2_exp(env, stmt->val) < 0 ||
    scan2_stmt(env, stmt->stmt) < 0) ? 1 : -1)
scan2_stmt_func(if, Stmt_If,, !(scan2_exp(env, stmt->cond) < 0 ||
    scan2_stmt(env, stmt->if_body) < 0 ||
    (stmt->else_body && scan2_stmt(env, stmt->else_body) < 0)) ? 1 : -1)

ANN static inline m_bool scan2_stmt_code(const Env env, const Stmt_Code stmt) { GWDEBUG_EXE
  if(stmt->stmt_list)
    { RET_NSPC(scan2_stmt_list(env, stmt->stmt_list)) }
  return GW_OK;
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
  return GW_OK;
}

ANN m_bool scan2_stmt_union(const Env env, const Stmt_Union stmt) { GWDEBUG_EXE
  const m_uint scope = union_push(env, stmt);
  Decl_List l = stmt->l;
  do CHECK_BB(scan2_exp(env, l->self))
  while((l = l->next));
  union_pop(env, stmt, scope);
  return GW_OK;
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
  return GW_OK;
}

ANN static m_bool scan2_func_def_overload(const Func_Def f, const Value overload) { GWDEBUG_EXE
  const m_bool base = tmpl_list_base(f->tmpl);
  const m_bool tmpl = GET_FLAG(overload, template);
  if(isa(overload->type, t_function) < 0)
    ERR_B(f->td->xid->pos, "function name '%s' is already used by another value",
          s_name(f->name))
  if((!tmpl && base) || (tmpl && !base && !GET_FLAG(f, template)))
    ERR_B(f->td->xid->pos, "must overload template function with template")
  return GW_OK;
}

ANN static Func scan_new_func(const Env env, const Func_Def f, const m_str name) {
  const Func func = new_func(name, f);
  if(env->class_def) {
    if(GET_FLAG(env->class_def, template))
      SET_FLAG(func, ref);
    if(!GET_FLAG(f, static))
      SET_FLAG(func, member);
  }
  return func;
}

ANN static Type func_type(const Env env, const Func func) {
  const Type t = type_copy(t_function);
  t->name = func->name;
  t->owner = env->curr;
  if(GET_FLAG(func, member))
    t->size += SZ_INT;
  t->d.func = func;
  return t;
}

ANN2(1,2) static Value func_value(const Env env, const Func f,
    const Value overload) {
  const Type  t = func_type(env, f);
  const Value v = new_value(env->gwion, t, t->name);
  CHECK_OO(scan2_func_assign(env, f->def, f, v))
  if(!overload) {
    ADD_REF(v);
    nspc_add_value(env->curr, f->def->name, v);
  } else /* if(!GET_FLAG(f->def, template)) */ {
    f->next = overload->d.func_ref->next;
    overload->d.func_ref->next = f;
  }
  return v;
}

ANN2(1, 2) static m_bool scan2_func_def_template(const Env env, const Func_Def f, const Value overload) { GWDEBUG_EXE
  const m_str func_name = s_name(f->name);
  const Func func = scan_new_func(env, f, func_name);
  const Value value = func_value(env, func, overload);
  SET_FLAG(value, checked | ae_flag_template);
  SET_FLAG(value->type, func); // the only types with func flag, name could be better
  Type type = env->class_def;
  Nspc nspc = env->curr;
  uint i = 0;
  do {
    const Value v = nspc_lookup_value1(nspc, f->name);
    if(v) {
      Func ff = v->d.func_ref;
      do {
        if(ff->def == f) {
          ++i;
          continue;
        }
        m_bool ret = compat_func(ff->def, f);
        if(ret > 0) {
          const Symbol sym = func_symbol(env->curr->name, func_name,
            "template", ff->vt_index);
          nspc_add_value(env->curr, sym, value);
          if(!overload) {
            ADD_REF(value)
            nspc_add_value(env->curr, f->name, value);
          }
          func->vt_index = ff->vt_index;
          return GW_OK;
        }
      } while((ff = ff->next) && ++i);
   }
  } while(type && (type = type->parent) && (nspc = type->nspc));
  --i;
  const Symbol sym = func_symbol(env->curr->name, func_name, "template", i);
  nspc_add_value(env->curr, sym, value);
  if(!overload) {
    func->vt_index = i;
    ADD_REF(value)
    nspc_add_value(env->curr, f->name, value);
  } else
    func->vt_index = ++overload->offset;
  return GW_OK;
}

ANN static m_bool scan2_func_def_builtin(const Func func, const m_str name) { GWDEBUG_EXE
  SET_FLAG(func, builtin);
  func->code = new_vm_code(NULL, func->def->stack_depth, func->flag, name);
  func->code->native_func = (m_uint)func->def->d.dl_func_ptr;
  return GW_OK;
}

ANN static m_bool scan2_func_def_op(const Env env, const Func_Def f) { GWDEBUG_EXE
  assert(f->arg_list);
  const Operator op = name2op(s_name(f->name));
  const Type l = GET_FLAG(f, unary) ? NULL :
    f->arg_list->var_decl->value->type;
  const Type r = GET_FLAG(f, unary) ? f->arg_list->var_decl->value->type :
    f->arg_list->next ? f->arg_list->next->var_decl->value->type : NULL;
  struct Op_Import opi = { .op=op, .lhs=l, .rhs=r, .ret=f->ret_type };
  CHECK_BB(env_add_op(env, &opi))
  if(env->class_def) {
    if(env->class_def == l)
      REM_REF(l)
    if(env->class_def == r)
      REM_REF(r)
    if(env->class_def == f->ret_type)
      REM_REF(f->ret_type)
  }
  return GW_OK;
}

ANN static m_bool scan2_func_def_code(const Env env, const Func_Def f) { GWDEBUG_EXE
  const Func former = env->func;
  env->func = f->func;
  CHECK_BB(scan2_stmt_code(env, &f->d.code->d.stmt_code))
  env->func = former;
  return GW_OK;
}

ANN static void scan2_func_def_flag(const Env env, const Func_Def f) { GWDEBUG_EXE
  if(!GET_FLAG(f, builtin))
    SET_FLAG(f->func, pure);
  if(GET_FLAG(f, dtor)) {
    SET_FLAG(env->class_def, dtor);
    SET_FLAG(f->func, dtor);
  }
}

ANN static m_str func_tmpl_name(const Env env, const Func_Def f) {
  const m_str func_name = s_name(f->name);
  struct Vector_ v;
  ID_List id = f->tmpl->list;
  m_uint tlen = 0;
  vector_init(&v);
  do {
    const Type t = nspc_lookup_type0(env->curr, id->xid);
    vector_add(&v, (vtype)t);
    tlen += strlen(t->name);
  } while((id = id->next) && ++tlen);
  char tmpl_name[tlen + 1];
  m_str str = tmpl_name;
  for(m_uint i = 0; i < vector_size(&v); ++i) {
    const m_str s = ((Type)vector_at(&v, i))->name;
    strcpy(str, s);
    str += strlen(s);
    if(i + 1 < vector_size(&v))
      *str++ = ',';
  }
  tmpl_name[tlen+1] = '\0';
  vector_release(&v);
  const Symbol sym = func_symbol(env->curr->name, func_name, tmpl_name, (m_uint)f->tmpl->base);
  return s_name(sym);
}


ANN2(1,2,4) static Value func_create(const Env env, const Func_Def f,
     const Value overload, const m_str func_name) {
  const Func func = scan_new_func(env, f, func_name);
  nspc_add_func(env->curr, insert_symbol(func->name), func);
  const Value v = func_value(env, func, overload);
  scan2_func_def_flag(env, f);
  if(GET_FLAG(f, builtin))
    CHECK_BO(scan2_func_def_builtin(func, func->name))
  if(GET_FLAG(func, member))
    f->stack_depth += SZ_INT;
  if(GET_FLAG(func->def, variadic))
    f->stack_depth += SZ_INT;
  nspc_add_value(env->curr, insert_symbol(func->name), v);
  return v;
}

ANN m_bool scan2_func_def(const Env env, const Func_Def f) { GWDEBUG_EXE
  Value value    = NULL;
  f->stack_depth = 0;
  m_uint scope = env->scope->depth;
  if(GET_FLAG(f, global))
    scope = env_push_global(env);
  const Value overload = nspc_lookup_value0(env->curr, f->name);
  m_str func_name = s_name(f->name);
  if(overload)
    CHECK_BB(scan2_func_def_overload(f, overload))
  if(tmpl_list_base(f->tmpl))
    return scan2_func_def_template(env, f, overload);
  if(!f->tmpl) {
    const Symbol sym  = func_symbol(env->curr->name, func_name, NULL, overload ? ++overload->offset : 0);
    func_name = s_name(sym);
  } else {
    if(f->func)
      func_name = f->func->name;
    else
      func_name = func_tmpl_name(env, f);
    const Func func = nspc_lookup_func1(env->curr, insert_symbol(func_name));
    if(func) {
      if(GET_FLAG(func, member))
        f->stack_depth += SZ_INT;
      if(GET_FLAG(func->def, variadic))
        f->stack_depth += SZ_INT;
      f->ret_type = type_decl_resolve(env, f->td);
      return (f->arg_list && f->arg_list->type) ? scan2_args(env, f) : GW_OK;
    }
  }
  const Func base = get_func(env, f);
  if(!base) {
    CHECK_OB((value = func_create(env, f, overload, func_name)))
  } else {
    f->func = base;
}
  if(f->arg_list)
    CHECK_BB(scan2_args(env, f))
  if(!GET_FLAG(f, builtin) && f->d.code->d.stmt_code.stmt_list)
    CHECK_BB(scan2_func_def_code(env, f))
  if(!base) {
    if(GET_FLAG(f, op))
      CHECK_BB(scan2_func_def_op(env, f))
    SET_FLAG(value, checked);
  }
  if(GET_FLAG(f, global))
    env_pop(env, scope);
  return GW_OK;
}

DECL_SECTION_FUNC(scan2)

ANN static m_bool scan2_class_parent(const Env env, const Class_Def class_def) {
  const Type t = class_def->type->parent->array_depth ?
    array_base(class_def->type->parent) : class_def->type->parent;
  if(!GET_FLAG(t, scan2) && GET_FLAG(class_def->ext, typedef))
    CHECK_BB(scan2_class_def(env, t->def))
  if(class_def->ext->array)
    CHECK_BB(scan2_exp(env, class_def->ext->array->exp))
  return GW_OK;
}

ANN static m_bool scan2_class_body(const Env env, const Class_Def class_def) {
  const m_uint scope = env_push_type(env, class_def->type);
  Class_Body body = class_def->body;
  do CHECK_BB(scan2_section(env, body->section))
  while((body = body->next));
  env_pop(env, scope);
  return GW_OK;
}

ANN m_bool scan2_class_def(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  if(tmpl_class_base(class_def->tmpl))
    return GW_OK;
  if(class_def->ext)
    CHECK_BB(scan2_class_parent(env, class_def))
  if(class_def->body)
    CHECK_BB(scan2_class_body(env, class_def))
  SET_FLAG(class_def->type, scan2);
  return GW_OK;
}

ANN m_bool scan2_ast(const Env env, Ast ast) { GWDEBUG_EXE
  do CHECK_BB(scan2_section(env, ast->section))
  while((ast = ast->next));
  return GW_OK;
}
