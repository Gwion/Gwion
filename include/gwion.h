#ifndef __GWION
#define __GWION
typedef struct Gwion_* Gwion;
#include "plug.h"
#include "driver.h"
struct Gwion_ {
  PlugInfo* plug;
  Driver* driver;
// scanner
// sym
// mem
// rnd
// dl
  Env      env;
  Emitter  emit;
  VM*      vm;
};
ANN void gwion_init(const Gwion, const Vector);
ANN m_bool gwion_audio(const Gwion gwion, DriverInfo* di);
ANN void gwion_run(const Gwion gwion, DriverInfo* di);
ANN m_bool gwion_engine(const Gwion gwion);
ANN void gwion_release(const Gwion gwion, DriverInfo* di);
#endif
