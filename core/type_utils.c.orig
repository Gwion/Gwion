#define _XOPEN_SOURCE 700
#include <math.h>
#include <string.h>
#include "err_msg.h"
#include "absyn.h"
#include "context.h"
#include "type.h"

m_uint num_digit(m_uint i) {
  return i ? (m_uint)floor(log10(i) + 1) : 1;
}

int verify_array(Array_Sub array) {
  if(array->err_num) {
    if(array->err_num == 1)
      CHECK_BB(err_msg(UTIL_, array->pos,
              "invalid format for array init [...][...]..."))
    else if(array->err_num == 2)
      CHECK_BB(err_msg(UTIL_, array->pos,
              "partially empty array init [...][]..."))
  }
  return 1;
}

int isa(Type var, Type parent) {
  if(var->xid == parent->xid)
    return 1;
  if(var->parent)
    return isa(var->parent, parent);
  return -1;
}

int isres(Env env, S_Symbol xid, int pos) {
  m_str s = s_name(xid);
  if(!strcmp(s, "this") || !strcmp(s, "now") || !name2op(s))
    goto error;
  return -1;
error:
  err_msg(UTIL_, 0, "%s is reserved.", s_name(xid));
  return 1;
}

int isprim(Type type) {
  if(type->array_depth)
    return -1;
  if(isa(type, &t_object) > 0)
    return -1;
  return 1;
}

Type find_type(Env env, ID_List path) {
  S_Symbol xid = NULL;
  Nspc nspc;
  Type t = NULL;

Type type = nspc_lookup_type(env->curr, path->xid, 1);
  if(!type)
    return NULL;
  nspc = type->info;
  path = path->next;

  while(path) {
    xid = path->xid;
    t = nspc_lookup_type(nspc, xid, 1);
    while(!t && type && type->parent) {
      t = nspc_lookup_type(type->parent->info, xid, -1);
      type = type->parent;
    }
    if(!t) {
      err_msg(UTIL_, path->pos,
              "...(cannot find class '%s' in nspc '%s')",
              s_name(xid), nspc->name);
      return NULL;
    }
    type = t;
    if(type)
      nspc = type->info;
    path = path->next;
  }
  return type;
}

m_bool env_add_value(Env env, m_str name, Type type, m_bool is_const, void* data) {
  Value v = new_value(type, name);
  ae_flag flag = ae_flag_checked | ae_flag_global | ae_flag_builtin | (is_const ? ae_flag_const : 0);
  v->flag = flag;
  v->ptr = data;
  v->owner = env->global_nspc;
  nspc_add_value(env->global_nspc, insert_symbol(name), v);
  return 1;
}

m_bool name_valid(m_str a) {
  m_uint i, len = strlen(a);
  for(i = 0; i < len; i++) {
    char c = a[i];
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
        || (c == '_') || (c >= '0' && c <= '9'))
      continue;
    else
      CHECK_BB(err_msg(UTIL_,  0, "illegal character '%c' in name '%s'...", a, a))
  }
  return 1;
}

Value find_value(Type type, S_Symbol xid) {
  Value value = NULL;
  if(!type || !type->info)
    return NULL;
  if((value = nspc_lookup_value(type->info, xid, -1)))
    return value;
  if(type->parent)
    return find_value(type->parent, xid);
  return NULL;
}

m_str type_path(ID_List list) {
  m_uint len = 0;
  ID_List path = list;
  while(path) {
    len += strlen(s_name(path->xid));
    if(path->next)
      len++;
    path = path->next;
  }
  char str[++len];
  memset(str, 0, len);
  path = list;
  while(path) {
    strcat(str, s_name(path->xid));
    if(path->next)
      strcat(str, ".");
    path = path->next;
  }
  return strdup(str);
}

Kindof kindof(Type type) {
  if(type->array_depth)
    return Kindof_Int;
  if(isa(type, &t_void) > 0)
    return Kindof_Void;
  else if(isa(type, &t_complex) > 0 || isa(type, &t_polar) > 0)
    return Kindof_Complex;
  if(isa(type, &t_int) > 0 || isa(type, &t_object) > 0)
    return Kindof_Int;
  else if(isa(type, &t_float) > 0 || isa(type, &t_time) > 0 || isa(type, &t_dur) > 0)
    return Kindof_Float;
  else if(isa(type, &t_vec3) > 0)
    return Kindof_Vec3;
  else if(isa(type, &t_vec4) > 0)
    return Kindof_Vec4;
  return Kindof_Int;
}


Type new_array_type(Env env, m_uint depth, Type base_type, Nspc owner_nspc) {
  Type t = new_type(te_array, base_type->name);
  t->parent = &t_array;
  t->size = SZ_INT;
  t->array_depth = depth;
  t->d.array_type = base_type;
  t->info = t_array.info;
  ADD_REF(t->info);
  t->owner = owner_nspc;
  return t;
}

m_int str2char(const m_str c, m_int linepos) {
  if(c[0] == '\\') {
    switch(c[1]) {
    case '0':  return '\0';
    case '\'': return '\'';
    case '\\': return '\\';
    case 'a':  return '\a';
    case 'b':  return '\b';
    case 'f':  return '\f';
    case 'n':  return '\n';
    case 'r':  return '\r';
    case 't':  return '\t';
    case 'v':  return 'v';
    default:
      err_msg(UTIL_, linepos, "unrecognized escape sequence '\\%c'", c[1]);
      return -1;
    }
  } else
    return c[0];
}
