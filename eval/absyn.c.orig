#include <stdlib.h>
#include "absyn.h"
#include "oo.h"
#include "func.h"
#include "type.h"

void free_expression(Exp exp);
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
    if(GET_FLAG(a->value, ae_flag_arg)) { // func argument. this migth change
      if(a->value->m_type->array_depth)
        REM_REF(a->value->m_type);
      REM_REF(a->value);
    } else if(!a->value->owner_class) { // breaks for loop ?
if(!GET_FLAG(a->value, ae_flag_global)) {
//      REM_REF(a->value->m_type);
      REM_REF(a->value);
}else      if(!GET_FLAG(a->value, 
ae_flag_builtin) && a->value->m_type->array_depth) {
}
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
  return a;
}

Array_Sub new_array_sub(Exp exp, int pos) {
  Array_Sub a = calloc(1, sizeof(struct Array_Sub_));
  a->exp_list = exp;
  a->depth = 1;
  a->err_num = 0;
  a->pos = pos;
  return a;
}

Array_Sub prepend_array_sub(Array_Sub a, Exp exp, int pos) {
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

Exp new_array(Exp base, Array_Sub indices, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_array;
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

static void free_array_expression(Exp_Array* a) {
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
  free(a);
}

Exp new_exp_decl(Type_Decl* type, Var_Decl_List list, m_bool is_static, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_decl;
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

static void free_exp_decl(Exp_Decl* a) {
  free_type_decl(a->type);
  free_var_decl_list(a->list);
}

Exp new_exp_binary(Exp lhs, Operator op, Exp rhs, int pos) {
  Exp a = calloc(1,  sizeof(struct Exp_));
  a->exp_type = ae_exp_binary;
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

static void free_exp_binary(Exp_Binary* binary) {
  free_expression(binary->lhs);
  free_expression(binary->rhs);
}

Exp new_exp_cast(Type_Decl* type, Exp exp, int pos) {
  Exp a = calloc(1,  sizeof(struct Exp_));
  a->exp_type = ae_exp_cast;
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

__inline static void free_exp_cast(Exp_Cast* a) {
  free_type_decl(a->type);
  free_expression(a->exp);
}

Exp new_exp_postfix(Exp exp, Operator op, int pos) {
  Exp a = calloc(1,  sizeof(struct Exp_));
  a->exp_type = ae_exp_postfix;
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

static void free_exp_postfix(Exp_Postfix* postfix) {
  free_expression(postfix->exp);
}

Exp new_exp_dur(Exp base, Exp unit, int pos) {
  Exp a = calloc(1,  sizeof(struct Exp_));
  a->exp_type = ae_exp_dur;
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

Exp new_exp_prim_int(long i, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_primary;
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

Exp new_exp_prim_float(m_float num, int pos) {
  Exp a = calloc(1,  sizeof(struct Exp_));
  a->exp_type = ae_exp_primary;
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

Exp new_exp_prim_string(m_str s, int pos) {
  Exp a = calloc(1,  sizeof(struct Exp_));
  a->exp_type = ae_exp_primary;
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

Exp new_exp_prim_nil(int pos) {
  Exp  a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_primary;
  a->meta = ae_meta_value;
  a->d.exp_primary.type = ae_primary_nil;
  a->d.exp_primary.pos = pos;
  a->pos = pos;
  a->d.exp_primary.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Exp new_exp_prim_ID(m_str s, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_primary;
  a->meta = ae_meta_var;
  a->emit_var = 0;
  a->d.exp_primary.type = ae_primary_id;
  a->d.exp_primary.d.var = insert_symbol(s);
  a->pos = pos;
  a->d.exp_primary.pos = pos;
  a->d.exp_primary.self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Exp new_exp_prim_hack(Exp exp, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_primary;
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

Complex* new_complex(Exp re, int pos) {
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

Exp new_exp_prim_char(m_str chr, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_primary;
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

Exp new_exp_prim_array(Array_Sub exp_list, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_primary;
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

Exp new_exp_prim_complex(Complex* exp, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_primary;
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

Exp new_exp_prim_polar(Polar* exp, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_primary;
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

Polar* new_polar(Exp mod, int pos) {
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

Vec new_vec(Exp e, int pos) {
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

Exp new_exp_prim_vec(Vec exp, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_primary;
  a->meta = ae_meta_value;
  a->d.exp_primary.type = ae_primary_vec;
  a->d.exp_primary.d.vec = exp;
  a->d.exp_primary.pos = pos;
  a->pos = pos;
  a->d.exp_primary.d.vec->self = a;
  a->d.exp_primary.self = a;
  return a;
}

Exp new_exp_unary(Operator oper, Exp exp, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_unary;
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

Exp new_exp_unary2(Operator oper, Type_Decl* type, Array_Sub array, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_unary;
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

Exp new_exp_unary3(Operator oper, Stmt code, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_unary;
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

static void free_unary_expression(Exp_Unary* a) {
  if(a->exp) // sporked func
    free_expression(a->exp);
  if(a->type)
    free_type_decl(a->type);
  if(a->code)
    free_stmt(a->code);
}

Exp new_exp_if(Exp cond, Exp if_exp, Exp else_exp, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_if;
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

static void free_if_expression(Exp_If* a) {
  free_expression(a->cond);
  free_expression(a->if_exp);
  free_expression(a->else_exp);
}

Func_Def new_func_def(ae_flag flag, Type_Decl* type_decl, m_str name, Arg_List arg_list, Stmt code, int pos) {
  Func_Def a = calloc(1, sizeof(struct Func_Def_));
  a->flag = flag;
  a->type_decl = type_decl;
  a->name = insert_symbol(name);
  a->arg_list = arg_list;
  a->code = code;
  a->stack_depth = 0;
  a->pos = pos;
  return a;
}

void free_func_def(Func_Def a) {
  if(!GET_FLAG(a, ae_flag_template)) {
    if(a->types)
      free_id_list(a->types);
    if(a->ret_type && a->ret_type->array_depth)
      REM_REF(a->ret_type);
    if(a->arg_list)
      free_arg_list(a->arg_list);
    free_type_decl(a->type_decl);
  }
  free(a);
}

Stmt new_func_ptr_stmt(ae_flag key, m_str xid, Type_Decl* decl, Arg_List args, int pos) {
  Stmt a              = calloc(1, sizeof(struct Stmt_));
  a->type             = ae_stmt_funcptr;
  a->d.stmt_ptr.flag  = key;
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
  if(a->func)
    REM_REF(a->func)
  else {
    if(a->args) // commented 13/04/17 for typedef int[]
      free_arg_list(a->args);
    free_type_decl(a->type);
  }
/*
  if(a->value && !GET_FLAG(a->value, ae_flag_member) && !GET_FLAG(a, ae_flag_static)) {
    REM_REF(a->value->m_type);
    REM_REF(a->value)
  }
*/
}

Exp new_exp_call(Exp base, Exp args, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_call;
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

static void free_exp_call(Exp_Func* a) {
  if(a->types)
    free_type_list(a->types);
//  if(a->base)
//    free_type_list(a->base);
  free_expression(a->func);
  if(a->args)
    free_expression(a->args);
}

Exp new_exp_dot(Exp base, m_str xid, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_dot;
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

static void free_dot_member_expression(Exp_Dot* dot) {
  if(dot->base)
    free_expression(dot->base);
}

Exp prepend_expression(Exp exp, Exp next, int pos) {
  exp->next = next;
  return exp;
}

static void free_exp_primary(Exp_Primary* a) {
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

void free_expression(Exp exp) {
  Exp tmp, curr = exp;
  while(curr) {
    switch(curr->exp_type) {
    case ae_exp_decl:
      free_exp_decl(&curr->d.exp_decl);
      break;
    case ae_exp_binary:
      free_exp_binary(&curr->d.exp_binary);
      break;
    case ae_exp_unary:
      free_unary_expression(&curr->d.exp_unary);
      break;
    case ae_exp_primary:
      free_exp_primary(&curr->d.exp_primary);
      break;
    case ae_exp_cast:
      free_exp_cast(&curr->d.exp_cast);
      break;
    case ae_exp_postfix:
      free_exp_postfix(&curr->d.exp_postfix);
      break;
    case ae_exp_call:
      free_exp_call(&curr->d.exp_func);
      break;
    case ae_exp_array:
      free_array_expression(&curr->d.exp_array);
      break;
    case ae_exp_if:
      free_if_expression(&curr->d.exp_if);
      break;
    case ae_exp_dot:
      free_dot_member_expression(&curr->d.exp_dot);
      break;
    case ae_exp_dur:
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

Stmt new_stmt_expression(Exp exp, int pos) {
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

Stmt new_stmt_return(Exp exp, int pos) {
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

Stmt new_stmt_while(Exp cond, Stmt body, m_bool is_do, int pos) {
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

Stmt new_stmt_until(Exp cond, Stmt body, m_bool is_do, int pos) {
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

Stmt new_stmt_for(Stmt c1, Stmt c2, Exp c3, Stmt body, int pos) {
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

Stmt new_stmt_loop(Exp cond, Stmt body, int pos) {
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

Stmt new_stmt_if(Exp cond, Stmt if_body, Stmt else_body, int pos) {
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

Stmt new_stmt_switch(Exp val, Stmt stmt, int pos) {
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

Stmt new_stmt_case(Exp val, int pos) {
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
  vector_init(&a->d.stmt_enum.values);
  a->pos = pos;
  a->d.stmt_enum.pos = pos;
  a->d.stmt_enum.self = a;
  return a;
}

static void free_stmt_enum(Stmt_Enum a) {
  free_id_list(a->list);
  vector_release(&a->values);
}

Stmt new_stmt_union(Decl_List l, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->type = ae_stmt_union;
  a->d.stmt_union.l = l;
  vector_init(&a->d.stmt_union.v);
  a->pos = pos;
  a->d.stmt_union.pos = pos;
  a->d.stmt_union.self = a;
  return a;
}

Decl_List new_decl_list(Exp d, Decl_List l) {
  Decl_List a = calloc(1, sizeof(struct Decl_List_));
  a->self = d;
  a->next = l;
  return a;
}

static void free_decl_list(Decl_List a) {
  if(!a)
    return;
  if(a->next)
    free_decl_list(a->next);
  if(a->self)
    free_expression(a->self);
  free(a);
}

__inline static void free_stmt_union(Stmt_Union a) {
  free_decl_list(a->l);
  vector_release(&a->v);
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
//if(section->d.func_def->types) {
//REM_REF(section->d.func_def->d.func->value_ref)
//REM_REF(section->d.func_def->d.func)

//}    else 
if(!section->d.func_def->d.func)
      free_func_def(section->d.func_def);
    break;
  }
  free(section);
}

Class_Def new_class_def(ae_flag class_decl, ID_List name, Class_Ext ext, Class_Body body, int pos) {
  Class_Def a = calloc(1, sizeof(struct Class_Def_));
  a->decl = class_decl;
  a->name = name;
  a->ext  = ext;
  a->body = body;
  a->pos  = pos;
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
    free_section(tmp->section);
    free(tmp);
  }
}
