#include "type.h"
#include "instr.h"
#include "import.h"

GWION_IMPORT(extend_pair_test) {
  m_str types[] = { "A", "B" };
  Type t_pair_ext ;
  CHECK_OB((t_pair_ext = gwi_mk_type(gwi, "PairExt", SZ_INT , NULL)))
  CHECK_BB(gwi_tmpl_ini(gwi, 2, types))
  CHECK_BB(gwi_class_ini(gwi, t_pair_ext, NULL, NULL))
  CHECK_BB(gwi_tmpl_end(gwi))
  Type_Decl* td  = new_type_decl(new_id_list(insert_symbol("Pair"), 0), 0, 0);
  Type_Decl* td0 = new_type_decl(new_id_list(insert_symbol("A"), 0), 0, 0);
  Type_Decl* td1 = new_type_decl(new_id_list(insert_symbol("B"), 0), 0, 0);
  Type_List tl1  = new_type_list(td1, NULL);
  Type_List tl0  = new_type_list(td0, tl1);
  td->types = tl0;
  CHECK_BB(gwi_class_ext(gwi, td))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
