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
} SP_osc; // copied from generated osc.c

TICK(sinosc_tick) {
  SP_osc* ug = (SP_osc*)u->ug;
  sp_osc_compute(ug->sp, ug->osc, NULL, &u->out);
  return 1;
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
  if(size <= 0) {
    err_msg(INSTR_, 0, "%s size requested for sinosc. doing nothing",
            size < 0 ? "negative" : "zero");
    return;
  }
  SP_osc* ug = (SP_osc*)UGEN(o)->ug;
  sp_ftbl_destroy(&ug->tbl);
  sp_osc_destroy(&ug->osc);
  sp_osc_create(&ug->osc);
  sp_ftbl_create(shred->vm_ref->sp, &ug->tbl, size);
  sp_gen_sine(shred->vm_ref->sp, ug->tbl);
  sp_osc_init(shred->vm_ref->sp, ug->osc, ug->tbl, 0.);
}

static MFUN(sinosc_size_phase) {
  int size    = *(m_int*)(shred->mem + SZ_INT);
  float phase = *(m_int*)(shred->mem + SZ_INT * 2);
  if(size <= 0) {
    err_msg(INSTR_, 0, "%s size requested for sinosc. doing nothing",
            size < 0 ? "negative" : "zero");
    return;
  }
  SP_osc* ug = (SP_osc*)UGEN(o)->ug;
  sp_ftbl_destroy(&ug->tbl);
  sp_osc_destroy(&ug->osc);
  sp_osc_create(&ug->osc);
  sp_ftbl_create(shred->vm_ref->sp, &ug->tbl, size);
  sp_gen_sine(shred->vm_ref->sp, ug->tbl);
  sp_osc_init(shred->vm_ref->sp, (sp_osc*)ug->osc, ug->tbl, phase);
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

static m_bool import_sinosc(Env env) {
  DL_Func fun;

  CHECK_BB(import_class_begin(env, &t_sinosc, env->global_nspc, sinosc_ctor, sinosc_dtor))
  dl_func_init(&fun, "void", "init", (m_uint)sinosc_size);
  dl_func_add_arg(&fun, "int", "size");
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "void", "init", (m_uint)sinosc_size_phase);
  dl_func_add_arg(&fun, "int", "size");
  dl_func_add_arg(&fun, "float", "phase");
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "float", "freq", (m_uint)sinosc_get_freq);
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "float", "freq", (m_uint)sinosc_set_freq);
  dl_func_add_arg(&fun, "float", "freq");
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "float", "amp", (m_uint)sinosc_get_amp);
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "float", "amp", (m_uint)sinosc_set_amp);
  dl_func_add_arg(&fun, "float", "amp");
  CHECK_BB(import_fun(env, &fun, 0))
  CHECK_BB(import_class_end(env))
  return 1;
}

static DTOR(basic_dtor) {
  free(UGEN(o)->ug);
}

static struct Type_ t_gain      = { "Gain", SZ_INT, &t_ugen };
static m_bool gain_tick(UGen u) {
  base_tick(u);
  u->out *= *(m_float*)u->ug;
  return 1;
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

static m_bool import_gain(Env env) {
  DL_Func fun;
  CHECK_BB(import_class_begin(env, &t_gain, env->global_nspc, gain_ctor, basic_dtor))
  dl_func_init(&fun, "float", "gain", (m_uint)gain_get_gain);
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "float", "gain", (m_uint)gain_set_gain);
  dl_func_add_arg(&fun, "float", "arg0");
  CHECK_BB(import_fun(env, &fun, 0))
  CHECK_BB(import_class_end(env))
  return 1;
}


static struct Type_ t_impulse      = { "Impulse", SZ_INT, &t_ugen };
static m_bool impulse_tick(UGen u) {
  u->out = *(m_float*)u->ug;
  *(m_float*)u->ug = 0;
  return 1;
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

static m_bool import_impulse(Env env) {
  DL_Func fun;
  CHECK_BB(import_class_begin(env, &t_impulse, env->global_nspc, impulse_ctor, basic_dtor))
  dl_func_init(&fun, "float", "next", (m_uint)impulse_get_next);
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "float", "next", (m_uint)impulse_set_next);
  dl_func_add_arg(&fun, "float", "arg0");
  CHECK_BB(import_fun(env, &fun, 0))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_fullrect = { "FullRect", SZ_INT, &t_ugen };
static m_bool fullrect_tick(UGen u) {
  base_tick(u);
  u->out = fabs(u->in);
  return 1;
}

static CTOR(fullrect_ctor) {
  assign_ugen(UGEN(o), 1, 1, 0, malloc(sizeof(m_float)));
  UGEN(o)->tick = fullrect_tick;
  *(m_float*)UGEN(o)->ug = 1;
}

static m_bool import_fullrect(Env env) {
  CHECK_BB(import_class_begin(env, &t_fullrect, env->global_nspc, fullrect_ctor, basic_dtor))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_halfrect = { "HalfRect", SZ_INT, &t_ugen };
static m_bool halfrect_tick(UGen u) {
  base_tick(u);
  if(u->in > 0)
    u->out = u->in;
  else
    u->out = 0;
  return 1;
}

static CTOR(halfrect_ctor) {
  assign_ugen(UGEN(o), 1, 1, 0, malloc(sizeof(m_float)));
  UGEN(o)->tick = halfrect_tick;
  *(m_float*)UGEN(o)->ug = 1;
}

static m_bool import_halfrect(Env env) {
  CHECK_BB(import_class_begin(env, &t_halfrect, env->global_nspc, halfrect_ctor, basic_dtor))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_step = { "Step", SZ_INT, &t_ugen };
static m_bool step_tick(UGen u) {
  u->out = *(m_float*)u->ug;
  return 1;
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

static m_bool import_step(Env env) {
  DL_Func fun;
  CHECK_BB(import_class_begin(env, &t_step, env->global_nspc, step_ctor, basic_dtor))
  dl_func_init(&fun, "float", "next", (m_uint)step_get_next);
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "float", "next", (m_uint)step_set_next);
  dl_func_add_arg(&fun, "float", "arg0");
  CHECK_BB(import_fun(env, &fun, 0))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_zerox       = { "ZeroX",       SZ_INT, &t_ugen };

static m_bool zerox_tick(UGen u) {
  base_tick(u);
  m_float in = (u->in < 0) ? -1 : (u->in > 0);
  m_float f = *(m_float*)u->ug;
  u->out = f == in ? 1 : 0;
  *(m_float*) u->ug = in;
  return 1;
}

static CTOR(zerox_ctor) {
  assign_ugen(UGEN(o), 1, 1, 0, malloc(sizeof(m_float)));
  UGEN(o)->tick = zerox_tick;
  *(m_float*)UGEN(o)->ug = 1;
}

static m_bool import_zerox(Env env) {
  CHECK_BB(import_class_begin(env, &t_zerox, env->global_nspc, zerox_ctor, basic_dtor))
  CHECK_BB(import_class_end(env))
  return 1;
}

m_bool import_modules(Env env) {
  CHECK_BB(import_sinosc(env))
  CHECK_BB(import_gain(env))
  CHECK_BB(import_impulse(env))
  CHECK_BB(import_fullrect(env))
  CHECK_BB(import_halfrect(env))
  CHECK_BB(import_step(env))
  CHECK_BB(import_zerox(env))
  return 1;
}
