#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

IMPORT
{
  CHECK_BB(importer_union_ini(importer, NULL))
  CHECK_BB(importer_union_add(importer,"float", "f"))
  CHECK_BB(importer_union_add(importer,"int", "i"))
  /*CHECK_BB(importer_union_add(importer,"Object", "o", ae_flag_ref))*/
  CHECK_BB(importer_union_end(importer, 0))
  return 1;
}
