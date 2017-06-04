#include <stdlib.h>
#include "absyn.h"
#include "func.h"
#include "type.h"

void free_expression(Expression exp);
void free_arg_list(Arg_List a);
static void free_stmt(Stmt a);
static void free_stmt_list(Stmt_List a);
static void free_stmt_code(Stmt_Code a);
static void free_section();

Var_Decl new_var_decl(m_str name, Array_Sub array, int pos) {
  Var_Decl a = calloc(1, sizeof(struct Var_Decl_));
  a->xid = insert_symbol(name);
  a->array = array;
  a->pos = pos;
  return a;
}

__inline static void free_array_sub(Array_Sub a) {
  free_expression(a->exp_list);
  free(a);
}

static void free_var_decl(Var_Decl a) {
  if(a->value) {
    if(GET_FLAG(a->value, ae_value_arg)) { // func argument. this migth change
      if(a->value->m_type->array_type)
        REM_REF(a->value->m_type);
      REM_REF(a->value);
    } else if(!a->value->owner_class) { // breaks for loop ?
      if(a->value->m_type->array_type)
        REM_REF(a->value->m_type);
      REM_REF(a->value);
    }
  }
  if(a->array)
    free_array_sub(a->array);
  free(a);
}

Var_Decl_List new_var_decl_list(Var_Decl decl, Var_Decl_List list, int pos) {
  Var_Decl_List a = calloc(1, sizeof(struct Var_Decl_List_));
  a->self = decl;
  a->next = list;
  a->pos = pos;
  a->doc = NULL;
  return a;
}

static void free_var_decl_list(Var_Decl_List a) {
  Var_Decl_List tmp, list = a;
  while(list) {
    free_var_decl(list->self);
    tmp = list;
    list = list->next;
    free(tmp);
  }
}

Type_Decl* new_type_decl(ID_List xid, int ref, int pos) {
  Type_Decl* a = calloc(1, sizeof(Type_Decl));
  a->xid = xid;
  a->ref = ref;
  a->pos = pos;
  a->array = NULL;
  a->dot = NULL;
  a->doc = NULL;
  return a;
}

Array_Sub new_array_sub(Expression exp, int pos) {
  Array_Sub a = calloc(1, sizeof(struct Array_Sub_));
  a->exp_list = exp;
  a->depth = 1;
  a->err_num = 0;
  a->pos = pos;
  return a;
}

Array_Sub prepend_array_sub(Array_Sub a, Expression exp, int pos) {
  if(a->err_num)
    goto error;
  if(exp && exp->next) {
    a->err_num = 1;
    a->err_pos = exp->pos;
    goto error;
  }
  if((exp && !a->exp_list) || (!exp && a->exp_list)) {
    a->err_num = 2;
    a->err_pos = pos;
    goto error;
  }

  if(exp) {
    exp->next = a->exp_list;
    a->exp_list = exp;
  }
  a->depth++;
  return a;

error:
  free_expression(exp);
  return a;
}

Type_Decl* add_type_decl_array(Type_Decl* a, Array_Sub array, int pos) {
  a->array = array;
  return a;
}

