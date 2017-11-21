#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "absyn.h"

#define TABLEN 2

extern m_str op2str(Operator);

typedef struct {
  FILE*  file;
  m_uint indent;
  m_bool skip;
  m_bool nonl;
} Linter;

static void lint_exp(Linter* linter, Exp exp);
static void lint_stmt(Linter* linter, Stmt stmt);
static void lint_stmt_if(Linter* linter, Stmt_If stmt);
static void lint_stmt_list(Linter* linter, Stmt_List list);
static void lint_class_def(Linter* linter, Class_Def class_def);

static void lint_print(Linter* linter, const char* fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  vfprintf(linter->file, fmt, arg);
  va_end(arg);
}

static void lint_indent(Linter* linter) {
  m_uint i, j;
  if(linter->skip) {
    lint_print(linter, " ");
    linter->skip--;
    return;
  }
  for(i = 0; i < linter->indent; i++)
    for(j = 0; j < TABLEN; j++)
      lint_print(linter, " ");
}

static void lint_id_list(Linter* linter, ID_List list) {
  m_bool next = list->next ? 1 : 0;
  if(next)
    lint_print(linter, "<");
  while(list) {
    lint_print(linter, s_name(list->xid));
    list = list->next;
    if(list)
      lint_print(linter, ".");
  }
  if(next)
    lint_print(linter, ">");
}

static void lint_array(Linter* linter, Array_Sub array) {
  m_uint i;
  Exp exp = array->exp_list;
  for(i = 0; i < array->depth; i++) {
    Exp tmp = exp ? exp->next : NULL;
    if(exp)
      exp->next = NULL;
    lint_print(linter, "[");
    if(exp)
     lint_exp(linter, exp);
    lint_print(linter, "]");
    if(exp) {
      exp->next = tmp;
      exp = tmp;
    }
  }
}

static void lint_array_lit(Linter* linter, Array_Sub array) {
  m_uint i;
  Exp exp = array->exp_list;
  for(i = 0; i < array->depth; i++) {
    Exp tmp = exp ?exp->next : NULL;
    lint_print(linter, "[");
    if(exp)
     lint_exp(linter, exp);
    lint_print(linter, "]");
    if(exp)
      exp->next = tmp;
  }
}

static void lint_type_decl(Linter* linter, Type_Decl* type) {
  if(GET_FLAG(type, ae_flag_static))
    lint_print(linter, "static ");
  if(type->xid->ref) {
    lint_print(linter, "typeof ");
    lint_id_list(linter, type->xid->ref ? type->xid->ref : type->xid);
  } else
    lint_id_list(linter, type->xid);
  if(GET_FLAG(type, ae_flag_ref))
    lint_print(linter, "@");
  if(type->array)
    lint_array(linter, type->array);
}

static void lint_type_list(Linter* linter, Type_List list) {
  lint_print(linter, "<{");
  while(list) {
    lint_id_list(linter, list->list);
    list = list->next;
    if(list)
      lint_print(linter, ", ");
  }
  lint_print(linter, "}>");
}

static void lint_stmt_indent(Linter* linter, Stmt stmt) {
  if(stmt->type == ae_stmt_if) {
    lint_print(linter, " ");
    lint_stmt_if(linter, &stmt->d.stmt_if);
    return;
  } else if(stmt->type != ae_stmt_code) {
    lint_print(linter, "\n");
    linter->indent++;
  } else if(!linter->skip)
      lint_print(linter, " ");
  lint_stmt(linter, stmt);
  if(stmt->type != ae_stmt_code)
    linter->indent--;
}

static void lint_exp_decl(Linter* linter, Exp_Decl* decl) {
  Var_Decl_List list = decl->list;
  if(decl->types)
    lint_type_list(linter, decl->types);
  lint_type_decl(linter, decl->type);
  lint_print(linter, " ");
  while(list) {
    lint_print(linter, s_name(list->self->xid));
    if(list->self->array)
      lint_array(linter, list->self->array);
    list = list->next;
    if(list)
      lint_print(linter, ", ");
  }
}

