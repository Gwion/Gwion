#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "absyn.h"
#include "map.h"

#define TABLEN 2

extern m_str op2str(Operator);

typedef struct {
  const m_str  filename;
  Vector class_stack;
  FILE*  file;
} Tagger;

static void tag_exp(Tagger* tagger, Exp exp);
static void tag_type_decl(Tagger* tagger, Type_Decl* type);
static void tag_stmt(Tagger* tagger, Stmt stmt);
static void tag_stmt_list(Tagger* tagger, Stmt_List list);
static void tag_class_def(Tagger* tagger, Class_Def class_def);

static void tag_print(Tagger* tagger, const char* fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  vfprintf(tagger->file, fmt, arg);
  va_end(arg);
}

static void tag(Tagger* tagger, const m_str name) {
  tag_print(tagger, "%s\t%s\t", name, tagger->filename);
}

static void tag_id_list(Tagger* tagger, ID_List list) {
  m_bool next = list->next ? 1 : 0;
  if(next)
    tag_print(tagger, "<");
  while(list) {
    tag_print(tagger, s_name(list->xid));
    list = list->next;
    if(list)
      tag_print(tagger, ".");
  }
  if(next)
    tag_print(tagger, ">");
}

static void tag_array(Tagger* tagger, Array_Sub array) {
  m_uint i;
  Exp exp = array->exp_list;
  for(i = 0; i < array->depth; i++) {
    Exp tmp = exp ? exp->next : NULL;
    if(exp)
      exp->next = NULL;
    tag_print(tagger, "[");
    if(exp)
     tag_exp(tagger, exp);
    tag_print(tagger, "]");
    if(exp) {
      exp->next = tmp;
      exp = tmp;
    }
  }
}

static void tag_type_list(Tagger* tagger, Type_List list) {
  tag_print(tagger, "<{");
  while(list) {
    tag_type_decl(tagger, list->list);
    list = list->next;
    if(list)
      tag_print(tagger, ", ");
  }
  tag_print(tagger, "}>");
}

static void tag_type_decl(Tagger* tagger, Type_Decl* type) {
  if(type->xid->ref) {
    tag_print(tagger, "typeof ");
    tag_id_list(tagger, type->xid->ref ? type->xid->ref : type->xid);
  } else {
    if(type->types)
      tag_type_list(tagger, type->types);
    tag_id_list(tagger, type->xid);
  }
  if(GET_FLAG(type, ae_flag_ref))
    tag_print(tagger, "@");
  if(type->array)
    tag_array(tagger, type->array);
}

static void tag_exp_decl(Tagger* tagger, Exp_Decl* decl) {
  Var_Decl_List list = decl->list;
  while(list) {
    tag(tagger, s_name(list->self->xid));
    tag_print(tagger, "/^");
    tag_type_decl(tagger, decl->type);
    if(list->self->array)
      tag_array(tagger, list->self->array);
    tag_print(tagger, "$/;\"\t%s\n", vector_at(tagger->class_stack, 0) ?
        "m" : "v");
    list = list->next;
  }
}

static void tag_exp_unary(Tagger* tagger, Exp_Unary* unary) {
  return;
}

static void tag_exp_binary(Tagger* tagger, Exp_Binary* binary) {
  tag_exp(tagger, binary->lhs);
  tag_exp(tagger, binary->rhs);
}

static void tag_exp_primary(Tagger* tagger, Exp_Primary* exp) {
  return;
}

static void tag_exp_array(Tagger* tagger, Exp_Array* array) {
  tag_exp(tagger, array->base);
  tag_exp(tagger, array->indices->exp_list);
 }

static void tag_exp_cast(Tagger* tagger, Exp_Cast* cast) {
  tag_exp(tagger, cast->exp);
}

static void tag_exp_post(Tagger* tagger, Exp_Postfix* post) {
  tag_exp(tagger, post->exp);
}

static void tag_exp_call(Tagger* tagger, Exp_Func* exp_func) {
  return;
}

static void  tag_exp_dot(Tagger* tagger, Exp_Dot* member) {
  return;
}

static void  tag_exp_dur(Tagger* tagger, Exp_Dur* member) {
  return;
}

static void tag_exp_if(Tagger* tagger, Exp_If* exp_if) {
  tag_exp(tagger, exp_if->cond);
  tag_exp(tagger, exp_if->if_exp);
  tag_exp(tagger, exp_if->else_exp);
}

