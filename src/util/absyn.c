#include <stdlib.h>
#include "absyn.h"
#include "oo.h"
#include "func.h"
#include "type.h"
#include "value.h"
#include "mpool.h"
POOL_HANDLE(Ast,           8)
POOL_HANDLE(Section,       32)
POOL_HANDLE(Class_Def,     32)
POOL_HANDLE(Class_Body,    32)
POOL_HANDLE(Func_Def,      1024)
POOL_HANDLE(Arg_List,      1024)
POOL_HANDLE(Array_Sub,     128)
POOL_HANDLE(Stmt_List,     1024)
POOL_HANDLE(Stmt,          1024)
POOL_HANDLE(Type_Decl,     512)
POOL_HANDLE(Type_List,     32)
POOL_HANDLE(Tmpl_Call,     32)
POOL_HANDLE(Tmpl_Class,    32)
POOL_HANDLE(Tmpl_List,     32)
POOL_HANDLE(Decl_List,     8)
POOL_HANDLE(ID_List,       512)
POOL_HANDLE(Var_Decl_List, 256)
POOL_HANDLE(Var_Decl,      1024)
POOL_HANDLE(Exp,           1024)

ANN static void free_stmt_list(Stmt_List);
ANN static void free_stmt_code(Stmt_Code);
ANN static void free_section(Section*);

__attribute__((nonnull(1)))
Var_Decl new_var_decl(const Symbol xid, const Array_Sub array, const int pos) {
  Var_Decl a = mp_alloc(Var_Decl);
  a->xid = xid;
  a->array = array;
  a->pos = pos;
  return a;
}

ANN void free_array_sub(Array_Sub a) {
  if(a->exp_list)
    free_exp(a->exp_list);
  mp_free(Array_Sub, a);
}

ANN static void free_var_decl(Var_Decl a) {
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
  mp_free(Var_Decl, a);
}

__attribute__((nonnull(1)))
Var_Decl_List new_var_decl_list(const Var_Decl decl, const Var_Decl_List list, const int pos) {
  Var_Decl_List a = mp_alloc(Var_Decl_List);
  a->self = decl;
  a->next = list;
  a->pos = pos;
  return a;
}

ANN static void free_var_decl_list(Var_Decl_List a) {
  if(a->next)
    free_var_decl_list(a->next);
  free_var_decl(a->self);
  mp_free(Var_Decl_List, a);
}


static Type_Decl* td_alloc(const ae_flag flag, const int pos) {
  Type_Decl* a = mp_alloc(Type_Decl);\
  a->flag = flag;
  a->pos = pos;
  return a;
}

ANN Type_Decl* new_type_decl(const ID_List xid, const ae_flag flag, const int pos) {
  Type_Decl* a = td_alloc(flag, pos);
  a->xid = xid;
  return a;
}

ANN Type_Decl* new_type_decl2(const ID_List xid, const ae_flag flag, const int pos) {
  Type_Decl* a = td_alloc(flag, pos);
  a->xid = new_id_list(insert_symbol(""), pos);
  a->xid->ref = xid;
  return a;
}
Array_Sub new_array_sub(const Exp exp, const int pos) {
  Array_Sub a = mp_alloc(Array_Sub);
  a->exp_list = exp;
  a->depth = 1;
  a->pos = pos;
  return a;
}

__attribute__((nonnull(1)))
Array_Sub prepend_array_sub(const Array_Sub a, const Exp exp) {
  if(exp) {
    exp->next = a->exp_list;
    a->exp_list = exp;
  }
  a->depth++;
  return a;
}

ANN Type_Decl* add_type_decl_array(Type_Decl* a, const Array_Sub array) {
  a->array = array;
  return a;
}

ANN Exp new_array(const Exp base, const Array_Sub indices, const int pos) {
  Exp a = mp_alloc(Exp);
  a->exp_type = ae_exp_array;
  a->meta = ae_meta_var;
  a->d.exp_array.base = base;
  a->d.exp_array.indices = indices;
  a->pos = a->d.exp_array.pos = pos;
  a->d.exp_array.self = a;
  return a;
}

ANN static void free_array_exp(Exp_Array* a) {
  if(a->base && a->base->type && a->indices->depth < a->base->type->array_depth)
    REM_REF(a->self->type);
  free_array_sub(a->indices);
  free_exp(a->base);
}

