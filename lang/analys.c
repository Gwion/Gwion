#include <stdlib.h>
#include <math.h>
#include "absyn.h"
#include "err_msg.h"
#include "instr.h"
#include "import.h"
#include "ugen.h"

typedef struct {
  unsigned int size;
  unsigned int pos;
  m_float* data;
  Vector curr;
} sp_buffer;



static m_int sp_buffer_create(sp_buffer** p, m_uint size) {
  *p = malloc(sizeof(sp_buffer));
  (*p)->size = size;
  (*p)->pos = 0;
  (*p)->data = calloc(size, sizeof(m_float));
  return SP_OK;
}

static m_int sp_buffer_destroy(sp_buffer* buffer) {
  free(buffer->data);
  free(buffer);
  return SP_OK;
}

static m_int sp_buffer_add(sp_buffer* buffer, m_float f) {
  buffer->data[ buffer->pos ] = f;
  buffer->pos++;
  buffer->pos %= buffer->size;
  return SP_OK;
}

static m_float* sp_buffer_get(sp_buffer* buffer) {
  m_uint i;
  m_float* ret = calloc(buffer->size, sizeof(m_float));
  for(i = 0; i < buffer->size; i++)
    ret[i] = buffer->data[(buffer->pos + i) % buffer->size];
  return ret;
}

static struct Type_ t_fft = { "FFT", SZ_INT, &t_ugen};
typedef struct {
  sp_data* sp;
  sp_buffer*  buf;
  /*  m_float*  (*win)(m_float* buf, m_uint size);*/
  FFTFREQS*    frq;
  FFTwrapper* fft;
  m_uint last;
} Fft;


static m_bool fft_tick(UGen u) {
  Fft* ana = (Fft*)u->ug;
  base_tick(u);                         // compute inputs
  if(!ana->buf)
    return 1;
  sp_buffer_add(ana->buf, u->in);      // add them to buffer
  if(u->trig) {
    base_tick(UGEN(u->trig));
    if(ana->last == ana->sp->pos || UGEN(u->trig)->out) {  // if trigged, compute fft
      m_float* smp = sp_buffer_get(ana->buf);
      /*    if(ana->win)*/                  // do windowing
      /*      ana->win(smp, ana->buf->size);*/
      smps2freqs(ana->fft, smp, ana->frq);
      free(smp);
      ana->last = ana->sp->pos;
    }
  }
  return 1;
}

static CTOR(fft_ctor) {
  Fft* fft = UGEN(o)->ug = calloc(1, sizeof(Fft));
  assign_ugen(UGEN(o), 1, 1, 1, fft);
  UGEN(o)->tick = fft_tick;
  fft->sp = shred->vm_ref->sp;
}

static DTOR(fft_dtor) {
  Fft* ana = (Fft*)UGEN(o)->ug;
  if(ana->buf)
    sp_buffer_destroy(ana->buf);
  if(ana->frq) {
    deleteFFTFREQS(ana->frq);
    free(ana->frq);
  }
  if(ana->fft)
    FFTwrapper_destroy(&ana->fft);
  free(ana);
}

static MFUN(fft_init) {
  Fft* ana = (Fft*)UGEN(o)->ug;
  m_int size = *(m_int*)MEM(SZ_INT);
  if(size <= 0 || size % 2)Except(shred, "FftInvalidSizeException.")
    if(ana->buf)
      sp_buffer_destroy(ana->buf);
  if(ana->frq) {
    deleteFFTFREQS(ana->frq);
    free(ana->frq);
  }
  if(ana->fft)
    FFTwrapper_destroy(&ana->fft);
  sp_buffer_create(&ana->buf, size);
  ana->frq = calloc(size, sizeof(FFTFREQS));
  newFFTFREQS(ana->frq, size);
  FFTwrapper_create(&ana->fft, size);
  ana->frq->size = size;
  *(m_uint*)RETURN = size;
}

