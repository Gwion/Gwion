#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "object.h"
#include "instr.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"

GWION_IMPORT(enum_test) {
  CHECK_BB(gwi_enum_ini(gwi, NULL))
    CHECK_BB(gwi_enum_add(gwi, "ENUM0", 0))
    CHECK_BB(gwi_enum_add(gwi, "ENUM1", 1))
    CHECK_BB(gwi_enum_add(gwi, "ENUM2", 2))
    CHECK_BB(gwi_enum_add(gwi, "ENUM3", 3))
    CHECK_BB(gwi_enum_add(gwi, "ENUM4", 4))
    CHECK_BB(gwi_enum_add(gwi, "ENUM5", 5))
    CHECK_BB(gwi_enum_add(gwi, "ENUM6", 6))
    CHECK_BB(gwi_enum_add(gwi, "ENUM7", 7))
    CHECK_BB(gwi_enum_add(gwi, "ENUM8", 8))
    CHECK_BB(gwi_enum_add(gwi, "ENUM9", 9))
  CHECK_OB(gwi_enum_end(gwi))

  CHECK_BB(gwi_enum_ini(gwi, "test"))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM0", 0))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM1", 1))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM2", 2))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM3", 3))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM4", 4))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM5", 5))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM6", 6))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM7", 7))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM8", 8))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM9", 9))
  CHECK_OB(gwi_enum_end(gwi))

  Type t_enum;
  CHECK_OB((t_myenum = gwi_mk_type(gwi, "Enum", 0, NULL)))
  CHECK_BB(gwi_class_ini(gwi, t_myenum, NULL, NULL))
    CHECK_BB(gwi_enum_ini(gwi, 0))
      CHECK_BB(gwi_enum_add(gwi, "ENUM0", 0))
      CHECK_BB(gwi_enum_add(gwi, "ENUM1", 1))
      CHECK_BB(gwi_enum_add(gwi, "ENUM2", 2))
      CHECK_BB(gwi_enum_add(gwi, "ENUM3", 3))
      CHECK_BB(gwi_enum_add(gwi, "ENUM4", 4))
      CHECK_BB(gwi_enum_add(gwi, "ENUM5", 5))
      CHECK_BB(gwi_enum_add(gwi, "ENUM6", 6))
      CHECK_BB(gwi_enum_add(gwi, "ENUM7", 7))
      CHECK_BB(gwi_enum_add(gwi, "ENUM8", 8))
      CHECK_BB(gwi_enum_add(gwi, "ENUM9", 9))
    CHECK_OB(gwi_enum_end(gwi))

    CHECK_BB(gwi_enum_ini(gwi, "Enumtest"))
      CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM0", 0))
      CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM1", 1))
      CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM2", 2))
      CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM3", 3))
      CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM4", 4))
      CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM5", 5))
      CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM6", 6))
      CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM7", 7))
      CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM8", 8))
      CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM9", 9))
    CHECK_OB(gwi_enum_end(gwi))
  CHECK_OB(gwi_class_end(gwi))

  return GW_OK;
}
