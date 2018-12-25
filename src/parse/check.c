#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "value.h"
#include "func.h"
#include "instr.h"
#include "object.h"
#include "import.h"
#include "traverse.h"
#include "template.h"
#include "optim.h"
#include "parse.h"
#include "nspc.h"
#include "operator.h"
#include "switch.h"

#define OP_RET(a, b)\
  const Type op_ret = op_check(env, &opi);\
  if(!op_ret)\
    ERR_O(a->self->pos, "in %s expression", b)\
  return op_ret;

ANN static Type   check_exp(const Env env, Exp exp);
ANN static m_bool check_stmt(const Env env, const Stmt stmt);
ANN static m_bool check_stmt_list(const Env env, Stmt_List list);
ANN m_bool check_class_def(const Env env, const Class_Def class_def);

ANN m_bool check_exp_array_subscripts(Env env, Exp exp) { GWDEBUG_EXE
  CHECK_OB(check_exp(env, exp))
  do if(isa(exp->type, t_int) < 0)
      ERR_B(exp->pos, "incompatible array subscript type '%s' ...", exp->type->name)
  while((exp = exp->next));
  return GW_OK;
}

ANN static inline m_bool check_exp_decl_parent(const Env env, const Var_Decl var) { GWDEBUG_EXE
  const Value value = find_value(env->class_def->parent, var->xid);
  if(value)
    ERR_B(var->pos,
          "in class '%s': '%s' has already been defined in parent class '%s' ...",
          env->class_def->name, s_name(var->xid), value->owner_class->name)
  return GW_OK;
}

#define describe_check_decl(a, b)                                 \
ANN static inline void decl_##a(const Nspc nspc, const Value v) { \
  SET_FLAG(v, a);                                                 \
  v->offset = nspc->b;                                            \
  nspc->b += v->type->size;                                       \
}
describe_check_decl(member, offset)
describe_check_decl(static, class_data_size)

ANN static m_bool check_fptr_decl(const Env env, const Var_Decl var) {
  const Value v    = var->value;
  const Func  func = v->type->d.func;
  const Type type = func->value_ref->owner_class;
  if(!env->class_def) {
    if(!type || GET_FLAG(func, global))
      return GW_OK;
    ERR_B(var->pos, "can't use non public typedef at global scope.")
  }
  if(isa(type, env->class_def) < 0 && !GET_FLAG(func, global))
    ERR_B(var->pos, "can't use non global fptr of other class.")
  if(GET_FLAG(func, member)) {
    if(GET_FLAG(v, static))
      ERR_B(var->pos, "can't use static variables for member function.")
    if(!GET_FLAG(v, member))
      ERR_B(var->pos, "can't use member variables for static function.")
  } //else if(GET_FLAG(v, member))
    //ERR_B(var->pos, "can't use member variables for static function.")
  return GW_OK;
}

ANN Type check_exp_decl(const Env env, const Exp_Decl* decl) { GWDEBUG_EXE
  Var_Decl_List list = decl->list;
  if(GET_FLAG(decl->type , template)) {
    const Type t = typedef_base(decl->type);
    CHECK_BO(traverse_template(env, t->def))
  }
  m_uint scope;
  const m_bool global = GET_FLAG(decl->td, global);
  if(global)
    scope = env_push_global(env);
  do {
    if(!env->class_def && !GET_FLAG(list->self->value, builtin) &&
        !GET_FLAG(list->self->value, used)) {
      err_msg(list->self->pos, "unused variable '%s'",
          list->self->value->name);
      continue;
    }
    const Var_Decl var = list->self;
    const Value v = var->value;
    if(env->class_def && !env->scope && env->class_def->parent)
      CHECK_BO(check_exp_decl_parent(env, var))
    if(var->array && var->array->exp)
      CHECK_BO(check_exp_array_subscripts(env, var->array->exp))
    if(GET_FLAG(decl->td, member))
      decl_member(env->curr, v);
    else if(GET_FLAG(decl->td, static))
      decl_static(env->curr, v);
    if(isa(decl->type, t_fptr) > 0)
      CHECK_BO(check_fptr_decl(env, var))
  SET_FLAG(v, checked | ae_flag_used);
  nspc_add_value(env->curr, var->xid, v);
  } while((list = list->next));
  if(global)
    env_pop(env, scope);
  return decl->type;
}

ANN static m_bool prim_array_inner(const Type t, Type type, const Exp e) {
  const Type common = find_common_anc(t, type);
  if(common)
    return GW_OK;
  else if(isa(t, t_int) > 0 && isa(type, t_float) > 0) {
      e->cast_to = type;
      return GW_OK;
  }
  return err_msg(e->pos, "array init [...] contains incompatible types ...");
}

ANN static inline Type prim_array_match(Exp e) {
  const Type type = e->type;
  do CHECK_BO(prim_array_inner(e->type, type, e))
  while((e = e->next));
  return array_type(type->array_depth ? array_base(type) : type, type->array_depth + 1);
}

ANN static Type prim_array(const Env env, const Exp_Primary* primary) {
  const Array_Sub array = primary->d.array;
  const Exp e = array->exp;
  if(!e)
    ERR_O(primary->self->pos, "must provide values/expressions for array [...]")
  CHECK_OO(check_exp(env, e))
  return (array->type = prim_array_match(e));
}

ANN static Value check_non_res_value(const Env env, const Exp_Primary* primary) {
  const Value value = nspc_lookup_value1(env->curr, primary->d.var);
  if(env->class_def) {
    const Value v = value ? value : find_value(env->class_def, primary->d.var);
    if(v && SAFE_FLAG(env->func, static) && GET_FLAG(v, member))
      ERR_O(primary->self->pos,
            "non-static member '%s' used from static function.", s_name(primary->d.var))
    return v;
  }
  return value;
}

