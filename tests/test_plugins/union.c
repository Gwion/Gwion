#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_union= { "Union", SZ_INT, &t_object };

IMPORT
{
  CHECK_BB(importer_class_ini(importer, &t_union, NULL, NULL))
  CHECK_BB(importer_union_add(importer,"float", "f", 0))
  CHECK_BB(importer_union_add(importer,"int", "i", 0))
  /*CHECK_BB(importer_union_add(importer,"Object", "o", ae_flag_ref))*/
  CHECK_BB(importer_union_end(importer))
  CHECK_BB(importer_class_end(importer))
printf("%lu\n", t_union.info->offset);
printf("%lu\n", t_union.obj_size);
  return 1;
}
