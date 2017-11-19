#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "absyn.h"

static void lint_print(FILE* file, const char* fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  vfprintf(file, fmt, arg);
  va_end(arg);
}

#define TABLEN 2

static m_uint indent;
static m_bool skip, nonl;

static void lint_exp(FILE* file, Exp exp);
static void lint_stmt(FILE* file, Stmt stmt);
static void lint_stmt_if(FILE* file, Stmt_If stmt);
static void lint_stmt_list(FILE* file, Stmt_List list);
static void lint_class_def(FILE* file, Class_Def class_def);

static void lint_indent(FILE* file) {
  m_uint i, j;
  if(skip) {
    lint_print(file, " ");
    skip--;
    return;
  }
  for(i = 0; i < indent; i++)
    for(j = 0; j < TABLEN; j++)
      lint_print(file, " ");
}

static void lint_id_list(FILE* file, ID_List list) {
  m_bool next = list->next ? 1 : 0;
  if(next)
    lint_print(file, "<");
  while(list) {
    lint_print(file, s_name(list->xid));
    list = list->next;
    if(list)
      lint_print(file, ".");
  }
  if(next)
    lint_print(file, ">");
}

static void lint_array(FILE* file, Array_Sub array) {
  m_uint i;
  Exp exp = array->exp_list;
  for(i = 0; i < array->depth; i++) {
    Exp tmp = exp ? exp->next : NULL;
    if(exp)
      exp->next = NULL;
    lint_print(file, "[");
    if(exp)
     lint_exp(file, exp);
    lint_print(file, "]");
    if(exp) {
      exp->next = tmp;
      exp = tmp;
    }
  }
}

static void lint_array_lit(FILE* file, Array_Sub array) {
  m_uint i;
  Exp exp = array->exp_list;
  for(i = 0; i < array->depth; i++) {
    Exp tmp = exp ?exp->next : NULL;
    lint_print(file, "[");
    if(exp)
     lint_exp(file, exp); 
    lint_print(file, "]");
    if(exp)
      exp->next = tmp;
  }
}

static void lint_type_decl(FILE* file, Type_Decl* type) {
  if(GET_FLAG(type, ae_flag_static))
    lint_print(file, "static ");
  if(type->xid->ref) {
    lint_print(file, "typeof ");
    lint_id_list(file, type->xid->ref ? type->xid->ref : type->xid);
  } else
    lint_id_list(file, type->xid);
  if(GET_FLAG(type, ae_flag_ref))
    lint_print(file, "@");
  if(type->array)
    lint_array(file, type->array);
}

static void lint_type_list(FILE* file, Type_List list) {
  lint_print(file, "<{");
  while(list) {
    lint_id_list(file, list->list);
    list = list->next;
    if(list)
      lint_print(file, ", ");
  }
  lint_print(file, "}>");
}

static void lint_stmt_indent(FILE* file, Stmt stmt) {
  if(stmt->type == ae_stmt_if) {
    lint_print(file, " ");
    lint_stmt_if(file, &stmt->d.stmt_if);
    return;
  } else if(stmt->type != ae_stmt_code) {
    lint_print(file, "\n");
    indent++;
  }/* else
    lint_print(file, " "); */
  lint_stmt(file, stmt);
  if(stmt->type != ae_stmt_code)
    indent--;
}

static void lint_exp_decl(FILE* file, Exp_Decl* decl) {
  Var_Decl_List list = decl->list;
  if(decl->types)
    lint_type_list(file, decl->types);
  lint_type_decl(file, decl->type);
  lint_print(file, " ");
  while(list) {
    lint_print(file, s_name(list->self->xid));
    if(list->self->array)
      lint_array(file, list->self->array);
    list = list->next;
    if(list)
      lint_print(file, ", ");
  }
}

static void lint_exp_unary(FILE* file, Exp_Unary* unary) {
  lint_print(file, "%s", op2str(unary->op));
  switch(unary->op) {
    case op_plusplus:
    case op_minusminus:
      lint_exp(file, unary->exp);
      break;
    case op_new:
      lint_print(file, " ");
      lint_type_decl(file, unary->type);
      lint_array(file, unary->array);
    case op_spork:
      lint_print(file, " ~");
      if(unary->code)
        lint_stmt(file, unary->code);
      else
        lint_exp(file, unary->exp);
    default:
      break;
  }
}

static void lint_exp_binary(FILE* file, Exp_Binary* binary) {
  lint_exp(file, binary->lhs);
  lint_print(file, " %s ", op2str(binary->op));
  lint_exp(file, binary->rhs);
}

