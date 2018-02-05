#include <stdlib.h>
#include "type.h"
#include "import.h"

IMPORT {
  CHECK_BB(importer_item_ini(importer,"int", "i"))
  CHECK_BB(importer_item_end(importer, 0, NULL))
  return 1;
}