ANN static Type prim_id_non_res(const Env env, const Exp_Primary* primary) {
  const Value v = check_non_res_value(env, primary);
  if(!v || !GET_FLAG(v, checked)) {
    err_msg(primary->self->pos,
          "variable %s not legit at this point.", s_name(primary->d.var));
    did_you_mean(s_name(primary->d.var));
    return NULL;
  }
  if(env->func && !GET_FLAG(v, const) && v->owner)
    UNSET_FLAG(env->func, pure);
  SET_FLAG(v, used);
  ((Exp_Primary*)primary)->value = v;
  if(GET_FLAG(v, const) || !strcmp(s_name(primary->d.var), "maybe"))
    primary->self->meta = ae_meta_value;
//  OPTIMIZE_PRIM_CONST(primary, v->d.ptr)
  return v->type;
}

ANN static Type check_exp_prim_this(const Env env, const Exp_Primary* primary) {
  if(!env->class_def)
    ERR_O(primary->self->pos, "keyword 'this' can be used only inside class definition...")
  if(env->func && !GET_FLAG(env->func, member))
    ERR_O(primary->self->pos, "keyword 'this' cannot be used inside static functions...")
  primary->self->meta = ae_meta_value;
  return env->class_def;
}

ANN static Type prim_id(const Env env, const Exp_Primary* primary) {
  const m_str str = s_name(primary->d.var);
  if(!strcmp(str, "this"))
    return check_exp_prim_this(env, primary);
  else
    return prim_id_non_res(env, primary);
}

ANN static m_bool vec_value(const Env env, Exp e, const m_str s) {
  int count = 1;
  CHECK_OB(check_exp(env, e))
  do {
    const Type t = e->type;
    if(isa(t, t_float) < 0) {
      if(isa(t, t_int) > 0)
        e->cast_to = t_float;
      else
        ERR_B(e->pos, "invalid type '%s' in %s value #%d...\n"
              "    (must be of type 'int' or 'float')", t->name, s, count)
    }
    ++count;
  } while((e = e->next));
  return GW_OK;
}

struct VecInfo {
  Type   t;
  m_str  s;
  m_uint n;
};

ANN static void vec_info(const ae_prim_t t, struct VecInfo* v) {
  if(t == ae_primary_complex) {
    v->s = "complex";
    v->t = t_complex;
    v->n = 2;
  } else if(t == ae_primary_vec) {
    v->t = v->n == 4 ? t_vec4 : t_vec3;
    v->n = 4;
    v->s = "vector";
  } else {
    v->s = "polar";
    v->t = t_polar;
    v->n = 2;
  }
}

ANN static Type prim_vec(const Env env, const Exp_Primary* primary) {
  const Vec * vec = &primary->d.vec;
  const ae_prim_t t = primary->primary_type;
  struct VecInfo info = { .n=vec->dim };
  vec_info(t, &info);
  if(vec->dim > info.n)
    ERR_O(vec->exp->pos, "extraneous component of %s value...", info.s)
  CHECK_BO(vec_value(env, vec->exp, info.s))
  return info.t;
}

ANN static Type prim_gack(const Env env, const Exp_Primary * primary) {
  if(env->func)
    UNSET_FLAG(env->func, pure);
  CHECK_OO((check_exp(env, primary->d.exp)))
  return t_gack;
}

#define describe_prim_xxx(name, type) \
ANN static Type prim_##name(const Env env __attribute__((unused)), const Exp_Primary * primary __attribute__((unused))) {\
  return type; \
}
describe_prim_xxx(num, t_int)
describe_prim_xxx(float, t_float)
describe_prim_xxx(str, t_string)
describe_prim_xxx(nil, t_void)

typedef Type (*_type_func)(const Env, const void*);
static const _type_func prim_func[] = {
  (_type_func)prim_id,    (_type_func)prim_num,  (_type_func)prim_float, (_type_func)prim_str,
  (_type_func)prim_array, (_type_func)prim_gack, (_type_func)prim_vec,   (_type_func)prim_vec,
  (_type_func)prim_vec,   (_type_func)prim_num,  (_type_func)prim_nil,
};

ANN static Type check_exp_primary(const Env env, const Exp_Primary* primary) { GWDEBUG_EXE
  return primary->self->type = prim_func[primary->primary_type](env, primary);
}

ANN Type check_exp_array(const Env env, const Exp_Array* array) { GWDEBUG_EXE
  Type t_base = check_exp(env, array->base);
  CHECK_OO(t_base)
  Exp e = array->array->exp;
  CHECK_OO(check_exp(env, e))
  m_uint depth = 1;
  do {
    if(isa(e->type, t_int) < 0)
      ERR_O(e->pos, "array index %i must be of type 'int', not '%s'",
            depth, e->type->name)
  } while((e = e->next) && ++depth);
  if(depth != array->array->depth)
    ERR_O(array->self->pos, "invalid array acces expression.")

  while(t_base && array->array->depth > t_base->array_depth) {
     depth -= t_base->array_depth;
     if(t_base->parent)
       t_base = t_base->parent;
     else
       ERR_O(array->self->pos,
             "array subscripts (%i) exceeds defined dimension (%i)",
             array->array->depth, t_base->array_depth)
  }
  return depth == t_base->array_depth ? array_base(t_base) :
    array_type(array_base(t_base), t_base->array_depth - depth);
}

ANN static Type_List mk_type_list(const Env env, const Type type) {
  struct Vector_ v;
  vector_init(&v);
  vector_add(&v, (vtype)insert_symbol(type->name));
  Nspc nspc = type->owner;
  while(nspc && nspc != env->curr && nspc != env->global_nspc) {
    const Type t = nspc_lookup_type0(nspc->parent, insert_symbol(nspc->name));
    vector_add(&v, (vtype)insert_symbol(t->name));
    nspc = nspc->parent;
  }
  ID_List id = NULL;
  for(m_uint i = vector_size(&v) + 1; --i;)
    id = prepend_id_list((Symbol)vector_at(&v, i - 1), id, 0);
  vector_release(&v);
  assert(id);
  Type_Decl* td = new_type_decl(id, 0);
  return new_type_list(td, NULL);
}

