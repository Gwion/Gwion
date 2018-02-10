#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "env.h"
#include "type.h"

static m_uint template_size(Env env, Class_Def c, Type_List call) {
  ID_List base = c->tmpl->list;
  m_uint size = strlen(c->type->name) + 3;
  while(base) {
    Type t = find_type(env, call->list->xid);
    t = scan_type(env, t, call->list);
    if(call->list->array)
      t = get_array(t, call->list->array, "template name");
    size += strlen(t->name);
    call = call->next;
    base = base->next;
    if(base)
      size++;
  }
  return size;
}

static m_bool template_name(Env env, Class_Def c, Type_List call, m_str str) {
  ID_List base = c->tmpl->list;
  strcpy(str, c->type->name);
  strcat(str, "<");
  while(base) { // TODO: error checking
    Type t = find_type(env, call->list->xid);
    t = scan_type(env, t, call->list);
    if(call->list->array)
      t = get_array(t, call->list->array, "template name");
    strcat(str, t->name);
    call = call->next;
    base = base->next;
    if(base)
      strcat(str, ",");
  }
  strcat(str, ">");
  return 1;
}

static ID_List template_id(Env env, Class_Def c, Type_List call) {
  m_uint size = template_size(env, c, call);
  char name[size];
  ID_List list;

  CHECK_BO(template_name(env, c, call, name))
  list = new_id_list(insert_symbol(name), call->pos);
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

static Class_Def template_class(Env env, Class_Def def, Type_List call) {
  Type t;
  ID_List name;
  name = template_id(env, def, call);
  if((t = nspc_lookup_type1(env->curr, name->xid))) {
    free_id_list(name);
    //ADD_REF(t)
    return t->def;
  }
  return new_class_def(def->flag, name, def->ext, def->body, call->pos);
}

m_bool template_push_types(Env env, ID_List base, Type_List call) {
  nspc_push_type(env->curr);
  while(base) {
    Type t = find_type(env, call->list->xid);
    CHECK_OB(t)
    CHECK_OB((t = scan_type(env, t, call->list)))
    if(call->list->array)
      CHECK_OB((t = get_array(t, call->list->array, "template name")))
    nspc_add_type(env->curr, base->xid, t);
    base = base->next;
    call = call->next;
  }
  return 1;
}

extern m_bool scan0_class_def(Env, Class_Def);
extern m_bool scan1_class_def(Env, Class_Def);
Type scan_type(Env env, Type t, Type_Decl* type) {
  if(GET_FLAG(t, ae_flag_template)) {
    if(GET_FLAG(t, ae_flag_ref))
      return t;
    if(!type->types)
      CHECK_BO(err_msg(SCAN1_, type->pos,
        "you must provide template types for type '%s'", t->name))
    if(template_match(t->def->tmpl->list, type->types) < 0)
      CHECK_BO(err_msg(SCAN1_, type->pos, "invalid template types number"))

    CHECK_BO(template_push_types(env, t->def->tmpl->list, type->types))
    Class_Def a = template_class(env, t->def, type->types);
    if(a->type) {
      nspc_pop_type(env->curr);
      return a->type;
    }
    CHECK_BO(scan0_class_def(env, a))
    SET_FLAG(a->type, ae_flag_template);
    SET_FLAG(a->type, ae_flag_ref);
    if(GET_FLAG(t, ae_flag_builtin))
      SET_FLAG(a->type, ae_flag_builtin);
    CHECK_BO(scan1_class_def(env, a))
    nspc_pop_type(env->curr);
    if(t->info->dtor) {
      a->type->info->dtor = t->info->dtor;
      SET_FLAG(a->type, ae_flag_dtor);
    }
    a->tmpl = new_tmpl_list(t->def->tmpl->list, 0);
    a->base = type->types;
    t = a->type;
  } else if(type->types)
      CHECK_BO(err_msg(SCAN1_, type->pos,
            "type '%s' is not template. You should not provide template types", t->name))
  return t;
}
