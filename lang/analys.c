#include <stdlib.h>
#include <math.h>

#include "defs.h"
#include "vm.h"
#include "type.h"
/*#include "dl.h"*/
/*#include "err_msg.h"*/
#include "import.h"
/*#include "lang.h"*/
#include "bbq.h"
#include "ugen.h"
#include "array.h"
#include "object.h"
#include "err_msg.h"


typedef struct {
  unsigned int size;
  unsigned int pos;
  m_float* data;
  Vector curr;
} sp_buffer;



static m_int sp_buffer_create(sp_buffer** p, m_uint size)
{
  *p = malloc(sizeof(sp_buffer));
  (*p)->size = size;
  (*p)->pos = 0;
  (*p)->data = calloc(size, sizeof(m_float));
  return SP_OK;
}

static m_int sp_buffer_destroy(sp_buffer* buffer)
{
  free(buffer->data);
  free(buffer);
  return SP_OK;
}

static m_int sp_buffer_add(sp_buffer* buffer, m_float f)
{
  buffer->data[ buffer->pos ] = f;
  buffer->pos++;
  buffer->pos %= buffer->size;
  return SP_OK;
}

static m_float* sp_buffer_get(sp_buffer* buffer)
{
  m_uint i;
  m_float* ret = calloc(buffer->size, sizeof(m_float));
  for(i = 0; i < buffer->size; i++)
    ret[i] = buffer->data[(buffer->pos + i) % buffer->size];
  return ret;
}

static struct Type_ t_fft = { "FFT", SZ_INT, &t_ugen};
typedef struct {
  sp_buffer*  buf;
  /*  m_float*  (*win)(m_float* buf, m_uint size);*/
  FFTFREQS*    frq;
  FFTwrapper* fft;
} FFT;


static m_bool fft_tick(UGen u)
{
  m_float* smp;
  FFT* ana = (FFT*)u->ug;
  base_tick(u);                         // compute inputs
  if(!ana->buf)
    return 1;
  sp_buffer_add(ana->buf, u->in);      // add them to buffer
  if(u->trig && u->trig->ugen->out) {  // if trigged, compute fft
    smp = sp_buffer_get(ana->buf);
    /*    if(ana->win)*/                  // do windowing
    /*      ana->win(smp, ana->buf->size);*/
    smps2freqs(ana->fft, smp, ana->frq);
    free(smp);
  }
  return 1;
}

static void fft_ctor(M_Object o, VM_Shred shred)
{
  FFT* ana = o->ugen->ug = calloc(1, sizeof(FFT));
  assign_ugen(o->ugen, 1, 1, 1, ana);
  o->ugen->tick = fft_tick;
}

