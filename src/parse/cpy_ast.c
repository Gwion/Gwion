#include "gwion_util.h"
#include "gwion_ast.h"
#include "cpy_ast.h"

ANN static Stmt cpy_stmt(MemPool p, const Stmt src);
ANN static Exp cpy_exp(MemPool p, const Exp src);
ANN Type_List cpy_type_list(MemPool p, const Type_List src);
ANN static Arg_List cpy_arg_list(MemPool p, const Arg_List src);
ANN Class_Def cpy_class_def(MemPool p, const Class_Def src);
ANN static Stmt_List cpy_stmt_list(MemPool p, const Stmt_List src);

ANN static void cpy_exp_dot(MemPool p, Exp_Dot *a, const Exp_Dot *src) {
  a->base = cpy_exp(p, src->base);
  a->xid = src->xid;
}

ANN static void cpy_exp_lambda(MemPool p, Exp_Lambda *a, const Exp_Lambda *src) {
  if(src->args)
    a->args = cpy_arg_list(p, src->args);
  a->code = cpy_stmt(p, src->code);
  a->name = src->name;
}

ANN Array_Sub cpy_array_sub(MemPool p, const Array_Sub src) {
  Array_Sub a = mp_calloc(p, Array_Sub);
  if(src->exp)
    a->exp = cpy_exp(p, src->exp);
  a->depth = src->depth;
  return a;
}

ANN static void cpy_exp_array(MemPool p, Exp_Array *a, const Exp_Array *src) {
  a->base = cpy_exp(p, src->base);
  a->array = cpy_array_sub(p, src->array);
}

ANN static Var_Decl cpy_var_decl(MemPool p, const Var_Decl src) {
  Var_Decl a = mp_calloc(p, Var_Decl);
  a->xid = src->xid; // 1 
  if(src->array)
    a->array = cpy_array_sub(p, src->array); // 1 
  a->pos = loc_cpy(p, src->pos); // 1 
  return a;
}

ANN static Var_Decl_List cpy_var_decl_list(MemPool p, const Var_Decl_List src) {
  Var_Decl_List a = mp_calloc(p, Var_Decl_List);
  a->self = cpy_var_decl(p, src->self); // 1 
  if(src->next)
    a->next = cpy_var_decl_list(p, src->next); // 1 
  return a;
}

ANN Type_Decl* cpy_type_decl(MemPool p, const Type_Decl* src) {
  Type_Decl* a = mp_calloc(p, Type_Decl);
  if(src->xid)
    a->xid = cpy_id_list(p, src->xid); // 1 
  if(src->exp)
    a->exp = cpy_exp(p, src->exp); // 1 
  if(src->array)
    a->array = cpy_array_sub(p, src->array); // 1 
  if(src->types)
    a->types = cpy_type_list(p, src->types); // 1 
  a->flag = src->flag; // 1 
  return a;
}

ANN ID_List cpy_id_list(MemPool p, const ID_List src) {
  ID_List a = mp_calloc(p, ID_List);
  a->xid = src->xid; // 1 
  if(src->next)
    a->next = cpy_id_list(p, src->next); // 1 
  if(src->pos)
    a->pos = loc_cpy(p, src->pos); // 1 
  return a;
}

ANN Type_List cpy_type_list(MemPool p, const Type_List src) {
  Type_List a = mp_calloc(p, Type_List);
  if(src->td)
    a->td = cpy_type_decl(p, src->td); // 1 
  if(src->next)
    a->next = cpy_type_list(p, src->next); // 1 
  return a;
}

ANN static void cpy_vec(MemPool p, Vec* a, const Vec* src) {
  a->exp = cpy_exp(p, src->exp);
  a->dim = src->dim;
}

ANN static Arg_List cpy_arg_list(MemPool p, const Arg_List src) {
  Arg_List a = mp_calloc(p, Arg_List);
  if(src->td)
    a->td = cpy_type_decl(p, src->td); // 1 
  if(src->var_decl)
    a->var_decl = cpy_var_decl(p, src->var_decl); // 1 
  if(src->next)
    a->next = cpy_arg_list(p, src->next); // 1 
  return a;
}

