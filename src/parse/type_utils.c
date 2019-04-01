#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "value.h"
#include "type.h"
#include "vm.h"
#include "parse.h"

ANN m_bool isres(const Symbol xid) {
  const m_str s = s_name(xid);
  if(!strcmp(s, "this") || !strcmp(s, "vararg") || !name2op(s))
    ERR_B(0, "%s is reserved.", s_name(xid));
  return GW_OK;
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
