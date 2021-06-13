#include <stdlib.h>
#include <math.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "gwi.h"

static TICK(trig_tick) { u->out = u->module.gen.trig->in ? u->in : 0; }

static CTOR(trig_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), trig_tick, (void *)1, 1);
}

static CTOR(trig2_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), trig_tick, (void *)1, 1);
}

GWION_IMPORT(trig) {
  GWI_OB(gwi_class_ini(gwi, "Trig", "UGen"))
  gwi_class_xtor(gwi, trig_ctor, NULL);
  gwi_class_end(gwi);

  GWI_OB(gwi_class_ini(gwi, "Trig2", "UGen"))
  gwi_class_xtor(gwi, trig2_ctor, NULL);
  return gwi_class_end(gwi);
}