static MFUN(fft_compute) {
  m_float* smp;
  Fft* ana = (Fft*)UGEN(o)->ug;
  if(!ana || ana->sp->pos == ana->last || !ana->buf) {
    *(m_uint*)RETURN = 0;
    return;
  }
  smp = sp_buffer_get(ana->buf);
  if(smp)
    smps2freqs(ana->fft, smp, ana->frq);
  free(smp);
  ana->last = ana->sp->pos;
}

static m_bool import_fft(Env env) {
  DL_Func fun;
  CHECK_BB(import_class_begin(env, &t_fft, env->global_nspc, fft_ctor, fft_dtor))
  dl_func_init(&fun, "int", "init", (m_uint)fft_init);
  dl_func_add_arg(&fun, "int", "size");
  CHECK_BB(import_fun(env, &fun, 0))
  /*  dl_func_init(&fun, "int", "init", (m_uint)fft_init2);*/
  /*    dl_func_add_arg(&fun, "int", "size");*/
  /*    dl_func_add_arg(&fun, "float[]", "window");*/
  /*  CHECK_BB(import_fun(env, &fun, 0))*/
  /*  dl_func_init(&fun, "int", "init", (m_uint)fft_init3);*/
  /*    dl_func_add_arg(&fun, "int", "size");*/
  /*    dl_func_add_arg(&fun, "string", "window");*/
  /*  CHECK_BB(import_fun(env, &fun, 0))*/
  /*  dl_func_init(&fun, "int", "window", (m_uint)fft_win);*/
  /*    dl_func_add_arg(&fun, "float[]", "window");*/
  /*  CHECK_BB(import_fun(env, &fun, 0))*/
  /*  dl_func_init(&fun, "int", "window", (m_uint)fft_win_name);*/
  /*    dl_func_add_arg(&fun, "string", "name");*/
  /*  CHECK_BB(import_fun(env, &fun, 0))*/
  /*  dl_func_init(&fun, "complex[]", "compute", (m_uint)fft_compute);*/
  dl_func_init(&fun, "void", "compute", (m_uint)fft_compute);
  CHECK_BB(import_fun(env, &fun, 0))

  CHECK_BB(import_class_end(env))
  return 1;
}

typedef struct Ana {
  m_uint size, sr;
  m_float* fval;
  m_float  percent;     // rollof
  /*  m_float* norm, *prev; // flux*/
  /*  m_float* cval[2];*/ // corr
  sp_data* sp;
  m_uint last;
} Ana;

typedef double (*f_analys)(Ana* fft);
/*
m_float array_max(m_float* f, unsigned int size, unsigned int* index)
{
  unsigned int i;
  m_float max = -INFINITY;
  for(i = 0; i < size; i++) {
    if(f[i] > max) {
      max = f[i];
      *index = i;
    }
  }
  return max;
}
*/
/* from chuck ;-) */
m_float compute_centroid(Ana* fft) {
  m_float m0 = 0.0;
  m_float m1 = 0.0;
  m_float centroid = 0.0;
  unsigned int i;
  /* Compute centroid using moments */
  for(i = 0; i < fft->size / 2; i++) {
    m1 += i * fft->fval[i];
    m0 += fft->fval[i];
  }
  if(m0 != 0.0)
    centroid = m1 / m0;
  else
    centroid = fft->size / 2.0; /* Perfectly balanced */
  return centroid / fft->size;
}

m_float compute_spread(Ana* fft) {
  unsigned int i;
  m_float ret = 0;
  m_float mu = compute_centroid(fft);
  for(i = 0; i < fft->size / 2; i++)
    ret += ((i - mu) * (i - mu)) * fft->fval[i];
  return ret / fft->size;
}

m_float compute_skewness(Ana* fft) {
  unsigned int i;
  m_float ret = 0;
  m_float mu = compute_centroid(fft);
  for(i = 0; i < fft->size / 2; i++)
    ret += ((i - mu) * (i - mu) * (i - mu)) * fft->fval[i];
  return ret / fft->size;
}

m_float compute_kurtosis(Ana* fft) {
  unsigned int i;
  m_float ret = 0;
  m_float mu = compute_centroid(fft);
  for(i = 0; i < fft->size / 2; i++)
    ret += ((i - mu) * (i - mu) * (i - mu) * (i - mu)) * fft->fval[i];
  return ret / fft->size;
}


