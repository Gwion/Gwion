#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"

GWION_IMPORT(enum_test) {
  GWI_BB(gwi_enum_ini(gwi, "test"))
  GWI_BB(gwi_enum_add(gwi, "TYPED_ENUM0", 0))
  GWI_BB(gwi_enum_add(gwi, "TYPED_ENUM1", 1))
  GWI_BB(gwi_enum_add(gwi, "TYPED_ENUM2", 2))
  GWI_BB(gwi_enum_add(gwi, "TYPED_ENUM3", 3))
  GWI_BB(gwi_enum_add(gwi, "TYPED_ENUM4", 4))
  GWI_BB(gwi_enum_add(gwi, "TYPED_ENUM5", 5))
  GWI_BB(gwi_enum_add(gwi, "TYPED_ENUM6", 6))
  GWI_BB(gwi_enum_add(gwi, "TYPED_ENUM7", 7))
  GWI_BB(gwi_enum_add(gwi, "TYPED_ENUM8", 8))
  GWI_BB(gwi_enum_add(gwi, "TYPED_ENUM9", 9))
  GWI_OB(gwi_enum_end(gwi))

  GWI_OB(gwi_class_ini(gwi, "Enum", NULL))
  GWI_BB(gwi_enum_ini(gwi, "MyTest"))
  GWI_BB(gwi_enum_add(gwi, "ENUM0", 0))
  GWI_BB(gwi_enum_add(gwi, "ENUM1", 1))
  GWI_BB(gwi_enum_add(gwi, "ENUM2", 2))
  GWI_BB(gwi_enum_add(gwi, "ENUM3", 3))
  GWI_BB(gwi_enum_add(gwi, "ENUM4", 4))
  GWI_BB(gwi_enum_add(gwi, "ENUM5", 5))
  GWI_BB(gwi_enum_add(gwi, "ENUM6", 6))
  GWI_BB(gwi_enum_add(gwi, "ENUM7", 7))
  GWI_BB(gwi_enum_add(gwi, "ENUM8", 8))
  GWI_BB(gwi_enum_add(gwi, "ENUM9", 9))
  GWI_OB(gwi_enum_end(gwi))

  GWI_OB(gwi_class_end(gwi))

  return GW_OK;
}
