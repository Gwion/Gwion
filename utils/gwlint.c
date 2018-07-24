#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "absyn.h"

#define TABLEN 2

extern m_str op2str(Operator);

typedef struct {
  const m_str name;
  FILE*  file;
  m_uint line, pos;
  m_uint indent;
  m_bool skip;
  m_bool nonl;
} Linter;

ANN static void lint_type_decl(Linter* linter, Type_Decl* type);
ANN static void lint_exp(Linter* linter, Exp exp);
ANN static void lint_stmt(Linter* linter, Stmt stmt);
ANN static void lint_stmt_if(Linter* linter, Stmt_If stmt);
ANN static void lint_stmt_list(Linter* linter, Stmt_List list);
ANN static void lint_class_def(Linter* linter, Class_Def class_def);

ANN static void lint_print(Linter* linter, const char* fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  vfprintf(linter->file, fmt, arg);
  va_end(arg);
}

ANN static void lint_nl(Linter* linter) {
  m_uint pos = linter->pos;
  fprintf(linter->file, "\n");
  linter->pos = ftell(linter->file);
  if(linter->pos - pos > 80)
    fprintf(stderr, "'\033[31;1m%s\033[0m' long line %" INT_F "\n",
        linter->name, linter->line);
  linter->line++;
}

ANN static void lint_indent(Linter* linter) {
  if(linter->skip) {
    lint_print(linter, " ");
    linter->skip--;
    return;
  }
  for(m_uint i = 0; i < linter->indent; i++)
    for(m_uint j = 0; j < TABLEN; j++)
      lint_print(linter, " ");
}

