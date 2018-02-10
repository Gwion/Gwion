#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_enum = { "Enum", SZ_INT };

IMPORT {
  CHECK_BB(gwi_enum_ini(gwi, NULL))
  CHECK_BB(gwi_enum_add(gwi, "ENUM0", 0))
  CHECK_BB(gwi_enum_add(gwi, "ENUM1", 0))
  CHECK_BB(gwi_enum_add(gwi, "ENUM2", 0))
  CHECK_BB(gwi_enum_add(gwi, "ENUM3", 0))
  CHECK_BB(gwi_enum_add(gwi, "ENUM4", 0))
  CHECK_BB(gwi_enum_add(gwi, "ENUM5", 0))
  CHECK_BB(gwi_enum_add(gwi, "ENUM6", 0))
  CHECK_BB(gwi_enum_add(gwi, "ENUM7", 0))
  CHECK_BB(gwi_enum_add(gwi, "ENUM8", 0))
  CHECK_BB(gwi_enum_add(gwi, "ENUM9", 0))
  CHECK_BB(gwi_enum_end(gwi))

  CHECK_BB(gwi_enum_ini(gwi, "test"))
  CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM0", 0))
  CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM1", 0))
  CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM2", 0))
  CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM3", 0))
  CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM4", 0))
  CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM5", 0))
  CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM6", 0))
  CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM7", 0))
  CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM8", 0))
  CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM9", 0))
  CHECK_BB(gwi_enum_end(gwi))
  
  CHECK_BB(gwi_class_ini(gwi, &t_enum, 0, NULL))
  
    CHECK_BB(gwi_enum_ini(gwi, 0))
    CHECK_BB(gwi_enum_add(gwi, "ENUM0", 0))
    CHECK_BB(gwi_enum_add(gwi, "ENUM1", 0))
    CHECK_BB(gwi_enum_add(gwi, "ENUM2", 0))
    CHECK_BB(gwi_enum_add(gwi, "ENUM3", 0))
    CHECK_BB(gwi_enum_add(gwi, "ENUM4", 0))
    CHECK_BB(gwi_enum_add(gwi, "ENUM5", 0))
    CHECK_BB(gwi_enum_add(gwi, "ENUM6", 0))
    CHECK_BB(gwi_enum_add(gwi, "ENUM7", 0))
    CHECK_BB(gwi_enum_add(gwi, "ENUM8", 0))
    CHECK_BB(gwi_enum_add(gwi, "ENUM9", 0))
    CHECK_BB(gwi_enum_end(gwi))

    CHECK_BB(gwi_enum_ini(gwi, "test"))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM0", 0))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM1", 0))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM2", 0))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM3", 0))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM4", 0))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM5", 0))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM6", 0))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM7", 0))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM8", 0))
    CHECK_BB(gwi_enum_add(gwi, "TYPED_ENUM9", 0))
    CHECK_BB(gwi_enum_end(gwi))
  
  CHECK_BB(gwi_class_end(gwi))

  return 1;
}
