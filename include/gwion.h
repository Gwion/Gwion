#ifndef __GWION
#define __GWION

struct Arg_;
typedef struct Gwion_* Gwion;
#include "plug.h"
#include "driver.h"
struct Gwion_ {
  PlugInfo* plug;
  DriverInfo* di;
  struct Arg_* arg;
// sym
// mem
  Env      env;
  Emitter  emit;
  VM*      vm;
};
ANN m_bool gwion_ini(const Gwion, int, char**);
ANN void   gwion_run(const Gwion gwion);
ANN void   gwion_end(const Gwion gwion);
#endif
