#ifndef __GWION
#define __GWION
typedef struct Gwion_* Gwion;
struct Gwion_ {
// Vector args
// PlugInfo
// => Vector v[GWION_NVEC];
// sym
// mem
// rnd
// dl
  Env      env;
  Emitter  emit;
  VM*      vm;
};
ANN void gwion_init(Gwion gwion);
ANN void gwion_release(Gwion gwion);
#endif
