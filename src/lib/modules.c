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

static DTOR(basic_dtor) {
  free(UGEN(o)->module.gen.data);
}

static TICK(gain_tick) {
  u->out = (u->in * *(m_float*)u->module.gen.data);
}

static CTOR(gain_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), gain_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  UGEN(o)->module.gen.tick = gain_tick;
  *(m_float*)UGEN(o)->module.gen.data = 1;
}


static MFUN(gain_get_gain) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->module.gen.data;
}

static MFUN(gain_set_gain) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->module.gen.data = *(m_float*)MEM(SZ_INT);
}

static GWION_IMPORT(gain) {
  GWI_OB(gwi_class_ini(gwi,  "Gain", "UGen"))
  gwi_class_xtor(gwi, gain_ctor, basic_dtor);
  gwi_func_ini(gwi, "float", "gain");
  GWI_BB(gwi_func_end(gwi, gain_get_gain, ae_flag_none))
  gwi_func_ini(gwi, "float", "gain");
  gwi_func_arg(gwi, "float", "arg0");
  GWI_BB(gwi_func_end(gwi, gain_set_gain, ae_flag_none))
  return gwi_class_end(gwi);
}

static TICK(impulse_tick) {
  u->out = *(m_float*)u->module.gen.data;
  *(m_float*)u->module.gen.data = 0;
}

static CTOR(impulse_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), impulse_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  *(m_float*)UGEN(o)->module.gen.data = 0;
}

static MFUN(impulse_get_next) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->module.gen.data;
}

static MFUN(impulse_set_next) {
  *(m_float*)RETURN = (*(m_float*)UGEN(o)->module.gen.data = *(m_float*)MEM(SZ_INT));
}

static GWION_IMPORT(impulse) {
  GWI_OB(gwi_class_ini(gwi, "Impulse", "UGen"))
  gwi_class_xtor(gwi, impulse_ctor, basic_dtor);
  gwi_func_ini(gwi, "float", "next");
  GWI_BB(gwi_func_end(gwi, impulse_get_next, ae_flag_none))
  gwi_func_ini(gwi, "float", "next");
  gwi_func_arg(gwi, "float", "arg0");
  GWI_BB(gwi_func_end(gwi, impulse_set_next, ae_flag_none))
  return gwi_class_end(gwi);
}

static TICK(fullrect_tick) {
  u->out = fabs(u->in);
}

static CTOR(fullrect_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), fullrect_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  *(m_float*)UGEN(o)->module.gen.data = 1;
}

static GWION_IMPORT(fullrect) {
  GWI_OB(gwi_class_ini(gwi, "FullRect", "UGen"))
  gwi_class_xtor(gwi, fullrect_ctor, basic_dtor);
  return gwi_class_end(gwi);
}

static TICK(halfrect_tick) {
  if(u->in > 0)
    u->out = u->in;
  else
    u->out = 0;
}

static CTOR(halfrect_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), halfrect_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  *(m_float*)UGEN(o)->module.gen.data = 1;
}

static GWION_IMPORT(halfrect) {
  GWI_OB(gwi_class_ini(gwi, "HalfRect", "UGen"))
  gwi_class_xtor(gwi, halfrect_ctor, basic_dtor);
  return gwi_class_end(gwi);
}

static TICK(step_tick) {
  u->out = *(m_float*)u->module.gen.data;
}

static CTOR(step_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), step_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  *(m_float*)UGEN(o)->module.gen.data = 0;
}

static MFUN(step_get_next) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->module.gen.data;
}

static MFUN(step_set_next) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->module.gen.data = *(m_float*)(shred->mem + SZ_INT);
}

static GWION_IMPORT(step) {
  GWI_OB(gwi_class_ini(gwi, "Step", "UGen"))
  gwi_class_xtor(gwi, step_ctor, basic_dtor);
  gwi_func_ini(gwi, "float", "next");
  GWI_BB(gwi_func_end(gwi, step_get_next, ae_flag_none))
  gwi_func_ini(gwi, "float", "next");
  gwi_func_arg(gwi, "float", "arg0");
  GWI_BB(gwi_func_end(gwi, step_set_next, ae_flag_none))
  return gwi_class_end(gwi);
}

static TICK(zerox_tick) {
  m_float in = (u->in < 0) ? -1 : (u->in > 0);
  m_float f = *(m_float*)u->module.gen.data;
  u->out = f == in ? 1 : 0;
  *(m_float*) u->module.gen.data = in;
}

static CTOR(zerox_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), zerox_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  *(m_float*)UGEN(o)->module.gen.data = 1;
}

static GWION_IMPORT(zerox) {
  GWI_OB(gwi_class_ini(gwi, "ZeroX", "UGen"))
  gwi_class_xtor(gwi, zerox_ctor, basic_dtor);
  return gwi_class_end(gwi);
}

GWION_IMPORT(modules) {
  GWI_BB(import_gain(gwi))
  GWI_BB(import_impulse(gwi))
  GWI_BB(import_fullrect(gwi))
  GWI_BB(import_halfrect(gwi))
  GWI_BB(import_step(gwi))
  return import_zerox(gwi);
}
