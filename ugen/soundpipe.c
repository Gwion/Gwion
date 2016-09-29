#include "vm.h"
#include "type.h"
#include "dl.h"
#include "err_msg.h"
#include "import.h"
#include "ugen.h"
#include "bbq.h"
#include "array.h"
#include "func.h"
#include "object.h"
extern m_int o_string_data;
#define FTBL(o) *((sp_ftbl**)((M_Object)o)->data + o_ftbl_data)
struct Type_ t_ftbl     = { "ftbl",     sizeof(m_uint), &t_object};
m_int o_ftbl_data;

static sp_ftbl* to_ftbl(M_Object obj, VM_Shred shred)
{
  m_uint i;
	M_Vector* v = obj->array;
	sp_ftbl*ftbl;
sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, m_vector_size(v));
	for(i = 0; i < m_vector_size(v); i++)
		ftbl->tbl[i] = f_vector_at(v, i);
  return ftbl;
}
static M_Object to_array(sp_ftbl* ftbl){  m_uint i;	M_Object array = new_M_Array(SZ_FLOAT, ftbl->size);	for(i = 0; i < ftbl->size; i++)		f_vector_set(array->array, i, ftbl->tbl[i]);	return array;}
static void TO_FTBL(DL_Return * RETURN, VM_Shred shred)
{  m_uint i;
	M_Object obj = *(M_Object*)(shred->mem + SZ_INT*2);
	M_Object ret = new_M_Object();
	initialize_object(ret, &t_ftbl);
	FTBL(ret) = to_ftbl(obj, shred);
	RETURN->v_uint = (m_uint)ret;
}
static void TO_ARRAY(DL_Return * RETURN, VM_Shred shred)
{
	M_Object obj = *(M_Object*)(shred->mem + SZ_INT*2);
  RETURN->v_uint = (m_uint)to_array(FTBL(obj));
}
static void ftbl_dtor(M_Object o, VM_Shred shred)
{
	if(FTBL(o))  sp_ftbl_destroy(&FTBL(o));
}

static void ftbl_sp_gen_composite(M_Object o, DL_Return * RETURN, VM_Shred shred)
{
  m_uint offset = SZ_INT*2;
  sp_ftbl* o_ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&o_ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  if(size < 0)
    return;
  sp_ftbl* ftbl;
  sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
  M_Object argstring_obj   = *(M_Object*)(shred->mem + offset);
  offset+=SZ_INT;
  m_str    argstring   = STRING(argstring_obj);
  sp_gen_composite(shred->vm_ref->bbq->sp, ftbl, argstring);
  FTBL(o) = ftbl;
  shred->reg -= SZ_INT *  2;
}

static void ftbl_sp_gen_file(M_Object o, DL_Return * RETURN, VM_Shred shred)
{
  m_uint offset = SZ_INT*2;
  sp_ftbl* o_ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&o_ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  if(size < 0)
    return;
  sp_ftbl* ftbl;
  sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
  M_Object filename_obj   = *(M_Object*)(shred->mem + offset);
  offset+=SZ_INT;
  m_str    filename   = STRING(filename_obj);
  sp_gen_file(shred->vm_ref->bbq->sp, ftbl, filename);
  FTBL(o) = ftbl;
  shred->reg -= SZ_INT *  2;
}

static void ftbl_sp_gen_gauss(M_Object o, DL_Return * RETURN, VM_Shred shred)
{
  m_uint offset = SZ_INT*2;
  sp_ftbl* o_ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&o_ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  if(size < 0)
    return;
  sp_ftbl* ftbl;
  sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
  m_float scale = *(m_float*)(shred->mem + offset);
  offset+=SZ_FLOAT;
  m_int seed = *(m_int*)(shred->mem + offset);
  offset+=SZ_INT;
  sp_gen_gauss(shred->vm_ref->bbq->sp, ftbl, scale, seed);
  FTBL(o) = ftbl;
  shred->reg -= SZ_INT *  3;
}

static void ftbl_sp_gen_line(M_Object o, DL_Return * RETURN, VM_Shred shred)
{
  m_uint offset = SZ_INT*2;
  sp_ftbl* o_ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&o_ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  if(size < 0)
    return;
  sp_ftbl* ftbl;
  sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
  M_Object argstring_obj   = *(M_Object*)(shred->mem + offset);
  offset+=SZ_INT;
  m_str    argstring   = STRING(argstring_obj);
  sp_gen_line(shred->vm_ref->bbq->sp, ftbl, argstring);
  FTBL(o) = ftbl;
  shred->reg -= SZ_INT *  2;
}

static void ftbl_sp_gen_sine(M_Object o, DL_Return * RETURN, VM_Shred shred)
{
  m_uint offset = SZ_INT*2;
  sp_ftbl* o_ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&o_ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  if(size < 0)
    return;
  sp_ftbl* ftbl;
  sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
  sp_gen_sine(shred->vm_ref->bbq->sp, ftbl);
  FTBL(o) = ftbl;
  shred->reg -= SZ_INT *  1;
}

static void ftbl_sp_gen_sinesum(M_Object o, DL_Return * RETURN, VM_Shred shred)
{
  m_uint offset = SZ_INT*2;
  sp_ftbl* o_ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&o_ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  if(size < 0)
    return;
  sp_ftbl* ftbl;
  sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
  M_Object argstring_obj   = *(M_Object*)(shred->mem + offset);
  offset+=SZ_INT;
  m_str    argstring   = STRING(argstring_obj);
  sp_gen_sinesum(shred->vm_ref->bbq->sp, ftbl, argstring);
  FTBL(o) = ftbl;
  shred->reg -= SZ_INT *  2;
}

static void ftbl_sp_gen_vals(M_Object o, DL_Return * RETURN, VM_Shred shred)
{
  m_uint offset = SZ_INT*2;
  sp_ftbl* o_ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&o_ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  if(size < 0)
    return;
  sp_ftbl* ftbl;
  sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
  M_Object argstring_obj   = *(M_Object*)(shred->mem + offset);
  offset+=SZ_INT;
  m_str    argstring   = STRING(argstring_obj);
  sp_gen_vals(shred->vm_ref->bbq->sp, ftbl, argstring);
  FTBL(o) = ftbl;
  shred->reg -= SZ_INT *  2;
}

static void ftbl_sp_gen_xline(M_Object o, DL_Return * RETURN, VM_Shred shred)
{
  m_uint offset = SZ_INT*2;
  sp_ftbl* o_ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&o_ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  if(size < 0)
    return;
  sp_ftbl* ftbl;
  sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
  M_Object argstring_obj   = *(M_Object*)(shred->mem + offset);
  offset+=SZ_INT;
  m_str    argstring   = STRING(argstring_obj);
  sp_gen_xline(shred->vm_ref->bbq->sp, ftbl, argstring);
  FTBL(o) = ftbl;
  shred->reg -= SZ_INT *  2;
}

static void ftbl_sp_gen_padsynth(M_Object o, DL_Return * RETURN, VM_Shred shred)
{
  m_uint offset = SZ_INT*2;
  sp_ftbl* o_ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&o_ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  if(size < 0)
    return;
  sp_ftbl* ftbl;
  sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
  M_Object 	amps_obj   = *(M_Object*)(shred->mem + offset);
  offset+=SZ_INT;
  sp_ftbl*   amps   = FTBL(amps_obj);
  m_float f = *(m_float*)(shred->mem + offset);
  offset+=SZ_FLOAT;
  m_float bw = *(m_float*)(shred->mem + offset);
  offset+=SZ_FLOAT;
  sp_gen_padsynth(shred->vm_ref->bbq->sp, ftbl, amps, f, bw);
  FTBL(o) = ftbl;
  shred->reg -= SZ_INT *  4;
}


m_bool import_ftbl(Env env)
{
  DL_Func* fun;
  Func     f;
  DL_Value* arg;

  CHECK_BB(add_global_type(env, &t_ftbl))
  CHECK_BB(import_class_begin(env, &t_ftbl, env->global_nspc, NULL, ftbl_dtor))
  o_ftbl_data = import_mvar(env, "int", "@data", 0, 0, "sp_ftbl placeholder");
  fun = new_DL_Func("ftbl", "from_array", (m_uint)TO_FTBL);    arg = dl_func_add_arg(fun, "float[]", "array");
    arg->doc = "the array to convert";
  CHECK_BB((f = import_sfun(env, fun)))
  f->doc = "convert float[] to ftbl";

  fun = new_DL_Func("void", "composite", (m_uint)ftbl_sp_gen_composite);
    arg = dl_func_add_arg(fun, "string", "argstring");
    arg->doc = "a string of space-separated parameters, in groups of four:\narg 1 is the partial number. must be positive, but it doesn't need to be a whole number.\narg 2 is the strength.\narg 3 is the initial phase (expressed in degrees)\narg 4 is the dc offset. A dc offset of 2 will put a 2-strength sinusoid in the rangefrom (-2,2) to (0, 4)\n";
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Generate a composite waveform of sinusoids";

  fun = new_DL_Func("void", "file", (m_uint)ftbl_sp_gen_file);
    arg = dl_func_add_arg(fun, "string", "filename");
    arg->doc = "filename";
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Reads from a wav file\n    This will only load as many samples as the length of the ftable.";

  fun = new_DL_Func("void", "gauss", (m_uint)ftbl_sp_gen_gauss);
    arg = dl_func_add_arg(fun, "float", "scale");
    arg->doc = "The scale of the distribution, in the range of -/+scale";
    arg = dl_func_add_arg(fun, "int", "seed");
    arg->doc = "Random seed.";
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Gaussian distribution";

  fun = new_DL_Func("void", "line", (m_uint)ftbl_sp_gen_line);
    arg = dl_func_add_arg(fun, "string", "argstring");
    arg->doc = "A list of ordered xy pairs. X expects whole number integers, as they correlate to index positions in the ftable.";
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "A series of line segments";

  fun = new_DL_Func("void", "sine", (m_uint)ftbl_sp_gen_sine);
    arg = dl_func_add_arg(fun, "int", "size");
    arg->doc = "the size";
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = " sine wave ";

  fun = new_DL_Func("void", "sinesum", (m_uint)ftbl_sp_gen_sinesum);
    arg = dl_func_add_arg(fun, "string", "argstring");
    arg->doc = "A list of amplitudes, in the range 0-1, separated by spaces.Each position coordinates to their partial number. Position 1 is the fundamental amplitude (1 * freq). Position 2 is the first overtone (2 * freq), 3 is the second (3 * freq), etc...";
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Waveform as a sum of harmonically related sine waves ";

  fun = new_DL_Func("void", "vals", (m_uint)ftbl_sp_gen_vals);
    arg = dl_func_add_arg(fun, "string", "argstring");
    arg->doc = "values";
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Reads from a wav file\n    This will only load as many samples as the length of the ftable.";

  fun = new_DL_Func("void", "xline", (m_uint)ftbl_sp_gen_xline);
    arg = dl_func_add_arg(fun, "string", "argstring");
    arg->doc = "A list of ordered xy pairs. X expects whole number integers, as they correlate to index positions in the ftable.";
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "A series of exponential segments";

  fun = new_DL_Func("void", "padsynth", (m_uint)ftbl_sp_gen_padsynth);
    arg = dl_func_add_arg(fun, "ftbl", "amps");
    arg->doc = "ftable of amplitudes to use";
    arg = dl_func_add_arg(fun, "float", "f");
    arg->doc = "Base frequency.";
    arg = dl_func_add_arg(fun, "float", "bw");
    arg->doc = "Bandwidth.";
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "An implementation of the Padsynth Algorithm by Paul Nasca. \nThis is a basic implemenation of the padsynth algorithm. More information can be found here:\nhttp://zynaddsubfx.sourceforge.net/doc/PADsynth/PADsynth.htm\nThis gen routine requires libfftw, and is not compiled by default. See config.mk for more info.        ";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_adsr* osc;
} SP_adsr;

