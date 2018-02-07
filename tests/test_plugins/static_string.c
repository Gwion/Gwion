#include <stdlib.h>
#include "type.h"
#include "import.h"
#include "object.h"

static struct Type_ t_ss = { "StaticString", 0, &t_object };

IMPORT
{
//  CHECK_BB(importer_class_ini(importer, &t_ss, NULL, NULL))
  CHECK_BB(importer_item_ini(importer, "string", "self"))
  M_Object obj = new_String(NULL, "test static string");
//  CHECK_BB(importer_add_value(importer, "self", &t_string, 1, obj))
//  ALLOC_PTR(obj, M_Object, new_String(NULL, "test static string"));
  CHECK_BB(importer_item_end(importer, ae_flag_unary | ae_flag_global, obj))
//  CHECK_BB(importer_class_end(importer))
  return 1;
}
