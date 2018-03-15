#include <stdlib.h>
#include "type.h"
#include "import.h"
#include "object.h"

static struct Type_ t_ss = { "StaticString", 0, t_object };

IMPORT
{
//  CHECK_BB(gwi_class_ini(gwi, &t_ss, NULL, NULL))
  CHECK_BB(gwi_item_ini(gwi, "string", "self"))
  M_Object obj = new_String(NULL, "test static string");
//  CHECK_BB(gwi_add_value(gwi, "self", t_string, 1, obj))
//  ALLOC_PTR(obj, M_Object, new_String(NULL, "test static string"));
  CHECK_BB(gwi_item_end(gwi, ae_flag_global, obj))
//  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
