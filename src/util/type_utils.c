#include <string.h>
#include "stdlib.h"
#include "defs.h"
#include "type.h"
#include "err_msg.h"
m_bool isa(Type var, Type parent) {
  return (var->xid == parent->xid) ? 1 : var->parent ? isa(var->parent, parent) : -1;
}

m_bool isprim(Type type) {
  return (type->array_depth || isa(type, &t_object) > 0) ? -1 : 1;
}

m_bool isres(S_Symbol xid, m_uint pos) {
  m_str s = s_name(xid);
  if(!strcmp(s, "this") || !strcmp(s, "now") || !name2op(s)) {
    int ret = err_msg(TYPE_, 0, "%s is reserved.", s_name(xid));
    return -ret;
  }
  return -1;
}

Type find_common_anc(Type lhs, Type rhs) {
  return isa(lhs, rhs) > 0 ? rhs : isa(rhs, lhs) > 0 ? lhs : NULL;
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
  else if(isa(type, &t_vec4) > 0)
    return Kindof_Vec4;
  return Kindof_Int;
}