ANN static m_bool func_match_inner(const Env env, const Exp e, const Type t,
  const m_bool implicit, const m_bool specific) {
  const m_bool match = (specific ? e->type == t : isa(e->type, t) > 0) &&
    e->type->array_depth == t->array_depth &&
    array_base(e->type) == array_base(t);
  if(!match && implicit) {
    const struct Implicit imp = { e, t };
    struct Op_Import opi = { .op=op_impl, .lhs=e->type, .rhs=t, .data=(m_uint)&imp };
    return op_check(env, &opi) ? 1 : -1;
  }
  return match ? 1 : -1;
}

ANN2(1,2) static Func find_func_match_actual(const Env env, Func func, const Exp args,
  const m_bool implicit, const m_bool specific) {
  do {
    Exp e = args;
    Arg_List e1 = func->def->arg_list;
    while(e) {
      if(!e1) {
        if(GET_FLAG(func->def, variadic))
          return func;
        CHECK_OO(func->next);
        return find_func_match_actual(env, func->next, args, implicit, specific);
      }
      if(func_match_inner(env, e, e1->type, implicit, specific) < 0)
        break;
      e = e->next;
      e1 = e1->next;
    }
    if(!e1)
      return func;
  } while((func = func->next));
  return NULL;
}

ANN2(1, 2) static Func find_func_match(const Env env, const Func up, const Exp exp) {
  Func func;
  const Exp args = (exp && isa(exp->type, t_void) < 0) ? exp : NULL;
  if((func = find_func_match_actual(env, up, args, 0, 1)) ||
     (func = find_func_match_actual(env, up, args, 1, 1)) ||
     (func = find_func_match_actual(env, up, args, 0, 0)) ||
     (func = find_func_match_actual(env, up, args, 1, 0)))
    return func;
  return NULL;
}

ANN static m_bool check_call(const Env env, const Exp_Call* exp) {
  CHECK_OB(check_exp(env, exp->func))
  return exp->args ? !!check_exp(env, exp->args) : -1;
}

ANN static inline Value template_get_ready(const Value v, const m_str tmpl, const m_uint i) {
  const Symbol sym = func_symbol(v->owner->name, v->name, tmpl, i);
  return v->owner_class ? find_value(v->owner_class, sym) :
      nspc_lookup_value1(v->owner, sym);
}

ANN Func find_template_match(const Env env, const Value v, const Exp_Call* exp) {
  const Exp args = exp->args;
  const Type_List types = exp->tmpl->types;
  Func m_func = exp->m_func;
  const m_str tmpl_name = tl2str(env, types);
  const m_uint scope = env_push(env, v->owner_class, v->owner);
  for(m_uint i = 0; i < v->offset + 1; ++i) {
    Func_Def def = NULL;
    Func_Def base = NULL;
    Value value = template_get_ready(v, tmpl_name, i);
    if(value) {
      if(env->func == value->d.func_ref) {
        free(tmpl_name);
        CHECK_BO(check_call(env, exp))
        return env->func;
      }
      base = def = value->d.func_ref->def;
    } else {
      if(!(value = template_get_ready(v, "template", i)))
        continue;
      base = value->d.func_ref->def;
      def = new_func_def(base->td, insert_symbol(v->name),
                base->arg_list, base->d.code, base->flag);
      def->tmpl = new_tmpl_list(base->tmpl->list, (m_int)i);
      SET_FLAG(def, template);
    }
    if(traverse_func_template(env, def, types) > 0) {
      nspc_pop_type(env->curr);
      if(check_call(env, exp) > 0) {
        const Func next = def->func->next;
        def->func->next = NULL;
        m_func = find_func_match(env, def->func, args);
        def->func->next = next;
        if(m_func) {
          SET_FLAG(m_func, checked | ae_flag_template);
          goto end;
        }
      }
    }
    SET_FLAG(base, template);
    free_func_def(def);
  }
  assert(exp->self);
  err_msg(exp->self->pos, "arguments do not match for template call");
end:
  free(tmpl_name);
  env_pop(env, scope);
  return m_func;
}

ANN static void print_current_args(Exp e) {
  gw_err("and not\n\t");
  do {
    gw_err(" \033[32m%s\033[0m", e->type->name);
    if(e->type->array_depth)
      REM_REF(e->type)
  } while((e = e->next) && gw_err(","));
  gw_err("\n");
}

ANN static void print_arg(Arg_List e) {
  do gw_err(" \033[32m%s\033[0m \033[1m%s\033[0m", e->type->name,
     s_name(e->var_decl->xid));
  while((e = e->next) && gw_err(","));
}

ANN2(1) static void* function_alternative(const Type f, Exp args){
  err_msg(args ? args->pos : 0, "argument type(s) do not match for function. should be :");
  Func up = f->d.func;
  do {
    gw_err("(%s)\t", up->name);
    const Arg_List e = up->def->arg_list;
    gw_err("\t");
    if(e)
      print_arg(e);
    else
      gw_err("\033[32mvoid\033[0m");
    gw_err("\n");
  } while((up = up->next));
  if(args)
    print_current_args(args);
  else
    gw_err("and not:\n\t\033[32mvoid\033[0m\n");
  return NULL;
}

ANN static m_uint get_type_number(ID_List list) {
  m_uint type_number = 0;
  do ++type_number;
  while((list = list->next));
  return type_number;
}

ANN static Func get_template_func(const Env env, const Exp_Call* func, const Exp base, const Value v) {
  const Func f = find_template_match(env, v, func);
  if(f) {
    Tmpl_Call* tmpl = new_tmpl_call(func->tmpl->types);
    tmpl->base = v->d.func_ref->def->tmpl->list;
    if(base->exp_type == ae_exp_call)
      base->d.exp_call.tmpl = tmpl;
    else // if(base->exp_type == ae_exp_binary)
      base->d.exp_binary.tmpl = tmpl;
    return f;
  }
  assert(func->self);
  ERR_O(func->self->pos,
        "function is template. automatic type guess not fully implemented yet.\n"
        "\tplease provide template types. eg: '<type1, type2, ...>'")
}

