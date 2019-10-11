#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "object.h"
#include "instr.h"
#include "gwion.h"
#include "value.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

GWION_IMPORT(class_template_fail) {
  Type t_fail;
  const m_str list[2] = { "A", "B" };
  gwi_tmpl_ini(gwi, 2, list);
  GWI_OB((t_fail = gwi_mk_type(gwi, "ClassTemplate", SZ_INT, "Object")))
  GWI_BB(gwi_class_ini(gwi, t_fail, NULL, NULL))
  gwi_tmpl_end(gwi);
  gwi_tmpl_ini(gwi, 2, list);
  GWI_BB(gwi_func_ini(gwi, "int", "test", (f_xfun)1))
  GWI_BB(gwi_func_end(gwi, ae_flag_none))
  gwi_tmpl_end(gwi);
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