static m_bool adsr_tick(UGen u)
{
  SP_adsr* ug = (SP_adsr*)u->ug;
  ugen_compute(u->trig->ugen);
  sp_adsr_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void adsr_ctor(M_Object obj, VM_Shred shred)
{
  SP_adsr* ug = malloc(sizeof(SP_adsr));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_adsr_create(&ug->osc);
  sp_adsr_init(ug->sp, ug->osc);
  obj->ugen->tick = adsr_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void adsr_dtor(M_Object obj, VM_Shred shred)
{
  SP_adsr* ug = (SP_adsr*)obj->ugen->ug;
  sp_adsr_destroy(&ug->osc);
  free(ug);
}

static void adsr_get_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_adsr* ug = (SP_adsr*)obj->ugen->ug;
  RETURN->v_float = ug->osc->atk * ug->sp->sr;
}

static void adsr_set_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_adsr* ug = (SP_adsr*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->atk = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void adsr_get_dec(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_adsr* ug = (SP_adsr*)obj->ugen->ug;
  RETURN->v_float = ug->osc->dec * ug->sp->sr;
}

static void adsr_set_dec(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_adsr* ug = (SP_adsr*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->dec = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void adsr_get_sus(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_adsr* ug = (SP_adsr*)obj->ugen->ug;
  RETURN->v_float = ug->osc->sus;
}

static void adsr_set_sus(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_adsr* ug = (SP_adsr*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->sus = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void adsr_get_rel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_adsr* ug = (SP_adsr*)obj->ugen->ug;
  RETURN->v_float = ug->osc->rel * ug->sp->sr;
}

static void adsr_set_rel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_adsr* ug = (SP_adsr*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->rel = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

struct Type_ t_adsr = { "Adsr", sizeof(m_uint), &t_ugen };

static m_bool import_adsr(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_adsr))
  CHECK_BB(import_class_begin(env, &t_adsr, env->global_nspc, adsr_ctor, adsr_dtor))
  env->class_def->doc = "ADSR generatorThis is an ADSR generator whose curves are created using a one-pole low pass filter.";

  fun = new_DL_Func("dur", "atk", (m_uint)adsr_get_atk);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Attack time (in seconds)";

  fun = new_DL_Func("dur", "atk", (m_uint)adsr_set_atk);
    dl_func_add_arg(fun, "dur", "atk");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Attack time (in seconds)";

  fun = new_DL_Func("dur", "dec", (m_uint)adsr_get_dec);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Decay time (in seconds)";

  fun = new_DL_Func("dur", "dec", (m_uint)adsr_set_dec);
    dl_func_add_arg(fun, "dur", "dec");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Decay time (in seconds)";

  fun = new_DL_Func("float", "sus", (m_uint)adsr_get_sus);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Sustain (in range 0-1)";

  fun = new_DL_Func("float", "sus", (m_uint)adsr_set_sus);
    dl_func_add_arg(fun, "float", "sus");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Sustain (in range 0-1)";

  fun = new_DL_Func("dur", "rel", (m_uint)adsr_get_rel);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Release time (in seconds)";

  fun = new_DL_Func("dur", "rel", (m_uint)adsr_set_rel);
    dl_func_add_arg(fun, "dur", "rel");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Release time (in seconds)";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_allpass* osc;
  char is_init;
  m_float     looptime;
} SP_allpass;

static m_bool allpass_tick(UGen u)
{
  SP_allpass* ug = (SP_allpass*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  base_tick(u);
  sp_allpass_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void allpass_ctor(M_Object obj, VM_Shred shred)
{
  SP_allpass* ug = malloc(sizeof(SP_allpass));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_allpass_create(&ug->osc);
  obj->ugen->tick = allpass_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void allpass_dtor(M_Object obj, VM_Shred shred)
{
  SP_allpass* ug = (SP_allpass*)obj->ugen->ug;
  if(ug->is_init)
    sp_allpass_destroy(&ug->osc);
  free(ug);
}

static void allpass_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_allpass* ug = (SP_allpass*)obj->ugen->ug;
  if(ug->is_init)
  allpass_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    looptime   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_allpass_init(ug->sp, ug->osc, looptime / (m_float)ug->sp->sr);
}

static void allpass_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_allpass* ug = (SP_allpass*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    looptime   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_allpass_init(ug->sp, ug->osc, looptime / (m_float)ug->sp->sr);
}

static void allpass_get_revtime(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_allpass* ug = (SP_allpass*)obj->ugen->ug;
  RETURN->v_float = ug->osc->revtime * ug->sp->sr;
}

static void allpass_set_revtime(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_allpass* ug = (SP_allpass*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->revtime = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

struct Type_ t_allpass = { "Allpass", sizeof(m_uint), &t_ugen };

static m_bool import_allpass(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_allpass))
  CHECK_BB(import_class_begin(env, &t_allpass, env->global_nspc, allpass_ctor, allpass_dtor))
  env->class_def->doc = "Allpass filter    Often used for the creation of reverb modules.";

  fun = new_DL_Func("void", "init", (m_uint)allpass_init);
   dl_func_add_arg(fun, "dur", "looptime");
  f->doc = "The loop time of the filter, in seconds. This can also be thought of as the delay time.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)allpass_init_array);
   dl_func_add_arg(fun, "dur", "looptime");
  f->doc = "The loop time of the filter, in seconds. This can also be thought of as the delay time.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("dur", "revtime", (m_uint)allpass_get_revtime);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The reverberation time, in seconds (RT-60).";

  fun = new_DL_Func("dur", "revtime", (m_uint)allpass_set_revtime);
    dl_func_add_arg(fun, "dur", "revtime");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The reverberation time, in seconds (RT-60).";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_atone* osc;
} SP_atone;

static m_bool atone_tick(UGen u)
{
  SP_atone* ug = (SP_atone*)u->ug;
  base_tick(u);
  sp_atone_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void atone_ctor(M_Object obj, VM_Shred shred)
{
  SP_atone* ug = malloc(sizeof(SP_atone));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_atone_create(&ug->osc);
  sp_atone_init(ug->sp, ug->osc);
  obj->ugen->tick = atone_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void atone_dtor(M_Object obj, VM_Shred shred)
{
  SP_atone* ug = (SP_atone*)obj->ugen->ug;
  sp_atone_destroy(&ug->osc);
  free(ug);
}

static void atone_get_hp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_atone* ug = (SP_atone*)obj->ugen->ug;
  RETURN->v_float = ug->osc->hp;
}

static void atone_set_hp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_atone* ug = (SP_atone*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->hp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_atone = { "Atone", sizeof(m_uint), &t_ugen };

static m_bool import_atone(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_atone))
  CHECK_BB(import_class_begin(env, &t_atone, env->global_nspc, atone_ctor, atone_dtor))
  env->class_def->doc = "First-order recursive highpass filterThis is the complement to the tone module.";

  fun = new_DL_Func("float", "hp", (m_uint)atone_get_hp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The response curve's half power point (cutoff frequency).";

  fun = new_DL_Func("float", "hp", (m_uint)atone_set_hp);
    dl_func_add_arg(fun, "float", "hp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The response curve's half power point (cutoff frequency).";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_autowah* osc;
} SP_autowah;

static m_bool autowah_tick(UGen u)
{
  SP_autowah* ug = (SP_autowah*)u->ug;
  base_tick(u);
  sp_autowah_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void autowah_ctor(M_Object obj, VM_Shred shred)
{
  SP_autowah* ug = malloc(sizeof(SP_autowah));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_autowah_create(&ug->osc);
  sp_autowah_init(ug->sp, ug->osc);
  obj->ugen->tick = autowah_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void autowah_dtor(M_Object obj, VM_Shred shred)
{
  SP_autowah* ug = (SP_autowah*)obj->ugen->ug;
  sp_autowah_destroy(&ug->osc);
  free(ug);
}

static void autowah_get_level(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_autowah* ug = (SP_autowah*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->level;
}

static void autowah_set_level(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_autowah* ug = (SP_autowah*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->level = *(m_float*)(shred->mem + SZ_INT));
}

static void autowah_get_wah(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_autowah* ug = (SP_autowah*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->wah;
}

static void autowah_set_wah(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_autowah* ug = (SP_autowah*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->wah = *(m_float*)(shred->mem + SZ_INT));
}

static void autowah_get_mix(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_autowah* ug = (SP_autowah*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->mix;
}

static void autowah_set_mix(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_autowah* ug = (SP_autowah*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->mix = *(m_float*)(shred->mem + SZ_INT));
}

struct Type_ t_autowah = { "Autowah", sizeof(m_uint), &t_ugen };

static m_bool import_autowah(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_autowah))
  CHECK_BB(import_class_begin(env, &t_autowah, env->global_nspc, autowah_ctor, autowah_dtor))
  env->class_def->doc = "Automatic wah pedalAn automatic wah effect, ported from Guitarix via Faust. ";

  fun = new_DL_Func("float", "level", (m_uint)autowah_get_level);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Overall level (between 0 and 1)";

  fun = new_DL_Func("float", "level", (m_uint)autowah_set_level);
    dl_func_add_arg(fun, "float", "level");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Overall level (between 0 and 1)";

  fun = new_DL_Func("float", "wah", (m_uint)autowah_get_wah);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "wah amount";

  fun = new_DL_Func("float", "wah", (m_uint)autowah_set_wah);
    dl_func_add_arg(fun, "float", "wah");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "wah amount";

  fun = new_DL_Func("float", "mix", (m_uint)autowah_get_mix);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Wet/dry amount (100 = wet, 0 = dry)";

  fun = new_DL_Func("float", "mix", (m_uint)autowah_set_mix);
    dl_func_add_arg(fun, "float", "mix");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Wet/dry amount (100 = wet, 0 = dry)";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_bal* osc;
} SP_bal;

static m_bool bal_tick(UGen u)
{
  SP_bal* ug = (SP_bal*)u->ug;
  base_tick(u);
  ugen_compute(u->trig->ugen);
  sp_bal_compute(ug->sp, ug->osc, &u->in, &u->trig->ugen->out, &u->out);
}

static void bal_ctor(M_Object obj, VM_Shred shred)
{
  SP_bal* ug = malloc(sizeof(SP_bal));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_bal_create(&ug->osc);
  sp_bal_init(ug->sp, ug->osc);
  obj->ugen->tick = bal_tick;
  assign_ugen(obj->ugen, 1, 1, 1, ug);
}

static void bal_dtor(M_Object obj, VM_Shred shred)
{
  SP_bal* ug = (SP_bal*)obj->ugen->ug;
  sp_bal_destroy(&ug->osc);
  free(ug);
}

struct Type_ t_bal = { "Bal", sizeof(m_uint), &t_ugen };

static m_bool import_bal(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_bal))
  CHECK_BB(import_class_begin(env, &t_bal, env->global_nspc, bal_ctor, bal_dtor))
  env->class_def->doc = "Balance the gain of one signal based on another signal    This is often used to restore gain lost in the output of a filter.In the source code, the value \"ihp\" is set to a static 10hz. This is the default value in Csound, and should not often need to be changed.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_bar* osc;
  char is_init;
  m_float   iK;
  m_float   ib;
} SP_bar;

static m_bool bar_tick(UGen u)
{
  SP_bar* ug = (SP_bar*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  ugen_compute(u->trig->ugen);
  sp_bar_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void bar_ctor(M_Object obj, VM_Shred shred)
{
  SP_bar* ug = malloc(sizeof(SP_bar));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_bar_create(&ug->osc);
  obj->ugen->tick = bar_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void bar_dtor(M_Object obj, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  if(ug->is_init)
    sp_bar_destroy(&ug->osc);
  free(ug);
}

static void bar_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  if(ug->is_init)
  bar_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    iK   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  m_float    ib   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_bar_init(ug->sp, ug->osc, (ug->iK = iK), (ug->ib = ib));
  shred->reg  -= SZ_INT*1;
}

static void bar_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    iK   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  m_float    ib   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_bar_init(ug->sp, ug->osc, (ug->iK = iK), (ug->ib = ib));
}

static void bar_get_bcL(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  RETURN->v_float = ug->osc->bcL;
}

static void bar_set_bcL(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->bcL = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void bar_get_bcR(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  RETURN->v_float = ug->osc->bcR;
}

static void bar_set_bcR(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->bcR = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void bar_get_T30(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  RETURN->v_float = ug->osc->T30 * ug->sp->sr;
}

static void bar_set_T30(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->T30 = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void bar_get_scan(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  RETURN->v_float = ug->osc->scan;
}

static void bar_set_scan(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->scan = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void bar_get_pos(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  RETURN->v_float = ug->osc->pos;
}

static void bar_set_pos(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->pos = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void bar_get_vel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  RETURN->v_float = ug->osc->vel;
}

static void bar_set_vel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->vel = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void bar_get_wid(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  RETURN->v_float = ug->osc->wid;
}

static void bar_set_wid(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bar* ug = (SP_bar*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->wid = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_bar = { "Bar", sizeof(m_uint), &t_ugen };

static m_bool import_bar(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_bar))
  CHECK_BB(import_class_begin(env, &t_bar, env->global_nspc, bar_ctor, bar_dtor))
  env->class_def->doc = "Physical model approximating the sound of a struck metal bar";

  fun = new_DL_Func("void", "init", (m_uint)bar_init);
   dl_func_add_arg(fun, "float", "iK");
  f->doc = "Dimensionless stiffness parameter";

   dl_func_add_arg(fun, "float", "ib");
  f->doc = "High-frequency loss parameter. Keep this small";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)bar_init_array);
   dl_func_add_arg(fun, "float", "iK");
  f->doc = "Dimensionless stiffness parameter";

   dl_func_add_arg(fun, "float", "ib");
  f->doc = "High-frequency loss parameter. Keep this small";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "bcL", (m_uint)bar_get_bcL);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Boundary condition at left end of bar. 1 = clamped, 2 = pivoting, 3 = free";

  fun = new_DL_Func("float", "bcL", (m_uint)bar_set_bcL);
    dl_func_add_arg(fun, "float", "bcL");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Boundary condition at left end of bar. 1 = clamped, 2 = pivoting, 3 = free";

  fun = new_DL_Func("float", "bcR", (m_uint)bar_get_bcR);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Boundary condition at right end of bar. 1 = clamped, 2 = pivoting, 3 = free";

  fun = new_DL_Func("float", "bcR", (m_uint)bar_set_bcR);
    dl_func_add_arg(fun, "float", "bcR");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Boundary condition at right end of bar. 1 = clamped, 2 = pivoting, 3 = free";

  fun = new_DL_Func("dur", "T30", (m_uint)bar_get_T30);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "30db decay time (in seconds).";

  fun = new_DL_Func("dur", "T30", (m_uint)bar_set_T30);
    dl_func_add_arg(fun, "dur", "T30");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "30db decay time (in seconds).";

  fun = new_DL_Func("float", "scan", (m_uint)bar_get_scan);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Speed of scanning the output location.";

  fun = new_DL_Func("float", "scan", (m_uint)bar_set_scan);
    dl_func_add_arg(fun, "float", "scan");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Speed of scanning the output location.";

  fun = new_DL_Func("float", "pos", (m_uint)bar_get_pos);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Position along bar that strike occurs.";

  fun = new_DL_Func("float", "pos", (m_uint)bar_set_pos);
    dl_func_add_arg(fun, "float", "pos");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Position along bar that strike occurs.";

  fun = new_DL_Func("float", "vel", (m_uint)bar_get_vel);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Normalized strike velocity";

  fun = new_DL_Func("float", "vel", (m_uint)bar_set_vel);
    dl_func_add_arg(fun, "float", "vel");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Normalized strike velocity";

  fun = new_DL_Func("float", "wid", (m_uint)bar_get_wid);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Spatial width of strike.";

  fun = new_DL_Func("float", "wid", (m_uint)bar_set_wid);
    dl_func_add_arg(fun, "float", "wid");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Spatial width of strike.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_biquad* osc;
} SP_biquad;

static m_bool biquad_tick(UGen u)
{
  SP_biquad* ug = (SP_biquad*)u->ug;
  base_tick(u);
  sp_biquad_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void biquad_ctor(M_Object obj, VM_Shred shred)
{
  SP_biquad* ug = malloc(sizeof(SP_biquad));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_biquad_create(&ug->osc);
  sp_biquad_init(ug->sp, ug->osc);
  obj->ugen->tick = biquad_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void biquad_dtor(M_Object obj, VM_Shred shred)
{
  SP_biquad* ug = (SP_biquad*)obj->ugen->ug;
  sp_biquad_destroy(&ug->osc);
  free(ug);
}

static void biquad_get_b0(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_biquad* ug = (SP_biquad*)obj->ugen->ug;
  RETURN->v_float = ug->osc->b0;
}

static void biquad_set_b0(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_biquad* ug = (SP_biquad*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->b0 = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void biquad_get_b1(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_biquad* ug = (SP_biquad*)obj->ugen->ug;
  RETURN->v_float = ug->osc->b1;
}

static void biquad_set_b1(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_biquad* ug = (SP_biquad*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->b1 = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void biquad_get_b2(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_biquad* ug = (SP_biquad*)obj->ugen->ug;
  RETURN->v_float = ug->osc->b2;
}

static void biquad_set_b2(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_biquad* ug = (SP_biquad*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->b2 = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void biquad_get_a0(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_biquad* ug = (SP_biquad*)obj->ugen->ug;
  RETURN->v_float = ug->osc->a0;
}

static void biquad_set_a0(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_biquad* ug = (SP_biquad*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->a0 = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void biquad_get_a1(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_biquad* ug = (SP_biquad*)obj->ugen->ug;
  RETURN->v_float = ug->osc->a1;
}

static void biquad_set_a1(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_biquad* ug = (SP_biquad*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->a1 = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void biquad_get_a2(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_biquad* ug = (SP_biquad*)obj->ugen->ug;
  RETURN->v_float = ug->osc->a2;
}

static void biquad_set_a2(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_biquad* ug = (SP_biquad*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->a2 = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_biquad = { "Biquad", sizeof(m_uint), &t_ugen };

static m_bool import_biquad(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_biquad))
  CHECK_BB(import_class_begin(env, &t_biquad, env->global_nspc, biquad_ctor, biquad_dtor))
  env->class_def->doc = "A sweepable biquadratic general purpose filter    ";

  fun = new_DL_Func("float", "b0", (m_uint)biquad_get_b0);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "biquad coefficient.";

  fun = new_DL_Func("float", "b0", (m_uint)biquad_set_b0);
    dl_func_add_arg(fun, "float", "b0");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "biquad coefficient.";

  fun = new_DL_Func("float", "b1", (m_uint)biquad_get_b1);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "biquad coefficient.";

  fun = new_DL_Func("float", "b1", (m_uint)biquad_set_b1);
    dl_func_add_arg(fun, "float", "b1");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "biquad coefficient.";

  fun = new_DL_Func("float", "b2", (m_uint)biquad_get_b2);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "biquad coefficient.";

  fun = new_DL_Func("float", "b2", (m_uint)biquad_set_b2);
    dl_func_add_arg(fun, "float", "b2");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "biquad coefficient.";

  fun = new_DL_Func("float", "a0", (m_uint)biquad_get_a0);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "biquad coefficient.";

  fun = new_DL_Func("float", "a0", (m_uint)biquad_set_a0);
    dl_func_add_arg(fun, "float", "a0");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "biquad coefficient.";

  fun = new_DL_Func("float", "a1", (m_uint)biquad_get_a1);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "biquad coefficient.";

  fun = new_DL_Func("float", "a1", (m_uint)biquad_set_a1);
    dl_func_add_arg(fun, "float", "a1");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "biquad coefficient.";

  fun = new_DL_Func("float", "a2", (m_uint)biquad_get_a2);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "biquad coefficient.";

  fun = new_DL_Func("float", "a2", (m_uint)biquad_set_a2);
    dl_func_add_arg(fun, "float", "a2");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "biquad coefficient.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_biscale* osc;
} SP_biscale;

static m_bool biscale_tick(UGen u)
{
  SP_biscale* ug = (SP_biscale*)u->ug;
  base_tick(u);
  sp_biscale_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void biscale_ctor(M_Object obj, VM_Shred shred)
{
  SP_biscale* ug = malloc(sizeof(SP_biscale));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_biscale_create(&ug->osc);
  sp_biscale_init(ug->sp, ug->osc);
  obj->ugen->tick = biscale_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void biscale_dtor(M_Object obj, VM_Shred shred)
{
  SP_biscale* ug = (SP_biscale*)obj->ugen->ug;
  sp_biscale_destroy(&ug->osc);
  free(ug);
}

static void biscale_get_min(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_biscale* ug = (SP_biscale*)obj->ugen->ug;
  RETURN->v_float = ug->osc->min;
}

static void biscale_set_min(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_biscale* ug = (SP_biscale*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->min = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void biscale_get_max(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_biscale* ug = (SP_biscale*)obj->ugen->ug;
  RETURN->v_float = ug->osc->max;
}

static void biscale_set_max(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_biscale* ug = (SP_biscale*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->max = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_biscale = { "Biscale", sizeof(m_uint), &t_ugen };

static m_bool import_biscale(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_biscale))
  CHECK_BB(import_class_begin(env, &t_biscale, env->global_nspc, biscale_ctor, biscale_dtor))
  env->class_def->doc = "Bipolar Scale    This module scales from bipolar [-1, 1] to another range defined by min and max.";

  fun = new_DL_Func("float", "min", (m_uint)biscale_get_min);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Minimum value to scale to.";

  fun = new_DL_Func("float", "min", (m_uint)biscale_set_min);
    dl_func_add_arg(fun, "float", "min");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Minimum value to scale to.";

  fun = new_DL_Func("float", "max", (m_uint)biscale_get_max);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Maximum value to scale to.";

  fun = new_DL_Func("float", "max", (m_uint)biscale_set_max);
    dl_func_add_arg(fun, "float", "max");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Maximum value to scale to.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_bitcrush* osc;
} SP_bitcrush;

static m_bool bitcrush_tick(UGen u)
{
  SP_bitcrush* ug = (SP_bitcrush*)u->ug;
  base_tick(u);
  sp_bitcrush_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void bitcrush_ctor(M_Object obj, VM_Shred shred)
{
  SP_bitcrush* ug = malloc(sizeof(SP_bitcrush));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_bitcrush_create(&ug->osc);
  sp_bitcrush_init(ug->sp, ug->osc);
  obj->ugen->tick = bitcrush_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void bitcrush_dtor(M_Object obj, VM_Shred shred)
{
  SP_bitcrush* ug = (SP_bitcrush*)obj->ugen->ug;
  sp_bitcrush_destroy(&ug->osc);
  free(ug);
}

static void bitcrush_get_bitdepth(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bitcrush* ug = (SP_bitcrush*)obj->ugen->ug;
  RETURN->v_float = ug->osc->bitdepth;
}

static void bitcrush_set_bitdepth(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bitcrush* ug = (SP_bitcrush*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->bitdepth = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void bitcrush_get_srate(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bitcrush* ug = (SP_bitcrush*)obj->ugen->ug;
  RETURN->v_float = ug->osc->srate;
}

static void bitcrush_set_srate(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bitcrush* ug = (SP_bitcrush*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->srate = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_bitcrush = { "Bitcrush", sizeof(m_uint), &t_ugen };

static m_bool import_bitcrush(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_bitcrush))
  CHECK_BB(import_class_begin(env, &t_bitcrush, env->global_nspc, bitcrush_ctor, bitcrush_dtor))
  env->class_def->doc = "Digital signal degradation    Bitcrusher will digitally degrade a signal by altering the bitdepth and sampling-rate. ";

  fun = new_DL_Func("float", "bitdepth", (m_uint)bitcrush_get_bitdepth);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Bit depth. Expects an integer in the range of 1-16. Fractional values will be truncated.";

  fun = new_DL_Func("float", "bitdepth", (m_uint)bitcrush_set_bitdepth);
    dl_func_add_arg(fun, "float", "bitdepth");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Bit depth. Expects an integer in the range of 1-16. Fractional values will be truncated.";

  fun = new_DL_Func("float", "srate", (m_uint)bitcrush_get_srate);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Sampling rate.";

  fun = new_DL_Func("float", "srate", (m_uint)bitcrush_set_srate);
    dl_func_add_arg(fun, "float", "srate");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Sampling rate.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_blsaw* osc;
} SP_blsaw;

static m_bool blsaw_tick(UGen u)
{
  SP_blsaw* ug = (SP_blsaw*)u->ug;
  sp_blsaw_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void blsaw_ctor(M_Object obj, VM_Shred shred)
{
  SP_blsaw* ug = malloc(sizeof(SP_blsaw));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_blsaw_create(&ug->osc);
  sp_blsaw_init(ug->sp, ug->osc);
  obj->ugen->tick = blsaw_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void blsaw_dtor(M_Object obj, VM_Shred shred)
{
  SP_blsaw* ug = (SP_blsaw*)obj->ugen->ug;
  sp_blsaw_destroy(&ug->osc);
  free(ug);
}

static void blsaw_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_blsaw* ug = (SP_blsaw*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->freq;
}

static void blsaw_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_blsaw* ug = (SP_blsaw*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->freq = *(m_float*)(shred->mem + SZ_INT));
}

static void blsaw_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_blsaw* ug = (SP_blsaw*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->amp;
}

static void blsaw_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_blsaw* ug = (SP_blsaw*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->amp = *(m_float*)(shred->mem + SZ_INT));
}

struct Type_ t_blsaw = { "Blsaw", sizeof(m_uint), &t_ugen };

static m_bool import_blsaw(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_blsaw))
  CHECK_BB(import_class_begin(env, &t_blsaw, env->global_nspc, blsaw_ctor, blsaw_dtor))
  env->class_def->doc = "Bandlimited sawtooth oscillatorThis is a bandlimited sawtooth oscillator ported from the \"sawtooth\" function from the Faustprogramming language.";

  fun = new_DL_Func("float", "freq", (m_uint)blsaw_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency, (range 0-20000)";

  fun = new_DL_Func("float", "freq", (m_uint)blsaw_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency, (range 0-20000)";

  fun = new_DL_Func("float", "amp", (m_uint)blsaw_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude (range 0-1).";

  fun = new_DL_Func("float", "amp", (m_uint)blsaw_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude (range 0-1).";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_blsquare* osc;
} SP_blsquare;

static m_bool blsquare_tick(UGen u)
{
  SP_blsquare* ug = (SP_blsquare*)u->ug;
  sp_blsquare_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void blsquare_ctor(M_Object obj, VM_Shred shred)
{
  SP_blsquare* ug = malloc(sizeof(SP_blsquare));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_blsquare_create(&ug->osc);
  sp_blsquare_init(ug->sp, ug->osc);
  obj->ugen->tick = blsquare_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void blsquare_dtor(M_Object obj, VM_Shred shred)
{
  SP_blsquare* ug = (SP_blsquare*)obj->ugen->ug;
  sp_blsquare_destroy(&ug->osc);
  free(ug);
}

static void blsquare_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_blsquare* ug = (SP_blsquare*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->freq;
}

static void blsquare_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_blsquare* ug = (SP_blsquare*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->freq = *(m_float*)(shred->mem + SZ_INT));
}

static void blsquare_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_blsquare* ug = (SP_blsquare*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->amp;
}

static void blsquare_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_blsquare* ug = (SP_blsquare*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->amp = *(m_float*)(shred->mem + SZ_INT));
}

static void blsquare_get_width(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_blsquare* ug = (SP_blsquare*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->width;
}

static void blsquare_set_width(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_blsquare* ug = (SP_blsquare*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->width = *(m_float*)(shred->mem + SZ_INT));
}

struct Type_ t_blsquare = { "Blsquare", sizeof(m_uint), &t_ugen };

static m_bool import_blsquare(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_blsquare))
  CHECK_BB(import_class_begin(env, &t_blsquare, env->global_nspc, blsquare_ctor, blsquare_dtor))
  env->class_def->doc = "Bandlimited square oscillatorThis is a bandlimited square oscillator ported from the \"squaretooth\" function from the Faustprogramming language.";

  fun = new_DL_Func("float", "freq", (m_uint)blsquare_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency, (range 0-20000)";

  fun = new_DL_Func("float", "freq", (m_uint)blsquare_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency, (range 0-20000)";

  fun = new_DL_Func("float", "amp", (m_uint)blsquare_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude (range 0-1).";

  fun = new_DL_Func("float", "amp", (m_uint)blsquare_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude (range 0-1).";

  fun = new_DL_Func("float", "width", (m_uint)blsquare_get_width);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Duty cycle width (range 0-1).";

  fun = new_DL_Func("float", "width", (m_uint)blsquare_set_width);
    dl_func_add_arg(fun, "float", "width");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Duty cycle width (range 0-1).";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_bltriangle* osc;
} SP_bltriangle;

static m_bool bltriangle_tick(UGen u)
{
  SP_bltriangle* ug = (SP_bltriangle*)u->ug;
  sp_bltriangle_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void bltriangle_ctor(M_Object obj, VM_Shred shred)
{
  SP_bltriangle* ug = malloc(sizeof(SP_bltriangle));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_bltriangle_create(&ug->osc);
  sp_bltriangle_init(ug->sp, ug->osc);
  obj->ugen->tick = bltriangle_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void bltriangle_dtor(M_Object obj, VM_Shred shred)
{
  SP_bltriangle* ug = (SP_bltriangle*)obj->ugen->ug;
  sp_bltriangle_destroy(&ug->osc);
  free(ug);
}

static void bltriangle_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bltriangle* ug = (SP_bltriangle*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->freq;
}

static void bltriangle_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bltriangle* ug = (SP_bltriangle*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->freq = *(m_float*)(shred->mem + SZ_INT));
}

static void bltriangle_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bltriangle* ug = (SP_bltriangle*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->amp;
}

static void bltriangle_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_bltriangle* ug = (SP_bltriangle*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->amp = *(m_float*)(shred->mem + SZ_INT));
}

struct Type_ t_bltriangle = { "Bltriangle", sizeof(m_uint), &t_ugen };

static m_bool import_bltriangle(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_bltriangle))
  CHECK_BB(import_class_begin(env, &t_bltriangle, env->global_nspc, bltriangle_ctor, bltriangle_dtor))
  env->class_def->doc = "Bandlimited triangle oscillatorThis is a bandlimited triangle oscillator ported from the \"triangletooth\" function from the Faustprogramming language.";

  fun = new_DL_Func("float", "freq", (m_uint)bltriangle_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency, (range 0-20000)";

  fun = new_DL_Func("float", "freq", (m_uint)bltriangle_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency, (range 0-20000)";

  fun = new_DL_Func("float", "amp", (m_uint)bltriangle_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude (range 0-1).";

  fun = new_DL_Func("float", "amp", (m_uint)bltriangle_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude (range 0-1).";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_butbp* osc;
} SP_butbp;

static m_bool butbp_tick(UGen u)
{
  SP_butbp* ug = (SP_butbp*)u->ug;
  base_tick(u);
  sp_butbp_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void butbp_ctor(M_Object obj, VM_Shred shred)
{
  SP_butbp* ug = malloc(sizeof(SP_butbp));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_butbp_create(&ug->osc);
  sp_butbp_init(ug->sp, ug->osc);
  obj->ugen->tick = butbp_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void butbp_dtor(M_Object obj, VM_Shred shred)
{
  SP_butbp* ug = (SP_butbp*)obj->ugen->ug;
  sp_butbp_destroy(&ug->osc);
  free(ug);
}

static void butbp_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_butbp* ug = (SP_butbp*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void butbp_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_butbp* ug = (SP_butbp*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void butbp_get_bw(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_butbp* ug = (SP_butbp*)obj->ugen->ug;
  RETURN->v_float = ug->osc->bw;
}

static void butbp_set_bw(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_butbp* ug = (SP_butbp*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->bw = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_butbp = { "Butbp", sizeof(m_uint), &t_ugen };

static m_bool import_butbp(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_butbp))
  CHECK_BB(import_class_begin(env, &t_butbp, env->global_nspc, butbp_ctor, butbp_dtor))
  env->class_def->doc = "Bandpass Butterworth filter";

  fun = new_DL_Func("float", "freq", (m_uint)butbp_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Center Frequency. (in Hertz)";

  fun = new_DL_Func("float", "freq", (m_uint)butbp_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Center Frequency. (in Hertz)";

  fun = new_DL_Func("float", "bw", (m_uint)butbp_get_bw);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Bandwidth. (in Hertz)";

  fun = new_DL_Func("float", "bw", (m_uint)butbp_set_bw);
    dl_func_add_arg(fun, "float", "bw");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Bandwidth. (in Hertz)";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_butbr* osc;
} SP_butbr;

static m_bool butbr_tick(UGen u)
{
  SP_butbr* ug = (SP_butbr*)u->ug;
  base_tick(u);
  sp_butbr_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void butbr_ctor(M_Object obj, VM_Shred shred)
{
  SP_butbr* ug = malloc(sizeof(SP_butbr));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_butbr_create(&ug->osc);
  sp_butbr_init(ug->sp, ug->osc);
  obj->ugen->tick = butbr_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void butbr_dtor(M_Object obj, VM_Shred shred)
{
  SP_butbr* ug = (SP_butbr*)obj->ugen->ug;
  sp_butbr_destroy(&ug->osc);
  free(ug);
}

static void butbr_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_butbr* ug = (SP_butbr*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void butbr_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_butbr* ug = (SP_butbr*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void butbr_get_bw(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_butbr* ug = (SP_butbr*)obj->ugen->ug;
  RETURN->v_float = ug->osc->bw;
}

static void butbr_set_bw(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_butbr* ug = (SP_butbr*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->bw = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_butbr = { "Butbr", sizeof(m_uint), &t_ugen };

static m_bool import_butbr(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_butbr))
  CHECK_BB(import_class_begin(env, &t_butbr, env->global_nspc, butbr_ctor, butbr_dtor))
  env->class_def->doc = "Band-reject Butterworth filter";

  fun = new_DL_Func("float", "freq", (m_uint)butbr_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Center Frequency. (in Hertz)";

  fun = new_DL_Func("float", "freq", (m_uint)butbr_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Center Frequency. (in Hertz)";

  fun = new_DL_Func("float", "bw", (m_uint)butbr_get_bw);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Bandwidth. (in Hertz)";

  fun = new_DL_Func("float", "bw", (m_uint)butbr_set_bw);
    dl_func_add_arg(fun, "float", "bw");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Bandwidth. (in Hertz)";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_buthp* osc;
} SP_buthp;

static m_bool buthp_tick(UGen u)
{
  SP_buthp* ug = (SP_buthp*)u->ug;
  base_tick(u);
  sp_buthp_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void buthp_ctor(M_Object obj, VM_Shred shred)
{
  SP_buthp* ug = malloc(sizeof(SP_buthp));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_buthp_create(&ug->osc);
  sp_buthp_init(ug->sp, ug->osc);
  obj->ugen->tick = buthp_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void buthp_dtor(M_Object obj, VM_Shred shred)
{
  SP_buthp* ug = (SP_buthp*)obj->ugen->ug;
  sp_buthp_destroy(&ug->osc);
  free(ug);
}

static void buthp_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_buthp* ug = (SP_buthp*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void buthp_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_buthp* ug = (SP_buthp*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_buthp = { "Buthp", sizeof(m_uint), &t_ugen };

static m_bool import_buthp(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_buthp))
  CHECK_BB(import_class_begin(env, &t_buthp, env->global_nspc, buthp_ctor, buthp_dtor))
  env->class_def->doc = "Highpass Butterworth filter";

  fun = new_DL_Func("float", "freq", (m_uint)buthp_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Cutoff Frequency.";

  fun = new_DL_Func("float", "freq", (m_uint)buthp_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Cutoff Frequency.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_butlp* osc;
} SP_butlp;

static m_bool butlp_tick(UGen u)
{
  SP_butlp* ug = (SP_butlp*)u->ug;
  base_tick(u);
  sp_butlp_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void butlp_ctor(M_Object obj, VM_Shred shred)
{
  SP_butlp* ug = malloc(sizeof(SP_butlp));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_butlp_create(&ug->osc);
  sp_butlp_init(ug->sp, ug->osc);
  obj->ugen->tick = butlp_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void butlp_dtor(M_Object obj, VM_Shred shred)
{
  SP_butlp* ug = (SP_butlp*)obj->ugen->ug;
  sp_butlp_destroy(&ug->osc);
  free(ug);
}

static void butlp_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_butlp* ug = (SP_butlp*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void butlp_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_butlp* ug = (SP_butlp*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_butlp = { "Butlp", sizeof(m_uint), &t_ugen };

static m_bool import_butlp(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_butlp))
  CHECK_BB(import_class_begin(env, &t_butlp, env->global_nspc, butlp_ctor, butlp_dtor))
  env->class_def->doc = "Lowpass Butterworth filter";

  fun = new_DL_Func("float", "freq", (m_uint)butlp_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Cutoff Frequency.";

  fun = new_DL_Func("float", "freq", (m_uint)butlp_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Cutoff Frequency.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_clip* osc;
} SP_clip;

static m_bool clip_tick(UGen u)
{
  SP_clip* ug = (SP_clip*)u->ug;
  base_tick(u);
  sp_clip_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void clip_ctor(M_Object obj, VM_Shred shred)
{
  SP_clip* ug = malloc(sizeof(SP_clip));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_clip_create(&ug->osc);
  sp_clip_init(ug->sp, ug->osc);
  obj->ugen->tick = clip_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void clip_dtor(M_Object obj, VM_Shred shred)
{
  SP_clip* ug = (SP_clip*)obj->ugen->ug;
  sp_clip_destroy(&ug->osc);
  free(ug);
}

static void clip_get_lim(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_clip* ug = (SP_clip*)obj->ugen->ug;
  RETURN->v_float = ug->osc->lim;
}

static void clip_set_lim(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_clip* ug = (SP_clip*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->lim = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_clip = { "Clip", sizeof(m_uint), &t_ugen };

static m_bool import_clip(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_clip))
  CHECK_BB(import_class_begin(env, &t_clip, env->global_nspc, clip_ctor, clip_dtor))
  env->class_def->doc = "Applies clip-limiting to a signal";

  fun = new_DL_Func("float", "lim", (m_uint)clip_get_lim);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "threshold / limiting value.";

  fun = new_DL_Func("float", "lim", (m_uint)clip_set_lim);
    dl_func_add_arg(fun, "float", "lim");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "threshold / limiting value.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_comb* osc;
  char is_init;
  m_float     looptime;
} SP_comb;

static m_bool comb_tick(UGen u)
{
  SP_comb* ug = (SP_comb*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  base_tick(u);
  sp_comb_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void comb_ctor(M_Object obj, VM_Shred shred)
{
  SP_comb* ug = malloc(sizeof(SP_comb));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_comb_create(&ug->osc);
  obj->ugen->tick = comb_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void comb_dtor(M_Object obj, VM_Shred shred)
{
  SP_comb* ug = (SP_comb*)obj->ugen->ug;
  if(ug->is_init)
    sp_comb_destroy(&ug->osc);
  free(ug);
}

static void comb_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_comb* ug = (SP_comb*)obj->ugen->ug;
  if(ug->is_init)
  comb_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    looptime   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_comb_init(ug->sp, ug->osc, looptime / (m_float)ug->sp->sr);
}

static void comb_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_comb* ug = (SP_comb*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    looptime   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_comb_init(ug->sp, ug->osc, looptime / (m_float)ug->sp->sr);
}

static void comb_get_revtime(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_comb* ug = (SP_comb*)obj->ugen->ug;
  RETURN->v_float = ug->osc->revtime * ug->sp->sr;
}

static void comb_set_revtime(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_comb* ug = (SP_comb*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->revtime = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

struct Type_ t_comb = { "Comb", sizeof(m_uint), &t_ugen };

static m_bool import_comb(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_comb))
  CHECK_BB(import_class_begin(env, &t_comb, env->global_nspc, comb_ctor, comb_dtor))
  env->class_def->doc = "Comb filter";

  fun = new_DL_Func("void", "init", (m_uint)comb_init);
   dl_func_add_arg(fun, "dur", "looptime");
  f->doc = "The loop time of the filter, in seconds. This can also be thought of as the delay time.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)comb_init_array);
   dl_func_add_arg(fun, "dur", "looptime");
  f->doc = "The loop time of the filter, in seconds. This can also be thought of as the delay time.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("dur", "revtime", (m_uint)comb_get_revtime);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Reverberation time, in seconds (RT-60).";

  fun = new_DL_Func("dur", "revtime", (m_uint)comb_set_revtime);
    dl_func_add_arg(fun, "dur", "revtime");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Reverberation time, in seconds (RT-60).";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_compressor* osc;
} SP_compressor;

static m_bool compressor_tick(UGen u)
{
  SP_compressor* ug = (SP_compressor*)u->ug;
  base_tick(u);
  sp_compressor_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void compressor_ctor(M_Object obj, VM_Shred shred)
{
  SP_compressor* ug = malloc(sizeof(SP_compressor));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_compressor_create(&ug->osc);
  sp_compressor_init(ug->sp, ug->osc);
  obj->ugen->tick = compressor_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void compressor_dtor(M_Object obj, VM_Shred shred)
{
  SP_compressor* ug = (SP_compressor*)obj->ugen->ug;
  sp_compressor_destroy(&ug->osc);
  free(ug);
}

static void compressor_get_ratio(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_compressor* ug = (SP_compressor*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->ratio;
}

static void compressor_set_ratio(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_compressor* ug = (SP_compressor*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->ratio = *(m_float*)(shred->mem + SZ_INT));
}

static void compressor_get_thresh(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_compressor* ug = (SP_compressor*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->thresh;
}

static void compressor_set_thresh(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_compressor* ug = (SP_compressor*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->thresh = *(m_float*)(shred->mem + SZ_INT));
}

static void compressor_get_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_compressor* ug = (SP_compressor*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->atk;
}

static void compressor_set_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_compressor* ug = (SP_compressor*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->atk = *(m_float*)(shred->mem + SZ_INT));
}

static void compressor_get_rel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_compressor* ug = (SP_compressor*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->rel;
}

static void compressor_set_rel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_compressor* ug = (SP_compressor*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->rel = *(m_float*)(shred->mem + SZ_INT));
}

struct Type_ t_compressor = { "Compressor", sizeof(m_uint), &t_ugen };

static m_bool import_compressor(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_compressor))
  CHECK_BB(import_class_begin(env, &t_compressor, env->global_nspc, compressor_ctor, compressor_dtor))
  env->class_def->doc = "Compressor";

  fun = new_DL_Func("float", "ratio", (m_uint)compressor_get_ratio);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Ratio to compress with, a value > 1 will compress";

  fun = new_DL_Func("float", "ratio", (m_uint)compressor_set_ratio);
    dl_func_add_arg(fun, "float", "ratio");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Ratio to compress with, a value > 1 will compress";

  fun = new_DL_Func("float", "thresh", (m_uint)compressor_get_thresh);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Threshold (in dB) 0 = max";

  fun = new_DL_Func("float", "thresh", (m_uint)compressor_set_thresh);
    dl_func_add_arg(fun, "float", "thresh");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Threshold (in dB) 0 = max";

  fun = new_DL_Func("float", "atk", (m_uint)compressor_get_atk);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Compressor attack";

  fun = new_DL_Func("float", "atk", (m_uint)compressor_set_atk);
    dl_func_add_arg(fun, "float", "atk");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Compressor attack";

  fun = new_DL_Func("float", "rel", (m_uint)compressor_get_rel);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Compressor release";

  fun = new_DL_Func("float", "rel", (m_uint)compressor_set_rel);
    dl_func_add_arg(fun, "float", "rel");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Compressor release";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_conv* osc;
  char is_init;
  sp_ftbl*  ft;
  m_float   iPartLen;
} SP_conv;

static m_bool conv_tick(UGen u)
{
  SP_conv* ug = (SP_conv*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  base_tick(u);
  sp_conv_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void conv_ctor(M_Object obj, VM_Shred shred)
{
  SP_conv* ug = malloc(sizeof(SP_conv));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_conv_create(&ug->osc);
  obj->ugen->tick = conv_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void conv_dtor(M_Object obj, VM_Shred shred)
{
  SP_conv* ug = (SP_conv*)obj->ugen->ug;
  if(ug->is_init)
    sp_conv_destroy(&ug->osc);
  if(ug->is_init)
    sp_ftbl_destroy(&ug->ft);
  free(ug);
}

static void conv_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_conv* ug = (SP_conv*)obj->ugen->ug;
  if(ug->is_init)
  conv_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object ft_obj   = *(M_Object*)(shred->mem + offset);
  sp_ftbl* ft_ftbl  = FTBL(ft_obj);
  ug->ft            = ft_ftbl;
  offset += SZ_INT;
  m_float    iPartLen   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_conv_init(ug->sp, ug->osc, ft_ftbl, (ug->iPartLen = iPartLen));
  shred->reg  -= SZ_INT*1;
}

static void conv_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_conv* ug = (SP_conv*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  sp_ftbl* ft_ftbl = ug->ft = to_ftbl(*(M_Object*)(shred->mem + offset), shred);
  offset += SZ_INT;
  m_float    iPartLen   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_conv_init(ug->sp, ug->osc, ft_ftbl, (ug->iPartLen = iPartLen));
}

struct Type_ t_conv = { "Conv", sizeof(m_uint), &t_ugen };

static m_bool import_conv(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_conv))
  CHECK_BB(import_class_begin(env, &t_conv, env->global_nspc, conv_ctor, conv_dtor))
  env->class_def->doc = "Partioned convolution. This module will perform partitioned convolution on an input signal usingan ftable as an impulse response.";

  fun = new_DL_Func("void", "init", (m_uint)conv_init);
   dl_func_add_arg(fun, "ftbl", "ft");
  f->doc = "Ftable used as the impulse response. ";

   dl_func_add_arg(fun, "float", "iPartLen");
  f->doc = "Partition length (in samples). Must be a power of 2. Lower values will add less latency, at the cost of requiring more CPU power. ";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)conv_init_array);
   dl_func_add_arg(fun, "float[]", "ft");
  f->doc = "Ftable used as the impulse response. ";

   dl_func_add_arg(fun, "float", "iPartLen");
  f->doc = "Partition length (in samples). Must be a power of 2. Lower values will add less latency, at the cost of requiring more CPU power. ";

  CHECK_BB((f = import_mfun(env, fun)))

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_count* osc;
} SP_count;

static m_bool count_tick(UGen u)
{
  SP_count* ug = (SP_count*)u->ug;
  ugen_compute(u->trig->ugen);
  sp_count_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void count_ctor(M_Object obj, VM_Shred shred)
{
  SP_count* ug = malloc(sizeof(SP_count));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_count_create(&ug->osc);
  sp_count_init(ug->sp, ug->osc);
  obj->ugen->tick = count_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void count_dtor(M_Object obj, VM_Shred shred)
{
  SP_count* ug = (SP_count*)obj->ugen->ug;
  sp_count_destroy(&ug->osc);
  free(ug);
}

static void count_get_count(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_count* ug = (SP_count*)obj->ugen->ug;
  RETURN->v_float = ug->osc->count;
}

static void count_set_count(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_count* ug = (SP_count*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->count = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void count_get_mode(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_count* ug = (SP_count*)obj->ugen->ug;
  RETURN->v_float = ug->osc->mode;
}

static void count_set_mode(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_count* ug = (SP_count*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->mode = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_count = { "Count", sizeof(m_uint), &t_ugen };

static m_bool import_count(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_count))
  CHECK_BB(import_class_begin(env, &t_count, env->global_nspc, count_ctor, count_dtor))
  env->class_def->doc = "Trigger-based fixed counter    The signal output will count from 0 to [N-1], and thenrepeat itself. Count will start when it has been triggered, otherwise it will be -1.";

  fun = new_DL_Func("float", "count", (m_uint)count_get_count);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Number to count up to (count - 1). Decimal points will be truncated.";

  fun = new_DL_Func("float", "count", (m_uint)count_set_count);
    dl_func_add_arg(fun, "float", "count");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Number to count up to (count - 1). Decimal points will be truncated.";

  fun = new_DL_Func("float", "mode", (m_uint)count_get_mode);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Counting mode. 0 = wrap-around, 1 = count up to N -1, then stop and spit out -1";

  fun = new_DL_Func("float", "mode", (m_uint)count_set_mode);
    dl_func_add_arg(fun, "float", "mode");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Counting mode. 0 = wrap-around, 1 = count up to N -1, then stop and spit out -1";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_crossfade* osc;
} SP_crossfade;

static m_bool crossfade_tick(UGen u)
{
  SP_crossfade* ug = (SP_crossfade*)u->ug;
  M_Object l = (M_Object)u->channel[0];
  M_Object r = (M_Object)u->channel[1];
  ugen_compute(l->ugen);
  ugen_compute(l->ugen);
  sp_crossfade_compute(ug->sp, ug->osc, &l->ugen->in, &r->ugen->in, &u->out);
}

static void crossfade_ctor(M_Object obj, VM_Shred shred)
{
  SP_crossfade* ug = malloc(sizeof(SP_crossfade));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_crossfade_create(&ug->osc);
  sp_crossfade_init(ug->sp, ug->osc);
  obj->ugen->tick = crossfade_tick;
  assign_ugen(obj->ugen, 2, 1, 0, ug);
}

static void crossfade_dtor(M_Object obj, VM_Shred shred)
{
  SP_crossfade* ug = (SP_crossfade*)obj->ugen->ug;
  sp_crossfade_destroy(&ug->osc);
  free(ug);
}

static void crossfade_get_pos(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_crossfade* ug = (SP_crossfade*)obj->ugen->ug;
  RETURN->v_float = ug->osc->pos;
}

static void crossfade_set_pos(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_crossfade* ug = (SP_crossfade*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->pos = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_crossfade = { "Crossfade", sizeof(m_uint), &t_ugen };

static m_bool import_crossfade(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_crossfade))
  CHECK_BB(import_class_begin(env, &t_crossfade, env->global_nspc, crossfade_ctor, crossfade_dtor))
  env->class_def->doc = "Crossfade two signals. This module will perform a linear crossfade between two input signals.";

  fun = new_DL_Func("float", "pos", (m_uint)crossfade_get_pos);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Crossfade position. 0 = all signal 1, 1 = all signal 2";

  fun = new_DL_Func("float", "pos", (m_uint)crossfade_set_pos);
    dl_func_add_arg(fun, "float", "pos");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Crossfade position. 0 = all signal 1, 1 = all signal 2";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_dcblock* osc;
} SP_dcblock;

static m_bool dcblock_tick(UGen u)
{
  SP_dcblock* ug = (SP_dcblock*)u->ug;
  base_tick(u);
  sp_dcblock_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void dcblock_ctor(M_Object obj, VM_Shred shred)
{
  SP_dcblock* ug = malloc(sizeof(SP_dcblock));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_dcblock_create(&ug->osc);
  sp_dcblock_init(ug->sp, ug->osc);
  obj->ugen->tick = dcblock_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void dcblock_dtor(M_Object obj, VM_Shred shred)
{
  SP_dcblock* ug = (SP_dcblock*)obj->ugen->ug;
  sp_dcblock_destroy(&ug->osc);
  free(ug);
}

struct Type_ t_dcblock = { "Dcblock", sizeof(m_uint), &t_ugen };

static m_bool import_dcblock(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_dcblock))
  CHECK_BB(import_class_begin(env, &t_dcblock, env->global_nspc, dcblock_ctor, dcblock_dtor))
  env->class_def->doc = "A simple DC block filter";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_delay* osc;
  char is_init;
  m_float     time;
} SP_delay;

static m_bool delay_tick(UGen u)
{
  SP_delay* ug = (SP_delay*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  base_tick(u);
  sp_delay_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void delay_ctor(M_Object obj, VM_Shred shred)
{
  SP_delay* ug = malloc(sizeof(SP_delay));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_delay_create(&ug->osc);
  obj->ugen->tick = delay_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void delay_dtor(M_Object obj, VM_Shred shred)
{
  SP_delay* ug = (SP_delay*)obj->ugen->ug;
  if(ug->is_init)
    sp_delay_destroy(&ug->osc);
  free(ug);
}

static void delay_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_delay* ug = (SP_delay*)obj->ugen->ug;
  if(ug->is_init)
  delay_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    time   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_delay_init(ug->sp, ug->osc, time / (m_float)ug->sp->sr);
}

static void delay_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_delay* ug = (SP_delay*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    time   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_delay_init(ug->sp, ug->osc, time / (m_float)ug->sp->sr);
}

static void delay_get_feedback(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_delay* ug = (SP_delay*)obj->ugen->ug;
  RETURN->v_float = ug->osc->feedback;
}

static void delay_set_feedback(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_delay* ug = (SP_delay*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->feedback = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_delay = { "Delay", sizeof(m_uint), &t_ugen };

static m_bool import_delay(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_delay))
  CHECK_BB(import_class_begin(env, &t_delay, env->global_nspc, delay_ctor, delay_dtor))
  env->class_def->doc = "Adds a delay to an incoming signal with optional feedback.";

  fun = new_DL_Func("void", "init", (m_uint)delay_init);
   dl_func_add_arg(fun, "dur", "time");
  f->doc = "Delay time, in seconds.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)delay_init_array);
   dl_func_add_arg(fun, "dur", "time");
  f->doc = "Delay time, in seconds.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "feedback", (m_uint)delay_get_feedback);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Feedback amount. Should be a value between 0-1.";

  fun = new_DL_Func("float", "feedback", (m_uint)delay_set_feedback);
    dl_func_add_arg(fun, "float", "feedback");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Feedback amount. Should be a value between 0-1.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_diskin* osc;
  char is_init;
  m_str     filename;
} SP_diskin;

static m_bool diskin_tick(UGen u)
{
  SP_diskin* ug = (SP_diskin*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  sp_diskin_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void diskin_ctor(M_Object obj, VM_Shred shred)
{
  SP_diskin* ug = malloc(sizeof(SP_diskin));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_diskin_create(&ug->osc);
  obj->ugen->tick = diskin_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void diskin_dtor(M_Object obj, VM_Shred shred)
{
  SP_diskin* ug = (SP_diskin*)obj->ugen->ug;
  if(ug->is_init)
    sp_diskin_destroy(&ug->osc);
  free(ug);
}

static void diskin_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_diskin* ug = (SP_diskin*)obj->ugen->ug;
  if(ug->is_init)
  diskin_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object filename_obj   = *(M_Object*)(shred->mem + offset);
  m_str    filename_str   = STRING(obj);
  offset += SZ_INT;
  sp_diskin_init(ug->sp, ug->osc, filename_str);
}

static void diskin_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_diskin* ug = (SP_diskin*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object filename_obj   = *(M_Object*)(shred->mem + offset);
  m_str    filename_str   = STRING(obj);
  offset += SZ_INT;
  sp_diskin_init(ug->sp, ug->osc, filename_str);
}

struct Type_ t_diskin = { "Diskin", sizeof(m_uint), &t_ugen };

static m_bool import_diskin(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_diskin))
  CHECK_BB(import_class_begin(env, &t_diskin, env->global_nspc, diskin_ctor, diskin_dtor))
  env->class_def->doc = "Read from an audio file    Expects a 1-channel file matching the project samplerate. Diskin should be able to read any file format that libsndfile supports.";

  fun = new_DL_Func("void", "init", (m_uint)diskin_init);
   dl_func_add_arg(fun, "string", "filename");
  f->doc = "Filename of the audio file.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)diskin_init_array);
   dl_func_add_arg(fun, "string", "filename");
  f->doc = "Filename of the audio file.";

  CHECK_BB((f = import_mfun(env, fun)))

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_dist* osc;
} SP_dist;

static m_bool dist_tick(UGen u)
{
  SP_dist* ug = (SP_dist*)u->ug;
  base_tick(u);
  sp_dist_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void dist_ctor(M_Object obj, VM_Shred shred)
{
  SP_dist* ug = malloc(sizeof(SP_dist));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_dist_create(&ug->osc);
  sp_dist_init(ug->sp, ug->osc);
  obj->ugen->tick = dist_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void dist_dtor(M_Object obj, VM_Shred shred)
{
  SP_dist* ug = (SP_dist*)obj->ugen->ug;
  sp_dist_destroy(&ug->osc);
  free(ug);
}

static void dist_get_pregain(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dist* ug = (SP_dist*)obj->ugen->ug;
  RETURN->v_float = ug->osc->pregain;
}

static void dist_set_pregain(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dist* ug = (SP_dist*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->pregain = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void dist_get_postgain(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dist* ug = (SP_dist*)obj->ugen->ug;
  RETURN->v_float = ug->osc->postgain;
}

static void dist_set_postgain(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dist* ug = (SP_dist*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->postgain = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void dist_get_shape1(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dist* ug = (SP_dist*)obj->ugen->ug;
  RETURN->v_float = ug->osc->shape1;
}

static void dist_set_shape1(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dist* ug = (SP_dist*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->shape1 = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void dist_get_shape2(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dist* ug = (SP_dist*)obj->ugen->ug;
  RETURN->v_float = ug->osc->shape2;
}

static void dist_set_shape2(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dist* ug = (SP_dist*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->shape2 = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_dist = { "Dist", sizeof(m_uint), &t_ugen };

static m_bool import_dist(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_dist))
  CHECK_BB(import_class_begin(env, &t_dist, env->global_nspc, dist_ctor, dist_dtor))
  env->class_def->doc = "Distortion using a modified hyperbolic tangent function";

  fun = new_DL_Func("float", "pregain", (m_uint)dist_get_pregain);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Gain applied before waveshaping.";

  fun = new_DL_Func("float", "pregain", (m_uint)dist_set_pregain);
    dl_func_add_arg(fun, "float", "pregain");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Gain applied before waveshaping.";

  fun = new_DL_Func("float", "postgain", (m_uint)dist_get_postgain);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Gain applied after waveshaping";

  fun = new_DL_Func("float", "postgain", (m_uint)dist_set_postgain);
    dl_func_add_arg(fun, "float", "postgain");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Gain applied after waveshaping";

  fun = new_DL_Func("float", "shape1", (m_uint)dist_get_shape1);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Shape of the positive part of the signal. A value of 0 gets a flat clip.";

  fun = new_DL_Func("float", "shape1", (m_uint)dist_set_shape1);
    dl_func_add_arg(fun, "float", "shape1");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Shape of the positive part of the signal. A value of 0 gets a flat clip.";

  fun = new_DL_Func("float", "shape2", (m_uint)dist_get_shape2);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Like shape1, only for the negative part of the signal.";

  fun = new_DL_Func("float", "shape2", (m_uint)dist_set_shape2);
    dl_func_add_arg(fun, "float", "shape2");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Like shape1, only for the negative part of the signal.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_dmetro* osc;
} SP_dmetro;

static m_bool dmetro_tick(UGen u)
{
  SP_dmetro* ug = (SP_dmetro*)u->ug;
  sp_dmetro_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void dmetro_ctor(M_Object obj, VM_Shred shred)
{
  SP_dmetro* ug = malloc(sizeof(SP_dmetro));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_dmetro_create(&ug->osc);
  sp_dmetro_init(ug->sp, ug->osc);
  obj->ugen->tick = dmetro_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void dmetro_dtor(M_Object obj, VM_Shred shred)
{
  SP_dmetro* ug = (SP_dmetro*)obj->ugen->ug;
  sp_dmetro_destroy(&ug->osc);
  free(ug);
}

static void dmetro_get_time(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dmetro* ug = (SP_dmetro*)obj->ugen->ug;
  RETURN->v_float = ug->osc->time * ug->sp->sr;
}

static void dmetro_set_time(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dmetro* ug = (SP_dmetro*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->time = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

struct Type_ t_dmetro = { "Dmetro", sizeof(m_uint), &t_ugen };

static m_bool import_dmetro(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_dmetro))
  CHECK_BB(import_class_begin(env, &t_dmetro, env->global_nspc, dmetro_ctor, dmetro_dtor))
  env->class_def->doc = "Delta Metro    Produce a set of triggers spaced apart by time.";

  fun = new_DL_Func("dur", "time", (m_uint)dmetro_get_time);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Time between triggers (in seconds). This will update at the start of each trigger.";

  fun = new_DL_Func("dur", "time", (m_uint)dmetro_set_time);
    dl_func_add_arg(fun, "dur", "time");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Time between triggers (in seconds). This will update at the start of each trigger.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_drip* osc;
  char is_init;
  m_float   dettack;
} SP_drip;

static m_bool drip_tick(UGen u)
{
  SP_drip* ug = (SP_drip*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  ugen_compute(u->trig->ugen);
  sp_drip_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void drip_ctor(M_Object obj, VM_Shred shred)
{
  SP_drip* ug = malloc(sizeof(SP_drip));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_drip_create(&ug->osc);
  obj->ugen->tick = drip_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void drip_dtor(M_Object obj, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  if(ug->is_init)
    sp_drip_destroy(&ug->osc);
  free(ug);
}

static void drip_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  if(ug->is_init)
  drip_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    dettack   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_drip_init(ug->sp, ug->osc, (ug->dettack = dettack));
}

static void drip_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    dettack   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_drip_init(ug->sp, ug->osc, (ug->dettack = dettack));
}

static void drip_get_num_tubes(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  RETURN->v_float = ug->osc->num_tubes;
}

static void drip_set_num_tubes(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->num_tubes = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void drip_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  RETURN->v_float = ug->osc->amp;
}

static void drip_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->amp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void drip_get_damp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  RETURN->v_float = ug->osc->damp;
}

static void drip_set_damp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->damp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void drip_get_shake_max(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  RETURN->v_float = ug->osc->shake_max;
}

static void drip_set_shake_max(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->shake_max = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void drip_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void drip_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void drip_get_freq1(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq1;
}

static void drip_set_freq1(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq1 = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void drip_get_freq2(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq2;
}

static void drip_set_freq2(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_drip* ug = (SP_drip*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq2 = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_drip = { "Drip", sizeof(m_uint), &t_ugen };

static m_bool import_drip(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_drip))
  CHECK_BB(import_class_begin(env, &t_drip, env->global_nspc, drip_ctor, drip_dtor))
  env->class_def->doc = "Water drop physical model    Physical model of the sound of dripping water. When triggered, it will produce a droplet of water.";

  fun = new_DL_Func("void", "init", (m_uint)drip_init);
   dl_func_add_arg(fun, "float", "dettack");
  f->doc = "Period of time over which all sound is stopped.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)drip_init_array);
   dl_func_add_arg(fun, "float", "dettack");
  f->doc = "Period of time over which all sound is stopped.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "num_tubes", (m_uint)drip_get_num_tubes);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Number of units.";

  fun = new_DL_Func("float", "num_tubes", (m_uint)drip_set_num_tubes);
    dl_func_add_arg(fun, "float", "num_tubes");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Number of units.";

  fun = new_DL_Func("float", "amp", (m_uint)drip_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude.";

  fun = new_DL_Func("float", "amp", (m_uint)drip_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude.";

  fun = new_DL_Func("float", "damp", (m_uint)drip_get_damp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The damping factor. Maximum value is 2.0.";

  fun = new_DL_Func("float", "damp", (m_uint)drip_set_damp);
    dl_func_add_arg(fun, "float", "damp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The damping factor. Maximum value is 2.0.";

  fun = new_DL_Func("float", "shake_max", (m_uint)drip_get_shake_max);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The amount of energy to add back into the system.";

  fun = new_DL_Func("float", "shake_max", (m_uint)drip_set_shake_max);
    dl_func_add_arg(fun, "float", "shake_max");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The amount of energy to add back into the system.";

  fun = new_DL_Func("float", "freq", (m_uint)drip_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Main resonant frequency.";

  fun = new_DL_Func("float", "freq", (m_uint)drip_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Main resonant frequency.";

  fun = new_DL_Func("float", "freq1", (m_uint)drip_get_freq1);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The first resonant frequency.";

  fun = new_DL_Func("float", "freq1", (m_uint)drip_set_freq1);
    dl_func_add_arg(fun, "float", "freq1");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The first resonant frequency.";

  fun = new_DL_Func("float", "freq2", (m_uint)drip_get_freq2);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The second resonant frequency.";

  fun = new_DL_Func("float", "freq2", (m_uint)drip_set_freq2);
    dl_func_add_arg(fun, "float", "freq2");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The second resonant frequency.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_dtrig* osc;
  char is_init;
  sp_ftbl*  ft;
} SP_dtrig;

static m_bool dtrig_tick(UGen u)
{
  SP_dtrig* ug = (SP_dtrig*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  ugen_compute(u->trig->ugen);
  sp_dtrig_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void dtrig_ctor(M_Object obj, VM_Shred shred)
{
  SP_dtrig* ug = malloc(sizeof(SP_dtrig));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_dtrig_create(&ug->osc);
  obj->ugen->tick = dtrig_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void dtrig_dtor(M_Object obj, VM_Shred shred)
{
  SP_dtrig* ug = (SP_dtrig*)obj->ugen->ug;
  if(ug->is_init)
    sp_dtrig_destroy(&ug->osc);
  if(ug->is_init)
    sp_ftbl_destroy(&ug->ft);
  free(ug);
}

static void dtrig_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dtrig* ug = (SP_dtrig*)obj->ugen->ug;
  if(ug->is_init)
  dtrig_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object ft_obj   = *(M_Object*)(shred->mem + offset);
  sp_ftbl* ft_ftbl  = FTBL(ft_obj);
  ug->ft            = ft_ftbl;
  offset += SZ_INT;
  sp_dtrig_init(ug->sp, ug->osc, ft_ftbl);
}

static void dtrig_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dtrig* ug = (SP_dtrig*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  sp_ftbl* ft_ftbl = ug->ft = to_ftbl(*(M_Object*)(shred->mem + offset), shred);
  offset += SZ_INT;
  sp_dtrig_init(ug->sp, ug->osc, ft_ftbl);
}

static void dtrig_get_loop(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dtrig* ug = (SP_dtrig*)obj->ugen->ug;
  RETURN->v_uint = ug->osc->loop;
}

static void dtrig_set_loop(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dtrig* ug = (SP_dtrig*)obj->ugen->ug;
  RETURN->v_uint = (ug->osc->loop = *(m_int*)(shred->mem + SZ_INT));
}

static void dtrig_get_delay(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dtrig* ug = (SP_dtrig*)obj->ugen->ug;
  RETURN->v_float = ug->osc->delay * ug->sp->sr;
}

static void dtrig_set_delay(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dtrig* ug = (SP_dtrig*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->delay = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void dtrig_get_scale(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dtrig* ug = (SP_dtrig*)obj->ugen->ug;
  RETURN->v_float = ug->osc->scale;
}

static void dtrig_set_scale(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dtrig* ug = (SP_dtrig*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->scale = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_dtrig = { "Dtrig", sizeof(m_uint), &t_ugen };

static m_bool import_dtrig(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_dtrig))
  CHECK_BB(import_class_begin(env, &t_dtrig, env->global_nspc, dtrig_ctor, dtrig_dtor))
  env->class_def->doc = " Delta trigger    This is able to create spaced out triggers. It is set off by a single trigger.";

  fun = new_DL_Func("void", "init", (m_uint)dtrig_init);
   dl_func_add_arg(fun, "ftbl", "ft");
  f->doc = "An ftable containing times in seconds.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)dtrig_init_array);
   dl_func_add_arg(fun, "float[]", "ft");
  f->doc = "An ftable containing times in seconds.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("int", "loop", (m_uint)dtrig_get_loop);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "When set to 1, dtrig will wrap around and loop again.";

  fun = new_DL_Func("int", "loop", (m_uint)dtrig_set_loop);
    dl_func_add_arg(fun, "int", "loop");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "When set to 1, dtrig will wrap around and loop again.";

  fun = new_DL_Func("dur", "delay", (m_uint)dtrig_get_delay);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "This sets a delay (in seconds) on the triggered output when it is initially triggered. This is useful for rhythmic sequences with rests in the beginnings.";

  fun = new_DL_Func("dur", "delay", (m_uint)dtrig_set_delay);
    dl_func_add_arg(fun, "dur", "delay");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "This sets a delay (in seconds) on the triggered output when it is initially triggered. This is useful for rhythmic sequences with rests in the beginnings.";

  fun = new_DL_Func("float", "scale", (m_uint)dtrig_get_scale);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Scales the timing signal. A scale of 1 is normal, a scale of 2 will double the duration, and a scale of 0.5 will halve it.";

  fun = new_DL_Func("float", "scale", (m_uint)dtrig_set_scale);
    dl_func_add_arg(fun, "float", "scale");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Scales the timing signal. A scale of 1 is normal, a scale of 2 will double the duration, and a scale of 0.5 will halve it.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_dust* osc;
} SP_dust;

static m_bool dust_tick(UGen u)
{
  SP_dust* ug = (SP_dust*)u->ug;
  sp_dust_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void dust_ctor(M_Object obj, VM_Shred shred)
{
  SP_dust* ug = malloc(sizeof(SP_dust));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_dust_create(&ug->osc);
  sp_dust_init(ug->sp, ug->osc);
  obj->ugen->tick = dust_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void dust_dtor(M_Object obj, VM_Shred shred)
{
  SP_dust* ug = (SP_dust*)obj->ugen->ug;
  sp_dust_destroy(&ug->osc);
  free(ug);
}

static void dust_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dust* ug = (SP_dust*)obj->ugen->ug;
  RETURN->v_float = ug->osc->amp;
}

static void dust_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dust* ug = (SP_dust*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->amp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void dust_get_density(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dust* ug = (SP_dust*)obj->ugen->ug;
  RETURN->v_float = ug->osc->density;
}

static void dust_set_density(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dust* ug = (SP_dust*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->density = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void dust_get_bipolar(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dust* ug = (SP_dust*)obj->ugen->ug;
  RETURN->v_uint = ug->osc->bipolar;
}

static void dust_set_bipolar(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_dust* ug = (SP_dust*)obj->ugen->ug;
  RETURN->v_uint = (ug->osc->bipolar = *(m_int*)(shred->mem + SZ_INT));
}

struct Type_ t_dust = { "Dust", sizeof(m_uint), &t_ugen };

static m_bool import_dust(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_dust))
  CHECK_BB(import_class_begin(env, &t_dust, env->global_nspc, dust_ctor, dust_dtor))
  env->class_def->doc = "A series of random impulses";

  fun = new_DL_Func("float", "amp", (m_uint)dust_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "amplitude";

  fun = new_DL_Func("float", "amp", (m_uint)dust_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "amplitude";

  fun = new_DL_Func("float", "density", (m_uint)dust_get_density);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "dust density";

  fun = new_DL_Func("float", "density", (m_uint)dust_set_density);
    dl_func_add_arg(fun, "float", "density");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "dust density";

  fun = new_DL_Func("int", "bipolar", (m_uint)dust_get_bipolar);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Bipolar flag. A non-zero makes the signal bipolar as opposed to unipolar. ";

  fun = new_DL_Func("int", "bipolar", (m_uint)dust_set_bipolar);
    dl_func_add_arg(fun, "int", "bipolar");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Bipolar flag. A non-zero makes the signal bipolar as opposed to unipolar. ";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_eqfil* osc;
} SP_eqfil;

static m_bool eqfil_tick(UGen u)
{
  SP_eqfil* ug = (SP_eqfil*)u->ug;
  base_tick(u);
  sp_eqfil_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void eqfil_ctor(M_Object obj, VM_Shred shred)
{
  SP_eqfil* ug = malloc(sizeof(SP_eqfil));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_eqfil_create(&ug->osc);
  sp_eqfil_init(ug->sp, ug->osc);
  obj->ugen->tick = eqfil_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void eqfil_dtor(M_Object obj, VM_Shred shred)
{
  SP_eqfil* ug = (SP_eqfil*)obj->ugen->ug;
  sp_eqfil_destroy(&ug->osc);
  free(ug);
}

static void eqfil_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_eqfil* ug = (SP_eqfil*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void eqfil_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_eqfil* ug = (SP_eqfil*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void eqfil_get_bw(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_eqfil* ug = (SP_eqfil*)obj->ugen->ug;
  RETURN->v_float = ug->osc->bw;
}

static void eqfil_set_bw(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_eqfil* ug = (SP_eqfil*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->bw = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void eqfil_get_gain(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_eqfil* ug = (SP_eqfil*)obj->ugen->ug;
  RETURN->v_float = ug->osc->gain;
}

static void eqfil_set_gain(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_eqfil* ug = (SP_eqfil*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->gain = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_eqfil = { "Eqfil", sizeof(m_uint), &t_ugen };

static m_bool import_eqfil(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_eqfil))
  CHECK_BB(import_class_begin(env, &t_eqfil, env->global_nspc, eqfil_ctor, eqfil_dtor))
  env->class_def->doc = "2nd order tunable equalization filter    This provides a peak/notch filter for building parametric/graphic equalizers. With gain above 1, there will be a peak at the center frequency with a width dependent on bw. If gain is less than 1, a notch is formed around the center frequency (freq).    ";

  fun = new_DL_Func("float", "freq", (m_uint)eqfil_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The center frequency of the filter";

  fun = new_DL_Func("float", "freq", (m_uint)eqfil_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The center frequency of the filter";

  fun = new_DL_Func("float", "bw", (m_uint)eqfil_get_bw);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The peak/notch bandwidth in Hertz";

  fun = new_DL_Func("float", "bw", (m_uint)eqfil_set_bw);
    dl_func_add_arg(fun, "float", "bw");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The peak/notch bandwidth in Hertz";

  fun = new_DL_Func("float", "gain", (m_uint)eqfil_get_gain);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The peak/notch gain";

  fun = new_DL_Func("float", "gain", (m_uint)eqfil_set_gain);
    dl_func_add_arg(fun, "float", "gain");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The peak/notch gain";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_expon* osc;
} SP_expon;

static m_bool expon_tick(UGen u)
{
  SP_expon* ug = (SP_expon*)u->ug;
  ugen_compute(u->trig->ugen);
  sp_expon_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void expon_ctor(M_Object obj, VM_Shred shred)
{
  SP_expon* ug = malloc(sizeof(SP_expon));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_expon_create(&ug->osc);
  sp_expon_init(ug->sp, ug->osc);
  obj->ugen->tick = expon_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void expon_dtor(M_Object obj, VM_Shred shred)
{
  SP_expon* ug = (SP_expon*)obj->ugen->ug;
  sp_expon_destroy(&ug->osc);
  free(ug);
}

static void expon_get_a(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_expon* ug = (SP_expon*)obj->ugen->ug;
  RETURN->v_float = ug->osc->a;
}

static void expon_set_a(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_expon* ug = (SP_expon*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->a = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void expon_get_dur(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_expon* ug = (SP_expon*)obj->ugen->ug;
  RETURN->v_float = ug->osc->dur * ug->sp->sr;
}

static void expon_set_dur(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_expon* ug = (SP_expon*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->dur = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void expon_get_b(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_expon* ug = (SP_expon*)obj->ugen->ug;
  RETURN->v_float = ug->osc->b;
}

static void expon_set_b(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_expon* ug = (SP_expon*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->b = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_expon = { "Expon", sizeof(m_uint), &t_ugen };

static m_bool import_expon(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_expon))
  CHECK_BB(import_class_begin(env, &t_expon, env->global_nspc, expon_ctor, expon_dtor))
  env->class_def->doc = "Produce a line segment with exponential slopeThis will generate a line from value A to value B in given amount of time. When it reaches it's target, it will stay at that value. ";

  fun = new_DL_Func("float", "a", (m_uint)expon_get_a);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Inital point.";

  fun = new_DL_Func("float", "a", (m_uint)expon_set_a);
    dl_func_add_arg(fun, "float", "a");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Inital point.";

  fun = new_DL_Func("dur", "dur", (m_uint)expon_get_dur);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Duration (in seconds)";

  fun = new_DL_Func("dur", "dur", (m_uint)expon_set_dur);
    dl_func_add_arg(fun, "dur", "dur");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Duration (in seconds)";

  fun = new_DL_Func("float", "b", (m_uint)expon_get_b);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "End point";

  fun = new_DL_Func("float", "b", (m_uint)expon_set_b);
    dl_func_add_arg(fun, "float", "b");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "End point";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_fofilt* osc;
} SP_fofilt;

static m_bool fofilt_tick(UGen u)
{
  SP_fofilt* ug = (SP_fofilt*)u->ug;
  base_tick(u);
  sp_fofilt_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void fofilt_ctor(M_Object obj, VM_Shred shred)
{
  SP_fofilt* ug = malloc(sizeof(SP_fofilt));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_fofilt_create(&ug->osc);
  sp_fofilt_init(ug->sp, ug->osc);
  obj->ugen->tick = fofilt_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void fofilt_dtor(M_Object obj, VM_Shred shred)
{
  SP_fofilt* ug = (SP_fofilt*)obj->ugen->ug;
  sp_fofilt_destroy(&ug->osc);
  free(ug);
}

static void fofilt_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fofilt* ug = (SP_fofilt*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void fofilt_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fofilt* ug = (SP_fofilt*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void fofilt_get_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fofilt* ug = (SP_fofilt*)obj->ugen->ug;
  RETURN->v_float = ug->osc->atk * ug->sp->sr;
}

static void fofilt_set_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fofilt* ug = (SP_fofilt*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->atk = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void fofilt_get_dec(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fofilt* ug = (SP_fofilt*)obj->ugen->ug;
  RETURN->v_float = ug->osc->dec * ug->sp->sr;
}

static void fofilt_set_dec(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fofilt* ug = (SP_fofilt*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->dec = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

struct Type_ t_fofilt = { "Fofilt", sizeof(m_uint), &t_ugen };

static m_bool import_fofilt(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_fofilt))
  CHECK_BB(import_class_begin(env, &t_fofilt, env->global_nspc, fofilt_ctor, fofilt_dtor))
  env->class_def->doc = "Formant filter    When fed with a pulse train, it will generate a series of overlapping grains. Overlapping will occur when 1/freq < dec, but there is no upper limit on the number of overlaps. (cited from www.csounds.com/manual/html/fofilter.html)";

  fun = new_DL_Func("float", "freq", (m_uint)fofilt_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Center frequency.";

  fun = new_DL_Func("float", "freq", (m_uint)fofilt_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Center frequency.";

  fun = new_DL_Func("dur", "atk", (m_uint)fofilt_get_atk);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Impulse response attack time (in seconds).";

  fun = new_DL_Func("dur", "atk", (m_uint)fofilt_set_atk);
    dl_func_add_arg(fun, "dur", "atk");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Impulse response attack time (in seconds).";

  fun = new_DL_Func("dur", "dec", (m_uint)fofilt_get_dec);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Impulse reponse decay time (in seconds)";

  fun = new_DL_Func("dur", "dec", (m_uint)fofilt_set_dec);
    dl_func_add_arg(fun, "dur", "dec");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Impulse reponse decay time (in seconds)";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_fof* osc;
  char is_init;
  sp_ftbl*  sine;
  sp_ftbl*  win;
  m_int     iolaps;
  m_float   iphs;
} SP_fof;

static m_bool fof_tick(UGen u)
{
  SP_fof* ug = (SP_fof*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  sp_fof_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void fof_ctor(M_Object obj, VM_Shred shred)
{
  SP_fof* ug = malloc(sizeof(SP_fof));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_fof_create(&ug->osc);
  obj->ugen->tick = fof_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void fof_dtor(M_Object obj, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  if(ug->is_init)
    sp_fof_destroy(&ug->osc);
  if(ug->is_init)
    sp_ftbl_destroy(&ug->sine);
  if(ug->is_init)
    sp_ftbl_destroy(&ug->win);
  free(ug);
}

static void fof_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  if(ug->is_init)
  fof_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object sine_obj   = *(M_Object*)(shred->mem + offset);
  sp_ftbl* sine_ftbl  = FTBL(sine_obj);
  ug->sine            = sine_ftbl;
  offset += SZ_INT;
  M_Object win_obj   = *(M_Object*)(shred->mem + offset);
  sp_ftbl* win_ftbl  = FTBL(win_obj);
  ug->win            = win_ftbl;
  offset += SZ_INT;
  m_float    iphs   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_fof_init(ug->sp, ug->osc, sine_ftbl, win_ftbl, *(m_int*)(shred->mem + SZ_INT*3), (ug->iphs = iphs));
  shred->reg  -= SZ_INT*3;
}

static void fof_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  sp_ftbl* sine_ftbl = ug->sine = to_ftbl(*(M_Object*)(shred->mem + offset), shred);
  offset += SZ_INT;
  sp_ftbl* win_ftbl = ug->win = to_ftbl(*(M_Object*)(shred->mem + offset), shred);
  offset += SZ_INT;
  m_float    iphs   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_fof_init(ug->sp, ug->osc, sine_ftbl, win_ftbl, *(m_int*)(shred->mem + SZ_INT*3), (ug->iphs = iphs));
}

static void fof_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  RETURN->v_float = ug->osc->amp;
}

static void fof_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->amp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void fof_get_fund(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  RETURN->v_float = ug->osc->fund;
}

static void fof_set_fund(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->fund = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void fof_get_form(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  RETURN->v_float = ug->osc->form;
}

static void fof_set_form(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->form = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void fof_get_oct(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  RETURN->v_float = ug->osc->oct;
}

static void fof_set_oct(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->oct = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void fof_get_band(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  RETURN->v_float = ug->osc->band;
}

static void fof_set_band(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->band = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void fof_get_ris(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  RETURN->v_float = ug->osc->ris * ug->sp->sr;
}

static void fof_set_ris(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->ris = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void fof_get_dec(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  RETURN->v_float = ug->osc->dec * ug->sp->sr;
}

static void fof_set_dec(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->dec = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void fof_get_dur(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  RETURN->v_float = ug->osc->dur * ug->sp->sr;
}

static void fof_set_dur(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fof* ug = (SP_fof*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->dur = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

struct Type_ t_fof = { "Fof", sizeof(m_uint), &t_ugen };

static m_bool import_fof(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_fof))
  CHECK_BB(import_class_begin(env, &t_fof, env->global_nspc, fof_ctor, fof_dtor))
  env->class_def->doc = "Produces sinusoid bursts for granular and formant synthesis";

  fun = new_DL_Func("void", "init", (m_uint)fof_init);
   dl_func_add_arg(fun, "ftbl", "sine");
  f->doc = "ftable for sine wave.";

   dl_func_add_arg(fun, "ftbl", "win");
  f->doc = "Ftable for envelope function (use either gen_line or gen_sinecomp)";

   dl_func_add_arg(fun, "int", "iolaps");
  f->doc = "Maximum number of foflet overlaps.";

   dl_func_add_arg(fun, "float", "iphs");
  f->doc = "Phase";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)fof_init_array);
   dl_func_add_arg(fun, "float[]", "sine");
  f->doc = "ftable for sine wave.";

   dl_func_add_arg(fun, "float[]", "win");
  f->doc = "Ftable for envelope function (use either gen_line or gen_sinecomp)";

   dl_func_add_arg(fun, "int", "iolaps");
  f->doc = "Maximum number of foflet overlaps.";

   dl_func_add_arg(fun, "float", "iphs");
  f->doc = "Phase";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "amp", (m_uint)fof_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Overall amplitude";

  fun = new_DL_Func("float", "amp", (m_uint)fof_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Overall amplitude";

  fun = new_DL_Func("float", "fund", (m_uint)fof_get_fund);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Fundamental frequency";

  fun = new_DL_Func("float", "fund", (m_uint)fof_set_fund);
    dl_func_add_arg(fun, "float", "fund");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Fundamental frequency";

  fun = new_DL_Func("float", "form", (m_uint)fof_get_form);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Formant frequency.";

  fun = new_DL_Func("float", "form", (m_uint)fof_set_form);
    dl_func_add_arg(fun, "float", "form");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Formant frequency.";

  fun = new_DL_Func("float", "oct", (m_uint)fof_get_oct);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Octaviation index, if greater than zero, lowers the effective fund frequency by attenuating odd-numbered sine bursts. whole numbers are full octaves. fractions transpositional.";

  fun = new_DL_Func("float", "oct", (m_uint)fof_set_oct);
    dl_func_add_arg(fun, "float", "oct");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Octaviation index, if greater than zero, lowers the effective fund frequency by attenuating odd-numbered sine bursts. whole numbers are full octaves. fractions transpositional.";

  fun = new_DL_Func("float", "band", (m_uint)fof_get_band);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Bandwidth (in -6db) expressed in Hz. The bandwidth determines the rate of exponential decay throughout the sineburst, before the enveloping is applied.";

  fun = new_DL_Func("float", "band", (m_uint)fof_set_band);
    dl_func_add_arg(fun, "float", "band");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Bandwidth (in -6db) expressed in Hz. The bandwidth determines the rate of exponential decay throughout the sineburst, before the enveloping is applied.";

  fun = new_DL_Func("dur", "ris", (m_uint)fof_get_ris);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Rise of sinusoid burst (in seconds)";

  fun = new_DL_Func("dur", "ris", (m_uint)fof_set_ris);
    dl_func_add_arg(fun, "dur", "ris");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Rise of sinusoid burst (in seconds)";

  fun = new_DL_Func("dur", "dec", (m_uint)fof_get_dec);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Decay of the sinusoid burst (in seconds).";

  fun = new_DL_Func("dur", "dec", (m_uint)fof_set_dec);
    dl_func_add_arg(fun, "dur", "dec");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Decay of the sinusoid burst (in seconds).";

  fun = new_DL_Func("dur", "dur", (m_uint)fof_get_dur);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "OVerall duration of sinusoid burst (in seconds).";

  fun = new_DL_Func("dur", "dur", (m_uint)fof_set_dur);
    dl_func_add_arg(fun, "dur", "dur");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "OVerall duration of sinusoid burst (in seconds).";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_fog* osc;
  char is_init;
  sp_ftbl*  wav;
  sp_ftbl*  win;
  m_int     iolaps;
  m_float   iphs;
} SP_fog;

static m_bool fog_tick(UGen u)
{
  SP_fog* ug = (SP_fog*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  sp_fog_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void fog_ctor(M_Object obj, VM_Shred shred)
{
  SP_fog* ug = malloc(sizeof(SP_fog));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_fog_create(&ug->osc);
  obj->ugen->tick = fog_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void fog_dtor(M_Object obj, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  if(ug->is_init)
    sp_fog_destroy(&ug->osc);
  if(ug->is_init)
    sp_ftbl_destroy(&ug->wav);
  if(ug->is_init)
    sp_ftbl_destroy(&ug->win);
  free(ug);
}

static void fog_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  if(ug->is_init)
  fog_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object wav_obj   = *(M_Object*)(shred->mem + offset);
  sp_ftbl* wav_ftbl  = FTBL(wav_obj);
  ug->wav            = wav_ftbl;
  offset += SZ_INT;
  M_Object win_obj   = *(M_Object*)(shred->mem + offset);
  sp_ftbl* win_ftbl  = FTBL(win_obj);
  ug->win            = win_ftbl;
  offset += SZ_INT;
  m_float    iphs   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_fog_init(ug->sp, ug->osc, wav_ftbl, win_ftbl, *(m_int*)(shred->mem + SZ_INT*3), (ug->iphs = iphs));
  shred->reg  -= SZ_INT*3;
}

static void fog_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  sp_ftbl* wav_ftbl = ug->wav = to_ftbl(*(M_Object*)(shred->mem + offset), shred);
  offset += SZ_INT;
  sp_ftbl* win_ftbl = ug->win = to_ftbl(*(M_Object*)(shred->mem + offset), shred);
  offset += SZ_INT;
  m_float    iphs   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_fog_init(ug->sp, ug->osc, wav_ftbl, win_ftbl, *(m_int*)(shred->mem + SZ_INT*3), (ug->iphs = iphs));
}

static void fog_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = ug->osc->amp;
}

static void fog_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->amp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void fog_get_dens(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = ug->osc->dens;
}

static void fog_set_dens(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->dens = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void fog_get_trans(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = ug->osc->trans;
}

static void fog_set_trans(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->trans = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void fog_get_spd(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = ug->osc->spd;
}

static void fog_set_spd(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->spd = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void fog_get_oct(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = ug->osc->oct;
}

static void fog_set_oct(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->oct = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void fog_get_band(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = ug->osc->band;
}

static void fog_set_band(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->band = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void fog_get_ris(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = ug->osc->ris * ug->sp->sr;
}

static void fog_set_ris(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->ris = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void fog_get_dec(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = ug->osc->dec * ug->sp->sr;
}

static void fog_set_dec(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->dec = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void fog_get_dur(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = ug->osc->dur * ug->sp->sr;
}

static void fog_set_dur(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fog* ug = (SP_fog*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->dur = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

struct Type_ t_fog = { "Fog", sizeof(m_uint), &t_ugen };

static m_bool import_fog(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_fog))
  CHECK_BB(import_class_begin(env, &t_fog, env->global_nspc, fog_ctor, fog_dtor))
  env->class_def->doc = "Sucession of grains from data in a stored function table";

  fun = new_DL_Func("void", "init", (m_uint)fog_init);
   dl_func_add_arg(fun, "ftbl", "wav");
  f->doc = "ftable for sample.";

   dl_func_add_arg(fun, "ftbl", "win");
  f->doc = "Ftable for envelope function (use either gen_line or gen_sinecomp)";

   dl_func_add_arg(fun, "int", "iolaps");
  f->doc = "Maximum number of foglet overlaps.";

   dl_func_add_arg(fun, "float", "iphs");
  f->doc = "Phase";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)fog_init_array);
   dl_func_add_arg(fun, "float[]", "wav");
  f->doc = "ftable for sample.";

   dl_func_add_arg(fun, "float[]", "win");
  f->doc = "Ftable for envelope function (use either gen_line or gen_sinecomp)";

   dl_func_add_arg(fun, "int", "iolaps");
  f->doc = "Maximum number of foglet overlaps.";

   dl_func_add_arg(fun, "float", "iphs");
  f->doc = "Phase";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "amp", (m_uint)fog_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Overall amplitude";

  fun = new_DL_Func("float", "amp", (m_uint)fog_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Overall amplitude";

  fun = new_DL_Func("float", "dens", (m_uint)fog_get_dens);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Density. The frequency of grains/second.";

  fun = new_DL_Func("float", "dens", (m_uint)fog_set_dens);
    dl_func_add_arg(fun, "float", "dens");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Density. The frequency of grains/second.";

  fun = new_DL_Func("float", "trans", (m_uint)fog_get_trans);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Transposition, in terms of playback speed";

  fun = new_DL_Func("float", "trans", (m_uint)fog_set_trans);
    dl_func_add_arg(fun, "float", "trans");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Transposition, in terms of playback speed";

  fun = new_DL_Func("float", "spd", (m_uint)fog_get_spd);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Starting sample index, normalized 0-1.";

  fun = new_DL_Func("float", "spd", (m_uint)fog_set_spd);
    dl_func_add_arg(fun, "float", "spd");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Starting sample index, normalized 0-1.";

  fun = new_DL_Func("float", "oct", (m_uint)fog_get_oct);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Octaviation index, if greater than zero, lowers the effective fund frequency by attenuating odd-numbered sine bursts. whole numbers are full octaves. fractions transpositional.";

  fun = new_DL_Func("float", "oct", (m_uint)fog_set_oct);
    dl_func_add_arg(fun, "float", "oct");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Octaviation index, if greater than zero, lowers the effective fund frequency by attenuating odd-numbered sine bursts. whole numbers are full octaves. fractions transpositional.";

  fun = new_DL_Func("float", "band", (m_uint)fog_get_band);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Bandwidth (in -6db) expressed in Hz. The bandwidth determines the rate of exponential decay throughout the sineburst, before the enveloping is applied.";

  fun = new_DL_Func("float", "band", (m_uint)fog_set_band);
    dl_func_add_arg(fun, "float", "band");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Bandwidth (in -6db) expressed in Hz. The bandwidth determines the rate of exponential decay throughout the sineburst, before the enveloping is applied.";

  fun = new_DL_Func("dur", "ris", (m_uint)fog_get_ris);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Rise of sinusoid burst (in seconds)";

  fun = new_DL_Func("dur", "ris", (m_uint)fog_set_ris);
    dl_func_add_arg(fun, "dur", "ris");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Rise of sinusoid burst (in seconds)";

  fun = new_DL_Func("dur", "dec", (m_uint)fog_get_dec);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Decay of the sinusoid burst (in seconds).";

  fun = new_DL_Func("dur", "dec", (m_uint)fog_set_dec);
    dl_func_add_arg(fun, "dur", "dec");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Decay of the sinusoid burst (in seconds).";

  fun = new_DL_Func("dur", "dur", (m_uint)fog_get_dur);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "OVerall duration of sinusoid burst (in seconds).";

  fun = new_DL_Func("dur", "dur", (m_uint)fog_set_dur);
    dl_func_add_arg(fun, "dur", "dur");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "OVerall duration of sinusoid burst (in seconds).";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_fold* osc;
} SP_fold;

static m_bool fold_tick(UGen u)
{
  SP_fold* ug = (SP_fold*)u->ug;
  base_tick(u);
  sp_fold_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void fold_ctor(M_Object obj, VM_Shred shred)
{
  SP_fold* ug = malloc(sizeof(SP_fold));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_fold_create(&ug->osc);
  sp_fold_init(ug->sp, ug->osc);
  obj->ugen->tick = fold_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void fold_dtor(M_Object obj, VM_Shred shred)
{
  SP_fold* ug = (SP_fold*)obj->ugen->ug;
  sp_fold_destroy(&ug->osc);
  free(ug);
}

static void fold_get_incr(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fold* ug = (SP_fold*)obj->ugen->ug;
  RETURN->v_float = ug->osc->incr;
}

static void fold_set_incr(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fold* ug = (SP_fold*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->incr = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_fold = { "Fold", sizeof(m_uint), &t_ugen };

static m_bool import_fold(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_fold))
  CHECK_BB(import_class_begin(env, &t_fold, env->global_nspc, fold_ctor, fold_dtor))
  env->class_def->doc = "Adds artificial foldover to an audio signal    Primarily created for use with Decimator.";

  fun = new_DL_Func("float", "incr", (m_uint)fold_get_incr);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Increment";

  fun = new_DL_Func("float", "incr", (m_uint)fold_set_incr);
    dl_func_add_arg(fun, "float", "incr");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Increment";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_fosc* osc;
  char is_init;
  sp_ftbl*  tbl;
} SP_fosc;

static m_bool fosc_tick(UGen u)
{
  SP_fosc* ug = (SP_fosc*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  sp_fosc_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void fosc_ctor(M_Object obj, VM_Shred shred)
{
  SP_fosc* ug = malloc(sizeof(SP_fosc));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_fosc_create(&ug->osc);
  obj->ugen->tick = fosc_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void fosc_dtor(M_Object obj, VM_Shred shred)
{
  SP_fosc* ug = (SP_fosc*)obj->ugen->ug;
  if(ug->is_init)
    sp_fosc_destroy(&ug->osc);
  if(ug->is_init)
    sp_ftbl_destroy(&ug->tbl);
  free(ug);
}

static void fosc_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fosc* ug = (SP_fosc*)obj->ugen->ug;
  if(ug->is_init)
  fosc_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object tbl_obj   = *(M_Object*)(shred->mem + offset);
  sp_ftbl* tbl_ftbl  = FTBL(tbl_obj);
  ug->tbl            = tbl_ftbl;
  offset += SZ_INT;
  sp_fosc_init(ug->sp, ug->osc, tbl_ftbl);
}

static void fosc_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fosc* ug = (SP_fosc*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  sp_ftbl* tbl_ftbl = ug->tbl = to_ftbl(*(M_Object*)(shred->mem + offset), shred);
  offset += SZ_INT;
  sp_fosc_init(ug->sp, ug->osc, tbl_ftbl);
}

static void fosc_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fosc* ug = (SP_fosc*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void fosc_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fosc* ug = (SP_fosc*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void fosc_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fosc* ug = (SP_fosc*)obj->ugen->ug;
  RETURN->v_float = ug->osc->amp;
}

static void fosc_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fosc* ug = (SP_fosc*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->amp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void fosc_get_car(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fosc* ug = (SP_fosc*)obj->ugen->ug;
  RETURN->v_float = ug->osc->car;
}

static void fosc_set_car(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fosc* ug = (SP_fosc*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->car = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void fosc_get_mod(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fosc* ug = (SP_fosc*)obj->ugen->ug;
  RETURN->v_float = ug->osc->mod;
}

static void fosc_set_mod(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fosc* ug = (SP_fosc*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->mod = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void fosc_get_indx(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fosc* ug = (SP_fosc*)obj->ugen->ug;
  RETURN->v_float = ug->osc->indx;
}

static void fosc_set_indx(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_fosc* ug = (SP_fosc*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->indx = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_fosc = { "Fosc", sizeof(m_uint), &t_ugen };

static m_bool import_fosc(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_fosc))
  CHECK_BB(import_class_begin(env, &t_fosc, env->global_nspc, fosc_ctor, fosc_dtor))
  env->class_def->doc = "FM oscilator pair with linear interpolation";

  fun = new_DL_Func("void", "init", (m_uint)fosc_init);
   dl_func_add_arg(fun, "ftbl", "tbl");
  f->doc = "Wavetable to read from. Note: the size of this table must be a power of 2.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)fosc_init_array);
   dl_func_add_arg(fun, "float[]", "tbl");
  f->doc = "Wavetable to read from. Note: the size of this table must be a power of 2.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "freq", (m_uint)fosc_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency (in Hz)";

  fun = new_DL_Func("float", "freq", (m_uint)fosc_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency (in Hz)";

  fun = new_DL_Func("float", "amp", (m_uint)fosc_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude (typically a value between 0 and 1).";

  fun = new_DL_Func("float", "amp", (m_uint)fosc_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude (typically a value between 0 and 1).";

  fun = new_DL_Func("float", "car", (m_uint)fosc_get_car);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Carrier frequency, expressed as a ratio number in C:M ratio. Typically an integer.";

  fun = new_DL_Func("float", "car", (m_uint)fosc_set_car);
    dl_func_add_arg(fun, "float", "car");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Carrier frequency, expressed as a ratio number in C:M ratio. Typically an integer.";

  fun = new_DL_Func("float", "mod", (m_uint)fosc_get_mod);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Modulator frequency, expressed as a ratio number in C:M ratio. Typically an integer.";

  fun = new_DL_Func("float", "mod", (m_uint)fosc_set_mod);
    dl_func_add_arg(fun, "float", "mod");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Modulator frequency, expressed as a ratio number in C:M ratio. Typically an integer.";

  fun = new_DL_Func("float", "indx", (m_uint)fosc_get_indx);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Modulation index. Most commercial synthesizers have a range from 0-8, but there's nothing stopping you from going beyond that.";

  fun = new_DL_Func("float", "indx", (m_uint)fosc_set_indx);
    dl_func_add_arg(fun, "float", "indx");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Modulation index. Most commercial synthesizers have a range from 0-8, but there's nothing stopping you from going beyond that.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_gbuzz* osc;
  char is_init;
  sp_ftbl*  ft;
  m_float   iphs;
} SP_gbuzz;

static m_bool gbuzz_tick(UGen u)
{
  SP_gbuzz* ug = (SP_gbuzz*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  sp_gbuzz_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void gbuzz_ctor(M_Object obj, VM_Shred shred)
{
  SP_gbuzz* ug = malloc(sizeof(SP_gbuzz));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_gbuzz_create(&ug->osc);
  obj->ugen->tick = gbuzz_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void gbuzz_dtor(M_Object obj, VM_Shred shred)
{
  SP_gbuzz* ug = (SP_gbuzz*)obj->ugen->ug;
  if(ug->is_init)
    sp_gbuzz_destroy(&ug->osc);
  if(ug->is_init)
    sp_ftbl_destroy(&ug->ft);
  free(ug);
}

static void gbuzz_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_gbuzz* ug = (SP_gbuzz*)obj->ugen->ug;
  if(ug->is_init)
  gbuzz_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object ft_obj   = *(M_Object*)(shred->mem + offset);
  sp_ftbl* ft_ftbl  = FTBL(ft_obj);
  ug->ft            = ft_ftbl;
  offset += SZ_INT;
  m_float    iphs   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_gbuzz_init(ug->sp, ug->osc, ft_ftbl, (ug->iphs = iphs));
  shred->reg  -= SZ_INT*1;
}

static void gbuzz_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_gbuzz* ug = (SP_gbuzz*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  sp_ftbl* ft_ftbl = ug->ft = to_ftbl(*(M_Object*)(shred->mem + offset), shred);
  offset += SZ_INT;
  m_float    iphs   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_gbuzz_init(ug->sp, ug->osc, ft_ftbl, (ug->iphs = iphs));
}

static void gbuzz_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_gbuzz* ug = (SP_gbuzz*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void gbuzz_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_gbuzz* ug = (SP_gbuzz*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void gbuzz_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_gbuzz* ug = (SP_gbuzz*)obj->ugen->ug;
  RETURN->v_float = ug->osc->amp;
}

static void gbuzz_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_gbuzz* ug = (SP_gbuzz*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->amp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void gbuzz_get_nharm(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_gbuzz* ug = (SP_gbuzz*)obj->ugen->ug;
  RETURN->v_float = ug->osc->nharm;
}

static void gbuzz_set_nharm(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_gbuzz* ug = (SP_gbuzz*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->nharm = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void gbuzz_get_lharm(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_gbuzz* ug = (SP_gbuzz*)obj->ugen->ug;
  RETURN->v_float = ug->osc->lharm;
}

static void gbuzz_set_lharm(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_gbuzz* ug = (SP_gbuzz*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->lharm = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void gbuzz_get_mul(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_gbuzz* ug = (SP_gbuzz*)obj->ugen->ug;
  RETURN->v_float = ug->osc->mul;
}

static void gbuzz_set_mul(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_gbuzz* ug = (SP_gbuzz*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->mul = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_gbuzz = { "Gbuzz", sizeof(m_uint), &t_ugen };

static m_bool import_gbuzz(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_gbuzz))
  CHECK_BB(import_class_begin(env, &t_gbuzz, env->global_nspc, gbuzz_ctor, gbuzz_dtor))
  env->class_def->doc = "Series of partials from the harmonic series    GBuzz comes from the \"buzz\" family of Csound opcodes, and is capable of producing a rich spectrum of harmonic content, useful for subtractive synthesis implementation.";

  fun = new_DL_Func("void", "init", (m_uint)gbuzz_init);
   dl_func_add_arg(fun, "ftbl", "ft");
  f->doc = "Soundpipe function table used internally. This should be a sine wave.";

   dl_func_add_arg(fun, "float", "iphs");
  f->doc = "Phase to start on (in the range 0-1)";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)gbuzz_init_array);
   dl_func_add_arg(fun, "float[]", "ft");
  f->doc = "Soundpipe function table used internally. This should be a sine wave.";

   dl_func_add_arg(fun, "float", "iphs");
  f->doc = "Phase to start on (in the range 0-1)";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "freq", (m_uint)gbuzz_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency, in Hertz.";

  fun = new_DL_Func("float", "freq", (m_uint)gbuzz_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency, in Hertz.";

  fun = new_DL_Func("float", "amp", (m_uint)gbuzz_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude (Typically a value between 0 and 1).";

  fun = new_DL_Func("float", "amp", (m_uint)gbuzz_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude (Typically a value between 0 and 1).";

  fun = new_DL_Func("float", "nharm", (m_uint)gbuzz_get_nharm);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Number of harmonics.";

  fun = new_DL_Func("float", "nharm", (m_uint)gbuzz_set_nharm);
    dl_func_add_arg(fun, "float", "nharm");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Number of harmonics.";

  fun = new_DL_Func("float", "lharm", (m_uint)gbuzz_get_lharm);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Lowest harmonic present. This should be a whole number integer.";

  fun = new_DL_Func("float", "lharm", (m_uint)gbuzz_set_lharm);
    dl_func_add_arg(fun, "float", "lharm");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Lowest harmonic present. This should be a whole number integer.";

  fun = new_DL_Func("float", "mul", (m_uint)gbuzz_get_mul);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Multiplier. This determines the relative strength of each harmonic.";

  fun = new_DL_Func("float", "mul", (m_uint)gbuzz_set_mul);
    dl_func_add_arg(fun, "float", "mul");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Multiplier. This determines the relative strength of each harmonic.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_hilbert* osc;
} SP_hilbert;

static m_bool hilbert_tick(UGen u)
{
  SP_hilbert* ug = (SP_hilbert*)u->ug;
  M_Object l = (M_Object)u->channel[0];
  M_Object r = (M_Object)u->channel[1];
  u->in = (l->ugen->in + r->ugen->in)/2;
  sp_hilbert_compute(ug->sp, ug->osc, &u->in, &l->ugen->out, &r->ugen->out);
}

static void hilbert_ctor(M_Object obj, VM_Shred shred)
{
  SP_hilbert* ug = malloc(sizeof(SP_hilbert));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_hilbert_create(&ug->osc);
  sp_hilbert_init(ug->sp, ug->osc);
  obj->ugen->tick = hilbert_tick;
  assign_ugen(obj->ugen, 1, 2, 0, ug);
}

static void hilbert_dtor(M_Object obj, VM_Shred shred)
{
  SP_hilbert* ug = (SP_hilbert*)obj->ugen->ug;
  sp_hilbert_destroy(&ug->osc);
  free(ug);
}

struct Type_ t_hilbert = { "Hilbert", sizeof(m_uint), &t_ugen };

static m_bool import_hilbert(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_hilbert))
  CHECK_BB(import_class_begin(env, &t_hilbert, env->global_nspc, hilbert_ctor, hilbert_dtor))
  env->class_def->doc = "Hilbert transform";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_in* osc;
} SP_in;

static m_bool in_tick(UGen u)
{
  SP_in* ug = (SP_in*)u->ug;
  sp_in_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void in_ctor(M_Object obj, VM_Shred shred)
{
  SP_in* ug = malloc(sizeof(SP_in));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_in_create(&ug->osc);
  sp_in_init(ug->sp, ug->osc);
  obj->ugen->tick = in_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void in_dtor(M_Object obj, VM_Shred shred)
{
  SP_in* ug = (SP_in*)obj->ugen->ug;
  sp_in_destroy(&ug->osc);
  free(ug);
}

struct Type_ t_in = { "In", sizeof(m_uint), &t_ugen };

static m_bool import_in(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_in))
  CHECK_BB(import_class_begin(env, &t_in, env->global_nspc, in_ctor, in_dtor))
  env->class_def->doc = "Reads from standard input.    Expects type of SPFLOAT, which by default is a float. If the input data is larger than the number of samples, you will get a complaint about a broken pipe (but it will still work). If there is no input data from STDIN, it will hang.The expected use case of sp_in is to utilize pipes from the commandline, like so:cat /dev/urandom | ./my_programAssuming my_program is using sp_in, this will write /dev/urandom (essentially white noise) to an audio file.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_jcrev* osc;
} SP_jcrev;

static m_bool jcrev_tick(UGen u)
{
  SP_jcrev* ug = (SP_jcrev*)u->ug;
  base_tick(u);
  sp_jcrev_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void jcrev_ctor(M_Object obj, VM_Shred shred)
{
  SP_jcrev* ug = malloc(sizeof(SP_jcrev));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_jcrev_create(&ug->osc);
  sp_jcrev_init(ug->sp, ug->osc);
  obj->ugen->tick = jcrev_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void jcrev_dtor(M_Object obj, VM_Shred shred)
{
  SP_jcrev* ug = (SP_jcrev*)obj->ugen->ug;
  sp_jcrev_destroy(&ug->osc);
  free(ug);
}

struct Type_ t_jcrev = { "Jcrev", sizeof(m_uint), &t_ugen };

static m_bool import_jcrev(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_jcrev))
  CHECK_BB(import_class_begin(env, &t_jcrev, env->global_nspc, jcrev_ctor, jcrev_dtor))
  env->class_def->doc = "John Chowning reverberator    This is was built using the JC reverb implentation found in FAUST. According to the source code, the specifications forthis implementation were found on an old SAIL DART backup tape.  This class is derived from the CLM JCRev function, which is based on the use of  networks of simple allpass and comb delay filters.  This class implements three series  allpass units, followed by four parallel comb filters, and two decorrelation delay lines in  parallel at the output.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_jitter* osc;
} SP_jitter;

static m_bool jitter_tick(UGen u)
{
  SP_jitter* ug = (SP_jitter*)u->ug;
  sp_jitter_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void jitter_ctor(M_Object obj, VM_Shred shred)
{
  SP_jitter* ug = malloc(sizeof(SP_jitter));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_jitter_create(&ug->osc);
  sp_jitter_init(ug->sp, ug->osc);
  obj->ugen->tick = jitter_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void jitter_dtor(M_Object obj, VM_Shred shred)
{
  SP_jitter* ug = (SP_jitter*)obj->ugen->ug;
  sp_jitter_destroy(&ug->osc);
  free(ug);
}

static void jitter_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_jitter* ug = (SP_jitter*)obj->ugen->ug;
  RETURN->v_float = ug->osc->amp;
}

static void jitter_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_jitter* ug = (SP_jitter*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->amp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void jitter_get_cpsMin(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_jitter* ug = (SP_jitter*)obj->ugen->ug;
  RETURN->v_float = ug->osc->cpsMin;
}

static void jitter_set_cpsMin(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_jitter* ug = (SP_jitter*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->cpsMin = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void jitter_get_cpsMax(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_jitter* ug = (SP_jitter*)obj->ugen->ug;
  RETURN->v_float = ug->osc->cpsMax;
}

static void jitter_set_cpsMax(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_jitter* ug = (SP_jitter*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->cpsMax = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_jitter = { "Jitter", sizeof(m_uint), &t_ugen };

static m_bool import_jitter(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_jitter))
  CHECK_BB(import_class_begin(env, &t_jitter, env->global_nspc, jitter_ctor, jitter_dtor))
  env->class_def->doc = "A signal with random fluctuations     This is useful for emulating jitter found in analogue equipment. ";

  fun = new_DL_Func("float", "amp", (m_uint)jitter_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The amplitude of the line. Will produce values in the range of (+/-)amp.";

  fun = new_DL_Func("float", "amp", (m_uint)jitter_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The amplitude of the line. Will produce values in the range of (+/-)amp.";

  fun = new_DL_Func("float", "cpsMin", (m_uint)jitter_get_cpsMin);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The minimum frequency of change in Hz.";

  fun = new_DL_Func("float", "cpsMin", (m_uint)jitter_set_cpsMin);
    dl_func_add_arg(fun, "float", "cpsMin");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The minimum frequency of change in Hz.";

  fun = new_DL_Func("float", "cpsMax", (m_uint)jitter_get_cpsMax);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The maximum frequency of change in Hz.";

  fun = new_DL_Func("float", "cpsMax", (m_uint)jitter_set_cpsMax);
    dl_func_add_arg(fun, "float", "cpsMax");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The maximum frequency of change in Hz.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_line* osc;
} SP_line;

static m_bool line_tick(UGen u)
{
  SP_line* ug = (SP_line*)u->ug;
  ugen_compute(u->trig->ugen);
  sp_line_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void line_ctor(M_Object obj, VM_Shred shred)
{
  SP_line* ug = malloc(sizeof(SP_line));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_line_create(&ug->osc);
  sp_line_init(ug->sp, ug->osc);
  obj->ugen->tick = line_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void line_dtor(M_Object obj, VM_Shred shred)
{
  SP_line* ug = (SP_line*)obj->ugen->ug;
  sp_line_destroy(&ug->osc);
  free(ug);
}

static void line_get_a(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_line* ug = (SP_line*)obj->ugen->ug;
  RETURN->v_float = ug->osc->a;
}

static void line_set_a(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_line* ug = (SP_line*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->a = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void line_get_dur(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_line* ug = (SP_line*)obj->ugen->ug;
  RETURN->v_float = ug->osc->dur * ug->sp->sr;
}

static void line_set_dur(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_line* ug = (SP_line*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->dur = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void line_get_b(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_line* ug = (SP_line*)obj->ugen->ug;
  RETURN->v_float = ug->osc->b;
}

static void line_set_b(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_line* ug = (SP_line*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->b = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_line = { "Line", sizeof(m_uint), &t_ugen };

static m_bool import_line(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_line))
  CHECK_BB(import_class_begin(env, &t_line, env->global_nspc, line_ctor, line_dtor))
  env->class_def->doc = "Produce a line segment with linear slopeThis will generate a line from value A to value B in given amount of time. When it reaches it's target, it will stay at that value. ";

  fun = new_DL_Func("float", "a", (m_uint)line_get_a);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Inital point.";

  fun = new_DL_Func("float", "a", (m_uint)line_set_a);
    dl_func_add_arg(fun, "float", "a");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Inital point.";

  fun = new_DL_Func("dur", "dur", (m_uint)line_get_dur);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Duration (in seconds)";

  fun = new_DL_Func("dur", "dur", (m_uint)line_set_dur);
    dl_func_add_arg(fun, "dur", "dur");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Duration (in seconds)";

  fun = new_DL_Func("float", "b", (m_uint)line_get_b);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "End point";

  fun = new_DL_Func("float", "b", (m_uint)line_set_b);
    dl_func_add_arg(fun, "float", "b");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "End point";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_lpf18* osc;
} SP_lpf18;

static m_bool lpf18_tick(UGen u)
{
  SP_lpf18* ug = (SP_lpf18*)u->ug;
  base_tick(u);
  sp_lpf18_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void lpf18_ctor(M_Object obj, VM_Shred shred)
{
  SP_lpf18* ug = malloc(sizeof(SP_lpf18));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_lpf18_create(&ug->osc);
  sp_lpf18_init(ug->sp, ug->osc);
  obj->ugen->tick = lpf18_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void lpf18_dtor(M_Object obj, VM_Shred shred)
{
  SP_lpf18* ug = (SP_lpf18*)obj->ugen->ug;
  sp_lpf18_destroy(&ug->osc);
  free(ug);
}

static void lpf18_get_cutoff(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_lpf18* ug = (SP_lpf18*)obj->ugen->ug;
  RETURN->v_float = ug->osc->cutoff;
}

static void lpf18_set_cutoff(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_lpf18* ug = (SP_lpf18*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->cutoff = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void lpf18_get_res(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_lpf18* ug = (SP_lpf18*)obj->ugen->ug;
  RETURN->v_float = ug->osc->res;
}

static void lpf18_set_res(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_lpf18* ug = (SP_lpf18*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->res = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void lpf18_get_dist(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_lpf18* ug = (SP_lpf18*)obj->ugen->ug;
  RETURN->v_float = ug->osc->dist;
}

static void lpf18_set_dist(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_lpf18* ug = (SP_lpf18*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->dist = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_lpf18 = { "Lpf18", sizeof(m_uint), &t_ugen };

static m_bool import_lpf18(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_lpf18))
  CHECK_BB(import_class_begin(env, &t_lpf18, env->global_nspc, lpf18_ctor, lpf18_dtor))
  env->class_def->doc = "3-pole (18 db/oct slope) Low-Pass filter with resonance and tanh distortion";

  fun = new_DL_Func("float", "cutoff", (m_uint)lpf18_get_cutoff);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Filter cutoff frequency, in Hertz";

  fun = new_DL_Func("float", "cutoff", (m_uint)lpf18_set_cutoff);
    dl_func_add_arg(fun, "float", "cutoff");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Filter cutoff frequency, in Hertz";

  fun = new_DL_Func("float", "res", (m_uint)lpf18_get_res);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Resonance. Expects a value in the range 0-1. A value of 1.0 will self oscillate at the cutoff frequency.";

  fun = new_DL_Func("float", "res", (m_uint)lpf18_set_res);
    dl_func_add_arg(fun, "float", "res");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Resonance. Expects a value in the range 0-1. A value of 1.0 will self oscillate at the cutoff frequency.";

  fun = new_DL_Func("float", "dist", (m_uint)lpf18_get_dist);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Distortion amount.";

  fun = new_DL_Func("float", "dist", (m_uint)lpf18_set_dist);
    dl_func_add_arg(fun, "float", "dist");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Distortion amount.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_maygate* osc;
} SP_maygate;

static m_bool maygate_tick(UGen u)
{
  SP_maygate* ug = (SP_maygate*)u->ug;
  ugen_compute(u->trig->ugen);
  sp_maygate_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void maygate_ctor(M_Object obj, VM_Shred shred)
{
  SP_maygate* ug = malloc(sizeof(SP_maygate));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_maygate_create(&ug->osc);
  sp_maygate_init(ug->sp, ug->osc);
  obj->ugen->tick = maygate_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void maygate_dtor(M_Object obj, VM_Shred shred)
{
  SP_maygate* ug = (SP_maygate*)obj->ugen->ug;
  sp_maygate_destroy(&ug->osc);
  free(ug);
}

static void maygate_get_prob(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_maygate* ug = (SP_maygate*)obj->ugen->ug;
  RETURN->v_float = ug->osc->prob;
}

static void maygate_set_prob(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_maygate* ug = (SP_maygate*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->prob = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void maygate_get_mode(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_maygate* ug = (SP_maygate*)obj->ugen->ug;
  RETURN->v_uint = ug->osc->mode;
}

static void maygate_set_mode(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_maygate* ug = (SP_maygate*)obj->ugen->ug;
  RETURN->v_uint = (ug->osc->mode = *(m_int*)(shred->mem + SZ_INT));
}

struct Type_ t_maygate = { "Maygate", sizeof(m_uint), &t_ugen };

static m_bool import_maygate(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_maygate))
  CHECK_BB(import_class_begin(env, &t_maygate, env->global_nspc, maygate_ctor, maygate_dtor))
  env->class_def->doc = "A randomly open or closed \"maybe gate\"    It takes in a trigger, and then it will randomly decide to turn the gate on or not. One particular application for maygate is to arbitrarily turn on/off sends to effects. One specific example of this could be a randomized reverb throw on a snare.";

  fun = new_DL_Func("float", "prob", (m_uint)maygate_get_prob);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Probability of maygate. This is a value between 0-1. The closer to 1, the more likely the maygate will let a signal through.";

  fun = new_DL_Func("float", "prob", (m_uint)maygate_set_prob);
    dl_func_add_arg(fun, "float", "prob");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Probability of maygate. This is a value between 0-1. The closer to 1, the more likely the maygate will let a signal through.";

  fun = new_DL_Func("int", "mode", (m_uint)maygate_get_mode);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "If mode is nonzero, maygate will output one sample triggers instead of a gate signal.";

  fun = new_DL_Func("int", "mode", (m_uint)maygate_set_mode);
    dl_func_add_arg(fun, "int", "mode");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "If mode is nonzero, maygate will output one sample triggers instead of a gate signal.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_metro* osc;
} SP_metro;

static m_bool metro_tick(UGen u)
{
  SP_metro* ug = (SP_metro*)u->ug;
  ugen_compute(u->trig->ugen);
  sp_metro_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void metro_ctor(M_Object obj, VM_Shred shred)
{
  SP_metro* ug = malloc(sizeof(SP_metro));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_metro_create(&ug->osc);
  sp_metro_init(ug->sp, ug->osc);
  obj->ugen->tick = metro_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void metro_dtor(M_Object obj, VM_Shred shred)
{
  SP_metro* ug = (SP_metro*)obj->ugen->ug;
  sp_metro_destroy(&ug->osc);
  free(ug);
}

static void metro_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_metro* ug = (SP_metro*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void metro_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_metro* ug = (SP_metro*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_metro = { "Metro", sizeof(m_uint), &t_ugen };

static m_bool import_metro(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_metro))
  CHECK_BB(import_class_begin(env, &t_metro, env->global_nspc, metro_ctor, metro_dtor))
  env->class_def->doc = "Metronome    Metro produces a series of 1-sample ticks at a regular rate. Typically, this is used alongside trigger-driven modules.";

  fun = new_DL_Func("float", "freq", (m_uint)metro_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The frequency to repeat.";

  fun = new_DL_Func("float", "freq", (m_uint)metro_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The frequency to repeat.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_mincer* osc;
  char is_init;
  sp_ftbl*  ft;
} SP_mincer;

static m_bool mincer_tick(UGen u)
{
  SP_mincer* ug = (SP_mincer*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  sp_mincer_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void mincer_ctor(M_Object obj, VM_Shred shred)
{
  SP_mincer* ug = malloc(sizeof(SP_mincer));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_mincer_create(&ug->osc);
  obj->ugen->tick = mincer_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void mincer_dtor(M_Object obj, VM_Shred shred)
{
  SP_mincer* ug = (SP_mincer*)obj->ugen->ug;
  if(ug->is_init)
    sp_mincer_destroy(&ug->osc);
  if(ug->is_init)
    sp_ftbl_destroy(&ug->ft);
  free(ug);
}

static void mincer_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_mincer* ug = (SP_mincer*)obj->ugen->ug;
  if(ug->is_init)
  mincer_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object ft_obj   = *(M_Object*)(shred->mem + offset);
  sp_ftbl* ft_ftbl  = FTBL(ft_obj);
  ug->ft            = ft_ftbl;
  offset += SZ_INT;
  sp_mincer_init(ug->sp, ug->osc, ft_ftbl);
}

static void mincer_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_mincer* ug = (SP_mincer*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  sp_ftbl* ft_ftbl = ug->ft = to_ftbl(*(M_Object*)(shred->mem + offset), shred);
  offset += SZ_INT;
  sp_mincer_init(ug->sp, ug->osc, ft_ftbl);
}

static void mincer_get_time(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_mincer* ug = (SP_mincer*)obj->ugen->ug;
  RETURN->v_float = ug->osc->time;
}

static void mincer_set_time(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_mincer* ug = (SP_mincer*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->time = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void mincer_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_mincer* ug = (SP_mincer*)obj->ugen->ug;
  RETURN->v_float = ug->osc->amp;
}

static void mincer_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_mincer* ug = (SP_mincer*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->amp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void mincer_get_pitch(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_mincer* ug = (SP_mincer*)obj->ugen->ug;
  RETURN->v_float = ug->osc->pitch;
}

static void mincer_set_pitch(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_mincer* ug = (SP_mincer*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->pitch = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_mincer = { "Mincer", sizeof(m_uint), &t_ugen };

static m_bool import_mincer(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_mincer))
  CHECK_BB(import_class_begin(env, &t_mincer, env->global_nspc, mincer_ctor, mincer_dtor))
  env->class_def->doc = "Phase-locked vocoder.    Mincer is a phase-locked vocoder. It has the ability to play back an audio file loaded into an ftable like a sampler would. Unlike a typical sampler, mincer allowstime and pitch to be controlled separately. ";

  fun = new_DL_Func("void", "init", (m_uint)mincer_init);
   dl_func_add_arg(fun, "ftbl", "ft");
  f->doc = "ftable containing an audio file.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)mincer_init_array);
   dl_func_add_arg(fun, "float[]", "ft");
  f->doc = "ftable containing an audio file.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "time", (m_uint)mincer_get_time);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Position in time. When non-changing it will do a spectral freeze of a the current point in time.";

  fun = new_DL_Func("float", "time", (m_uint)mincer_set_time);
    dl_func_add_arg(fun, "float", "time");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Position in time. When non-changing it will do a spectral freeze of a the current point in time.";

  fun = new_DL_Func("float", "amp", (m_uint)mincer_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude.";

  fun = new_DL_Func("float", "amp", (m_uint)mincer_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude.";

  fun = new_DL_Func("float", "pitch", (m_uint)mincer_get_pitch);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Pitch ratio. A value of. 1  normal, 2 is double speed, 0.5 is halfspeed, etc.";

  fun = new_DL_Func("float", "pitch", (m_uint)mincer_set_pitch);
    dl_func_add_arg(fun, "float", "pitch");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Pitch ratio. A value of. 1  normal, 2 is double speed, 0.5 is halfspeed, etc.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_mode* osc;
} SP_mode;

static m_bool mode_tick(UGen u)
{
  SP_mode* ug = (SP_mode*)u->ug;
  base_tick(u);
  sp_mode_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void mode_ctor(M_Object obj, VM_Shred shred)
{
  SP_mode* ug = malloc(sizeof(SP_mode));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_mode_create(&ug->osc);
  sp_mode_init(ug->sp, ug->osc);
  obj->ugen->tick = mode_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void mode_dtor(M_Object obj, VM_Shred shred)
{
  SP_mode* ug = (SP_mode*)obj->ugen->ug;
  sp_mode_destroy(&ug->osc);
  free(ug);
}

static void mode_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_mode* ug = (SP_mode*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void mode_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_mode* ug = (SP_mode*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void mode_get_q(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_mode* ug = (SP_mode*)obj->ugen->ug;
  RETURN->v_float = ug->osc->q;
}

static void mode_set_q(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_mode* ug = (SP_mode*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->q = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_mode = { "Mode", sizeof(m_uint), &t_ugen };

static m_bool import_mode(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_mode))
  CHECK_BB(import_class_begin(env, &t_mode, env->global_nspc, mode_ctor, mode_dtor))
  env->class_def->doc = "Resonance filter used for modal synthesis    Plucked and bell sounds can be created by passing an impulse through a combination of modal filters. ";

  fun = new_DL_Func("float", "freq", (m_uint)mode_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Resonant frequency of the filter.";

  fun = new_DL_Func("float", "freq", (m_uint)mode_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Resonant frequency of the filter.";

  fun = new_DL_Func("float", "q", (m_uint)mode_get_q);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Quality factor of the filter. Roughly equal to q/freq.";

  fun = new_DL_Func("float", "q", (m_uint)mode_set_q);
    dl_func_add_arg(fun, "float", "q");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Quality factor of the filter. Roughly equal to q/freq.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_moogladder* osc;
} SP_moogladder;

static m_bool moogladder_tick(UGen u)
{
  SP_moogladder* ug = (SP_moogladder*)u->ug;
  base_tick(u);
  sp_moogladder_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void moogladder_ctor(M_Object obj, VM_Shred shred)
{
  SP_moogladder* ug = malloc(sizeof(SP_moogladder));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_moogladder_create(&ug->osc);
  sp_moogladder_init(ug->sp, ug->osc);
  obj->ugen->tick = moogladder_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void moogladder_dtor(M_Object obj, VM_Shred shred)
{
  SP_moogladder* ug = (SP_moogladder*)obj->ugen->ug;
  sp_moogladder_destroy(&ug->osc);
  free(ug);
}

static void moogladder_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_moogladder* ug = (SP_moogladder*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void moogladder_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_moogladder* ug = (SP_moogladder*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void moogladder_get_res(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_moogladder* ug = (SP_moogladder*)obj->ugen->ug;
  RETURN->v_float = ug->osc->res;
}

static void moogladder_set_res(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_moogladder* ug = (SP_moogladder*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->res = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_moogladder = { "Moogladder", sizeof(m_uint), &t_ugen };

static m_bool import_moogladder(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_moogladder))
  CHECK_BB(import_class_begin(env, &t_moogladder, env->global_nspc, moogladder_ctor, moogladder_dtor))
  env->class_def->doc = "Low pass resonant filter based on the Moogladder filter";

  fun = new_DL_Func("float", "freq", (m_uint)moogladder_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Filter cutoff frequency.";

  fun = new_DL_Func("float", "freq", (m_uint)moogladder_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Filter cutoff frequency.";

  fun = new_DL_Func("float", "res", (m_uint)moogladder_get_res);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Filter resonance";

  fun = new_DL_Func("float", "res", (m_uint)moogladder_set_res);
    dl_func_add_arg(fun, "float", "res");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Filter resonance";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_noise* osc;
} SP_noise;

static m_bool noise_tick(UGen u)
{
  SP_noise* ug = (SP_noise*)u->ug;
  sp_noise_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void noise_ctor(M_Object obj, VM_Shred shred)
{
  SP_noise* ug = malloc(sizeof(SP_noise));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_noise_create(&ug->osc);
  sp_noise_init(ug->sp, ug->osc);
  obj->ugen->tick = noise_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void noise_dtor(M_Object obj, VM_Shred shred)
{
  SP_noise* ug = (SP_noise*)obj->ugen->ug;
  sp_noise_destroy(&ug->osc);
  free(ug);
}

static void noise_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_noise* ug = (SP_noise*)obj->ugen->ug;
  RETURN->v_float = ug->osc->amp;
}

static void noise_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_noise* ug = (SP_noise*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->amp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_noise = { "Noise", sizeof(m_uint), &t_ugen };

static m_bool import_noise(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_noise))
  CHECK_BB(import_class_begin(env, &t_noise, env->global_nspc, noise_ctor, noise_dtor))
  env->class_def->doc = "White noise generator";

  fun = new_DL_Func("float", "amp", (m_uint)noise_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude. (Value between 0-1).";

  fun = new_DL_Func("float", "amp", (m_uint)noise_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude. (Value between 0-1).";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_nsmp* osc;
  char is_init;
  sp_ftbl*  ft;
  m_int     sr;
  m_str     ini;
} SP_nsmp;

static m_bool nsmp_tick(UGen u)
{
  SP_nsmp* ug = (SP_nsmp*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  ugen_compute(u->trig->ugen);
  sp_nsmp_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void nsmp_ctor(M_Object obj, VM_Shred shred)
{
  SP_nsmp* ug = malloc(sizeof(SP_nsmp));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_nsmp_create(&ug->osc);
  obj->ugen->tick = nsmp_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void nsmp_dtor(M_Object obj, VM_Shred shred)
{
  SP_nsmp* ug = (SP_nsmp*)obj->ugen->ug;
  if(ug->is_init)
    sp_nsmp_destroy(&ug->osc);
  if(ug->is_init)
    sp_ftbl_destroy(&ug->ft);
  free(ug);
}

static void nsmp_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_nsmp* ug = (SP_nsmp*)obj->ugen->ug;
  if(ug->is_init)
  nsmp_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object ft_obj   = *(M_Object*)(shred->mem + offset);
  sp_ftbl* ft_ftbl  = FTBL(ft_obj);
  ug->ft            = ft_ftbl;
  offset += SZ_INT;
  M_Object ini_obj   = *(M_Object*)(shred->mem + offset);
  m_str    ini_str   = STRING(obj);
  offset += SZ_INT;
  sp_nsmp_init(ug->sp, ug->osc, ft_ftbl, *(m_int*)(shred->mem + SZ_INT*2), ini_str);
  shred->reg  -= SZ_INT*2;
}

static void nsmp_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_nsmp* ug = (SP_nsmp*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  sp_ftbl* ft_ftbl = ug->ft = to_ftbl(*(M_Object*)(shred->mem + offset), shred);
  offset += SZ_INT;
  M_Object ini_obj   = *(M_Object*)(shred->mem + offset);
  m_str    ini_str   = STRING(obj);
  offset += SZ_INT;
  sp_nsmp_init(ug->sp, ug->osc, ft_ftbl, *(m_int*)(shred->mem + SZ_INT*2), ini_str);
}

static void nsmp_get_index(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_nsmp* ug = (SP_nsmp*)obj->ugen->ug;
  RETURN->v_uint = ug->osc->index;
}

static void nsmp_set_index(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_nsmp* ug = (SP_nsmp*)obj->ugen->ug;
  RETURN->v_uint = (ug->osc->index = *(m_int*)(shred->mem + SZ_INT));
}

struct Type_ t_nsmp = { "Nsmp", sizeof(m_uint), &t_ugen };

static m_bool import_nsmp(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_nsmp))
  CHECK_BB(import_class_begin(env, &t_nsmp, env->global_nspc, nsmp_ctor, nsmp_dtor))
  env->class_def->doc = "Nanosamp: a tiny sampler built for Soundpipe    A nanosamp file is comprised of a mono audio file and an ini file. Nanosamp is geared towards percussive and found sound sample players, and is intended to be combined with soundpipe modules.The ini file contains mappings that correspond to the audio file. Such an entry would look like this:[keyword]pos = 2.3size = 3</pre>In this instance, an entry called \"keyword\" has been made, starting at 2.3 seconds in theaudio file, with a length of 3 seconds. An example file oneart.ini has been created in theexamples folder.The SoundPipe implementation of nanosamp will automatically index the entriesin the order they appear in the INI file and must be selected this way by changing the indexparameter. Soundpipe will only select the new entry when the trigger input is a non-zero value.";

  fun = new_DL_Func("void", "init", (m_uint)nsmp_init);
   dl_func_add_arg(fun, "ftbl", "ft");
  f->doc = "The filepath of the audio file. It should be mono and matching the samplerate of the project, as no resampling is done.";

   dl_func_add_arg(fun, "int", "sr");
  f->doc = "samplerate of the project.";

   dl_func_add_arg(fun, "string", "ini");
  f->doc = "The filepath of the INI file. See oneart.ini in the examples folder for an example of an INI file.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)nsmp_init_array);
   dl_func_add_arg(fun, "float[]", "ft");
  f->doc = "The filepath of the audio file. It should be mono and matching the samplerate of the project, as no resampling is done.";

   dl_func_add_arg(fun, "int", "sr");
  f->doc = "samplerate of the project.";

   dl_func_add_arg(fun, "string", "ini");
  f->doc = "The filepath of the INI file. See oneart.ini in the examples folder for an example of an INI file.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("int", "index", (m_uint)nsmp_get_index);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "This is an optional parameter. These values are always set to a value by default, and can be set after the init function has been called.";

  fun = new_DL_Func("int", "index", (m_uint)nsmp_set_index);
    dl_func_add_arg(fun, "int", "index");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "This is an optional parameter. These values are always set to a value by default, and can be set after the init function has been called.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_osc* osc;
  char is_init;
  sp_ftbl*  tbl;
  m_float   phase;
} SP_osc;

static m_bool osc_tick(UGen u)
{
  SP_osc* ug = (SP_osc*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  sp_osc_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void osc_ctor(M_Object obj, VM_Shred shred)
{
  SP_osc* ug = malloc(sizeof(SP_osc));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_osc_create(&ug->osc);
  obj->ugen->tick = osc_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void osc_dtor(M_Object obj, VM_Shred shred)
{
  SP_osc* ug = (SP_osc*)obj->ugen->ug;
  if(ug->is_init)
    sp_osc_destroy(&ug->osc);
  if(ug->is_init)
    sp_ftbl_destroy(&ug->tbl);
  free(ug);
}

static void osc_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_osc* ug = (SP_osc*)obj->ugen->ug;
  if(ug->is_init)
  osc_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object tbl_obj   = *(M_Object*)(shred->mem + offset);
  sp_ftbl* tbl_ftbl  = FTBL(tbl_obj);
  ug->tbl            = tbl_ftbl;
  offset += SZ_INT;
  m_float    phase   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_osc_init(ug->sp, ug->osc, tbl_ftbl, (ug->phase = phase));
  shred->reg  -= SZ_INT*1;
}

static void osc_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_osc* ug = (SP_osc*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  sp_ftbl* tbl_ftbl = ug->tbl = to_ftbl(*(M_Object*)(shred->mem + offset), shred);
  offset += SZ_INT;
  m_float    phase   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_osc_init(ug->sp, ug->osc, tbl_ftbl, (ug->phase = phase));
}

static void osc_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_osc* ug = (SP_osc*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void osc_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_osc* ug = (SP_osc*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void osc_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_osc* ug = (SP_osc*)obj->ugen->ug;
  RETURN->v_float = ug->osc->amp;
}

static void osc_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_osc* ug = (SP_osc*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->amp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_osc = { "Osc", sizeof(m_uint), &t_ugen };

static m_bool import_osc(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_osc))
  CHECK_BB(import_class_begin(env, &t_osc, env->global_nspc, osc_ctor, osc_dtor))
  env->class_def->doc = " Table-lookup oscilator with linear interpolation";

  fun = new_DL_Func("void", "init", (m_uint)osc_init);
   dl_func_add_arg(fun, "ftbl", "tbl");
  f->doc = "Wavetable to read from. Note: the size of this table must be a power of 2.";

   dl_func_add_arg(fun, "float", "phase");
  f->doc = "Initial phase of waveform, expects a value 0-1";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)osc_init_array);
   dl_func_add_arg(fun, "float[]", "tbl");
  f->doc = "Wavetable to read from. Note: the size of this table must be a power of 2.";

   dl_func_add_arg(fun, "float", "phase");
  f->doc = "Initial phase of waveform, expects a value 0-1";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "freq", (m_uint)osc_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency (in Hz)";

  fun = new_DL_Func("float", "freq", (m_uint)osc_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency (in Hz)";

  fun = new_DL_Func("float", "amp", (m_uint)osc_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude (typically a value between 0 and 1).";

  fun = new_DL_Func("float", "amp", (m_uint)osc_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude (typically a value between 0 and 1).";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_oscmorph* osc;
  char is_init;
  sp_ftbl** tbl;
  m_int     nft;
  m_float   phase;
} SP_oscmorph;

static m_bool oscmorph_tick(UGen u)
{
  SP_oscmorph* ug = (SP_oscmorph*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  sp_oscmorph_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void oscmorph_ctor(M_Object obj, VM_Shred shred)
{
  SP_oscmorph* ug = malloc(sizeof(SP_oscmorph));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_oscmorph_create(&ug->osc);
  obj->ugen->tick = oscmorph_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void oscmorph_dtor(M_Object obj, VM_Shred shred)
{
  SP_oscmorph* ug = (SP_oscmorph*)obj->ugen->ug;
  if(ug->is_init)
    sp_oscmorph_destroy(&ug->osc);
  free(ug);
}

static void oscmorph_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_oscmorph* ug = (SP_oscmorph*)obj->ugen->ug;
  if(ug->is_init)
  oscmorph_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object tbl_obj   = *(M_Object*)(shred->mem + offset);
	 M_Vector* tbl_vector  = tbl_obj->array;
  m_uint tbl_count, tbl_len = m_vector_size(tbl_vector);
  sp_ftbl* tbl_ftbl_ptr[tbl_len];
  for(tbl_count = 0; tbl_count < tbl_len; tbl_count++)
    tbl_ftbl_ptr[tbl_count] = (sp_ftbl*)i_vector_at(tbl_vector, tbl_count);
  offset += SZ_INT;
  m_float    phase   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_oscmorph_init(ug->sp, ug->osc, tbl_ftbl_ptr, *(m_int*)(shred->mem + SZ_INT*2), (ug->phase = phase));
  shred->reg  -= SZ_INT*2;
}

static void oscmorph_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_oscmorph* ug = (SP_oscmorph*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object tbl_obj   = *(M_Object*)(shred->mem + offset);
	 M_Vector* tbl_vector  = tbl_obj->array;
  m_uint tbl_count, tbl_len = m_vector_size(tbl_vector);
  sp_ftbl* tbl_ftbl_ptr[tbl_len];
  for(tbl_count = 0; tbl_count < tbl_len; tbl_count++)
  {
  tbl_ftbl_ptr[tbl_count] = to_ftbl((M_Object)i_vector_at(tbl_vector, tbl_count), shred);
  }
  offset += SZ_INT;
  m_float    phase   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_oscmorph_init(ug->sp, ug->osc, tbl_ftbl_ptr, *(m_int*)(shred->mem + SZ_INT*2), (ug->phase = phase));
}

static void oscmorph_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_oscmorph* ug = (SP_oscmorph*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void oscmorph_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_oscmorph* ug = (SP_oscmorph*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void oscmorph_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_oscmorph* ug = (SP_oscmorph*)obj->ugen->ug;
  RETURN->v_float = ug->osc->amp;
}

static void oscmorph_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_oscmorph* ug = (SP_oscmorph*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->amp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void oscmorph_get_wtpos(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_oscmorph* ug = (SP_oscmorph*)obj->ugen->ug;
  RETURN->v_float = ug->osc->wtpos;
}

static void oscmorph_set_wtpos(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_oscmorph* ug = (SP_oscmorph*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->wtpos = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_oscmorph = { "Oscmorph", sizeof(m_uint), &t_ugen };

static m_bool import_oscmorph(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_oscmorph))
  CHECK_BB(import_class_begin(env, &t_oscmorph, env->global_nspc, oscmorph_ctor, oscmorph_dtor))
  env->class_def->doc = "Wavetable morphing oscillatorThis is an oscillator with linear interpolation that is capable of morphing between an arbitrary number of wavetables. ";

  fun = new_DL_Func("void", "init", (m_uint)oscmorph_init);
   dl_func_add_arg(fun, "ftbl[]", "tbl");
  f->doc = "An array of ftables to read from. Note: the size of these tables must be a power of 2 (and the same size as well).";

   dl_func_add_arg(fun, "int", "nft");
  f->doc = "Number of ftbls";

   dl_func_add_arg(fun, "float", "phase");
  f->doc = "Initial phase of waveform, expects a value 0-1";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)oscmorph_init_array);
   dl_func_add_arg(fun, "ftbl[]", "tbl");
  f->doc = "An array of ftables to read from. Note: the size of these tables must be a power of 2 (and the same size as well).";

   dl_func_add_arg(fun, "int", "nft");
  f->doc = "Number of ftbls";

   dl_func_add_arg(fun, "float", "phase");
  f->doc = "Initial phase of waveform, expects a value 0-1";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "freq", (m_uint)oscmorph_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency (in Hz)";

  fun = new_DL_Func("float", "freq", (m_uint)oscmorph_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency (in Hz)";

  fun = new_DL_Func("float", "amp", (m_uint)oscmorph_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude (typically a value between 0 and 1).";

  fun = new_DL_Func("float", "amp", (m_uint)oscmorph_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude (typically a value between 0 and 1).";

  fun = new_DL_Func("float", "wtpos", (m_uint)oscmorph_get_wtpos);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Wavetable position. (scaled 0-1)";

  fun = new_DL_Func("float", "wtpos", (m_uint)oscmorph_set_wtpos);
    dl_func_add_arg(fun, "float", "wtpos");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Wavetable position. (scaled 0-1)";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_pan2* osc;
} SP_pan2;

static m_bool pan2_tick(UGen u)
{
  SP_pan2* ug = (SP_pan2*)u->ug;
  M_Object l = (M_Object)u->channel[0];
  M_Object r = (M_Object)u->channel[1];
  u->in = (l->ugen->in + r->ugen->in)/2;
  sp_pan2_compute(ug->sp, ug->osc, &u->in, &l->ugen->out, &r->ugen->out);
}

static void pan2_ctor(M_Object obj, VM_Shred shred)
{
  SP_pan2* ug = malloc(sizeof(SP_pan2));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_pan2_create(&ug->osc);
  sp_pan2_init(ug->sp, ug->osc);
  obj->ugen->tick = pan2_tick;
  assign_ugen(obj->ugen, 1, 2, 0, ug);
}

static void pan2_dtor(M_Object obj, VM_Shred shred)
{
  SP_pan2* ug = (SP_pan2*)obj->ugen->ug;
  sp_pan2_destroy(&ug->osc);
  free(ug);
}

static void pan2_get_type(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pan2* ug = (SP_pan2*)obj->ugen->ug;
  RETURN->v_uint = ug->osc->type;
}

static void pan2_set_type(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pan2* ug = (SP_pan2*)obj->ugen->ug;
  RETURN->v_uint = (ug->osc->type = *(m_int*)(shred->mem + SZ_INT));
}

static void pan2_get_pan(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pan2* ug = (SP_pan2*)obj->ugen->ug;
  RETURN->v_float = ug->osc->pan;
}

static void pan2_set_pan(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pan2* ug = (SP_pan2*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->pan = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_pan2 = { "Pan2", sizeof(m_uint), &t_ugen };

static m_bool import_pan2(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_pan2))
  CHECK_BB(import_class_begin(env, &t_pan2, env->global_nspc, pan2_ctor, pan2_dtor))
  env->class_def->doc = "Panner";

  fun = new_DL_Func("int", "type", (m_uint)pan2_get_type);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Panning type. 0 = equal power, 1 = square root, 2 = linear,3 = alternative equal power. Values outside this range will wrap. ";

  fun = new_DL_Func("int", "type", (m_uint)pan2_set_type);
    dl_func_add_arg(fun, "int", "type");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Panning type. 0 = equal power, 1 = square root, 2 = linear,3 = alternative equal power. Values outside this range will wrap. ";

  fun = new_DL_Func("float", "pan", (m_uint)pan2_get_pan);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Panning. A value of -1 is hard left, and a value of 1 is hard right, and 0 is center.";

  fun = new_DL_Func("float", "pan", (m_uint)pan2_set_pan);
    dl_func_add_arg(fun, "float", "pan");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Panning. A value of -1 is hard left, and a value of 1 is hard right, and 0 is center.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_panst* osc;
} SP_panst;

static m_bool panst_tick(UGen u)
{
  SP_panst* ug = (SP_panst*)u->ug;
  M_Object l = (M_Object)u->channel[0];
  M_Object r = (M_Object)u->channel[1];
  ugen_compute(l->ugen);
  ugen_compute(l->ugen);
  sp_panst_compute(ug->sp, ug->osc, &l->ugen->in, &r->ugen->in, &l->ugen->out, &r->ugen->out);
}

static void panst_ctor(M_Object obj, VM_Shred shred)
{
  SP_panst* ug = malloc(sizeof(SP_panst));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_panst_create(&ug->osc);
  sp_panst_init(ug->sp, ug->osc);
  obj->ugen->tick = panst_tick;
  assign_ugen(obj->ugen, 2, 2, 0, ug);
}

static void panst_dtor(M_Object obj, VM_Shred shred)
{
  SP_panst* ug = (SP_panst*)obj->ugen->ug;
  sp_panst_destroy(&ug->osc);
  free(ug);
}

static void panst_get_type(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_panst* ug = (SP_panst*)obj->ugen->ug;
  RETURN->v_uint = ug->osc->type;
}

static void panst_set_type(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_panst* ug = (SP_panst*)obj->ugen->ug;
  RETURN->v_uint = (ug->osc->type = *(m_int*)(shred->mem + SZ_INT));
}

static void panst_get_pan(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_panst* ug = (SP_panst*)obj->ugen->ug;
  RETURN->v_float = ug->osc->pan;
}

static void panst_set_pan(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_panst* ug = (SP_panst*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->pan = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_panst = { "Panst", sizeof(m_uint), &t_ugen };

static m_bool import_panst(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_panst))
  CHECK_BB(import_class_begin(env, &t_panst, env->global_nspc, panst_ctor, panst_dtor))
  env->class_def->doc = "Stereo Panner";

  fun = new_DL_Func("int", "type", (m_uint)panst_get_type);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Panning type. 0 = equal power, 1 = square root, 2 = linear,3 = alternative equal power. Values outside this range will wrap. ";

  fun = new_DL_Func("int", "type", (m_uint)panst_set_type);
    dl_func_add_arg(fun, "int", "type");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Panning type. 0 = equal power, 1 = square root, 2 = linear,3 = alternative equal power. Values outside this range will wrap. ";

  fun = new_DL_Func("float", "pan", (m_uint)panst_get_pan);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Panning. A value of -1 is hard left, and a value of 1 is hard right.";

  fun = new_DL_Func("float", "pan", (m_uint)panst_set_pan);
    dl_func_add_arg(fun, "float", "pan");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Panning. A value of -1 is hard left, and a value of 1 is hard right.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_pareq* osc;
} SP_pareq;

static m_bool pareq_tick(UGen u)
{
  SP_pareq* ug = (SP_pareq*)u->ug;
  base_tick(u);
  sp_pareq_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void pareq_ctor(M_Object obj, VM_Shred shred)
{
  SP_pareq* ug = malloc(sizeof(SP_pareq));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_pareq_create(&ug->osc);
  sp_pareq_init(ug->sp, ug->osc);
  obj->ugen->tick = pareq_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void pareq_dtor(M_Object obj, VM_Shred shred)
{
  SP_pareq* ug = (SP_pareq*)obj->ugen->ug;
  sp_pareq_destroy(&ug->osc);
  free(ug);
}

static void pareq_get_fc(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pareq* ug = (SP_pareq*)obj->ugen->ug;
  RETURN->v_float = ug->osc->fc;
}

static void pareq_set_fc(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pareq* ug = (SP_pareq*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->fc = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void pareq_get_v(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pareq* ug = (SP_pareq*)obj->ugen->ug;
  RETURN->v_float = ug->osc->v;
}

static void pareq_set_v(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pareq* ug = (SP_pareq*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->v = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void pareq_get_q(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pareq* ug = (SP_pareq*)obj->ugen->ug;
  RETURN->v_float = ug->osc->q;
}

static void pareq_set_q(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pareq* ug = (SP_pareq*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->q = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void pareq_get_mode(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pareq* ug = (SP_pareq*)obj->ugen->ug;
  RETURN->v_float = ug->osc->mode;
}

static void pareq_set_mode(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pareq* ug = (SP_pareq*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->mode = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_pareq = { "Pareq", sizeof(m_uint), &t_ugen };

static m_bool import_pareq(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_pareq))
  CHECK_BB(import_class_begin(env, &t_pareq, env->global_nspc, pareq_ctor, pareq_dtor))
  env->class_def->doc = "Parametric EqualizerThis is an implementation of Zoelzer's parametric equalizer filter.";

  fun = new_DL_Func("float", "fc", (m_uint)pareq_get_fc);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Center frequency in peak mode, corner frequency in shelving mode.";

  fun = new_DL_Func("float", "fc", (m_uint)pareq_set_fc);
    dl_func_add_arg(fun, "float", "fc");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Center frequency in peak mode, corner frequency in shelving mode.";

  fun = new_DL_Func("float", "v", (m_uint)pareq_get_v);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amount at which the center frequency value shall be increased or decreased. A value of 1 is a flat response.";

  fun = new_DL_Func("float", "v", (m_uint)pareq_set_v);
    dl_func_add_arg(fun, "float", "v");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amount at which the center frequency value shall be increased or decreased. A value of 1 is a flat response.";

  fun = new_DL_Func("float", "q", (m_uint)pareq_get_q);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Q of the filter. sqrt(0.5) is no resonance.";

  fun = new_DL_Func("float", "q", (m_uint)pareq_set_q);
    dl_func_add_arg(fun, "float", "q");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Q of the filter. sqrt(0.5) is no resonance.";

  fun = new_DL_Func("float", "mode", (m_uint)pareq_get_mode);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "EQ mode. 0 = peak, 1 = low shelving, 2 = high shelving";

  fun = new_DL_Func("float", "mode", (m_uint)pareq_set_mode);
    dl_func_add_arg(fun, "float", "mode");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "EQ mode. 0 = peak, 1 = low shelving, 2 = high shelving";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_paulstretch* osc;
  char is_init;
  sp_ftbl*  ft;
  m_float     windowsize;
  m_float   stretch;
} SP_paulstretch;

static m_bool paulstretch_tick(UGen u)
{
  SP_paulstretch* ug = (SP_paulstretch*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  sp_paulstretch_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void paulstretch_ctor(M_Object obj, VM_Shred shred)
{
  SP_paulstretch* ug = malloc(sizeof(SP_paulstretch));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_paulstretch_create(&ug->osc);
  obj->ugen->tick = paulstretch_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void paulstretch_dtor(M_Object obj, VM_Shred shred)
{
  SP_paulstretch* ug = (SP_paulstretch*)obj->ugen->ug;
  if(ug->is_init)
    sp_paulstretch_destroy(&ug->osc);
  if(ug->is_init)
    sp_ftbl_destroy(&ug->ft);
  free(ug);
}

static void paulstretch_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_paulstretch* ug = (SP_paulstretch*)obj->ugen->ug;
  if(ug->is_init)
  paulstretch_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object ft_obj   = *(M_Object*)(shred->mem + offset);
  sp_ftbl* ft_ftbl  = FTBL(ft_obj);
  ug->ft            = ft_ftbl;
  offset += SZ_INT;
  m_float    windowsize   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  m_float    stretch   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_paulstretch_init(ug->sp, ug->osc, ft_ftbl, windowsize / (m_float)ug->sp->sr, (ug->stretch = stretch));
  shred->reg  -= SZ_INT*2;
}

static void paulstretch_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_paulstretch* ug = (SP_paulstretch*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  sp_ftbl* ft_ftbl = ug->ft = to_ftbl(*(M_Object*)(shred->mem + offset), shred);
  offset += SZ_INT;
  m_float    windowsize   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  m_float    stretch   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_paulstretch_init(ug->sp, ug->osc, ft_ftbl, windowsize / (m_float)ug->sp->sr, (ug->stretch = stretch));
}

struct Type_ t_paulstretch = { "Paulstretch", sizeof(m_uint), &t_ugen };

static m_bool import_paulstretch(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_paulstretch))
  CHECK_BB(import_class_begin(env, &t_paulstretch, env->global_nspc, paulstretch_ctor, paulstretch_dtor))
  env->class_def->doc = "An extreme time-stretching algorithm by Paul Nasca OctavianThis is an implementation of the popular paulstretch algorithm used for timestretching an audio signal to create ambient textures. Ideally, this algorithm is best used for stretching signals by very very long amounts. This version of paulstretch will take an ftable and loop through it, makeit an ideal means for creating sustained pads. ";

  fun = new_DL_Func("void", "init", (m_uint)paulstretch_init);
   dl_func_add_arg(fun, "ftbl", "ft");
  f->doc = "ftable containing audio data";

   dl_func_add_arg(fun, "dur", "windowsize");
  f->doc = "Window size, in seconds.";

   dl_func_add_arg(fun, "float", "stretch");
  f->doc = "Stretch factor, 1.0 is no stretch.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)paulstretch_init_array);
   dl_func_add_arg(fun, "float[]", "ft");
  f->doc = "ftable containing audio data";

   dl_func_add_arg(fun, "dur", "windowsize");
  f->doc = "Window size, in seconds.";

   dl_func_add_arg(fun, "float", "stretch");
  f->doc = "Stretch factor, 1.0 is no stretch.";

  CHECK_BB((f = import_mfun(env, fun)))

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_pdhalf* osc;
} SP_pdhalf;

static m_bool pdhalf_tick(UGen u)
{
  SP_pdhalf* ug = (SP_pdhalf*)u->ug;
  base_tick(u);
  sp_pdhalf_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void pdhalf_ctor(M_Object obj, VM_Shred shred)
{
  SP_pdhalf* ug = malloc(sizeof(SP_pdhalf));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_pdhalf_create(&ug->osc);
  sp_pdhalf_init(ug->sp, ug->osc);
  obj->ugen->tick = pdhalf_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void pdhalf_dtor(M_Object obj, VM_Shred shred)
{
  SP_pdhalf* ug = (SP_pdhalf*)obj->ugen->ug;
  sp_pdhalf_destroy(&ug->osc);
  free(ug);
}

static void pdhalf_get_amount(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pdhalf* ug = (SP_pdhalf*)obj->ugen->ug;
  RETURN->v_float = ug->osc->amount;
}

static void pdhalf_set_amount(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pdhalf* ug = (SP_pdhalf*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->amount = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_pdhalf = { "Pdhalf", sizeof(m_uint), &t_ugen };

static m_bool import_pdhalf(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_pdhalf))
  CHECK_BB(import_class_begin(env, &t_pdhalf, env->global_nspc, pdhalf_ctor, pdhalf_dtor))
  env->class_def->doc = "Casio-style phase distortion with \"pivot point\" on the X axisThis module is designed to emulate the classic phase distortion synthesis technique.From the mid 90's. The technique reads the first and second halves of the ftblat different rates in order to warp the waveform. For example, pdhalf can smoothly transition a sinewave into something approximating a sawtooth wave.";

  fun = new_DL_Func("float", "amount", (m_uint)pdhalf_get_amount);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amount of distortion, within the range [-1, 1]. 0 is no distortion.";

  fun = new_DL_Func("float", "amount", (m_uint)pdhalf_set_amount);
    dl_func_add_arg(fun, "float", "amount");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amount of distortion, within the range [-1, 1]. 0 is no distortion.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_peaklim* osc;
} SP_peaklim;

static m_bool peaklim_tick(UGen u)
{
  SP_peaklim* ug = (SP_peaklim*)u->ug;
  base_tick(u);
  sp_peaklim_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void peaklim_ctor(M_Object obj, VM_Shred shred)
{
  SP_peaklim* ug = malloc(sizeof(SP_peaklim));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_peaklim_create(&ug->osc);
  sp_peaklim_init(ug->sp, ug->osc);
  obj->ugen->tick = peaklim_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void peaklim_dtor(M_Object obj, VM_Shred shred)
{
  SP_peaklim* ug = (SP_peaklim*)obj->ugen->ug;
  sp_peaklim_destroy(&ug->osc);
  free(ug);
}

static void peaklim_get_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_peaklim* ug = (SP_peaklim*)obj->ugen->ug;
  RETURN->v_float = ug->osc->atk * ug->sp->sr;
}

static void peaklim_set_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_peaklim* ug = (SP_peaklim*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->atk = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void peaklim_get_rel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_peaklim* ug = (SP_peaklim*)obj->ugen->ug;
  RETURN->v_float = ug->osc->rel * ug->sp->sr;
}

static void peaklim_set_rel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_peaklim* ug = (SP_peaklim*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->rel = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void peaklim_get_thresh(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_peaklim* ug = (SP_peaklim*)obj->ugen->ug;
  RETURN->v_float = ug->osc->thresh;
}

static void peaklim_set_thresh(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_peaklim* ug = (SP_peaklim*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->thresh = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_peaklim = { "Peaklim", sizeof(m_uint), &t_ugen };

static m_bool import_peaklim(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_peaklim))
  CHECK_BB(import_class_begin(env, &t_peaklim, env->global_nspc, peaklim_ctor, peaklim_dtor))
  env->class_def->doc = "Peak limiter This is a simple peak limiting algorithm, based off code from the StanfordMusic-424 class.";

  fun = new_DL_Func("dur", "atk", (m_uint)peaklim_get_atk);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Attack time, in seconds";

  fun = new_DL_Func("dur", "atk", (m_uint)peaklim_set_atk);
    dl_func_add_arg(fun, "dur", "atk");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Attack time, in seconds";

  fun = new_DL_Func("dur", "rel", (m_uint)peaklim_get_rel);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Release time, in seconds";

  fun = new_DL_Func("dur", "rel", (m_uint)peaklim_set_rel);
    dl_func_add_arg(fun, "dur", "rel");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Release time, in seconds";

  fun = new_DL_Func("float", "thresh", (m_uint)peaklim_get_thresh);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Threshold, in dB";

  fun = new_DL_Func("float", "thresh", (m_uint)peaklim_set_thresh);
    dl_func_add_arg(fun, "float", "thresh");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Threshold, in dB";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_phaser* osc;
} SP_phaser;

static m_bool phaser_tick(UGen u)
{
  SP_phaser* ug = (SP_phaser*)u->ug;
  M_Object l = (M_Object)u->channel[0];
  M_Object r = (M_Object)u->channel[1];
  ugen_compute(l->ugen);
  ugen_compute(l->ugen);
  sp_phaser_compute(ug->sp, ug->osc, &l->ugen->in, &r->ugen->in, &l->ugen->out, &r->ugen->out);
}

static void phaser_ctor(M_Object obj, VM_Shred shred)
{
  SP_phaser* ug = malloc(sizeof(SP_phaser));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_phaser_create(&ug->osc);
  sp_phaser_init(ug->sp, ug->osc);
  obj->ugen->tick = phaser_tick;
  assign_ugen(obj->ugen, 2, 2, 0, ug);
}

static void phaser_dtor(M_Object obj, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  sp_phaser_destroy(&ug->osc);
  free(ug);
}

static void phaser_get_MaxNotch1Freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->MaxNotch1Freq;
}

static void phaser_set_MaxNotch1Freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->MaxNotch1Freq = *(m_float*)(shred->mem + SZ_INT));
}

static void phaser_get_MinNotch1Freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->MinNotch1Freq;
}

static void phaser_set_MinNotch1Freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->MinNotch1Freq = *(m_float*)(shred->mem + SZ_INT));
}

static void phaser_get_Notch_width(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->Notch_width;
}

static void phaser_set_Notch_width(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->Notch_width = *(m_float*)(shred->mem + SZ_INT));
}

static void phaser_get_NotchFreq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->NotchFreq;
}

static void phaser_set_NotchFreq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->NotchFreq = *(m_float*)(shred->mem + SZ_INT));
}

static void phaser_get_VibratoMode(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->VibratoMode;
}

static void phaser_set_VibratoMode(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->VibratoMode = *(m_float*)(shred->mem + SZ_INT));
}

static void phaser_get_depth(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->depth;
}

static void phaser_set_depth(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->depth = *(m_float*)(shred->mem + SZ_INT));
}

static void phaser_get_feedback_gain(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->feedback_gain;
}

static void phaser_set_feedback_gain(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->feedback_gain = *(m_float*)(shred->mem + SZ_INT));
}

static void phaser_get_invert(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->invert;
}

static void phaser_set_invert(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->invert = *(m_float*)(shred->mem + SZ_INT));
}

static void phaser_get_level(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->level;
}

static void phaser_set_level(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->level = *(m_float*)(shred->mem + SZ_INT));
}

static void phaser_get_lfobpm(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->lfobpm;
}

static void phaser_set_lfobpm(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phaser* ug = (SP_phaser*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->lfobpm = *(m_float*)(shred->mem + SZ_INT));
}

struct Type_ t_phaser = { "Phaser", sizeof(m_uint), &t_ugen };

static m_bool import_phaser(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_phaser))
  CHECK_BB(import_class_begin(env, &t_phaser, env->global_nspc, phaser_ctor, phaser_dtor))
  env->class_def->doc = "A stereo phaserThis is a stereo phaser, generated from Faust code taken from the Guitarix project.";

  fun = new_DL_Func("float", "MaxNotch1Freq", (m_uint)phaser_get_MaxNotch1Freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Between 20 and 10000";

  fun = new_DL_Func("float", "MaxNotch1Freq", (m_uint)phaser_set_MaxNotch1Freq);
    dl_func_add_arg(fun, "float", "MaxNotch1Freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Between 20 and 10000";

  fun = new_DL_Func("float", "MinNotch1Freq", (m_uint)phaser_get_MinNotch1Freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Between 20 and 5000";

  fun = new_DL_Func("float", "MinNotch1Freq", (m_uint)phaser_set_MinNotch1Freq);
    dl_func_add_arg(fun, "float", "MinNotch1Freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Between 20 and 5000";

  fun = new_DL_Func("float", "Notch_width", (m_uint)phaser_get_Notch_width);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Between 10 and 5000";

  fun = new_DL_Func("float", "Notch_width", (m_uint)phaser_set_Notch_width);
    dl_func_add_arg(fun, "float", "Notch_width");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Between 10 and 5000";

  fun = new_DL_Func("float", "NotchFreq", (m_uint)phaser_get_NotchFreq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Between 1.1 and 4";

  fun = new_DL_Func("float", "NotchFreq", (m_uint)phaser_set_NotchFreq);
    dl_func_add_arg(fun, "float", "NotchFreq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Between 1.1 and 4";

  fun = new_DL_Func("float", "VibratoMode", (m_uint)phaser_get_VibratoMode);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "1 or 0";

  fun = new_DL_Func("float", "VibratoMode", (m_uint)phaser_set_VibratoMode);
    dl_func_add_arg(fun, "float", "VibratoMode");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "1 or 0";

  fun = new_DL_Func("float", "depth", (m_uint)phaser_get_depth);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Between 0 and 1";

  fun = new_DL_Func("float", "depth", (m_uint)phaser_set_depth);
    dl_func_add_arg(fun, "float", "depth");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Between 0 and 1";

  fun = new_DL_Func("float", "feedback_gain", (m_uint)phaser_get_feedback_gain);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Between 0 and 1";

  fun = new_DL_Func("float", "feedback_gain", (m_uint)phaser_set_feedback_gain);
    dl_func_add_arg(fun, "float", "feedback_gain");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Between 0 and 1";

  fun = new_DL_Func("float", "invert", (m_uint)phaser_get_invert);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "1 or 0";

  fun = new_DL_Func("float", "invert", (m_uint)phaser_set_invert);
    dl_func_add_arg(fun, "float", "invert");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "1 or 0";

  fun = new_DL_Func("float", "level", (m_uint)phaser_get_level);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "-60 to 10 dB";

  fun = new_DL_Func("float", "level", (m_uint)phaser_set_level);
    dl_func_add_arg(fun, "float", "level");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "-60 to 10 dB";

  fun = new_DL_Func("float", "lfobpm", (m_uint)phaser_get_lfobpm);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Between 24 and 360";

  fun = new_DL_Func("float", "lfobpm", (m_uint)phaser_set_lfobpm);
    dl_func_add_arg(fun, "float", "lfobpm");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Between 24 and 360";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_phasor* osc;
  char is_init;
  m_float   iphs;
} SP_phasor;

static m_bool phasor_tick(UGen u)
{
  SP_phasor* ug = (SP_phasor*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  sp_phasor_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void phasor_ctor(M_Object obj, VM_Shred shred)
{
  SP_phasor* ug = malloc(sizeof(SP_phasor));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_phasor_create(&ug->osc);
  obj->ugen->tick = phasor_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void phasor_dtor(M_Object obj, VM_Shred shred)
{
  SP_phasor* ug = (SP_phasor*)obj->ugen->ug;
  if(ug->is_init)
    sp_phasor_destroy(&ug->osc);
  free(ug);
}

static void phasor_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phasor* ug = (SP_phasor*)obj->ugen->ug;
  if(ug->is_init)
  phasor_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    iphs   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_phasor_init(ug->sp, ug->osc, (ug->iphs = iphs));
}

static void phasor_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phasor* ug = (SP_phasor*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    iphs   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_phasor_init(ug->sp, ug->osc, (ug->iphs = iphs));
}

static void phasor_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phasor* ug = (SP_phasor*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void phasor_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_phasor* ug = (SP_phasor*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_phasor = { "Phasor", sizeof(m_uint), &t_ugen };

static m_bool import_phasor(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_phasor))
  CHECK_BB(import_class_begin(env, &t_phasor, env->global_nspc, phasor_ctor, phasor_dtor))
  env->class_def->doc = "Normalized sawtooth wave from 0 to 1    Phasors are often used when building table-lookup oscillators.";

  fun = new_DL_Func("void", "init", (m_uint)phasor_init);
   dl_func_add_arg(fun, "float", "iphs");
  f->doc = "initial phase";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)phasor_init_array);
   dl_func_add_arg(fun, "float", "iphs");
  f->doc = "initial phase";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "freq", (m_uint)phasor_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency.";

  fun = new_DL_Func("float", "freq", (m_uint)phasor_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_pinknoise* osc;
} SP_pinknoise;

static m_bool pinknoise_tick(UGen u)
{
  SP_pinknoise* ug = (SP_pinknoise*)u->ug;
  sp_pinknoise_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void pinknoise_ctor(M_Object obj, VM_Shred shred)
{
  SP_pinknoise* ug = malloc(sizeof(SP_pinknoise));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_pinknoise_create(&ug->osc);
  sp_pinknoise_init(ug->sp, ug->osc);
  obj->ugen->tick = pinknoise_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void pinknoise_dtor(M_Object obj, VM_Shred shred)
{
  SP_pinknoise* ug = (SP_pinknoise*)obj->ugen->ug;
  sp_pinknoise_destroy(&ug->osc);
  free(ug);
}

static void pinknoise_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pinknoise* ug = (SP_pinknoise*)obj->ugen->ug;
  RETURN->v_float = ug->osc->amp;
}

static void pinknoise_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pinknoise* ug = (SP_pinknoise*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->amp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_pinknoise = { "Pinknoise", sizeof(m_uint), &t_ugen };

static m_bool import_pinknoise(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_pinknoise))
  CHECK_BB(import_class_begin(env, &t_pinknoise, env->global_nspc, pinknoise_ctor, pinknoise_dtor))
  env->class_def->doc = "Pink pinknoise generator";

  fun = new_DL_Func("float", "amp", (m_uint)pinknoise_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude. (Value between 0-1).";

  fun = new_DL_Func("float", "amp", (m_uint)pinknoise_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude. (Value between 0-1).";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_pitchamdf* osc;
  char is_init;
  m_float   min;
  m_float   max;
} SP_pitchamdf;

static m_bool pitchamdf_tick(UGen u)
{
  SP_pitchamdf* ug = (SP_pitchamdf*)u->ug;
  if(!ug->is_init)
{
    int i;
    for(i = 0; i < u->n_out; i++)
    {
      u->channel[i]->ugen->out = 0;
    }
  return 1;
 }
  M_Object l = (M_Object)u->channel[0];
  M_Object r = (M_Object)u->channel[1];
  u->in = (l->ugen->in + r->ugen->in)/2;
  sp_pitchamdf_compute(ug->sp, ug->osc, &u->in, &l->ugen->out, &r->ugen->out);
}

static void pitchamdf_ctor(M_Object obj, VM_Shred shred)
{
  SP_pitchamdf* ug = malloc(sizeof(SP_pitchamdf));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_pitchamdf_create(&ug->osc);
  obj->ugen->tick = pitchamdf_tick;
  assign_ugen(obj->ugen, 1, 2, 0, ug);
}

static void pitchamdf_dtor(M_Object obj, VM_Shred shred)
{
  SP_pitchamdf* ug = (SP_pitchamdf*)obj->ugen->ug;
  if(ug->is_init)
    sp_pitchamdf_destroy(&ug->osc);
  free(ug);
}

static void pitchamdf_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pitchamdf* ug = (SP_pitchamdf*)obj->ugen->ug;
  if(ug->is_init)
  pitchamdf_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    min   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  m_float    max   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_pitchamdf_init(ug->sp, ug->osc, (ug->min = min), (ug->max = max));
  shred->reg  -= SZ_INT*1;
}

static void pitchamdf_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pitchamdf* ug = (SP_pitchamdf*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    min   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  m_float    max   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_pitchamdf_init(ug->sp, ug->osc, (ug->min = min), (ug->max = max));
}

struct Type_ t_pitchamdf = { "Pitchamdf", sizeof(m_uint), &t_ugen };

static m_bool import_pitchamdf(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_pitchamdf))
  CHECK_BB(import_class_begin(env, &t_pitchamdf, env->global_nspc, pitchamdf_ctor, pitchamdf_dtor))
  env->class_def->doc = "Pitch detection using AMDF method.Pitchamdf tracks the pitch of signal using the AMDF (Average Magnitude Difference Function) method of Pitch following. ";

  fun = new_DL_Func("void", "init", (m_uint)pitchamdf_init);
   dl_func_add_arg(fun, "float", "min");
  f->doc = "Minimum expected frequency to detect";

   dl_func_add_arg(fun, "float", "max");
  f->doc = "Maximum expected frequency to detect";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)pitchamdf_init_array);
   dl_func_add_arg(fun, "float", "min");
  f->doc = "Minimum expected frequency to detect";

   dl_func_add_arg(fun, "float", "max");
  f->doc = "Maximum expected frequency to detect";

  CHECK_BB((f = import_mfun(env, fun)))

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_pluck* osc;
  char is_init;
  m_float   ifreq;
} SP_pluck;

static m_bool pluck_tick(UGen u)
{
  SP_pluck* ug = (SP_pluck*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  ugen_compute(u->trig->ugen);
  sp_pluck_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void pluck_ctor(M_Object obj, VM_Shred shred)
{
  SP_pluck* ug = malloc(sizeof(SP_pluck));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_pluck_create(&ug->osc);
  obj->ugen->tick = pluck_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void pluck_dtor(M_Object obj, VM_Shred shred)
{
  SP_pluck* ug = (SP_pluck*)obj->ugen->ug;
  if(ug->is_init)
    sp_pluck_destroy(&ug->osc);
  free(ug);
}

static void pluck_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pluck* ug = (SP_pluck*)obj->ugen->ug;
  if(ug->is_init)
  pluck_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    ifreq   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_pluck_init(ug->sp, ug->osc, (ug->ifreq = ifreq));
}

static void pluck_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pluck* ug = (SP_pluck*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    ifreq   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_pluck_init(ug->sp, ug->osc, (ug->ifreq = ifreq));
}

static void pluck_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pluck* ug = (SP_pluck*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void pluck_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pluck* ug = (SP_pluck*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void pluck_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pluck* ug = (SP_pluck*)obj->ugen->ug;
  RETURN->v_float = ug->osc->amp;
}

static void pluck_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pluck* ug = (SP_pluck*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->amp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_pluck = { "Pluck", sizeof(m_uint), &t_ugen };

static m_bool import_pluck(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_pluck))
  CHECK_BB(import_class_begin(env, &t_pluck, env->global_nspc, pluck_ctor, pluck_dtor))
  env->class_def->doc = "Karplus-Strong plucked string instrument.";

  fun = new_DL_Func("void", "init", (m_uint)pluck_init);
   dl_func_add_arg(fun, "float", "ifreq");
  f->doc = "Sets the initial frequency. This frequency is used to allocate all the buffers needed for the delay. This should be the lowest frequency you plan on using.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)pluck_init_array);
   dl_func_add_arg(fun, "float", "ifreq");
  f->doc = "Sets the initial frequency. This frequency is used to allocate all the buffers needed for the delay. This should be the lowest frequency you plan on using.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "freq", (m_uint)pluck_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Variable frequency. Values less than the initial frequency (ifreq) will be doubled until it is greater than or equal to ifreq.";

  fun = new_DL_Func("float", "freq", (m_uint)pluck_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Variable frequency. Values less than the initial frequency (ifreq) will be doubled until it is greater than or equal to ifreq.";

  fun = new_DL_Func("float", "amp", (m_uint)pluck_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude";

  fun = new_DL_Func("float", "amp", (m_uint)pluck_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_port* osc;
  char is_init;
  m_float   htime;
} SP_port;

static m_bool port_tick(UGen u)
{
  SP_port* ug = (SP_port*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  base_tick(u);
  sp_port_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void port_ctor(M_Object obj, VM_Shred shred)
{
  SP_port* ug = malloc(sizeof(SP_port));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_port_create(&ug->osc);
  obj->ugen->tick = port_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void port_dtor(M_Object obj, VM_Shred shred)
{
  SP_port* ug = (SP_port*)obj->ugen->ug;
  if(ug->is_init)
    sp_port_destroy(&ug->osc);
  free(ug);
}

static void port_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_port* ug = (SP_port*)obj->ugen->ug;
  if(ug->is_init)
  port_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    htime   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_port_init(ug->sp, ug->osc, (ug->htime = htime));
}

static void port_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_port* ug = (SP_port*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    htime   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_port_init(ug->sp, ug->osc, (ug->htime = htime));
}

struct Type_ t_port = { "Port", sizeof(m_uint), &t_ugen };

static m_bool import_port(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_port))
  CHECK_BB(import_class_begin(env, &t_port, env->global_nspc, port_ctor, port_dtor))
  env->class_def->doc = " Portamento-style control signal smoothing    Useful for smoothing out low-resolution signals and applying glissando to filters.";

  fun = new_DL_Func("void", "init", (m_uint)port_init);
   dl_func_add_arg(fun, "float", "htime");
  f->doc = "portamento time";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)port_init_array);
   dl_func_add_arg(fun, "float", "htime");
  f->doc = "portamento time";

  CHECK_BB((f = import_mfun(env, fun)))

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_posc3* osc;
  char is_init;
  sp_ftbl*  tbl;
} SP_posc3;

static m_bool posc3_tick(UGen u)
{
  SP_posc3* ug = (SP_posc3*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  sp_posc3_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void posc3_ctor(M_Object obj, VM_Shred shred)
{
  SP_posc3* ug = malloc(sizeof(SP_posc3));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_posc3_create(&ug->osc);
  obj->ugen->tick = posc3_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void posc3_dtor(M_Object obj, VM_Shred shred)
{
  SP_posc3* ug = (SP_posc3*)obj->ugen->ug;
  if(ug->is_init)
    sp_posc3_destroy(&ug->osc);
  if(ug->is_init)
    sp_ftbl_destroy(&ug->tbl);
  free(ug);
}

static void posc3_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_posc3* ug = (SP_posc3*)obj->ugen->ug;
  if(ug->is_init)
  posc3_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object tbl_obj   = *(M_Object*)(shred->mem + offset);
  sp_ftbl* tbl_ftbl  = FTBL(tbl_obj);
  ug->tbl            = tbl_ftbl;
  offset += SZ_INT;
  sp_posc3_init(ug->sp, ug->osc, tbl_ftbl);
}

static void posc3_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_posc3* ug = (SP_posc3*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  sp_ftbl* tbl_ftbl = ug->tbl = to_ftbl(*(M_Object*)(shred->mem + offset), shred);
  offset += SZ_INT;
  sp_posc3_init(ug->sp, ug->osc, tbl_ftbl);
}

static void posc3_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_posc3* ug = (SP_posc3*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void posc3_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_posc3* ug = (SP_posc3*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void posc3_get_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_posc3* ug = (SP_posc3*)obj->ugen->ug;
  RETURN->v_float = ug->osc->amp;
}

static void posc3_set_amp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_posc3* ug = (SP_posc3*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->amp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_posc3 = { "Posc3", sizeof(m_uint), &t_ugen };

static m_bool import_posc3(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_posc3))
  CHECK_BB(import_class_begin(env, &t_posc3, env->global_nspc, posc3_ctor, posc3_dtor))
  env->class_def->doc = " High-precision table-lookup posc3ilator with cubic interpolation";

  fun = new_DL_Func("void", "init", (m_uint)posc3_init);
   dl_func_add_arg(fun, "ftbl", "tbl");
  f->doc = "Wavetable to read from. Note: the size of this table must be a power of 2.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)posc3_init_array);
   dl_func_add_arg(fun, "float[]", "tbl");
  f->doc = "Wavetable to read from. Note: the size of this table must be a power of 2.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "freq", (m_uint)posc3_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency (in Hz)";

  fun = new_DL_Func("float", "freq", (m_uint)posc3_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency (in Hz)";

  fun = new_DL_Func("float", "amp", (m_uint)posc3_get_amp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude (typically a value between 0 and 1).";

  fun = new_DL_Func("float", "amp", (m_uint)posc3_set_amp);
    dl_func_add_arg(fun, "float", "amp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Amplitude (typically a value between 0 and 1).";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_prop* osc;
  char is_init;
  m_str     str;
} SP_prop;

static m_bool prop_tick(UGen u)
{
  SP_prop* ug = (SP_prop*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  sp_prop_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void prop_ctor(M_Object obj, VM_Shred shred)
{
  SP_prop* ug = malloc(sizeof(SP_prop));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_prop_create(&ug->osc);
  obj->ugen->tick = prop_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void prop_dtor(M_Object obj, VM_Shred shred)
{
  SP_prop* ug = (SP_prop*)obj->ugen->ug;
  if(ug->is_init)
    sp_prop_destroy(&ug->osc);
  free(ug);
}

static void prop_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_prop* ug = (SP_prop*)obj->ugen->ug;
  if(ug->is_init)
  prop_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object str_obj   = *(M_Object*)(shred->mem + offset);
  m_str    str_str   = STRING(obj);
  offset += SZ_INT;
  sp_prop_init(ug->sp, ug->osc, str_str);
}

static void prop_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_prop* ug = (SP_prop*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object str_obj   = *(M_Object*)(shred->mem + offset);
  m_str    str_str   = STRING(obj);
  offset += SZ_INT;
  sp_prop_init(ug->sp, ug->osc, str_str);
}

static void prop_get_bpm(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_prop* ug = (SP_prop*)obj->ugen->ug;
  RETURN->v_float = ug->osc->bpm;
}

static void prop_set_bpm(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_prop* ug = (SP_prop*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->bpm = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_prop = { "Prop", sizeof(m_uint), &t_ugen };

static m_bool import_prop(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_prop))
  CHECK_BB(import_class_begin(env, &t_prop, env->global_nspc, prop_ctor, prop_dtor))
  env->class_def->doc = "Simple rhythmic notation gate generator    Creates a gate using a simple rhythmic notation system called prop. When it reaches the end of the prop string, it will loop back to the beginning.Prop has a few basic rules:1. A '+' denotes a on. A '-' denotes an off (rest). They each have an equal duration of a quarter note.2. On and off values can be strung together to create equally spaced gates: +-+--3. When notes are enclosed in parantheses '()' following a positive integer N, their duration is reduced N times: ++2(+-)4. When notes are enclosed in brackets '[]' following a positive integer N, their duration is scaled by a factor of N: ++2[++]5. Parenthesis and brackets can be nested: +- 2[3(+2(++)+)]2(++)";

  fun = new_DL_Func("void", "init", (m_uint)prop_init);
   dl_func_add_arg(fun, "string", "str");
  f->doc = "Prop string to be parsed.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)prop_init_array);
   dl_func_add_arg(fun, "string", "str");
  f->doc = "Prop string to be parsed.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "bpm", (m_uint)prop_get_bpm);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Beats per minute of the prop string.";

  fun = new_DL_Func("float", "bpm", (m_uint)prop_set_bpm);
    dl_func_add_arg(fun, "float", "bpm");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Beats per minute of the prop string.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_pshift* osc;
} SP_pshift;

static m_bool pshift_tick(UGen u)
{
  SP_pshift* ug = (SP_pshift*)u->ug;
  base_tick(u);
  sp_pshift_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void pshift_ctor(M_Object obj, VM_Shred shred)
{
  SP_pshift* ug = malloc(sizeof(SP_pshift));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_pshift_create(&ug->osc);
  sp_pshift_init(ug->sp, ug->osc);
  obj->ugen->tick = pshift_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void pshift_dtor(M_Object obj, VM_Shred shred)
{
  SP_pshift* ug = (SP_pshift*)obj->ugen->ug;
  sp_pshift_destroy(&ug->osc);
  free(ug);
}

static void pshift_get_shift(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pshift* ug = (SP_pshift*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->shift;
}

static void pshift_set_shift(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pshift* ug = (SP_pshift*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->shift = *(m_float*)(shred->mem + SZ_INT));
}

static void pshift_get_window(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pshift* ug = (SP_pshift*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->window;
}

static void pshift_set_window(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pshift* ug = (SP_pshift*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->window = *(m_float*)(shred->mem + SZ_INT));
}

static void pshift_get_xfade(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pshift* ug = (SP_pshift*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->xfade;
}

static void pshift_set_xfade(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_pshift* ug = (SP_pshift*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->xfade = *(m_float*)(shred->mem + SZ_INT));
}

struct Type_ t_pshift = { "Pshift", sizeof(m_uint), &t_ugen };

static m_bool import_pshift(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_pshift))
  CHECK_BB(import_class_begin(env, &t_pshift, env->global_nspc, pshift_ctor, pshift_dtor))
  env->class_def->doc = "Time-domain pitch shifter.";

  fun = new_DL_Func("float", "shift", (m_uint)pshift_get_shift);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Pitch shift (in semitones), range -24/24.";

  fun = new_DL_Func("float", "shift", (m_uint)pshift_set_shift);
    dl_func_add_arg(fun, "float", "shift");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Pitch shift (in semitones), range -24/24.";

  fun = new_DL_Func("float", "window", (m_uint)pshift_get_window);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Window size (in samples), max 10000";

  fun = new_DL_Func("float", "window", (m_uint)pshift_set_window);
    dl_func_add_arg(fun, "float", "window");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Window size (in samples), max 10000";

  fun = new_DL_Func("float", "xfade", (m_uint)pshift_get_xfade);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Crossfade (in samples), max 10000";

  fun = new_DL_Func("float", "xfade", (m_uint)pshift_set_xfade);
    dl_func_add_arg(fun, "float", "xfade");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Crossfade (in samples), max 10000";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_ptrack* osc;
  char is_init;
  m_int     ihopsize;
  m_int     ipeaks;
} SP_ptrack;

static m_bool ptrack_tick(UGen u)
{
  SP_ptrack* ug = (SP_ptrack*)u->ug;
  if(!ug->is_init)
{
    int i;
    for(i = 0; i < u->n_out; i++)
    {
      u->channel[i]->ugen->out = 0;
    }
  return 1;
 }
  M_Object l = (M_Object)u->channel[0];
  M_Object r = (M_Object)u->channel[1];
  u->in = (l->ugen->in + r->ugen->in)/2;
  sp_ptrack_compute(ug->sp, ug->osc, &u->in, &l->ugen->out, &r->ugen->out);
}

static void ptrack_ctor(M_Object obj, VM_Shred shred)
{
  SP_ptrack* ug = malloc(sizeof(SP_ptrack));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_ptrack_create(&ug->osc);
  obj->ugen->tick = ptrack_tick;
  assign_ugen(obj->ugen, 1, 2, 0, ug);
}

static void ptrack_dtor(M_Object obj, VM_Shred shred)
{
  SP_ptrack* ug = (SP_ptrack*)obj->ugen->ug;
  if(ug->is_init)
    sp_ptrack_destroy(&ug->osc);
  free(ug);
}

static void ptrack_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_ptrack* ug = (SP_ptrack*)obj->ugen->ug;
  if(ug->is_init)
  ptrack_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  sp_ptrack_init(ug->sp, ug->osc, *(m_int*)(shred->mem + SZ_INT*1), *(m_int*)(shred->mem + SZ_INT*2));
  shred->reg  -= SZ_INT*1;
}

static void ptrack_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_ptrack* ug = (SP_ptrack*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  sp_ptrack_init(ug->sp, ug->osc, *(m_int*)(shred->mem + SZ_INT*1), *(m_int*)(shred->mem + SZ_INT*2));
}

struct Type_ t_ptrack = { "Ptrack", sizeof(m_uint), &t_ugen };

static m_bool import_ptrack(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_ptrack))
  CHECK_BB(import_class_begin(env, &t_ptrack, env->global_nspc, ptrack_ctor, ptrack_dtor))
  env->class_def->doc = "Track pitch of a signal.Ptrack is a pitch tracker, based on an algorithm originally created byMiller Puckette.";

  fun = new_DL_Func("void", "init", (m_uint)ptrack_init);
   dl_func_add_arg(fun, "int", "ihopsize");
  f->doc = "hop size.";

   dl_func_add_arg(fun, "int", "ipeaks");
  f->doc = "Number of peaks.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)ptrack_init_array);
   dl_func_add_arg(fun, "int", "ihopsize");
  f->doc = "hop size.";

   dl_func_add_arg(fun, "int", "ipeaks");
  f->doc = "Number of peaks.";

  CHECK_BB((f = import_mfun(env, fun)))

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_randh* osc;
} SP_randh;

static m_bool randh_tick(UGen u)
{
  SP_randh* ug = (SP_randh*)u->ug;
  sp_randh_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void randh_ctor(M_Object obj, VM_Shred shred)
{
  SP_randh* ug = malloc(sizeof(SP_randh));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_randh_create(&ug->osc);
  sp_randh_init(ug->sp, ug->osc);
  obj->ugen->tick = randh_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void randh_dtor(M_Object obj, VM_Shred shred)
{
  SP_randh* ug = (SP_randh*)obj->ugen->ug;
  sp_randh_destroy(&ug->osc);
  free(ug);
}

static void randh_get_min(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_randh* ug = (SP_randh*)obj->ugen->ug;
  RETURN->v_float = ug->osc->min;
}

static void randh_set_min(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_randh* ug = (SP_randh*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->min = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void randh_get_max(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_randh* ug = (SP_randh*)obj->ugen->ug;
  RETURN->v_float = ug->osc->max;
}

static void randh_set_max(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_randh* ug = (SP_randh*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->max = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void randh_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_randh* ug = (SP_randh*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void randh_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_randh* ug = (SP_randh*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_randh = { "Randh", sizeof(m_uint), &t_ugen };

static m_bool import_randh(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_randh))
  CHECK_BB(import_class_begin(env, &t_randh, env->global_nspc, randh_ctor, randh_dtor))
  env->class_def->doc = "Random number generator with hold time.Randh is loosely based off of the Csound opcode randomh. The design is equivalentto scaled noise sent through a classic sample and hold module.";

  fun = new_DL_Func("float", "min", (m_uint)randh_get_min);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Minimum value to use.";

  fun = new_DL_Func("float", "min", (m_uint)randh_set_min);
    dl_func_add_arg(fun, "float", "min");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Minimum value to use.";

  fun = new_DL_Func("float", "max", (m_uint)randh_get_max);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Maximum value to use.";

  fun = new_DL_Func("float", "max", (m_uint)randh_set_max);
    dl_func_add_arg(fun, "float", "max");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Maximum value to use.";

  fun = new_DL_Func("float", "freq", (m_uint)randh_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency of randomization (in Hz)";

  fun = new_DL_Func("float", "freq", (m_uint)randh_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency of randomization (in Hz)";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_randi* osc;
} SP_randi;

static m_bool randi_tick(UGen u)
{
  SP_randi* ug = (SP_randi*)u->ug;
  sp_randi_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void randi_ctor(M_Object obj, VM_Shred shred)
{
  SP_randi* ug = malloc(sizeof(SP_randi));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_randi_create(&ug->osc);
  sp_randi_init(ug->sp, ug->osc);
  obj->ugen->tick = randi_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void randi_dtor(M_Object obj, VM_Shred shred)
{
  SP_randi* ug = (SP_randi*)obj->ugen->ug;
  sp_randi_destroy(&ug->osc);
  free(ug);
}

static void randi_get_min(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_randi* ug = (SP_randi*)obj->ugen->ug;
  RETURN->v_float = ug->osc->min;
}

static void randi_set_min(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_randi* ug = (SP_randi*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->min = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void randi_get_max(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_randi* ug = (SP_randi*)obj->ugen->ug;
  RETURN->v_float = ug->osc->max;
}

static void randi_set_max(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_randi* ug = (SP_randi*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->max = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void randi_get_cps(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_randi* ug = (SP_randi*)obj->ugen->ug;
  RETURN->v_float = ug->osc->cps;
}

static void randi_set_cps(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_randi* ug = (SP_randi*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->cps = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void randi_get_mode(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_randi* ug = (SP_randi*)obj->ugen->ug;
  RETURN->v_float = ug->osc->mode;
}

static void randi_set_mode(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_randi* ug = (SP_randi*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->mode = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_randi = { "Randi", sizeof(m_uint), &t_ugen };

static m_bool import_randi(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_randi))
  CHECK_BB(import_class_begin(env, &t_randi, env->global_nspc, randi_ctor, randi_dtor))
  env->class_def->doc = "Line segments between random values within a range";

  fun = new_DL_Func("float", "min", (m_uint)randi_get_min);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Minimum value";

  fun = new_DL_Func("float", "min", (m_uint)randi_set_min);
    dl_func_add_arg(fun, "float", "min");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Minimum value";

  fun = new_DL_Func("float", "max", (m_uint)randi_get_max);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Maximum value";

  fun = new_DL_Func("float", "max", (m_uint)randi_set_max);
    dl_func_add_arg(fun, "float", "max");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Maximum value";

  fun = new_DL_Func("float", "cps", (m_uint)randi_get_cps);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency to change values.";

  fun = new_DL_Func("float", "cps", (m_uint)randi_set_cps);
    dl_func_add_arg(fun, "float", "cps");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency to change values.";

  fun = new_DL_Func("float", "mode", (m_uint)randi_get_mode);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Randi mode (not yet implemented yet.)";

  fun = new_DL_Func("float", "mode", (m_uint)randi_set_mode);
    dl_func_add_arg(fun, "float", "mode");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Randi mode (not yet implemented yet.)";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_random* osc;
} SP_random;

static m_bool random_tick(UGen u)
{
  SP_random* ug = (SP_random*)u->ug;
  sp_random_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void random_ctor(M_Object obj, VM_Shred shred)
{
  SP_random* ug = malloc(sizeof(SP_random));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_random_create(&ug->osc);
  sp_random_init(ug->sp, ug->osc);
  obj->ugen->tick = random_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void random_dtor(M_Object obj, VM_Shred shred)
{
  SP_random* ug = (SP_random*)obj->ugen->ug;
  sp_random_destroy(&ug->osc);
  free(ug);
}

static void random_get_min(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_random* ug = (SP_random*)obj->ugen->ug;
  RETURN->v_float = ug->osc->min;
}

static void random_set_min(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_random* ug = (SP_random*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->min = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void random_get_max(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_random* ug = (SP_random*)obj->ugen->ug;
  RETURN->v_float = ug->osc->max;
}

static void random_set_max(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_random* ug = (SP_random*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->max = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_random = { "Random", sizeof(m_uint), &t_ugen };

static m_bool import_random(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_random))
  CHECK_BB(import_class_begin(env, &t_random, env->global_nspc, random_ctor, random_dtor))
  env->class_def->doc = "Random values within a range";

  fun = new_DL_Func("float", "min", (m_uint)random_get_min);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Minimum value.";

  fun = new_DL_Func("float", "min", (m_uint)random_set_min);
    dl_func_add_arg(fun, "float", "min");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Minimum value.";

  fun = new_DL_Func("float", "max", (m_uint)random_get_max);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Maximum value.";

  fun = new_DL_Func("float", "max", (m_uint)random_set_max);
    dl_func_add_arg(fun, "float", "max");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Maximum value.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_reson* osc;
} SP_reson;

static m_bool reson_tick(UGen u)
{
  SP_reson* ug = (SP_reson*)u->ug;
  base_tick(u);
  sp_reson_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void reson_ctor(M_Object obj, VM_Shred shred)
{
  SP_reson* ug = malloc(sizeof(SP_reson));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_reson_create(&ug->osc);
  sp_reson_init(ug->sp, ug->osc);
  obj->ugen->tick = reson_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void reson_dtor(M_Object obj, VM_Shred shred)
{
  SP_reson* ug = (SP_reson*)obj->ugen->ug;
  sp_reson_destroy(&ug->osc);
  free(ug);
}

static void reson_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_reson* ug = (SP_reson*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void reson_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_reson* ug = (SP_reson*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void reson_get_bw(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_reson* ug = (SP_reson*)obj->ugen->ug;
  RETURN->v_float = ug->osc->bw;
}

static void reson_set_bw(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_reson* ug = (SP_reson*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->bw = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_reson = { "Reson", sizeof(m_uint), &t_ugen };

static m_bool import_reson(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_reson))
  CHECK_BB(import_class_begin(env, &t_reson, env->global_nspc, reson_ctor, reson_dtor))
  env->class_def->doc = "A second-order resonant filter. NOTE: The output for reson appears to be very hot, so take caution when using this module.";

  fun = new_DL_Func("float", "freq", (m_uint)reson_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Center frequency of the filter, or frequency position of the peak response.";

  fun = new_DL_Func("float", "freq", (m_uint)reson_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Center frequency of the filter, or frequency position of the peak response.";

  fun = new_DL_Func("float", "bw", (m_uint)reson_get_bw);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Bandwidth of the filter.";

  fun = new_DL_Func("float", "bw", (m_uint)reson_set_bw);
    dl_func_add_arg(fun, "float", "bw");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Bandwidth of the filter.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_reverse* osc;
  char is_init;
  m_float     delay;
} SP_reverse;

static m_bool reverse_tick(UGen u)
{
  SP_reverse* ug = (SP_reverse*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  base_tick(u);
  sp_reverse_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void reverse_ctor(M_Object obj, VM_Shred shred)
{
  SP_reverse* ug = malloc(sizeof(SP_reverse));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_reverse_create(&ug->osc);
  obj->ugen->tick = reverse_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void reverse_dtor(M_Object obj, VM_Shred shred)
{
  SP_reverse* ug = (SP_reverse*)obj->ugen->ug;
  if(ug->is_init)
    sp_reverse_destroy(&ug->osc);
  free(ug);
}

static void reverse_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_reverse* ug = (SP_reverse*)obj->ugen->ug;
  if(ug->is_init)
  reverse_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    delay   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_reverse_init(ug->sp, ug->osc, delay / (m_float)ug->sp->sr);
}

static void reverse_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_reverse* ug = (SP_reverse*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    delay   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_reverse_init(ug->sp, ug->osc, delay / (m_float)ug->sp->sr);
}

struct Type_ t_reverse = { "Reverse", sizeof(m_uint), &t_ugen };

static m_bool import_reverse(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_reverse))
  CHECK_BB(import_class_begin(env, &t_reverse, env->global_nspc, reverse_ctor, reverse_dtor))
  env->class_def->doc = "Signal reverserReverse will store a signal inside a buffer and play it back reversed.";

  fun = new_DL_Func("void", "init", (m_uint)reverse_init);
   dl_func_add_arg(fun, "dur", "delay");
  f->doc = "Delay time in seconds.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)reverse_init_array);
   dl_func_add_arg(fun, "dur", "delay");
  f->doc = "Delay time in seconds.";

  CHECK_BB((f = import_mfun(env, fun)))

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_revsc* osc;
} SP_revsc;

static m_bool revsc_tick(UGen u)
{
  SP_revsc* ug = (SP_revsc*)u->ug;
  M_Object l = (M_Object)u->channel[0];
  M_Object r = (M_Object)u->channel[1];
  ugen_compute(l->ugen);
  ugen_compute(l->ugen);
  sp_revsc_compute(ug->sp, ug->osc, &l->ugen->in, &r->ugen->in, &l->ugen->out, &r->ugen->out);
}

static void revsc_ctor(M_Object obj, VM_Shred shred)
{
  SP_revsc* ug = malloc(sizeof(SP_revsc));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_revsc_create(&ug->osc);
  sp_revsc_init(ug->sp, ug->osc);
  obj->ugen->tick = revsc_tick;
  assign_ugen(obj->ugen, 2, 2, 0, ug);
}

static void revsc_dtor(M_Object obj, VM_Shred shred)
{
  SP_revsc* ug = (SP_revsc*)obj->ugen->ug;
  sp_revsc_destroy(&ug->osc);
  free(ug);
}

static void revsc_get_feedback(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_revsc* ug = (SP_revsc*)obj->ugen->ug;
  RETURN->v_float = ug->osc->feedback;
}

static void revsc_set_feedback(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_revsc* ug = (SP_revsc*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->feedback = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void revsc_get_lpfreq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_revsc* ug = (SP_revsc*)obj->ugen->ug;
  RETURN->v_float = ug->osc->lpfreq;
}

static void revsc_set_lpfreq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_revsc* ug = (SP_revsc*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->lpfreq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_revsc = { "Revsc", sizeof(m_uint), &t_ugen };

static m_bool import_revsc(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_revsc))
  CHECK_BB(import_class_begin(env, &t_revsc, env->global_nspc, revsc_ctor, revsc_dtor))
  env->class_def->doc = " 8 FDN stereo reverb";

  fun = new_DL_Func("float", "feedback", (m_uint)revsc_get_feedback);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Value between 0-1 that sets feedback value. The larger the value, the longer the decay.";

  fun = new_DL_Func("float", "feedback", (m_uint)revsc_set_feedback);
    dl_func_add_arg(fun, "float", "feedback");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Value between 0-1 that sets feedback value. The larger the value, the longer the decay.";

  fun = new_DL_Func("float", "lpfreq", (m_uint)revsc_get_lpfreq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "low pass cutoff frequency.";

  fun = new_DL_Func("float", "lpfreq", (m_uint)revsc_set_lpfreq);
    dl_func_add_arg(fun, "float", "lpfreq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "low pass cutoff frequency.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_rms* osc;
} SP_rms;

static m_bool rms_tick(UGen u)
{
  SP_rms* ug = (SP_rms*)u->ug;
  base_tick(u);
  sp_rms_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void rms_ctor(M_Object obj, VM_Shred shred)
{
  SP_rms* ug = malloc(sizeof(SP_rms));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_rms_create(&ug->osc);
  sp_rms_init(ug->sp, ug->osc);
  obj->ugen->tick = rms_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void rms_dtor(M_Object obj, VM_Shred shred)
{
  SP_rms* ug = (SP_rms*)obj->ugen->ug;
  sp_rms_destroy(&ug->osc);
  free(ug);
}

static void rms_get_ihp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_rms* ug = (SP_rms*)obj->ugen->ug;
  RETURN->v_float = ug->osc->ihp;
}

static void rms_set_ihp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_rms* ug = (SP_rms*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->ihp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_rms = { "Rms", sizeof(m_uint), &t_ugen };

static m_bool import_rms(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_rms))
  CHECK_BB(import_class_begin(env, &t_rms, env->global_nspc, rms_ctor, rms_dtor))
  env->class_def->doc = "RMS-averaged signal amplitude    Perform \"root-mean-square\" on a signal to get overall amplitude of a signal. The output signal looks similar to that of a classic VU meter.";

  fun = new_DL_Func("float", "ihp", (m_uint)rms_get_ihp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Half-power point (in Hz) of internal lowpass filter. This parameter is fixed at 10Hz and is not yet mutable.";

  fun = new_DL_Func("float", "ihp", (m_uint)rms_set_ihp);
    dl_func_add_arg(fun, "float", "ihp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Half-power point (in Hz) of internal lowpass filter. This parameter is fixed at 10Hz and is not yet mutable.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_rpt* osc;
  char is_init;
  m_float     maxdur;
} SP_rpt;

static m_bool rpt_tick(UGen u)
{
  SP_rpt* ug = (SP_rpt*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  base_tick(u);
  ugen_compute(u->trig->ugen);
  sp_rpt_compute(ug->sp, ug->osc, &u->in, &u->trig->ugen->out, &u->out);
}

static void rpt_ctor(M_Object obj, VM_Shred shred)
{
  SP_rpt* ug = malloc(sizeof(SP_rpt));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_rpt_create(&ug->osc);
  obj->ugen->tick = rpt_tick;
  assign_ugen(obj->ugen, 1, 1, 1, ug);
}

static void rpt_dtor(M_Object obj, VM_Shred shred)
{
  SP_rpt* ug = (SP_rpt*)obj->ugen->ug;
  if(ug->is_init)
    sp_rpt_destroy(&ug->osc);
  free(ug);
}

static void rpt_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_rpt* ug = (SP_rpt*)obj->ugen->ug;
  if(ug->is_init)
  rpt_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    maxdur   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_rpt_init(ug->sp, ug->osc, maxdur / (m_float)ug->sp->sr);
}

static void rpt_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_rpt* ug = (SP_rpt*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    maxdur   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_rpt_init(ug->sp, ug->osc, maxdur / (m_float)ug->sp->sr);
}

struct Type_ t_rpt = { "Rpt", sizeof(m_uint), &t_ugen };

static m_bool import_rpt(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_rpt))
  CHECK_BB(import_class_begin(env, &t_rpt, env->global_nspc, rpt_ctor, rpt_dtor))
  env->class_def->doc = "Trigger based beat-repeat stuttering effect    When the input is a non-zero value, rpt will load up the buffer and loop a certain number of times. Speed and repeat amounts can be set with the sp_rpt_set function.";

  fun = new_DL_Func("void", "init", (m_uint)rpt_init);
   dl_func_add_arg(fun, "dur", "maxdur");
  f->doc = "Maximum delay duration in seconds. This will set the buffer size.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)rpt_init_array);
   dl_func_add_arg(fun, "dur", "maxdur");
  f->doc = "Maximum delay duration in seconds. This will set the buffer size.";

  CHECK_BB((f = import_mfun(env, fun)))

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_samphold* osc;
} SP_samphold;

static m_bool samphold_tick(UGen u)
{
  SP_samphold* ug = (SP_samphold*)u->ug;
  base_tick(u);
  ugen_compute(u->trig->ugen);
  sp_samphold_compute(ug->sp, ug->osc, &u->in, &u->trig->ugen->out, &u->out);
}

static void samphold_ctor(M_Object obj, VM_Shred shred)
{
  SP_samphold* ug = malloc(sizeof(SP_samphold));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_samphold_create(&ug->osc);
  sp_samphold_init(ug->sp, ug->osc);
  obj->ugen->tick = samphold_tick;
  assign_ugen(obj->ugen, 1, 1, 1, ug);
}

static void samphold_dtor(M_Object obj, VM_Shred shred)
{
  SP_samphold* ug = (SP_samphold*)obj->ugen->ug;
  sp_samphold_destroy(&ug->osc);
  free(ug);
}

struct Type_ t_samphold = { "Samphold", sizeof(m_uint), &t_ugen };

static m_bool import_samphold(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_samphold))
  CHECK_BB(import_class_begin(env, &t_samphold, env->global_nspc, samphold_ctor, samphold_dtor))
  env->class_def->doc = "Classic sample and hold";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_scale* osc;
} SP_scale;

static m_bool scale_tick(UGen u)
{
  SP_scale* ug = (SP_scale*)u->ug;
  base_tick(u);
  sp_scale_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void scale_ctor(M_Object obj, VM_Shred shred)
{
  SP_scale* ug = malloc(sizeof(SP_scale));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_scale_create(&ug->osc);
  sp_scale_init(ug->sp, ug->osc);
  obj->ugen->tick = scale_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void scale_dtor(M_Object obj, VM_Shred shred)
{
  SP_scale* ug = (SP_scale*)obj->ugen->ug;
  sp_scale_destroy(&ug->osc);
  free(ug);
}

static void scale_get_min(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_scale* ug = (SP_scale*)obj->ugen->ug;
  RETURN->v_float = ug->osc->min;
}

static void scale_set_min(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_scale* ug = (SP_scale*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->min = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void scale_get_max(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_scale* ug = (SP_scale*)obj->ugen->ug;
  RETURN->v_float = ug->osc->max;
}

static void scale_set_max(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_scale* ug = (SP_scale*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->max = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_scale = { "Scale", sizeof(m_uint), &t_ugen };

static m_bool import_scale(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_scale))
  CHECK_BB(import_class_begin(env, &t_scale, env->global_nspc, scale_ctor, scale_dtor))
  env->class_def->doc = "Bipolar Scale    This module scales from unipolar [0, 1] to another range defined by min and max.";

  fun = new_DL_Func("float", "min", (m_uint)scale_get_min);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Minimum value to scale to.";

  fun = new_DL_Func("float", "min", (m_uint)scale_set_min);
    dl_func_add_arg(fun, "float", "min");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Minimum value to scale to.";

  fun = new_DL_Func("float", "max", (m_uint)scale_get_max);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Maximum value to scale to.";

  fun = new_DL_Func("float", "max", (m_uint)scale_set_max);
    dl_func_add_arg(fun, "float", "max");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Maximum value to scale to.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_sdelay* osc;
  char is_init;
  m_float   size;
} SP_sdelay;

static m_bool sdelay_tick(UGen u)
{
  SP_sdelay* ug = (SP_sdelay*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  base_tick(u);
  sp_sdelay_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void sdelay_ctor(M_Object obj, VM_Shred shred)
{
  SP_sdelay* ug = malloc(sizeof(SP_sdelay));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_sdelay_create(&ug->osc);
  obj->ugen->tick = sdelay_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void sdelay_dtor(M_Object obj, VM_Shred shred)
{
  SP_sdelay* ug = (SP_sdelay*)obj->ugen->ug;
  if(ug->is_init)
    sp_sdelay_destroy(&ug->osc);
  free(ug);
}

static void sdelay_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_sdelay* ug = (SP_sdelay*)obj->ugen->ug;
  if(ug->is_init)
  sdelay_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    size   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_sdelay_init(ug->sp, ug->osc, (ug->size = size));
}

static void sdelay_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_sdelay* ug = (SP_sdelay*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    size   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_sdelay_init(ug->sp, ug->osc, (ug->size = size));
}

struct Type_ t_sdelay = { "Sdelay", sizeof(m_uint), &t_ugen };

static m_bool import_sdelay(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_sdelay))
  CHECK_BB(import_class_begin(env, &t_sdelay, env->global_nspc, sdelay_ctor, sdelay_dtor))
  env->class_def->doc = "Delays a signal by a number of samples.";

  fun = new_DL_Func("void", "init", (m_uint)sdelay_init);
   dl_func_add_arg(fun, "float", "size");
  f->doc = "Size of delay (in samples)";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)sdelay_init_array);
   dl_func_add_arg(fun, "float", "size");
  f->doc = "Size of delay (in samples)";

  CHECK_BB((f = import_mfun(env, fun)))

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_streson* osc;
} SP_streson;

static m_bool streson_tick(UGen u)
{
  SP_streson* ug = (SP_streson*)u->ug;
  ugen_compute(u->trig->ugen);
  sp_streson_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void streson_ctor(M_Object obj, VM_Shred shred)
{
  SP_streson* ug = malloc(sizeof(SP_streson));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_streson_create(&ug->osc);
  sp_streson_init(ug->sp, ug->osc);
  obj->ugen->tick = streson_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void streson_dtor(M_Object obj, VM_Shred shred)
{
  SP_streson* ug = (SP_streson*)obj->ugen->ug;
  sp_streson_destroy(&ug->osc);
  free(ug);
}

static void streson_get_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_streson* ug = (SP_streson*)obj->ugen->ug;
  RETURN->v_float = ug->osc->freq;
}

static void streson_set_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_streson* ug = (SP_streson*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->freq = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void streson_get_fdbgain(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_streson* ug = (SP_streson*)obj->ugen->ug;
  RETURN->v_float = ug->osc->fdbgain;
}

static void streson_set_fdbgain(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_streson* ug = (SP_streson*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->fdbgain = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_streson = { "Streson", sizeof(m_uint), &t_ugen };

static m_bool import_streson(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_streson))
  CHECK_BB(import_class_begin(env, &t_streson, env->global_nspc, streson_ctor, streson_dtor))
  env->class_def->doc = "String resonator filter";

  fun = new_DL_Func("float", "freq", (m_uint)streson_get_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Fundamental frequency of string.";

  fun = new_DL_Func("float", "freq", (m_uint)streson_set_freq);
    dl_func_add_arg(fun, "float", "freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Fundamental frequency of string.";

  fun = new_DL_Func("float", "fdbgain", (m_uint)streson_get_fdbgain);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Feedback amount (value between 0-1)";

  fun = new_DL_Func("float", "fdbgain", (m_uint)streson_set_fdbgain);
    dl_func_add_arg(fun, "float", "fdbgain");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Feedback amount (value between 0-1)";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_switch* osc;
} SP_switch;

static m_bool switch_tick(UGen u)
{
  SP_switch* ug = (SP_switch*)u->ug;
  M_Object l = (M_Object)u->channel[0];
  M_Object r = (M_Object)u->channel[1];
  ugen_compute(l->ugen);
  ugen_compute(l->ugen);
  ugen_compute(u->trig->ugen);
  sp_switch_compute(ug->sp, ug->osc, &l->ugen->in, &r->ugen->in, &u->trig->ugen->out, &u->out);
}

static void switch_ctor(M_Object obj, VM_Shred shred)
{
  SP_switch* ug = malloc(sizeof(SP_switch));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_switch_create(&ug->osc);
  sp_switch_init(ug->sp, ug->osc);
  obj->ugen->tick = switch_tick;
  assign_ugen(obj->ugen, 2, 1, 1, ug);
}

static void switch_dtor(M_Object obj, VM_Shred shred)
{
  SP_switch* ug = (SP_switch*)obj->ugen->ug;
  sp_switch_destroy(&ug->osc);
  free(ug);
}

struct Type_ t_switch = { "Switch", sizeof(m_uint), &t_ugen };

static m_bool import_switch(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_switch))
  CHECK_BB(import_class_begin(env, &t_switch, env->global_nspc, switch_ctor, switch_dtor))
  env->class_def->doc = "Switch between two signals    By default, the incoming first signal is selected. When triggered, the output signal will switch to the other signal.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_tabread* osc;
  char is_init;
  sp_ftbl*  ft;
  m_float   mode;
} SP_tabread;

static m_bool tabread_tick(UGen u)
{
  SP_tabread* ug = (SP_tabread*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  sp_tabread_compute(ug->sp, ug->osc, NULL, &u->out);
}

static void tabread_ctor(M_Object obj, VM_Shred shred)
{
  SP_tabread* ug = malloc(sizeof(SP_tabread));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_tabread_create(&ug->osc);
  obj->ugen->tick = tabread_tick;
  assign_ugen(obj->ugen, 0, 1, 0, ug);
}

static void tabread_dtor(M_Object obj, VM_Shred shred)
{
  SP_tabread* ug = (SP_tabread*)obj->ugen->ug;
  if(ug->is_init)
    sp_tabread_destroy(&ug->osc);
  if(ug->is_init)
    sp_ftbl_destroy(&ug->ft);
  free(ug);
}

static void tabread_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tabread* ug = (SP_tabread*)obj->ugen->ug;
  if(ug->is_init)
  tabread_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object ft_obj   = *(M_Object*)(shred->mem + offset);
  sp_ftbl* ft_ftbl  = FTBL(ft_obj);
  ug->ft            = ft_ftbl;
  offset += SZ_INT;
  m_float    mode   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_tabread_init(ug->sp, ug->osc, ft_ftbl, (ug->mode = mode));
  shred->reg  -= SZ_INT*1;
}

static void tabread_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tabread* ug = (SP_tabread*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  sp_ftbl* ft_ftbl = ug->ft = to_ftbl(*(M_Object*)(shred->mem + offset), shred);
  offset += SZ_INT;
  m_float    mode   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_tabread_init(ug->sp, ug->osc, ft_ftbl, (ug->mode = mode));
}

static void tabread_get_index(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tabread* ug = (SP_tabread*)obj->ugen->ug;
  RETURN->v_float = ug->osc->index;
}

static void tabread_set_index(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tabread* ug = (SP_tabread*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->index = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void tabread_get_offset(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tabread* ug = (SP_tabread*)obj->ugen->ug;
  RETURN->v_float = ug->osc->offset;
}

static void tabread_set_offset(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tabread* ug = (SP_tabread*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->offset = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void tabread_get_wrap(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tabread* ug = (SP_tabread*)obj->ugen->ug;
  RETURN->v_float = ug->osc->wrap;
}

static void tabread_set_wrap(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tabread* ug = (SP_tabread*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->wrap = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_tabread = { "Tabread", sizeof(m_uint), &t_ugen };

static m_bool import_tabread(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_tabread))
  CHECK_BB(import_class_begin(env, &t_tabread, env->global_nspc, tabread_ctor, tabread_dtor))
  env->class_def->doc = "Table         Read through an sp_ftbl with linear interpolation.    ";

  fun = new_DL_Func("void", "init", (m_uint)tabread_init);
   dl_func_add_arg(fun, "ftbl", "ft");
  f->doc = "A properly allocated table (using a function like sp_gen_file).";

   dl_func_add_arg(fun, "float", "mode");
  f->doc = "1 = scaled index, 0 = unscaled index";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)tabread_init_array);
   dl_func_add_arg(fun, "float[]", "ft");
  f->doc = "A properly allocated table (using a function like sp_gen_file).";

   dl_func_add_arg(fun, "float", "mode");
  f->doc = "1 = scaled index, 0 = unscaled index";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "index", (m_uint)tabread_get_index);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "index position, either scaled or unscaled with mode parameter";

  fun = new_DL_Func("float", "index", (m_uint)tabread_set_index);
    dl_func_add_arg(fun, "float", "index");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "index position, either scaled or unscaled with mode parameter";

  fun = new_DL_Func("float", "offset", (m_uint)tabread_get_offset);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Offset from beginning of ftable. If the mode is scaled, then it is in range 0-1, other wise it is the index position.";

  fun = new_DL_Func("float", "offset", (m_uint)tabread_set_offset);
    dl_func_add_arg(fun, "float", "offset");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Offset from beginning of ftable. If the mode is scaled, then it is in range 0-1, other wise it is the index position.";

  fun = new_DL_Func("float", "wrap", (m_uint)tabread_get_wrap);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Enable wraparound. 1 = on; 0 = 0ff.";

  fun = new_DL_Func("float", "wrap", (m_uint)tabread_set_wrap);
    dl_func_add_arg(fun, "float", "wrap");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Enable wraparound. 1 = on; 0 = 0ff.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_tadsr* osc;
} SP_tadsr;

static m_bool tadsr_tick(UGen u)
{
  SP_tadsr* ug = (SP_tadsr*)u->ug;
  ugen_compute(u->trig->ugen);
  sp_tadsr_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void tadsr_ctor(M_Object obj, VM_Shred shred)
{
  SP_tadsr* ug = malloc(sizeof(SP_tadsr));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_tadsr_create(&ug->osc);
  sp_tadsr_init(ug->sp, ug->osc);
  obj->ugen->tick = tadsr_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void tadsr_dtor(M_Object obj, VM_Shred shred)
{
  SP_tadsr* ug = (SP_tadsr*)obj->ugen->ug;
  sp_tadsr_destroy(&ug->osc);
  free(ug);
}

static void tadsr_get_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tadsr* ug = (SP_tadsr*)obj->ugen->ug;
  RETURN->v_float = ug->osc->atk;
}

static void tadsr_set_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tadsr* ug = (SP_tadsr*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->atk = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void tadsr_get_dec(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tadsr* ug = (SP_tadsr*)obj->ugen->ug;
  RETURN->v_float = ug->osc->dec;
}

static void tadsr_set_dec(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tadsr* ug = (SP_tadsr*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->dec = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void tadsr_get_sus(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tadsr* ug = (SP_tadsr*)obj->ugen->ug;
  RETURN->v_float = ug->osc->sus;
}

static void tadsr_set_sus(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tadsr* ug = (SP_tadsr*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->sus = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void tadsr_get_rel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tadsr* ug = (SP_tadsr*)obj->ugen->ug;
  RETURN->v_float = ug->osc->rel;
}

static void tadsr_set_rel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tadsr* ug = (SP_tadsr*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->rel = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_tadsr = { "Tadsr", sizeof(m_uint), &t_ugen };

static m_bool import_tadsr(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_tadsr))
  CHECK_BB(import_class_begin(env, &t_tadsr, env->global_nspc, tadsr_ctor, tadsr_dtor))
  env->class_def->doc = "Triggerable classic ADSR envelope";

  fun = new_DL_Func("float", "atk", (m_uint)tadsr_get_atk);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Attack time";

  fun = new_DL_Func("float", "atk", (m_uint)tadsr_set_atk);
    dl_func_add_arg(fun, "float", "atk");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Attack time";

  fun = new_DL_Func("float", "dec", (m_uint)tadsr_get_dec);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Decay time";

  fun = new_DL_Func("float", "dec", (m_uint)tadsr_set_dec);
    dl_func_add_arg(fun, "float", "dec");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Decay time";

  fun = new_DL_Func("float", "sus", (m_uint)tadsr_get_sus);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Sustain Level";

  fun = new_DL_Func("float", "sus", (m_uint)tadsr_set_sus);
    dl_func_add_arg(fun, "float", "sus");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Sustain Level";

  fun = new_DL_Func("float", "rel", (m_uint)tadsr_get_rel);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "release";

  fun = new_DL_Func("float", "rel", (m_uint)tadsr_set_rel);
    dl_func_add_arg(fun, "float", "rel");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "release";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_tblrec* osc;
  char is_init;
  sp_ftbl*  bar;
} SP_tblrec;

static m_bool tblrec_tick(UGen u)
{
  SP_tblrec* ug = (SP_tblrec*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  base_tick(u);
  ugen_compute(u->trig->ugen);
  sp_tblrec_compute(ug->sp, ug->osc, &u->in, &u->trig->ugen->out, &u->out);
}

static void tblrec_ctor(M_Object obj, VM_Shred shred)
{
  SP_tblrec* ug = malloc(sizeof(SP_tblrec));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_tblrec_create(&ug->osc);
  obj->ugen->tick = tblrec_tick;
  assign_ugen(obj->ugen, 1, 1, 1, ug);
}

static void tblrec_dtor(M_Object obj, VM_Shred shred)
{
  SP_tblrec* ug = (SP_tblrec*)obj->ugen->ug;
  if(ug->is_init)
    sp_tblrec_destroy(&ug->osc);
  if(ug->is_init)
    sp_ftbl_destroy(&ug->bar);
  free(ug);
}

static void tblrec_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tblrec* ug = (SP_tblrec*)obj->ugen->ug;
  if(ug->is_init)
  tblrec_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object bar_obj   = *(M_Object*)(shred->mem + offset);
  sp_ftbl* bar_ftbl  = FTBL(bar_obj);
  ug->bar            = bar_ftbl;
  offset += SZ_INT;
  sp_tblrec_init(ug->sp, ug->osc, bar_ftbl);
}

static void tblrec_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tblrec* ug = (SP_tblrec*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  sp_ftbl* bar_ftbl = ug->bar = to_ftbl(*(M_Object*)(shred->mem + offset), shred);
  offset += SZ_INT;
  sp_tblrec_init(ug->sp, ug->osc, bar_ftbl);
}

struct Type_ t_tblrec = { "Tblrec", sizeof(m_uint), &t_ugen };

static m_bool import_tblrec(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_tblrec))
  CHECK_BB(import_class_begin(env, &t_tblrec, env->global_nspc, tblrec_ctor, tblrec_dtor))
  env->class_def->doc = "Record a signal to an ftable.This module will write audio-rate signals to a preallocated soundpipe ftable. Every time record is enabled, it will got back to index 0 overwrite anyprevious information that was on it. ";

  fun = new_DL_Func("void", "init", (m_uint)tblrec_init);
   dl_func_add_arg(fun, "ftbl", "bar");
  f->doc = "bar";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)tblrec_init_array);
   dl_func_add_arg(fun, "float[]", "bar");
  f->doc = "bar";

  CHECK_BB((f = import_mfun(env, fun)))

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_tbvcf* osc;
} SP_tbvcf;

static m_bool tbvcf_tick(UGen u)
{
  SP_tbvcf* ug = (SP_tbvcf*)u->ug;
  base_tick(u);
  sp_tbvcf_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void tbvcf_ctor(M_Object obj, VM_Shred shred)
{
  SP_tbvcf* ug = malloc(sizeof(SP_tbvcf));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_tbvcf_create(&ug->osc);
  sp_tbvcf_init(ug->sp, ug->osc);
  obj->ugen->tick = tbvcf_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void tbvcf_dtor(M_Object obj, VM_Shred shred)
{
  SP_tbvcf* ug = (SP_tbvcf*)obj->ugen->ug;
  sp_tbvcf_destroy(&ug->osc);
  free(ug);
}

static void tbvcf_get_fco(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tbvcf* ug = (SP_tbvcf*)obj->ugen->ug;
  RETURN->v_float = ug->osc->fco;
}

static void tbvcf_set_fco(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tbvcf* ug = (SP_tbvcf*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->fco = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void tbvcf_get_res(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tbvcf* ug = (SP_tbvcf*)obj->ugen->ug;
  RETURN->v_float = ug->osc->res;
}

static void tbvcf_set_res(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tbvcf* ug = (SP_tbvcf*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->res = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void tbvcf_get_dist(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tbvcf* ug = (SP_tbvcf*)obj->ugen->ug;
  RETURN->v_float = ug->osc->dist;
}

static void tbvcf_set_dist(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tbvcf* ug = (SP_tbvcf*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->dist = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void tbvcf_get_asym(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tbvcf* ug = (SP_tbvcf*)obj->ugen->ug;
  RETURN->v_float = ug->osc->asym;
}

static void tbvcf_set_asym(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tbvcf* ug = (SP_tbvcf*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->asym = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_tbvcf = { "Tbvcf", sizeof(m_uint), &t_ugen };

static m_bool import_tbvcf(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_tbvcf))
  CHECK_BB(import_class_begin(env, &t_tbvcf, env->global_nspc, tbvcf_ctor, tbvcf_dtor))
  env->class_def->doc = "Emulation of the Roland TB-303 filter";

  fun = new_DL_Func("float", "fco", (m_uint)tbvcf_get_fco);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Filter cutoff frequency";

  fun = new_DL_Func("float", "fco", (m_uint)tbvcf_set_fco);
    dl_func_add_arg(fun, "float", "fco");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Filter cutoff frequency";

  fun = new_DL_Func("float", "res", (m_uint)tbvcf_get_res);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Resonance";

  fun = new_DL_Func("float", "res", (m_uint)tbvcf_set_res);
    dl_func_add_arg(fun, "float", "res");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Resonance";

  fun = new_DL_Func("float", "dist", (m_uint)tbvcf_get_dist);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Distortion. Value is typically 2.0; deviation from this can cause stability issues. ";

  fun = new_DL_Func("float", "dist", (m_uint)tbvcf_set_dist);
    dl_func_add_arg(fun, "float", "dist");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Distortion. Value is typically 2.0; deviation from this can cause stability issues. ";

  fun = new_DL_Func("float", "asym", (m_uint)tbvcf_get_asym);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Asymmetry of resonance. Value is between 0-1";

  fun = new_DL_Func("float", "asym", (m_uint)tbvcf_set_asym);
    dl_func_add_arg(fun, "float", "asym");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Asymmetry of resonance. Value is between 0-1";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_tdiv* osc;
} SP_tdiv;

static m_bool tdiv_tick(UGen u)
{
  SP_tdiv* ug = (SP_tdiv*)u->ug;
  ugen_compute(u->trig->ugen);
  sp_tdiv_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void tdiv_ctor(M_Object obj, VM_Shred shred)
{
  SP_tdiv* ug = malloc(sizeof(SP_tdiv));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_tdiv_create(&ug->osc);
  sp_tdiv_init(ug->sp, ug->osc);
  obj->ugen->tick = tdiv_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void tdiv_dtor(M_Object obj, VM_Shred shred)
{
  SP_tdiv* ug = (SP_tdiv*)obj->ugen->ug;
  sp_tdiv_destroy(&ug->osc);
  free(ug);
}

static void tdiv_get_num(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tdiv* ug = (SP_tdiv*)obj->ugen->ug;
  RETURN->v_float = ug->osc->num;
}

static void tdiv_set_num(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tdiv* ug = (SP_tdiv*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->num = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void tdiv_get_offset(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tdiv* ug = (SP_tdiv*)obj->ugen->ug;
  RETURN->v_float = ug->osc->offset;
}

static void tdiv_set_offset(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tdiv* ug = (SP_tdiv*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->offset = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_tdiv = { "Tdiv", sizeof(m_uint), &t_ugen };

static m_bool import_tdiv(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_tdiv))
  CHECK_BB(import_class_begin(env, &t_tdiv, env->global_nspc, tdiv_ctor, tdiv_dtor))
  env->class_def->doc = "Trigger divider.This module will take in a trigger signal, and output a trigger signalevery N times.For instance, when N = 3:in: * * * * * * * * *out *     *     *   ";

  fun = new_DL_Func("float", "num", (m_uint)tdiv_get_num);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Triggers every N times.";

  fun = new_DL_Func("float", "num", (m_uint)tdiv_set_num);
    dl_func_add_arg(fun, "float", "num");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Triggers every N times.";

  fun = new_DL_Func("float", "offset", (m_uint)tdiv_get_offset);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Offset amoutn.";

  fun = new_DL_Func("float", "offset", (m_uint)tdiv_set_offset);
    dl_func_add_arg(fun, "float", "offset");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Offset amoutn.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_tenv2* osc;
} SP_tenv2;

static m_bool tenv2_tick(UGen u)
{
  SP_tenv2* ug = (SP_tenv2*)u->ug;
  ugen_compute(u->trig->ugen);
  sp_tenv2_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void tenv2_ctor(M_Object obj, VM_Shred shred)
{
  SP_tenv2* ug = malloc(sizeof(SP_tenv2));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_tenv2_create(&ug->osc);
  sp_tenv2_init(ug->sp, ug->osc);
  obj->ugen->tick = tenv2_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void tenv2_dtor(M_Object obj, VM_Shred shred)
{
  SP_tenv2* ug = (SP_tenv2*)obj->ugen->ug;
  sp_tenv2_destroy(&ug->osc);
  free(ug);
}

static void tenv2_get_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenv2* ug = (SP_tenv2*)obj->ugen->ug;
  RETURN->v_float = ug->osc->atk * ug->sp->sr;
}

static void tenv2_set_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenv2* ug = (SP_tenv2*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->atk = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void tenv2_get_rel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenv2* ug = (SP_tenv2*)obj->ugen->ug;
  RETURN->v_float = ug->osc->rel * ug->sp->sr;
}

static void tenv2_set_rel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenv2* ug = (SP_tenv2*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->rel = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

struct Type_ t_tenv2 = { "Tenv2", sizeof(m_uint), &t_ugen };

static m_bool import_tenv2(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_tenv2))
  CHECK_BB(import_class_begin(env, &t_tenv2, env->global_nspc, tenv2_ctor, tenv2_dtor))
  env->class_def->doc = "Linear 2-stage Attack/Release envelope generator    This envelope takes 2 triggers. When triggered once,the envelope will rise to 1 according to the attack time. When triggered again, it will decay to 0 according tothe decay time.";

  fun = new_DL_Func("dur", "atk", (m_uint)tenv2_get_atk);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Attack time (in seconds).";

  fun = new_DL_Func("dur", "atk", (m_uint)tenv2_set_atk);
    dl_func_add_arg(fun, "dur", "atk");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Attack time (in seconds).";

  fun = new_DL_Func("dur", "rel", (m_uint)tenv2_get_rel);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Release time (in seconds).";

  fun = new_DL_Func("dur", "rel", (m_uint)tenv2_set_rel);
    dl_func_add_arg(fun, "dur", "rel");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Release time (in seconds).";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_tenv* osc;
} SP_tenv;

static m_bool tenv_tick(UGen u)
{
  SP_tenv* ug = (SP_tenv*)u->ug;
  ugen_compute(u->trig->ugen);
  sp_tenv_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void tenv_ctor(M_Object obj, VM_Shred shred)
{
  SP_tenv* ug = malloc(sizeof(SP_tenv));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_tenv_create(&ug->osc);
  sp_tenv_init(ug->sp, ug->osc);
  obj->ugen->tick = tenv_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void tenv_dtor(M_Object obj, VM_Shred shred)
{
  SP_tenv* ug = (SP_tenv*)obj->ugen->ug;
  sp_tenv_destroy(&ug->osc);
  free(ug);
}

static void tenv_get_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenv* ug = (SP_tenv*)obj->ugen->ug;
  RETURN->v_float = ug->osc->atk * ug->sp->sr;
}

static void tenv_set_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenv* ug = (SP_tenv*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->atk = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void tenv_get_hold(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenv* ug = (SP_tenv*)obj->ugen->ug;
  RETURN->v_float = ug->osc->hold * ug->sp->sr;
}

static void tenv_set_hold(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenv* ug = (SP_tenv*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->hold = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void tenv_get_rel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenv* ug = (SP_tenv*)obj->ugen->ug;
  RETURN->v_float = ug->osc->rel * ug->sp->sr;
}

static void tenv_set_rel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenv* ug = (SP_tenv*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->rel = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void tenv_get_sigmode(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenv* ug = (SP_tenv*)obj->ugen->ug;
  RETURN->v_uint = ug->osc->sigmode;
}

static void tenv_set_sigmode(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenv* ug = (SP_tenv*)obj->ugen->ug;
  RETURN->v_uint = (ug->osc->sigmode = *(m_int*)(shred->mem + SZ_INT));
}

static void tenv_get_input(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenv* ug = (SP_tenv*)obj->ugen->ug;
  RETURN->v_float = ug->osc->input;
}

static void tenv_set_input(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenv* ug = (SP_tenv*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->input = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_tenv = { "Tenv", sizeof(m_uint), &t_ugen };

static m_bool import_tenv(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_tenv))
  CHECK_BB(import_class_begin(env, &t_tenv, env->global_nspc, tenv_ctor, tenv_dtor))
  env->class_def->doc = "Trigger based linear AHD envelope generator";

  fun = new_DL_Func("dur", "atk", (m_uint)tenv_get_atk);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Attack time, in seconds.";

  fun = new_DL_Func("dur", "atk", (m_uint)tenv_set_atk);
    dl_func_add_arg(fun, "dur", "atk");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Attack time, in seconds.";

  fun = new_DL_Func("dur", "hold", (m_uint)tenv_get_hold);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Hold time, in seconds.";

  fun = new_DL_Func("dur", "hold", (m_uint)tenv_set_hold);
    dl_func_add_arg(fun, "dur", "hold");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Hold time, in seconds.";

  fun = new_DL_Func("dur", "rel", (m_uint)tenv_get_rel);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Release time, in seconds.";

  fun = new_DL_Func("dur", "rel", (m_uint)tenv_set_rel);
    dl_func_add_arg(fun, "dur", "rel");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Release time, in seconds.";

  fun = new_DL_Func("int", "sigmode", (m_uint)tenv_get_sigmode);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "If set to non-zero value, tenv will multiply the envelope with an internal signal instead of just returning an enveloped signal.";

  fun = new_DL_Func("int", "sigmode", (m_uint)tenv_set_sigmode);
    dl_func_add_arg(fun, "int", "sigmode");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "If set to non-zero value, tenv will multiply the envelope with an internal signal instead of just returning an enveloped signal.";

  fun = new_DL_Func("float", "input", (m_uint)tenv_get_input);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Internal input signal. If sigmode variable is set, it will multiply the envelope by this variable. Most of the time, this should be updated at audiorate.";

  fun = new_DL_Func("float", "input", (m_uint)tenv_set_input);
    dl_func_add_arg(fun, "float", "input");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Internal input signal. If sigmode variable is set, it will multiply the envelope by this variable. Most of the time, this should be updated at audiorate.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_tenvx* osc;
} SP_tenvx;

static m_bool tenvx_tick(UGen u)
{
  SP_tenvx* ug = (SP_tenvx*)u->ug;
  ugen_compute(u->trig->ugen);
  sp_tenvx_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void tenvx_ctor(M_Object obj, VM_Shred shred)
{
  SP_tenvx* ug = malloc(sizeof(SP_tenvx));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_tenvx_create(&ug->osc);
  sp_tenvx_init(ug->sp, ug->osc);
  obj->ugen->tick = tenvx_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void tenvx_dtor(M_Object obj, VM_Shred shred)
{
  SP_tenvx* ug = (SP_tenvx*)obj->ugen->ug;
  sp_tenvx_destroy(&ug->osc);
  free(ug);
}

static void tenvx_get_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenvx* ug = (SP_tenvx*)obj->ugen->ug;
  RETURN->v_float = ug->osc->atk * ug->sp->sr;
}

static void tenvx_set_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenvx* ug = (SP_tenvx*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->atk = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void tenvx_get_hold(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenvx* ug = (SP_tenvx*)obj->ugen->ug;
  RETURN->v_float = ug->osc->hold * ug->sp->sr;
}

static void tenvx_set_hold(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenvx* ug = (SP_tenvx*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->hold = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void tenvx_get_rel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenvx* ug = (SP_tenvx*)obj->ugen->ug;
  RETURN->v_float = ug->osc->rel * ug->sp->sr;
}

static void tenvx_set_rel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tenvx* ug = (SP_tenvx*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->rel = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

struct Type_ t_tenvx = { "Tenvx", sizeof(m_uint), &t_ugen };

static m_bool import_tenvx(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_tenvx))
  CHECK_BB(import_class_begin(env, &t_tenvx, env->global_nspc, tenvx_ctor, tenvx_dtor))
  env->class_def->doc = "Trigger based exponential AHD envelope generator.    This envelope generator emulates the exponential behavior of analogue envelope generators by passing a gate signal (whose duration is specified viathe hold parameter) through a one-pole filter, whose filter coefficeints arecalculated in terms of tau.      ";

  fun = new_DL_Func("dur", "atk", (m_uint)tenvx_get_atk);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Tau attack time, in seconds. Must be non-zero.";

  fun = new_DL_Func("dur", "atk", (m_uint)tenvx_set_atk);
    dl_func_add_arg(fun, "dur", "atk");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Tau attack time, in seconds. Must be non-zero.";

  fun = new_DL_Func("dur", "hold", (m_uint)tenvx_get_hold);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Hold time, in seconds. The duration of the gate signal.";

  fun = new_DL_Func("dur", "hold", (m_uint)tenvx_set_hold);
    dl_func_add_arg(fun, "dur", "hold");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Hold time, in seconds. The duration of the gate signal.";

  fun = new_DL_Func("dur", "rel", (m_uint)tenvx_get_rel);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Tau release time, in seconds. Must be non-zero.";

  fun = new_DL_Func("dur", "rel", (m_uint)tenvx_set_rel);
    dl_func_add_arg(fun, "dur", "rel");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Tau release time, in seconds. Must be non-zero.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_thresh* osc;
} SP_thresh;

static m_bool thresh_tick(UGen u)
{
  SP_thresh* ug = (SP_thresh*)u->ug;
  base_tick(u);
  sp_thresh_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void thresh_ctor(M_Object obj, VM_Shred shred)
{
  SP_thresh* ug = malloc(sizeof(SP_thresh));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_thresh_create(&ug->osc);
  sp_thresh_init(ug->sp, ug->osc);
  obj->ugen->tick = thresh_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void thresh_dtor(M_Object obj, VM_Shred shred)
{
  SP_thresh* ug = (SP_thresh*)obj->ugen->ug;
  sp_thresh_destroy(&ug->osc);
  free(ug);
}

static void thresh_get_thresh(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_thresh* ug = (SP_thresh*)obj->ugen->ug;
  RETURN->v_float = ug->osc->thresh;
}

static void thresh_set_thresh(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_thresh* ug = (SP_thresh*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->thresh = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void thresh_get_mode(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_thresh* ug = (SP_thresh*)obj->ugen->ug;
  RETURN->v_uint = ug->osc->mode;
}

static void thresh_set_mode(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_thresh* ug = (SP_thresh*)obj->ugen->ug;
  RETURN->v_uint = (ug->osc->mode = *(m_int*)(shred->mem + SZ_INT));
}

struct Type_ t_thresh = { "Thresh", sizeof(m_uint), &t_ugen };

static m_bool import_thresh(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_thresh))
  CHECK_BB(import_class_begin(env, &t_thresh, env->global_nspc, thresh_ctor, thresh_dtor))
  env->class_def->doc = "Trigger generator for signals that cross a given threshold. ";

  fun = new_DL_Func("float", "thresh", (m_uint)thresh_get_thresh);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Threshold level";

  fun = new_DL_Func("float", "thresh", (m_uint)thresh_set_thresh);
    dl_func_add_arg(fun, "float", "thresh");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Threshold level";

  fun = new_DL_Func("int", "mode", (m_uint)thresh_get_mode);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Sets when to detect theshold crossings. 0 = from below. 1 = from above. 2 = from above/below";

  fun = new_DL_Func("int", "mode", (m_uint)thresh_set_mode);
    dl_func_add_arg(fun, "int", "mode");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Sets when to detect theshold crossings. 0 = from below. 1 = from above. 2 = from above/below";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_timer* osc;
} SP_timer;

static m_bool timer_tick(UGen u)
{
  SP_timer* ug = (SP_timer*)u->ug;
  ugen_compute(u->trig->ugen);
  sp_timer_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void timer_ctor(M_Object obj, VM_Shred shred)
{
  SP_timer* ug = malloc(sizeof(SP_timer));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_timer_create(&ug->osc);
  sp_timer_init(ug->sp, ug->osc);
  obj->ugen->tick = timer_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void timer_dtor(M_Object obj, VM_Shred shred)
{
  SP_timer* ug = (SP_timer*)obj->ugen->ug;
  sp_timer_destroy(&ug->osc);
  free(ug);
}

struct Type_ t_timer = { "Timer", sizeof(m_uint), &t_ugen };

static m_bool import_timer(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_timer))
  CHECK_BB(import_class_begin(env, &t_timer, env->global_nspc, timer_ctor, timer_dtor))
  env->class_def->doc = "Tap-tempo like timerWhen triggered, timer will begin an internal stopwatch until it is triggered again.The output of the timer will be the time elapsed in seconds.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_tin* osc;
} SP_tin;

static m_bool tin_tick(UGen u)
{
  SP_tin* ug = (SP_tin*)u->ug;
  ugen_compute(u->trig->ugen);
  sp_tin_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void tin_ctor(M_Object obj, VM_Shred shred)
{
  SP_tin* ug = malloc(sizeof(SP_tin));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_tin_create(&ug->osc);
  sp_tin_init(ug->sp, ug->osc);
  obj->ugen->tick = tin_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void tin_dtor(M_Object obj, VM_Shred shred)
{
  SP_tin* ug = (SP_tin*)obj->ugen->ug;
  sp_tin_destroy(&ug->osc);
  free(ug);
}

struct Type_ t_tin = { "Tin", sizeof(m_uint), &t_ugen };

static m_bool import_tin(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_tin))
  CHECK_BB(import_class_begin(env, &t_tin, env->global_nspc, tin_ctor, tin_dtor))
  env->class_def->doc = "Similar to in, tin reads SPFLOATs (by default, this is a 4 byte binary float) from standard input every time it is triggered. behaves like a sample and hold, retaining the previous value (initial set to 0) until triggered. ";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_tone* osc;
} SP_tone;

static m_bool tone_tick(UGen u)
{
  SP_tone* ug = (SP_tone*)u->ug;
  base_tick(u);
  sp_tone_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void tone_ctor(M_Object obj, VM_Shred shred)
{
  SP_tone* ug = malloc(sizeof(SP_tone));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_tone_create(&ug->osc);
  sp_tone_init(ug->sp, ug->osc);
  obj->ugen->tick = tone_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void tone_dtor(M_Object obj, VM_Shred shred)
{
  SP_tone* ug = (SP_tone*)obj->ugen->ug;
  sp_tone_destroy(&ug->osc);
  free(ug);
}

static void tone_get_hp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tone* ug = (SP_tone*)obj->ugen->ug;
  RETURN->v_float = ug->osc->hp;
}

static void tone_set_hp(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tone* ug = (SP_tone*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->hp = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_tone = { "Tone", sizeof(m_uint), &t_ugen };

static m_bool import_tone(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_tone))
  CHECK_BB(import_class_begin(env, &t_tone, env->global_nspc, tone_ctor, tone_dtor))
  env->class_def->doc = "First-order recursive lowpass filter";

  fun = new_DL_Func("float", "hp", (m_uint)tone_get_hp);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The response curve's half power point (aka cutoff frequency).";

  fun = new_DL_Func("float", "hp", (m_uint)tone_set_hp);
    dl_func_add_arg(fun, "float", "hp");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The response curve's half power point (aka cutoff frequency).";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_trand* osc;
} SP_trand;

static m_bool trand_tick(UGen u)
{
  SP_trand* ug = (SP_trand*)u->ug;
  ugen_compute(u->trig->ugen);
  sp_trand_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void trand_ctor(M_Object obj, VM_Shred shred)
{
  SP_trand* ug = malloc(sizeof(SP_trand));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_trand_create(&ug->osc);
  sp_trand_init(ug->sp, ug->osc);
  obj->ugen->tick = trand_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void trand_dtor(M_Object obj, VM_Shred shred)
{
  SP_trand* ug = (SP_trand*)obj->ugen->ug;
  sp_trand_destroy(&ug->osc);
  free(ug);
}

static void trand_get_min(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_trand* ug = (SP_trand*)obj->ugen->ug;
  RETURN->v_float = ug->osc->min;
}

static void trand_set_min(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_trand* ug = (SP_trand*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->min = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void trand_get_max(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_trand* ug = (SP_trand*)obj->ugen->ug;
  RETURN->v_float = ug->osc->max;
}

static void trand_set_max(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_trand* ug = (SP_trand*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->max = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_trand = { "Trand", sizeof(m_uint), &t_ugen };

static m_bool import_trand(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_trand))
  CHECK_BB(import_class_begin(env, &t_trand, env->global_nspc, trand_ctor, trand_dtor))
  env->class_def->doc = "Triggered random number generator.";

  fun = new_DL_Func("float", "min", (m_uint)trand_get_min);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Minimum value to use.";

  fun = new_DL_Func("float", "min", (m_uint)trand_set_min);
    dl_func_add_arg(fun, "float", "min");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Minimum value to use.";

  fun = new_DL_Func("float", "max", (m_uint)trand_get_max);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Maximum value to use.";

  fun = new_DL_Func("float", "max", (m_uint)trand_set_max);
    dl_func_add_arg(fun, "float", "max");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Maximum value to use.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_tseg* osc;
  char is_init;
  m_float   ibeg;
} SP_tseg;

static m_bool tseg_tick(UGen u)
{
  SP_tseg* ug = (SP_tseg*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  ugen_compute(u->trig->ugen);
  sp_tseg_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void tseg_ctor(M_Object obj, VM_Shred shred)
{
  SP_tseg* ug = malloc(sizeof(SP_tseg));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_tseg_create(&ug->osc);
  obj->ugen->tick = tseg_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void tseg_dtor(M_Object obj, VM_Shred shred)
{
  SP_tseg* ug = (SP_tseg*)obj->ugen->ug;
  if(ug->is_init)
    sp_tseg_destroy(&ug->osc);
  free(ug);
}

static void tseg_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tseg* ug = (SP_tseg*)obj->ugen->ug;
  if(ug->is_init)
  tseg_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    ibeg   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_tseg_init(ug->sp, ug->osc, (ug->ibeg = ibeg));
}

static void tseg_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tseg* ug = (SP_tseg*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    ibeg   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_tseg_init(ug->sp, ug->osc, (ug->ibeg = ibeg));
}

static void tseg_get_end(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tseg* ug = (SP_tseg*)obj->ugen->ug;
  RETURN->v_float = ug->osc->end;
}

static void tseg_set_end(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tseg* ug = (SP_tseg*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->end = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void tseg_get_dur(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tseg* ug = (SP_tseg*)obj->ugen->ug;
  RETURN->v_float = ug->osc->dur * ug->sp->sr;
}

static void tseg_set_dur(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tseg* ug = (SP_tseg*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->dur = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

static void tseg_get_type(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tseg* ug = (SP_tseg*)obj->ugen->ug;
  RETURN->v_float = ug->osc->type;
}

static void tseg_set_type(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tseg* ug = (SP_tseg*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->type = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_tseg = { "Tseg", sizeof(m_uint), &t_ugen };

static m_bool import_tseg(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_tseg))
  CHECK_BB(import_class_begin(env, &t_tseg, env->global_nspc, tseg_ctor, tseg_dtor))
  env->class_def->doc = "This module creates a series of line segments. ";

  fun = new_DL_Func("void", "init", (m_uint)tseg_init);
   dl_func_add_arg(fun, "float", "ibeg");
  f->doc = "Beginning value.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)tseg_init_array);
   dl_func_add_arg(fun, "float", "ibeg");
  f->doc = "Beginning value.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "end", (m_uint)tseg_get_end);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "End parameter to go to.";

  fun = new_DL_Func("float", "end", (m_uint)tseg_set_end);
    dl_func_add_arg(fun, "float", "end");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "End parameter to go to.";

  fun = new_DL_Func("dur", "dur", (m_uint)tseg_get_dur);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "duration to rise to (in seconds).";

  fun = new_DL_Func("dur", "dur", (m_uint)tseg_set_dur);
    dl_func_add_arg(fun, "dur", "dur");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "duration to rise to (in seconds).";

  fun = new_DL_Func("float", "type", (m_uint)tseg_get_type);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The type of line, which determines slope of line";

  fun = new_DL_Func("float", "type", (m_uint)tseg_set_type);
    dl_func_add_arg(fun, "float", "type");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "The type of line, which determines slope of line";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_tseq* osc;
  char is_init;
  sp_ftbl*  ft;
} SP_tseq;

static m_bool tseq_tick(UGen u)
{
  SP_tseq* ug = (SP_tseq*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  ugen_compute(u->trig->ugen);
  sp_tseq_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
}

static void tseq_ctor(M_Object obj, VM_Shred shred)
{
  SP_tseq* ug = malloc(sizeof(SP_tseq));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_tseq_create(&ug->osc);
  obj->ugen->tick = tseq_tick;
  assign_ugen(obj->ugen, 0, 1, 1, ug);
}

static void tseq_dtor(M_Object obj, VM_Shred shred)
{
  SP_tseq* ug = (SP_tseq*)obj->ugen->ug;
  if(ug->is_init)
    sp_tseq_destroy(&ug->osc);
  if(ug->is_init)
    sp_ftbl_destroy(&ug->ft);
  free(ug);
}

static void tseq_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tseq* ug = (SP_tseq*)obj->ugen->ug;
  if(ug->is_init)
  tseq_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  M_Object ft_obj   = *(M_Object*)(shred->mem + offset);
  sp_ftbl* ft_ftbl  = FTBL(ft_obj);
  ug->ft            = ft_ftbl;
  offset += SZ_INT;
  sp_tseq_init(ug->sp, ug->osc, ft_ftbl);
}

static void tseq_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tseq* ug = (SP_tseq*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  sp_ftbl* ft_ftbl = ug->ft = to_ftbl(*(M_Object*)(shred->mem + offset), shred);
  offset += SZ_INT;
  sp_tseq_init(ug->sp, ug->osc, ft_ftbl);
}

static void tseq_get_shuf(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tseq* ug = (SP_tseq*)obj->ugen->ug;
  RETURN->v_uint = ug->osc->shuf;
}

static void tseq_set_shuf(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_tseq* ug = (SP_tseq*)obj->ugen->ug;
  RETURN->v_uint = (ug->osc->shuf = *(m_int*)(shred->mem + SZ_INT));
}

struct Type_ t_tseq = { "Tseq", sizeof(m_uint), &t_ugen };

static m_bool import_tseq(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_tseq))
  CHECK_BB(import_class_begin(env, &t_tseq, env->global_nspc, tseq_ctor, tseq_dtor))
  env->class_def->doc = "Function table looper    TSeq runs through values in an ftable. It will change values when the trigger input is a non-zero value, and wrap around when it reaches the end.";

  fun = new_DL_Func("void", "init", (m_uint)tseq_init);
   dl_func_add_arg(fun, "ftbl", "ft");
  f->doc = "An ftable of values";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)tseq_init_array);
   dl_func_add_arg(fun, "float[]", "ft");
  f->doc = "An ftable of values";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("int", "shuf", (m_uint)tseq_get_shuf);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "When shuf is non-zero, randomly pick a value rather than cycle through sequentially.";

  fun = new_DL_Func("int", "shuf", (m_uint)tseq_set_shuf);
    dl_func_add_arg(fun, "int", "shuf");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "When shuf is non-zero, randomly pick a value rather than cycle through sequentially.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_vdelay* osc;
  char is_init;
  m_float     maxdel;
} SP_vdelay;

static m_bool vdelay_tick(UGen u)
{
  SP_vdelay* ug = (SP_vdelay*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  base_tick(u);
  sp_vdelay_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void vdelay_ctor(M_Object obj, VM_Shred shred)
{
  SP_vdelay* ug = malloc(sizeof(SP_vdelay));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_vdelay_create(&ug->osc);
  obj->ugen->tick = vdelay_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void vdelay_dtor(M_Object obj, VM_Shred shred)
{
  SP_vdelay* ug = (SP_vdelay*)obj->ugen->ug;
  if(ug->is_init)
    sp_vdelay_destroy(&ug->osc);
  free(ug);
}

static void vdelay_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_vdelay* ug = (SP_vdelay*)obj->ugen->ug;
  if(ug->is_init)
  vdelay_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    maxdel   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_vdelay_init(ug->sp, ug->osc, maxdel / (m_float)ug->sp->sr);
}

static void vdelay_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_vdelay* ug = (SP_vdelay*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    maxdel   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_vdelay_init(ug->sp, ug->osc, maxdel / (m_float)ug->sp->sr);
}

static void vdelay_get_del(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_vdelay* ug = (SP_vdelay*)obj->ugen->ug;
  RETURN->v_float = ug->osc->del * ug->sp->sr;
}

static void vdelay_set_del(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_vdelay* ug = (SP_vdelay*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->del = (*(m_float*)(shred->mem + SZ_FLOAT) / (m_float)ug->sp->sr));
}

struct Type_ t_vdelay = { "Vdelay", sizeof(m_uint), &t_ugen };

static m_bool import_vdelay(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_vdelay))
  CHECK_BB(import_class_begin(env, &t_vdelay, env->global_nspc, vdelay_ctor, vdelay_dtor))
  env->class_def->doc = "Delay line with cubic interpolation";

  fun = new_DL_Func("void", "init", (m_uint)vdelay_init);
   dl_func_add_arg(fun, "dur", "maxdel");
  f->doc = "The maximum delay time, in seconds.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)vdelay_init_array);
   dl_func_add_arg(fun, "dur", "maxdel");
  f->doc = "The maximum delay time, in seconds.";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("dur", "del", (m_uint)vdelay_get_del);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Delay time (in seconds) that can be changed during performance. This value must not exceed the maximum delay time.";

  fun = new_DL_Func("dur", "del", (m_uint)vdelay_set_del);
    dl_func_add_arg(fun, "dur", "del");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Delay time (in seconds) that can be changed during performance. This value must not exceed the maximum delay time.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_vocoder* osc;
} SP_vocoder;

static m_bool vocoder_tick(UGen u)
{
  SP_vocoder* ug = (SP_vocoder*)u->ug;
  base_tick(u);
  ugen_compute(u->trig->ugen);
  sp_vocoder_compute(ug->sp, ug->osc, &u->in, &u->trig->ugen->out, &u->out);
}

static void vocoder_ctor(M_Object obj, VM_Shred shred)
{
  SP_vocoder* ug = malloc(sizeof(SP_vocoder));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_vocoder_create(&ug->osc);
  sp_vocoder_init(ug->sp, ug->osc);
  obj->ugen->tick = vocoder_tick;
  assign_ugen(obj->ugen, 1, 1, 1, ug);
}

static void vocoder_dtor(M_Object obj, VM_Shred shred)
{
  SP_vocoder* ug = (SP_vocoder*)obj->ugen->ug;
  sp_vocoder_destroy(&ug->osc);
  free(ug);
}

static void vocoder_get_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_vocoder* ug = (SP_vocoder*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->atk;
}

static void vocoder_set_atk(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_vocoder* ug = (SP_vocoder*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->atk = *(m_float*)(shred->mem + SZ_INT));
}

static void vocoder_get_rel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_vocoder* ug = (SP_vocoder*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->rel;
}

static void vocoder_set_rel(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_vocoder* ug = (SP_vocoder*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->rel = *(m_float*)(shred->mem + SZ_INT));
}

static void vocoder_get_bwratio(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_vocoder* ug = (SP_vocoder*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->bwratio;
}

static void vocoder_set_bwratio(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_vocoder* ug = (SP_vocoder*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->bwratio = *(m_float*)(shred->mem + SZ_INT));
}

struct Type_ t_vocoder = { "Vocoder", sizeof(m_uint), &t_ugen };

static m_bool import_vocoder(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_vocoder))
  CHECK_BB(import_class_begin(env, &t_vocoder, env->global_nspc, vocoder_ctor, vocoder_dtor))
  env->class_def->doc = "16-band channel vocoder";

  fun = new_DL_Func("float", "atk", (m_uint)vocoder_get_atk);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Attack time . (Range 0.001 and 0.5 seconds)";

  fun = new_DL_Func("float", "atk", (m_uint)vocoder_set_atk);
    dl_func_add_arg(fun, "float", "atk");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Attack time . (Range 0.001 and 0.5 seconds)";

  fun = new_DL_Func("float", "rel", (m_uint)vocoder_get_rel);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Release time";

  fun = new_DL_Func("float", "rel", (m_uint)vocoder_set_rel);
    dl_func_add_arg(fun, "float", "rel");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Release time";

  fun = new_DL_Func("float", "bwratio", (m_uint)vocoder_get_bwratio);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Coeffecient to adjust the bandwidth of each band (0.1 - 2)";

  fun = new_DL_Func("float", "bwratio", (m_uint)vocoder_set_bwratio);
    dl_func_add_arg(fun, "float", "bwratio");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Coeffecient to adjust the bandwidth of each band (0.1 - 2)";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_waveset* osc;
  char is_init;
  m_float     ilen;
} SP_waveset;

static m_bool waveset_tick(UGen u)
{
  SP_waveset* ug = (SP_waveset*)u->ug;
  if(!ug->is_init)
{
    u->out = 0;
  return 1;
 }
  base_tick(u);
  sp_waveset_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void waveset_ctor(M_Object obj, VM_Shred shred)
{
  SP_waveset* ug = malloc(sizeof(SP_waveset));
  ug->is_init = 0;
  ug->sp = shred->vm_ref->bbq->sp;
  sp_waveset_create(&ug->osc);
  obj->ugen->tick = waveset_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void waveset_dtor(M_Object obj, VM_Shred shred)
{
  SP_waveset* ug = (SP_waveset*)obj->ugen->ug;
  if(ug->is_init)
    sp_waveset_destroy(&ug->osc);
  free(ug);
}

static void waveset_init(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_waveset* ug = (SP_waveset*)obj->ugen->ug;
  if(ug->is_init)
  waveset_ctor(obj, shred);  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    ilen   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_waveset_init(ug->sp, ug->osc, ilen / (m_float)ug->sp->sr);
}

static void waveset_init_array(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_waveset* ug = (SP_waveset*)obj->ugen->ug;
  ug->is_init = 1;
  int offset = SZ_INT;
  m_float    ilen   = *(m_float*)(shred->mem + offset);
  offset += SZ_FLOAT;
  sp_waveset_init(ug->sp, ug->osc, ilen / (m_float)ug->sp->sr);
}

static void waveset_get_rep(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_waveset* ug = (SP_waveset*)obj->ugen->ug;
  RETURN->v_float = ug->osc->rep;
}

static void waveset_set_rep(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_waveset* ug = (SP_waveset*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->rep = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_waveset = { "Waveset", sizeof(m_uint), &t_ugen };

static m_bool import_waveset(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_waveset))
  CHECK_BB(import_class_begin(env, &t_waveset, env->global_nspc, waveset_ctor, waveset_dtor))
  env->class_def->doc = "Simple Time-stretching from repeating wavecylesThis module looks for zero-crossings and repeats them by a integer factor.While a crude means for time stretching, it is a very aesthetically pleasing effect to use on sounds and often produces at \"wet\" sound.The waveset algorithm was originally created by Trevor Wishart for the ComposerDesktop Project (CDP), and was then ported to Csound. ";

  fun = new_DL_Func("void", "init", (m_uint)waveset_init);
   dl_func_add_arg(fun, "dur", "ilen");
  f->doc = "Length of buffer (in seconds).";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("void", "init", (m_uint)waveset_init_array);
   dl_func_add_arg(fun, "dur", "ilen");
  f->doc = "Length of buffer (in seconds).";

  CHECK_BB((f = import_mfun(env, fun)))

  fun = new_DL_Func("float", "rep", (m_uint)waveset_get_rep);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Number of repeats.";

  fun = new_DL_Func("float", "rep", (m_uint)waveset_set_rep);
    dl_func_add_arg(fun, "float", "rep");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Number of repeats.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_wpkorg35* osc;
} SP_wpkorg35;

static m_bool wpkorg35_tick(UGen u)
{
  SP_wpkorg35* ug = (SP_wpkorg35*)u->ug;
  base_tick(u);
  sp_wpkorg35_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static void wpkorg35_ctor(M_Object obj, VM_Shred shred)
{
  SP_wpkorg35* ug = malloc(sizeof(SP_wpkorg35));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_wpkorg35_create(&ug->osc);
  sp_wpkorg35_init(ug->sp, ug->osc);
  obj->ugen->tick = wpkorg35_tick;
  assign_ugen(obj->ugen, 1, 1, 0, ug);
}

static void wpkorg35_dtor(M_Object obj, VM_Shred shred)
{
  SP_wpkorg35* ug = (SP_wpkorg35*)obj->ugen->ug;
  sp_wpkorg35_destroy(&ug->osc);
  free(ug);
}

static void wpkorg35_get_cutoff(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_wpkorg35* ug = (SP_wpkorg35*)obj->ugen->ug;
  RETURN->v_float = ug->osc->cutoff;
}

static void wpkorg35_set_cutoff(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_wpkorg35* ug = (SP_wpkorg35*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->cutoff = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void wpkorg35_get_res(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_wpkorg35* ug = (SP_wpkorg35*)obj->ugen->ug;
  RETURN->v_float = ug->osc->res;
}

static void wpkorg35_set_res(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_wpkorg35* ug = (SP_wpkorg35*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->res = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

static void wpkorg35_get_saturation(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_wpkorg35* ug = (SP_wpkorg35*)obj->ugen->ug;
  RETURN->v_float = ug->osc->saturation;
}

static void wpkorg35_set_saturation(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_wpkorg35* ug = (SP_wpkorg35*)obj->ugen->ug;
  RETURN->v_float = (ug->osc->saturation = (*(m_float*)(shred->mem + SZ_FLOAT)));
}

struct Type_ t_wpkorg35 = { "Wpkorg35", sizeof(m_uint), &t_ugen };

static m_bool import_wpkorg35(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_wpkorg35))
  CHECK_BB(import_class_begin(env, &t_wpkorg35, env->global_nspc, wpkorg35_ctor, wpkorg35_dtor))
  env->class_def->doc = " Analogue model of the Korg 35 Lowpass FilterOriginal port done by Will Pirkle:http://www.willpirkle.com/Downloads/AN-5Korg35_V3.pdf";

  fun = new_DL_Func("float", "cutoff", (m_uint)wpkorg35_get_cutoff);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Filter cutoff";

  fun = new_DL_Func("float", "cutoff", (m_uint)wpkorg35_set_cutoff);
    dl_func_add_arg(fun, "float", "cutoff");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Filter cutoff";

  fun = new_DL_Func("float", "res", (m_uint)wpkorg35_get_res);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Filter resonance (should be between 0-2)";

  fun = new_DL_Func("float", "res", (m_uint)wpkorg35_set_res);
    dl_func_add_arg(fun, "float", "res");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Filter resonance (should be between 0-2)";

  fun = new_DL_Func("float", "saturation", (m_uint)wpkorg35_get_saturation);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Filter saturation.";

  fun = new_DL_Func("float", "saturation", (m_uint)wpkorg35_set_saturation);
    dl_func_add_arg(fun, "float", "saturation");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Filter saturation.";

  CHECK_BB(import_class_end(env))
  return 1;
}
typedef struct
{
  sp_data* sp;
  sp_zitarev* osc;
} SP_zitarev;

static m_bool zitarev_tick(UGen u)
{
  SP_zitarev* ug = (SP_zitarev*)u->ug;
  M_Object l = (M_Object)u->channel[0];
  M_Object r = (M_Object)u->channel[1];
  ugen_compute(l->ugen);
  ugen_compute(l->ugen);
  sp_zitarev_compute(ug->sp, ug->osc, &l->ugen->in, &r->ugen->in, &l->ugen->out, &r->ugen->out);
}

static void zitarev_ctor(M_Object obj, VM_Shred shred)
{
  SP_zitarev* ug = malloc(sizeof(SP_zitarev));
  ug->sp = shred->vm_ref->bbq->sp;
  sp_zitarev_create(&ug->osc);
  sp_zitarev_init(ug->sp, ug->osc);
  obj->ugen->tick = zitarev_tick;
  assign_ugen(obj->ugen, 2, 2, 0, ug);
}

static void zitarev_dtor(M_Object obj, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  sp_zitarev_destroy(&ug->osc);
  free(ug);
}

static void zitarev_get_in_delay(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->in_delay;
}

static void zitarev_set_in_delay(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->in_delay = *(m_float*)(shred->mem + SZ_INT));
}

static void zitarev_get_lf_x(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->lf_x;
}

static void zitarev_set_lf_x(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->lf_x = *(m_float*)(shred->mem + SZ_INT));
}

static void zitarev_get_rt60_low(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->rt60_low;
}

static void zitarev_set_rt60_low(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->rt60_low = *(m_float*)(shred->mem + SZ_INT));
}

static void zitarev_get_rt60_mid(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->rt60_mid;
}

static void zitarev_set_rt60_mid(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->rt60_mid = *(m_float*)(shred->mem + SZ_INT));
}

static void zitarev_get_hf_damping(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->hf_damping;
}

static void zitarev_set_hf_damping(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->hf_damping = *(m_float*)(shred->mem + SZ_INT));
}

static void zitarev_get_eq1_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->eq1_freq;
}

static void zitarev_set_eq1_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->eq1_freq = *(m_float*)(shred->mem + SZ_INT));
}

static void zitarev_get_eq1_level(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->eq1_level;
}

static void zitarev_set_eq1_level(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->eq1_level = *(m_float*)(shred->mem + SZ_INT));
}

static void zitarev_get_eq2_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->eq2_freq;
}

static void zitarev_set_eq2_freq(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->eq2_freq = *(m_float*)(shred->mem + SZ_INT));
}

static void zitarev_get_eq2_level(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->eq2_level;
}

static void zitarev_set_eq2_level(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->eq2_level = *(m_float*)(shred->mem + SZ_INT));
}

static void zitarev_get_mix(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->mix;
}

static void zitarev_set_mix(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->mix = *(m_float*)(shred->mem + SZ_INT));
}

static void zitarev_get_level(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = *ug->osc->level;
}

static void zitarev_set_level(M_Object obj, DL_Return * RETURN, VM_Shred shred)
{
  SP_zitarev* ug = (SP_zitarev*)obj->ugen->ug;
  RETURN->v_float = (*ug->osc->level = *(m_float*)(shred->mem + SZ_INT));
}

struct Type_ t_zitarev = { "Zitarev", sizeof(m_uint), &t_ugen };

static m_bool import_zitarev(Env env)
{
  DL_Func* fun;
  Func     f;
  CHECK_BB(add_global_type(env, &t_zitarev))
  CHECK_BB(import_class_begin(env, &t_zitarev, env->global_nspc, zitarev_ctor, zitarev_dtor))
  env->class_def->doc = "8 FDN stereo zitareverb algorithm, imported from Faust.";

  fun = new_DL_Func("float", "in_delay", (m_uint)zitarev_get_in_delay);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Delay in ms before reverberation begins.";

  fun = new_DL_Func("float", "in_delay", (m_uint)zitarev_set_in_delay);
    dl_func_add_arg(fun, "float", "in_delay");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Delay in ms before reverberation begins.";

  fun = new_DL_Func("float", "lf_x", (m_uint)zitarev_get_lf_x);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Crossover frequency separating low and middle frequencies (Hz).";

  fun = new_DL_Func("float", "lf_x", (m_uint)zitarev_set_lf_x);
    dl_func_add_arg(fun, "float", "lf_x");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Crossover frequency separating low and middle frequencies (Hz).";

  fun = new_DL_Func("float", "rt60_low", (m_uint)zitarev_get_rt60_low);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Time (in seconds) to decay 60db in low-frequency band.";

  fun = new_DL_Func("float", "rt60_low", (m_uint)zitarev_set_rt60_low);
    dl_func_add_arg(fun, "float", "rt60_low");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Time (in seconds) to decay 60db in low-frequency band.";

  fun = new_DL_Func("float", "rt60_mid", (m_uint)zitarev_get_rt60_mid);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Time (in seconds) to decay 60db in mid-frequency band.";

  fun = new_DL_Func("float", "rt60_mid", (m_uint)zitarev_set_rt60_mid);
    dl_func_add_arg(fun, "float", "rt60_mid");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Time (in seconds) to decay 60db in mid-frequency band.";

  fun = new_DL_Func("float", "hf_damping", (m_uint)zitarev_get_hf_damping);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency (Hz) at which the high-frequency T60 is half the middle-band's T60.";

  fun = new_DL_Func("float", "hf_damping", (m_uint)zitarev_set_hf_damping);
    dl_func_add_arg(fun, "float", "hf_damping");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Frequency (Hz) at which the high-frequency T60 is half the middle-band's T60.";

  fun = new_DL_Func("float", "eq1_freq", (m_uint)zitarev_get_eq1_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Center frequency of second-order Regalia Mitra peaking equalizer section 1.";

  fun = new_DL_Func("float", "eq1_freq", (m_uint)zitarev_set_eq1_freq);
    dl_func_add_arg(fun, "float", "eq1_freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Center frequency of second-order Regalia Mitra peaking equalizer section 1.";

  fun = new_DL_Func("float", "eq1_level", (m_uint)zitarev_get_eq1_level);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Peak level in dB of second-order Regalia-Mitra peaking equalizer section 1";

  fun = new_DL_Func("float", "eq1_level", (m_uint)zitarev_set_eq1_level);
    dl_func_add_arg(fun, "float", "eq1_level");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Peak level in dB of second-order Regalia-Mitra peaking equalizer section 1";

  fun = new_DL_Func("float", "eq2_freq", (m_uint)zitarev_get_eq2_freq);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Center frequency of second-order Regalia Mitra peaking equalizer section 2.";

  fun = new_DL_Func("float", "eq2_freq", (m_uint)zitarev_set_eq2_freq);
    dl_func_add_arg(fun, "float", "eq2_freq");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Center frequency of second-order Regalia Mitra peaking equalizer section 2.";

  fun = new_DL_Func("float", "eq2_level", (m_uint)zitarev_get_eq2_level);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Peak level in dB of second-order Regalia-Mitra peaking equalizer section 2";

  fun = new_DL_Func("float", "eq2_level", (m_uint)zitarev_set_eq2_level);
    dl_func_add_arg(fun, "float", "eq2_level");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Peak level in dB of second-order Regalia-Mitra peaking equalizer section 2";

  fun = new_DL_Func("float", "mix", (m_uint)zitarev_get_mix);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "0 = all dry, 1 = all wet";

  fun = new_DL_Func("float", "mix", (m_uint)zitarev_set_mix);
    dl_func_add_arg(fun, "float", "mix");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "0 = all dry, 1 = all wet";

  fun = new_DL_Func("float", "level", (m_uint)zitarev_get_level);
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Output scale factor (in dB).";

  fun = new_DL_Func("float", "level", (m_uint)zitarev_set_level);
    dl_func_add_arg(fun, "float", "level");
  CHECK_BB((f = import_mfun(env, fun)))
  f->doc = "Output scale factor (in dB).";

  CHECK_BB(import_class_end(env))
  return 1;
}
m_bool import_soundpipe(Env env)
{

  CHECK_BB(import_ftbl(env))
  CHECK_BB(import_adsr(env))
  CHECK_BB(import_allpass(env))
  CHECK_BB(import_atone(env))
  CHECK_BB(import_autowah(env))
  CHECK_BB(import_bal(env))
  CHECK_BB(import_bar(env))
  CHECK_BB(import_biquad(env))
  CHECK_BB(import_biscale(env))
  CHECK_BB(import_bitcrush(env))
  CHECK_BB(import_blsaw(env))
  CHECK_BB(import_blsquare(env))
  CHECK_BB(import_bltriangle(env))
  CHECK_BB(import_butbp(env))
  CHECK_BB(import_butbr(env))
  CHECK_BB(import_buthp(env))
  CHECK_BB(import_butlp(env))
  CHECK_BB(import_clip(env))
  CHECK_BB(import_comb(env))
  CHECK_BB(import_compressor(env))
  CHECK_BB(import_conv(env))
  CHECK_BB(import_count(env))
  CHECK_BB(import_crossfade(env))
  CHECK_BB(import_dcblock(env))
  CHECK_BB(import_delay(env))
  CHECK_BB(import_diskin(env))
  CHECK_BB(import_dist(env))
  CHECK_BB(import_dmetro(env))
  CHECK_BB(import_drip(env))
  CHECK_BB(import_dtrig(env))
  CHECK_BB(import_dust(env))
  CHECK_BB(import_eqfil(env))
  CHECK_BB(import_expon(env))
  CHECK_BB(import_fof(env))
  CHECK_BB(import_fofilt(env))
  CHECK_BB(import_fog(env))
  CHECK_BB(import_fold(env))
  CHECK_BB(import_fosc(env))
  CHECK_BB(import_gbuzz(env))
  CHECK_BB(import_hilbert(env))
  CHECK_BB(import_in(env))
  CHECK_BB(import_jcrev(env))
  CHECK_BB(import_jitter(env))
  CHECK_BB(import_line(env))
  CHECK_BB(import_lpf18(env))
  CHECK_BB(import_maygate(env))
  CHECK_BB(import_metro(env))
  CHECK_BB(import_mincer(env))
  CHECK_BB(import_mode(env))
  CHECK_BB(import_moogladder(env))
  CHECK_BB(import_noise(env))
  CHECK_BB(import_nsmp(env))
  CHECK_BB(import_osc(env))
  CHECK_BB(import_oscmorph(env))
  CHECK_BB(import_pan2(env))
  CHECK_BB(import_panst(env))
  CHECK_BB(import_pareq(env))
  CHECK_BB(import_paulstretch(env))
  CHECK_BB(import_pdhalf(env))
  CHECK_BB(import_peaklim(env))
  CHECK_BB(import_phaser(env))
  CHECK_BB(import_phasor(env))
  CHECK_BB(import_pinknoise(env))
  CHECK_BB(import_pitchamdf(env))
  CHECK_BB(import_pluck(env))
  CHECK_BB(import_port(env))
  CHECK_BB(import_posc3(env))
  CHECK_BB(import_prop(env))
  CHECK_BB(import_pshift(env))
  CHECK_BB(import_ptrack(env))
  CHECK_BB(import_randh(env))
  CHECK_BB(import_randi(env))
  CHECK_BB(import_random(env))
  CHECK_BB(import_reson(env))
  CHECK_BB(import_reverse(env))
  CHECK_BB(import_revsc(env))
  CHECK_BB(import_rms(env))
  CHECK_BB(import_rpt(env))
  CHECK_BB(import_samphold(env))
  CHECK_BB(import_scale(env))
  CHECK_BB(import_sdelay(env))
  CHECK_BB(import_streson(env))
  CHECK_BB(import_switch(env))
  CHECK_BB(import_tabread(env))
  CHECK_BB(import_tadsr(env))
  CHECK_BB(import_tblrec(env))
  CHECK_BB(import_tbvcf(env))
  CHECK_BB(import_tdiv(env))
  CHECK_BB(import_tenv2(env))
  CHECK_BB(import_tenv(env))
  CHECK_BB(import_tenvx(env))
  CHECK_BB(import_thresh(env))
  CHECK_BB(import_timer(env))
  CHECK_BB(import_tin(env))
  CHECK_BB(import_tone(env))
  CHECK_BB(import_trand(env))
  CHECK_BB(import_tseg(env))
  CHECK_BB(import_tseq(env))
  CHECK_BB(import_vdelay(env))
  CHECK_BB(import_vocoder(env))
  CHECK_BB(import_waveset(env))
  CHECK_BB(import_wpkorg35(env))
  CHECK_BB(import_zitarev(env))
  return 1;
}