m_float compute_rms(Ana* fft) {
  m_float rms = 0.0;
  m_uint  i;
  /* get sum of squares */
  for(i = 0; i < fft->size / 2; i++)
    rms += (fft->fval[i] * fft->fval[i]);
  rms /= fft->size;
  rms = sqrt(rms);
  return rms;
}

m_float compute_rolloff(Ana* fft) {
  m_float sum = 0.0, target;
  unsigned int i;
  /* sanity check */
  /* assert(percent >= 0 && percent <= 1); */
  /* iterate */
  for(i = 0; i < fft->size / 2; i++)
    sum += fft->fval[i];
  /* the target */
  target = sum * fft->percent / 100.;
  sum = 0.0;
  /* iterate */
  for(i = 0; i < fft->size; i++) {
    sum += fft->fval[i];
    if(sum >= target)
      break;
  }
  return i / (m_float)fft->size;
}

m_float compute_freq(Ana* fft) {
  unsigned int i;
  m_float max = -0;
  m_float where = 0;
  for(i = 0; i < fft->size / 2; i++) {
    /*		if(fft->cval[i][0] > max)*/
    if(fft->fval[i] > max) {
      /*			max = fft->cval[i][0];*/
      max = fft->fval[i];
      where = i;
    }
  }
  return where / fft->size * fft->sr;
}

m_float compute_asc(Ana* fft) {
  unsigned int i;
  m_float ret = 0.0;
  m_float sum = 0.0;
  for(i = 0; i < fft->size / 2; i++) {
    ret += log2((compute_freq(fft) / 1000.)) * fft->fval[i];
    sum += fft->fval[i];
  }
  return ret / sum;
}

m_float compute_ass(Ana* fft) {
  unsigned int i;
  m_float ret = 0.0;
  m_float sum = 0.0;
  m_float asc = compute_asc(fft);
  for(i = 0; i < fft->size / 2; i++) {
    m_float f;
    f = log2(compute_freq(fft) / 1000.) - asc;
    f *= f;
    f *= fft->fval[i];
    ret += f;
    sum += fft->fval[i];
  }
  return ret / sum;
}

/*
m_float compute_normrms(Ana* fft)
{
  unsigned int i;
  m_float energy = 0.0;

  // get energy
  for(i = 0; i < fft->size/2; i++)
      energy += (fft->fval[i]*fft->fval[i]);

  // check energy
  if (energy == 0.0)
  {
    for(i = 0; i < fft->size/2; i++)
      fft->norm[i] = 0;
    return 0;
  }
  else
      energy = sqrt(energy);

  for(i = 0; i < fft->size/2; i++)
  {
      if(fft->fval[i] > 0.0)
        fft->norm[i] = (fft->fval[i] / energy);
      else
          fft->norm[i] = 0.0;
  }
  return array_max(fft->norm, fft->size/2, i);
}
*/
/*
m_float compute_flux(Ana* fft)
{
  unsigned int i;
  m_float result = 0.0;

  if(fft->prev)
  {
    // compute normalize rms
    compute_normrms(fft);
    // compute flux
    for(i = 0; i < fft->size; i++)
      result += (fft->fval[i] - fft->prev[i])*(fft->fval[i] - fft->prev[i]);
    // take square root of flux
    result = sqrt(result);
  }
  else
  {
    fft->prev = malloc(fft->size * sizeof(m_float));
    fft->norm = malloc(fft->size * sizeof(m_float));
  }
  // copy curr to prev
  for(i = 0; i < fft->size; i++)
    fft->prev[i] = fft->norm[i];
  return result;
}
*/
/*
m_float compute_corr(Ana* a,Ana* b)
{
  unsigned int i;
  fftw_complex *spec, *a_spec, *b_spec;
  if(a->size != b->size)
    return NAN;
  if(!a->corr)
    a->corr = fftw_malloc(a->size * sizeof(m_float));
  // forward fft both input spectrum
  spec    = malloc(a->size * sizeof(m_float));
  a_spec  = malloc(a->size * sizeof(m_float));
  b_spec  = malloc(b->size * sizeof(m_float));
  fftw_execute_dft_r2c(a->plan, a->fval, a_spec);
  fftw_execute_dft_r2c(b->plan, b->fval, b_spec);

  // conjugate a_spec and multiply a and b
  for(i = 0; i < a->size/2; i++)
  {
    a_spec[i][1] *= -1;
    spec[i][0] = (a_spec[i][0]*b_spec[i][0] - a_spec[i][1]*b_spec[i][1]);
    spec[i][1] = (a_spec[i][0]*b_spec[i][1] - a_spec[i][1]*b_spec[i][0]);
  }
  // take the ifft of spec
  if(!a->iplan)
    a->iplan = fftw_plan_dft_c2r_1d(a->size, spec, a->corr, FFTW_PATIENT);
  else
    fftw_execute_dft_c2r(a->iplan, b_spec, a->corr);

  fftw_free(spec);
  fftw_free(a_spec);
  fftw_free(b_spec);
  return array_max(a->corr, a->size/2, i) / a->size;
}
*/
/*
#define __SGN(x)  (x >= 0.0f ? 1.0f : -1.0f)

m_float compute_zerox(Ana* fft, m_float* buffer)
{
  unsigned int i, xings = 0;
  m_float v = 0, p = 0;
  p = buffer[0];
  for(i = 0; i < fft->size; i++)
  {
    v = buffer[i];
    xings += __SGN(v) != __SGN(p);
    p = v;
  }
  return xings/fft->size;
}
*/
static struct Type_ t_ana = { "ANA", SZ_INT, &t_object};
m_int o_ana_ana;
m_int o_ana_fft;
m_int o_ana_fn;

