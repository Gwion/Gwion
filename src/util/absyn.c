#include <stdlib.h>
#include "absyn.h"
#include "oo.h"
#include "func.h"
#include "type.h"

void free_exp(Exp exp);
void free_arg_list(Arg_List a);
void free_stmt(Stmt a);
static void free_stmt_list(Stmt_List a);
static void free_stmt_code(Stmt_Code a);
static void free_section();

Var_Decl new_var_decl(S_Symbol xid, Array_Sub array, int pos) {
  Var_Decl a = calloc(1, sizeof(struct Var_Decl_));
  a->xid = xid;
  a->array = array;
  a->pos = pos;
  return a;
}

void free_array_sub(Array_Sub a) {
  free_exp(a->exp_list);
  free(a);
}

static void free_var_decl(Var_Decl a) {
  if(a->value) {
    if(GET_FLAG(a->value, ae_flag_arg)) {
      if(a->value->m_type->array_depth)
        REM_REF(a->value->m_type);
      REM_REF(a->value);
    } else if(!a->value->owner_class && !GET_FLAG(a->value, ae_flag_global))
        REM_REF(a->value)
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
  if(a->next)
    free_var_decl_list(a->next);
  free_var_decl(a->self);
  free(a);
}

Type_Decl* new_type_decl(ID_List xid, int ref, int pos) {
  Type_Decl* a = calloc(1, sizeof(Type_Decl));
  a->xid = xid;
  if(ref)
    SET_FLAG(a, ae_flag_ref);
  a->pos = pos;
  return a;
}

Type_Decl* new_type_decl2(ID_List xid, int ref, int pos) {
  Type_Decl* a = calloc(1, sizeof(Type_Decl));
  a->xid = new_id_list(insert_symbol(""), pos);
  a->xid->ref = xid;
  if(ref)
    SET_FLAG(a, ae_flag_ref);
  a->pos = pos;
  return a;
}
Array_Sub new_array_sub(Exp exp, int pos) {
  Array_Sub a = calloc(1, sizeof(struct Array_Sub_));
  a->exp_list = exp;
  a->depth = 1;
  a->pos = pos;
  return a;
}

Array_Sub prepend_array_sub(Array_Sub a, Exp exp) {
  if(exp) {
    exp->next = a->exp_list;
    a->exp_list = exp;
  }
  a->depth++;
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
  a->pos = a->d.exp_array.pos = pos;
  a->d.exp_array.self = a;
  return a;
}

static void free_array_exp(Exp_Array* a) {
  if(a->base && a->base->type && a->indices->depth < a->base->type->array_depth)
    free(a->self->type);
  free_array_sub(a->indices);
  free_exp(a->base);
}

ID_List new_id_list(const S_Symbol xid, int pos) {
  ID_List a = calloc(1,  sizeof(struct ID_List_));
  a->xid = xid;
  a->pos = pos;
  return a;
}

ID_List prepend_id_list(const S_Symbol xid, ID_List list, int pos) {
  ID_List a = new_id_list(xid, pos);
  a->next = list;
  a->pos = pos;
  return a;
}

void free_id_list(ID_List a) {
  if(a->ref)
    free_id_list(a->ref);
  if(a->next)
    free_id_list(a->next);
  free(a);
}

void free_type_decl(Type_Decl* a) {
  if(a->types)
    free_type_list(a->types);
  if(a->array)
    free_array_sub(a->array);
  free_id_list(a->xid);
  free(a);
}

Exp new_exp_decl(Type_Decl* type, Var_Decl_List list, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_decl;
  a->d.exp_decl.type = type;
  a->d.exp_decl.list = list;
  a->pos  = a->d.exp_decl.pos  = pos;
  a->d.exp_decl.self = a;
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
  a->pos = a->d.exp_binary.pos = pos;
  a->d.exp_binary.self = a;
  return a;
}

static void free_exp_binary(Exp_Binary* binary) {
  free_exp(binary->lhs);
  free_exp(binary->rhs);
}

Exp new_exp_cast(Type_Decl* type, Exp exp, int pos) {
  Exp a = calloc(1,  sizeof(struct Exp_));
  a->exp_type = ae_exp_cast;
  a->meta = ae_meta_value;
  a->d.exp_cast.type = type;
  a->pos = a->d.exp_cast.pos = pos;
  a->d.exp_cast.exp = exp;
  a->d.exp_cast.self = a;
  return a;
}

static void free_exp_cast(Exp_Cast* a) {
  free_type_decl(a->type);
  if(a->t)
    REM_REF(a->t)
  free_exp(a->exp);
}

Exp new_exp_post(Exp exp, Operator op, int pos) {
  Exp a = calloc(1,  sizeof(struct Exp_));
  a->exp_type = ae_exp_post;
  a->meta = ae_meta_var;
  a->d.exp_post.exp = exp;
  a->d.exp_post.op = op;
  a->pos = a->d.exp_post.pos = pos;
  a->d.exp_post.self = a;
  return a;
}

static void free_exp_post(Exp_Postfix* post) {
  free_exp(post->exp);
}

Exp new_exp_dur(Exp base, Exp unit, int pos) {
  Exp a = calloc(1,  sizeof(struct Exp_));
  a->exp_type = ae_exp_dur;
  a->meta = ae_meta_value;
  a->d.exp_dur.base = base;
  a->d.exp_dur.unit = unit;
  a->pos = a->d.exp_dur.pos = pos;
  a->d.exp_dur.self = a;
  return a;
}

static void free_dur_exp(Exp_Dur* a) {
  free_exp(a->base);
  free_exp(a->unit);
}

static Exp new_exp_prim(int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_primary;
  a->meta = ae_meta_value;
  a->pos = a->d.exp_primary.pos = pos;
  a->d.exp_primary.self = a;
  return a;
}

Exp new_exp_prim_int(long i, int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.type = ae_primary_num;
  a->d.exp_primary.d.num = i;
  return a;
}

Exp new_exp_prim_float(m_float num, int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.type = ae_primary_float;
  a->d.exp_primary.d.fnum = num;
  return a;
}

Exp new_exp_prim_string(m_str s, int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.type = ae_primary_str;
  a->d.exp_primary.d.str = s;
  return a;
}

Exp new_exp_prim_nil(int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.type = ae_primary_nil;
  return a;
}

Exp new_exp_prim_id(S_Symbol xid, int pos) {
  Exp a = new_exp_prim(pos);
  a->meta = ae_meta_var;
  a->d.exp_primary.type = ae_primary_id;
  a->d.exp_primary.d.var = xid;
  return a;
}

Exp new_exp_prim_hack(Exp exp, int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.type = ae_primary_hack;
  a->d.exp_primary.d.exp = exp;
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
  free_exp(a->re);
  free(a);

}

Exp new_exp_prim_char(m_str chr, int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.type = ae_primary_char;
  a->d.exp_primary.d.chr = chr;
  return a;
}

Exp new_exp_prim_array(Array_Sub exp_list, int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.type = ae_primary_array;
  a->d.exp_primary.d.array = exp_list;
  return a;
}

Exp new_exp_prim_complex(Complex* exp, int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.type  = ae_primary_complex;
  a->d.exp_primary.d.cmp = exp;
  return a;
}

Exp new_exp_prim_polar(Polar* exp, int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.type = ae_primary_polar;
  a->d.exp_primary.d.polar = exp;
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
  free_exp(a->mod);
  free(a);
}

Vec* new_vec(Exp e, int pos) {
  Vec* a = calloc(1, sizeof(Vec));
  a->args = e;
  while(e) {
    a->numdims++;
    e = e->next;
  }
  a->pos = pos;
  return a;
}

__inline static void free_vec(Vec* a) {
  free_exp(a->args);
  free(a);
}

Exp new_exp_prim_vec(Vec* exp, int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.type = ae_primary_vec;
  a->d.exp_primary.d.vec = exp;
  a->d.exp_primary.self = a;
  return a;
}

static Exp new_exp_unary_base(int pos)  {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->meta = ae_meta_value;
  a->exp_type = ae_exp_unary;
  a->pos = a->d.exp_unary.pos = pos;
  a->d.exp_unary.self = a;
  return a;
}

Exp new_exp_unary(Operator oper, Exp exp, int pos) {
  Exp a = new_exp_unary_base(pos);
  a->meta = exp->meta;
  a->d.exp_unary.op = oper;
  a->d.exp_unary.exp = exp;
  return a;
}

Exp new_exp_unary2(Operator oper, Type_Decl* type, int pos) {
  Exp a = new_exp_unary_base(pos);
  a->d.exp_unary.op = oper;
  a->d.exp_unary.type = type;
  return a;
}

Exp new_exp_unary3(Operator oper, Stmt code, int pos) {
  Exp a = new_exp_unary_base(pos);
  a->d.exp_unary.op = oper;
  ID_List id = new_id_list(insert_symbol("void"), pos);
  a->d.exp_unary.type = new_type_decl(id, 0, pos);
  a->d.exp_unary.code = code;
  return a;
}

static void free_unary_exp(Exp_Unary* a) {
  if(a->exp)
    free_exp(a->exp);
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
  a->pos = a->d.exp_if.pos = pos;
  a->d.exp_if.self = a;
  return a;
}

static void free_if_exp(Exp_If* a) {
  free_exp(a->cond);
  free_exp(a->if_exp);
  free_exp(a->else_exp);
}

Func_Def new_func_def(ae_flag flag, Type_Decl* type_decl, S_Symbol xid, Arg_List arg_list, Stmt code, int pos) {
  Func_Def a = calloc(1, sizeof(struct Func_Def_));
  a->flag = flag;
  a->type_decl = type_decl;
  a->name = xid;
  a->arg_list = arg_list;
  a->code = code;
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

Stmt new_func_ptr_stmt(ae_flag key, S_Symbol xid, Type_Decl* decl, Arg_List args, int pos) {
  Stmt a              = calloc(1, sizeof(struct Stmt_));
  a->stmt_type             = ae_stmt_funcptr;
  a->d.stmt_ptr.flag  = key;
  a->d.stmt_ptr.type  = decl;
  a->d.stmt_ptr.xid   = xid;
  a->d.stmt_ptr.args  = args;
  a->pos = a->d.stmt_ptr.pos  = pos;
  return a;

}

Stmt new_stmt_typedef(Type_Decl* decl, S_Symbol xid, int pos) {
  Stmt a              = calloc(1, sizeof(struct Stmt_));
  a->stmt_type             = ae_stmt_typedef;
  a->d.stmt_type.type  = decl;
  a->d.stmt_type.xid   = xid;
  a->pos = a->d.stmt_ptr.pos  = pos;
  return a;
}

static void free_stmt_typedef(Stmt_Typedef a){
  free_type_decl(a->type);
}

static void free_stmt_func_ptr(Stmt_Ptr a) {
  if(a->func)
    REM_REF(a->func)
  else {
    if(a->args)
      free_arg_list(a->args);
    free_type_decl(a->type);
  }
}

Exp new_exp_call(Exp base, Exp args, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_call;
  a->meta = ae_meta_value;
  a->d.exp_func.func = base;
  a->d.exp_func.args = args;
  a->d.exp_func.m_func = NULL;
  a->pos = a->d.exp_func.pos = pos;
  a->d.exp_func.self = a;
  return a;
}

static void free_exp_call(Exp_Func* a) {
  if(a->m_func && GET_FLAG(a->m_func, ae_flag_checked))
    free(a->m_func->def);
  if(a->types)
    free_type_list(a->types);
  free_exp(a->func);
  if(a->args)
    free_exp(a->args);
}

Exp new_exp_dot(Exp base, S_Symbol xid, int pos) {
  Exp a = calloc(1, sizeof(struct Exp_));
  a->exp_type = ae_exp_dot;
  a->meta = ae_meta_var;
  a->d.exp_dot.base = base;
  a->d.exp_dot.xid = xid;
  a->pos = a->d.exp_dot.pos = pos;
  a->d.exp_dot.self = a;
  return a;
}

static void free_dot_member_exp(Exp_Dot* dot) {
  if(dot->base)
    free_exp(dot->base);
}

Exp prepend_exp(Exp exp, Exp next, int pos) {
  exp->next = next;
  return exp;
}

static void free_exp_primary(Exp_Primary* a) {
  if(a->type == ae_primary_hack)
    free_exp(a->d.exp);
  else if(a->type == ae_primary_array)
    free_array_sub(a->d.array);
  else if(a->type == ae_primary_complex)
    free_complex(a->d.cmp);
  else if(a->type == ae_primary_polar)
    free_polar(a->d.polar);
  else if(a->type == ae_primary_vec)
    free_vec(a->d.vec);
}

void free_exp(Exp exp) {
  if(!exp)
    return;
  switch(exp->exp_type) {
    case ae_exp_decl:
      free_exp_decl(&exp->d.exp_decl);
      break;
    case ae_exp_binary:
      free_exp_binary(&exp->d.exp_binary);
      break;
    case ae_exp_unary:
      free_unary_exp(&exp->d.exp_unary);
      break;
    case ae_exp_primary:
      free_exp_primary(&exp->d.exp_primary);
      break;
    case ae_exp_cast:
      free_exp_cast(&exp->d.exp_cast);
      break;
    case ae_exp_post:
      free_exp_post(&exp->d.exp_post);
      break;
    case ae_exp_call:
      free_exp_call(&exp->d.exp_func);
      break;
    case ae_exp_array:
      free_array_exp(&exp->d.exp_array);
      break;
    case ae_exp_if:
      free_if_exp(&exp->d.exp_if);
      break;
    case ae_exp_dot:
      free_dot_member_exp(&exp->d.exp_dot);
      break;
    case ae_exp_dur:
      free_dur_exp(&exp->d.exp_dur);
      break;
  }
  free_exp(exp->next);
  free(exp);
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

Stmt new_stmt_exp(Exp exp, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->stmt_type = ae_stmt_exp;
  a->d.stmt_exp.val = exp;
  a->pos = a->d.stmt_exp.pos = pos;
  return a;
}

Stmt new_stmt_code(Stmt_List stmt_list, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->stmt_type = ae_stmt_code;
  a->d.stmt_code.stmt_list = stmt_list;
  a->pos = a->d.stmt_code.pos = pos;
  return a;
}

static void free_stmt_code(Stmt_Code a) {
  free_stmt_list(a->stmt_list);
}

Stmt new_stmt_return(Exp exp, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->stmt_type = ae_stmt_return;
  a->d.stmt_return.val = exp;
  a->pos = a->d.stmt_return.pos = pos;
  return a;
}

__inline static void free_stmt_exp(struct Stmt_Exp_* a) {
  free_exp(a->val);
}

Stmt new_stmt_break(int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->stmt_type = ae_stmt_break;
  a->pos = a->d.stmt_break.pos = pos;
  return a;
}

Stmt new_stmt_continue(int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->stmt_type = ae_stmt_continue;
  a->pos = a->d.stmt_continue.pos = pos;
  return a;
}

Stmt new_stmt_while(Exp cond, Stmt body, m_bool is_do, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->stmt_type = ae_stmt_while;
  a->d.stmt_while.is_do = is_do;
  a->d.stmt_while.cond = cond;
  a->d.stmt_while.body = body;
  a->pos = a->d.stmt_while.pos = pos;
  a->d.stmt_while.self = a;
  return a;
}

static void free_stmt_flow(struct Stmt_Flow_* a) {
  free_exp(a->cond);
  free_stmt(a->body);
}

Stmt new_stmt_until(Exp cond, Stmt body, m_bool is_do, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->stmt_type = ae_stmt_until;
  a->d.stmt_until.is_do = is_do;
  a->d.stmt_until.cond = cond;
  a->d.stmt_until.body = body;
  a->pos = a->d.stmt_until.pos = pos;
  a->d.stmt_until.self = a;
  return a;
}

Stmt new_stmt_for(Stmt c1, Stmt c2, Exp c3, Stmt body, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->stmt_type = ae_stmt_for;
  a->d.stmt_for.c1 = c1;
  a->d.stmt_for.c2 = c2;
  a->d.stmt_for.c3 = c3;
  a->d.stmt_for.body = body;
  a->pos = a->d.stmt_for.pos = pos;
  a->d.stmt_for.self = a;
  return a;
}

static void free_stmt_for(Stmt_For a) {
  free_stmt(a->c1);
  free_stmt(a->c2);
  free_exp(a->c3);
  free_stmt(a->body);
}

Stmt new_stmt_auto(S_Symbol sym, Exp exp, Stmt body, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->stmt_type = ae_stmt_auto;
  a->d.stmt_auto.sym = sym;
  a->d.stmt_auto.exp = exp;
  a->d.stmt_auto.body = body;
  a->pos = a->d.stmt_auto.pos = pos;
  return a;
}

static void free_stmt_auto(Stmt_Auto a) {
  free_exp(a->exp);
  free_stmt(a->body);
  if(a->v)
    REM_REF(a->v)
}

Stmt new_stmt_gotolabel(S_Symbol xid, m_bool is_label, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->stmt_type = ae_stmt_gotolabel;
  a->d.stmt_gotolabel.name = xid;
  a->d.stmt_gotolabel.is_label = is_label;
  a->pos = a->d.stmt_gotolabel.pos = pos;
  return a;
}

Stmt new_stmt_loop(Exp cond, Stmt body, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->stmt_type = ae_stmt_loop;
  a->d.stmt_loop.cond = cond;
  a->d.stmt_loop.body = body;
  a->pos = a->d.stmt_loop.pos = pos;
  a->d.stmt_loop.self = a;
  return a;
}

static void free_stmt_loop(Stmt_Loop a) {
  free_exp(a->cond);
  free_stmt(a->body);
}

Stmt new_stmt_if(Exp cond, Stmt if_body, Stmt else_body, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->stmt_type = ae_stmt_if;
  a->d.stmt_if.cond = cond;
  a->d.stmt_if.if_body = if_body;
  a->d.stmt_if.else_body = else_body;
  a->pos = a->d.stmt_if.pos = pos;
  return a;
}

static void free_stmt_if(Stmt_If a) {
  free_exp(a->cond);
  free_stmt(a->if_body);
  if(a->else_body)
    free_stmt(a->else_body);
}

Stmt new_stmt_switch(Exp val, Stmt stmt, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->stmt_type = ae_stmt_switch;
  a->d.stmt_switch.val = val;
  a->d.stmt_switch.stmt = stmt;
  a->pos = a->d.stmt_switch.pos = pos;
  a->d.stmt_switch.self = a;
  return a;
}

__inline static void free_stmt_switch(Stmt_Switch a) {
  free_exp(a->val);
  free_stmt(a->stmt);
}

Stmt new_stmt_case(Exp val, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->stmt_type = ae_stmt_case;
  a->d.stmt_case.val = val;
  a->pos = a->d.stmt_case.pos = pos;
  return a;
}

Stmt new_stmt_enum(ID_List list, S_Symbol xid, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->stmt_type = ae_stmt_enum;
  a->d.stmt_enum.xid = xid;
  a->d.stmt_enum.list = list;
  a->d.stmt_enum.flag = 0;
  vector_init(&a->d.stmt_enum.values);
  a->pos = a->d.stmt_enum.pos = pos;
  return a;
}

static void free_stmt_enum(Stmt_Enum a) {
  free_id_list(a->list);
  vector_release(&a->values);
}

Stmt new_stmt_union(Decl_List l, int pos) {
  Stmt a = calloc(1, sizeof(struct Stmt_));
  a->stmt_type = ae_stmt_union;
  a->d.stmt_union.l = l;
  vector_init(&a->d.stmt_union.v);
  a->pos = a->d.stmt_union.pos = pos;
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
    free_exp(a->self);
  free(a);
}

__inline static void free_stmt_union(Stmt_Union a) {
  free_decl_list(a->l);
  vector_release(&a->v);
}

void free_stmt(Stmt stmt) {
  switch(stmt->stmt_type) {
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
    case ae_stmt_auto:
      free_stmt_auto(&stmt->d.stmt_auto);
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
    case ae_stmt_typedef:
      free_stmt_typedef(&stmt->d.stmt_type);
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
  if(!list)
    return;
  free_stmt(list->stmt);
  free_stmt_list(list->next);
  free(list);
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

void free_class_body(Class_Body a) {
  if(!a)
    return;
  free_class_body(a->next);
  if(a->section)
    free_section(a->section);
  free(a);
}

void free_class_def(Class_Def a) {
  if(a->type && GET_FLAG(a->type, ae_flag_template))
    return;
  if(a->ext)
    free_id_list(a->ext);
  if(a->types)
    free_id_list(a->types);
  if(!a->type || !GET_FLAG(a->type, ae_flag_ref))
    free_class_body(a->body);
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
      if(section->d.func_def->code)
        free_stmt(section->d.func_def->code);
      if(!section->d.func_def->d.func || GET_FLAG(section->d.func_def, ae_flag_builtin))
        free_func_def(section->d.func_def);
      break;
  }
  free(section);
}

Class_Def new_class_def(ae_flag class_decl, ID_List name, ID_List ext, Class_Body body, int pos) {
  Class_Def a = calloc(1, sizeof(struct Class_Def_));
  a->flag = class_decl;
  a->name = name;
  a->ext  = ext;
  a->body = body;
  a->pos  = pos;
  return a;
}

Class_Body new_class_body(Section* section, Class_Body body, int pos) {
  Class_Body a = calloc(1, sizeof(struct Class_Body_));
  a->section = section;
  a->next = body;
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
  Section* a = calloc(1, sizeof(Section));
  a->type = ae_section_class;
  a->d.class_def = class_def;
  a->pos = pos;
  return a;
}

Ast new_ast(Section* section, Ast next, int pos) {
  Ast ast = malloc(sizeof(struct Ast_));
  ast->section = section;
  ast->next = next;
  ast->pos = pos;
  return ast;
}

void free_ast(Ast a) {
  if(!a)
    return;
  free_ast(a->next);
  free_section(a->section);
  free(a);
}
