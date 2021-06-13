#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "emit.h"
#include "specialid.h"
#include "gwi.h"

static ID_EMIT(spidem) {
  const Instr instr = emit_add_instr(emit, RegPushImm);
  instr->m_val      = 1234;
  return instr;
}

GWION_IMPORT(spid_test) {
  struct SpecialId_ spid = {
      .type = gwi->gwion->type[et_int], .em = spidem, .is_const = 1};
  gwi_specialid(gwi, "testid", &spid);
  return GW_OK;
}
