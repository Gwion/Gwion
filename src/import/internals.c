#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "value.h"
#include "traverse.h"
#include "instr.h"
#include "object.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

void gwi_body(const Gwi gwi, const Class_Body body) {
  const Class_Def cdef = gwi->gwion->env->class_def->e->def;
  if(!cdef->body)
    cdef->body = body;
  else
    gwi->body->next = body;
  gwi->body = body;
}

ANN m_bool _name_valid(const Gwi gwi, const m_str a) {
  const m_uint len = strlen(a);
  m_uint lvl = 0;
  for(m_uint i = 0; i < len; i++) {
    char c = a[i];
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
        || (c == '_') || (c >= '0' && c <= '9'))
      continue;
    if(c == '<') {
      if(a[++i] != '~')
        GWI_ERR_B(_("illegal templating in name '%s'."), a)
      ++lvl;
      ++i;
      continue;
    } else if(c == ',') {
      if(!lvl)
        GWI_ERR_B(_("illegal use of ',' outside of templating in name '%s'."), a)
    } else if(c == '~') {
      if(!lvl || a[++i] != '>')
        GWI_ERR_B(_("illegal templating in name '%s'."), a)
      --lvl;
    } else
      GWI_ERR_B(_("illegal character '%c' in name '%s'."), c, a)
  }
  return !lvl ? 1 : -1;
}

ANN m_bool name_valid(const Gwi gwi, const m_str a) {
  return _name_valid(gwi, a[0] != '@' ? a : a + 1);
}

ANN void gwi_reset(const Gwi gwi) {
  if(gwi->ck)
    ck_clean(gwi);
  mp_free2(gwi->gwion->mp, sizeof(ImportCK), gwi->ck);
  gwi->ck = NULL;
  env_reset(gwi->gwion->env);
}
