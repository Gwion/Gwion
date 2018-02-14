typedef void (*f_ugop)(UGen, const m_float);
typedef void (*f_tick)(UGen ug);

struct UGen_ {
  m_uint n_in, n_out, n_chan;
  m_float in, out, last;
//  m_int op;
  f_ugop op;
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

static inline void ugop_plus   (UGen u, m_float f) { u->in += f; }
static inline void ugop_minus  (UGen u, m_float f) { u->in -= f; }
static inline void ugop_times  (UGen u, m_float f) { u->in *= f; }
static inline void ugop_divide (UGen u, m_float f) { u->in /= f; }

m_bool assign_ugen(UGen u, m_uint n_in, m_uint n_out, m_bool trig, void* ug);
void ugen_compute(const UGen u);
#ifndef TICK
#define TICK(a) inline void a(UGen)
#endif
TICK(base_tick);
