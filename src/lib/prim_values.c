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
  VM* vm = gwi_vm(gwi);
  ALLOC_PTR(gwi->gwion->mp, d_zero, m_float, 0.0);
  ALLOC_PTR(gwi->gwion->mp, sr,     m_float, (m_float)vm->bbq->si->sr);
  ALLOC_PTR(gwi->gwion->mp, samp,   m_float, 1.0);
  ALLOC_PTR(gwi->gwion->mp, ms,     m_float, (m_float)(*sr     / 1000.));
  ALLOC_PTR(gwi->gwion->mp, second, m_float, (m_float)*sr);
  ALLOC_PTR(gwi->gwion->mp, minute, m_float, (m_float)(*sr     * 60.0));
  ALLOC_PTR(gwi->gwion->mp, hour,   m_float, (m_float)(*minute * 60.0));
  ALLOC_PTR(gwi->gwion->mp, t_zero, m_float, 0.0);
  ALLOC_PTR(gwi->gwion->mp, pi, m_float, (m_float)M_PI);
  gwi_item_ini(gwi, "float", "samplerate");
  gwi_item_end(gwi, ae_flag_const, sr);
  gwi_item_ini(gwi, "float", "pi");
  gwi_item_end(gwi, ae_flag_const, pi);
  gwi_item_ini(gwi, "dur", "d_zero");
  gwi_item_end(gwi, ae_flag_const, d_zero);
  gwi_item_ini(gwi, "dur", "samp");
  gwi_item_end(gwi, ae_flag_const, samp);
  gwi_item_ini(gwi, "dur", "ms");
  gwi_item_end(gwi, ae_flag_const, ms);
  gwi_item_ini(gwi, "dur", "second");
  gwi_item_end(gwi, ae_flag_const, second);
  gwi_item_ini(gwi, "dur", "minute");
  gwi_item_end(gwi, ae_flag_const, minute);
  gwi_item_ini(gwi, "dur", "hour");
  gwi_item_end(gwi, ae_flag_const, hour);
  gwi_item_ini(gwi, "time", "t_zero");
  gwi_item_end(gwi, ae_flag_const, t_zero);
  return GW_OK;
}