ANN static Type check_exp_call_template(const Env env, const Exp_Call *exp) {
  const Exp call = exp->func;
  const Exp args = exp->args;
  const Exp base = exp->self;
  m_uint args_number = 0;
  const Value value = nspc_lookup_value1(call->type->owner, insert_symbol(call->type->name));
  CHECK_OO(value)
  const m_uint type_number = get_type_number(value->d.func_ref->def->tmpl->list);
  Type_List tl[type_number];
  ID_List list = value->d.func_ref->def->tmpl->list;
  while(list) {
    Arg_List arg = value->d.func_ref->def->arg_list;
    Exp template_arg = args;
    while(arg && template_arg) {
      char path[id_list_len(arg->td->xid)];
      type_path(path, arg->td->xid);
      if(!strcmp(s_name(list->xid), path)) {
        tl[args_number] = mk_type_list(env, template_arg->type);
        if(args_number)
          tl[args_number - 1]->next = tl[args_number];
        ++args_number;
        break;
      }
      arg = arg->next;
      template_arg = template_arg->next;
    }
    list = list->next;
  }
  if(args_number < type_number)
    ERR_O(call->pos, "not able to guess types for template call.")
  Tmpl_Call tmpl = { .types=tl[0] };
  const Exp_Call tmp_func = { .func=call, .args=args, .tmpl=&tmpl, .self=base };
  const Func func = get_template_func(env, &tmp_func, base, value);
  if(base->exp_type == ae_exp_call)
    base->d.exp_call.m_func = func;
  else // if(base->exp_type == ae_exp_binary)
    base->d.exp_binary.func = func;
  return func ? func->def->ret_type : NULL;
}

ANN static m_bool check_exp_call1_check(const Env env, const Exp exp) {
  if(!check_exp(env, exp))
    ERR_B(exp->pos, "function call using a non-existing function")
  if(isa(exp->type, t_function) < 0)
    ERR_B(exp->pos, "function call using a non-function value")
  return GW_OK;
}

ANN Type check_exp_call1(const Env env, const Exp_Call *exp) {
  CHECK_BO(check_exp_call1_check(env, exp->func))
  if(GET_FLAG(exp->func->type->d.func, ref)) {
    const Value value = exp->func->type->d.func->value_ref;
    CHECK_BO(traverse_template(env, value->owner_class->def))
  }
  if(exp->args)
    CHECK_OO(check_exp(env, exp->args))
  if(GET_FLAG(exp->func->type, func))
    return check_exp_call_template(env, exp);
  const Func func = find_func_match(env, exp->func->type->d.func, exp->args);
  if(!func)
    return function_alternative(exp->func->type, exp->args);
  if(exp->self->exp_type == ae_exp_call)
    exp->self->d.exp_call.m_func = func;
  else // if(exp->self->exp_type == ae_exp_binary)
    exp->self->d.exp_binary.func = func;
  return func->def->ret_type;
}

ANN static Type check_exp_binary(const Env env, const Exp_Binary* bin) { GWDEBUG_EXE
  struct Op_Import opi = { .op=bin->op, .lhs=check_exp(env, bin->lhs),
    .rhs=check_exp(env, bin->rhs), .data=(uintptr_t)bin };
  CHECK_OO(opi.lhs)
  CHECK_OO(opi.rhs)
  const Type op_ret = op_check(env, &opi);
  if(!op_ret)
    ERR_O(bin->self->pos, "in binary expression")
  OPTIMIZE_CONST(bin)
  return op_ret;
}

ANN static Type check_exp_cast(const Env env, const Exp_Cast* cast) { GWDEBUG_EXE
  const Type t = check_exp(env, cast->exp);
  CHECK_OO(t)
  CHECK_OO((cast->self->type = known_type(env, cast->td)))
  struct Op_Import opi = { .op=op_cast, .lhs=t, .rhs=cast->self->type, .data=(uintptr_t)cast };
  OP_RET(cast, "cast")
}

ANN static Type check_exp_post(const Env env, const Exp_Postfix* post) { GWDEBUG_EXE
  struct Op_Import opi = { .op=post->op, .lhs=check_exp(env, post->exp), .data=(uintptr_t)post };
  CHECK_OO(opi.lhs)
  OP_RET(post, "postfix");
}

ANN static Type check_exp_dur(const Env env, const Exp_Dur* exp) { GWDEBUG_EXE
  CHECK_OO(check_exp(env, exp->base))
  CHECK_OO(check_exp(env, exp->unit))
  if(isa(exp->base->type, t_float) < 0) {
    if(isa(exp->base->type, t_int) > 0)
      exp->base->cast_to = t_float;
    else
      ERR_O(exp->base->pos, "invalid type '%s' in prefix of dur expression...\n"
          "    (must be of type 'int' or 'float')", exp->base->type->name)
  }
  if(isa(exp->unit->type, t_dur) < 0)
    ERR_O(exp->unit->pos, "invalid type '%s' in postfix of dur expression...\n"
          "    (must be of type 'dur')", exp->base->type->name)
  return exp->unit->type;
}

ANN static Type check_exp_call(const Env env, Exp_Call* exp) { GWDEBUG_EXE
  if(exp->tmpl) {
    CHECK_OO(check_exp(env, exp->func)) // → puts this up ?
    const Type t = actual_type(exp->func->type);
    const Value v = nspc_lookup_value1(t->owner, insert_symbol(t->name));
    if(!v)
      ERR_O(exp->self->pos, " template call of non-existant function.")
    if(!GET_FLAG(v, func))
      ERR_O(exp->self->pos, "template call of non-function value.")
    if(!v->d.func_ref->def->tmpl)
      ERR_O(exp->self->pos, "template call of non-template function.")
    const Func ret = find_template_match(env, v, exp);
    CHECK_OO((exp->m_func = ret))
    return ret->def->ret_type;
  }
  return check_exp_call1(env, exp);
}

ANN static inline m_bool check_exp_unary_spork1(const Env env, const Stmt code) {
  RET_NSPC(check_stmt(env, code))
}

