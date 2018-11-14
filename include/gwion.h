#ifndef __GWION
#define __GWION
struct Gwion_ {
  Scanner scan
  Env     env
  Emit    emit
  VM      vm
};
#endif
