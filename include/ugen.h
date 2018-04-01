#define TICK(a) __attribute__((hot)) ANN inline void a(const UGen u)
typedef void (*f_ugop)(const UGen, const m_float) ANN;
typedef void (*f_tick)(const UGen ug) ANN;

struct UGen_ {
  m_uint n_in, n_out, n_chan;
  m_float in, out, last;
  f_ugop op;
//  union {
  struct Vector_ from, to; // store connected UGens
  M_Object* channel;
//  };
  UGen trig;
  f_tick compute;
  f_tick tick;
  UGen ref;
  void* ug;
  m_bool done;
};

ANN static inline void ugop_plus   (const UGen u, const m_float f) { u->in += f; }
ANN static inline void ugop_minus  (const UGen u, const m_float f) { u->in -= f; }
ANN static inline void ugop_times  (const UGen u, const m_float f) { u->in *= f; }
ANN static inline void ugop_divide (const UGen u, const m_float f) { u->in /= f; }

ANN2(1) void assign_ugen(const UGen, const m_uint, const m_uint, void* ug);
ANN void assign_trig(UGen);
//ANN void ugen_compute(const UGen u) __attribute__((hot));
