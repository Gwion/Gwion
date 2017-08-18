#include "absyn.h"

m_bool scan0_Ast(Env env, Ast ast);
m_bool scan1_ast(Env env, Ast ast);
m_bool scan2_ast(Env env, Ast ast);
m_bool check_ast(Env env, Ast ast);

m_bool scan1_exp_decl(Env env, Exp_Decl* decl);
m_bool scan2_exp_decl(Env env, Exp_Decl* decl);
m_bool check_exp_decl(Env env, Exp_Decl* decl);

m_bool scan1_func_def(Env env, Func_Def def);
m_bool scan2_func_def(Env env, Func_Def def);
m_bool check_func_def(Env env, Func_Def def);

m_bool scan1_class_def(Env env, Class_Def def);
m_bool scan2_class_def(Env env, Class_Def def);
m_bool check_class_def(Env env, Class_Def def);

m_bool traverse_ast(Env env, Ast ast) {
  if(scan0_Ast(env, ast) < 0 ||
     scan1_ast(env, ast) < 0 ||
     scan2_ast(env, ast) < 0 ||
     check_ast(env, ast) < 0)
    return -1;
  return 1;
}

m_bool traverse_decl(Env env, Exp_Decl* decl) {
  if(scan1_exp_decl(env, decl) < 0 ||
     scan2_exp_decl(env, decl) < 0 ||
     check_exp_decl(env, decl) ? 0:1)
    return -1;
  return 1;
}

m_bool traverse_def(Env env, Func_Def def) {
  if(scan1_func_def(env, def) < 0 ||
     scan2_func_def(env, def) < 0 ||
     check_func_def(env, def) < 0)
    return -1;
  return 1;
}

m_bool traverse_class_def(Env env, Class_Def def) {
  if(scan1_class_def(env, def) < 0 ||
     scan2_class_def(env, def) < 0 ||
     check_class_def(env, def) < 0)
    return -1;
  return 1;
}



#include <string.h>
#include "env.h"
#include "type.h"

static m_uint template_size(Env env, Class_Def c, Type_List call) {
  ID_List base = c->types;
  m_uint size = strlen(c->type->name) + 2;
  while(base) {
    Type t = find_type(env, call->list);
    size += strlen(t->name) + 1;
    call = call->next;
    base = base->next;
  }
  return size;
}

static m_bool template_name(Env env, Class_Def c, Type_List call, m_str str) {
  ID_List base = c->types;
  str[0] = '\0';
  strcat(str, c->type->name);
  while(base) {
    Type t = find_type(env, call->list);
    strcat(str, "@");
    strcat(str, t->name);
    call = call->next;
    base = base->next;
  }
  return 1;
}

static ID_List template_id(Env env, Class_Def c, Type_List call) {
  m_uint size = template_size(env, c, call);
  char name[size];
  ID_List list;

  CHECK_BO(template_name(env, c, call, name))
  list = new_id_list(name, call->pos);
  return list;
}

static m_bool template_match(ID_List base, Type_List call) {
  while(base) {
    if(!call)
      return -1;
    base = base->next;
    call = call->next;
  }
  if(call)
    return -1;
  return 1;
}

Class_Def template_class(Env env, Class_Def def, Type_List call) {
  ID_List name;

  CHECK_BO(template_match(def->types, call))
  name = template_id(env, def, call);
  return new_class_def(def->decl, name, def->ext, def->body, call->pos);
}

m_bool template_push_types(Env env, ID_List base, Type_List call) {
  m_uint count = 1;
  nspc_push_type(env->curr);
  while(base) {
    Type t = find_type(env, call->list);
    nspc_add_type(env->curr, base->xid, t);
    call = call->next;
    base = base->next;
    count++;
  }
  return 1;
}
