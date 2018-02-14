#include <stdlib.h>
#include <math.h>
#include "vm.h"
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "import.h"
#include "ugen.h"
#include "func.h"

extern struct Type_ t_osc;
static struct Type_ t_sinosc      = { "SinOsc",      SZ_INT, &t_ugen };

typedef struct {
  sp_data* sp;
  sp_osc* osc;
  m_bool is_init;
  sp_ftbl*  tbl;
  m_float   phase;
} SP_osc; // copied from ge nerated osc.c

static TICK(sinosc_tick) {
  SP_osc* ug = (SP_osc*)u->ug;
  sp_osc_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void refresh_sine(SP_osc* ug, m_int size, m_float phase) {
  if(size <= 0) {
    err_msg(INSTR_, 0, "%s size requested for sinosc. doing nothing",
            size < 0 ? "negative" : "zero");
    return;
  }
  sp_ftbl_destroy(&ug->tbl);
  sp_osc_destroy(&ug->osc);
  sp_osc_create(&ug->osc);
  sp_ftbl_create(vm->sp, &ug->tbl, size);
  sp_gen_sine(vm->sp, ug->tbl);
  sp_osc_init(vm->sp, (sp_osc*)ug->osc, ug->tbl, phase);
}

static CTOR(sinosc_ctor) {
  SP_osc* ug = malloc(sizeof(SP_osc));
  sp_osc_create(&ug->osc);
  sp_ftbl_create(shred->vm_ref->sp, &ug->tbl, 2048);
  sp_gen_sine(shred->vm_ref->sp, ug->tbl);
  sp_osc_init(shred->vm_ref->sp, ug->osc, ug->tbl, 0.);
  assign_ugen(UGEN(o), 0, 1, 0, ug);
  UGEN(o)->tick = sinosc_tick;
  ug->is_init = 1;
}

DTOR(sinosc_dtor) {
  SP_osc* ug = (SP_osc*)UGEN(o)->ug;
  sp_osc_destroy(&ug->osc);
  sp_ftbl_destroy(&ug->tbl);
  free(ug);
}

static MFUN(sinosc_size) {
  int size = *(m_int*)(shred->mem + SZ_INT);
  SP_osc* ug = (SP_osc*)UGEN(o)->ug;
  refresh_sine(ug, size, 0);
}

static MFUN(sinosc_size_phase) {
  int size    = *(m_int*)(shred->mem + SZ_INT);
  float phase = *(m_int*)(shred->mem + SZ_INT * 2);
  SP_osc* ug = (SP_osc*)UGEN(o)->ug;
  refresh_sine(ug, size, phase);
}

MFUN(sinosc_get_freq) {
  SP_osc* ug = (SP_osc*)UGEN(o)->ug;
  *(m_float*)RETURN = ug->osc->freq;
}

MFUN(sinosc_set_freq) {
  SP_osc* ug = (SP_osc*)UGEN(o)->ug;
  m_float freq = *(m_float*)(shred->mem + SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(sinosc_get_amp) {
  SP_osc* ug = (SP_osc*)UGEN(o)->ug;
  *(m_float*)RETURN = ug->osc->amp;
}

MFUN(sinosc_set_amp) {
  SP_osc* ug = (SP_osc*)UGEN(o)->ug;
  m_float amp = *(m_float*)(shred->mem + SZ_INT);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static m_bool import_sinosc(Gwi gwi) {
  CHECK_BB(gwi_class_ini(gwi,  &t_sinosc, sinosc_ctor, sinosc_dtor))
  gwi_func_ini(gwi, "void", "init", sinosc_size);
  gwi_func_arg(gwi, "int", "size");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "void", "init", sinosc_size_phase);
  gwi_func_arg(gwi, "int", "size");
  gwi_func_arg(gwi, "float", "phase");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", sinosc_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", sinosc_set_freq);
  gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", sinosc_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", sinosc_set_amp);
  gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}

static DTOR(basic_dtor) {
  free(UGEN(o)->ug);
}

static struct Type_ t_gain      = { "Gain", SZ_INT, &t_ugen };

static TICK(gain_tick) {
  u->out = (u->in * *(m_float*)u->ug);
}

static CTOR(gain_ctor) {
  assign_ugen(UGEN(o), 1, 1, 0, malloc(sizeof(m_float)));
  UGEN(o)->tick = gain_tick;
  *(m_float*)UGEN(o)->ug = 1;
}


static MFUN(gain_get_gain) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->ug;
}

static MFUN(gain_set_gain) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->ug = *(m_float*)MEM(SZ_INT);
}

