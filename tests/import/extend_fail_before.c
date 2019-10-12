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
#include "value.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

GWION_IMPORT(extend_event_test) {
  Type_Decl* td = new_type_decl(gwi->gwion->st->p, new_id_list(gwi->gwion->st->p, insert_symbol(gwi->gwion->st, "Event"), GWI_LOC));
  return gwi_class_ext(gwi, td);
}