static DTOR(fft_dtor)
{
  FFT* ana = (FFT*)o->ugen->ug;
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

static MFUN(fft_init)
{
  FFT* ana = (FFT*)o->ugen->ug;
  m_int size = *(m_int*)(shred->mem + SZ_INT);

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
  RETURN->d.v_uint = size;
}

static MFUN(fft_compute)
{
  m_float* smp;
  FFT* ana = (FFT*)o->ugen->ug;
  if(!ana)
	return;
  if(!ana->buf) {
    RETURN->d.v_uint = 0;
    return;
  }
  smp = sp_buffer_get(ana->buf);
  smps2freqs(ana->fft, smp, ana->frq);
  free(smp);
}

static m_bool import_fft(Env env)
{
  DL_Func* fun;
  CHECK_BB(add_global_type(env, &t_fft))
  CHECK_OB(import_class_begin(env, &t_fft, env->global_nspc, fft_ctor, fft_dtor))
  fun = new_DL_Func("int", "init", (m_uint)fft_init);
  dl_func_add_arg(fun, "int", "size");
  CHECK_OB(import_mfun(env, fun))
  /*  fun = new_DL_Func("int", "init", (m_uint)fft_init2);*/
  /*    dl_func_add_arg(fun, "int", "size");*/
  /*    dl_func_add_arg(fun, "float[]", "window");*/
  /*  CHECK_OB(import_mfun(env, fun))*/
  /*  fun = new_DL_Func("int", "init", (m_uint)fft_init3);*/
  /*    dl_func_add_arg(fun, "int", "size");*/
  /*    dl_func_add_arg(fun, "string", "window");*/
  /*  CHECK_OB(import_mfun(env, fun))*/
  /*  fun = new_DL_Func("int", "window", (m_uint)fft_win);*/
  /*    dl_func_add_arg(fun, "float[]", "window");*/
  /*  CHECK_OB(import_mfun(env, fun))*/
  /*  fun = new_DL_Func("int", "window", (m_uint)fft_win_name);*/
  /*    dl_func_add_arg(fun, "string", "name");*/
  /*  CHECK_OB(import_mfun(env, fun))*/
  /*  fun = new_DL_Func("complex[]", "compute", (m_uint)fft_compute);*/
  fun = new_DL_Func("void", "compute", (m_uint)fft_compute);
  CHECK_OB(import_mfun(env, fun))

  CHECK_BB(import_class_end(env))
  return 1;
}

typedef struct _FFT {
  m_uint size, sr;
  m_float* fval;
  m_float  percent;     // rollof
  /*  m_float* norm, *prev; // flux*/
  /*  m_float* cval[2];*/ // corr
} _FFT;

typedef double (*f_analys)(_FFT* fft);

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
/* from chuck ;-) */
m_float compute_centroid(_FFT* fft )
{
  m_float m0 = 0.0;
  m_float m1 = 0.0;
  m_float centroid = 0.0;
  unsigned int i;
  /* Compute centroid using moments */
  for( i = 0; i < fft->size / 2; i++ ) {
    m1 += i * fft->fval[i];
    m0 += fft->fval[i];
  }
  if( m0 != 0.0 )
    centroid = m1 / m0;
  else
    centroid = fft->size / 2.0; /* Perfectly balanced */
  return centroid / fft->size;
}

m_float compute_spread(_FFT* fft)
{
  unsigned int i;
  m_float ret = 0;
  m_float mu = compute_centroid(fft);
  for(i = 0; i < fft->size / 2; i++)
    ret += ( (i - mu) * (i - mu) ) * fft->fval[i];
  return ret / fft->size;
}

m_float compute_skewness(_FFT* fft)
{
  unsigned int i;
  m_float ret = 0;
  m_float mu = compute_centroid(fft);
  for(i = 0; i < fft->size / 2; i++)
    ret += ( (i - mu) * (i - mu) * (i - mu) ) * fft->fval[i];
  return ret / fft->size;
}

m_float compute_kurtosis(_FFT* fft)
{
  unsigned int i;
  m_float ret = 0;
  m_float mu = compute_centroid(fft);
  for(i = 0; i < fft->size / 2; i++)
    ret += ( (i - mu) * (i - mu) * (i - mu) * (i - mu)) * fft->fval[i];
  return ret / fft->size;
}


m_float compute_rms(_FFT* fft)
{
  m_float rms = 0.0;
  m_uint  i;
  /* get sum of squares */
  for(i = 0; i < fft->size / 2; i++)
    rms += (fft->fval[i] * fft->fval[i]);
  rms /= fft->size;
  rms = sqrt(rms);
  return rms;
}

m_float compute_rolloff(_FFT* fft)
{
  m_float sum = 0.0, target;
  unsigned int i;
  /* sanity check */
  /* assert( percent >= 0 && percent <= 1 ); */
  /* iterate */
  for( i = 0; i < fft->size / 2; i++ )
    sum += fft->fval[i];
  /* the target */
  target = sum * fft->percent / 100.;
  sum = 0.0;
  /* iterate */
  for( i = 0; i < fft->size; i++ ) {
    sum += fft->fval[i];
    if( sum >= target )
      break;
  }
  return i / (m_float)fft->size;
}

m_float compute_freq(_FFT* fft)
{
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

m_float compute_asc(_FFT* fft)
{
  unsigned int i;
  m_float ret = 0.0;
  m_float sum = 0.0;
  for(i = 0; i < fft->size / 2; i++) {
    ret += log2( ( compute_freq(fft) / 1000. ) ) * fft->fval[i];
    sum += fft->fval[i];
  }
  return ret / sum;
}

m_float compute_ass(_FFT* fft)
{
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
m_float compute_normrms(_FFT* fft)
{
  unsigned int i;
  m_float energy = 0.0;

  // get energy
  for( i = 0; i < fft->size/2; i++ )
      energy += ( fft->fval[i]*fft->fval[i] );

  // check energy
  if (energy == 0.0)
  {
    for( i = 0; i < fft->size/2; i++ )
      fft->norm[i] = 0;
    return 0;
  }
  else
      energy = sqrt( energy );

  for( i = 0; i < fft->size/2; i++ )
  {
      if( fft->fval[i] > 0.0)
        fft->norm[i] = ( fft->fval[i] / energy );
      else
          fft->norm[i] = 0.0;
  }
  return array_max(fft->norm, fft->size/2, i);
}
*/
/*
m_float compute_flux(_FFT* fft)
{
  unsigned int i;
  m_float result = 0.0;

  if(fft->prev)
  {
    // compute normalize rms
    compute_normrms( fft );
    // compute flux
    for( i = 0; i < fft->size; i++ )
      result += (fft->fval[i] - fft->prev[i])*(fft->fval[i] - fft->prev[i]);
    // take square root of flux
    result = sqrt( result );
  }
  else
  {
    fft->prev = malloc(fft->size * sizeof(m_float));
    fft->norm = malloc(fft->size * sizeof(m_float));
  }
  // copy curr to prev
  for( i = 0; i < fft->size; i++ )
    fft->prev[i] = fft->norm[i];
  return result;
}
*/
/*
m_float compute_corr(_FFT* a,_FFT* b)
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
  for( i = 0; i < a->size/2; i++ )
  {
    a_spec[i][1] *= -1;
    spec[i][0] = ( a_spec[i][0]*b_spec[i][0] - a_spec[i][1]*b_spec[i][1]);
    spec[i][1] = ( a_spec[i][0]*b_spec[i][1] - a_spec[i][1]*b_spec[i][0]);
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
#define __SGN(x)  (x >= 0.0f ? 1.0f : -1.0f )

m_float compute_zerox(_FFT* fft, m_float* buffer)
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
m_int o_ana__fft;
m_int o_ana_fft;
m_int o_ana_fn;

static m_float ana_dummy(FFT* fft)
{
  return 0.0;
}
static MFUN(ana_compute)
{
  M_Object   fft = *(M_Object*) (o->d.data + o_ana_fft);
  _FFT* _fft = *(_FFT**)(o->d.data + o_ana__fft);
  f_analys f = *(f_analys*)(o->d.data + o_ana_fn);
  if(!fft)
    return;
  RETURN->d.v_float = f(_fft);
}

static MFUN(ana_get_fft)
{
  RETURN->d.v_uint = (m_uint)*(M_Object*) (o->d.data + o_ana_fft);
}

static MFUN(ana_set_fft)
{
  FFT* ana;
  M_Object fft = *(M_Object*) (o->d.data + o_ana_fft);
  _FFT* _fft = *(_FFT**)(o->d.data + o_ana__fft);
//  if(fft)
//    release(fft, shred);
  fft = *(M_Object*)(shred->mem + SZ_INT);
  if(!fft) {
    _fft->size = 0;
    _fft->fval = NULL;
    RETURN->d.v_uint = 0;
    return;
  }
  fft->ref++;
  ana = (FFT*)fft->ugen->ug;
  if(!ana || !ana->buf) {
    err_msg(INSTR_, 0, "FFT probably not initialised.");
    return;
  }
  _fft->size = ana->fft->fftsize;
  _fft->fval = ana->frq->s;
  *(M_Object*) (o->d.data + o_ana_fft) = fft;
  RETURN->d.v_uint = (m_uint) * (M_Object*)(shred->mem + SZ_INT);
}

static void ana_ctor(M_Object o, VM_Shred shred)
{
  _FFT* _fft = *(_FFT**)(o->d.data + o_ana__fft) = malloc(sizeof(_FFT));
  _fft->sr = shred->vm_ref->bbq->sp->sr;
  _fft->percent = 50; // rolloff;
  *(f_analys*)(o->d.data + o_ana_fn) = (f_analys)ana_dummy;
}

static void ana_dtor(M_Object o, VM_Shred shred)
{
  free(*(_FFT**)(o->d.data + o_ana__fft));
}

static m_bool import_ana(Env env)
{
  DL_Func* fun;
  CHECK_BB(add_global_type(env, &t_ana))
  CHECK_OB(import_class_begin(env, &t_ana, env->global_nspc, ana_ctor, ana_dtor))
  o_ana_fft = import_mvar(env, "int", "@_fft", 0, 0, "internal _fft");
  CHECK_BB(o_ana__fft)
  o_ana_fft = import_mvar(env, "FFT", "@fft",  0, 1, "fft reference");
  CHECK_BB(o_ana_fft)
  o_ana_fn = import_mvar(env,  "int", "@fn",   0, 0, "internal compute fonction");
  CHECK_BB(o_ana_fn)
  fun = new_DL_Func("float", "compute", (m_uint)ana_compute);
  CHECK_OB(import_mfun(env, fun))
  fun = new_DL_Func("FFT", "fft", (m_uint)ana_get_fft);
  CHECK_OB(import_mfun(env, fun))
  fun = new_DL_Func("FFT", "fft", (m_uint)ana_set_fft);
  dl_func_add_arg(fun, "FFT", "arg");
  CHECK_OB(import_mfun(env, fun))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_centroid = { "Centroid", SZ_INT, &t_ana };
static void centroid_ctor(M_Object o, VM_Shred shred)
{
  *(f_analys*)(o->d.data + o_ana_fn) = (f_analys)compute_centroid;
}
static m_bool import_centroid(Env env)
{
  CHECK_BB(add_global_type(env, &t_centroid))
  CHECK_OB(import_class_begin(env, &t_centroid, env->global_nspc, centroid_ctor, NULL))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_spread = { "Spread", SZ_INT, &t_ana };
static void spread_ctor(M_Object o, VM_Shred shred)
{
  *(f_analys*)(o->d.data + o_ana_fn) = (f_analys)compute_spread;
}
static m_bool import_spread(Env env)
{
  CHECK_BB(add_global_type(env, &t_spread))
  CHECK_OB(import_class_begin(env, &t_spread, env->global_nspc, spread_ctor, NULL))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_skewness = { "Skewness", SZ_INT, &t_ana };
static void skewness_ctor(M_Object o, VM_Shred shred)
{
  *(f_analys*)(o->d.data + o_ana_fn) = (f_analys)compute_skewness;
}
static m_bool import_skewness(Env env)
{
  CHECK_BB(add_global_type(env, &t_skewness))
  CHECK_OB(import_class_begin(env, &t_skewness, env->global_nspc, skewness_ctor, NULL))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_kurtosis = { "Kurtosis", SZ_INT, &t_ana };
static void kurtosis_ctor(M_Object o, VM_Shred shred)
{
  *(f_analys*)(o->d.data + o_ana_fn) = (f_analys)compute_kurtosis;
}
static m_bool import_kurtosis(Env env)
{
  CHECK_BB(add_global_type(env, &t_kurtosis))
  CHECK_OB(import_class_begin(env, &t_kurtosis, env->global_nspc, kurtosis_ctor, NULL))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_rms = { "RMS", SZ_INT, &t_ana };
static void rms_ctor(M_Object o, VM_Shred shred)
{
  *(f_analys*)(o->d.data + o_ana_fn) = (f_analys)compute_rms;
}
static m_bool import_rms(Env env)
{
  CHECK_BB(add_global_type(env, &t_rms))
  CHECK_OB(import_class_begin(env, &t_rms, env->global_nspc, rms_ctor, NULL))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_rolloff = { "Rolloff", SZ_INT, &t_ana };
static void rolloff_ctor(M_Object o, VM_Shred shred)
{
  *(f_analys*)(o->d.data + o_ana_fn) = (f_analys)compute_rolloff;
}
static MFUN(rolloff_get_percent)
{
  _FFT* _fft = *(_FFT**)(o->d.data + o_ana__fft);
  RETURN->d.v_float = _fft->percent;
}
static MFUN(rolloff_set_percent)
{
  _FFT* _fft = *(_FFT**)(o->d.data + o_ana__fft);
  RETURN->d.v_float = (_fft->percent = *(m_float*)(shred->mem + SZ_INT));
}
static m_bool import_rolloff(Env env)
{
  DL_Func* fun;
  CHECK_BB(add_global_type(env, &t_rolloff))
  CHECK_OB(import_class_begin(env, &t_rolloff, env->global_nspc, rolloff_ctor, NULL))
  fun = new_DL_Func("float", "percent", (m_uint)rolloff_get_percent);
  CHECK_OB(import_mfun(env, fun))
  fun = new_DL_Func("float", "percent", (m_uint)rolloff_set_percent);
  dl_func_add_arg(fun, "float", "arg");
  CHECK_OB(import_mfun(env, fun))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_freq = { "Freq", SZ_INT, &t_ana };
static void freq_ctor(M_Object o, VM_Shred shred)
{
  *(f_analys*)(o->d.data + o_ana_fn) = (f_analys)compute_freq;
}
static m_bool import_freq(Env env)
{
  CHECK_BB(add_global_type(env, &t_freq))
  CHECK_OB(import_class_begin(env, &t_freq, env->global_nspc, freq_ctor, NULL))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_asc = { "ASC", SZ_INT, &t_ana };
static void asc_ctor(M_Object o, VM_Shred shred)
{
  *(f_analys*)(o->d.data + o_ana_fn) = (f_analys)compute_asc;
}
static m_bool import_asc(Env env)
{
  CHECK_BB(add_global_type(env, &t_asc))
  CHECK_OB(import_class_begin(env, &t_asc, env->global_nspc, asc_ctor, NULL))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_ass = { "ASS", SZ_INT, &t_ana };
static void ass_ctor(M_Object o, VM_Shred shred)
{
  *(f_analys*)(o->d.data + o_ana_fn) = (f_analys)compute_ass;
}
static m_bool import_ass(Env env)
{
  CHECK_BB(add_global_type(env, &t_ass))
  CHECK_OB(import_class_begin(env, &t_ass, env->global_nspc, ass_ctor, NULL))
  CHECK_BB(import_class_end(env))
  return 1;
}

static struct Type_ t_fc = { "FC", SZ_INT, &t_object };
static m_int o_fc_vector;
static void fc_ctor(M_Object o, VM_Shred shred)
{
  *(Vector*)(o->d.data + o_fc_vector) = new_Vector();
}
static void fc_dtor(M_Object o, VM_Shred shred)
{
  free_Vector(*(Vector*)(o->d.data + o_fc_vector));
}

static MFUN(fc_compute)
{
  m_uint i;
  M_Object ret;
  Vector v = *(Vector*)(o->d.data + o_fc_vector);
  ret = new_M_Array(SZ_FLOAT, vector_size(v), 1);
  for(i = 0; i < vector_size(v); i++) {
    M_Object obj = (M_Object)vector_at(v, i);
    if(!obj)
      continue;
    _FFT* _fft   = *(_FFT**)(obj->d.data + o_ana__fft);
    if(!_fft)
      continue;
    FFT* fft   = *(FFT**)(obj->d.data + o_ana_fft);
    if(!fft)
      continue;
    f_analys fn  = *(f_analys*)(obj->d.data + o_ana_fn);
    m_float f = fn(_fft);
    f_vector_set(ret->d.array, i, f);
  }
  RETURN->d.v_uint = (m_uint)ret;
}

static MFUN(fc_add)
{
  Vector v = *(Vector*)(o->d.data + o_fc_vector);
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);
  if(obj)
    vector_append(v, (vtype)obj);
  RETURN->d.v_uint = (m_uint)obj;
}

static MFUN(fc_rem)
{
  Vector v = *(Vector*)(o->d.data + o_fc_vector);
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);
  if(obj)
    vector_remove(v, vector_find(v, (vtype)obj));
  RETURN->d.v_uint = (m_uint)obj;
}

INSTR(fc_connect)
{
  POP_REG(shred, SZ_INT * 2);
  M_Object o   = *(M_Object*)(shred->reg);
  M_Object obj = **(M_Object**)(shred->reg + SZ_INT);
  if(o) {
    if(obj) {
      Vector v = *(Vector*)(obj->d.data + o_fc_vector);
      vector_append(v, (vtype)o);
      release(obj, shred);
    }
    release(o, shred);
  }
  *(M_Object*)shred->reg = obj;
  PUSH_REG(shred, SZ_INT);
}

INSTR(fc_disconnect)
{
  POP_REG(shred, SZ_INT * 2);
  M_Object o   = *(M_Object*)(shred->reg);
  M_Object obj = *(M_Object*)(shred->reg + SZ_INT); // WARN inconsistency
  if(o) {
    if(obj) {
      Vector v = *(Vector*)(obj->d.data + o_fc_vector);
      vector_remove(v, vector_find(v, (vtype)o));
      release(obj, shred);
    }
    release(o, shred);
  }
  *(M_Object*)shred->reg = obj;
  PUSH_REG(shred, SZ_INT);
}

static m_bool import_fc(Env env)
{
  DL_Func* fun;
  CHECK_BB(add_global_type(env, &t_fc))
  CHECK_OB(import_class_begin(env, &t_fc, env->global_nspc, fc_ctor, fc_dtor))
  o_fc_vector = import_mvar(env, "int", "@vector", 0, 0, "internal ANA clients");
  CHECK_BB(o_fc_vector)
  fun = new_DL_Func("float[]", "compute", (m_uint)fc_compute);
  CHECK_OB(import_mfun(env, fun))
  fun = new_DL_Func("ANA", "add", (m_uint)fc_add);
  dl_func_add_arg(fun, "ANA", "arg");
  CHECK_OB(import_mfun(env, fun))
  fun = new_DL_Func("ANA", "rem", (m_uint)fc_rem);
  dl_func_add_arg(fun, "ANA", "arg");
  CHECK_OB(import_mfun(env, fun))
  CHECK_BB(import_class_end(env))
  return 1;
}

m_bool import_analys(Env env)
{
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
  CHECK_BB(add_binary_op(env, op_chuck,   &t_ana, &t_fc, &t_fc, fc_connect,    1, 0))
  CHECK_BB(add_binary_op(env, op_unchuck, &t_ana, &t_fc, &t_fc, fc_disconnect, 1, 0))
  return 1;
}