static void tag_exp(Tagger* tagger,  Exp exp) {
  while(exp) {
    switch(exp->exp_type) {
      case ae_exp_primary:
        tag_exp_primary(tagger, &exp->d.exp_primary);
        break;
      case ae_exp_decl:
        tag_exp_decl(tagger, &exp->d.exp_decl);
        break;
      case ae_exp_unary:
        tag_exp_unary(tagger, &exp->d.exp_unary);
        break;
      case ae_exp_binary:
        tag_exp_binary(tagger, &exp->d.exp_binary);
        break;
      case ae_exp_post:
        tag_exp_post(tagger, &exp->d.exp_post);
        break;
      case ae_exp_cast:
        tag_exp_cast(tagger, &exp->d.exp_cast);
        break;
      case ae_exp_call:
        tag_exp_call(tagger, &exp->d.exp_func);
        break;
      case ae_exp_array:
        tag_exp_array(tagger, &exp->d.exp_array);
        break;
      case ae_exp_dot:
        tag_exp_dot(tagger, &exp->d.exp_dot);
        break;
      case ae_exp_dur:
        tag_exp_dur(tagger, &exp->d.exp_dur);
        break;
      case ae_exp_if:
        tag_exp_if(tagger, &exp->d.exp_if);
        break;
    }
    exp = exp->next;
  }
}

static void tag_stmt_code(Tagger* tagger, Stmt_Code stmt) {
  if(stmt->stmt_list)
    tag_stmt_list(tagger, stmt->stmt_list);
}

static void tag_stmt_return(Tagger* tagger, Stmt_Return stmt) {
  if(stmt->val)
    tag_exp(tagger, stmt->val);
}

static void tag_stmt_flow(Tagger* tagger, struct Stmt_Flow_* stmt) {
  tag_exp(tagger, stmt->cond);
  tag_stmt(tagger, stmt->body);
}

static void tag_stmt_for(Tagger* tagger, Stmt_For stmt) {
  tag_stmt(tagger, stmt->c1);
  tag_stmt(tagger, stmt->c2);
  tag_exp(tagger, stmt->c3);
  tag_stmt(tagger, stmt->body);
}

static void tag_stmt_auto(Tagger* tagger, Stmt_Auto stmt) {
  // TODO tag id
  tag_exp(tagger, stmt->exp);
  tag_stmt(tagger, stmt->body);
}
static void tag_stmt_loop(Tagger* tagger, Stmt_Loop stmt) {
  tag_exp(tagger, stmt->cond);
  tag_stmt(tagger, stmt->body);
}

static void tag_stmt_switch(Tagger* tagger, Stmt_Switch stmt) {
  tag_exp(tagger, stmt->val);
}

static void tag_stmt_case(Tagger* tagger, Stmt_Case stmt) {
  tag_exp(tagger, stmt->val);
}

static void tag_stmt_if(Tagger* tagger, Stmt_If stmt) {
  tag_exp(tagger, stmt->cond);
  tag_stmt(tagger, stmt->if_body);
  if(stmt->else_body)
    tag_stmt(tagger, stmt->else_body);
}

void tag_stmt_enum(Tagger* tagger, Stmt_Enum stmt) {
  ID_List list = stmt->list;
  if(stmt->xid) {
    tag(tagger, s_name(stmt->xid));
    tag_print(tagger, "0;\"\tt");
  }
  while(list) {
    tag(tagger, s_name(list->xid));
    tag_print(tagger, "0;\"\te");
    list = list->next;
  }
}

void tag_stmt_fptr(Tagger* tagger, Stmt_Ptr ptr) {
  Arg_List list = ptr->args;
  tag(tagger, s_name(ptr->xid));
  tag_print(tagger, "/^");
  tag_type_decl(tagger, ptr->type);
  tag_print(tagger, " ");
  tag_print(tagger, s_name(ptr->xid));
  tag_print(tagger, "(");
  while(list) {
    tag_type_decl(tagger, list->type_decl);
    tag_print(tagger, " %s", s_name(list->var_decl->xid));
    list = list->next;
    if(list)
      tag_print(tagger, ", ");
    else if(GET_FLAG(ptr->type, ae_flag_variadic))
      tag_print(tagger, ", ...");
  }

  tag_print(tagger, ") {$/;\tt\n");
}

void tag_stmt_typedef(Tagger* tagger, Stmt_Typedef ptr) {
  tag(tagger, s_name(ptr->xid));
  tag_print(tagger, "/^");
  tag_type_decl(tagger, ptr->type);
  tag_print(tagger, " ");
  tag_print(tagger, s_name(ptr->xid));
  tag_print(tagger, ") {$/;\tt\n");
}

void tag_stmt_union(Tagger* tagger, Stmt_Union stmt) {
  Decl_List l = stmt->l;
  if(stmt->xid) {
    tag(tagger, s_name(stmt->xid));
    tag_print(tagger, "0;\"\tu");
  }
  while(l) {
    tag_exp(tagger, l->self);
    l = l->next;
  }
}

void tag_stmt_goto(Tagger* tagger, Stmt_Goto_Label stmt) {
  return;
}

void tag_stmt_continue(Tagger* tagger, Stmt_Continue stmt) {
  return;
}

void tag_stmt_break(Tagger* tagger, Stmt_Break stmt) {
  return;
}

