#include <stdlib.h>
#include <math.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "vm.h"
#include "gwion.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "value.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "func.h"
#include "gwi.h"

ANN /*static */m_bool _path_valid(const Env env, const m_str path, const loc_t);
GWION_IMPORT(str2decl) {
//  const m_str list[2] = { "A", "B" };
//  gwi_tmpl_ini(gwi, 2, list);
  const Type t_t0 = gwi_mk_type(gwi, "Test", SZ_INT, "Object");
  GWI_BB(gwi_class_ini(gwi, t_t0, NULL, NULL))
//  gwi_tmpl_end(gwi);
    const Type t_t2 = gwi_mk_type(gwi, "Child", SZ_INT, "Object");
    GWI_BB(gwi_class_ini(gwi, t_t2, NULL, NULL))
    GWI_BB(gwi_class_end(gwi))
  GWI_BB(gwi_class_end(gwi))
//  const Type t_t1 = gwi_mk_type(gwi, "Test2", SZ_INT, "<~int,<~int~>Ptr~>Test.Child");
  const Type t_t1 = gwi_mk_type(gwi, "Test2", SZ_INT, "Test.Child");
  GWI_BB(gwi_class_ini(gwi, t_t1, NULL, NULL))
  return gwi_class_end(gwi);
}