static void lint_exp_unary(Linter* linter, Exp_Unary* unary) {
  lint_print(linter, "%s", op2str(unary->op));
  switch(unary->op) {
    case op_plusplus:
    case op_minusminus:
      lint_exp(linter, unary->exp);
      break;
    case op_new:
      lint_print(linter, " ");
      lint_type_decl(linter, unary->type);
      lint_array(linter, unary->array);
      break;
    case op_spork:
      lint_print(linter, " ~");
      if(unary->code)
        lint_stmt(linter, unary->code);
      else
        lint_exp(linter, unary->exp);
  }
}

static void lint_exp_binary(Linter* linter, Exp_Binary* binary) {
  lint_exp(linter, binary->lhs);
  lint_print(linter, " %s ", op2str(binary->op));
  lint_exp(linter, binary->rhs);
}

static void lint_exp_primary(Linter* linter, Exp_Primary* exp) {
  switch(exp->type) {
    case ae_primary_id:
      lint_print(linter, "%s", s_name(exp->d.var));
      break;
    case ae_primary_num:
      lint_print(linter, "%li", exp->d.num);
      break;
    case ae_primary_float:
      lint_print(linter, "%.g", exp->d.fnum);
      if(!(exp->d.fnum - floor(exp->d.fnum)))
        lint_print(linter, ".0");
      break;
    case ae_primary_str:
      lint_print(linter, "\"%s\"", exp->d.str);
      break;
    case ae_primary_array:
      lint_array_lit(linter, exp->d.array);
      break;
    case ae_primary_hack:
      lint_print(linter, "<<<");
      lint_exp(linter, exp->d.exp);
      lint_print(linter, ">>>");
      break;
    case ae_primary_complex:
      lint_print(linter, "#(");
      lint_exp(linter, exp->d.cmp->re);
      lint_print(linter, ")");
      break;
    case ae_primary_polar:
      lint_print(linter, "%(");
      lint_exp(linter, exp->d.polar->mod);
      lint_print(linter, ")");
      break;
    case ae_primary_vec:
      lint_print(linter, "@(");
      lint_exp(linter, exp->d.vec->args);
      lint_print(linter, ")");
      break;
    case ae_primary_char:
      lint_print(linter, "'%s'", exp->d.chr);
      break;
    case ae_primary_nil:
      lint_print(linter, "()");
      break;
  }
}
static void lint_exp_array(Linter* linter, Exp_Array* array) {
  lint_exp(linter, array->base);
  lint_exp(linter, array->indices->exp_list);
 }

static void lint_exp_cast(Linter* linter, Exp_Cast* cast) {
  lint_exp(linter, cast->exp);
}

static void lint_exp_postfix(Linter* linter, Exp_Postfix* postfix) {
  lint_exp(linter, postfix->exp);
  lint_print(linter, "%s", op2str(postfix->op));
}

static void lint_exp_dur(Linter* linter, Exp_Dur* dur) {
  lint_exp(linter, dur->base);
  lint_print(linter, "::");
  lint_exp(linter, dur->unit);
}

static void lint_exp_call(Linter* linter, Exp_Func* exp_func) {
  if(exp_func->types)
    lint_type_list(linter, exp_func->types);
  lint_exp(linter, exp_func->func);
  lint_print(linter, "(");
  if(exp_func->args)
   lint_exp(linter, exp_func->args);
  lint_print(linter, ")");
}

static void  lint_exp_dot(Linter* linter, Exp_Dot* member) {
  lint_exp(linter, member->base);
  lint_print(linter, ".%s", s_name(member->xid));
}

static void lint_exp_if(Linter* linter, Exp_If* exp_if) {
  lint_exp(linter, exp_if->cond);
  lint_print(linter, " ? ");
  lint_exp(linter, exp_if->if_exp);
  lint_print(linter, " : ");
  lint_exp(linter, exp_if->else_exp);
}

