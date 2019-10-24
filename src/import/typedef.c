#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
#include "emit.h"
#include "func.h"
#include "nspc.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

ANN m_int gwi_typedef_ini(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  gwi->val.type = type;
  gwi->val.name = name;
  return GW_OK;
}

ANN Type gwi_typedef_end(const Gwi gwi, const ae_flag flag) {
  struct func_checker ck = { .name=gwi->val.name, .flag=flag };
  CHECK_BO(check_typename_def(gwi, &ck))
  Type_Decl* td = import_td(gwi, gwi->val.type, 0); // TODO: make it GW_PASS
  if(td) {
    td->flag |= flag;
    const Symbol sym = insert_symbol(gwi->gwion->st, ck.name);
    const Type_Def tdef = new_type_def(gwi->gwion->mp, td, sym);
    if(ck.tmpl)
      tdef->tmpl = new_tmpl(gwi->gwion->mp, ck.tmpl, -1);
    traverse_type_def(gwi->gwion->env, tdef);
    const Type t = tdef->type;
    free_type_def(gwi->gwion->mp, tdef);
    return t;
  }
  func_checker_clean(gwi, &ck);
  return NULL;
}