ANN static void cpy_exp_decl(MemPool p, Exp_Decl *a, const Exp_Decl *src) {
  a->td = cpy_type_decl(p, src->td);
  a->list = cpy_var_decl_list(p, src->list);
}

ANN static void cpy_exp_primary(MemPool p, Exp_Primary *a, const Exp_Primary *src) {
  switch(src->primary_type) {
    case ae_primary_id:
      a->d.var = src->d.var;
      break;
    case ae_primary_num:
      a->d.num = src->d.num;
      break;
    case ae_primary_float:
      a->d.fnum = src->d.fnum;
      break;
    case ae_primary_char:
      a->d.chr = src->d.chr;
      break;
    case ae_primary_str:
      a->d.str = src->d.str;
      break;
    case ae_primary_array:
      a->d.array = cpy_array_sub(p, src->d.array);
      break;
    case ae_primary_vec:
      cpy_vec(p, &a->d.vec, &src->d.vec);
      break;
    default:
      if(src->d.exp)
        a->d.exp = cpy_exp(p, src->d.exp);
      break;
  }
  a->primary_type = src->primary_type;
}

ANN Tmpl* cpy_tmpl(MemPool p, const Tmpl *src) {
  Tmpl *a = mp_calloc(p, Tmpl);
  if(src->list)
    a->list = cpy_id_list(p, src->list);
  if(src->call)
    a->call = cpy_type_list(p, src->call);
  a->base = src->base;
  return a;
}

ANN static void cpy_exp_call(MemPool p, Exp_Call *a, const Exp_Call *src) {
  a->func = cpy_exp(p, src->func);
  if(src->args)
    a->args = cpy_exp(p, src->args);
  if(src->tmpl)
    a->tmpl = cpy_tmpl(p, src->tmpl);
}

ANN static void cpy_exp_cast(MemPool p, Exp_Cast *a, const Exp_Cast *src) {
  a->td = cpy_type_decl(p, src->td);
  a->exp = cpy_exp(p, src->exp);
}

ANN static void cpy_exp_binary(MemPool p, Exp_Binary *a, const Exp_Binary *src) {
  a->lhs = cpy_exp(p, src->lhs);
  a->rhs = cpy_exp(p, src->rhs);
  a->op = src->op;
}

ANN static void cpy_exp_postfix(MemPool p, Exp_Postfix *a, const Exp_Postfix *src) {
  a->op = src->op;
  a->exp = cpy_exp(p, src->exp);
}

ANN static void cpy_exp_if(MemPool p, Exp_If *a, const Exp_If *src) {
  a->cond = cpy_exp(p, src->cond);
  if(src->if_exp)
    a->if_exp = cpy_exp(p, src->if_exp);
  if(src->else_exp)
    a->else_exp = cpy_exp(p, src->else_exp);
}


// TODO check me
ANN static void cpy_exp_unary(MemPool p, Exp_Unary *a, const Exp_Unary *src) {
  a->op = src->op;
  if(src->exp)
    a->exp = cpy_exp(p, src->exp);
  if(src->td)
    a->td = cpy_type_decl(p, src->td);
  if(src->code)
    a->code = cpy_stmt(p, src->code);
}

ANN static void cpy_exp_typeof(MemPool p, Exp_Typeof *a, const Exp_Typeof *src) {
  a->exp = cpy_exp(p, src->exp);
}

