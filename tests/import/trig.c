#include <stdlib.h>
#include <math.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "vm.h"
#include "gwion.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "value.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "func.h"
#include "gwi.h"

static TICK(trig_tick) {
  u->out = u->module.gen.trig->in ? u->in : 0;
}

static CTOR(trig_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), trig_tick, (void*)1, 1);
}


GWION_IMPORT(trig) {
  const Type t_gain = gwi_mk_type(gwi, "Trig", SZ_INT, "UGen");
  GWI_BB(gwi_class_ini(gwi, t_gain, trig_ctor, NULL))
  return gwi_class_end(gwi);
}
