#ifndef __UGEN
#define __UGEN
typedef struct UGen_ *UGen;
typedef void (*f_tick)(const UGen ug) ANN;
// typedef void (*f_ugop)(const UGen, const m_float) ANN;

struct ugen_net_ {
  struct Vector_ from;
  struct Vector_ to;
  uint16_t       size;
};

struct ugen_multi_ {
  M_Object *channel;
  uint8_t   n_in;
  uint8_t   n_out;
  uint8_t   n_chan;
};

struct ugen_gen {
  f_tick tick;
  void * data;
  UGen   trig;
};

struct UGen_ {
  f_tick compute;
  union {
    struct ugen_net_ *  net;
    struct ugen_multi_ *multi;
  } connect;
  union {
    struct ugen_gen gen;
    UGen            ref;
  } module;
  m_float in, out;
  uint8_t op;
  bool    multi;
  bool    done;
};

#define TICK(a) __attribute__((hot)) ANN inline void a(const UGen u)

ANN void ugen_ini(const struct Gwion_ *, const UGen, const uint, const uint);
ANN void ugen_gen(const struct Gwion_ *, const UGen, const f_tick, void *,
                  const m_bool);
ANN void ugen_connect(const UGen lhs, const UGen rhs);
ANN void ugen_disconnect(const UGen lhs, const UGen rhs);
#endif