static m_bool import_gain(Gwi gwi) {
  CHECK_BB(gwi_class_ini(gwi,  &t_gain, gain_ctor, basic_dtor))
  gwi_func_ini(gwi, "float", "gain", gain_get_gain);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "gain", gain_set_gain);
  gwi_func_arg(gwi, "float", "arg0");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}


static struct Type_ t_impulse      = { "Impulse", SZ_INT, &t_ugen };
static TICK(impulse_tick) {
  u->out = *(m_float*)u->ug;
  *(m_float*)u->ug = 0;
}

static CTOR(impulse_ctor) {
  assign_ugen(UGEN(o), 0, 1, 0, malloc(sizeof(m_float)));
  UGEN(o)->tick = impulse_tick;
  *(m_float*)UGEN(o)->ug = 0;
}

static MFUN(impulse_get_next) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->ug;
}

static MFUN(impulse_set_next) {
  *(m_float*)RETURN = (*(m_float*)UGEN(o)->ug = *(m_float*)MEM(SZ_INT));
}

static m_bool import_impulse(Gwi gwi) {
  CHECK_BB(gwi_class_ini(gwi,  &t_impulse, impulse_ctor, basic_dtor))
  gwi_func_ini(gwi, "float", "next", impulse_get_next);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "next", impulse_set_next);
  gwi_func_arg(gwi, "float", "arg0");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}

static struct Type_ t_fullrect = { "FullRect", SZ_INT, &t_ugen };
static TICK(fullrect_tick) {
  base_tick(u);
  u->out = fabs(u->in);
}

static CTOR(fullrect_ctor) {
  assign_ugen(UGEN(o), 1, 1, 0, malloc(sizeof(m_float)));
  UGEN(o)->tick = fullrect_tick;
  *(m_float*)UGEN(o)->ug = 1;
}

static m_bool import_fullrect(Gwi gwi) {
  CHECK_BB(gwi_class_ini(gwi,  &t_fullrect, fullrect_ctor, basic_dtor))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}

static struct Type_ t_halfrect = { "HalfRect", SZ_INT, &t_ugen };
static TICK(halfrect_tick) {
  base_tick(u);
  if(u->in > 0)
    u->out = u->in;
  else
    u->out = 0;
}

static CTOR(halfrect_ctor) {
  assign_ugen(UGEN(o), 1, 1, 0, malloc(sizeof(m_float)));
  UGEN(o)->tick = halfrect_tick;
  *(m_float*)UGEN(o)->ug = 1;
}

static m_bool import_halfrect(Gwi gwi) {
  CHECK_BB(gwi_class_ini(gwi,  &t_halfrect, halfrect_ctor, basic_dtor))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}

static struct Type_ t_step = { "Step", SZ_INT, &t_ugen };
static TICK(step_tick) {
  u->out = *(m_float*)u->ug;
}

static CTOR(step_ctor) {
  assign_ugen(UGEN(o), 0, 1, 0, malloc(sizeof(m_float)));
  UGEN(o)->tick = step_tick;
  *(m_float*)UGEN(o)->ug = 0;
}

static MFUN(step_get_next) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->ug;
}

static MFUN(step_set_next) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->ug = *(m_float*)(shred->mem + SZ_INT);
}

static m_bool import_step(Gwi gwi) {
  CHECK_BB(gwi_class_ini(gwi,  &t_step, step_ctor, basic_dtor))
  gwi_func_ini(gwi, "float", "next", step_get_next);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "next", step_set_next);
  gwi_func_arg(gwi, "float", "arg0");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}

static struct Type_ t_zerox       = { "ZeroX",       SZ_INT, &t_ugen };

static TICK(zerox_tick) {
  base_tick(u);
  m_float in = (u->in < 0) ? -1 : (u->in > 0);
  m_float f = *(m_float*)u->ug;
  u->out = f == in ? 1 : 0;
  *(m_float*) u->ug = in;
}

static CTOR(zerox_ctor) {
  assign_ugen(UGEN(o), 1, 1, 0, malloc(sizeof(m_float)));
  UGEN(o)->tick = zerox_tick;
  *(m_float*)UGEN(o)->ug = 1;
}

static m_bool import_zerox(Gwi gwi) {
  CHECK_BB(gwi_class_ini(gwi,  &t_zerox, zerox_ctor, basic_dtor))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}

m_bool import_modules(Gwi gwi) {
  CHECK_BB(import_sinosc(gwi))
  CHECK_BB(import_gain(gwi))
  CHECK_BB(import_impulse(gwi))
  CHECK_BB(import_fullrect(gwi))
  CHECK_BB(import_halfrect(gwi))
  CHECK_BB(import_step(gwi))
  CHECK_BB(import_zerox(gwi))
  return 1;
}
