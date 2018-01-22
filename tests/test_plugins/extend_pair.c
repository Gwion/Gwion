#include "type.h"
#include "instr.h"
#include "import.h"
#include "emit.h"

#include "func.h"


static struct Type_ t_pair_ext = { "PairExt", SZ_INT };

IMPORT {
  m_str types[] = { "A", "B" };
  CHECK_BB(importer_tmpl_ini(importer, 2, types))
  CHECK_BB(importer_class_ini(importer, &t_pair_ext, NULL, NULL))
  CHECK_BB(importer_tmpl_end(importer))
  Type_Decl* td  = new_type_decl(new_id_list(insert_symbol("Pair"), 0), 0, 0);
  Type_Decl* td0 = new_type_decl(new_id_list(insert_symbol("A"), 0), 0, 0);
  Type_Decl* td1 = new_type_decl(new_id_list(insert_symbol("B"), 0), 0, 0);
  Type_List tl1  = new_type_list(td1, NULL, 0);
  Type_List tl0  = new_type_list(td0, tl1, 0);
  td->types = tl0;
  CHECK_BB(importer_class_ext(importer, td))
  CHECK_BB(importer_class_end(importer))
  return 1;
}