ANN Type check_exp_unary_spork(const Env env, const Stmt code) { GWDEBUG_EXE
  CHECK_BO(check_exp_unary_spork1(env, code))
  return t_shred;
}

ANN static Type check_exp_unary(const Env env, const Exp_Unary* unary) { GWDEBUG_EXE
  struct Op_Import opi = { .op=unary->op, .rhs=unary->exp ? check_exp(env, unary->exp) : NULL,
    .data=(uintptr_t)unary };
  if(unary->exp && !opi.rhs)
    return NULL;
  OP_RET(unary, "unary")
}

ANN static Type check_exp_if(const Env env, const Exp_If* exp_if) { GWDEBUG_EXE
  const Type cond     = check_exp(env, exp_if->cond);
  CHECK_OO(cond)
  const Type if_exp   = check_exp(env, exp_if->if_exp);
  CHECK_OO(if_exp)
  const Type else_exp = check_exp(env, exp_if->else_exp);
  CHECK_OO(else_exp)
  if(isa(cond, t_int) < 0 && isa(cond, t_float) < 0 && isa(cond, t_object) < 0)
    ERR_O(exp_if->self->pos,
          "Invalid type '%s' in if expression condition.", cond->name)
  const Type ret = find_common_anc(if_exp, else_exp);
  if(!ret)
    ERR_O(exp_if->self->pos,
          "incompatible types '%s' and '%s' in if expression...",
          if_exp->name, else_exp->name)
  return ret;
}

ANN static Type check_exp_dot(const Env env, Exp_Dot* member) { GWDEBUG_EXE
  const m_str str = s_name(member->xid);
  CHECK_OO((member->t_base = check_exp(env, member->base)))
  const m_bool base_static = isa(member->t_base, t_class) > 0;
  const Type the_base = base_static ? member->t_base->d.base_type : member->t_base;
  if(!the_base->nspc)
    ERR_O(member->base->pos,
          "type '%s' does not have members - invalid use in dot expression of %s",
          the_base->name, str)
  if(!strcmp(str, "this") && base_static)
    ERR_O(member->self->pos,
          "keyword 'this' must be associated with object instance...")
  const Value value = find_value(the_base, member->xid);
  if(!value)
    ERR_O(member->base->pos,
          "class '%s' has no member '%s'", the_base->name, str)
  if(!env->class_def || isa(env->class_def, value->owner_class) < 0) {
    if(GET_FLAG(value, private))
      ERR_O(member->self->pos,
          "can't access private '%s' outside of class...", value->name)
    else if(GET_FLAG(value, protect))
      member->self->meta = ae_meta_protect;
  }
  if(base_static && GET_FLAG(value, member))
    ERR_O(member->self->pos,
          "cannot access member '%s.%s' without object instance...",
          the_base->name, str)
  if(GET_FLAG(value, const) || GET_FLAG(value, enum))
    member->self->meta = ae_meta_value;
  return value->type;
}

ANN static m_bool check_stmt_type(const Env env, const Stmt_Type stmt) { GWDEBUG_EXE
  return stmt->type->def ? check_class_def(env, stmt->type->def) : 1;
}

static const _type_func exp_func[] = {
  (_type_func)check_exp_decl,    (_type_func)check_exp_binary, (_type_func)check_exp_unary,
  (_type_func)check_exp_primary, (_type_func)check_exp_cast,   (_type_func)check_exp_post,
  (_type_func)check_exp_call,    (_type_func)check_exp_array,  (_type_func)check_exp_if,
  (_type_func)check_exp_dot,     (_type_func)check_exp_dur
};

ANN static inline Type check_exp(const Env env, const Exp exp) { GWDEBUG_EXE
  Exp curr = exp;
  do {
    CHECK_OO((curr->type = exp_func[curr->exp_type](env, &curr->d)))
    if(env->func && isa(curr->type, t_function) > 0 && !GET_FLAG(curr->type->d.func, pure))
      UNSET_FLAG(env->func, pure);
  } while((curr = curr->next));
  return exp->type;
}

ANN m_bool check_stmt_enum(const Env env, const Stmt_Enum stmt) { GWDEBUG_EXE
  if(env->class_def) {
    ID_List list = stmt->list;
    do {
      const Value v = nspc_lookup_value0(env->curr, list->xid);
      decl_static(env->curr, v);
    } while((list = list->next));
  }
  return GW_OK;
}

ANN static m_bool check_stmt_code(const Env env, const Stmt_Code stmt) { GWDEBUG_EXE
  if(stmt->stmt_list) { RET_NSPC(check_stmt_list(env, stmt->stmt_list)) }
  return GW_OK;
}

ANN static m_bool check_flow(const Exp exp, const m_str orig) { GWDEBUG_EXE
  if(isa(exp->type, t_object) > 0 || isa(exp->type, t_int) > 0 || isa(exp->type, t_float) > 0 ||
     isa(exp->type, t_dur) > 0 || isa(exp->type, t_time)  > 0)
    return GW_OK;
  ERR_B(exp->pos, "invalid type '%s' (in '%s' condition)", exp->type->name, orig)
}

ANN static m_bool check_breaks(const Env env, const Stmt a, const Stmt b) { GWDEBUG_EXE
  vector_add(&env->breaks, (vtype)a);
  RET_NSPC(check_stmt(env, b))
  vector_pop(&env->breaks);
  return ret;
}

ANN static m_bool check_conts(const Env env, const Stmt a, const Stmt b) { GWDEBUG_EXE
  vector_add(&env->conts, (vtype)a);
  CHECK_BB(check_breaks(env, a, b))
  vector_pop(&env->conts);
  return GW_OK;
}

ANN static inline m_bool for_empty(const Stmt_For stmt) {
  if(!stmt->c2 || !stmt->c2->d.stmt_exp.val)
    ERR_B(stmt->self->pos, "empty for loop condition...",
          "...(note: explicitly use 'true' if it's the intent)",
          "...(e.g., 'for(; true;){ /*...*/ }')")
  return GW_OK;
}

