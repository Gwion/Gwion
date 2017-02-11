//typedef m_bool (*f_tick) (UGen ug, m_float *in, m_float *out);
typedef m_bool (*f_tick) (UGen ug);
//typedef enum { e_is_multi, e_is_chan, e_is_none } e_ugen_type;
//typedef enum { e_chan_in, e_chan_out, e_chan_none } e_chan_type;

struct UGen_ {
  m_bool done;
  m_uint n_in, n_out, n_chan;
//  m_float *in, *out;
  m_float in, out, last;
//  m_float last;
  m_int op;
//  union {
  Vector ugen, to; // store connected UGens
  M_Object* channel;
//  };
//  e_ugen_type type;
  M_Object trig;
  f_tick tick;
  UGen ref; // for stereo ?

  void* ug;
};

//static UGen new_UGen();
void ugen_ctor(M_Object o, VM_Shred shred);
void ugen_dtor(M_Object o, VM_Shred shred);
m_bool import_ugen(Env env);
m_bool import_modules(Env env); // additionnal UGen modules
m_bool assign_ugen(UGen u, m_uint n_in, m_uint n_out, m_bool trig, void* ug);
void ugen_compute(UGen u);

m_bool dac_tick(UGen u);
m_bool adc_tick(UGen u);
m_bool base_tick(UGen u);
m_bool blackhole_tick(UGen u);