ANN ID_List new_id_list(const Symbol xid, const int pos) {
  ID_List a = mp_alloc(ID_List);
  a->xid = xid;
  a->pos = pos;
  return a;
}

ANN ID_List prepend_id_list(const Symbol xid, const ID_List list, const int pos) {
  ID_List a = new_id_list(xid, pos);
  a->next = list;
  a->pos = pos;
  return a;
}

ANN void free_id_list(ID_List a) {
  if(a->ref)
    free_id_list(a->ref);
  if(a->next)
    free_id_list(a->next);
  mp_free(ID_List, a);
}

ANN void free_type_decl(Type_Decl* a) {
  if(a->types)
    free_type_list(a->types);
  if(a->array)
    free_array_sub(a->array);
  free_id_list(a->xid);
  mp_free(Type_Decl, a);
}

Exp new_exp_decl(Type_Decl* type, const Var_Decl_List list, const int pos) {
  Exp a = mp_alloc(Exp);
  a->exp_type = ae_exp_decl;
  a->d.exp_decl.type = type;
  a->d.exp_decl.list = list;
  a->pos  = a->d.exp_decl.pos  = pos;
  a->d.exp_decl.self = a;
  return a;
}

ANN static void free_exp_decl(Exp_Decl* a) {
  free_type_decl(a->type);
  free_var_decl_list(a->list);
}

Exp new_exp_binary(const Exp lhs, const Operator op, const Exp rhs, const int pos) {
  Exp a = mp_alloc(Exp);
  a->exp_type = ae_exp_binary;
  a->meta = ae_meta_value;
  a->d.exp_binary.lhs = lhs;
  a->d.exp_binary.op = op;
  a->d.exp_binary.rhs = rhs;
  a->pos = a->d.exp_binary.pos = pos;
  a->d.exp_binary.self = a;
  return a;
}

ANN static void free_exp_binary(Exp_Binary* binary) {
  free_exp(binary->lhs);
  free_exp(binary->rhs);
}

Exp new_exp_cast(Type_Decl* type, const Exp exp, const int pos) {
  Exp a = mp_alloc(Exp);
  a->exp_type = ae_exp_cast;
  a->meta = ae_meta_value;
  a->d.exp_cast.type = type;
  a->pos = a->d.exp_cast.pos = pos;
  a->d.exp_cast.exp = exp;
  a->d.exp_cast.self = a;
  return a;
}

ANN static void free_exp_cast(Exp_Cast* a) {
  free_type_decl(a->type);
  if(a->self->type && a->self->type->array_depth)
    REM_REF(a->self->type)
  free_exp(a->exp);
}

Exp new_exp_post(const Exp exp, const Operator op, const int pos) {
  Exp a = mp_alloc(Exp);
  a->exp_type = ae_exp_post;
  a->meta = ae_meta_var;
  a->d.exp_post.exp = exp;
  a->d.exp_post.op = op;
  a->pos = a->d.exp_post.pos = pos;
  a->d.exp_post.self = a;
  return a;
}

ANN static void free_exp_post(Exp_Postfix* post) {
  free_exp(post->exp);
}

Exp new_exp_dur(const Exp base, const Exp unit, const int pos) {
  Exp a = mp_alloc(Exp);
  a->exp_type = ae_exp_dur;
  a->meta = ae_meta_value;
  a->d.exp_dur.base = base;
  a->d.exp_dur.unit = unit;
  a->pos = a->d.exp_dur.pos = pos;
  a->d.exp_dur.self = a;
  return a;
}

ANN static void free_dur_exp(Exp_Dur* a) {
  free_exp(a->base);
  free_exp(a->unit);
}

static Exp new_exp_prim(const int pos) {
  Exp a = mp_alloc(Exp);
  a->exp_type = ae_exp_primary;
  a->meta = ae_meta_value;
  a->pos = a->d.exp_primary.pos = pos;
  a->d.exp_primary.self = a;
  return a;
}

Exp new_exp_prim_int(const long i, const int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.primary_type = ae_primary_num;
  a->d.exp_primary.d.num = i;
  return a;
}

