#include <stdlib.h>
#include "absyn.h"

void free_Expression(Expression exp);

int get_pos(void* data)
{
  MyArg* arg = (MyArg*)map_get(scan_map, data);
  return arg->line;
}

Var_Decl new_Var_Decl(m_str name, Array_Sub array, int pos)
{
	Var_Decl a = calloc(1, sizeof(struct Var_Decl_));
  a->xid= insert_symbol(name);
  a->array = array;
  a->pos = pos;
  return a;
}
void free_Var_Decl(Var_Decl a)
{
  free(a);
}
Var_Decl_List new_Var_Decl_List(Var_Decl decl, Var_Decl_List list, int pos)
{
	Var_Decl_List a = calloc(1, sizeof(struct Var_Decl_List_));
  a->self = decl;
  a->next = list;
  a->pos = pos;
  a->doc = NULL;
  return a;
}
void free_Var_Decl_List(Var_Decl_List a)
{
  Var_Decl_List list = a;
  Var_Decl_List tmp;
  while(list)
  {
    free_Var_Decl(list->self);
    tmp = list;
    list = list->next;
    free(tmp);
/*    list = list->next;*/
  }
}
/*Type_Decl* new_Type_Decl(m_str name, int ref, int pos)*/
Type_Decl* new_Type_Decl(ID_List xid, int ref, int pos)
{
  Type_Decl* a = calloc(1, sizeof(Type_Decl));
  a->xid = xid;
  a->ref = ref;
  a->pos = pos;
  a->array = NULL;
  a->dot = NULL;
  a->doc = NULL;
  return a;
}
Type_Decl* new_Type_Decl_from_dot(Dot_Member* dot, int ref, int pos)
{
  Type_Decl* a = calloc(1, sizeof(Type_Decl));
  a->xid = NULL;
  a->ref = ref;
  a->pos = pos;
  a->array = NULL;
  a->dot = dot;
  return a;
}
Array_Sub new_array_sub(Expression exp, int pos)
{
  Array_Sub a = calloc(1, sizeof(struct Array_Sub_));
  a->exp_list = exp;
  a->depth = 1;
  a->err_num = 0;
  a->pos = pos;
  return a;
}

Array_Sub prepend_array_sub(Array_Sub a, Expression exp, int pos)
{
  // if already error
  if(a->err_num)
    goto error;

  // make sure no multi
  if(exp && exp->next)
  {
    a->err_num = 1;            // multi
    a->err_pos = exp->pos; // set error for type-checker
    goto error;
  }

  // empty or not
  if((exp && !a->exp_list) || (!exp && a->exp_list))
  {
    a->err_num = 2;   // partial
    a->err_pos = pos; // set error for type-checker
    goto error;
  }

  // prepend
  if(exp)
  {
    exp->next = a->exp_list;
    a->exp_list = exp;
  }
  // count
  a->depth++;
  return a;

error:
    free_Expression( exp );
    return a;
/*    return NULL;*/
/*    exit(78);*/
}

Type_Decl* add_type_decl_array(Type_Decl* a, Array_Sub array, int pos )
{
  a->array = array;
  return a;
}