static void lint_exp(Linter* linter,  Exp exp) {
  while(linter, exp) {
    switch(linter, exp->exp_type) {
      case ae_exp_primary:
        lint_exp_primary(linter, &exp->d.exp_primary);
        break;
      case ae_exp_decl:
        lint_exp_decl(linter, &exp->d.exp_decl);
        break;
      case ae_exp_unary:
        lint_exp_unary(linter, &exp->d.exp_unary);
        break;
      case ae_exp_binary:
        lint_exp_binary(linter, &exp->d.exp_binary);
        break;
      case ae_exp_postfix:
        lint_exp_postfix(linter, &exp->d.exp_postfix);
        break;
      case ae_exp_cast:
        lint_exp_cast(linter, &exp->d.exp_cast);
        break;
      case ae_exp_call:
        lint_exp_call(linter, &exp->d.exp_func);
        break;
      case ae_exp_array:
        lint_exp_array(linter, &exp->d.exp_array);
        break;
      case ae_exp_dot:
        lint_exp_dot(linter, &exp->d.exp_dot);
        break;
      case ae_exp_dur:
        lint_exp_dur(linter, &exp->d.exp_dur);
        break;
      case ae_exp_if:
        lint_exp_if(linter, &exp->d.exp_if);
        break;
    }
    exp = exp->next;
    if(exp)
      lint_print(linter, ", ");
  }
}

static void lint_stmt_code(Linter* linter, Stmt_Code stmt) {
  if(!stmt->stmt_list) {
    lint_print(linter, "{}");
    return;
  }
  lint_print(linter, "{\n");
  linter->indent++;
  if(stmt->stmt_list)
    lint_stmt_list(linter, stmt->stmt_list);
  linter->indent--;
  lint_indent(linter);
  lint_print(linter, "}");
}

static void lint_stmt_return(Linter* linter, Stmt_Return stmt) {
  lint_print(linter, "return");
  if(stmt->val) {
    lint_print(linter, " ");
    lint_exp(linter, stmt->val);
  }
  lint_print(linter, ";\n");
}

static void lint_stmt_flow(Linter* linter, struct Stmt_Flow_* stmt, m_str str) {
  if(!stmt->is_do) {
    lint_print(linter, "%s(", str);
    lint_exp(linter, stmt->cond);
    lint_print(linter, ")");
    linter->skip++;
    lint_stmt_indent(linter, stmt->body);
    lint_print(linter, "\n");
  } else {
    lint_print(linter, "do");
    lint_stmt_indent(linter, stmt->body);
    lint_print(linter, " %s(", str);
    lint_exp(linter, stmt->cond);
    lint_print(linter, ")\n");
  }
}

static void lint_stmt_for(Linter* linter, Stmt_For stmt) {
  lint_print(linter, "for(");
  linter->nonl++;
  lint_stmt(linter, stmt->c1);
  lint_print(linter, " ");
  lint_stmt(linter, stmt->c2);
  lint_print(linter, " ");
  lint_exp(linter, stmt->c3);
  lint_print(linter, ")");
  linter->nonl--;
  lint_stmt_indent(linter, stmt->body);
}

static void lint_stmt_loop(Linter* linter, Stmt_Loop stmt) {
  lint_print(linter, "repeat(");
  lint_exp(linter, stmt->cond);
  lint_print(linter, ")");
  lint_stmt(linter, stmt->body);
}

static void lint_stmt_switch(Linter* linter, Stmt_Switch stmt) {
  lint_print(linter, "switch(");
  lint_exp(linter, stmt->val);
  lint_print(linter, ")");
}

static void lint_stmt_case(Linter* linter, Stmt_Case stmt) {
  lint_print(linter, "case ");
  lint_exp(linter, stmt->val);
  lint_print(linter, ":\n");
}