ANN static m_bool do_stmt_auto(const Env env, const Stmt_Auto stmt) { GWDEBUG_EXE
  Type t = check_exp(env, stmt->exp);
  CHECK_OB(t)
  Type ptr = array_base(t);
  const m_uint depth = t->array_depth - 1;
  if(GET_FLAG(t, typedef))
    t = t->parent;
  if(!t || !ptr || isa(t, t_array) < 0)
    ERR_B(stmt->self->pos, "type '%s' is not array.\n"
          " This is not allowed in auto loop", stmt->exp->type->name)
  if(stmt->is_ptr) {
    struct ID_List_   id0, id;
    struct Type_List_ tl;
    struct Array_Sub_ array;
    Type_Decl td0, td;
    memset(&id0, 0, sizeof(struct ID_List_));
    memset(&id, 0, sizeof(struct ID_List_));
    memset(&tl, 0, sizeof(struct Type_List_));
    memset(&array, 0, sizeof(struct Array_Sub_));
    memset(&td0, 0, sizeof(Type_Decl));
    memset(&td, 0, sizeof(Type_Decl));
    id.xid   = insert_symbol("Ptr");
    id0.xid  = insert_symbol(ptr->name);
    td0.xid  = &id0;
    td.xid   = &id;
    tl.td    = &td0;
    td.types = &tl;
    if(depth) {
      array.depth = depth;
      td.array = &array;
    }
    ptr = type_decl_resolve(env, &td);
    if(!GET_FLAG(ptr, checked))
      check_class_def(env, ptr->def);
  }
  t = depth ? array_type(ptr, depth) : ptr;
  stmt->v = new_value(env->gwion, t, s_name(stmt->sym));
  SET_FLAG(stmt->v, checked);
  nspc_add_value(env->curr, stmt->sym, stmt->v);
  return check_conts(env, stmt->self, stmt->body);
}

ANN static m_bool cond_type(const Exp e) {
  const Type t = e->type;
  if(isa(t, t_int) > 0)
    return GW_OK;
  if(isa(t, t_float) > 0) {
    e->cast_to = t_int;
    return GW_OK;
  }
  ERR_B(e->pos, "conditional must be of type 'int'...")
}
#define stmt_func_xxx(name, type, prolog, exp) describe_stmt_func(check, name, type, prolog, exp)
stmt_func_xxx(if, Stmt_If,, !(!check_exp(env, stmt->cond) ||
  check_flow(stmt->cond, "if") < 0   ||
  check_stmt(env, stmt->if_body) < 0 ||
  (stmt->else_body && check_stmt(env, stmt->else_body) < 0)) ? 1 : -1)
stmt_func_xxx(flow, Stmt_Flow,,
  !(!check_exp(env, stmt->cond) ||
    check_flow(stmt->cond, stmt->self->stmt_type == ae_stmt_while ? "while" : "until") < 0 ||
    check_conts(env, stmt->self, stmt->body) < 0) ? 1 : -1)
stmt_func_xxx(for, Stmt_For,, !(
  for_empty(stmt) < 0 ||
  check_stmt(env, stmt->c1) < 0 ||
  check_stmt(env, stmt->c2) < 0 ||
  check_flow(stmt->c2->d.stmt_exp.val, "for") < 0 ||
  (stmt->c3 && !check_exp(env, stmt->c3)) ||
  check_conts(env, stmt->self, stmt->body) < 0) ? 1 : -1)
stmt_func_xxx(loop, Stmt_Loop,, !(!check_exp(env, stmt->cond) ||
  cond_type(stmt->cond) < 0 ||
  check_conts(env, stmt->self, stmt->body) < 0) ? 1 : -1)
stmt_func_xxx(switch, Stmt_Switch,, !(!check_exp(env, stmt->val) ||
  cond_type(stmt->val) < 0 || !switch_add(env, stmt) ||
  check_breaks(env, stmt->self, stmt->stmt) < 0 || !switch_pop(env)) ? 1 : -1)
stmt_func_xxx(auto, Stmt_Auto,, do_stmt_auto(env, stmt))

ANN static m_bool check_stmt_return(const Env env, const Stmt_Exp stmt) { GWDEBUG_EXE
  if(!env->func)
    ERR_B(stmt->self->pos, "'return' statement found outside function definition")
  const Type ret_type = stmt->val ? check_exp(env, stmt->val) : t_void;
  CHECK_OB(ret_type)
  if(isa(ret_type, t_null) > 0 &&
     isa(env->func->def->ret_type, t_object) > 0)
    return GW_OK;
  if(isa(ret_type, env->func->def->ret_type) < 0)
    ERR_B(stmt->self->pos, "invalid return type '%s' -- expecting '%s'",
          ret_type->name, env->func->def->ret_type->name)
  return GW_OK;
}

#define describe_check_stmt_stack(stack, name)                                     \
ANN static m_bool check_stmt_##name(const Env env, const Stmt stmt) { GWDEBUG_EXE \
  if(!vector_size(&env->stack))                                                    \
    ERR_B(stmt->pos, "'"#name"' found outside of for/while/until...")             \
  return GW_OK;                                                                        \
}
describe_check_stmt_stack(conts,  continue)
describe_check_stmt_stack(breaks, break)

ANN Value case_value(const Exp exp);
ANN static m_bool check_stmt_case(const Env env, const Stmt_Exp stmt) { GWDEBUG_EXE
  const Type t = check_exp(env, stmt->val);
  CHECK_OB(t);
  if(isa(t, t_int) < 0)
    ERR_B(stmt->self->pos, "invalid type '%s' case expression. should be 'int'", t->name)
  const Value v = case_value(stmt->val);
  if(!v)
    return GW_OK;
  if(!GET_FLAG(v, const))
    ERR_B(stmt->val->pos, "'%s' is not constant.", v->name)
  if(!GET_FLAG(v, builtin) && !GET_FLAG(v, enum))
    switch_expset(env, stmt->val);
  return GW_OK;
}

