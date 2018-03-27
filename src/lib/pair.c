#include <string.h>
#include "type.h"
#include "instr.h"
#include "import.h"

static MFUN(gwf_pair_set) {
  const m_uint sz = sizeof(o->data);
  memcpy(o->data, MEM(SZ_INT), sz);
}

m_bool import_pair(Gwi gwi) {
  Type t_pair;
  const m_str list[] = { "A" , "B"};
  CHECK_OB((t_pair = gwi_mk_type(gwi, "Pair", SZ_INT, t_object)))
  CHECK_BB(gwi_tmpl_ini(gwi, 2, list))
  CHECK_BB(gwi_class_ini(gwi, t_pair, NULL, NULL))
  CHECK_BB(gwi_tmpl_end(gwi))
  CHECK_BB(gwi_item_ini(gwi, "A", "key"))
  CHECK_BB(gwi_item_end(gwi, 0, NULL))
  CHECK_BB(gwi_item_ini(gwi, "B", "val"))
  CHECK_BB(gwi_item_end(gwi, 0, NULL))
  CHECK_BB(gwi_func_ini(gwi, "void", "set", gwf_pair_set))
  CHECK_BB(gwi_func_arg(gwi, "A", "key"))
  CHECK_BB(gwi_func_arg(gwi, "B", "val"))
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