static void lint_exp_primary(FILE* file, Exp_Primary* exp) {
  switch(exp->type) {
    case ae_primary_id:
      lint_print(file, "%s", s_name(exp->d.var));
      break;
    case ae_primary_num:
      lint_print(file, "%li", exp->d.num);
      break;
    case ae_primary_float:
      lint_print(file, "%.g", exp->d.fnum);
      if(!(exp->d.fnum - floor(exp->d.fnum)))
        lint_print(file, ".0");
      break;
    case ae_primary_str:
      lint_print(file, "\"%s\"", exp->d.str);
      break;
    case ae_primary_array:
      lint_array_lit(file, exp->d.array);
      break;
    case ae_primary_hack:
      lint_print(file, "<<<");
      lint_exp(file, exp->d.exp);
      lint_print(file, ">>>");
      break;
    case ae_primary_complex:
      lint_print(file, "#(");
      lint_exp(file, exp->d.cmp->re);
      lint_print(file, ")");
      break;
    case ae_primary_polar:
      lint_print(file, "%(");
      lint_exp(file, exp->d.polar->mod);
      lint_print(file, ")");
      break;
    case ae_primary_vec:
      lint_print(file, "@(");
      lint_exp(file, exp->d.vec->args);
      lint_print(file, ")");
      break;
    case ae_primary_char:
      lint_print(file, "'%s'", exp->d.chr);
      break;
    case ae_primary_nil:
      lint_print(file, "()");
      break;
  }
}
static void lint_exp_array(FILE* file, Exp_Array* array) {
  lint_exp(file, array->base);
  lint_exp(file, array->indices->exp_list);
 }

static void lint_exp_cast(FILE* file, Exp_Cast* cast) {
  lint_exp(file, cast->exp);
}

static void lint_exp_postfix(FILE* file, Exp_Postfix* postfix) {
  lint_exp(file, postfix->exp);
  lint_print(file, "%s", op2str(postfix->op));
}

static void lint_exp_dur(FILE* file, Exp_Dur* dur) {
  lint_exp(file, dur->base);
  lint_print(file, "::");
  lint_exp(file, dur->unit);
}

static void lint_exp_call(FILE* file, Exp_Func* exp_func) {
  if(exp_func->types)
    lint_type_list(file, exp_func->types);
  lint_exp(file, exp_func->func);
  lint_print(file, "(");
  if(exp_func->args)
   lint_exp(file, exp_func->args);
  lint_print(file, ")");
} 

static void  lint_exp_dot(FILE* file, Exp_Dot* member) {
  lint_exp(file, member->base);
  lint_print(file, ".%s", s_name(member->xid));
}

static void lint_exp_if(FILE* file, Exp_If* exp_if) {
  lint_exp(file, exp_if->cond);
  lint_print(file, " ? ");
  lint_exp(file, exp_if->if_exp);
  lint_print(file, " : ");
  lint_exp(file, exp_if->else_exp);
}

static void lint_exp(FILE* file,  Exp exp) {
  while(file, exp) {
    switch(file, exp->exp_type) {
      case ae_exp_primary:
        lint_exp_primary(file, &exp->d.exp_primary);
        break;
      case ae_exp_decl:
        lint_exp_decl(file, &exp->d.exp_decl);
        break;
      case ae_exp_unary:
        lint_exp_unary(file, &exp->d.exp_unary);
        break;
      case ae_exp_binary:
        lint_exp_binary(file, &exp->d.exp_binary);
        break;
      case ae_exp_postfix:
        lint_exp_postfix(file, &exp->d.exp_postfix);
        break;
      case ae_exp_cast:
        lint_exp_cast(file, &exp->d.exp_cast);
        break;
      case ae_exp_call:
        lint_exp_call(file, &exp->d.exp_func);
        break;
      case ae_exp_array:
        lint_exp_array(file, &exp->d.exp_array);
        break;
      case ae_exp_dot:
        lint_exp_dot(file, &exp->d.exp_dot);
        break;
      case ae_exp_dur:
        lint_exp_dur(file, &exp->d.exp_dur);
        break;
      case ae_exp_if:
        lint_exp_if(file, &exp->d.exp_if);
        break;
    }
    exp = exp->next;
    if(exp)
      lint_print(file, ", ");
  }
}

static void lint_stmt_code(FILE* file, Stmt_Code stmt) {
  if(!stmt->stmt_list) {
    lint_print(file, "{}");
    return;
  }
  lint_print(file, "{\n");
  indent++;
  if(stmt->stmt_list)
    lint_stmt_list(file, stmt->stmt_list);
  indent--;
  lint_indent(file);
  lint_print(file, "}");
}