ANN static m_bool check_stmt_jump(const Env env, const Stmt_Jump stmt) { GWDEBUG_EXE
  if(stmt->is_label)
    return GW_OK;
  const Map label = env_label(env);
  const m_uint* key = env->class_def && !env->func ?
                (m_uint*)env->class_def : (m_uint*)env->func;
  const Map m = label->ptr ? (Map)map_get(label, (vtype)key) : NULL;
  if(!m)
    ERR_B(stmt->self->pos, "label '%s' used but not defined", s_name(stmt->name))
  const Stmt_Jump ref = (Stmt_Jump)map_get(m, (vtype)stmt->name);
  if(!ref) {
    for(m_uint i = 0; i < map_size(m); ++i) {
      const Stmt_Jump s = (Stmt_Jump)map_at(m, i);
      vector_release(&s->data.v);
    }
    ERR_B(stmt->self->pos, "label '%s' used but not defined", s_name(stmt->name))
  }
  vector_add(&ref->data.v, (vtype)stmt);
  return GW_OK;
}

ANN m_bool check_stmt_union(const Env env, const Stmt_Union stmt) { GWDEBUG_EXE
  if(stmt->xid) {
    if(env->class_def) {
      if(!GET_FLAG(stmt, static))
        decl_member(env->curr, stmt->value);
      else
        decl_static(env->curr, stmt->value);
    }
  } else if(env->class_def)  {
    if(!GET_FLAG(stmt, static))
      stmt->o = env->class_def->nspc->offset;
    else
      decl_static(env->curr, stmt->value);
  }
  const m_uint scope = union_push(env, stmt);
  Decl_List l = stmt->l;
  do {
    CHECK_OB(check_exp(env, l->self))
    if(isa(l->self->type, t_object) > 0 && !GET_FLAG(l->self->d.exp_decl.td, ref))
      ERR_B(l->self->pos, "In union, Objects must be declared as reference (use '@')")
    if(l->self->type->size > stmt->s)
      stmt->s = l->self->type->size;
  } while((l = l->next));
  union_pop(env, stmt, scope);
  return GW_OK;
}

ANN static m_bool check_stmt_exp(const Env env, const Stmt_Exp stmt) {
 return stmt->val ? check_exp(env, stmt->val) ? 1 : -1 : 1;
}

static const _exp_func stmt_func[] = {
  (_exp_func)check_stmt_exp,   (_exp_func)check_stmt_flow,     (_exp_func)check_stmt_flow,
  (_exp_func)check_stmt_for,   (_exp_func)check_stmt_auto,     (_exp_func)check_stmt_loop,
  (_exp_func)check_stmt_if,    (_exp_func)check_stmt_code,     (_exp_func)check_stmt_switch,
  (_exp_func)check_stmt_break, (_exp_func)check_stmt_continue, (_exp_func)check_stmt_return,
  (_exp_func)check_stmt_case,  (_exp_func)check_stmt_jump,     (_exp_func)check_stmt_enum,
  (_exp_func)dummy_func,       (_exp_func)check_stmt_type,     (_exp_func)check_stmt_union,
};

ANN static m_bool check_stmt(const Env env, const Stmt stmt) { GWDEBUG_EXE
  return stmt_func[stmt->stmt_type](env, &stmt->d);
}

ANN static m_bool check_stmt_list(const Env env, Stmt_List l) { GWDEBUG_EXE
  do CHECK_BB(check_stmt(env, l->stmt))
  while((l = l->next));
  return GW_OK;
}

ANN static m_bool check_signature_match(const Func_Def f, const Func parent) { GWDEBUG_EXE
  if(GET_FLAG(parent->def, static) || GET_FLAG(f, static)) {
    const m_str c_name  = f->func->value_ref->owner_class->name;
    const m_str p_name = parent->value_ref->owner_class->name;
    const m_str f_name = s_name(f->name);
    ERR_B(f->td->xid->pos,
          "function '%s.%s' ressembles '%s.%s' but cannot override...\n"
          "\t...(reason: '%s.%s' is declared as 'static')",
          c_name, f_name, p_name, c_name,
          GET_FLAG(f, static) ? c_name : p_name, f_name)
  }
  return isa(f->ret_type, parent->def->ret_type);
}

ANN static m_bool parent_match_actual(const Env env, const restrict Func_Def f,
    const restrict Func func) {
  Func parent_func = func;
  do {
    if(compat_func(f, parent_func->def) > 0) {
      CHECK_BB(check_signature_match(f, parent_func))
      f->func->vt_index = parent_func->vt_index;
      vector_set(&env->curr->vtable, f->func->vt_index, (vtype)func);
      return GW_OK;
    }
  } while((parent_func = parent_func->next));
  return 0;
}

ANN static m_bool check_parent_match(const Env env, const Func_Def f) { GWDEBUG_EXE
  const Func func = f->func;
  const Type parent = env->class_def->parent;
  if(parent) {
    const Value v = find_value(parent, f->name);
    if(v && isa(v->type, t_function) > 0) {
      const m_bool match = parent_match_actual(env, f, v->d.func_ref);
      if(match)
        return match;
    }
  }
  if(GET_FLAG(func, member)) {
    if(!env->curr->vtable.ptr)
      vector_init(&env->curr->vtable);
    func->vt_index = vector_size(&env->curr->vtable);
    vector_add(&env->curr->vtable, (vtype)func);
  }
  return GW_OK;
}

ANN static m_bool check_func_args(const Env env, Arg_List arg_list) { GWDEBUG_EXE
  do {
    const Var_Decl decl = arg_list->var_decl;
    const Value v = decl->value;
    if(isa(v->type, t_object) > 0 || isa(v->type, t_function) > 0)
      UNSET_FLAG(env->func, pure);
    CHECK_BB(already_defined(env, decl->xid, decl->pos))
    SET_FLAG(v, checked);
    nspc_add_value(env->curr, decl->xid, v);
  } while((arg_list = arg_list->next));
  return GW_OK;
}

