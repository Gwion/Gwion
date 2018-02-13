typedef void (*f_tick)(UGen ug);

struct UGen_ {
  m_uint n_in, n_out, n_chan;
  m_float in, out, last;
  m_int op;
//  union {
  struct Vector_ ugen, to; // store connected UGens
  M_Object* channel;
//  };
  M_Object trig;
  f_tick tick;
  UGen ref;
  void* ug;
  m_bool done;
};

m_bool assign_ugen(UGen u, m_uint n_in, m_uint n_out, m_bool trig, void* ug);
void ugen_compute(UGen u);

TICK(dac_tick);
TICK(adc_tick);
TICK(base_tick);
TICK(blackhole_tick);