static void lint_stmt_if(Linter* linter, Stmt_If stmt) {
  lint_print(linter, "if(");
  lint_exp(linter, stmt->cond);
  lint_print(linter, ")");
    if(stmt->if_body->type == ae_stmt_code)
      linter->skip++;
  lint_stmt_indent(linter, stmt->if_body);
  if(stmt->else_body) {
    if(stmt->if_body->type != ae_stmt_code)
      lint_indent(linter);
    else
      lint_print(linter, " ");
    lint_print(linter, "else");
    if(stmt->else_body->type == ae_stmt_code)
      linter->skip++;
    lint_stmt_indent(linter, stmt->else_body);
    if(stmt->else_body->type == ae_stmt_code)
      lint_print(linter, "\n");
  }
}

void lint_stmt_enum(Linter* linter, Stmt_Enum stmt) {
  ID_List list = stmt->list;
  lint_print(linter, "enum {\n");
  linter->indent++;
  while(list) {
    lint_indent(linter);
    lint_print(linter, "%s", s_name(list->xid));
    list = list->next;
    if(list)
      lint_print(linter, ",\n");
    else
      lint_print(linter, "\n");
  }
  linter->indent--;
  lint_indent(linter);
  lint_print(linter, "}");
  if(stmt->xid)
    lint_print(linter, " %s", s_name(stmt->xid));
  lint_print(linter, ";\n");
}

void lint_stmt_fptr(Linter* linter, Stmt_Ptr ptr) {
  Arg_List list = ptr->args;
  lint_print(linter, "typedef ");
  lint_print(linter, "%s", GET_FLAG(ptr->type, ae_flag_variadic) ?
      "variadic " : "function ");
  lint_type_decl(linter, ptr->type);
  lint_print(linter, " ");
  lint_print(linter, s_name(ptr->xid));
  lint_print(linter, "(");
  while(list) {
    lint_type_decl(linter, list->type_decl);
    lint_print(linter, " %s", s_name(list->var_decl->xid));
    list = list->next;
    if(list)
      lint_print(linter, ", ");
  }
  lint_print(linter, ")\n");
}

void lint_stmt_union(Linter* linter, Stmt_Union stmt) {
  Decl_List l = stmt->l;
  lint_print(linter, "union {\n");
  linter->indent++;
  while(l) {
    lint_indent(linter);
    lint_exp(linter, l->self);
    lint_print(linter, ";\n");
    l = l->next;
  }
  linter->indent--;
  lint_print(linter, "}\n");
}

void lint_stmt_goto(Linter* linter, Stmt_Goto_Label stmt) {
  lint_print(linter, "goto;\n");
}

void lint_stmt_continue(Linter* linter, Stmt_Continue stmt) {
  lint_print(linter, "continue;\n");
}

void lint_stmt_break(Linter* linter, Stmt_Break stmt) {
  lint_print(linter, "break;\n");
}

