#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "oo.h"
#include "env.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "import.h"

static CTOR(ev_ctor) { printf(" %p this to test ctor\n", (void*)o); }

GWION_IMPORT(extend_event_test) {
  Type t_ev ;
  CHECK_OB((t_ev = gwi_mk_type(gwi, "Ev", SZ_INT , NULL)))
  CHECK_BB(gwi_class_ini(gwi, t_ev, ev_ctor, NULL))
  Type_Decl* td = new_type_decl(new_id_list(insert_symbol("Event"), 0), 0);
  CHECK_BB(gwi_class_ext(gwi, td))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