static m_float ana_dummy(Fft* fft) {
  return 0.0;
}
static MFUN(ana_compute) {
  M_Object   fft = *(M_Object*)(o->data + o_ana_fft);
  Ana* ana = *(Ana**)(o->data + o_ana_ana);
  f_analys f = *(f_analys*)(o->data + o_ana_fn);
  if(!fft || ana->last == ana->sp->pos)
    return;
  *(m_float*)RETURN = f(ana);
  ana->last = ana->sp->pos;
}

static MFUN(ana_get_fft) {
  *(m_uint*)RETURN = (m_uint) * (M_Object*)(o->data + o_ana_fft);
}

static MFUN(ana_set_fft) {
  Fft* fft;
  M_Object obj = *(M_Object*)(o->data + o_ana_fft);
  Ana* ana = *(Ana**)(o->data + o_ana_ana);
  if(obj) release(obj, shred);
  obj = *(M_Object*)MEM(SZ_INT);
  if(!obj) {
    ana->size = 0;
    ana->fval = NULL;
    *(m_uint*)RETURN = 0;
    return;
  }
  fft = (Fft*)UGEN(obj)->ug;
  if(!fft || !fft->buf) {
    err_msg(INSTR_, 0, "FFT probably not initialised.");
    release(obj, shred);
    return;
  }
  ana->size = fft->fft->fftsize;
  ana->fval = fft->frq->s;
  *(M_Object*)RETURN = *(M_Object*)(o->data + o_ana_fft) = obj;
}

static CTOR(ana_ctor) {
  Ana* ana = *(Ana**)(o->data + o_ana_ana) = malloc(sizeof(Ana));
  ana->sr = shred->vm_ref->sp->sr;
  ana->percent = 50; // rolloff;
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)ana_dummy;
  ana->sp = shred->vm_ref->sp;
  ana->last = 0;
}

static DTOR(ana_dtor) {
  free(*(Ana**)(o->data + o_ana_ana));
}