static void lint_stmt(Linter* linter, Stmt stmt) {
  if(stmt->type == ae_stmt_exp && !stmt->d.stmt_exp.val)
    return;
  lint_indent(linter);
  switch(linter, stmt->type) {
    case ae_stmt_exp:
      lint_exp(linter, stmt->d.stmt_exp.val);
      if(linter->nonl)
        lint_print(linter, ";");
      else
        lint_print(linter, ";\n");
      break;
    case ae_stmt_code:
      lint_stmt_code(linter, &stmt->d.stmt_code);
      break;
    case ae_stmt_return:
      lint_stmt_return(linter, &stmt->d.stmt_return);
      break;
    case ae_stmt_if:
      lint_stmt_if(linter, &stmt->d.stmt_if);
      break;
    case ae_stmt_while:
      lint_stmt_flow(linter, &stmt->d.stmt_while, "while");
      break;
    case ae_stmt_for:
      lint_stmt_for(linter, &stmt->d.stmt_for);
      break;
    case ae_stmt_until:
      lint_stmt_flow(linter, &stmt->d.stmt_until, "until");
      break;
    case ae_stmt_loop:
      lint_stmt_loop(linter, &stmt->d.stmt_loop);
      break;
    case ae_stmt_switch:
      lint_stmt_switch(linter, &stmt->d.stmt_switch);
      break;
    case ae_stmt_case:
      lint_stmt_case(linter, &stmt->d.stmt_case);
      break;
    case ae_stmt_enum:
      lint_stmt_enum(linter, &stmt->d.stmt_enum);
      break;
    case ae_stmt_continue:
      lint_stmt_continue(linter, &stmt->d.stmt_continue);
      break;
    case ae_stmt_break:
      lint_stmt_break(linter, &stmt->d.stmt_break);
      break;
    case ae_stmt_gotolabel:
      lint_stmt_goto(linter, &stmt->d.stmt_gotolabel);
      break;
    case ae_stmt_funcptr:
      lint_stmt_fptr(linter, &stmt->d.stmt_ptr);
      break;
    case ae_stmt_union:
      lint_stmt_union(linter, &stmt->d.stmt_union);
      break;
  }
}

static void lint_stmt_list(Linter* linter, Stmt_List list) {
  while(list) {
    lint_stmt(linter, list->stmt);
    list = list->next;
  }
}

static void lint_func_def(Linter* linter, Func_Def f) {
  Arg_List list = f->arg_list;
  lint_indent(linter);
  lint_print(linter, "%s", GET_FLAG(f, ae_flag_variadic) ?
      "variadic " : "function ");
  if(GET_FLAG(f, ae_flag_static))
    lint_print(linter, "static ");
  lint_type_decl(linter, f->type_decl);
  lint_print(linter, " ");
  lint_print(linter, s_name(f->name));
  lint_print(linter, "(");
  while(list) {
    lint_type_decl(linter, list->type_decl);
    lint_print(linter, " %s", s_name(list->var_decl->xid));
    list = list->next;
    if(list)
      lint_print(linter, ", ");
  }
  lint_print(linter, ")");
  linter->skip++;
  lint_stmt_indent(linter, f->code);
  lint_print(linter, "\n");
  f->flag &= ~ae_flag_template;
}

static void lint_section(Linter* linter, Section* section) {
  ae_Section_Type t = section->type;
  if(linter, t == ae_section_stmt)
    lint_stmt_list(linter, section->d.stmt_list);
  else if(linter, t == ae_section_func)
    lint_func_def(linter, section->d.func_def);
  else if(linter, t == ae_section_class)
    lint_class_def(linter, section->d.class_def);
}

static void lint_class_def(Linter* linter, Class_Def class_def) {
  Class_Body body = class_def->body;
  lint_indent(linter);
  if(class_def->types) {
    lint_print(linter, "template");
    lint_id_list(linter, class_def->types);
  }

  lint_print(linter, "class %s", s_name(class_def->name->xid));
  if(class_def->ext) {
    lint_print(linter, " extends ");
    lint_id_list(linter, class_def->ext);
    lint_print(linter, "\n");
  }
  lint_print(linter, " {\n");
  linter->indent++;
  while(body) {
    lint_section(linter, body->section);
    body = body->next;
  }
  lint_print(linter, "}\n\n");
  linter->indent--;
}

void lint_ast(Linter* linter, Ast ast) {
  while(ast) {
    lint_section(linter, ast->section);
    ast = ast->next;
  }
}

int main(int argc, char** argv) {
  argc--; argv++;
  scan_map = new_map();
  while(argc--) {
    Ast ast;
    Linter linter = {};
    char c[strlen(*argv) + 6];
    sprintf(c, "%s.lint", *argv);
    if(!(ast = parse(*argv++)))
      continue;
    linter.file = fopen(c, "w");
    lint_ast(&linter, ast);
    free_ast(ast);
    fclose(linter.file);
  }
  free_map(scan_map);
  free_symbols();
  return 0;
}
