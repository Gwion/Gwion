#include "type.h"
#include "import.h"
#include "object.h"
#include "emit.h"

static struct Type_ t_func_typedef = { "FuncTypedef", SZ_INT, &t_object };

static MFUN(test_func) { puts("test"); }
IMPORT {
  CHECK_BB(importer_fptr_ini(importer, "void", "PtrType"))
  CHECK_BB(importer_fptr_end(importer, 0))
  
  CHECK_BB(importer_class_ini(importer, &t_func_typedef, NULL, NULL))
    CHECK_BB(importer_fptr_ini(importer, "void", "PtrType"))
    CHECK_BB(importer_fptr_end(importer, ae_flag_static))
    CHECK_BB(importer_func_ini(importer, "void", "test_func", (m_uint)test_func))
    CHECK_BB(importer_func_end(importer, ae_flag_static))
    CHECK_BB(importer_item_ini(importer, "PtrType", "ptr"))
    CHECK_BB(importer_item_end(importer, ae_flag_static, NULL))
  CHECK_BB(importer_class_end(importer))
  return 1;
}