Expression new_Array(Expression base, Array_Sub indices, int pos )
{
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Array_Expression_type;
  a->meta = ae_meta_var;
  a->array = calloc(1, sizeof(Array));
  a->array->base = base;
  a->array->indices = indices;
  a->pos = pos;
  a->array->pos = pos;
  a->array->self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

void free_Type_Decl(Type_Decl* a)
{
  free(a);
}

Expression new_Decl_Expression(Type_Decl* type, Var_Decl_List list, m_bool is_static, int pos)
{
	Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Decl_Expression_type;
	a->decl_exp  = calloc(1, sizeof(Decl_Expression));
	a->decl_exp->type = type;
	a->decl_exp->num_decl = 0;
	a->decl_exp->list = list;
	a->decl_exp->is_static = is_static;
  a->pos  = pos;
  a->decl_exp->pos  = pos;
  a->decl_exp->self = a;
  a->next = NULL;
  a->cast_to = NULL;
	return a;
}

void free_Decl_Expression(Decl_Expression* decl)
{
  free_Type_Decl(decl->type);
  free_Var_Decl_List(decl->list);
  free(decl);
}


Expression new_Binary_Expression(Expression lhs, Operator op, Expression rhs, int pos)
{
  Expression a = calloc(1,  sizeof( struct Expression_ ) );
  a->exp_type = Binary_Expression_type;
  a->meta = ae_meta_value;
  a->binary_exp = calloc(1, sizeof(Binary_Expression));
  a->binary_exp->lhs = lhs;
  a->binary_exp->op = op;
  a->binary_exp->rhs = rhs;
  a->pos = pos;
  a->binary_exp->pos = pos;
  a->binary_exp->self = a;
  a->next = NULL;
  a->cast_to = NULL;
	return a;
}

void free_Binary_Expression(Binary_Expression* binary)
{
  free_Expression(binary->lhs);
/*  free_Expression(binary->rhs);*/
/*  free_Expression(binary->self);*/
  free(binary);
}
Expression new_Cast_Expression(Type_Decl* type, Expression exp, int pos)
{
  Expression a = calloc(1,  sizeof( struct Expression_ ) );
  a->exp_type = Cast_Expression_type;
  a->meta = ae_meta_value;
  a->cast_exp = calloc(1, sizeof(Cast_Expression));
  a->cast_exp->type = type;
  a->pos = pos;
  a->cast_exp->pos = pos;
  a->cast_exp->exp = exp;
  a->cast_exp->self = a;
  a->next = NULL;
  a->cast_to = NULL;
	return a;
}
Expression new_Postfix_Expression(Expression exp, Operator op, int pos)
{
  Expression a = calloc(1,  sizeof( struct Expression_ ) );
  a->exp_type = Postfix_Expression_type;
  a->meta = ae_meta_var;
  a->postfix_exp = calloc(1, sizeof(Postfix_Expression));
  a->postfix_exp->exp = exp;
  a->postfix_exp->op = op;
  a->pos = pos;
  a->postfix_exp->pos = pos;
  a->postfix_exp->self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

void free_Postfix_Expression(Postfix_Expression* postfix)
{
  free_Expression(postfix->exp);
/*  free_Expression(postfix->self);*/
  free(postfix);
}
Expression new_Exp_Dur(Expression base, Expression unit, int pos)
{
  Expression a = calloc(1,  sizeof( struct Expression_ ) );
  a->exp_type = Dur_Expression_type;
  a->meta = ae_meta_value;
  a->postfix_exp = calloc(1, sizeof(Exp_Dur));
  a->dur->base = base;
  a->dur->unit = unit;
  a->pos = pos;
  a->dur->pos = pos;
  a->dur->self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

void free_Dur_Expression(Exp_Dur* a)
{
  free_Expression(a->base);
  free_Expression(a->unit);
  free(a);
}

Expression new_Primary_Expression_from_int(long i, int pos)
{
	Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->emit_var = 0;
  a->primary_exp = calloc(1, sizeof(Primary_Expression));
  a->primary_exp->type = ae_primary_num;
  a->primary_exp->num = i;
  a->pos =pos;
  a->primary_exp->pos = pos;
  a->primary_exp->self = a;
  a->next = NULL;
  a->cast_to = NULL;
	return a;
}
Expression new_Primary_Expression_from_float(double num, int pos)
{
  Expression a = calloc(1,  sizeof( struct Expression_ ) );
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->emit_var = 0; // ???
  a->primary_exp = calloc(1, sizeof(Primary_Expression));
  a->primary_exp->type = ae_primary_float;
  a->primary_exp->fnum = num;
  a->pos = pos;
  a->primary_exp->pos = pos;
  a->primary_exp->self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Expression new_Primary_Expression_from_string(m_str s, int pos)
{
  Expression a = calloc(1,  sizeof( struct Expression_ ) );
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->emit_var = 0; // ???
  a->primary_exp = calloc(1, sizeof(Primary_Expression));
  a->primary_exp->type = ae_primary_str;
  a->primary_exp->str = s;
  a->pos = pos;
  a->primary_exp->pos = pos;
  a->primary_exp->self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Expression new_Primary_Expression_from_nil( int pos )
{
  Expression  a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->primary_exp = calloc(1, sizeof(Primary_Expression));
  a->primary_exp->type = ae_primary_nil;
  a->primary_exp->pos = pos;
  a->pos = pos;
  a->primary_exp->self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Expression new_Primary_Expression_from_ID(m_str s, int pos)
{
	Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_var;
  a->emit_var = 0;
  a->primary_exp = calloc(1, sizeof(Primary_Expression));
  a->primary_exp->type = ae_primary_var;
  a->primary_exp->var = insert_symbol(s);
  a->pos = pos;
  a->primary_exp->pos = pos;
  a->primary_exp->self = a;
  a->next = NULL;
  a->cast_to = NULL;
	return a;
}

Expression new_Hack_Expression(Expression exp, int pos)
{
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->primary_exp = calloc(1, sizeof(Primary_Expression));
  a->primary_exp->type = ae_primary_hack;
  a->primary_exp->exp = exp;
  a->primary_exp->pos = pos;
  a->pos = pos;
  a->primary_exp->self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Complex* new_complex( Expression re, int pos )
{
  Complex* a = calloc(1, sizeof(Complex));
  a->re = re;
  if(re)
    a->im = re->next;
  a->pos = pos;
  return a;
}

Expression new_exp_from_char( c_str chr, int pos )
{
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->primary_exp = calloc(1, sizeof(Primary_Expression));
  a->primary_exp->type = ae_primary_char;
  a->primary_exp->chr = chr;
  a->pos = pos;
  a->primary_exp->pos = pos;
  a->primary_exp->self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Expression new_exp_from_array_lit(Array_Sub exp_list, int pos )
{
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->primary_exp = calloc(1, sizeof(Primary_Expression));
  a->primary_exp->type = ae_primary_array;
  a->primary_exp->array = exp_list;
  a->pos = pos;
  a->primary_exp->pos = pos;
  a->primary_exp->self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}
/*
Expression new_exp_from_array(Expression base, Array_Sub indices, int pos)
{
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = ae_exp_array;
  a->meta = ae_meta_var;
  a->array = calloc(1, sizeof(struct Array_Expression_));
  a->array->base = base;
  a->array->indices = indices;
  a->pos = pos;
  a->array->pos = pos;
  a->array->self = a;
  return a;
}
*/
Expression new_exp_from_complex(Complex* exp, int pos)
{
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->primary_exp = calloc(1, sizeof(Primary_Expression));
  a->primary_exp->type = ae_primary_complex;
  a->primary_exp->cmp = exp;
  a->primary_exp->pos = pos;
  a->pos = pos;
  a->primary_exp->cmp->self = a;
  a->primary_exp->self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Expression new_exp_from_polar(Polar* exp, int pos)
{
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Primary_Expression_type;
  a->meta = ae_meta_value;
  a->primary_exp = calloc(1, sizeof(Polar));
  a->primary_exp->type = ae_primary_polar;
  a->primary_exp->polar = exp;
  a->primary_exp->pos = pos;
  a->pos = pos;
  a->primary_exp->polar->self = a;
  a->primary_exp->self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Polar* new_polar(Expression mod, int pos)
{
  Polar* a = calloc(1, sizeof(Polar));
  a->mod = mod;
  if(mod)
    a->phase = mod->next;
  a->pos = pos;  
  return a;
}
Expression new_exp_from_unary(Operator oper, Expression exp, int pos)
{
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Unary_Expression_type;
  a->meta = exp->meta;
  a->unary = calloc(1, sizeof(Unary_Expression));
  a->unary->op = oper;
  a->unary->exp = exp;
  a->unary->code = NULL;
  a->pos = pos;
  a->unary->pos = pos;
  a->unary->self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}
Expression new_exp_from_unary2(Operator oper, Type_Decl* type, Array_Sub array, int pos )
{
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Unary_Expression_type;
  a->meta = ae_meta_value;
  a->unary = calloc(1, sizeof(Unary_Expression));
  a->unary->exp = NULL;
  a->unary->op = oper;
  a->unary->type = type;
  a->unary->array = array;
  a->unary->code = NULL;
  a->pos = pos;
  a->unary->pos = pos;
  a->unary->self = a;
  a->next = NULL;
  a->cast_to = NULL;
printf("New!!! %p\n", a);
  return a;
}

void free_Unary_Expression(Unary_Expression* unary)
{
  free(unary);
}
Expression new_exp_from_unary3(Operator oper, Stmt* code, int pos )
{
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Unary_Expression_type;
  a->meta = ae_meta_value;
  a->unary = calloc(1, sizeof(Unary_Expression));
  a->unary->exp = NULL;
  a->unary->op = oper;
  ID_List id = new_id_list("void", pos);
  a->unary->type = new_Type_Decl(id, 0, pos); // put it to use real type ?
/*  a->unary->type->m_type = &t_void;*/
  a->unary->array = NULL;
  a->unary->code = code;
  a->pos = pos;
  a->unary->pos = pos;
  a->unary->self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

Expression new_If_Expression(Expression cond, Expression if_exp, Expression else_exp, int pos )
{
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = If_Expression_type;
  a->meta = ( ( if_exp->meta == ae_meta_var && 
      else_exp->meta == ae_meta_var ) ? ae_meta_var : ae_meta_value );
  a->exp_if = calloc(1, sizeof(If_Expression));
  a->exp_if->cond = cond;
  a->exp_if->if_exp = if_exp;
  a->exp_if->else_exp = else_exp;
  a->pos = pos;
  a->exp_if->pos = pos;
  a->exp_if->self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}
Func_Def new_Func_Def(ae_Keyword func_decl, ae_Keyword static_decl, Type_Decl* type_decl, m_str name, Arg_List arg_list, Stmt* code, int pos)
{
  Func_Def a = calloc(1, sizeof(struct Func_Def_));
  a->func_decl = func_decl = func_decl;
  a->static_decl = static_decl;
  a->type_decl = type_decl;
  a->name = insert_symbol(name);
  a->arg_list = arg_list;
  a->s_type = ae_func_user;
  a->code = code;
  a->func = NULL;
  a->stack_depth = 0;
  a->pos = pos;
//	a->dl_func_ptr = NULL;
  a->spec = ae_func_spec_none;
  if(a->func_decl == ae_key_variadic)
  {
    a->func_decl = ae_key_func;
    a->is_variadic = 1;
  }
  return a;
}

void free_Func_Def(Func_Def func)
{
  free(func);
}

Stmt* new_Func_Ptr_Stmt(ae_Keyword key, m_str xid, Type_Decl* decl, Arg_List args, int pos)
{
  Stmt* a                = calloc(1, sizeof(Stmt));
  a->type                = ae_stmt_funcptr;
	a->stmt_funcptr        = calloc(1, sizeof(Func_Ptr));
	a->stmt_funcptr->key   = key;
	a->stmt_funcptr->type  = decl;
	a->stmt_funcptr->xid   = insert_symbol(xid);
	a->stmt_funcptr->args  = args;
	a->stmt_funcptr->func  = NULL; // get rid of that
	a->stmt_funcptr->value = NULL;
	a->stmt_funcptr->pos   = pos;
/*	a->stmt_funcptr->self  = a; // is it necessary ?*/
	a->pos             = pos;
/*  a->next = NULL;*/
/*  a->cast_to = NULL;*/
	return a;

}
Expression new_Func_Call(Expression base, Expression args, int pos )
{
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Func_Call_type;
  a->meta = ae_meta_value;
  a->func_call = calloc(1, sizeof(Func_Call));
  a->func_call->func = base;
  a->func_call->args = args;
  a->pos = pos;
  a->func_call->m_func = NULL;
  a->func_call->pos = pos;
  a->func_call->self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

void free_Func_Call(Func_Call* call)
{
/*  free_Expression(call->func);*/
  if(call->args)
    free_Expression(call->args);
  free(call);
}
Expression new_exp_from_member_dot(Expression base, m_str xid, int pos)
{
  Expression a = calloc(1, sizeof(struct Expression_));
  a->exp_type = Dot_Member_type;
  a->meta = ae_meta_var;
  a->dot_member = calloc(1, sizeof(Dot_Member));
  a->dot_member->base = base;
  a->dot_member->xid = insert_symbol( xid );
  a->pos = pos;
  a->dot_member->pos = pos;
  a->dot_member->self = a;
  a->next = NULL;
  a->cast_to = NULL;
  return a;
}

void free_Dot_Member_Expression(Dot_Member* dot)
{
  if(dot->base)
  free_Expression(dot->base);
  free_Expression(dot->self);
  free(dot);
}
Expression prepend_Expression(Expression exp, Expression next, int pos)
{
  exp->next = next;
  return exp;
}
void free_Primary_Expression(Primary_Expression* primary)
{
  free(primary);
}

void free_Expression(Expression exp)
{
  Expression curr = exp;
	while(curr)
	{
		switch(curr->exp_type)
		{
			case Decl_Expression_type:
        free_Decl_Expression(curr->decl_exp);
        break;
			case Binary_Expression_type:
        free_Binary_Expression(curr->binary_exp);
        break;
			case Unary_Expression_type:
        free_Unary_Expression(curr->unary);
        break;
			case Primary_Expression_type:
        free_Primary_Expression(curr->primary_exp);
        break;
			case Func_Call_type:
        free_Func_Call(curr->func_call);
        break;
			case Postfix_Expression_type:
        free_Postfix_Expression(curr->postfix_exp);
        break;
			case Dot_Member_type:
        free_Dot_Member_Expression(curr->dot_member);
        break;
			case Dur_Expression_type:
        free_Dur_Expression(curr->dur);
        break;
      default:
printf("free Expression: unhandled exp type %i\n", curr->exp_type);
        break;
    }
    curr = curr->next;
  }
}
/* FUNC */
Arg_List new_Arg_List(Type_Decl* type_decl, Var_Decl var_decl, Arg_List arg_list, int pos)
{
  Arg_List a = calloc(1, sizeof(struct Arg_List_));
  a->type_decl = type_decl;
  a->var_decl = var_decl;
  a->next = arg_list;
  a->doc = NULL;
  a->pos = pos;
  return a;
}
void free_Arg_List(Arg_List a)
{
  if(a->next)
    free_Arg_List(a->next);
  free_Type_Decl(a->type_decl);
  free_Var_Decl(a->var_decl);
  free(a);

}
Stmt* new_Stmt_from_Expression(Expression exp, int pos)
{
  Stmt* stmt = calloc(1, sizeof(Stmt));
  stmt->type = ae_stmt_exp;
  stmt->stmt_exp = exp;
  stmt->pos = pos;
  return stmt;
}

Stmt* new_stmt_from_code(Stmt_List stmt_list, int pos )
{
    Stmt* a = calloc(1, sizeof(Stmt));
    a->type = ae_stmt_code;
    a->stmt_code = calloc(1, sizeof(struct Stmt_Code_));
    a->stmt_code->stmt_list = stmt_list;
    a->pos = pos;
    a->stmt_code->pos = pos;
    a->stmt_code->self = a;

    return a;
}

Stmt* new_stmt_from_return(Expression exp, int pos)
{
  Stmt* a = calloc(1, sizeof(Stmt));
  a->type = ae_stmt_return;
  a->stmt_return = calloc(1, sizeof(struct Stmt_Return_));
  a->stmt_return->val = exp;
  a->pos = pos;
  a->stmt_return->pos = pos;
  a->stmt_return->self = a;
  return a;
}

Stmt* new_stmt_from_break(int pos)
{
  Stmt* a = calloc(1, sizeof(Stmt));
  a->type = ae_stmt_break;
  a->stmt_break = calloc(1, sizeof(struct Stmt_Break_));
  a->pos = pos;
  a->stmt_break->pos = pos;
  a->stmt_break->self = a;
  return a;
}

Stmt* new_stmt_from_continue( int pos )
{
  Stmt* a = calloc(1, sizeof(Stmt));
  a->type = ae_stmt_continue;
  a->stmt_return = calloc(1, sizeof(struct Stmt_Continue_));
  a->pos = pos;
  a->stmt_continue->pos = pos;
  a->stmt_continue->self = a;
  return a;
}

Stmt* new_stmt_from_while(Expression cond, Stmt* body, m_bool is_do, int pos)
{
  Stmt* a = calloc(1, sizeof(Stmt));
  a->type = ae_stmt_while;
  a->stmt_while = calloc(1, sizeof(struct Stmt_While_));
  a->stmt_while->is_do = is_do;
  a->stmt_while->cond = cond;
  a->stmt_while->body = body;
  a->pos = pos;
  a->stmt_while->pos = pos;
  a->stmt_while->self = a;
  return a;
}
Stmt* new_stmt_from_until(Expression cond, Stmt* body, m_bool is_do, int pos)
{
  Stmt* a = calloc(1, sizeof(Stmt));
  a->type = ae_stmt_until;
  a->stmt_until = calloc(1, sizeof(struct Stmt_Until_));
  a->stmt_until->is_do = 0;
  a->stmt_until->cond = cond;
  a->stmt_until->body = body;
  a->pos = pos;
  a->stmt_until->pos = pos;
  a->stmt_until->self = a;
  return a;
}

Stmt* new_stmt_from_for(Stmt* c1, Stmt* c2, Expression c3, Stmt* body, int pos)
{
  Stmt* a = calloc(1, sizeof(Stmt));
  a->type = ae_stmt_for;
  a->stmt_for = calloc(1, sizeof(struct Stmt_For_));
  a->stmt_for->c1 = c1;
  a->stmt_for->c2 = c2;
  a->stmt_for->c3 = c3;
  a->stmt_for->body = body;
  a->pos = pos;
  a->stmt_for->pos = pos;
  a->stmt_for->self = a;
  return a;
}

Stmt* new_stmt_from_gotolabel(m_str xid, m_bool is_label, int pos)
{
  Stmt* a = calloc(1, sizeof(Stmt));
  a->type = ae_stmt_gotolabel;
  a->stmt_gotolabel = calloc(1, sizeof(struct Stmt_Goto_Label_));
  a->stmt_gotolabel->name = insert_symbol( xid );
  a->stmt_gotolabel->is_label = is_label;
  a->pos = pos;
  a->stmt_gotolabel->pos = pos;
  a->stmt_gotolabel->self = a;
  return a;
}

Stmt* new_stmt_from_loop(Expression cond, Stmt* body, int pos)
{
  Stmt* a = calloc(1, sizeof(Stmt));
  a->type = ae_stmt_loop;
  a->stmt_loop = calloc(1, sizeof(struct Stmt_Loop_));
  a->stmt_loop->cond = cond;
  a->stmt_loop->body = body;
  a->pos = pos;
  a->stmt_loop->pos = pos;
  a->stmt_loop->self = a;
  return a;
}
Stmt* new_stmt_from_if(Expression cond, Stmt* if_body, Stmt* else_body, int pos )
{
  Stmt* a = calloc(1, sizeof(Stmt));
  a->type = ae_stmt_if;
  a->stmt_if = calloc(1, sizeof(struct Stmt_If_));
  a->stmt_if->cond = cond;
  a->stmt_if->if_body = if_body;
  a->stmt_if->else_body = else_body;
  a->pos = pos;
  a->stmt_if->pos = pos;
  a->stmt_if->self = a;
  return a;
}

Stmt* new_stmt_from_switch(Expression val, Stmt* stmt, int pos)
{
  Stmt* a = calloc(1, sizeof(Stmt));
  a->type = ae_stmt_switch;
  a->stmt_switch = calloc(1, sizeof(struct Stmt_Switch_));
  a->stmt_switch->val = val;
  a->stmt_switch->stmt = stmt;
  a->pos = pos;
  a->stmt_switch->pos = pos;
  a->stmt_switch->self = a;
  return a;
}

Stmt* new_stmt_from_case(Expression val, int pos)
{
  Stmt* a = calloc(1, sizeof(Stmt));
  a->type = ae_stmt_case;
  a->stmt_case= calloc(1, sizeof(struct Stmt_Case_));
  a->stmt_case->val = val;
  a->pos = pos;
  a->stmt_case->pos = pos;
  a->stmt_case->self = a;
  return a;
}

Stmt* new_stmt_from_enum(ID_List list, m_str xid, int pos)
{
  Stmt* a = calloc(1, sizeof(Stmt));
  a->type = ae_stmt_enum;
  a->stmt_enum = calloc(1, sizeof(struct Stmt_Enum_));
  a->stmt_enum->xid = xid ? insert_symbol(xid) : NULL;
  a->stmt_enum->list = list;
  a->stmt_enum->values = new_Vector();
  a->pos = pos;
  a->stmt_enum->pos = pos;
  a->stmt_enum->self = a;
  return a;  
}
Stmt* new_stmt_from_Union(Union* u, int pos)
{
  Stmt* a = calloc(1, sizeof(Stmt));
  a->type = ae_stmt_union;
  a->stmt_union = u;
  a->pos = pos;
  return a;
}

Decl_List new_Decl_List(Decl_Expression* d, Decl_List l)
{
  Decl_List a = malloc(sizeof(struct Decl_List_));
  a->self = d;
  a->next = l;
  return a;
}
Union* new_Union(Decl_List l)
{
  Union* a = malloc(sizeof(Union));
  a->l = l;
  a->v = new_Vector();
  a->s = 0;
}

void free_Stmt(Stmt* stmt)
{
  switch(stmt->type)
  {
    case ae_stmt_exp:
      free_Expression(stmt->stmt_exp);
      break;
  }
  free(stmt);
}



Stmt_List new_Stmt_List(Stmt* stmt, Stmt_List next, int pos)
{
  Stmt_List list = calloc(1, sizeof(struct Stmt_List_));
  list->stmt = stmt;
  list->next = next;
  return list;
}

void free_Stmt_List(Stmt_List list)
{
  Stmt_List tmp;
	while(list)
	{
    tmp = list;
    list = list->next;
    free_Stmt(tmp->stmt);
    free(tmp);
  }
}



Section* new_section_Stmt_List(Stmt_List list, int pos)
{
  Section* section = calloc(1, sizeof(Section));
  section->type = ae_section_stmt;
  section->stmt_list = list;
  section->pos = pos;
  return section;
}

Section* new_section_Func_Def( Func_Def func_def, int pos )
{
  Section* a = calloc(1, sizeof(Section));
  a->type = ae_section_func;
  a->func_def = func_def;
  a->pos = pos;
  return a;
}

void free_Section(Section* section)
{
  switch(section->type)
  {
/*    case ae_section_class:*/
/*      free_Class_Def(section->class_def);*/
/*      break;*/
/*    case ae_section_stmt:*/
/*      free_Stmt_List(section->stmt_list);*/
/*      break;*/
/*    case ae_section_stmt:*/
/*      free_Stmt_Func_Def(section->stmt_func_def);*/
/*      break;*/
  }
  free(section);
  section = NULL;
}

Class_Def new_class_def( ae_Keyword class_decl, ID_List name,
  Class_Ext ext, Class_Body body, int pos )
{
  Class_Def a = calloc(1, sizeof(struct Class_Def_));
  a->decl = class_decl;
  a->name = name;
  a->ext  = ext;
  a->home = NULL;
  a->body = body;
  a->pos  = pos;
  a->doc  = NULL;
  return a;
}

Class_Def new_iface_def( ae_Keyword class_decl, ID_List name,
  Class_Ext ext, Class_Body body, int pos )
{
  Class_Def a = new_class_def( class_decl, name, ext, body, pos );
  a->iface = 1;
  return a;
}

Class_Body new_class_body( Section* section, int pos )
{
  Class_Body a = calloc(1, sizeof(struct Class_Body_));
  a->section = section;
  a->pos = pos;
  a->next = NULL;
  return a;
}

Class_Body prepend_class_body(Section* section, Class_Body body, int pos )
{
  Class_Body a = new_class_body( section, pos );
  a->next = body;
  a->pos = pos;
  return a;
}

Class_Ext new_class_ext(ID_List extend_id, ID_List impl_list, int pos )
{
  Class_Ext a = calloc(1, sizeof(struct Class_Ext_));
  a->extend_id = extend_id;
  a->impl_list = impl_list;
  a->pos = pos;
  return a;
}


ID_List new_id_list(const m_str xid, int pos)
{
  ID_List a = calloc(1,  sizeof( struct ID_List_ ) );
  a->xid = insert_symbol( xid );
  a->next = NULL;
  a->pos = pos;
  return a;
}

ID_List prepend_id_list(const m_str xid, ID_List list, int pos)
{

  ID_List a = new_id_list(xid, pos);
  a->next = list;
  a->pos = pos;
  return a;
}

Type_List new_type_list(ID_List list, Type_List next, int pos)
{
  Type_List a = calloc(1,  sizeof(struct Type_List_));
  a->list = list;
  a->next = next;
  a->pos = pos;
  return a;
}
void free_ID_List(ID_List a)
{
#ifdef DEBUG_AST
  debug_msg("emit", "free_ID_LIST");
#endif
  ID_List curr = a, next = a->next, tmp;
  while(curr)
  {
    tmp = curr;
    curr = next;
    next = tmp->next;
    free(tmp);
  }
  a = NULL;
}

Section* new_section_Class_Def(Class_Def class_def, int pos )
{
#ifdef DEBUG_AST
  debug_msg("emit", "class def");
#endif
  Section* a = calloc(1, sizeof(Section));
  a->type = ae_section_class;
  a->class_def = class_def;
  a->pos = pos;
  return a;
}

Ast new_Ast(Section* section, Ast next, int pos)
{
#ifdef DEBUG_AST
  debug_msg("emit", "new Ast");
#endif
  Ast ast = calloc(1, sizeof(struct Ast_));
  ast->section = section;
  ast->next = next;
  ast->pos = pos;
  return ast;
}

void free_Ast(Ast ast)
{
#ifdef DEBUG_AST
  debug_msg("ast", "free AST");
#endif
  Ast tmp;
	while(ast)
	{
    tmp = ast;
    ast = ast->next;
    free_Section(tmp->section);
  }
  free(ast);
}
