#include "type.h"
#include "import.h"

struct Type_ t_pair = { "Pair", SZ_INT, &t_object};

m_bool import_pair(Importer importer) {
  const m_str list[] = { "A" , "B"};
  CHECK_BB(importer_tmpl_ini(importer, 2, list))
  CHECK_BB(importer_class_ini(importer, &t_pair, NULL, NULL))
  CHECK_BB(importer_tmpl_end(importer))
  CHECK_BB(importer_item_ini(importer, "A", "key"))
  CHECK_BB(importer_item_end(importer, 0, NULL))
  CHECK_BB(importer_item_ini(importer, "B", "val"))
  CHECK_BB(importer_item_end(importer, 0, NULL))
  CHECK_BB(importer_class_end(importer))
  return 1;
}