ANN static Exp cpy_exp(MemPool p, const Exp src) {
  Exp a = mp_calloc(p, Exp);
  if(src->next)
    a->next = cpy_exp(p, src->next);
  switch(src->exp_type) {
    case ae_exp_post:  // !! naming
      cpy_exp_postfix(p, &a->d.exp_post, &src->d.exp_post);
      break;
    case ae_exp_primary:
      cpy_exp_primary(p, &a->d.exp_primary, &src->d.exp_primary);
      break;
    case ae_exp_decl:
      cpy_exp_decl(p, &a->d.exp_decl, &src->d.exp_decl);
      break;
    case ae_exp_unary:
      cpy_exp_unary(p, &a->d.exp_unary, &src->d.exp_unary);
      break;
    case ae_exp_binary:
      cpy_exp_binary(p, &a->d.exp_binary, &src->d.exp_binary);
      break;
    case ae_exp_cast:
      cpy_exp_cast(p, &a->d.exp_cast, &src->d.exp_cast);
      break;
    case ae_exp_call:
      cpy_exp_call(p, &a->d.exp_call, &src->d.exp_call);
      break;
    case ae_exp_if:
      cpy_exp_if(p, &a->d.exp_if, &src->d.exp_if);
      break;
    case ae_exp_dot:
      cpy_exp_dot(p, &a->d.exp_dot, &src->d.exp_dot);
      break;
    case ae_exp_array:
      cpy_exp_array(p, &a->d.exp_array, &src->d.exp_array);
      break;
    case ae_exp_lambda:
      cpy_exp_lambda(p, &a->d.exp_lambda, &src->d.exp_lambda);
      break;
    case ae_exp_typeof:
      cpy_exp_typeof(p, &a->d.exp_typeof, &src->d.exp_typeof);
      break;
  }
  a->exp_type = src->exp_type;
  a->meta = src->meta;// maybe meta shoyuld be set as in constructors
  a->pos = loc_cpy(p, src->pos);
  return a;
}

ANN /*static */Decl_List cpy_decl_list(MemPool p, const Decl_List src) {
  Decl_List a = mp_calloc(p, Decl_List);
  a->self = cpy_exp(p, src->self);
  if(src->next)
    a->next = cpy_decl_list(p, src->next);
  return a;
}

ANN static void cpy_stmt_exp(MemPool p, const Stmt_Exp a, const Stmt_Exp src) {
  if(src->val)
    a->val = cpy_exp(p, src->val);
}

ANN static void cpy_stmt_flow(MemPool p, Stmt_Flow a,const Stmt_Flow src) {
  if(src->cond)
    a->cond = cpy_exp(p, src->cond);
  if(src->body)
    a->body = cpy_stmt(p, src->body);
  a->is_do = src->is_do;
}

ANN static void cpy_stmt_code(MemPool p, Stmt_Code a, const Stmt_Code src) {
  if(src->stmt_list)
    a->stmt_list = cpy_stmt_list(p, src->stmt_list);
}

ANN static void cpy_stmt_for(MemPool p, Stmt_For a, const Stmt_For src) {
  if(src->c1)
    a->c1 = cpy_stmt(p, src->c1); 
  if(src->c2)
    a->c2 = cpy_stmt(p, src->c2); 
  if(src->c3)
    a->c3 = cpy_exp(p, src->c3);
  if(src->body)
    a->body = cpy_stmt(p, src->body); 
}

ANN static void cpy_stmt_auto(MemPool p, Stmt_Auto a, const Stmt_Auto src) {
  if(src->sym)
    a->sym = src->sym;
  if(src->exp)
    a->exp = cpy_exp(p, src->exp);
  if(src->body)
    a->body = cpy_stmt(p, src->body);
  a->is_ptr = src->is_ptr;
}

ANN static void cpy_stmt_loop(MemPool p, Stmt_Loop a, const Stmt_Loop src) {
  if(src->cond)
    a->cond = cpy_exp(p, src->cond); 
  if(src->body)
    a->body = cpy_stmt(p, src->body); 
}

ANN static void cpy_stmt_if(MemPool p, Stmt_If a, const Stmt_If src) {
  if(src->cond)
    a->cond = cpy_exp(p, src->cond);
  if(src->if_body)
    a->if_body = cpy_stmt(p, src->if_body);
  if(src->else_body)
    a->else_body = cpy_stmt(p, src->else_body);
}