Exp new_exp_prim_float(const m_float num, const int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.primary_type = ae_primary_float;
  a->d.exp_primary.d.fnum = num;
  return a;
}

Exp new_exp_prim_string(const m_str s, const int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.primary_type = ae_primary_str;
  a->d.exp_primary.d.str = s;
  return a;
}

Exp new_exp_prim_nil(const int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.primary_type = ae_primary_nil;
  return a;
}

Exp new_exp_prim_id(const Symbol xid, const int pos) {
  Exp a = new_exp_prim(pos);
  a->meta = ae_meta_var;
  a->d.exp_primary.primary_type = ae_primary_id;
  a->d.exp_primary.d.var = xid;
  return a;
}

Exp new_exp_prim_hack(const Exp exp, const int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.primary_type = ae_primary_hack;
  a->d.exp_primary.d.exp = exp;
  return a;
}

Exp new_exp_prim_char(const m_str chr, const int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.primary_type = ae_primary_char;
  a->d.exp_primary.d.chr = chr;
  return a;
}

Exp new_exp_prim_array(const Array_Sub exp_list, const int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.primary_type = ae_primary_array;
  a->d.exp_primary.d.array = exp_list;
  return a;
}

Exp new_exp_prim_vec(const ae_Exp_Primary_Type t, Exp e, const int pos) {
  Exp a = new_exp_prim(pos);
  a->d.exp_primary.primary_type = t;
  a->d.exp_primary.d.vec.exp = e;
  do
    a->d.exp_primary.d.vec.dim++;
  while((e = e->next));
  a->d.exp_primary.self = a;
  return a;
}

static Exp new_exp_unary_base(const int pos)  {
  Exp a = mp_alloc(Exp);
  a->meta = ae_meta_value;
  a->exp_type = ae_exp_unary;
  a->pos = a->d.exp_unary.pos = pos;
  a->d.exp_unary.self = a;
  return a;
}

Exp new_exp_unary(const Operator oper, const Exp exp, const int pos) {
  Exp a = new_exp_unary_base(pos);
  a->meta = exp->meta;
  a->d.exp_unary.op = oper;
  a->d.exp_unary.exp = exp;
  return a;
}

Exp new_exp_unary2(const Operator oper, Type_Decl* type, const int pos) {
  Exp a = new_exp_unary_base(pos);
  a->d.exp_unary.op = oper;
  a->d.exp_unary.type = type;
  return a;
}

Exp new_exp_unary3(const Operator oper, const Stmt code, const int pos) {
  Exp a = new_exp_unary_base(pos);
  a->d.exp_unary.op = oper;
  ID_List id = new_id_list(insert_symbol("void"), pos);
  a->d.exp_unary.type = new_type_decl(id, 0, pos);
  a->d.exp_unary.code = code;
  return a;
}

ANN static void free_unary_exp(Exp_Unary* a) {
  if(a->exp)
    free_exp(a->exp);
  if(a->type)
    free_type_decl(a->type);
  if(a->code)
    free_stmt(a->code);
}

