#include "type.h"
#include "import.h"
#include "object.h"
#include "emit.h"

static struct Type_ t_func_typedef = { "FuncTypedef", SZ_INT, &t_object };

static MFUN(test_func) { puts("test"); }
IMPORT {
  CHECK_BB(gwi_fptr_ini(gwi, "void", "PtrType"))
  CHECK_BB(gwi_fptr_end(gwi, 0))
  
  CHECK_BB(gwi_class_ini(gwi, &t_func_typedef, NULL, NULL))
    CHECK_BB(gwi_fptr_ini(gwi, "void", "PtrType"))
    CHECK_BB(gwi_fptr_end(gwi, ae_flag_static))
    CHECK_BB(gwi_func_ini(gwi, "void", "test_func", test_func))
    CHECK_BB(gwi_func_end(gwi, ae_flag_static))
    CHECK_BB(gwi_item_ini(gwi, "PtrType", "ptr"))
    CHECK_BB(gwi_item_end(gwi, ae_flag_static, NULL))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