static void lint_stmt_return(FILE* file, Stmt_Return stmt) {
  lint_print(file, "return");
  if(stmt->val) {
    lint_print(file, " ");
    lint_exp(file, stmt->val);
  }
  lint_print(file, ";\n");
}

static void lint_stmt_flow(FILE* file, struct Stmt_Flow_* stmt, m_str str) {
  if(!stmt->is_do) {
    lint_print(file, "%s(", str);
    lint_exp(file, stmt->cond);
    lint_print(file, ")");
    lint_stmt_indent(file, stmt->body);
  } else {
    lint_print(file, "do");
    lint_stmt_indent(file, stmt->body);
    lint_print(file, " %s(", str);
    lint_exp(file, stmt->cond);
    lint_print(file, ")\n");
  }
}
static void lint_stmt_for(FILE* file, Stmt_For stmt) {
  lint_print(file, "for(");
  nonl++;
  lint_stmt(file, stmt->c1);
  lint_print(file, " ");
  lint_stmt(file, stmt->c2);
  lint_print(file, " ");
  lint_exp(file, stmt->c3);
  lint_print(file, ")");
  nonl--;
  lint_stmt_indent(file, stmt->body);
}

static void lint_stmt_loop(FILE* file, Stmt_Loop stmt) {
  lint_print(file, "repeat(");
  lint_exp(file, stmt->cond);
  lint_print(file, ")");
  lint_stmt(file, stmt->body);
}

static void lint_stmt_switch(FILE* file, Stmt_Switch stmt) {
  lint_print(file, "switch(");
  lint_exp(file, stmt->val);
  lint_print(file, ")");
}

static void lint_stmt_case(FILE* file, Stmt_Case stmt) {
  lint_print(file, "case ");
  lint_exp(file, stmt->val);
  lint_print(file, ":\n");
}

static void lint_stmt_if(FILE* file, Stmt_If stmt) {
  lint_print(file, "if(");
  lint_exp(file, stmt->cond);
  lint_print(file, ")");
    if(stmt->else_body->type == ae_stmt_code)
      skip++;
  lint_stmt_indent(file, stmt->if_body);
  if(stmt->else_body) {
    if(stmt->if_body->type != ae_stmt_code) {
//      lint_print(file, "\n");
      lint_indent(file);
    } else
      lint_print(file, " ");
    lint_print(file, "else");
    if(stmt->else_body->type == ae_stmt_code)
      skip++;
    lint_stmt_indent(file, stmt->else_body);
    if(stmt->else_body->type == ae_stmt_code)
      lint_print(file, "\n");
  }
}

void lint_stmt_enum(FILE* file, Stmt_Enum stmt) {
  ID_List list = stmt->list;
  lint_print(file, "enum {\n");
  indent++;
  while(list) {
    lint_indent(file);
    lint_print(file, "%s", s_name(list->xid));
    list = list->next;
    if(list)
      lint_print(file, ",\n");
    else
      lint_print(file, "\n");
  }
  indent--;
  lint_indent(file);
  lint_print(file, "}");
  if(stmt->xid)
    lint_print(file, " %s", s_name(stmt->xid));
  lint_print(file, ";\n");
}

void lint_stmt_fptr(FILE* file, Stmt_Ptr ptr) {
  Arg_List list = ptr->args;
  lint_print(file, "typedef ");
  lint_print(file, "%s", GET_FLAG(ptr->type, ae_flag_variadic) ?
      "variadic " : "function ");
  lint_type_decl(file, ptr->type);
  lint_print(file, " ");
  lint_print(file, s_name(ptr->xid));
  lint_print(file, "(");
  while(list) {
    lint_type_decl(file, list->type_decl);
    lint_print(file, " %s", s_name(list->var_decl->xid));
    list = list->next;
    if(list)
      lint_print(file, ", ");
  }
  lint_print(file, ")\n");
}

void lint_stmt_union(FILE* file, Stmt_Union stmt) {
  Decl_List l = stmt->l;
  lint_print(file, "union {\n");
  indent++;
  while(l) {
    lint_indent(file);
    lint_exp(file, l->self);
    lint_print(file, ";\n");
    l = l->next;
  }
  indent--;
  lint_print(file, "}\n");
}

void lint_stmt_goto(FILE* file, Stmt_Goto_Label stmt) {
  lint_print(file, "goto;\n");
}

void lint_stmt_continue(FILE* file, Stmt_Continue stmt) {
  lint_print(file, "continue;\n");
}

void lint_stmt_break(FILE* file, Stmt_Break stmt) {
  lint_print(file, "break;\n");
}

