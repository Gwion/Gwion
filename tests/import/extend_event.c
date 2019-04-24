#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "instr.h"
#include "import.h"
#include "vm.h"
#include "gwion.h"
#include "gwi.h"

static CTOR(ev_ctor) { printf(" %p this to test ctor\n", (void*)o); }

GWION_IMPORT(extend_event_test) {
  Type t_ev ;
  CHECK_OB((t_ev = gwi_mk_type(gwi, "Ev", SZ_INT , NULL)))
  CHECK_BB(gwi_class_ini(gwi, t_ev, ev_ctor, NULL))
  Type_Decl* td = new_type_decl(gwi->gwion->st->p, new_id_list(gwi->gwion->st->p, insert_symbol(gwi->gwion->st, "Event"), GWI_LOC), 0);
  CHECK_BB(gwi_class_ext(gwi, td))
  CHECK_BB(gwi_class_end(gwi))
  return GW_OK;
}
