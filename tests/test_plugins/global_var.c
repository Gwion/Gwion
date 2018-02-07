#include <stdlib.h>
#include "type.h"
#include "import.h"
#include "object.h"


IMPORT {
//  ALLOC_PTR(i, m_uint, 1);
  M_Object i = new_String(NULL, "test");
  CHECK_BB(importer_item_ini(importer,"string", "i"))
  CHECK_BB(importer_item_end(importer, 0, i))
  return 1;
}