ANN static void cpy_stmt_jump(MemPool p NUSED, const Stmt_Jump a,const Stmt_Jump src) {
  a->name = src->name;
  a->is_label = src->is_label;
}

ANN static Stmt cpy_stmt_case(MemPool p, const Stmt_Match src) {
  Stmt a = mp_calloc(p, Stmt);
  a->d.stmt_match.cond = cpy_exp(p, src->cond);
  a->d.stmt_match.list = cpy_stmt_list(p, src->list);
  if(src->when)
    a->d.stmt_match.when = cpy_exp(p, src->when);
  return a;
}

ANN static Stmt_List cpy_stmt_cases(MemPool p, const Stmt_List src) {
  Stmt_List a = mp_calloc(p, Stmt_List);
  if(src->next)
    a->next = cpy_stmt_cases(p, src->next);
  a->stmt = cpy_stmt_case(p, &src->stmt->d.stmt_match);
  return a;
}

ANN static void cpy_stmt_match(MemPool p, Stmt_Match a, const Stmt_Match src) {
  a->cond = cpy_exp(p, src->cond);
  a->list = cpy_stmt_cases(p, src->list);
  if(src->where)
    a->where = cpy_stmt(p, src->where);
}

ANN static Enum_Def cpy_enum_def(MemPool p, const Enum_Def src) {
  Enum_Def a = mp_calloc(p, Enum_Def);
  a->list = cpy_id_list(p, src->list);
  a->xid = src->xid;
  a->pos = loc_cpy(p, src->pos);
  a->flag = src->flag;
  vector_init(&a->values);
  return a;
}

ANN Func_Base* cpy_func_base(MemPool p, const Func_Base* src) {
  Func_Base *a = mp_calloc(p, Func_Base);
  if(src->td)
    a->td = cpy_type_decl(p, src->td); // 1 
  if(src->xid)
    a->xid = src->xid; // 1 
  if(src->args)
    a->args = cpy_arg_list(p, src->args); // 1 
  if(src->tmpl)
    a->tmpl = cpy_tmpl(p, src->tmpl); // 1 
  return a;
}

ANN static Fptr_Def cpy_fptr_def(MemPool p, const Fptr_Def src) {
  Fptr_Def a = mp_calloc(p, Fptr_Def);
  a->base = cpy_func_base(p, src->base);
  return a;
}

ANN static void cpy_type_def2(MemPool p, Type_Def a, const Type_Def src) {
  if(src->ext)
    a->ext = cpy_type_decl(p, src->ext);
  a->xid = src->xid;
  if(src->tmpl)
    a->tmpl = cpy_tmpl(p, src->tmpl);
}

ANN static Type_Def cpy_type_def(MemPool p, const Type_Def src) {
  Type_Def a = mp_calloc(p, Type_Def);
  cpy_type_def2(p, a, src);
  return a;
}

ANN static Union_Def cpy_union_def(MemPool p, const Union_Def src) {
  Union_Def a = mp_calloc(p, Union_Def);
  a->l = cpy_decl_list(p, src->l); // 1 
  if(src->xid)
    a->xid = src->xid; // 1 
  if(src->type_xid)
    a->type_xid = src->type_xid; // 1 
  if(src->tmpl)
    a->tmpl = cpy_tmpl(p, src->tmpl); // 1 
  a->pos = loc_cpy(p, src->pos);
  a->flag = src->flag; // 1 
  return a;
}

