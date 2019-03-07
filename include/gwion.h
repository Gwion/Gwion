#ifndef __GWION
#define __GWION

struct Arg_;
typedef struct Gwion_* Gwion;
#include "plug.h"
#include "driver.h"
struct Gwion_ {
  PlugInfo* plug;
  DriverInfo* di;
//  struct Arg* arg;

// sym
// mem
  Env      env;
  Emitter  emit;
  VM*      vm;
};
ANN void gwion_init(const Gwion, struct Arg_*);
ANN m_bool gwion_audio(const Gwion gwion);
ANN void gwion_run(const Gwion gwion);
ANN m_bool gwion_engine(const Gwion gwion);
ANN void gwion_release(const Gwion gwion);
#endif