static void tag_stmt(Tagger* tagger, Stmt stmt) {
  if(stmt->stmt_type == ae_stmt_exp && !stmt->d.stmt_exp.val)
    return;
  switch(stmt->stmt_type) {
    case ae_stmt_exp:
      tag_exp(tagger, stmt->d.stmt_exp.val);
      break;
    case ae_stmt_code:
      tag_stmt_code(tagger, &stmt->d.stmt_code);
      break;
    case ae_stmt_return:
      tag_stmt_return(tagger, &stmt->d.stmt_return);
      break;
    case ae_stmt_if:
      tag_stmt_if(tagger, &stmt->d.stmt_if);
      break;
    case ae_stmt_while:
      tag_stmt_flow(tagger, &stmt->d.stmt_while);
      break;
    case ae_stmt_for:
      tag_stmt_for(tagger, &stmt->d.stmt_for);
      break;
    case ae_stmt_auto:
      tag_stmt_auto(tagger, &stmt->d.stmt_auto);
      break;
    case ae_stmt_until:
      tag_stmt_flow(tagger, &stmt->d.stmt_until);
      break;
    case ae_stmt_loop:
      tag_stmt_loop(tagger, &stmt->d.stmt_loop);
      break;
    case ae_stmt_switch:
      tag_stmt_switch(tagger, &stmt->d.stmt_switch);
      break;
    case ae_stmt_case:
      tag_stmt_case(tagger, &stmt->d.stmt_case);
      break;
    case ae_stmt_enum:
      tag_stmt_enum(tagger, &stmt->d.stmt_enum);
      break;
    case ae_stmt_continue:
      tag_stmt_continue(tagger, &stmt->d.stmt_continue);
      break;
    case ae_stmt_break:
      tag_stmt_break(tagger, &stmt->d.stmt_break);
      break;
    case ae_stmt_gotolabel:
      tag_stmt_goto(tagger, &stmt->d.stmt_gotolabel);
      break;
    case ae_stmt_funcptr:
      tag_stmt_fptr(tagger, &stmt->d.stmt_ptr);
      break;
    case ae_stmt_typedef:
      tag_stmt_typedef(tagger, &stmt->d.stmt_type);
      break;
    case ae_stmt_union:
      tag_stmt_union(tagger, &stmt->d.stmt_union);
      break;
  }
}

static void tag_stmt_list(Tagger* tagger, Stmt_List list) {
  while(list) {
    tag_stmt(tagger, list->stmt);
    list = list->next;
  }
}

static void tag_func_def(Tagger* tagger, Func_Def f) {
  Arg_List list = f->arg_list;
  tag_print(tagger, s_name(f->name));
  tag_print(tagger, "/^");
  if(f->tmpl && f->tmpl->base) {
    tag_print(tagger, "template ");
    tag_id_list(tagger, f->tmpl->list);
    tag_print(tagger, " ");
  }
  tag_type_decl(tagger, f->type_decl);
  tag_print(tagger, " ");
  tag_print(tagger, s_name(f->name));
  tag_print(tagger, "(");
  while(list) {
    tag_type_decl(tagger, list->type_decl);
    tag_print(tagger, " %s", s_name(list->var_decl->xid));
    list = list->next;
    if(list)
      tag_print(tagger, ", ");
  }
  if(GET_FLAG(f, ae_flag_variadic))
    tag_print(tagger, ", ...");
  tag_print(tagger, ") {$/;\tf\n");
  f->flag &= ~ae_flag_template;
}

static void tag_section(Tagger* tagger, Section* section) {
  ae_Section_Type t = section->section_type;
  if(t == ae_section_stmt)
    tag_stmt_list(tagger, section->d.stmt_list);
  else if(t == ae_section_func)
    tag_func_def(tagger, section->d.func_def);
  else if(t == ae_section_class)
    tag_class_def(tagger, section->d.class_def);
}

static void tag_class_def(Tagger* tagger, Class_Def class_def) {
  Class_Body body = class_def->body;
  tag(tagger, s_name(class_def->name->xid));
  tag_print(tagger, "/^");
  if(class_def->tmpl) {
    tag_print(tagger, "template");
    tag_id_list(tagger, class_def->tmpl->list);
  }
// TODO: handle class extend
  tag_print(tagger, " class %s$/;\"\tc\n", s_name(class_def->name->xid));
  vector_add(tagger->class_stack, (vtype)class_def);
  while(body) {
    tag_section(tagger, body->section);
    body = body->next;
  }
  vector_pop(tagger->class_stack);
}

void tag_ast(Tagger* tagger, Ast ast) {
  while(ast) {
    tag_section(tagger, ast->section);
    ast = ast->next;
  }
}

int main(int argc, char** argv) {
  argc--; argv++;
  while(argc--) {
    Ast ast;
    Tagger tagger = { *argv , new_vector() };
    char c[strlen(*argv) + 6];
    sprintf(c, "%s.tag", *argv);
    if(!(ast = parse(*argv++)))
      continue;
    tagger.file = fopen(c, "w");
    tag_ast(&tagger, ast);
    free_ast(ast);
    free_vector(tagger.class_stack);
    fclose(tagger.file);
  }
  free_symbols();
  return 0;
}
