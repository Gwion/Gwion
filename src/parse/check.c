#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <dirent.h>
#include <inttypes.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "value.h"
#include "func.h"
#include "instr.h"
#include "import.h"
#include "traverse.h"
#include "optim.h"
#include "mpool.h" // for allocating object data

#define OP_RET(a, b)\
  const Type op_ret = op_check(env, &opi);\
  if(!op_ret)\
    ERR_O(TYPE_, a->self->pos, "in %s expression", b)\
  return op_ret;

ANN Type   check_exp(const Env env, Exp exp);
ANN static m_bool check_stmt(const Env env, const Stmt stmt);
ANN static m_bool check_stmt_list(const Env env, Stmt_List list);
ANN m_bool check_class_def(const Env env, const Class_Def class_def);

ANN m_bool check_exp_array_subscripts(Env env, Exp exp) { GWDEBUG_EXE
  CHECK_OB(check_exp(env, exp))
  do if(isa(exp->type, t_int) < 0)
      ERR_B(TYPE_, exp->pos, "incompatible array subscript type '%s' ...", exp->type->name)
  while((exp = exp->next));
  return 1;
}

ANN static inline m_bool check_exp_decl_parent(const Env env, const Var_Decl var) { GWDEBUG_EXE
  const Value value = find_value(env->class_def->parent, var->xid);
  if(value)
    ERR_B(TYPE_, var->pos,
          "in class '%s': '%s' has already been defined in parent class '%s' ...",
          env->class_def->name, s_name(var->xid), value->owner_class->name)
  return 1;
}

ANN static inline void check_exp_decl_member(const Nspc nspc, const Value v) { GWDEBUG_EXE
  v->offset = nspc->offset;
  nspc->offset += v->type->size;
}

ANN static void check_exp_decl_static(const Env env , const Value v) {
  const Nspc nspc = env->curr;
  SET_FLAG(v, ae_flag_static);
  v->offset = nspc->class_data_size;
  nspc->class_data_size += v->type->size;
}

ANN static void check_exp_decl_valid(const Env env, const Value v, const Symbol xid) {
  SET_FLAG(v, ae_flag_checked);
  if(!env->class_def || env->class_scope)
    nspc_add_value(env->curr, xid, v);
}

ANN static m_bool check_fptr_decl(const Env env, const Var_Decl var) {
  const Value v    = var->value;
  const Func  func = v->type->d.func;
  const Type type = func->value_ref->owner_class;

  if(!env->class_def) {
    if(!type || GET_FLAG(func, ae_flag_global)) {
      ADD_REF(var->value->type)
      return 1;
    }
    ERR_B(TYPE_, var->pos,
          "can't use non public typedef at global scope.")
  }
  if(isa(type, env->class_def) < 0)
    ERR_B(TYPE_, var->pos,
          "can't use static variables for member function.")
  if(GET_FLAG(func, ae_flag_member)) {
    if(GET_FLAG(v, ae_flag_static))
      ERR_B(TYPE_, var->pos,
            "can't use static variables for member function.")
    if(!GET_FLAG(v, ae_flag_member))
      ERR_B(TYPE_, var->pos,
            "can't use member variables for static function.")
  } else if(GET_FLAG(v, ae_flag_member))
  ERR_B(TYPE_, var->pos,
      "can't use member variables for static function.")
  ADD_REF(var->value->type)
  return 1;
}

ANN Type check_exp_decl(const Env env, const Exp_Decl* decl) { GWDEBUG_EXE
  Var_Decl_List list = decl->list;
  if(GET_FLAG(decl->type , ae_flag_template))
    CHECK_BO(traverse_template(env, decl->type->def))
  m_uint class_scope;
  const m_bool global = GET_FLAG(decl->td, ae_flag_global);
  if(global)
    env_push(env, NULL, env->global_nspc, &class_scope);
  do {
    const Var_Decl var = list->self;
    const Value value = var->value;
    if(isa(decl->type, t_fptr) > 0)
      CHECK_BO(check_fptr_decl(env, var))
    if(env->class_def && !env->class_scope && env->class_def->parent)
      CHECK_BO(check_exp_decl_parent(env, var))
    if(var->array && var->array->exp)
      CHECK_BO(check_exp_array_subscripts(env, var->array->exp))
    if(GET_FLAG(value, ae_flag_member))
      check_exp_decl_member(env->curr, value);
    else if(GET_FLAG(decl->td, ae_flag_static))
      check_exp_decl_static(env, value);
    check_exp_decl_valid(env, value, var->xid);
  } while((list = list->next));
  if(global)
    env_pop(env, class_scope);
  return decl->type;
}

ANN static m_bool check_exp_prim_array_inner(const Type t, Type type, const Exp e) { GWDEBUG_EXE
  const Type common = find_common_anc(t, type);
  if(common)
    type = common;
  else {
    if(isa(t, t_int) > 0 && isa(type, t_float) > 0) {
      e->cast_to = type;
      return 1;
    } else
      ERR_B(TYPE_, e->pos, "array init [...] contains incompatible types ...")
  }
  return 1;
}

ANN static Type check_exp_prim_array_match(Exp e) { GWDEBUG_EXE
  const Type type = e->type;
  do CHECK_BO(check_exp_prim_array_inner(e->type, type, e))
  while((e = e->next));
  return array_type(type->array_depth ? array_base(type) : type, type->array_depth + 1);
}

ANN static Type check_exp_prim_array(const Env env, const Array_Sub array) { GWDEBUG_EXE
  const Exp e = array->exp;
  if(!e)
    ERR_O(TYPE_, array->pos, "must provide values/expressions for array [...]")
  CHECK_OO(check_exp(env, e))
  return (array->type = check_exp_prim_array_match(e));
}

ANN static Value check_non_res_value(const Env env, const Exp_Primary* primary) { GWDEBUG_EXE
  const Value value = nspc_lookup_value1(env->curr, primary->d.var);
  if(env->class_def) {
    const Value v = value ? value : find_value(env->class_def, primary->d.var);
    if(v && SAFE_FLAG(env->func, ae_flag_static) && GET_FLAG(v, ae_flag_member))
      ERR_O(TYPE_, primary->self->pos,
            "non-static member '%s' used from static function.", s_name(primary->d.var))
    return v;
  }
  return value;
}

ANN static Type check_exp_prim_id_non_res(const Env env, const Exp_Primary* primary) { GWDEBUG_EXE
  const Value v = check_non_res_value(env, primary);
  if(!v || !GET_FLAG(v, ae_flag_checked))
    ERR_O(TYPE_, primary->self->pos,
          "variable %s not legit at this point.", s_name(primary->d.var))
  ((Exp_Primary*)primary)->value = v;
  if(GET_FLAG(v, ae_flag_const))
    primary->self->meta = ae_meta_value;
//  OPTIMIZE_PRIM_CONST(primary, v->d.ptr)
  return v->type;
}

