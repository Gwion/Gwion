#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "emit.h"
#include "traverse.h"
#include "template.h"
#include "parse.h"
#include "specialid.h"
#include "gwi.h"
#include "tmpl_info.h"

OP_CHECK(opck_foreach_scan) {
  struct TemplateScan *ts = (struct TemplateScan*)data;
  struct tmpl_info info = { .base=ts->t, .td=ts->td, .list=ts->t->info->cdef->base.tmpl->list  };
  const Type exists = tmpl_exists(env, &info);
  if(exists)
    return exists != env->gwion->type[et_error] ? exists : NULL;
  const Type base = known_type(env, ts->td->types->td);
  const Type t = new_type(env->gwion->mp, s_name(info.name), base);
  return t;
}

GWION_IMPORT(foreach) {
  const Type t_foreach = gwi_struct_ini(gwi, "@Foreach:[A]");
  set_tflag(t_foreach, tflag_ntmpl);
  GWI_BB(gwi_item_ini(gwi, "@internal", "val"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, NULL))
  GWI_BB(gwi_struct_end(gwi))
  GWI_BB(gwi_oper_ini(gwi, "@Foreach", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_foreach_scan))
  GWI_BB(gwi_oper_end(gwi, "@scan", NULL))
  return GW_OK;
}
