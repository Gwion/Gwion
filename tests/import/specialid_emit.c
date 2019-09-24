#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "object.h"
#include "instr.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "emit.h"
#include "specialid.h"


static ID_EMIT(spidem) {
  const Instr instr = emit_add_instr(emit, RegPushImm);
  instr->m_val = 1234;
  return instr;
}

GWION_IMPORT(spid_test) {
  struct SpecialId_ spid = { .type=t_int, .em=spidem, .is_const = 1 };
  gwi_specialid(gwi, "testid", &spid);
  return GW_OK;
}
