#ifndef __GWION
#define __GWION

typedef struct Gwion_* Gwion;
#include "plug.h"
#include "driver.h"
#include "gwiondata.h"
struct Arg_;

struct Gwion_ {
  Env      env;
  Emitter  emit;
  VM*      vm;
  struct GwionData_ *data;
  SymTable *st;
  MemPool mp;
  struct PPArg_ *ppa;
  Type *type;
};

ANN m_bool gwion_ini(const Gwion, struct Arg_*);
ANN VM* gwion_cpy(const VM*);
ANN void gwion_run(const Gwion gwion);
ANN void gwion_end(const Gwion gwion);
void free_code_instr(const Vector v, const Gwion gwion);
#endif