Exp new_exp_if(const Exp cond, const Exp if_exp, const Exp else_exp, const int pos) {
  Exp a = mp_alloc(Exp);
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

ANN static void free_if_exp(Exp_If* a) {
  free_exp(a->cond);
  free_exp(a->if_exp);
  free_exp(a->else_exp);
}

ANN Tmpl_List* new_tmpl_list(const ID_List list, const m_int base) {
  Tmpl_List* a = mp_alloc(Tmpl_List);
  a->list = list;
  a->base = base;
  return a;
}

ANN Tmpl_Class* new_tmpl_class(ID_List list, const m_bool base) {
  Tmpl_Class* a = mp_alloc(Tmpl_Class);
  a->list.list = list;
  a->list.base = base;
  a->base = NULL;
  return a;
}

ANN void free_tmpl_list(Tmpl_List* a) {
  if(a->base == -1)
    free_id_list(a->list);
  mp_free(Tmpl_List, a);
}

ANN void free_tmpl_class(Tmpl_Class* a) {
  if(a->list.base == -1)
    free_id_list(a->list.list);
  mp_free(Tmpl_Class, a);
}

m_bool tmpl_list_base(const Tmpl_List* a) {
  if(a && a->base == -1)
    return 1;
  return 0;
}

m_bool tmpl_class_base(const Tmpl_Class* a) {
  return a ? tmpl_list_base(&a->list) : 0;
}

Func_Def new_func_def(const ae_flag flag, Type_Decl* type_decl, const Symbol xid,
  const Arg_List arg_list, const Stmt code, const int pos) {
  Func_Def a = mp_alloc(Func_Def);
  a->flag = flag;
  a->type_decl = type_decl;
  a->name = xid;
  a->arg_list = arg_list;
  a->code = code;
  a->pos = pos;
  return a;
}

ANN void free_func_def(Func_Def a) {
  if(!GET_FLAG(a, ae_flag_template)) {
    if(a->ret_type && a->ret_type->array_depth)
      REM_REF(a->ret_type);
    if(a->arg_list)
      free_arg_list(a->arg_list);
    free_type_decl(a->type_decl);
  }
  if(a->tmpl)
    free_tmpl_list(a->tmpl);
  mp_free(Func_Def, a);
}
ANN void free_func_def_simple(Func_Def a) { mp_free(Func_Def, a); }

ANN Stmt new_func_ptr_stmt(const ae_flag key, const Symbol xid, Type_Decl* decl, const Arg_List args, const int pos) {
  Stmt a              = mp_alloc(Stmt);
  a->stmt_type        = ae_stmt_funcptr;
  a->d.stmt_ptr.flag  = key;
  a->d.stmt_ptr.type  = decl;
  a->d.stmt_ptr.xid   = xid;
  a->d.stmt_ptr.args  = args;
  a->pos = a->d.stmt_ptr.pos  = pos;
  return a;

}

ANN Stmt new_stmt_typedef(Type_Decl* decl, const Symbol xid, const int pos) {
  Stmt a              = mp_alloc(Stmt);
  a->stmt_type             = ae_stmt_typedef;
  a->d.stmt_type.type  = decl;
  a->d.stmt_type.xid   = xid;
  a->pos = a->d.stmt_ptr.pos  = pos;
  return a;
}

ANN static void free_stmt_typedef(Stmt_Typedef a){
  if(!a->m_type)
    free_type_decl(a->type);
}

ANN static void free_stmt_func_ptr(Stmt_Ptr a) {
  if(a->func)
    REM_REF(a->func)
  else {
    if(a->args)
      free_arg_list(a->args);
    free_type_decl(a->type);
  }
}

ANN Tmpl_Call* new_tmpl_call(const Type_List tl) {
  Tmpl_Call* a = mp_alloc(Tmpl_Call);
  a->types = tl;
  a->base = NULL;
  return a;
}

ANN static void free_tmpl_call(Tmpl_Call* a) {
  free_type_list(a->types);
  mp_free(Tmpl_Call, a);
}

Exp new_exp_call(const Exp base, const Exp args, const int pos) {
  Exp a = mp_alloc(Exp);
  a->exp_type = ae_exp_call;
  a->meta = ae_meta_value;
  a->d.exp_func.func = base;
  a->d.exp_func.args = args;
  a->pos = a->d.exp_func.pos = pos;
  a->d.exp_func.self = a;
  return a;
}

ANN static void free_exp_call(Exp_Func* a) {
  if(a->m_func && GET_FLAG(a->m_func, ae_flag_checked))
    if(a->m_func->def)
      free_func_def_simple(a->m_func->def);
  if(a->tmpl)
    free_tmpl_call(a->tmpl);
  free_exp(a->func);
  if(a->args)
    free_exp(a->args);
}

Exp new_exp_dot(const Exp base, const Symbol xid, const int pos) {
  Exp a = mp_alloc(Exp);
  a->exp_type = ae_exp_dot;
  a->meta = ae_meta_var;
  a->d.exp_dot.base = base;
  a->d.exp_dot.xid = xid;
  a->pos = a->d.exp_dot.pos = pos;
  a->d.exp_dot.self = a;
  return a;
}

ANN static void free_dot_member_exp(Exp_Dot* dot) {
  if(dot->base)
    free_exp(dot->base);
}

Exp prepend_exp(const Exp exp, const Exp next) {
  exp->next = next;
  return exp;
}

ANN static void free_exp_primary(Exp_Primary* a) {
  ae_Exp_Primary_Type t = a->primary_type;
  if(t == ae_primary_hack)
    free_exp(a->d.exp);
  else if(t == ae_primary_array)
    free_array_sub(a->d.array);
  else if(t== ae_primary_complex ||
          t == ae_primary_polar  ||
          t == ae_primary_vec)
    free_exp(a->d.vec.exp);
}

ANN void free_exp(Exp exp) {
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
  if(exp->next)
    free_exp(exp->next);
  mp_free(Exp, exp);
}

Arg_List new_arg_list(Type_Decl* type_decl, const Var_Decl var_decl,
    const Arg_List arg_list, const int pos) {
  Arg_List a = mp_alloc(Arg_List);
  a->type_decl = type_decl;
  a->var_decl = var_decl;
  a->next = arg_list;
  a->pos = pos;
  return a;
}

ANN void free_arg_list(Arg_List a) {
  if(a->next)
    free_arg_list(a->next);
  free_type_decl(a->type_decl);
  free_var_decl(a->var_decl);
  mp_free(Arg_List, a);

}

Stmt new_stmt_exp(const Exp exp, const int pos) {
  Stmt a = mp_alloc(Stmt);
  a->stmt_type = ae_stmt_exp;
  a->d.stmt_exp.val = exp;
  a->pos = a->d.stmt_exp.pos = pos;
  return a;
}

Stmt new_stmt_code(const Stmt_List stmt_list, const int pos) {
  Stmt a = mp_alloc(Stmt);
  a->stmt_type = ae_stmt_code;
  a->d.stmt_code.stmt_list = stmt_list;
  a->pos = a->d.stmt_code.pos = pos;
  return a;
}

ANN static void free_stmt_code(Stmt_Code a) {
  if(a->stmt_list)
    free_stmt_list(a->stmt_list);
}

Stmt new_stmt_return(const Exp exp, const int pos) {
  Stmt a = mp_alloc(Stmt);
  a->stmt_type = ae_stmt_return;
  a->d.stmt_return.val = exp;
  a->pos = a->d.stmt_return.pos = pos;
  return a;
}

ANN __inline static void free_stmt_exp(struct Stmt_Exp_* a) {
  if(a->val)
    free_exp(a->val);
}

Stmt new_stmt_break(const int pos) {
  Stmt a = mp_alloc(Stmt);
  a->stmt_type = ae_stmt_break;
  a->pos = a->d.stmt_break.pos = pos;
  return a;
}

Stmt new_stmt_continue(const int pos) {
  Stmt a = mp_alloc(Stmt);
  a->stmt_type = ae_stmt_continue;
  a->pos = a->d.stmt_continue.pos = pos;
  return a;
}

ANN Stmt new_stmt_while(const Exp cond, const Stmt body, const m_bool is_do, const int pos) {
  Stmt a = mp_alloc(Stmt);
  a->stmt_type = ae_stmt_while;
  a->d.stmt_while.is_do = is_do;
  a->d.stmt_while.cond = cond;
  a->d.stmt_while.body = body;
  a->pos = a->d.stmt_while.pos = pos;
  a->d.stmt_while.self = a;
  return a;
}

ANN static void free_stmt_flow(struct Stmt_Flow_* a) {
  free_exp(a->cond);
  free_stmt(a->body);
}

ANN Stmt new_stmt_until(const Exp cond, const Stmt body, const m_bool is_do, const int pos) {
  Stmt a = mp_alloc(Stmt);
  a->stmt_type = ae_stmt_until;
  a->d.stmt_until.is_do = is_do;
  a->d.stmt_until.cond = cond;
  a->d.stmt_until.body = body;
  a->pos = a->d.stmt_until.pos = pos;
  a->d.stmt_until.self = a;
  return a;
}

__attribute__((nonnull(1,2,4)))
Stmt new_stmt_for(const Stmt c1, const Stmt c2, const Exp c3, const Stmt body, const int pos) {
  Stmt a = mp_alloc(Stmt);
  a->stmt_type = ae_stmt_for;
  a->d.stmt_for.c1 = c1;
  a->d.stmt_for.c2 = c2;
  a->d.stmt_for.c3 = c3;
  a->d.stmt_for.body = body;
  a->pos = a->d.stmt_for.pos = pos;
  a->d.stmt_for.self = a;
  return a;
}

ANN static void free_stmt_for(Stmt_For a) {
  free_stmt(a->c1);
  free_stmt(a->c2);
  if(a->c3)
    free_exp(a->c3);
  free_stmt(a->body);
}

ANN Stmt new_stmt_auto(const Symbol sym, const Exp exp, const Stmt body, const int pos) {
  Stmt a = mp_alloc(Stmt);
  a->stmt_type = ae_stmt_auto;
  a->d.stmt_auto.sym = sym;
  a->d.stmt_auto.exp = exp;
  a->d.stmt_auto.body = body;
  a->d.stmt_auto.self = a;
  a->pos = a->d.stmt_auto.pos = pos;
  return a;
}

ANN static void free_stmt_auto(Stmt_Auto a) {
  free_exp(a->exp);
  free_stmt(a->body);
  if(a->v)
    REM_REF(a->v)
}

ANN Stmt new_stmt_gotolabel(const Symbol xid, const m_bool is_label, const int pos) {
  Stmt a = mp_alloc(Stmt);
  a->stmt_type = ae_stmt_gotolabel;
  a->d.stmt_gotolabel.name = xid;
  a->d.stmt_gotolabel.is_label = is_label;
  a->pos = a->d.stmt_gotolabel.pos = pos;
  return a;
}

ANN Stmt new_stmt_loop(const Exp cond, const Stmt body, const int pos) {
  Stmt a = mp_alloc(Stmt);
  a->stmt_type = ae_stmt_loop;
  a->d.stmt_loop.cond = cond;
  a->d.stmt_loop.body = body;
  a->pos = a->d.stmt_loop.pos = pos;
  a->d.stmt_loop.self = a;
  return a;
}

ANN static void free_stmt_loop(Stmt_Loop a) {
  free_exp(a->cond);
  free_stmt(a->body);
}

__attribute__((nonnull(1,2)))
Stmt new_stmt_if(const Exp cond, const Stmt if_body, const Stmt else_body, const int pos) {
  Stmt a = mp_alloc(Stmt);
  a->stmt_type = ae_stmt_if;
  a->d.stmt_if.cond = cond;
  a->d.stmt_if.if_body = if_body;
  a->d.stmt_if.else_body = else_body;
  a->pos = a->d.stmt_if.pos = pos;
  return a;
}

ANN static void free_stmt_if(Stmt_If a) {
  free_exp(a->cond);
  free_stmt(a->if_body);
  if(a->else_body)
    free_stmt(a->else_body);
}

ANN Stmt new_stmt_switch(const Exp val, Stmt stmt, const int pos) {
  Stmt a = mp_alloc(Stmt);
  a->stmt_type = ae_stmt_switch;
  a->d.stmt_switch.val = val;
  a->d.stmt_switch.stmt = stmt;
  a->pos = a->d.stmt_switch.pos = pos;
  a->d.stmt_switch.self = a;
  return a;
}

ANN __inline static void free_stmt_switch(Stmt_Switch a) {
  free_exp(a->val);
  free_stmt(a->stmt);
}

ANN Stmt new_stmt_case(const Exp val, const int pos) {
  Stmt a = mp_alloc(Stmt);
  a->stmt_type = ae_stmt_case;
  a->d.stmt_case.val = val;
  a->pos = a->d.stmt_case.pos = pos;
  return a;
}

__attribute__((nonnull(1)))
Stmt new_stmt_enum(const ID_List list, const Symbol xid, const int pos) {
  Stmt a = mp_alloc(Stmt);
  a->stmt_type = ae_stmt_enum;
  a->d.stmt_enum.xid = xid;
  a->d.stmt_enum.list = list;
  a->d.stmt_enum.flag = 0;
  vector_init(&a->d.stmt_enum.values);
  a->pos = a->d.stmt_enum.pos = pos;
  return a;
}

ANN static void free_stmt_enum(Stmt_Enum a) {
  free_id_list(a->list);
  vector_release(&a->values);
}

ANN Stmt new_stmt_union(const Decl_List l, const int pos) {
  Stmt a = mp_alloc(Stmt);
  a->stmt_type = ae_stmt_union;
  a->d.stmt_union.l = l;
  vector_init(&a->d.stmt_union.v);
  a->pos = a->d.stmt_union.pos = pos;
  return a;
}

Decl_List new_decl_list(const Exp d, const Decl_List l) {
  Decl_List a = mp_alloc(Decl_List);
  a->self = d;
  a->next = l;
  return a;
}

ANN static void free_decl_list(Decl_List a) {
  if(a->next)
    free_decl_list(a->next);
  if(a->self)
    free_exp(a->self);
  mp_free(Decl_List, a);
}

ANN __inline static void free_stmt_union(Stmt_Union a) {
  free_decl_list(a->l);
  vector_release(&a->v);
}

ANN void free_stmt(Stmt stmt) {
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
  mp_free(Stmt, stmt);
}

Stmt_List new_stmt_list(Stmt stmt, Stmt_List next) {
  Stmt_List list = mp_alloc(Stmt_List);
  list->stmt = stmt;
  list->next = next;
  return list;
}

ANN static void free_stmt_list(Stmt_List list) {
  free_stmt(list->stmt);
  if(list->next)
    free_stmt_list(list->next);
  mp_free(Stmt_List, list);
}

Section* new_section_stmt_list(const Stmt_List list, const int pos) {
  Section* a = mp_alloc(Section);
  a->section_type = ae_section_stmt;
  a->d.stmt_list = list;
  a->pos = pos;
  return a;
}

Section* new_section_func_def(const Func_Def func_def, const int pos) {
  Section* a = mp_alloc(Section);
  a->section_type = ae_section_func;
  a->d.func_def = func_def;
  a->pos = pos;
  return a;
}

ANN void free_class_body(Class_Body a) {
  if(a->next)
    free_class_body(a->next);
  if(a->section)
    free_section(a->section);
  mp_free(Class_Body, a);
}

ANN void free_class_def(Class_Def a) {
  if(a->type && GET_FLAG(a->type, ae_flag_template))
    return;
  if(a->ext) {
    if(a->ext->array && a->type->parent)
      REM_REF(a->type->parent); // ?
    free_type_decl(a->ext);
  }
  if(a->tmpl)
    free_tmpl_class(a->tmpl);
  if(a->body && (!a->type || !GET_FLAG(a->type, ae_flag_ref)))
    free_class_body(a->body);
  free_id_list(a->name);
  mp_free(Class_Def, a);
}
ANN void free_class_def_simple(Class_Def a) { mp_free(Class_Def, a); }

ANN static void free_section(Section* section) {
  switch(section->section_type) {
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
  mp_free(Section, section);
}

Class_Def new_class_def(const ae_flag class_decl, const ID_List name, Type_Decl* ext,
    const Class_Body body, const int pos) {
  Class_Def a = mp_alloc(Class_Def);
  a->flag = class_decl;
  a->name = name;
  a->ext  = ext;
  a->body = body;
  a->pos  = pos;
  return a;
}

Class_Body new_class_body(Section* section, const Class_Body body, const int pos) {
  Class_Body a = mp_alloc(Class_Body);
  a->section = section;
  a->next = body;
  a->pos = pos;
  return a;
}

Type_List new_type_list(Type_Decl* list, const Type_List next, const int pos) {
  Type_List a = mp_alloc(Type_List);
  a->list = list;
  a->next = next;
  a->pos = pos;
  return a;
}

ANN void free_type_list(Type_List a) {
  if(a->next)
    free_type_list(a->next);
  free_type_decl(a->list);
  mp_free(Type_List, a);
}

Section* new_section_class_def(const Class_Def class_def, const int pos) {
  Section* a = mp_alloc(Section);
  a->section_type = ae_section_class;
  a->d.class_def = class_def;
  a->pos = pos;
  return a;
}

Ast new_ast(Section* section, const Ast next, const int pos) {
  Ast ast = mp_alloc(Ast);
  ast->section = section;
  ast->next = next;
  ast->pos = pos;
  return ast;
}

ANN void free_ast(Ast a) {
  if(a->next)
    free_ast(a->next);
  free_section(a->section);
  mp_free(Ast, a);
}