static m_bool import_ana(Env env) {
  DL_Func fun;
  CHECK_BB(import_class_begin(env, &t_ana, env->global_nspc, ana_ctor, ana_dtor))
  o_ana_ana = import_var(env, "int", "@_fft", ae_flag_member, NULL);
  CHECK_BB(o_ana_ana)
  o_ana_fft = import_var(env, "FFT", "@fft",  ae_flag_ref, NULL);
  CHECK_BB(o_ana_fft)
  o_ana_fn = import_var(env,  "int", "@fn", ae_flag_member, NULL);
  CHECK_BB(o_ana_fn)
  dl_func_init(&fun, "float", "compute", (m_uint)ana_compute);
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "FFT", "fft", (m_uint)ana_get_fft);
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "FFT", "fft", (m_uint)ana_set_fft);
  dl_func_add_arg(&fun, "FFT", "arg");
  CHECK_BB(import_fun(env, &fun, 0))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_centroid = { "Centroid", SZ_INT, &t_ana };
static CTOR(centroid_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_centroid;
}
static m_bool import_centroid(Env env) {
  CHECK_BB(import_class_begin(env, &t_centroid, env->global_nspc, centroid_ctor, NULL))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_spread = { "Spread", SZ_INT, &t_ana };
static CTOR(spread_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_spread;
}
static m_bool import_spread(Env env) {
  CHECK_BB(import_class_begin(env, &t_spread, env->global_nspc, spread_ctor, NULL))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_skewness = { "Skewness", SZ_INT, &t_ana };
static CTOR(skewness_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_skewness;
}
static m_bool import_skewness(Env env) {
  CHECK_BB(import_class_begin(env, &t_skewness, env->global_nspc, skewness_ctor, NULL))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_kurtosis = { "Kurtosis", SZ_INT, &t_ana };
static CTOR(kurtosis_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_kurtosis;
}
static m_bool import_kurtosis(Env env) {
  CHECK_BB(import_class_begin(env, &t_kurtosis, env->global_nspc, kurtosis_ctor, NULL))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_rms = { "RMS", SZ_INT, &t_ana };
static CTOR(rms_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_rms;
}
static m_bool import_rms(Env env) {
  CHECK_BB(import_class_begin(env, &t_rms, env->global_nspc, rms_ctor, NULL))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_rolloff = { "Rolloff", SZ_INT, &t_ana };
static CTOR(rolloff_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_rolloff;
}
static MFUN(rolloff_get_percent) {
  Ana* ana = *(Ana**)(o->data + o_ana_ana);
  *(m_float*)RETURN = ana->percent;
}
static MFUN(rolloff_set_percent) {
  Ana* ana = *(Ana**)(o->data + o_ana_ana);
  *(m_float*)RETURN = (ana->percent = *(m_float*)MEM(SZ_INT));
}
static m_bool import_rolloff(Env env) {
  DL_Func fun;
  CHECK_BB(import_class_begin(env, &t_rolloff, env->global_nspc, rolloff_ctor, NULL))
  dl_func_init(&fun, "float", "percent", (m_uint)rolloff_get_percent);
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "float", "percent", (m_uint)rolloff_set_percent);
  dl_func_add_arg(&fun, "float", "arg");
  CHECK_BB(import_fun(env, &fun, 0))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_freq = { "Freq", SZ_INT, &t_ana };
static CTOR(freq_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_freq;
}
static m_bool import_freq(Env env) {
  CHECK_BB(import_class_begin(env, &t_freq, env->global_nspc, freq_ctor, NULL))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_asc = { "ASC", SZ_INT, &t_ana };
static CTOR(asc_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_asc;
}
static m_bool import_asc(Env env) {
  CHECK_BB(import_class_begin(env, &t_asc, env->global_nspc, asc_ctor, NULL))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_ass = { "ASS", SZ_INT, &t_ana };
static CTOR(ass_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_ass;
}
static m_bool import_ass(Env env) {
  CHECK_BB(import_class_begin(env, &t_ass, env->global_nspc, ass_ctor, NULL))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_fc = { "FC", SZ_INT, &t_object };
static m_int o_fc_vector;
static CTOR(fc_ctor) {
  *(Vector*)(o->data + o_fc_vector) = new_vector();
}
static DTOR(fc_dtor) {
  free_vector(*(Vector*)(o->data + o_fc_vector));
}

static MFUN(fc_compute) {
  m_uint i;
  M_Object ret;
  Vector v = *(Vector*)(o->data + o_fc_vector);
  ret = new_M_Array(SZ_FLOAT, vector_size(v), 1);
  vector_add(&shred->gc, (vtype)ret);
  for(i = 0; i < vector_size(v); i++) {
    M_Object obj = (M_Object)vector_at(v, i);
//    if(!obj) continue; // prevented in fc.add
    Ana* ana   = *(Ana**)(obj->data + o_ana_ana);
//    if(!_fft) continue; // seems prevented somehow. (this is unclear)
    Fft* fft   = *(Fft**)(obj->data + o_ana_fft);
    if(!fft)
      continue;
    f_analys fn  = *(f_analys*)(obj->data + o_ana_fn);
    m_float f = fn(ana);
    f_vector_set(ARRAY(ret), i, f);
  }
  *(m_uint*)RETURN = (m_uint)ret;
}

static MFUN(fc_add) {
  Vector v = *(Vector*)(o->data + o_fc_vector);
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  if(obj) {
    vector_add(v, (vtype)obj);
    release(obj, shred);
  }
  *(m_uint*)RETURN = (m_uint)obj;
}

static MFUN(fc_rem) {
  Vector v = *(Vector*)(o->data + o_fc_vector);
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  if(obj) {
    vector_rem(v, vector_find(v, (vtype)obj));
    release(obj, shred);
  }
  *(m_uint*)RETURN = (m_uint)obj;
}

INSTR(fc_connect) {
  POP_REG(shred, SZ_INT * 2);
  M_Object o   = *(M_Object*)REG(0);
  M_Object obj = **(M_Object**)REG(SZ_INT);
  if(o) {
    if(obj) {
      Vector v = *(Vector*)(obj->data + o_fc_vector);
      vector_add(v, (vtype)o);
      release(obj, shred);
    }
    release(o, shred);
  }
  *(M_Object*)REG(0) = obj;
  PUSH_REG(shred, SZ_INT);
}

INSTR(fc_disconnect) {
  POP_REG(shred, SZ_INT * 2);
  M_Object o   = *(M_Object*)REG(0);
  M_Object obj = *(M_Object*)REG(SZ_INT); // WARN inconsistency
  if(o) {
    if(obj) {
      Vector v = *(Vector*)(obj->data + o_fc_vector);
      vector_rem(v, vector_find(v, (vtype)o));
      release(obj, shred);
    }
    release(o, shred);
  }
  *(M_Object*)REG(0) = obj;
  PUSH_REG(shred, SZ_INT);
}

static m_bool import_fc(Env env) {
  DL_Func fun;
  CHECK_BB(import_class_begin(env, &t_fc, env->global_nspc, fc_ctor, fc_dtor))
  o_fc_vector = import_var(env, "int", "@vector", ae_flag_member, NULL);
  CHECK_BB(o_fc_vector)
  dl_func_init(&fun, "float[]", "compute", (m_uint)fc_compute);
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "ANA", "add", (m_uint)fc_add);
  dl_func_add_arg(&fun, "ANA", "arg");
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "ANA", "rem", (m_uint)fc_rem);
  dl_func_add_arg(&fun, "ANA", "arg");
  CHECK_BB(import_fun(env, &fun, 0))
  CHECK_BB(import_class_end(env))
  return 1;
}

m_bool import_analys(Env env) {
  CHECK_BB(import_fft(env))
  CHECK_BB(import_ana(env))
  CHECK_BB(import_centroid(env))
  CHECK_BB(import_spread(env))
  CHECK_BB(import_skewness(env))
  CHECK_BB(import_kurtosis(env))
  CHECK_BB(import_rms(env))
  CHECK_BB(import_rolloff(env))
  CHECK_BB(import_freq(env))
  CHECK_BB(import_asc(env))
  CHECK_BB(import_ass(env))
  CHECK_BB(import_fc(env))
  CHECK_BB(import_op(env, op_chuck,   "ANA", "FC", "FC", fc_connect,    1))
  CHECK_BB(import_op(env, op_unchuck, "ANA", "FC", "FC", fc_disconnect, 1))
  return 1;
}
