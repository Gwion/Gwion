#include "type.h"
#include "instr.h"
#include "import.h"
#include "emit.h"

#include "func.h"


static struct Type_ t_ev = { "Ev", SZ_INT };

static CTOR(ev_ctor) { printf(" %p this to test ctor\n", (void*)o); }

IMPORT {
  CHECK_BB(gwi_class_ini(gwi, &t_ev, ev_ctor, NULL))
//  CHECK_BB(gwi_class_ini(gwi, &t_ev, NULL, NULL))
  Type_Decl* td = new_type_decl(new_id_list(insert_symbol("Event"), 0), 0, 0);
  CHECK_BB(gwi_class_ext(gwi, td))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
