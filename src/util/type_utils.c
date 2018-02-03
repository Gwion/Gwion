#include <string.h>
#include "stdlib.h"
#include "defs.h"
#include "type.h"
#include "err_msg.h"
const m_bool isa(const Type var, const Type parent) {
  return (var->xid == parent->xid) ? 1 : var->parent ? isa(var->parent, parent) : -1;
}

const m_bool isprim(const Type type) {
  return (type->array_depth || isa(type, &t_object) > 0) ? -1 : 1;
}

const m_bool isres(const S_Symbol xid, const m_uint pos) {
  m_str s = s_name(xid);
  if(!strcmp(s, "this") || !strcmp(s, "now") || !name2op(s)) {
    int ret = err_msg(TYPE_, 0, "%s is reserved.", s_name(xid));
    return -ret;
  }
  return -1;
}

const Type find_common_anc(const Type lhs, const Type rhs) {
  return isa(lhs, rhs) > 0 ? rhs : isa(rhs, lhs) > 0 ? lhs : NULL;
}
