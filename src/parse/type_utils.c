#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "parse.h"

ANN m_bool isres(const Env env, const Symbol xid, const loc_t pos) {
  if(vector_find(&env->gwion->data->reserved, (vtype)xid) > -1)
    ERR_B(pos, _("%s is reserved."), s_name(xid));
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
  } while((l = l->next));
}