ANN static Stmt cpy_stmt(MemPool p, const Stmt src) {
  Stmt a = mp_calloc(p, Stmt);
  switch(src->stmt_type) {
    case ae_stmt_exp:
    case ae_stmt_return:
      cpy_stmt_exp(p, &a->d.stmt_exp, &src->d.stmt_exp);
      break;
    case ae_stmt_code:
      cpy_stmt_code(p, &a->d.stmt_code, &src->d.stmt_code);
      break;
    case ae_stmt_while:
    case ae_stmt_until:
      cpy_stmt_flow(p, &a->d.stmt_flow, &src->d.stmt_flow);
      break;
    case ae_stmt_loop:
      cpy_stmt_loop(p, &a->d.stmt_loop, &src->d.stmt_loop);
      break;
    case ae_stmt_for:
      cpy_stmt_for(p, &a->d.stmt_for, &src->d.stmt_for);
      break;
    case ae_stmt_auto:
      cpy_stmt_auto(p, &a->d.stmt_auto, &src->d.stmt_auto);
      break;
    case ae_stmt_if:
      cpy_stmt_if(p, &a->d.stmt_if, &src->d.stmt_if);
      break;
    case ae_stmt_jump:
      cpy_stmt_jump(p, &a->d.stmt_jump, &src->d.stmt_jump);
      break;
    case ae_stmt_match:
      cpy_stmt_match(p, &a->d.stmt_match, &src->d.stmt_match);
      break;
      case ae_stmt_break:
      case ae_stmt_continue:
        break;
    }
  a->stmt_type = src->stmt_type;
  a->pos = loc_cpy(p, src->pos);
  return a;
}

ANN Func_Def cpy_func_def(MemPool p, const Func_Def src) {
  Func_Def a = mp_calloc(p, Func_Def);
  a->base = cpy_func_base(p, src->base);
  if(!GET_FLAG(src, builtin)) {
    if(src->d.code)
      a->d.code = cpy_stmt(p, src->d.code);
  } else
    a->d.dl_func_ptr = src->d.dl_func_ptr;
  a->pos = loc_cpy(p, src->pos);
  a->flag = src->flag;
  return a;
}

ANN static Stmt_List cpy_stmt_list(MemPool p, const Stmt_List src) {
  Stmt_List a = mp_calloc(p, Stmt_List);
  if(src->next)
    a->next = cpy_stmt_list(p, src->next);
  a->stmt = cpy_stmt(p, src->stmt);
  return a;
}

ANN static Section* cpy_section(MemPool p, const Section *src) {
  Section* a = mp_calloc(p, Section);
  switch(src->section_type) {
    case ae_section_stmt:
      a->d.stmt_list = cpy_stmt_list(p, src->d.stmt_list);
      break;
    case ae_section_class:
      a->d.class_def = cpy_class_def(p, src->d.class_def);
      break;
    case ae_section_func:
      a->d.func_def = cpy_func_def(p, src->d.func_def);
      break;
    case ae_section_enum:
      a->d.enum_def = cpy_enum_def(p, src->d.enum_def);
      break;
    case ae_section_union:
      a->d.union_def = cpy_union_def(p, src->d.union_def);
      break;
    case ae_section_fptr:
      a->d.fptr_def = cpy_fptr_def(p, src->d.fptr_def);
      break;
    case ae_section_type:
      a->d.type_def = cpy_type_def(p, src->d.type_def);
      break;
  }
  a->section_type = src->section_type;
  return a;
}

ANN static Class_Body cpy_class_body(MemPool p, const Class_Body src) {
  Class_Body a = mp_calloc(p, Class_Body);
  a->section = cpy_section(p, src->section);
  if(src->next)
    a->next = cpy_class_body(p, src->next);
  return a;
}

ANN Class_Def cpy_class_def(MemPool p, const Class_Def src) {
  Class_Def a = mp_calloc(p, Class_Def);
  cpy_type_def2(p, &a->base, &src->base);
  if(src->body) {
    if(!GET_FLAG(src, union))
      a->body = cpy_class_body(p, src->body);
    else
      a->list = cpy_decl_list(p, src->list);
  }
  a->flag = src->flag;
  a->pos = loc_cpy(p, src->pos);
  return a;
}
/*
ANN static Ast cpy_ast(MemPool p, const Ast src) {
  Ast a = mp_calloc(p, Ast);
  a->section = cpy_section(p, src->section);
  if(src->next)
    a->next = cpy_ast(p, src->next);
  return a;
}
*/
