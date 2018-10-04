#include <string.h>
#include "stdlib.h"
#include "defs.h"
#include "type.h"
#include "err_msg.h"

ANN m_bool isa(const restrict Type var, const restrict Type parent) {
  return (var->xid == parent->xid) ? 1 : var->parent ? isa(var->parent, parent) : -1;
}

ANN m_bool isres(const Symbol xid) {
  const m_str s = s_name(xid);
  if(!strcmp(s, "this") || !strcmp(s, "now") ||
     !strcmp(s, "me")   || !strcmp(s, "vararg") ||
     !name2op(s)) {
    err_msg(TYPE_, 0, "%s is reserved.", s_name(xid));
    return 1;
  }
  return -1;
}

ANN Type find_common_anc(const restrict Type lhs, const restrict Type rhs) {
  return isa(lhs, rhs) > 0 ? rhs : isa(rhs, lhs) > 0 ? lhs : NULL;
}