ANN static inline Func get_overload(const Env env, const Func_Def def, const m_uint i) {
  const Symbol sym = func_symbol(env->curr->name, s_name(def->name), NULL, i);
  return nspc_lookup_func1(env->curr, sym); // was lookup2
}

ANN static m_bool check_func_overload(const Env env, const Func_Def f) {
  const Value v = f->func->value_ref;
  for(m_uint i = 0; i <= v->offset; ++i) {
    const Func f1 = get_overload(env, f, i);
    for(m_uint j = i + 1; f1 && j <= v->offset; ++j) {
      const Func f2 = get_overload(env, f, j);
      if(f2 && compat_func(f1->def, f2->def) > 0)
        ERR_B(f2->def->td->xid->pos, "global function '%s' already defined"
          " for those arguments", s_name(f->name))
    }
  }
  return GW_OK;
}

ANN static m_bool check_func_def_override(const Env env, const Func_Def f) { GWDEBUG_EXE
  const Func func = f->func;
  if(env->class_def && env->class_def->parent) {
    const Value override = find_value(env->class_def->parent, f->name);
    if(override && override->owner_class && isa(override->type, t_function) < 0)
      ERR_B(f->td->xid->pos,
            "function name '%s' conflicts with previously defined value...\n"
            "\tfrom super class '%s'...",
            s_name(f->name), override->owner_class->name)
  }
  if(func->value_ref->offset && (!f->tmpl || !f->tmpl->base))
    CHECK_BB(check_func_overload(env, f))
  return GW_OK;
}

ANN static Value set_variadic(const Env env) {
  const Value variadic = new_value(env->gwion, t_vararg, "vararg");
  SET_FLAG(variadic, checked);
  nspc_add_value(env->curr, insert_symbol("vararg"), variadic);
  return variadic;
}

ANN static void operator_func(Func f) {
  const Arg_List a = f->def->arg_list;
  const m_bool is_unary = GET_FLAG(f->def, unary);
  const Type l = is_unary ? NULL : a->type;
  const Type r = is_unary ? a->type : a->next ? a->next->type : NULL;
  const Operator op = name2op(s_name(f->def->name));
  struct Op_Import opi = { .op=op, .lhs=l, .rhs=r, .data=(m_uint)f };
  operator_set_func(&opi);
}

ANN m_bool check_func_def(const Env env, const Func_Def f) { GWDEBUG_EXE
  const Func func = get_func(env, f);
  m_bool ret = 1;
  if(tmpl_list_base(f->tmpl))
    return env->class_def ? check_parent_match(env, f) : 1;
  CHECK_BB(check_func_def_override(env, f))
  if(env->class_def)
    CHECK_BB(check_parent_match(env, f))
  const Func former = env->func;
  env->func = func;
  ++env->scope;
  nspc_push_value(env->curr);
  if(f->arg_list)
    ret = check_func_args(env, f->arg_list);
  const Value variadic = GET_FLAG(f, variadic) ? set_variadic(env) : NULL;
  if(!GET_FLAG(f, builtin) && check_stmt_code(env, &f->d.code->d.stmt_code) < 0)
    ret = err_msg(f->td->xid->pos, "...in function '%s'", s_name(f->name));
  if(variadic)
    REM_REF(variadic)
  if(GET_FLAG(f, builtin))
    func->code->stack_depth = f->stack_depth;
  else if(GET_FLAG(f, op))
    operator_func(func);
  nspc_pop_value(env->curr);
  --env->scope;
  env->func = former;
  return ret;
}

DECL_SECTION_FUNC(check)

ANN static m_bool check_class_parent(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  if(class_def->ext->array) {
    CHECK_BB(check_exp_array_subscripts(env, class_def->ext->array->exp))
    if(!GET_FLAG(class_def->type, check) && class_def->tmpl)
      REM_REF(class_def->type->parent->nspc);
  }
  if(class_def->ext->types) {
    const Type t = class_def->type->parent->array_depth ?
      array_base(class_def->type->parent) : class_def->type->parent;
    if(!GET_FLAG(t, checked)) {
      if(class_def->tmpl)
        CHECK_BB(template_push_types(env, class_def->tmpl->list.list, class_def->tmpl->base))
      CHECK_BB(traverse_template(env, t->def))
      if(class_def->tmpl)
        nspc_pop_type(env->curr);
    }
  }
  if(!GET_FLAG(class_def->type->parent, checked))
    CHECK_BB(check_class_def(env, class_def->type->parent->def))
  if(GET_FLAG(class_def->type->parent, typedef))
    SET_FLAG(class_def->type, typedef);
  return GW_OK;
}

ANN static m_bool check_class_body(const Env env, const Class_Def class_def) {
  const m_uint scope = env_push_type(env, class_def->type);
  Class_Body body = class_def->body;
  do CHECK_BB(check_section(env, body->section))
  while((body = body->next));
  env_pop(env, scope);
  return GW_OK;
}

ANN static inline void inherit(const Type t) {
  const Nspc nspc = t->nspc, parent = t->parent->nspc;
  nspc->offset = parent->offset;
  if(parent->vtable.ptr)
    vector_copy2(&parent->vtable, &nspc->vtable);
}

ANN m_bool check_class_def(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  if(tmpl_class_base(class_def->tmpl))
    return GW_OK;
  const Type the_class = class_def->type;
  if(class_def->ext)
    CHECK_BB(check_class_parent(env, class_def))
  else
    the_class->parent = t_object;
  inherit(the_class);
  if(class_def->body)
    CHECK_BB(check_class_body(env, class_def))
  if(!the_class->p && the_class->nspc->offset)
    the_class->p = mp_ini((uint32_t)the_class->nspc->offset);
  SET_FLAG(the_class, checked | ae_flag_check);
  return GW_OK;
}

ANN m_bool check_ast(const Env env, Ast ast) { GWDEBUG_EXE
  do CHECK_BB(check_section(env, ast->section))
  while((ast = ast->next));
  return GW_OK;
}
