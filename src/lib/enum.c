#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "gack.h"

static GACK(gack_enum) {
  const Map m = &t->nspc->info->value->map;
  const m_uint value = *(m_uint*)VALUE;
  for(m_uint i = 0; i < map_size(m); i++) {
    const Value v = (Value)map_at(&t->nspc->info->value->map, i);
    if((m_uint)v->d.ptr == value) {
      INTERP_PRINTF("%s", v->name);
      return;
    }
  }
  INTERP_PRINTF("%s", t->name);
}

GWION_IMPORT(enum) {
  const Type t_enum = gwi_mk_type(gwi, "enum", SZ_INT, "int");
  gwi_set_global_type(gwi, t_enum, et_enum);
  gwi_gack(gwi, t_enum, gack_enum);
  return GW_OK;
}