ANN static Type check_exp_prim_this(const Env env, const Exp_Primary* primary) { GWDEBUG_EXE
  if(!env->class_def)
    ERR_O(TYPE_, primary->self->pos, "keyword 'this' can be used only inside class definition...")
  if(env->func && !GET_FLAG(env->func, ae_flag_member))
    ERR_O(TYPE_, primary->self->pos, "keyword 'this' cannot be used inside static functions...")
  primary->self->meta = ae_meta_value;
  return env->class_def;
}

ANN static inline Type check_exp_prim_me(const Exp_Primary* primary) { GWDEBUG_EXE
  primary->self->meta = ae_meta_value;
  return t_shred;
}

ANN static inline Type check_exp_prim_now(const Exp_Primary* primary) { GWDEBUG_EXE
  primary->self->meta = ae_meta_var;
  return t_now;
}

ANN static Type check_exp_prim_id2(const Env env, const Exp_Primary* primary) { GWDEBUG_EXE
  const m_str str = s_name(primary->d.var);
  if(!strcmp(str, "true") || !strcmp(str, "false") || !strcmp(str, "maybe")) {
    primary->self->meta = ae_meta_value;
    return t_int;
  } else
    return check_exp_prim_id_non_res(env, primary);
}

ANN static Type check_exp_prim_id1(const Env env, const Exp_Primary* primary) { GWDEBUG_EXE
  const m_str str = s_name(primary->d.var);
  if(!strcmp(str, "NULL") || !strcmp(str, "null")) {
    primary->self->meta = ae_meta_value;
    return t_null;
  } else if(!strcmp(str, "__func__")) {
    primary->self->meta = ae_meta_value;
    return t_string;
  } else
    return check_exp_prim_id2(env, primary);
}

ANN static Type check_exp_prim_id(const Env env, const Exp_Primary* primary) { GWDEBUG_EXE
  const m_str str = s_name(primary->d.var);
  if(!strcmp(str, "this"))
    return check_exp_prim_this(env, primary);
  else if(!strcmp(str, "me"))
    return check_exp_prim_me(primary);
  else if(!strcmp(str, "now"))
    return check_exp_prim_now(primary);
  else
    return check_exp_prim_id1(env, primary);
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
        ERR_B(TYPE_, e->pos,
              "invalid type '%s' in %s value #%d...\n"
              "    (must be of type 'int' or 'float')", t->name, s, count)
    }
    ++count;
  } while((e = e->next));
  return 1;
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

ANN static Type check_exp_prim_vec(const Env env, const Vec* vec, const ae_prim_t t) { GWDEBUG_EXE
  struct VecInfo info = { NULL, NULL, vec->dim };
  vec_info(t, &info);
  if(vec->dim > info.n)
    ERR_O(TYPE_, vec->exp->pos,
          "extraneous component of %s value...", info.s)
  CHECK_BO(vec_value(env, vec->exp, info.s))
  return info.t;
}

ANN static Type check_exp_prim_gack(const Env env, const Exp e) { GWDEBUG_EXE
  if(e->exp_type == ae_exp_decl)
    ERR_O(TYPE_, e->pos, "cannot use <<< >>> on variable declarations...\n")
  CHECK_OO((check_exp(env, e)))
  return t_gack;
}

ANN static Type check_exp_primary(const Env env, const Exp_Primary* primary) { GWDEBUG_EXE
  Type t = NULL;
  switch(primary->primary_type) {
    case ae_primary_id:
      t = check_exp_prim_id(env, primary);
      break;
    case ae_primary_num:
    case ae_primary_char:
      t = t_int;
      break;
    case ae_primary_float:
      t = t_float;
      break;
    case ae_primary_complex:
    case ae_primary_polar:
    case ae_primary_vec:
      t = check_exp_prim_vec(env, &primary->d.vec, primary->primary_type);
      break;
    case ae_primary_nil:
      t = t_void;
      break;
    case ae_primary_str:
      t = t_string;
      break;
    case ae_primary_hack:
      t = check_exp_prim_gack(env, primary->d.exp);
      break;
    case ae_primary_array:
      t = check_exp_prim_array(env, primary->d.array);
      break;
    OPTIMIZE_DEFAULT
  }
  return primary->self->type = t;
}

ANN Type check_exp_array(const Env env, const Exp_Array* array) { GWDEBUG_EXE
  Type t_base = check_exp(env, array->base);
  CHECK_OO(t_base)
  Exp e = array->array->exp;
  CHECK_OO(check_exp(env, e))
  m_uint depth = 0;
  do {
    ++depth;
    if(isa(e->type, t_int) < 0)
      ERR_O(TYPE_,  e->pos, "array index %i must be of type 'int', not '%s'",
            depth, e->type->name)
  } while((e = e->next));
  if(depth != array->array->depth)
    ERR_O(TYPE_, array->self->pos, "invalid array acces expression.")

  while(t_base && array->array->depth > t_base->array_depth) {
     depth -= t_base->array_depth;
     if(t_base->parent)
       t_base = t_base->parent;
     else
       ERR_O(TYPE_,  array->self->pos,
             "array subscripts (%i) exceeds defined dimension (%i)",
             array->array->depth, t_base->array_depth)
  }
  return depth == t_base->array_depth ? array_base(t_base) :
    array_type(array_base(t_base), t_base->array_depth - depth);
}

ANN m_bool compat_func(const restrict Func_Def lhs, const restrict Func_Def rhs) {
  Arg_List e1 = lhs->arg_list;
  Arg_List e2 = rhs->arg_list;

  while(e1 && e2) {
    if(e1->type != e2->type)
      return -1;
    e1 = e1->next;
    e2 = e2->next;
  }
  if(e1 || e2)
    return -1;
  return 1;
}

ANN static Type_List mk_type_list(const Env env, const Type type) {
  Nspc nspc = type->nspc;
  struct Vector_ v;
  vector_init(&v);
  if(!nspc)
    vector_add(&v, (vtype)type->name);
  while(nspc && nspc != env->curr && nspc != env->global_nspc) {
    vector_add(&v, (vtype)s_name(insert_symbol((nspc->name))));
    nspc = nspc->parent;
  }
  ID_List id = NULL;
  for(m_uint i = vector_size(&v) + 1; --i;)
    id = prepend_id_list(insert_symbol((m_str)vector_at(&v, i - 1)), id, 0);
  vector_release(&v);
  Type_Decl* td = new_type_decl(id, 0, 0);
  if(type->array_depth) {
    Array_Sub array = new_array_sub(NULL, 0);
    array->depth = type->array_depth;
    add_type_decl_array(td, array);
  }
  return new_type_list(td, NULL);
}

