typedef m_bool (*f_tick) (UGen ug);

struct UGen_ {
  m_uint n_in, n_out, n_chan;
  m_float in, out, last;
  m_int op;
//  union {
  Vector ugen, to; // store connected UGens
  M_Object* channel;
//  };
  M_Object trig;
  f_tick tick;
  UGen ref;
  void* ug;
  m_bool done;
};

m_bool import_ugen(Env env);
m_bool import_modules(Env env); // additionnal UGen modules
m_bool assign_ugen(UGen u, m_uint n_in, m_uint n_out, m_bool trig, void* ug);
void ugen_compute(UGen u);

m_bool dac_tick(UGen u);
m_bool adc_tick(UGen u);
m_bool base_tick(UGen u);
m_bool blackhole_tick(UGen u);
