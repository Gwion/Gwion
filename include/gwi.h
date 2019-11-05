#ifndef __GWI
#define __GWI

#define loc(gwi) loc_cpy(gwi->gwion->mp, gwi->loc)

struct Gwi_ {
  struct Gwion_* gwion;
  Class_Body body;
  struct ImportCK *ck;
  struct OperCK *oper; // _misc
  loc_t loc;
};

#include "import/internals.h"
static inline Tmpl* gwi_tmpl(const Gwi gwi) {
  return new_tmpl_base(gwi->gwion->mp, gwi->ck->tmpl);
}
#endif
