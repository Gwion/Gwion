#ifndef __GWION
#define __GWION
typedef struct Gwion_* Gwion;
#include "plug.h"
struct Gwion_ {
// Vector args
// PlugInfo
// => Vector v[GWION_NVEC];
  PlugInfo plug;
// sym
// mem
// rnd
// dl
  Env      env;
  Emitter  emit;
  VM*      vm;
};
ANN void gwion_init(const Gwion, const Vector);
ANN void gwion_release(const Gwion gwion);
#endif
