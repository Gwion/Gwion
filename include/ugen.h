#define TICK(a) __attribute__((hot)) ANN inline void a(const UGen u)
typedef void (*f_tick)(const UGen ug) ANN;
typedef void (*f_ugop)(const UGen, const m_float) ANN;

struct ugen_net {
  struct Vector_ from;
  struct Vector_ to;
  m_uint size;
};

struct ugen_multi_ {
  M_Object* channel;
  m_uint    n_in;
  m_uint    n_out;
  m_uint    n_chan;
};

struct ugen_gen {
  f_tick tick;
  void*  data;
  UGen   trig;
};

enum ugen_flag { UGEN_MULTI = 1 << 1 };
struct UGen_ {
  f_tick compute;
  f_ugop op;
  union {
    struct ugen_net net;
    struct ugen_multi_* multi;
  } connect;
  union {
    struct ugen_gen gen;
    UGen ref;
  } module;
  m_float in, out;
  m_bool done;
  enum ugen_flag flag;
};
ANN void ugen_ini(const UGen, const m_uint, const m_uint);
ANN void ugen_gen(const UGen, const f_tick, void*, const m_bool);
