#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_enum = { "Enum", SZ_INT, &t_object };

IMPORT {
  CHECK_BB(importer_enum_ini(importer, NULL))
  CHECK_BB(importer_enum_add(importer, "ENUM0"))
  CHECK_BB(importer_enum_add(importer, "ENUM1"))
  CHECK_BB(importer_enum_add(importer, "ENUM2"))
  CHECK_BB(importer_enum_add(importer, "ENUM3"))
  CHECK_BB(importer_enum_add(importer, "ENUM4"))
  CHECK_BB(importer_enum_add(importer, "ENUM5"))
  CHECK_BB(importer_enum_add(importer, "ENUM6"))
  CHECK_BB(importer_enum_add(importer, "ENUM7"))
  CHECK_BB(importer_enum_add(importer, "ENUM8"))
  CHECK_BB(importer_enum_add(importer, "ENUM9"))
  CHECK_BB(importer_enum_end(importer))

  CHECK_BB(importer_enum_ini(importer, "test"))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM0"))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM1"))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM2"))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM3"))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM4"))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM5"))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM6"))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM7"))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM8"))
  CHECK_BB(importer_enum_add(importer, "TYPED_ENUM9"))
  CHECK_BB(importer_enum_end(importer))
  
  CHECK_BB(importer_class_ini(importer, &t_enum, NULL, NULL))
  
    CHECK_BB(importer_enum_ini(importer, NULL))
    CHECK_BB(importer_enum_add(importer, "ENUM0"))
    CHECK_BB(importer_enum_add(importer, "ENUM1"))
    CHECK_BB(importer_enum_add(importer, "ENUM2"))
    CHECK_BB(importer_enum_add(importer, "ENUM3"))
    CHECK_BB(importer_enum_add(importer, "ENUM4"))
    CHECK_BB(importer_enum_add(importer, "ENUM5"))
    CHECK_BB(importer_enum_add(importer, "ENUM6"))
    CHECK_BB(importer_enum_add(importer, "ENUM7"))
    CHECK_BB(importer_enum_add(importer, "ENUM8"))
    CHECK_BB(importer_enum_add(importer, "ENUM9"))
    CHECK_BB(importer_enum_end(importer))

    CHECK_BB(importer_enum_ini(importer, "test"))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM0"))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM1"))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM2"))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM3"))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM4"))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM5"))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM6"))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM7"))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM8"))
    CHECK_BB(importer_enum_add(importer, "TYPED_ENUM9"))
    CHECK_BB(importer_enum_end(importer))
  
  CHECK_BB(importer_class_end(importer))

  return 1;
}