ANN static void lint_id_list(Linter* linter, ID_List list) {
  const m_bool next = list->next ? 1 : 0;
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

ANN static void lint_array(Linter* linter, Array_Sub array) {
  Exp exp = array->exp;
  for(m_uint i = 0; i < array->depth; i++) {
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

ANN static void lint_array_lit(Linter* linter, Array_Sub array) {
  Exp exp = array->exp;
  for(m_uint i = 0; i < array->depth; i++) {
    Exp tmp = exp ? exp->next : NULL;
    lint_print(linter, "[");
    if(exp) {
      if(exp->exp_type == ae_exp_primary &&
          exp->d.exp_primary.primary_type == ae_primary_array)
        lint_print(linter, " ");
      lint_exp(linter, exp);
      if(exp->exp_type == ae_exp_primary &&
          exp->d.exp_primary.primary_type == ae_primary_array)
        lint_print(linter, " ");
    }
    lint_print(linter, "]");
    if(exp) {
      exp->next = tmp;
    }
  }
}

ANN static void lint_type_list(Linter* linter, Type_List list) {
  lint_print(linter, "<{");
  while(list) {
    lint_type_decl(linter, list->td);
    list = list->next;
    if(list)
      lint_print(linter, ", ");
  }
  lint_print(linter, "}>");
}

ANN static void lint_type_decl(Linter* linter, Type_Decl* type) {
  if(GET_FLAG(type, ae_flag_private))
    lint_print(linter, "private ");
  if(GET_FLAG(type, ae_flag_static))
    lint_print(linter, "static ");
  if(type->xid->ref) {
    lint_print(linter, "typeof ");
    lint_id_list(linter, type->xid->ref ? type->xid->ref : type->xid);
  } else {
    if(type->types)
    lint_type_list(linter, type->types);
    lint_id_list(linter, type->xid);
  }
  if(GET_FLAG(type, ae_flag_ref))
    lint_print(linter, "@");
  if(type->array)
    lint_array(linter, type->array);
}

static void lint_stmt_indent(Linter* linter, Stmt stmt) {
  if(stmt->stmt_type == ae_stmt_if) {
    lint_print(linter, " ");
    lint_stmt_if(linter, &stmt->d.stmt_if);
    return;
  } else if(stmt->stmt_type != ae_stmt_code) {
    lint_nl(linter);
    linter->indent++;
  } else if(!linter->skip)
      lint_print(linter, " ");
  lint_stmt(linter, stmt);
  if(stmt->stmt_type != ae_stmt_code)
    linter->indent--;
}

ANN static void lint_exp_decl(Linter* linter, Exp_Decl* decl) {
  Var_Decl_List list = decl->list;
  lint_type_decl(linter, decl->td);
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

ANN static void lint_exp_unary(Linter* linter, Exp_Unary* unary) {
  lint_print(linter, "%s", op2str(unary->op));
  switch(unary->op) {
    case op_plusplus:
    case op_minusminus:
      lint_exp(linter, unary->exp);
      break;
    case op_new:
      lint_print(linter, " ");
      lint_type_decl(linter, unary->td);
      break;
    case op_spork:
      lint_print(linter, " ~");
      if(unary->code)
        lint_stmt(linter, unary->code);
      else
        lint_exp(linter, unary->exp);
    default: break;
  }
}

ANN static void lint_exp_binary(Linter* linter, Exp_Binary* binary) {
  lint_exp(linter, binary->lhs);
  lint_print(linter, " %s ", op2str(binary->op));
  lint_exp(linter, binary->rhs);
}

ANN static void lint_exp_primary(Linter* linter, Exp_Primary* exp) {
  switch(exp->primary_type) {
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
      lint_exp(linter, exp->d.vec.exp);
      lint_print(linter, ")");
      break;
    case ae_primary_polar:
      lint_print(linter, "%(");
      lint_exp(linter, exp->d.vec.exp);
      lint_print(linter, ")");
      break;
    case ae_primary_vec:
      lint_print(linter, "@(");
      lint_exp(linter, exp->d.vec.exp);
      lint_print(linter, ")");
      break;
    case ae_primary_char:
      lint_print(linter, "'%s'", exp->d.chr);
      break;
    case ae_primary_nil:
      lint_print(linter, "()");
      break;
    default:
      break;
  }
}

ANN static void lint_exp_array(Linter* linter, Exp_Array* array) {
  lint_exp(linter, array->base);
  lint_exp(linter, array->array->exp);
}

ANN static void lint_exp_cast(Linter* linter, Exp_Cast* cast) {
  lint_exp(linter, cast->exp);
  lint_print(linter, "$ ");
  lint_type_decl(linter, cast->td);
}

ANN static void lint_exp_post(Linter* linter, Exp_Postfix* post) {
  lint_exp(linter, post->exp);
  lint_print(linter, "%s", op2str(post->op));
}

ANN static void lint_exp_dur(Linter* linter, Exp_Dur* dur) {
  lint_exp(linter, dur->base);
  lint_print(linter, "::");
  lint_exp(linter, dur->unit);
}

ANN static void lint_exp_call(Linter* linter, Exp_Func* exp_func) {
  if(exp_func->tmpl)
    lint_type_list(linter, exp_func->tmpl->types);
  lint_exp(linter, exp_func->func);
  lint_print(linter, "(");
  if(exp_func->args)
   lint_exp(linter, exp_func->args);
  lint_print(linter, ")");
}

ANN static void  lint_exp_dot(Linter* linter, Exp_Dot* member) {
  lint_exp(linter, member->base);
  lint_print(linter, ".%s", s_name(member->xid));
}

ANN static void lint_exp_if(Linter* linter, Exp_If* exp_if) {
  lint_exp(linter, exp_if->cond);
  lint_print(linter, " ? ");
  lint_exp(linter, exp_if->if_exp);
  lint_print(linter, " : ");
  lint_exp(linter, exp_if->else_exp);
}

static void lint_exp(Linter* linter,  Exp exp) {
  while(exp) {
    switch(exp->exp_type) {
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
      case ae_exp_post:
        lint_exp_post(linter, &exp->d.exp_post);
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
      default:
        break;
    }
    exp = exp->next;
    if(exp)
      lint_print(linter, ", ");
  }
}

ANN static void lint_stmt_code(Linter* linter, Stmt_Code stmt) {
  if(!stmt->stmt_list) {
    lint_print(linter, "{}");
    return;
  }
  lint_print(linter, "{");
  lint_nl(linter);
  linter->indent++;
  if(stmt->stmt_list)
    lint_stmt_list(linter, stmt->stmt_list);
  linter->indent--;
  lint_indent(linter);
  lint_print(linter, "}");
}

ANN static void lint_stmt_return(Linter* linter, Stmt_Exp stmt) {
  lint_print(linter, "return");
  if(stmt->val) {
    lint_print(linter, " ");
    lint_exp(linter, stmt->val);
  }
  lint_print(linter, ";");
  lint_nl(linter);
}

ANN static void lint_stmt_flow(Linter* linter, struct Stmt_Flow_* stmt, const m_str str) {
  if(!stmt->is_do) {
    lint_print(linter, "%s(", str);
    lint_exp(linter, stmt->cond);
    lint_print(linter, ")");
    linter->skip++;
    lint_stmt_indent(linter, stmt->body);
    lint_nl(linter);
  } else {
    lint_print(linter, "do");
    lint_stmt_indent(linter, stmt->body);
    lint_print(linter, " %s(", str);
    lint_exp(linter, stmt->cond);
    lint_print(linter, ")");
    lint_nl(linter);
  }
}

ANN static void lint_stmt_for(Linter* linter, Stmt_For stmt) {
  lint_print(linter, "for(");
  linter->nonl++;
  lint_stmt(linter, stmt->c1);
  lint_print(linter, " ");
  lint_stmt(linter, stmt->c2);
  lint_print(linter, " ");
  if(stmt->c3)
    lint_exp(linter, stmt->c3);
  lint_print(linter, ")");
  linter->nonl--;
  lint_stmt_indent(linter, stmt->body);
}

ANN static void lint_stmt_auto(Linter* linter, Stmt_Auto stmt) {
  lint_print(linter, "for(");
  lint_print(linter, "auto ");
  lint_print(linter, s_name(stmt->sym));
  lint_print(linter, " : ");
  lint_exp(linter, stmt->exp);
  lint_print(linter, ")");
  lint_stmt_indent(linter, stmt->body);
}

ANN static void lint_stmt_loop(Linter* linter, Stmt_Loop stmt) {
  lint_print(linter, "repeat(");
  lint_exp(linter, stmt->cond);
  lint_print(linter, ")");
  lint_stmt(linter, stmt->body);
}

ANN static void lint_stmt_switch(Linter* linter, Stmt_Switch stmt) {
  lint_print(linter, "switch(");
  lint_exp(linter, stmt->val);
  lint_print(linter, ")");
}

ANN static void lint_stmt_case(Linter* linter, Stmt_Exp stmt) {
  lint_print(linter, "case ");
  lint_exp(linter, stmt->val);
  lint_print(linter, ":");
  lint_nl(linter);
}

ANN static void lint_stmt_if(Linter* linter, Stmt_If stmt) {
  lint_print(linter, "if(");
  lint_exp(linter, stmt->cond);
  lint_print(linter, ")");
    if(stmt->if_body->stmt_type == ae_stmt_code)
      linter->skip++;
  lint_stmt_indent(linter, stmt->if_body);
  if(stmt->else_body) {
    if(stmt->if_body->stmt_type != ae_stmt_code)
      lint_indent(linter);
    else
      lint_print(linter, " ");
    lint_print(linter, "else");
    if(stmt->else_body->stmt_type == ae_stmt_code)
      linter->skip++;
    lint_stmt_indent(linter, stmt->else_body);
    if(stmt->else_body->stmt_type == ae_stmt_code)
      lint_nl(linter);
  }
}

ANN void lint_stmt_enum(Linter* linter, Stmt_Enum stmt) {
  ID_List list = stmt->list;
  lint_print(linter, "enum {");
  lint_nl(linter);
  linter->indent++;
  while(list) {
    lint_indent(linter);
    lint_print(linter, "%s", s_name(list->xid));
    list = list->next;
    if(list)
      lint_print(linter, ",");
    lint_nl(linter);
  }
  linter->indent--;
  lint_indent(linter);
  lint_print(linter, "}");
  if(stmt->xid)
    lint_print(linter, " %s", s_name(stmt->xid));
  lint_print(linter, ";");
  lint_nl(linter);
}

ANN void lint_stmt_fptr(Linter* linter, Stmt_Ptr ptr) {
  Arg_List list = ptr->args;
  lint_print(linter, "typedef ");
  lint_print(linter, "%s", GET_FLAG(ptr->td, ae_flag_variadic) ?
      "variadic " : "function ");
  lint_type_decl(linter, ptr->td);
  lint_print(linter, " ");
  lint_print(linter, s_name(ptr->xid));
  lint_print(linter, "(");
  while(list) {
    lint_type_decl(linter, list->td);
    lint_print(linter, " %s", s_name(list->var_decl->xid));
    list = list->next;
    if(list)
      lint_print(linter, ", ");
  }
  lint_print(linter, ")");
  lint_nl(linter);
}

ANN void lint_stmt_typedef(Linter* linter, Stmt_Typedef ptr) {
  lint_print(linter, "typedef ");
  lint_type_decl(linter, ptr->td);
  lint_print(linter, " ");
  lint_print(linter, s_name(ptr->xid));
  lint_print(linter, ";");
  lint_nl(linter);
}

ANN void lint_stmt_union(Linter* linter, Stmt_Union stmt) {
  Decl_List l = stmt->l;
  if(GET_FLAG(stmt, ae_flag_private))
    lint_print(linter, "private ");
  if(GET_FLAG(stmt, ae_flag_static))
    lint_print(linter, "static ");
  lint_print(linter, "union {");
  lint_nl(linter);
  linter->indent++;
  while(l) {
    lint_indent(linter);
    lint_exp(linter, l->self);
    lint_print(linter, ";");
    lint_nl(linter);
    l = l->next;
  }
  linter->indent--;
  lint_print(linter, "}");
  lint_nl(linter);
}

ANN void lint_stmt_goto(Linter* linter, Stmt_Goto_Label stmt) {
  if(stmt->is_label)
    lint_print(linter, "%s:", s_name(stmt->name));
  else
    lint_print(linter, "goto %s;", s_name(stmt->name));
  lint_nl(linter);
}

ANN void lint_stmt_continue(Linter* linter, Stmt stmt __attribute__((unused))) {
  lint_print(linter, "continue;");
  lint_nl(linter);
}

ANN void lint_stmt_break(Linter* linter, Stmt stmt __attribute__((unused))) {
  lint_print(linter, "break;");
  lint_nl(linter);
}

ANN static void lint_stmt(Linter* linter, Stmt stmt) {
  if(stmt->stmt_type == ae_stmt_exp && !stmt->d.stmt_exp.val)
    return;
  lint_indent(linter);
  switch(stmt->stmt_type) {
    case ae_stmt_exp:
      lint_exp(linter, stmt->d.stmt_exp.val);
        lint_print(linter, ";");
      if(!linter->nonl)
        lint_nl(linter);
        break;
    case ae_stmt_code:
      lint_stmt_code(linter, &stmt->d.stmt_code);
      break;
    case ae_stmt_return:
      lint_stmt_return(linter, &stmt->d.stmt_exp);
      break;
    case ae_stmt_if:
      lint_stmt_if(linter, &stmt->d.stmt_if);
      break;
    case ae_stmt_while:
      lint_stmt_flow(linter, &stmt->d.stmt_flow, "while");
      break;
    case ae_stmt_for:
      lint_stmt_for(linter, &stmt->d.stmt_for);
      break;
    case ae_stmt_auto:
      lint_stmt_auto(linter, &stmt->d.stmt_auto);
      break;
    case ae_stmt_until:
      lint_stmt_flow(linter, &stmt->d.stmt_flow, "until");
      break;
    case ae_stmt_loop:
      lint_stmt_loop(linter, &stmt->d.stmt_loop);
      break;
    case ae_stmt_switch:
      lint_stmt_switch(linter, &stmt->d.stmt_switch);
      break;
    case ae_stmt_case:
      lint_stmt_case(linter, &stmt->d.stmt_exp);
      break;
    case ae_stmt_enum:
      lint_stmt_enum(linter, &stmt->d.stmt_enum);
      break;
    case ae_stmt_continue:
      lint_stmt_continue(linter, stmt);
      break;
    case ae_stmt_break:
      lint_stmt_break(linter, stmt);
      break;
    case ae_stmt_gotolabel:
      lint_stmt_goto(linter, &stmt->d.stmt_gotolabel);
      break;
    case ae_stmt_funcptr:
      lint_stmt_fptr(linter, &stmt->d.stmt_ptr);
      break;
    case ae_stmt_typedef:
      lint_stmt_typedef(linter, &stmt->d.stmt_type);
      break;
    case ae_stmt_union:
      lint_stmt_union(linter, &stmt->d.stmt_union);
      break;
  }
}

ANN static void lint_stmt_list(Linter* linter, Stmt_List list) {
  while(list) {
    lint_stmt(linter, list->stmt);
    list = list->next;
  }
}

ANN static void lint_func_def(Linter* linter, Func_Def f) {
  Arg_List list = f->arg_list;
  lint_indent(linter);
  lint_print(linter, "%s", GET_FLAG(f, ae_flag_variadic) ?
      "variadic " : "function ");
  if(GET_FLAG(f, ae_flag_static))
    lint_print(linter, "static ");
  lint_type_decl(linter, f->td);
  lint_print(linter, " ");
  lint_print(linter, s_name(f->name));
  lint_print(linter, "(");
  while(list) {
    lint_type_decl(linter, list->td);
    lint_print(linter, " %s", s_name(list->var_decl->xid));
    list = list->next;
    if(list)
      lint_print(linter, ", ");
  }
  lint_print(linter, ")");
  linter->skip++;
  lint_stmt_indent(linter, f->d.code);
  lint_nl(linter);
  f->flag &= ~ae_flag_template;
}

ANN static void lint_section(Linter* linter, Section* section) {
  ae_Section_Type t = section->section_type;
  if(t == ae_section_stmt)
    lint_stmt_list(linter, section->d.stmt_list);
  else if(t == ae_section_func)
    lint_func_def(linter, section->d.func_def);
  else if(t == ae_section_class)
    lint_class_def(linter, section->d.class_def);
}

ANN static void lint_class_def(Linter* linter, Class_Def class_def) {
  Class_Body body = class_def->body;
  lint_indent(linter);
  if(class_def->tmpl) {
    lint_print(linter, "template");
    lint_id_list(linter, class_def->tmpl->list.list);
  }

  lint_print(linter, "class %s", s_name(class_def->name->xid));
  if(class_def->ext) {
    lint_print(linter, " extends ");
    lint_type_decl(linter, class_def->ext);
    lint_nl(linter);
  }
  lint_print(linter, " {");
  lint_nl(linter);
  linter->indent++;
  while(body) {
    lint_section(linter, body->section);
    body = body->next;
  }
  linter->indent--;
  lint_nl(linter);
  lint_nl(linter);
}

ANN void lint_ast(Linter* linter, Ast ast) {
  while(ast) {
    lint_section(linter, ast->section);
    ast = ast->next;
  }
}

int main(int argc, char** argv) {
  argc--; argv++;
  while(argc--) {
    Ast ast;
    Linter linter = { *argv, NULL, 1, 0, 0, 0, 0 };
    char c[strlen(*argv) + 6];
    sprintf(c, "%s.lint", *argv);
    FILE* f = fopen(*argv, "r");
    if(!f || !(ast = parse(*argv++, f)))
      goto close;
    linter.file = fopen(c, "w");
    lint_ast(&linter, ast);
    free_ast(ast);
    fclose(linter.file);
close:
    fclose(f);
  }
  free_symbols();
  return 0;
}
