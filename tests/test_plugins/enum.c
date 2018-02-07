#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_enum = { "Enum", SZ_INT };

IMPORT {
  CHECK_BB(importer_enum_ini(importer, NULL))
  CHECK_BB(importer_enum_add(importer, "ENUM0", 0))
  CHECK_BB(importer_enum_add(importer, "ENUM1", 0))
  CHECK_BB(importer_enum_add(importer, "ENUM2", 0))
  CHECK_BB(importer_enum_add(importer, "ENUM3", 0))
  CHECK_BB(importer_enum_add(importer, "ENUM4", 0))
  CHECK_BB(importer_enum_add(importer, "ENUM5", 0))
  CHECK_BB(importer_enum_add(importer, "ENUM6", 0))
  CHECK_BB(importer_enum_add(importer, "ENUM7", 0))
  CHECK_BB(importer_enum_add(importer, "ENUM8", 0))
  CHECK_BB(importer_enum_add(importer, "ENUM9", 0))
  CHECK_BB(importer_enum_end(importer))

  CHECK_BB(importer_enum_ini(importer, "test"))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM0", 0))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM1", 0))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM2", 0))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM3", 0))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM4", 0))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM5", 0))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM6", 0))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM7", 0))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM8", 0))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM9", 0))
  CHECK_BB(importer_enum_end(importer))
  
  CHECK_BB(importer_class_ini(importer, &t_enum, 0, NULL))
  
    CHECK_BB(importer_enum_ini(importer, 0))
    CHECK_BB(importer_enum_add(importer, "ENUM0", 0))
    CHECK_BB(importer_enum_add(importer, "ENUM1", 0))
    CHECK_BB(importer_enum_add(importer, "ENUM2", 0))
    CHECK_BB(importer_enum_add(importer, "ENUM3", 0))
    CHECK_BB(importer_enum_add(importer, "ENUM4", 0))
    CHECK_BB(importer_enum_add(importer, "ENUM5", 0))
    CHECK_BB(importer_enum_add(importer, "ENUM6", 0))
    CHECK_BB(importer_enum_add(importer, "ENUM7", 0))
    CHECK_BB(importer_enum_add(importer, "ENUM8", 0))
    CHECK_BB(importer_enum_add(importer, "ENUM9", 0))
    CHECK_BB(importer_enum_end(importer))

    CHECK_BB(importer_enum_ini(importer, "test"))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM0", 0))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM1", 0))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM2", 0))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM3", 0))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM4", 0))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM5", 0))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM6", 0))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM7", 0))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM8", 0))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM9", 0))
    CHECK_BB(importer_enum_end(importer))
  
  CHECK_BB(importer_class_end(importer))

  return 1;
}