static void lint_stmt(FILE* file, Stmt stmt) {
  if(stmt->type == ae_stmt_exp && !stmt->d.stmt_exp.val)
    return;
  lint_indent(file);
  switch(file, stmt->type) {
    case ae_stmt_exp:
      lint_exp(file, stmt->d.stmt_exp.val);
      if(nonl)
        lint_print(file, ";");
      else
        lint_print(file, ";\n");
      break;
    case ae_stmt_code:
      lint_stmt_code(file, &stmt->d.stmt_code);
      break;
    case ae_stmt_return:
      lint_stmt_return(file, &stmt->d.stmt_return);
      break;
    case ae_stmt_if:
      lint_stmt_if(file, &stmt->d.stmt_if);
      break;
    case ae_stmt_while:
      lint_stmt_flow(file, &stmt->d.stmt_while, "while");
      break;
    case ae_stmt_for:
      lint_stmt_for(file, &stmt->d.stmt_for);
      break;
    case ae_stmt_until:
      lint_stmt_flow(file, &stmt->d.stmt_until, "until");
      break;
    case ae_stmt_loop:
      lint_stmt_loop(file, &stmt->d.stmt_loop);
      break;
    case ae_stmt_switch:
      lint_stmt_switch(file, &stmt->d.stmt_switch);
      break;
    case ae_stmt_case:
      lint_stmt_case(file, &stmt->d.stmt_case);
      break;
    case ae_stmt_enum:
      lint_stmt_enum(file, &stmt->d.stmt_enum);
      break;
    case ae_stmt_continue:
      lint_stmt_continue(file, &stmt->d.stmt_continue);
      break;
    case ae_stmt_break:
      lint_stmt_break(file, &stmt->d.stmt_break);
      break;
    case ae_stmt_gotolabel:
      lint_stmt_goto(file, &stmt->d.stmt_gotolabel);
      break;
    case ae_stmt_funcptr:
      lint_stmt_fptr(file, &stmt->d.stmt_ptr);
      break;
    case ae_stmt_union:
      lint_stmt_union(file, &stmt->d.stmt_union);
      break;
  }
}

static void lint_stmt_list(FILE* file, Stmt_List list) {
  while(list) {
    lint_stmt(file, list->stmt);
    list = list->next;
  }
}

static void lint_func_def(FILE* file, Func_Def f) {
  Arg_List list = f->arg_list;
  lint_indent(file);
  lint_print(file, "%s", GET_FLAG(f, ae_flag_variadic) ?
      "variadic " : "function ");
  if(GET_FLAG(f, ae_flag_static))
    lint_print(file, "static ");
  lint_type_decl(file, f->type_decl);
  lint_print(file, " ");
  lint_print(file, s_name(f->name));
  lint_print(file, "(");
  while(list) {
    lint_type_decl(file, list->type_decl);
    lint_print(file, " %s", s_name(list->var_decl->xid));
    list = list->next;
    if(list)
      lint_print(file, ", ");
  }
  lint_print(file, ")");
  skip++;
  lint_stmt_indent(file, f->code);
  lint_print(file, "\n");
  f->flag &= ~ae_flag_template;
}

static void lint_section(FILE* file, Section* section) {
  ae_Section_Type t = section->type;
  if(file, t == ae_section_stmt)
    lint_stmt_list(file, section->d.stmt_list);
  else if(file, t == ae_section_func)
    lint_func_def(file, section->d.func_def);
  else if(file, t == ae_section_class)
    lint_class_def(file, section->d.class_def);
}

static void lint_class_def(FILE* file, Class_Def class_def) {
  Class_Body body = class_def->body;
  lint_indent(file);
  if(class_def->types) {
    lint_print(file, "template");
    lint_id_list(file, class_def->types);
  }

  lint_print(file, "class %s", s_name(class_def->name->xid));
  if(class_def->ext) {
    lint_print(file, " extends ");
    lint_id_list(file, class_def->ext);
    lint_print(file, "\n");
  }
  lint_print(file, " {\n");
  indent++;
  while(body) {
    lint_section(file, body->section);
    body = body->next;
  }
  lint_print(file, "}\n\n");
  indent--;
}

void lint_ast(FILE* file, Ast ast) {
  while(file, ast) {
    lint_section(file, ast->section);
    ast = ast->next;
  }
}

int main(int argc, char** argv) {
  argc--; argv++;
  scan_map = new_map();
  while(argc--) {
    char c[strlen(*argv) + 6];
    sprintf(c, "%s.lint", *argv);
    FILE* file = fopen(c, "w");
    Ast ast = parse(*argv++);
    indent = 0;
    lint_ast(file, ast);
    free_ast(ast);
    fclose(file);
  }
  free_map(scan_map);
  free_symbols();
  return 0;
}
