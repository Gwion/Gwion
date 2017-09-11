#include <stdio.h>
#include <string.h>
#ifndef __linux__
#include <sys/select.h>
#endif
#include <netdb.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdlib.h>
#define _XOPEN_SOURCE 700
#include <math.h>
#include <string.h>
#include "err_msg.h"
#include "absyn.h"
#include "context.h"
#include "type.h"
#include "compile.h"

m_uint num_digit(m_uint i) {
  return i ? (m_uint)floor(log10(i) + 1) : 1;
}

m_bool verify_array(Array_Sub array) {
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

m_bool isa(Type var, Type parent) {
  return (var->xid == parent->xid) ? 1 : var->parent ? isa(var->parent, parent) : -1;
}

m_bool isprim(Type type) {
  return (type->array_depth || isa(type, &t_object) > 0) ? -1 : 1;
}

Type find_typeof(Env env, ID_List path) {
  path = path->ref;
  Value v = nspc_lookup_value(env->curr, path->xid, -1);
  Type t = (isa(v->m_type, &t_class) > 0) ? v->m_type->d.actual_type : v->m_type;
  path = path->next;
  while(path) {
    CHECK_OO((v = find_value(t, path->xid)))
    t = v->m_type;
    path = path->next;
  }
  return v->m_type;
}

Type find_type(Env env, ID_List path) {
  Nspc nspc;
  Type type;

  if(path->ref)
    return find_typeof(env, path);
  CHECK_OO((type = nspc_lookup_type(env->curr, path->xid, 1)))
  nspc = type->info;
  path = path->next;

  while(path) {
    S_Symbol xid = path->xid;
    Type t = nspc_lookup_type(nspc, xid, 1);
    while(!t && type && type->parent) {
      t = nspc_lookup_type(type->parent->info, xid, -1);
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

m_bool name_valid(m_str a) {
  m_uint i, len = strlen(a);
  for(i = 0; i < len; i++) {
    char c = a[i];
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
        || (c == '_') || (c >= '0' && c <= '9'))
      continue;
    CHECK_BB(err_msg(UTIL_,  0, "illegal character '%c' in name '%s'...", c, a))
  }
  return 1;
}

Value find_value(Type type, S_Symbol xid) {
  Value value;
  if(!type || !type->info)
    return NULL;
  if((value = nspc_lookup_value(type->info, xid, -1)))
    return value;
  if(type->parent)
    return find_value(type->parent, xid);
  return NULL;
}

Func find_func(Type type, S_Symbol xid) {
  Func func;
  if((func = nspc_lookup_func(type->info, xid, -1)))
    return func;
  if(type->parent)
    return find_func(type->parent, xid);
  return NULL;
}

m_uint id_list_len(ID_List list) {
  m_uint len = 0;
  ID_List path = list;
  while(path) {
    len += strlen(s_name(path->xid));
    if(path->next)
      len++;
    path = path->next;
  }
  return len + 1;
}

void type_path(char* str, ID_List list) {
  ID_List path = list;

  str[0] = '\0';
  while(path) {
    strcat(str, s_name(path->xid));
    if(path->next)
      strcat(str, ".");
    path = path->next;
  }
}

static m_bool is_int(Type t) {
  if(t->array_depth || isa(t, &t_int) > 0 || isa(t, &t_object) > 0)
    return 1;
  return -1;
}

static m_bool is_float(Type t) {
  if(isa(t, &t_float) > 0 || isa(t, &t_time) > 0 || isa(t, &t_dur) > 0)
    return 1;
  return -1;
}

static m_bool is_complex(Type t) {
  if(isa(t, &t_complex) > 0 || isa(t, &t_polar) > 0)
    return 1;
  return -1;
}

Kindof kindof(Type type) {
  if(is_int(type) > 0)
    return Kindof_Int;
  if(isa(type, &t_void) > 0)
    return Kindof_Void;
  else if(is_complex(type) > 0)
    return Kindof_Complex;
  else if(is_float(type) > 0)
    return Kindof_Float;
  else if(isa(type, &t_vec3) > 0)
    return Kindof_Vec3;
  return Kindof_Vec4;
}

Type new_array_type(Env env, m_uint depth, Type base_type, Nspc owner_nspc) {
  Type t = new_type(te_array, base_type->name, &t_array);
  t->size = SZ_INT;
  t->array_depth = depth;
  t->d.array_type = base_type;
  t->info = t_array.info;
  ADD_REF(t->info);
  t->owner = owner_nspc;
  return t;
}

static const char escape1[] = "0'abfnrtv";
static const char escape2[] = "\0\'\a\b\f\n\r\t\v";

m_int get_escape(const char c, int linepos) {
  m_uint i = 0;
  while(escape1[i] != '\0') {
    if(c == escape1[i])
      return escape2[i];
    i++;
  }
  CHECK_BB(err_msg(UTIL_, linepos, "unrecognized escape sequence '\\%c'", c))
  return -1;
}

m_int str2char(const m_str c, m_int linepos) {
  if(c[0] == '\\')
    return get_escape(c[1], linepos);
  else
    return c[0];
}
