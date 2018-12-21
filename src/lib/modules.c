#include <stdlib.h>
#include <math.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "vm.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "import.h"
#include "ugen.h"
#include "func.h"

static DTOR(basic_dtor) {
  free(UGEN(o)->module.gen.data);
}

static TICK(gain_tick) {
  u->out = (u->in * *(m_float*)u->module.gen.data);
}

static CTOR(gain_ctor) {
  ugen_ini(UGEN(o), 1, 1);
  ugen_gen(UGEN(o), gain_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
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
  const Type t_gain = gwi_mk_type(gwi, "Gain", SZ_INT, t_ugen);
  CHECK_BB(gwi_class_ini(gwi,  t_gain, gain_ctor, basic_dtor))
  gwi_func_ini(gwi, "float", "gain", gain_get_gain);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "gain", gain_set_gain);
  gwi_func_arg(gwi, "float", "arg0");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}

static TICK(impulse_tick) {
  u->out = *(m_float*)u->module.gen.data;
  *(m_float*)u->module.gen.data = 0;
}

static CTOR(impulse_ctor) {
  ugen_ini(UGEN(o), 0, 1);
  ugen_gen(UGEN(o), impulse_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  *(m_float*)UGEN(o)->module.gen.data = 0;
}

static MFUN(impulse_get_next) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->module.gen.data;
}

static MFUN(impulse_set_next) {
  *(m_float*)RETURN = (*(m_float*)UGEN(o)->module.gen.data = *(m_float*)MEM(SZ_INT));
}

static GWION_IMPORT(impulse) {
  const Type t_impulse = gwi_mk_type(gwi, "Impulse", SZ_INT, t_ugen);
  CHECK_BB(gwi_class_ini(gwi,  t_impulse, impulse_ctor, basic_dtor))
  gwi_func_ini(gwi, "float", "next", impulse_get_next);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "next", impulse_set_next);
  gwi_func_arg(gwi, "float", "arg0");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}

static TICK(fullrect_tick) {
  u->out = fabs(u->in);
}

static CTOR(fullrect_ctor) {
  ugen_ini(UGEN(o), 1, 1);
  ugen_gen(UGEN(o), fullrect_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  *(m_float*)UGEN(o)->module.gen.data = 1;
}

static GWION_IMPORT(fullrect) {
  const Type t_fullrect = gwi_mk_type(gwi, "FullRect", SZ_INT, t_ugen);
  CHECK_BB(gwi_class_ini(gwi,  t_fullrect, fullrect_ctor, basic_dtor))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}

static TICK(halfrect_tick) {
  if(u->in > 0)
    u->out = u->in;
  else
    u->out = 0;
}

static CTOR(halfrect_ctor) {
  ugen_ini(UGEN(o), 1, 1);
  ugen_gen(UGEN(o), halfrect_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  *(m_float*)UGEN(o)->module.gen.data = 1;
}

static GWION_IMPORT(halfrect) {
  const Type t_halfrect = gwi_mk_type(gwi, "HalfRect", SZ_INT, t_ugen);
  CHECK_BB(gwi_class_ini(gwi,  t_halfrect, halfrect_ctor, basic_dtor))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}

static TICK(step_tick) {
  u->out = *(m_float*)u->module.gen.data;
}

static CTOR(step_ctor) {
  ugen_ini(UGEN(o), 0, 1);
  ugen_gen(UGEN(o), step_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  *(m_float*)UGEN(o)->module.gen.data = 0;
}

static MFUN(step_get_next) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->module.gen.data;
}

static MFUN(step_set_next) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->module.gen.data = *(m_float*)(shred->mem + SZ_INT);
}

static GWION_IMPORT(step) {
  const Type t_step = gwi_mk_type(gwi, "Step", SZ_INT, t_ugen);
  CHECK_BB(gwi_class_ini(gwi,  t_step, step_ctor, basic_dtor))
  gwi_func_ini(gwi, "float", "next", step_get_next);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "next", step_set_next);
  gwi_func_arg(gwi, "float", "arg0");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}

static TICK(zerox_tick) {
  m_float in = (u->in < 0) ? -1 : (u->in > 0);
  m_float f = *(m_float*)u->module.gen.data;
  u->out = f == in ? 1 : 0;
  *(m_float*) u->module.gen.data = in;
}

static CTOR(zerox_ctor) {
  ugen_ini(UGEN(o), 1, 1);
  ugen_gen(UGEN(o), zerox_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  *(m_float*)UGEN(o)->module.gen.data = 1;
}

static GWION_IMPORT(zerox) {
  const Type t_zerox = gwi_mk_type(gwi, "ZeroX", SZ_INT, t_ugen);
  CHECK_BB(gwi_class_ini(gwi, t_zerox, zerox_ctor, basic_dtor))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}

GWION_IMPORT(modules) {
  CHECK_BB(import_gain(gwi))
  CHECK_BB(import_impulse(gwi))
  CHECK_BB(import_fullrect(gwi))
  CHECK_BB(import_halfrect(gwi))
  CHECK_BB(import_step(gwi))
  CHECK_BB(import_zerox(gwi))
  return 1;
}
