#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "oo.h"
#include "env.h"
#include "value.h"
#include "type.h"
#include "err_msg.h"

ANN m_bool isres(const Symbol xid) {
  const m_str s = s_name(xid);
  if(!strcmp(s, "this") || !strcmp(s, "now") ||
     !strcmp(s, "me")   || !strcmp(s, "vararg") ||
     !name2op(s)) {
    err_msg(0, "%s is reserved.", s_name(xid));
    return 1;
  }
  return -1;
}

m_uint num_digit(const m_uint i) {
  return i ? (m_uint)floor(log10(i) + 1) : 1;
}

ANN m_uint id_list_len(ID_List l) {
  m_uint len = 0;
  do len += strlen(s_name(l->xid));
  while((l = l->next) && ++len);
  return len + 1;
}

ANN void type_path(const m_str str, ID_List l) {
  m_str s = str;
  do {
    const m_str name = s_name(l->xid);
    strcpy(s, name);
    s += strlen(name);
    if(l->next)
      strcpy(s++, ".");
  }
  while((l = l->next));
}
