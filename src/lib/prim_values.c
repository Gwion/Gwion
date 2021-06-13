#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "emit.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "driver.h"
#include "traverse.h"
#include "parse.h"
#include "specialid.h"
#include "array.h"
#include "gack.h"

GWION_IMPORT(values) {
  VM *          vm = gwi_vm(gwi);
  const m_float sr = vm->bbq->si->sr;
  gwi_item_ini(gwi, "float", "samplerate");
  gwi_item_end(gwi, ae_flag_const, fnum, sr);
  gwi_item_ini(gwi, "float", "pi");
  gwi_item_end(gwi, ae_flag_const, fnum, M_PI);
  gwi_item_ini(gwi, "dur", "d_zero");
  gwi_item_end(gwi, ae_flag_const, fnum, 0.0);
  gwi_item_ini(gwi, "dur", "samp");
  gwi_item_end(gwi, ae_flag_const, fnum, 1.0);
  gwi_item_ini(gwi, "dur", "ms");
  gwi_item_end(gwi, ae_flag_const, fnum, sr / 1000.0);
  gwi_item_ini(gwi, "dur", "second");
  gwi_item_end(gwi, ae_flag_const, fnum, sr);
  gwi_item_ini(gwi, "dur", "minute");
  gwi_item_end(gwi, ae_flag_const, fnum, sr * 60.0);
  gwi_item_ini(gwi, "dur", "hour");
  gwi_item_end(gwi, ae_flag_const, fnum, sr * 60 * 60);
  gwi_item_ini(gwi, "time", "t_zero");
  gwi_item_end(gwi, ae_flag_const, fnum, 0.0);
  return GW_OK;
}