Expression new_array(Expression base, Array_Sub indices, int pos) {
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Array_Expression_type;
  a->meta = ae_meta_var;
  a->d.exp_array.base = base;
  a->d.exp_array.indices = indices;
  a->pos = pos;
  a->d.exp_array.pos = pos;
  a->d.exp_array.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

static void free_array_expression(Array* a) {
  free_array_sub(a->indices);
  free_expression(a->base);
}

ID_List new_id_list(const m_str xid, int pos) {
  ID_List a = calloc(1,  sizeof(struct ID_List_));
  a->xid = insert_symbol(xid);
  a->next = NULL;
  a->pos = pos;
  return a;
}

ID_List prepend_id_list(const m_str xid, ID_List list, int pos) {

  ID_List a = new_id_list(xid, pos);
  a->next = list;
  a->pos = pos;
  return a;
}

void free_id_list(ID_List a) {
#ifdef DEBUG_AST
  debug_msg("emit", "free_ID_LIST");
#endif
  ID_List tmp, curr = a;
  while(curr) {
    tmp = curr;
    curr = curr->next;
    free(tmp);
  }
}

void free_type_decl(Type_Decl* a) {
  if(a->array) {
    free(a->array);
  }
  free_id_list(a->xid);
  if(a->doc)
    free(a->doc);
  free(a);
}

Expression new_decl_expression(Type_Decl* type, Var_Decl_List list, m_bool is_static, int pos) {
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Decl_Expression_type;
  a->d.exp_decl.type = type;
  a->d.exp_decl.num_decl = 0;
  a->d.exp_decl.list = list;
  a->d.exp_decl.is_static = is_static;
  a->pos  = pos;
  a->d.exp_decl.pos  = pos;
  a->d.exp_decl.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

static void free_decl_expression(Decl_Expression* a) {
  free_type_decl(a->type);
  free_var_decl_list(a->list);
}

Expression new_binary_expression(Expression lhs, Operator op, Expression rhs, int pos) {
  Expression a = calloc(1,  sizeof(struct Expression_));
  a->exp_type = Binary_Expression_type;
  a->meta = ae_meta_value;
  a->d.exp_binary.lhs = lhs;
  a->d.exp_binary.op = op;
  a->d.exp_binary.rhs = rhs;
  a->pos = pos;
  a->d.exp_binary.pos = pos;
  a->d.exp_binary.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

static void free_binary_expression(Binary_Expression* binary) {
  free_expression(binary->lhs);
  free_expression(binary->rhs);
}

Expression new_cast_expression(Type_Decl* type, Expression exp, int pos) {
  Expression a = calloc(1,  sizeof(struct Expression_));
  a->exp_type = Cast_Expression_type;
  a->meta = ae_meta_value;
  a->d.exp_cast.type = type;
  a->pos = pos;
  a->d.exp_cast.pos = pos;
  a->d.exp_cast.exp = exp;
  a->d.exp_cast.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

__inline static void free_cast_expression(Cast_Expression* a) {
  free_type_decl(a->type);
  free_expression(a->exp);
}

Expression new_postfix_expression(Expression exp, Operator op, int pos) {
  Expression a = calloc(1,  sizeof(struct Expression_));
  a->exp_type = Postfix_Expression_type;
  a->meta = ae_meta_var;
  a->d.exp_postfix.exp = exp;
  a->d.exp_postfix.op = op;
  a->pos = pos;
  a->d.exp_postfix.pos = pos;
  a->d.exp_postfix.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

static void free_postfix_expression(Postfix_Expression* postfix) {
  free_expression(postfix->exp);
}

Expression new_expr_dur(Expression base, Expression unit, int pos) {
  Expression a = calloc(1,  sizeof(struct Expression_));
  a->exp_type = Dur_Expression_type;
  a->meta = ae_meta_value;
  a->d.exp_dur.base = base;
  a->d.exp_dur.unit = unit;
  a->pos = pos;
  a->d.exp_dur.pos = pos;
  a->d.exp_dur.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

static void free_dur_expression(Exp_Dur* a) {
  free_expression(a->base);
  free_expression(a->unit);
}

Expression new_expr_prim_int(long i, int pos) {
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->emit_var = 0;
  a->d.exp_primary.type = ae_primary_num;
  a->d.exp_primary.d.num = i;
  a->pos = pos;
  a->d.exp_primary.pos = pos;
  a->d.exp_primary.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Expression new_expr_prim_float(m_float num, int pos) {
  Expression a = calloc(1,  sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->emit_var = 0; // ???
  a->d.exp_primary.type = ae_primary_float;
  a->d.exp_primary.d.fnum = num;
  a->pos = pos;
  a->d.exp_primary.pos = pos;
  a->d.exp_primary.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Expression new_expr_prim_string(m_str s, int pos) {
  Expression a = calloc(1,  sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->emit_var = 0; // ???
  a->d.exp_primary.type = ae_primary_str;
  a->d.exp_primary.d.str = s;
  a->pos = pos;
  a->d.exp_primary.pos = pos;
  a->d.exp_primary.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Expression new_expr_prim_nil(int pos) {
  Expression  a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->d.exp_primary.type = ae_primary_nil;
  a->d.exp_primary.pos = pos;
  a->pos = pos;
  a->d.exp_primary.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Expression new_expr_prim_ID(m_str s, int pos) {
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_var;
  a->emit_var = 0;
  a->d.exp_primary.type = ae_primary_var;
  a->d.exp_primary.d.var = insert_symbol(s);
  a->pos = pos;
  a->d.exp_primary.pos = pos;
  a->d.exp_primary.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Expression new_hack_expression(Expression exp, int pos) {
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->d.exp_primary.type = ae_primary_hack;
  a->d.exp_primary.d.exp = exp;
  a->d.exp_primary.pos = pos;
  a->pos = pos;
  a->d.exp_primary.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Complex* new_complex(Expression re, int pos) {
  Complex* a = calloc(1, sizeof(Complex));
  a->re = re;
  if(re)
    a->im = re->next;
  a->pos = pos;
  return a;
}

__inline static void free_complex(Complex* a) {
  free_expression(a->re);
  free(a);

}

Expression new_expr_prim_char(m_str chr, int pos) {
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->d.exp_primary.type = ae_primary_char;
  a->d.exp_primary.d.chr = chr;
  a->pos = pos;
  a->d.exp_primary.pos = pos;
  a->d.exp_primary.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Expression new_expr_array_lit(Array_Sub exp_list, int pos) {
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->d.exp_primary.type = ae_primary_array;
  a->d.exp_primary.d.array = exp_list;
  a->pos = pos;
  a->d.exp_primary.pos = pos;
  a->d.exp_primary.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Expression new_expr_complex(Complex* exp, int pos) {
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->d.exp_primary.type = ae_primary_complex;
  a->d.exp_primary.d.cmp = exp;
  a->d.exp_primary.pos = pos;
  a->pos = pos;
  a->d.exp_primary.d.cmp->self = a;
  a->d.exp_primary.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Expression new_expr_polar(Polar* exp, int pos) {
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->d.exp_primary.type = ae_primary_polar;
  a->d.exp_primary.d.polar = exp;
  a->d.exp_primary.pos = pos;
  a->pos = pos;
  a->d.exp_primary.d.polar->self = a;
  a->d.exp_primary.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Polar* new_polar(Expression mod, int pos) {
  Polar* a = calloc(1, sizeof(Polar));
  a->mod = mod;
  if(mod)
    a->phase = mod->next;
  a->pos = pos;
  return a;
}

__inline static void free_polar(Polar* a) {
  free_expression(a->mod);
  free(a);
}

Vec new_vec(Expression e, int pos) {
  Vec a = calloc(1, sizeof(struct Vec_));
  a->args = e;
  while(e) {
    a->numdims++;
    e = e->next;
  }
  a->pos = pos;
  return a;
}

__inline static void free_vec(Vec a) {
  free_expression(a->args);
  free(a);
}

Expression new_expr_prim_vec(Vec exp, int pos) {
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->d.exp_primary.type = ae_primary_vec;
  a->d.exp_primary.d.vec = exp;
  a->d.exp_primary.pos = pos;
  a->pos = pos;
  a->d.exp_primary.d.vec->self = a;
  a->d.exp_primary.self = a;
  return a;
}

Expression new_expr_unary(Operator oper, Expression exp, int pos) {
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Unary_Expression_type;
  a->meta = exp->meta;
  a->d.exp_unary.op = oper;
  a->d.exp_unary.exp = exp;
  a->d.exp_unary.code = NULL;
  a->pos = pos;
  a->d.exp_unary.pos = pos;
  a->d.exp_unary.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Expression new_expr_unary2(Operator oper, Type_Decl* type, Array_Sub array, int pos) {
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Unary_Expression_type;
  a->meta = ae_meta_value;
  a->d.exp_unary.exp = NULL;
  a->d.exp_unary.op = oper;
  a->d.exp_unary.type = type;
  a->d.exp_unary.array = array;
  a->d.exp_unary.code = NULL;
  a->pos = pos;
  a->d.exp_unary.pos = pos;
  a->d.exp_unary.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Expression new_expr_unary3(Operator oper, Stmt code, int pos) {
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Unary_Expression_type;
  a->meta = ae_meta_value;
  a->d.exp_unary.exp = NULL;
  a->d.exp_unary.op = oper;
  ID_List id = new_id_list("void", pos);
  a->d.exp_unary.type = new_type_decl(id, 0, pos);
  a->d.exp_unary.array = NULL;
  a->d.exp_unary.code = code;
  a->pos = pos;
  a->d.exp_unary.pos = pos;
  a->d.exp_unary.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

static void free_unary_expression(Unary_Expression* a) {
  if(a->exp) // sporked func
    free_expression(a->exp);
  if(a->type)
    free_type_decl(a->type);
  if(a->code)
    free_stmt(a->code);
}

Expression new_if_expression(Expression cond, Expression if_exp, Expression else_exp, int pos) {
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = If_Expression_type;
  a->meta = ((if_exp->meta == ae_meta_var &&
              else_exp->meta == ae_meta_var) ? ae_meta_var : ae_meta_value);
  a->d.exp_if.cond = cond;
  a->d.exp_if.if_exp = if_exp;
  a->d.exp_if.else_exp = else_exp;
  a->pos = pos;
  a->d.exp_if.pos = pos;
  a->d.exp_if.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

static void free_if_expression(If_Expression* a) {
  free_expression(a->cond);
  free_expression(a->if_exp);
  free_expression(a->else_exp);
}

Func_Def new_func_def(ae_Keyword func_decl, ae_Keyword static_decl, Type_Decl* type_decl, m_str name, Arg_List arg_list, Stmt code, int pos) {
  Func_Def a = calloc(1, sizeof(struct Func_Def_));
  a->func_decl = func_decl;
  a->static_decl = static_decl;
  a->type_decl = type_decl;
  a->name = insert_symbol(name);
  a->arg_list = arg_list;
  a->s_type = ae_func_user;
  a->code = code;
  a->func = NULL;
  a->stack_depth = 0;
  a->pos = pos;
  a->spec = ae_func_spec_none;
  if(a->func_decl == ae_key_variadic) {
    a->func_decl = ae_key_func;
    a->is_variadic = 1;
  }
  return a;
}

void free_func_def(Func_Def a) {
  if(!a->is_template) {
    if(a->types)
      free_id_list(a->types);
    if(a->ret_type && a->ret_type->array_type)
      REM_REF(a->ret_type);
    if(a->arg_list)
      free_arg_list(a->arg_list);
    free_type_decl(a->type_decl);
  }
  free(a);
}

Stmt new_func_ptr_stmt(ae_Keyword key, m_str xid, Type_Decl* decl, Arg_List args, int pos) {
  Stmt a                  = calloc(1, sizeof(struct Stmt_));
  a->type                 = ae_stmt_funcptr;
  a->d.stmt_ptr.key   = key;
  a->d.stmt_ptr.type  = decl;
  a->d.stmt_ptr.xid   = insert_symbol(xid);
  a->d.stmt_ptr.args  = args;
  a->d.stmt_ptr.func  = NULL;
  a->d.stmt_ptr.value = NULL;
  a->d.stmt_ptr.pos   = pos;
  a->pos             = pos;
  return a;

}

static void free_stmt_func_ptr(Stmt_Ptr a) {
//  if(a->args) // commented 13/04/17 for typedef int[]
//    free_arg_list(a->args);
  if(a->func)
    REM_REF(a->func)
    else
      free_type_decl(a->type);
  if(a->value && !GET_FLAG(a->value, ae_value_member) && !a->key) {
    REM_REF(a->value->m_type);
    REM_REF(a->value);
  }
}

Expression new_expr_call(Expression base, Expression args, int pos) {
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Func_Call_type;
  a->meta = ae_meta_value;
  a->d.exp_func.func = base;
  a->d.exp_func.args = args;
  a->d.exp_func.types = NULL;
  a->pos = pos;
  a->d.exp_func.m_func = NULL;
  a->d.exp_func.pos = pos;
  a->d.exp_func.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

static void free_func_call(Func_Call* a) {
  if(a->types)
    free_type_list(a->types);
//  if(a->base)
//    free_type_list(a->base);
  free_expression(a->func);
  if(a->args)
    free_expression(a->args);
}

Expression new_expr_dot(Expression base, m_str xid, int pos) {
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Dot_Member_type;
  a->meta = ae_meta_var;
  a->d.exp_dot.base = base;
  a->d.exp_dot.xid = insert_symbol(xid);
  a->pos = pos;
  a->d.exp_dot.pos = pos;
  a->d.exp_dot.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

static void free_dot_member_expression(Dot_Member* dot) {
  if(dot->base)
    free_expression(dot->base);
}

Expression prepend_expression(Expression exp, Expression next, int pos) {
  exp->next = next;
  return exp;
}

static void free_primary_expression(Primary_Expression* a) {
  if(a->type == ae_primary_hack)
    free_expression(a->d.exp);
  else if(a->type == ae_primary_array)
    free_array_sub(a->d.array);
  else if(a->type == ae_primary_complex)
    free_complex(a->d.cmp);
  else if(a->type == ae_primary_polar)
    free_polar(a->d.polar);
  else if(a->type == ae_primary_vec)
    free_vec(a->d.vec);
}

void free_expression(Expression exp) {
  Expression tmp, curr = exp;
  while(curr) {
    switch(curr->exp_type) {
    case Decl_Expression_type:
      free_decl_expression(&curr->d.exp_decl);
      break;
    case Binary_Expression_type:
      free_binary_expression(&curr->d.exp_binary);
      break;
    case Unary_Expression_type:
      free_unary_expression(&curr->d.exp_unary);
      break;
    case Primary_Expression_type:
      free_primary_expression(&curr->d.exp_primary);
      break;
    case Cast_Expression_type:
      free_cast_expression(&curr->d.exp_cast);
      break;
    case Postfix_Expression_type:
      free_postfix_expression(&curr->d.exp_postfix);
      break;
    case Func_Call_type:
      free_func_call(&curr->d.exp_func);
      break;
    case Array_Expression_type:
      free_array_expression(&curr->d.exp_array);
      break;
    case If_Expression_type:
      free_if_expression(&curr->d.exp_if);
      break;
    case Dot_Member_type:
      free_dot_member_expression(&curr->d.exp_dot);
      break;
    case Dur_Expression_type:
      free_dur_expression(&curr->d.exp_dur);
      break;
    }
    tmp = curr;
    curr = curr->next;
    free(tmp);
  }
}

Arg_List new_arg_list(Type_Decl* type_decl, Var_Decl var_decl, Arg_List arg_list, int pos) {
  Arg_List a = calloc(1, sizeof(struct Arg_List_));
  a->type_decl = type_decl;
  a->var_decl = var_decl;
  a->next = arg_list;
  a->doc = NULL;
  a->pos = pos;
  return a;
}

void free_arg_list(Arg_List a) {
  if(a->next)
    free_arg_list(a->next);
  free_type_decl(a->type_decl);
  free_var_decl(a->var_decl);
  free(a);

}

Stmt new_stmt_expression(Expression exp, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->type = ae_stmt_exp;
  a->d.stmt_exp.val = exp;
  a->pos = pos;
  a->d.stmt_exp.pos = pos;
  a->d.stmt_exp.self = a;
  return a;
}

Stmt new_stmt_code(Stmt_List stmt_list, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->type = ae_stmt_code;
  a->d.stmt_code.stmt_list = stmt_list;
  a->pos = pos;
  a->d.stmt_code.pos = pos;
  a->d.stmt_code.self = a;
  return a;
}

static void free_stmt_code(Stmt_Code a) {
  free_stmt_list(a->stmt_list);
}

Stmt new_stmt_return(Expression exp, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->type = ae_stmt_return;
  a->d.stmt_return.val = exp;
  a->pos = pos;
  a->d.stmt_return.pos = pos;
  a->d.stmt_return.self = a;
  return a;
}

__inline static void free_stmt_exp(struct Stmt_Exp_* a) {
  free_expression(a->val);
}

Stmt new_stmt_break(int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->type = ae_stmt_break;
  a->pos = pos;
  a->d.stmt_break.pos = pos;
  a->d.stmt_break.self = a;
  return a;
}

Stmt new_stmt_continue(int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->type = ae_stmt_continue;
  a->pos = pos;
  a->d.stmt_continue.pos = pos;
  a->d.stmt_continue.self = a;
  return a;
}

Stmt new_stmt_while(Expression cond, Stmt body, m_bool is_do, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->type = ae_stmt_while;
  a->d.stmt_while.is_do = is_do;
  a->d.stmt_while.cond = cond;
  a->d.stmt_while.body = body;
  a->pos = pos;
  a->d.stmt_while.pos = pos;
  a->d.stmt_while.self = a;
  return a;
}

static void free_stmt_flow(struct Stmt_Flow_* a) {
  free_expression(a->cond);
  free_stmt(a->body);
}

Stmt new_stmt_until(Expression cond, Stmt body, m_bool is_do, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->type = ae_stmt_until;
  a->d.stmt_until.is_do = is_do;
  a->d.stmt_until.cond = cond;
  a->d.stmt_until.body = body;
  a->pos = pos;
  a->d.stmt_until.pos = pos;
  a->d.stmt_until.self = a;
  return a;
}

Stmt new_stmt_for(Stmt c1, Stmt c2, Expression c3, Stmt body, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->type = ae_stmt_for;
  a->d.stmt_for.c1 = c1;
  a->d.stmt_for.c2 = c2;
  a->d.stmt_for.c3 = c3;
  a->d.stmt_for.body = body;
  a->pos = pos;
  a->d.stmt_for.pos = pos;
  a->d.stmt_for.self = a;
  return a;
}

static void free_stmt_for(Stmt_For a) {
  free_stmt(a->c1);
  free_stmt(a->c2);
  free_expression(a->c3);
  free_stmt(a->body);
}

Stmt new_stmt_gotolabel(m_str xid, m_bool is_label, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->type = ae_stmt_gotolabel;
  a->d.stmt_gotolabel.name = insert_symbol(xid);
  a->d.stmt_gotolabel.is_label = is_label;
  a->pos = pos;
  a->d.stmt_gotolabel.pos = pos;
  a->d.stmt_gotolabel.self = a;
  return a;
}

Stmt new_stmt_loop(Expression cond, Stmt body, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->type = ae_stmt_loop;
  a->d.stmt_loop.cond = cond;
  a->d.stmt_loop.body = body;
  a->pos = pos;
  a->d.stmt_loop.pos = pos;
  a->d.stmt_loop.self = a;
  return a;
}

static void free_stmt_loop(Stmt_Loop a) {
  free_expression(a->cond);
  free_stmt(a->body);
}

Stmt new_stmt_if(Expression cond, Stmt if_body, Stmt else_body, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->type = ae_stmt_if;
  a->d.stmt_if.cond = cond;
  a->d.stmt_if.if_body = if_body;
  a->d.stmt_if.else_body = else_body;
  a->pos = pos;
  a->d.stmt_if.pos = pos;
  a->d.stmt_if.self = a;
  return a;
}

static void free_stmt_if(Stmt_If a) {
  free_expression(a->cond);
  free_stmt(a->if_body);
  if(a->else_body)
    free_stmt(a->else_body);
}

Stmt new_stmt_switch(Expression val, Stmt stmt, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->type = ae_stmt_switch;
  a->d.stmt_switch.val = val;
  a->d.stmt_switch.stmt = stmt;
  a->pos = pos;
  a->d.stmt_switch.pos = pos;
  a->d.stmt_switch.self = a;
  return a;
}

__inline static void free_stmt_switch(Stmt_Switch a) {
  free_expression(a->val);
  free_stmt(a->stmt);
}

Stmt new_stmt_case(Expression val, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->type = ae_stmt_case;
  a->d.stmt_case.val = val;
  a->pos = pos;
  a->d.stmt_case.pos = pos;
  a->d.stmt_case.self = a;
  return a;
}

Stmt new_stmt_enum(ID_List list, m_str xid, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->type = ae_stmt_enum;
  a->d.stmt_enum.xid = xid ? insert_symbol(xid) : NULL;
  a->d.stmt_enum.list = list;
  a->d.stmt_enum.values = new_vector();
  a->pos = pos;
  a->d.stmt_enum.pos = pos;
  a->d.stmt_enum.self = a;
  return a;
}

static void free_stmt_enum(Stmt_Enum a) {
  vtype i;
  if(a->list)
    free_id_list(a->list);
  for(i = 0; i < vector_size(a->values); i++) {
    Value v = (Value)vector_at(a->values, i);
    if(!v->owner_class) {
      free(v);
    }
//else    REM_REF(v);
  }
  free_vector(a->values);
}

Stmt new_stmt_union(Decl_List l, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->type = ae_stmt_union;
  a->d.stmt_union.l = l;
  a->d.stmt_union.v = new_vector();
  a->pos = pos;
  a->d.stmt_union.pos = pos;
  a->d.stmt_union.self = a;
  return a;
}

Decl_List new_decl_list(Decl_Expression* d, Decl_List l) {
  Decl_List a = calloc(1, sizeof(struct Decl_List_));
  a->self = d;
  a->next = l;
  return a;
}

static void free_decl_list(Decl_List a) {
  if(a->next)
    free_decl_list(a->next);
  if(a->self)
    free_decl_expression(a->self);
  free(a);
}

__inline static void free_stmt_union(Stmt_Union a) {
  free_decl_list(a->l);
  free_vector(a->v);
}

static void free_stmt(Stmt stmt) {
  switch(stmt->type) {
  case ae_stmt_exp:
    free_stmt_exp(&stmt->d.stmt_exp);
    break;
  case ae_stmt_while:
    free_stmt_flow(&stmt->d.stmt_while);
    break;
  case ae_stmt_until:
    free_stmt_flow(&stmt->d.stmt_until);
    break;
  case ae_stmt_for:
    free_stmt_for(&stmt->d.stmt_for);
    break;
  case ae_stmt_loop:
    free_stmt_loop(&stmt->d.stmt_loop);
    break;
  case ae_stmt_if:
    free_stmt_if(&stmt->d.stmt_if);
    break;
  case ae_stmt_code:
    free_stmt_code(&stmt->d.stmt_code);
    break;
  case ae_stmt_switch:
    free_stmt_switch(&stmt->d.stmt_switch);
    break;
  case ae_stmt_break:
  case ae_stmt_continue:
    break;
  case ae_stmt_return:
    free_stmt_exp(&stmt->d.stmt_return);
    break;
  case ae_stmt_case:
    free_stmt_exp(&stmt->d.stmt_case);
    break;
  case ae_stmt_gotolabel:
    break;
  case ae_stmt_enum:
    free_stmt_enum(&stmt->d.stmt_enum);
    break;
  case ae_stmt_funcptr:
    free_stmt_func_ptr(&stmt->d.stmt_ptr);
    break;
  case ae_stmt_union:
    free_stmt_union(&stmt->d.stmt_union);
    break;
  }
  free(stmt);
}

Stmt_List new_stmt_list(Stmt stmt, Stmt_List next, int pos) {
  Stmt_List list = calloc(1, sizeof(struct Stmt_List_));
  list->stmt = stmt;
  list->next = next;
  return list;
}

static void free_stmt_list(Stmt_List list) {
  Stmt_List tmp;
  while(list) {
    tmp = list;
    list = list->next;
    free_stmt(tmp->stmt);
    free(tmp);
  }
}

Section* new_section_stmt_list(Stmt_List list, int pos) {
  Section* a = calloc(1, sizeof(Section));
  a->type = ae_section_stmt;
  a->d.stmt_list = list;
  a->pos = pos;
  return a;
}

Section* new_section_func_def(Func_Def func_def, int pos) {
  Section* a = calloc(1, sizeof(Section));
  a->type = ae_section_func;
  a->d.func_def = func_def;
  a->pos = pos;
  return a;
}

static void free_class_def(Class_Def a) {
  if(a->ext) {
    free_id_list(a->ext->extend_id);
    if(a->ext->impl_list)
      free_id_list(a->ext->impl_list);
    free(a->ext);
  }
  Class_Body tmp, b = a->body;
  while(b) {
    tmp = b;
    if(b->section)
      free_section(b->section);
    b = b->next;
    free(tmp);
  }
  free_id_list(a->name);
  free(a);
}

static void free_section(Section* section) {
  switch(section->type) {
  case ae_section_class:
    free_class_def(section->d.class_def);
    break;
  case ae_section_stmt:
    free_stmt_list(section->d.stmt_list);
    break;
  case ae_section_func:
    if(!section->d.func_def)
      break;
    free_stmt(section->d.func_def->code);
    if(section->d.func_def->types) {
      if(section->d.func_def->func && section->d.func_def->func->value_ref->owner_class) {
        scope_rem(section->d.func_def->func->value_ref->owner_class->info->value,
                  insert_symbol(section->d.func_def->func->value_ref->name));
        section->d.func_def->func->value_ref->func_ref = NULL;
        free(section->d.func_def->func);
        free_func_def(section->d.func_def);
        break;
      }
      if(section->d.func_def->arg_list)
        free_arg_list(section->d.func_def->arg_list);
      free_id_list(section->d.func_def->types);
      free_type_decl(section->d.func_def->type_decl);
      if(section->d.func_def->func) {
        REM_REF(section->d.func_def->func->value_ref);
        free(section->d.func_def->func);
      }
      free(section->d.func_def);
    } else {
      if(!section->d.func_def->func)
        free_func_def(section->d.func_def);
    }
    break;
  }
  free(section);
}

Class_Def new_class_def(ae_Keyword class_decl, ID_List name, Class_Ext ext, Class_Body body, int pos) {
  Class_Def a = calloc(1, sizeof(struct Class_Def_));
  a->decl = class_decl;
  a->name = name;
  a->ext  = ext;
  a->body = body;
  a->pos  = pos;
  a->doc  = NULL;
  a->type = NULL;
  return a;
}

Class_Body new_class_body(Section* section, int pos) {
  Class_Body a = calloc(1, sizeof(struct Class_Body_));
  a->section = section;
  a->pos = pos;
  a->next = NULL;
  return a;
}

Class_Body prepend_class_body(Section* section, Class_Body body, int pos) {
  Class_Body a = new_class_body(section, pos);
  a->next = body;
  a->pos = pos;
  return a;
}

Class_Ext new_class_ext(ID_List extend_id, ID_List impl_list, int pos) {
  Class_Ext a = calloc(1, sizeof(struct Class_Ext_));
  a->extend_id = extend_id;
  a->impl_list = impl_list;
  a->pos = pos;
  return a;
}

Type_List new_type_list(ID_List list, Type_List next, int pos) {
  Type_List a = calloc(1,  sizeof(struct Type_List_));
  a->list = list;
  a->next = next;
  a->pos = pos;
  return a;
}

void free_type_list(Type_List a) {
  if(a->next)
    free_type_list(a->next);
  free_id_list(a->list);
  free(a);
}

Section* new_section_class_def(Class_Def class_def, int pos) {
#ifdef DEBUG_AST
  debug_msg("emit", "class def");
#endif
  Section* a = calloc(1, sizeof(Section));
  a->type = ae_section_class;
  a->d.class_def = class_def;
  a->pos = pos;
  return a;
}

Ast new_ast(Section* section, Ast next, int pos) {
#ifdef DEBUG_AST
  debug_msg("emit", "new Ast");
#endif
  Ast ast = malloc(sizeof(struct Ast_));
  ast->section = section;
  ast->next = next;
  ast->pos = pos;
  ast->doc = NULL;
  return ast;
}

void free_ast(Ast prog) {
#ifdef DEBUG_AST
  debug_msg("ast", "free AST");
#endif
  Ast tmp, ast = prog;
  while(ast) {
    tmp = ast;
    ast = ast->next;
    if(tmp->doc)
      free(tmp->doc);
    free_section(tmp->section);
    free(tmp);
  }
}