ANN static m_bool func_match_inner(const Env env, const Exp e, const Type t,
  const m_bool implicit, const m_bool specific) {
  const m_bool match = (specific ? e->type == t : isa(e->type, t) > 0) &&
    e->type->array_depth == t->array_depth &&
    array_base(e->type) == array_base(t);
  if(!match && implicit) {
    const struct Implicit imp = { e, t };
    struct Op_Import opi = { op_impl, e->type, t, NULL,
      NULL, NULL, (m_uint)&imp };
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
        if(GET_FLAG(func->def, ae_flag_variadic))
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

ANN2(1, 2) static Func find_func_match(const Env env, const Func up, Exp args) {
  Func func;
  if(args && isa(args->type, t_void) > 0)
    args = NULL;
  if((func = find_func_match_actual(env, up, args, 0, 1)) ||
     (func = find_func_match_actual(env, up, args, 1, 1)) ||
     (func = find_func_match_actual(env, up, args, 0, 0)) ||
     (func = find_func_match_actual(env, up, args, 1, 0)))
    return func;
  return NULL;
}

ANN static m_bool find_template_match_inner(const Env env, const Exp_Func* exp, const Func_Def def) {
  const m_bool ret = traverse_func_def(env, def);
  nspc_pop_type(env->curr);
  if(ret < 0 || !check_exp(env, exp->func) ||
     (exp->args  && !check_exp(env, exp->args)))
    return -1;
  return 1;
}

ANN static Value template_get_ready(const Env env, const Value v, const m_str tmpl,
    const m_uint len, const m_uint i) {
  char c[len + 2];
  snprintf(c, len + 2, "%s<%s>@%" INT_F "@%s", v->name, tmpl, i, env->curr->name);
  return v->owner_class ? find_value(v->owner_class, insert_symbol(c)) :
      nspc_lookup_value1(env->curr, insert_symbol(c));
}

ANN Func find_template_match(const Env env, const Value v, const Exp_Func* exp_func) {
  const Exp args = exp_func->args;
  const Type_List types = exp_func->tmpl->types;
  Func m_func = exp_func->m_func;
  m_int mismatch = 0;
  const m_uint digit = num_digit(v->offset + 1);
  const m_uint len = strlen(v->name) + strlen(env->curr->name);
  const m_str tmpl_name = tl2str(env, types);
  const m_uint tlen = strlen(tmpl_name);
  m_uint class_scope;
  env_push(env, v->owner_class, v->owner, &class_scope);
  for(m_uint i = 0; i < v->offset + 1; ++i) {
    Func_Def def = NULL;
    Func_Def base = NULL;
    Value value = template_get_ready(env, v, tmpl_name, len + tlen + digit + 3, i);
    if(value) {
      if(env->func == value->d.func_ref) {
        free(tmpl_name);
        if(!check_exp(env, exp_func->func) ||
            (exp_func->args && !check_exp(env, exp_func->args)))
          return NULL;
        return env->func;
      }
      base = def = value->d.func_ref->def;
    } else {
      char name[len + digit + 13];
      snprintf(name, len + digit + 13, "%s<template>@%" INT_F "@%s", v->name, i, env->curr->name);
      if(!(value = v->owner_class ? find_value(v->owner_class, insert_symbol(name)) :
            nspc_lookup_value1(env->curr, insert_symbol(name))))
      continue;
      mismatch = 0;
      base = value->d.func_ref->def;
      def = new_func_def(base->td, insert_symbol(v->name),
                base->arg_list, base->d.code, base->flag);
      def->tmpl = new_tmpl_list(value->d.func_ref->def->tmpl->list, i);
      UNSET_FLAG(base, ae_flag_template);
      SET_FLAG(def, ae_flag_template);
      if((mismatch = template_match(base->tmpl->list, types)) < 0)
        goto next;
    }
    if(template_push_types(env, base->tmpl->list, types) < 0)
      goto next;
    if(find_template_match_inner(env, exp_func, def) < 0)
      goto next;
    Func next = def->func->next;
    def->func->next = NULL;
    m_func = find_func_match(env, def->func, args);
    def->func->next = next;
    if(m_func) {
      free(tmpl_name);
      env_pop(env, class_scope);
      SET_FLAG(base, ae_flag_template);
      SET_FLAG(m_func, ae_flag_checked | ae_flag_template);
      return m_func;
    }
next:
    SET_FLAG(base, ae_flag_template);
    if(def->func) // still leaks
      def->func->def = NULL;
    free_func_def(def);
  }
  free(tmpl_name);
  if(mismatch < 0)
    ERR_O(TYPE_, exp_func->self->pos, "template type number mismatch.")
  env_pop(env, class_scope);
  (void)err_msg(TYPE_, exp_func->self->pos, "arguments do not match for template call");
  return NULL;
}

ANN static void print_current_args(Exp e) {
  gw_err("and not");
  gw_err("\n\t");
  do {
    gw_err(" \033[32m%s\033[0m", e->type->name);
    if(e->type->array_depth)
      REM_REF(e->type)
    if(e->next)
      gw_err(",");
  } while((e = e->next));
  gw_err("\n");
}

ANN static void print_arg(Arg_List e) {
  do {
    gw_err(" \033[32m%s\033[0m \033[1m%s\033[0m", e->type->name,
        s_name(e->var_decl->xid));
    if(e->next)
      gw_err(",");
  } while((e = e->next));
}

ANN2(1) static void* function_alternative(const Type f, Exp args){
  if(err_msg(TYPE_, args ? args->pos : 0, "argument type(s) do not match for function. should be :") < 0){}
  Func up = f->d.func;
  do {
    const Arg_List e = up->def->arg_list;
    gw_err("\t");
    if(e)
      print_arg(e);
    else
      gw_err("\033[32mvoid\033[0m");
    gw_err(". (%s)\n%s", f->name, up->next ? "or :" : "");
  } while((up = up->next));
  if(args)
    print_current_args(args);
  else
    gw_err("and not:\n\t\033[32mvoid\033[0m\n");
  return NULL;
}

ANN static Value get_template_value(const Env env, const Exp exp_func) {
  Value v = NULL;
  if(exp_func->exp_type == ae_exp_primary)
    v = nspc_lookup_value1(env->curr, exp_func->d.exp_primary.d.var);
  else if(exp_func->exp_type == ae_exp_dot)
    v = find_value(exp_func->d.exp_dot.t_base, exp_func->d.exp_dot.xid);
  if(v)
    UNSET_FLAG(v->d.func_ref->def, ae_flag_template);
  else
    ERR_O(TYPE_, exp_func->pos,
      "unhandled expression type '%" UINT_F "\' in template call.",
      exp_func->exp_type)
  return v;
}

ANN static m_uint get_type_number(ID_List list) {
  m_uint type_number = 0;
  do ++type_number;
  while((list = list->next));
  return type_number;
}

ANN static Func get_template_func(const Env env, const Exp_Func* func, const Exp base, const Value v) {
  const Func f = find_template_match(env, v, func);
  if(f) {
    Tmpl_Call* tmpl = new_tmpl_call(func->tmpl->types);
    tmpl->base = v->d.func_ref->def->tmpl->list;
    if(base->exp_type == ae_exp_call)
      base->d.exp_func.tmpl = tmpl;
    else if(base->exp_type == ae_exp_binary)
      base->d.exp_binary.tmpl = tmpl;
    return f;
  }
  (void)err_msg(TYPE_, func->self->pos,
        "function is template. automatic type guess not fully implemented yet.\n"
        "\tplease provide template types. eg: '<type1, type2, ...>'");
  return NULL;
}

ANN static Type check_exp_call_template(const Env env, const Exp restrict exp_func,
    const restrict Exp args, const restrict Exp base) {
  m_uint args_number = 0;
  ID_List list;
  const Value value = get_template_value(env, exp_func);
  CHECK_OO(value)
  const m_uint type_number = get_type_number(value->d.func_ref->def->tmpl->list);

  list = value->d.func_ref->def->tmpl->list;
  Type_List tl[type_number];
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
    ERR_O(TYPE_, exp_func->pos,
          "not able to guess types for template call.")
  Tmpl_Call tmpl = { tl[0], NULL };
  const Exp_Func tmp_func = { exp_func, args, NULL, &tmpl, NULL };
  Func func = get_template_func(env, &tmp_func, base, value);
  if(base->exp_type == ae_exp_call)
    base->d.exp_func.m_func = func;
  else if(base->exp_type == ae_exp_binary)
    base->d.exp_binary.func = func;
  return func ? func->def->ret_type : NULL;
}

ANN static m_bool check_exp_call1_check(const Env env, const Exp exp_func, Value* ptr) { GWDEBUG_EXE
  if(!(exp_func->type = check_exp(env, exp_func)))
    ERR_B(TYPE_, exp_func->pos,
          "function call using a non-existing function")
  if(isa(exp_func->type, t_function) < 0)
    ERR_B(TYPE_, exp_func->pos,
          "function call using a non-function value")
  if(exp_func->exp_type == ae_exp_primary && exp_func->d.exp_primary.value &&
    !GET_FLAG(exp_func->d.exp_primary.value, ae_flag_const))
      *ptr = exp_func->d.exp_primary.value;
  return 1;
}

ANN2(1,2) Type check_exp_call1(const Env env, const restrict Exp exp_func,
    const restrict Exp args, restrict Exp base) { GWDEBUG_EXE
  Value ptr = NULL;
  CHECK_BO(check_exp_call1_check(env, exp_func, &ptr))
  if(exp_func->type->d.func) {
    const Value value = exp_func->type->d.func->value_ref;
    if(GET_FLAG(exp_func->type->d.func, ae_flag_ref))
    CHECK_BO(traverse_template(env, value->owner_class->def))
  }
  if(args)
    CHECK_OO(check_exp(env, args))
  if(!exp_func->type->d.func)
    return check_exp_call_template(env, exp_func, args, base);
  Func func = find_func_match(env, exp_func->type->d.func, args);
  if(!func)
    return function_alternative(exp_func->type, args);
  if(ptr) {
    const Func f = new_func_simple();
    memcpy(f, func, sizeof(struct Func_));
    f->value_ref = ptr;
    SET_FLAG(ptr, ae_flag_func); // there might be a better place
    if(ptr->d.func_ref)
      f->next = ptr->d.func_ref;
    func = ptr->d.func_ref = f;
  }
  if(base->exp_type == ae_exp_call)
    base->d.exp_func.m_func = func;
  else // if(base->exp_type == ae_exp_binary)
    base->d.exp_binary.func = func;
  return func->def->ret_type;
}

ANN static m_bool multi_decl(const Exp e, const Operator op) {
  if(e->exp_type == ae_exp_decl &&  e->d.exp_decl.list->next)
    ERR_B(TYPE_, e->pos,
          "cant '%s' from/to a multi-variable declaration.", op2str(op))
  return 1;
}

ANN static Type check_exp_binary(const Env env, const Exp_Binary* bin) { GWDEBUG_EXE
  struct Op_Import opi = { bin->op,
    check_exp(env, bin->lhs), check_exp(env, bin->rhs), NULL,
    NULL, NULL, (uintptr_t)bin };

  CHECK_BO(multi_decl(bin->lhs, bin->op));
  CHECK_BO(multi_decl(bin->rhs, bin->op));
  CHECK_OO(opi.lhs)
  CHECK_OO(opi.rhs)
  const Type op_ret = op_check(env, &opi);
  if(!op_ret)
    ERR_O(TYPE_, bin->self->pos, "in binary expression")
  OPTIMIZE_CONST(bin)
  return op_ret;
}

ANN static Type check_exp_cast(const Env env, const Exp_Cast* cast) { GWDEBUG_EXE
  const Type t = check_exp(env, cast->exp);
  CHECK_OO(t)
  CHECK_OO((cast->self->type = known_type(env, cast->td, "cast expression")))
  struct Op_Import opi = { op_cast, t, cast->self->type, NULL,
    NULL, NULL, (uintptr_t)cast };
  OP_RET(cast, "cast")
}

ANN static Type check_exp_post(const Env env, const Exp_Postfix* post) { GWDEBUG_EXE
  struct Op_Import opi = { post->op, check_exp(env, post->exp), NULL, NULL,
    NULL, NULL, (uintptr_t)post };
  CHECK_OO(opi.lhs)
  OP_RET(post, "postfix");
}

ANN static Type check_exp_dur(const Env env, const Exp_Dur* dur) { GWDEBUG_EXE
  const Type base = check_exp(env, dur->base);
  CHECK_OO(base)
  const Type unit = check_exp(env, dur->unit);
  CHECK_OO(unit)
  if(isa(base, t_int) < 0 && isa(base, t_float) < 0)
    ERR_O(TYPE_, dur->base->pos,
          "invalid type '%s' in prefix of dur expression...\n"
          "    (must be of type 'int' or 'float')", base->name)
  if(isa(unit, t_dur) < 0)
    ERR_O(TYPE_, dur->unit->pos,
          "invalid type '%s' in postfix of dur expression...\n"
          "    (must be of type 'dur')", unit->name)
  return unit;
}

ANN static Type check_exp_call(const Env env, Exp_Func* call) { GWDEBUG_EXE
  if(call->tmpl) {
    CHECK_OO(check_exp(env, call->func)) // → puts this up ?
    const Type t = actual_type(call->func->type);
    const Value v = nspc_lookup_value1(t->owner, insert_symbol(t->name));
    if(!v)
      ERR_O(TYPE_, call->self->pos,
            " template call of non-existant function.")
    if(!GET_FLAG(v, ae_flag_func))
      ERR_O(TYPE_, call->self->pos,
            "template call of non-function value.")
    if(!v->d.func_ref->def->tmpl)
      ERR_O(TYPE_, call->self->pos,
            "template call of non-template function.")
    const Func ret = find_template_match(env, v, call);
    CHECK_OO((call->m_func = ret))
    return ret->def->ret_type;
  }
  return check_exp_call1(env, call->func, call->args, call->self);
}

//static
// Move me ?
ANN Type check_exp_unary_spork(const Env env, const Stmt code) { GWDEBUG_EXE
  ++env->class_scope;
  nspc_push_value(env->curr);
  const m_bool ret = check_stmt(env, code);
  nspc_pop_value(env->curr);
  --env->class_scope;
  return (ret > 0) ? t_shred : NULL;
}

ANN static Type check_exp_unary(const Env env, const Exp_Unary* unary) { GWDEBUG_EXE
  struct Op_Import opi = { unary->op, NULL, unary->exp ? check_exp(env, unary->exp) : NULL,
    NULL, NULL, NULL, (uintptr_t)unary };
  if(unary->exp && !opi.rhs)return NULL;
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
    ERR_O(TYPE_, exp_if->self->pos,
          "Invalid type '%s' in if expression condition.", cond->name)
  const Type ret = find_common_anc(if_exp, else_exp);
  if(!ret)
    ERR_O(TYPE_, exp_if->self->pos,
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
    ERR_O(TYPE_,  member->base->pos,
          "type '%s' does not have members - invalid use in dot expression of %s",
          the_base->name, str)
  if(!strcmp(str, "this") && base_static)
    ERR_O(TYPE_,  member->self->pos,
          "keyword 'this' must be associated with object instance...")
  const Value value = find_value(the_base, member->xid);
  if(!value)
    ERR_O(TYPE_,  member->base->pos,
          "class '%s' has no member '%s'", the_base->name, str)
  if(!env->class_def || isa(env->class_def, value->owner_class) < 0) {
    if(GET_FLAG(value, ae_flag_private))
      ERR_O(TYPE_,  member->self->pos,
          "can't access private '%s' outside of class...", value->name)
    else if(GET_FLAG(value, ae_flag_protect))
      member->self->meta = ae_flag_protect;
  }
  if(base_static && GET_FLAG(value, ae_flag_member))
    ERR_O(TYPE_, member->self->pos,
          "cannot access member '%s.%s' without object instance...",
          the_base->name, str)
  if(GET_FLAG(value, ae_flag_const) || GET_FLAG(value, ae_flag_enum))
    member->self->meta = ae_meta_value;
  return value->type;
}

ANN static m_bool check_stmt_type(const Env env, const Stmt_Type stmt) { GWDEBUG_EXE
  return stmt->type->def ? check_class_def(env, stmt->type->def) : 1;
}

ANN Type check_exp(const Env env, const Exp exp) { GWDEBUG_EXE
  Exp curr = exp;
  do {
    switch(curr->exp_type) {
      case ae_exp_primary:
        curr->type = check_exp_primary(env, &curr->d.exp_primary);
        break;
      case ae_exp_decl:
        curr->type = check_exp_decl(env, &curr->d.exp_decl);
        break;
      case ae_exp_unary:
        curr->type = check_exp_unary(env, &curr->d.exp_unary);
        break;
      case ae_exp_binary:
        curr->type = check_exp_binary(env, &curr->d.exp_binary);
        break;
      case ae_exp_post:
        curr->type = check_exp_post(env, &curr->d.exp_post);
        break;
      case ae_exp_dur:
        curr->type = check_exp_dur(env, &curr->d.exp_dur);
        break;
      case ae_exp_cast:
        curr->type = check_exp_cast(env, &curr->d.exp_cast);
        break;
      case ae_exp_call:
        curr->type = check_exp_call(env, &curr->d.exp_func);
        break;
      case ae_exp_if:
        curr->type = check_exp_if(env, &curr->d.exp_if);
        break;
      case ae_exp_dot:
        curr->type = check_exp_dot(env, &curr->d.exp_dot);
        break;
      case ae_exp_array:
        curr->type = check_exp_array(env, &curr->d.exp_array);
        break;
#ifdef OPTIMIZE
      default:break;
#endif
    }
    CHECK_OO(curr->type)
  } while((curr = curr->next));
  return exp->type;
}

ANN m_bool check_stmt_enum(const Env env, const Stmt_Enum stmt) { GWDEBUG_EXE
  if(env->class_def) {
    ID_List list = stmt->list;
    do {
      const Value v = nspc_lookup_value0(env->curr, list->xid);
      SET_FLAG(v, ae_flag_static);
      v->offset = env->class_def->nspc->class_data_size;
      env->class_def->nspc->class_data_size += SZ_INT;
    } while((list = list->next));
  }
  return 1;
}

ANN static m_bool check_stmt_code(const Env env, const Stmt_Code stmt) { GWDEBUG_EXE
  return stmt->stmt_list ? check_stmt_list(env, stmt->stmt_list) : 1;
}

ANN static m_bool check_flow(const Exp exp) { GWDEBUG_EXE
  if(isa(exp->type, t_object) > 0 || isa(exp->type, t_int) > 0 || isa(exp->type, t_float) > 0 ||
     isa(exp->type, t_dur) > 0 || isa(exp->type, t_time)  > 0)
    return 1;
  ERR_B(TYPE_,  exp->pos, "invalid type '%s'", exp->type->name)
  return 1;
}

ANN static m_bool check_stmt_flow(const Env env, const Stmt_Flow stmt) { GWDEBUG_EXE
  if(!stmt->is_do)
    CHECK_OB(check_exp(env, stmt->cond))
  vector_add(&env->breaks, (vtype)stmt->self);
  vector_add(&env->conts, (vtype)stmt->self);
  nspc_push_value(env->curr);
  CHECK_BB(check_stmt(env, stmt->body))
  nspc_pop_value(env->curr);
  if(stmt->is_do)
    CHECK_OB(check_exp(env, stmt->cond))
  if(check_flow(stmt->cond) < 0) {
    const m_str str = stmt->self->stmt_type == ae_stmt_while ? "while" : "until";
    ERR_B(TYPE_, stmt->cond->pos, "\t... in '%s' condition.", str)
  }
  vector_pop(&env->breaks);
  vector_pop(&env->conts);
  return 1;
}

ANN static m_bool check_breaks(const Env env, const Stmt a, const Stmt b) { GWDEBUG_EXE
  vector_add(&env->breaks, (vtype)a);
  CHECK_BB(check_stmt(env, b))
  vector_pop(&env->breaks);
  return 1;
}

ANN static m_bool check_stmt_for(const Env env, const Stmt_For stmt) { GWDEBUG_EXE
  CHECK_BB(check_stmt(env, stmt->c1))
  CHECK_BB(check_stmt(env, stmt->c2))
  if(!stmt->c2 || !stmt->c2->d.stmt_exp.val)
    ERR_B(EMIT_, stmt->self->pos, "empty for loop condition...",
          "...(note: explicitly use 'true' if it's the intent)",
          "...(e.g., 'for(; true;){ /*...*/ }')")
  if(check_flow(stmt->c2->d.stmt_exp.val) < 0)
    ERR_B(TYPE_, stmt->c2->pos, "\t... in 'for' condition.")
  if(stmt->c3)
    CHECK_OB(check_exp(env, stmt->c3))
  return check_breaks(env, stmt->self, stmt->body);
}

ANN static m_bool check_stmt_auto(const Env env, const Stmt_Auto stmt) { GWDEBUG_EXE
  Type t = check_exp(env, stmt->exp);
  Type ptr = array_base(t);
  const m_uint depth = t->array_depth - 1;
  if(GET_FLAG(t, ae_flag_typedef))
    t = t->parent;
  if(!t || !ptr || isa(t, t_array) < 0)
    ERR_B(TYPE_, stmt->self->pos, "type '%s' is not array.\n"
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
    if(!GET_FLAG(ptr, ae_flag_checked))
      check_class_def(env, ptr->def);
  }
  t = depth ? array_type(ptr, depth) : ptr;
  stmt->v = new_value(t, s_name(stmt->sym));
  SET_FLAG(stmt->v, ae_flag_checked);
  nspc_add_value(env->curr, stmt->sym, stmt->v);
  return check_breaks(env, stmt->self, stmt->body);
}

ANN static m_bool check_stmt_loop(const Env env, const Stmt_Loop stmt) { GWDEBUG_EXE
  const Type type = check_exp(env, stmt->cond);
  CHECK_OB(type)
  if(isa(type, t_int) < 0) {
    if(isa(type, t_float) > 0)
      stmt->cond->cast_to = t_int;
    else
      ERR_B(TYPE_, stmt->self->pos,
            "loop * conditional must be of type 'int'...")
  }
  return check_breaks(env, stmt->self, stmt->body);
}

ANN static m_bool check_stmt_if(const Env env, const Stmt_If stmt) { GWDEBUG_EXE
  CHECK_OB(check_exp(env, stmt->cond))
  if(isa(stmt->cond->type, t_object) > 0)
    if(check_flow(stmt->cond) < 0)
      ERR_B(TYPE_, stmt->cond->pos, "\t... in 'if' condition.")
  CHECK_BB(check_stmt(env, stmt->if_body))
  if(stmt->else_body)
    CHECK_BB(check_stmt(env, stmt->else_body))
  return 1;
}

ANN static m_bool check_stmt_return(const Env env, const Stmt_Exp stmt) { GWDEBUG_EXE
  if(!env->func)
    ERR_B(TYPE_, stmt->self->pos,
          "'return' statement found outside function definition")
  const Type ret_type = stmt->val ? check_exp(env, stmt->val) : t_void;
  CHECK_OB(ret_type)
  if(isa(ret_type, t_null) > 0 &&
     isa(env->func->def->ret_type, t_object) > 0)
    return 1;
  if(isa(ret_type, env->func->def->ret_type) < 0)
    ERR_B(TYPE_, stmt->self->pos,
          "invalid return type '%s' -- expecting '%s'",
          ret_type->name, env->func->def->ret_type->name)
  return 1;
}

ANN static m_bool check_stmt_continue(const Env env, const Stmt stmt) { GWDEBUG_EXE
  if(!vector_size(&env->conts))
    ERR_B(TYPE_, stmt->pos,
             "'continue' found outside of for/while/until...")
  return 1;
}

ANN static m_bool check_stmt_break(const Env env, const Stmt stmt) { GWDEBUG_EXE
  if(!vector_size(&env->breaks))
    ERR_B(TYPE_,  stmt->pos,
             "'break' found outside of for/while/until...")
  return 1;
}

ANN static m_bool check_stmt_switch(const Env env, const Stmt_Switch a) { GWDEBUG_EXE
  const Type t = check_exp(env, a->val);
  if(!t || isa(t, t_int) < 0)
    ERR_B(TYPE_, a->val->pos,
          "invalid type '%s' in switch expression. should be 'int'",
          t ? t->name : "unknown")
  return check_breaks(env, a->self, a->stmt);
}

ANN static m_bool check_stmt_case(const Env env, const Stmt_Exp stmt) { GWDEBUG_EXE
  if(stmt->val->exp_type  != ae_exp_primary &&
      stmt->val->exp_type != ae_exp_dot)
    ERR_B(TYPE_, stmt->self->pos,
          "unhandled expression type '%i'", stmt->val->exp_type)
  const Type t = check_exp(env, stmt->val);
  if(!t || isa(t, t_int) < 0)
    ERR_B(TYPE_, stmt->self->pos,
          "invalid type '%s' case expression. should be 'int'",
          t ? t->name : "unknown")
  return 1;
}

ANN static m_bool check_stmt_jump(const Env env, const Stmt_Jump stmt) { GWDEBUG_EXE
  if(stmt->is_label)
    return 1;
  const Map label = env_label(env);
  const m_uint* key = env->class_def && !env->func ?
                (m_uint*)env->class_def : (m_uint*)env->func;
  const Map m = label->ptr ? (Map)map_get(label, (vtype)key) : NULL;
  if(!m)
    ERR_B(TYPE_, stmt->self->pos,
          "label '%s' used but not defined", s_name(stmt->name))
  const Stmt_Jump ref = (Stmt_Jump)map_get(m, (vtype)stmt->name);
  if(!ref) {
    for(m_uint i = 0; i < map_size(m); ++i) {
      const Stmt_Jump s = (Stmt_Jump)map_at(m, i);
      vector_release(&s->data.v);
    }
    ERR_B(TYPE_, stmt->self->pos,
          "label '%s' used but not defined", s_name(stmt->name))
  }
  vector_add(&ref->data.v, (vtype)stmt);
  return 1;
}

ANN m_bool check_stmt_union(const Env env, const Stmt_Union stmt) { GWDEBUG_EXE
  Decl_List l = stmt->l;
  m_uint class_scope;
  const m_bool global = GET_FLAG(stmt, ae_flag_global);

  if(stmt->xid) {
    if(env->class_def) {
      if(!GET_FLAG(stmt, ae_flag_static))
        check_exp_decl_member(env->curr, stmt->value);
      else
        check_exp_decl_static(env, stmt->value);
    }
    env_push(env, stmt->value->type, stmt->value->type->nspc, &class_scope);
  } else if(env->class_def)  {
      if(!GET_FLAG(stmt, ae_flag_static))
        stmt->o = env->class_def->nspc->offset;
      else {
        env->class_def->nspc->class_data_size += SZ_INT;
        env->class_def->nspc->offset += SZ_INT;
      }
  } else if(global)
    env_push(env, NULL, env->global_nspc, &class_scope);

  do {
    CHECK_OB(check_exp(env, l->self))
    if(isa(l->self->type, t_object) > 0 && !GET_FLAG(l->self->d.exp_decl.td, ae_flag_ref))
      ERR_B(TYPE_, l->self->pos, "In union, Objects must be declared as reference (use '@')")
    if(l->self->type->size > stmt->s)
      stmt->s = l->self->type->size;
  } while((l = l->next));
  if(stmt->xid || global)
    env_pop(env, class_scope);
  return 1;
}

ANN static m_bool check_stmt(const Env env, const Stmt stmt) { GWDEBUG_EXE
  m_bool ret = 1;
  switch(stmt->stmt_type) {
    case ae_stmt_exp:
      if(stmt->d.stmt_exp.val)
        ret = (check_exp(env, stmt->d.stmt_exp.val) ? 1 : -1);
      break;
    case ae_stmt_code:
      NSPC(ret = check_stmt_code(env, &stmt->d.stmt_code))
      break;
    case ae_stmt_return:
      ret = check_stmt_return(env, &stmt->d.stmt_exp);
      break;
    case ae_stmt_break:
      ret = check_stmt_break(env, stmt);
      break;
    case ae_stmt_continue:
      ret = check_stmt_continue(env, stmt);
      break;
    case ae_stmt_if:
      NSPC(ret = check_stmt_if(env, &stmt->d.stmt_if));
      break;
    case ae_stmt_while:
      NSPC(ret = check_stmt_flow(env, &stmt->d.stmt_flow))
      break;
    case ae_stmt_until:
      NSPC(ret = check_stmt_flow(env, &stmt->d.stmt_flow))
      break;
    case ae_stmt_for:
      NSPC(ret = check_stmt_for(env, &stmt->d.stmt_for))
      break;
    case ae_stmt_auto:
      NSPC(ret = check_stmt_auto(env, &stmt->d.stmt_auto))
      break;
    case ae_stmt_loop:
      NSPC(ret = check_stmt_loop(env, &stmt->d.stmt_loop))
      break;
    case ae_stmt_switch:
      NSPC(ret = check_stmt_switch(env, &stmt->d.stmt_switch))
      break;
    case ae_stmt_case:
      ret = check_stmt_case(env, &stmt->d.stmt_exp);
      break;
    case ae_stmt_enum:
      ret = check_stmt_enum(env, &stmt->d.stmt_enum);
      break;
    case ae_stmt_jump:
      ret = check_stmt_jump(env, &stmt->d.stmt_jump);
      break;
    case ae_stmt_fptr:
      ret = 1;
      break;
    case ae_stmt_type:
      ret = check_stmt_type(env, &stmt->d.stmt_type);
      break;
    case ae_stmt_union:
      ret = check_stmt_union(env, &stmt->d.stmt_union);
      break;
  }
  return ret;
}

ANN static m_bool check_stmt_list(const Env env, Stmt_List l) { GWDEBUG_EXE
  do CHECK_BB(check_stmt(env, l->stmt))
  while((l = l->next));
  return 1;
}

ANN static m_bool check_signature_match(const Func_Def f, const Func parent) { GWDEBUG_EXE
  const m_str c_name  = f->func->value_ref->owner_class->name;
  const m_str p_name = parent->value_ref->owner_class->name;
  const m_str f_name = s_name(f->name);
  if(GET_FLAG(parent->def, ae_flag_static) || GET_FLAG(f, ae_flag_static)) {
    ERR_B(TYPE_, f->td->pos,
          "function '%s.%s' ressembles '%s.%s' but cannot override...\n"
          "\t...(reason: '%s.%s' is declared as 'static')",
          c_name, f_name, p_name, c_name,
          GET_FLAG(f, ae_flag_static) ? c_name : p_name, f_name)
  }
  if(isa(f->ret_type, parent->def->ret_type) <  0) {
    ERR_B(TYPE_, f->td->pos,
          "function signatures differ in return type...\n"
          "\tfunction '%s.%s' matches '%s.%s' but cannot override...",
          c_name, f_name, p_name, f_name)
  }
  return 1;
}

ANN static m_bool parent_match_actual(const Env env, const restrict Func_Def f,
    const restrict Func func) {
  Func parent_func = func;
  do {
    if(compat_func(f, parent_func->def) > 0) {
      CHECK_BB(check_signature_match(f, parent_func))
      const Func func = f->func;
      func->vt_index = parent_func->vt_index;
      vector_set(&env->curr->vtable, func->vt_index, (vtype)func);
      return 1;
    }
  } while((parent_func = parent_func->next));
  return 0;
}

ANN static m_bool check_parent_match(const Env env, const Func_Def f) { GWDEBUG_EXE
  const Func func = f->func;
  const Type parent = env->class_def->parent;
  if(parent) {
    const Value v = find_value(parent, f->name);
    if(v) {
      const m_bool match = parent_match_actual(env, f, v->d.func_ref);
      if(match)
        return match;
    }
  }
  if(GET_FLAG(func, ae_flag_member)) {
    if(!env->curr->vtable.ptr)
      vector_init(&env->curr->vtable);
    func->vt_index = vector_size(&env->curr->vtable);
    vector_add(&env->curr->vtable, (vtype)func);
  }
  return 1;
}

ANN static m_bool check_func_args(const Env env, Arg_List arg_list) { GWDEBUG_EXE
  do {
    const Var_Decl decl = arg_list->var_decl;
    const Value v = decl->value;
    CHECK_BB(already_defined(env, decl->xid, decl->pos))
    SET_FLAG(v, ae_flag_checked);
    nspc_add_value(env->curr, decl->xid, v);
  } while((arg_list = arg_list->next));
  return 1;
}

ANN static m_bool check_func_overload_inner(const Env env, const Func_Def def,
  const m_str name, const m_uint j) {
  sprintf(name, "%s@%" INT_F "@%s", s_name(def->name), j, env->curr->name);
  const Func f2 = nspc_lookup_func2(env->curr, insert_symbol(name));
  if(f2 && compat_func(def, f2->def) > 0) {
    ERR_B(TYPE_, f2->def->td->pos,
        "global function '%s' already defined for those arguments",
        s_name(def->name))
  }
  return 1;
}

ANN static m_bool check_func_overload(const Env env, const Func_Def f) { GWDEBUG_EXE
  const Value v = f->func->value_ref;
  if(!f->tmpl || !f->tmpl->base) {
    char name[strlen(s_name(f->name)) + strlen(env->curr->name) +
                                      num_digit(v->offset) + 3];
    for(m_uint i = 0; i <= v->offset; ++i) {
      sprintf(name, "%s@%" INT_F "@%s", s_name(f->name), i, env->curr->name);
      const Func f1 = nspc_lookup_func2(env->curr, insert_symbol(name));
      for(m_uint j = 1; j <= v->offset; ++j) {
        if(i != j && f1)
          CHECK_BB(check_func_overload_inner(env, f1->def, name, j))
      }
    }
  }
  return 1;
}

ANN static m_bool check_func_def_override(const Env env, const Func_Def f) { GWDEBUG_EXE
  const Func func = f->func;
  if(env->class_def && env->class_def->parent) {
    const Value override = find_value(env->class_def->parent, f->name);
    if(override && isa(override->type, t_function) < 0)
      ERR_B(TYPE_, f->td->pos,
            "function name '%s' conflicts with previously defined value...\n"
            "\tfrom super class '%s'...",
            s_name(f->name), override->owner_class->name)
  }
  if(func->value_ref->offset && (!f->tmpl || !f->tmpl->base))
    CHECK_BB(check_func_overload(env, f))
  return 1;
}

ANN static Value set_variadic(const Env env) {
  const Value variadic = new_value(t_vararg, "vararg");
  SET_FLAG(variadic, ae_flag_checked);
  nspc_add_value(env->curr, insert_symbol("vararg"), variadic);
  return variadic;
}

ANN m_bool check_func_def(const Env env, const Func_Def f) { GWDEBUG_EXE
  const Func func = get_func(env, f);
  m_bool ret = 1;

  if(tmpl_list_base(f->tmpl))
    return 1;
//  if(env->class_def && env->class_def->parent)
//  if(!GET_FLAG(f, ae_flag_builtin))
  CHECK_BB(check_func_def_override(env, f))
  if(env->class_def)
    CHECK_BB(check_parent_match(env, f))
  const Func former = env->func;
  if(former) // parsing a template call in a func
    SET_FLAG(former, ae_flag_recurs);

  env->func = func;
  ++env->class_scope;
  nspc_push_value(env->curr);
  if(f->arg_list)
    ret = check_func_args(env, f->arg_list);
  const Value variadic = GET_FLAG(f, ae_flag_variadic) ? set_variadic(env) : NULL;
  if(!GET_FLAG(f, ae_flag_builtin) && check_stmt_code(env, &f->d.code->d.stmt_code) < 0)
    ret = err_msg(TYPE_, f->td->pos,
                  "...in function '%s'", s_name(f->name));
  if(variadic)
    REM_REF(variadic)
  if(GET_FLAG(f, ae_flag_builtin)) {
    if(GET_FLAG(func, ae_flag_member | ae_flag_ref))
      f->stack_depth += SZ_INT;
    func->code->stack_depth = f->stack_depth;
  }
  nspc_pop_value(env->curr);
  --env->class_scope;
  env->func = former;
  return ret;
}

ANN static m_bool check_section(const Env env, const Section* section) { GWDEBUG_EXE
  const ae_section_t t = section->section_type;
  if(t == ae_section_stmt)
    CHECK_BB(check_stmt_list(env, section->d.stmt_list))
  else if(t == ae_section_func)
    CHECK_BB(check_func_def(env, section->d.func_def))
  else if(t == ae_section_class)
    CHECK_BB(check_class_def(env, section->d.class_def))
  return 1;
}

ANN static m_bool check_class_parent(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  if(class_def->ext->array) {
    CHECK_BB(check_exp_array_subscripts(env, class_def->ext->array->exp))
    if(!GET_FLAG(class_def->type, ae_flag_check) && class_def->tmpl)
      REM_REF(class_def->type->parent->nspc);
  }
  if(class_def->ext->types) {
    const Type t = class_def->type->parent->array_depth ?
      array_base(class_def->type->parent) : class_def->type->parent;
    if(!GET_FLAG(t, ae_flag_checked)) {
      if(class_def->tmpl)
        CHECK_BB(template_push_types(env, class_def->tmpl->list.list, class_def->tmpl->base))
      CHECK_BB(traverse_template(env, t->def))
      if(class_def->tmpl)
        nspc_pop_type(env->curr);
    }
  }
  if(isa(class_def->type->parent, t_object) < 0)
    ERR_B(TYPE_, class_def->ext->pos,
            "cannot extend primitive type '%s'", class_def->type->parent->name)
  if(!GET_FLAG(class_def->type->parent, ae_flag_checked)) {
    if(GET_FLAG(class_def->ext, ae_flag_typedef)) // ??????
      CHECK_BB(check_class_def(env, class_def->type->parent->def))
    else
      ERR_B(TYPE_, class_def->ext->pos,
            "cannot extend incomplete type '%s'\n"
            "\t...(note: the parent's declaration must preceed child's)",
            class_def->type->parent->name)
  }
  if(GET_FLAG(class_def->type->parent, ae_flag_typedef))
    SET_FLAG(class_def->type, ae_flag_typedef);
  return 1;
}

ANN static m_bool check_class_body(const Env env, const Class_Def class_def) {
  Class_Body body = class_def->body;
  m_uint class_scope;
  env_push(env, class_def->type, class_def->type->nspc, &class_scope);
  do CHECK_BB(check_section(env, body->section))
  while((body = body->next));
  env_pop(env, class_scope);
  return 1;
}

ANN static inline void inherit(const Type t) {
  const Nspc nspc = t->nspc, parent = t->parent->nspc;
  nspc->offset = parent->offset;
  if(parent->vtable.ptr)
    vector_copy2(&parent->vtable, &nspc->vtable);
}

ANN m_bool check_class_def(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  if(tmpl_class_base(class_def->tmpl))
    return 1;
  const Type the_class = class_def->type;
  if(class_def->ext)
    CHECK_BB(check_class_parent(env, class_def))
  else
    the_class->parent = t_object;
  inherit(the_class);
  if(class_def->body)
    CHECK_BB(check_class_body(env, class_def))
  if(!the_class->p && the_class->nspc->offset)
    the_class->p = mp_ini(the_class->nspc->offset);
  SET_FLAG(the_class, ae_flag_checked | ae_flag_check);
  return 1;
}

ANN m_bool check_ast(const Env env, Ast ast) { GWDEBUG_EXE
  do CHECK_BB(check_section(env, ast->section))
  while((ast = ast->next));
  return 1;
}
