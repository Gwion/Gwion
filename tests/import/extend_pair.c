#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "instr.h"
#include "vm.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

GWION_IMPORT(extend_pair_test) {
  m_str types[] = { "A", "B" };
  Type t_pair_ext ;
  CHECK_OB((t_pair_ext = gwi_mk_type(gwi, "PairExt", SZ_INT , NULL)))
  CHECK_BB(gwi_tmpl_ini(gwi, 2, types))
  CHECK_BB(gwi_class_ini(gwi, t_pair_ext, NULL, NULL))
  CHECK_BB(gwi_tmpl_end(gwi))
  Type_Decl* td  = new_type_decl(gwi->gwion->st->p, new_id_list(gwi->gwion->st->p, insert_symbol(gwi->gwion->st, "Pair"), GWI_LOC));
  Type_Decl* td0 = new_type_decl(gwi->gwion->st->p ,new_id_list(gwi->gwion->st->p, insert_symbol(gwi->gwion->st, "A"), GWI_LOC));
  Type_Decl* td1 = new_type_decl(gwi->gwion->st->p ,new_id_list(gwi->gwion->st->p, insert_symbol(gwi->gwion->st, "B"),  GWI_LOC));
  Type_List tl1  = new_type_list(gwi->gwion->st->p, td1, NULL);
  Type_List tl0  = new_type_list(gwi->gwion->st->p,td0, tl1);
  td->types = tl0;
  CHECK_BB(gwi_class_ext(gwi, td))
  CHECK_BB(gwi_class_end(gwi))
  return GW_OK;
}
