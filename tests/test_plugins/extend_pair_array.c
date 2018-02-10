#include "type.h"
#include "instr.h"
#include "import.h"
#include "emit.h"

#include "func.h"


static struct Type_ t_pair_array_ext = { "PairArrayExt", SZ_INT };

static CTOR(pair_array_ext_ctor) { puts("!!! builtin template ctor !!!"); }
IMPORT {
  m_str types[] = { "A", "B" };
  CHECK_BB(gwi_tmpl_ini(gwi, 2, types))
  CHECK_BB(gwi_class_ini(gwi, &t_pair_array_ext, pair_array_ext_ctor, NULL))
  CHECK_BB(gwi_tmpl_end(gwi))
  Type_Decl* td  = new_type_decl(new_id_list(insert_symbol("Pair"), 0), 0, 0);
  Type_Decl* td0 = new_type_decl(new_id_list(insert_symbol("A"), 0), 0, 0);
  Type_Decl* td1 = new_type_decl(new_id_list(insert_symbol("B"), 0), 0, 0);
  Type_List tl1  = new_type_list(td1, NULL, 0);
  Type_List tl0  = new_type_list(td0, tl1, 0);
  td->types = tl0;
  Exp e = new_exp_prim_int(0, 0);
  Array_Sub array = new_array_sub(e, 0);
  add_type_decl_array(td, array, 0);
  CHECK_BB(gwi_class_ext(gwi, td))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
