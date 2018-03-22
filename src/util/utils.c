#include <string.h>
#include <math.h>
#include "defs.h"
#include "err_msg.h"
#include "absyn.h"
#include "type.h"
#include "value.h"

m_uint num_digit(const m_uint i) {
  return i ? (m_uint)floor(log10(i) + 1) : 1;
}

ANN static Type find_typeof(const Env env, ID_List path) {
  path = path->ref;
  Value v = nspc_lookup_value2(env->curr, path->xid);
  Type t = (isa(v->m_type, t_class) > 0) ? v->m_type->d.base_type : v->m_type;
  path = path->next;
  while(path) {
    CHECK_OO((v = find_value(t, path->xid)))
    t = v->m_type;
    path = path->next;
  }
  return v->m_type;
}

ANN Type find_type(const Env env, ID_List path) {
  Nspc nspc;
  Type type;

  if(path->ref)
    return find_typeof(env, path);
  CHECK_OO((type = nspc_lookup_type1(env->curr, path->xid)))
  nspc = type->info;
  path = path->next;

  while(path) {
    Symbol xid = path->xid;
    Type t = nspc_lookup_type1(nspc, xid);
    while(!t && type && type->parent) {
      t = nspc_lookup_type2(type->parent->info, xid);
      type = type->parent;
    }
    if(!t)
      CHECK_BO(err_msg(UTIL_, path->pos,
            "...(cannot find class '%s' in nspc '%s')", s_name(xid), nspc->name))
    type = t;
    nspc = type->info;
    path = path->next;
  }
  return type;
}

ANN Value find_value(const Type type, const Symbol xid) {
  Value value;
  if(!type->info)
    return NULL;
  if((value = nspc_lookup_value2(type->info, xid)))
    return value;
  if(type->parent)
    return find_value(type->parent, xid);
  return NULL;
}

ANN Func find_func(const Type type, const Symbol xid) {
  Func func;
  if((func = nspc_lookup_func2(type->info, xid)))
    return func;
  if(type->parent)
    return find_func(type->parent, xid);
  return NULL;
}

ANN m_uint id_list_len(ID_List l) {
  m_uint len = 0;
  while(l) {
    len += strlen(s_name(l->xid));
    if((l = l->next))
      len++;
  }
  return len + 1;
}

ANN void type_path(m_str s, ID_List l) {
  s[0] = '\0';
  while(l) {
    strcat(s, s_name(l->xid));
    if((l = l->next))
      strcat(s, ".");
  }
}

ANN Type array_base(Type t) {
  while(GET_FLAG(t, ae_flag_typedef))
    t = t->parent;
  return t->d.base_type;
}

ANN Type array_type(const Type base, const m_uint depth) {
  m_uint i = depth;
  Type t;
  Symbol sym;
  char name[strlen(base->name) + 2* depth + 1];

  strcpy(name, base->name);
  while(i--)
    strcat(name, "[]");
  sym = insert_symbol(name);
  if((t = nspc_lookup_type1(base->owner, sym))) {
    ADD_REF(t)
    return t;
  }
  t = new_type(t_array->xid, base->name, t_array);
  t->name = s_name(sym);
  t->size = SZ_INT;
  t->array_depth = depth;
  t->d.base_type = base;
  t->info = t_array->info;
  ADD_REF(t->info);
  ADD_REF(t);
  SET_FLAG(t, ae_flag_checked);
  t->owner = base->owner;
  nspc_add_type(base->owner, sym, t);
  return t;
}

static const char escape1[] = "0'abfnrtv";
static const char escape2[] = "\0\'\a\b\f\n\r\t\v";

m_int get_escape(const char c, const int linepos) {
  m_uint i = 0;
  while(escape1[i] != '\0') {
    if(c == escape1[i])
      return escape2[i];
    i++;
  }
  CHECK_BB(err_msg(UTIL_, linepos, "unrecognized escape sequence '\\%c'", c))
  return -1;
}

ANN m_int str2char(const m_str c, const m_int linepos) {
  if(c[0] == '\\')
    return get_escape(c[1], linepos);
  else
    return c[0];
}

ANN m_bool type_unknown(const ID_List id, const m_str orig) {
  char path[id_list_len(id)];
  type_path(path, id);
  CHECK_BB(err_msg(SCAN1_, id->pos,
        "'%s' unknown type in %s", path, orig))
  return -1;
}

m_bool check_array_empty(const Array_Sub a, const m_str orig) {
  if(a->exp_list)
    CHECK_BB(err_msg(SCAN1_, a->pos, "type must be defined with empty []'s"
          " in %s declaration", orig))
   return 1;
}

ANN m_bool type_ref(Type t) {
  while(t) {
    if(GET_FLAG(t, ae_flag_empty))return 1;
    if(GET_FLAG(t, ae_flag_typedef))
      if(t->def && (t->def->ext && t->def->ext->array && !t->def->ext->array->exp_list))
        return 1;
    t = t->parent;
  }
  return 0;
}
