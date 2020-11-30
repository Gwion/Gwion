#ifndef __GWI
#define __GWI

#define loc(gwi) loc_cpy(gwi->gwion->mp, gwi->loc)

struct Gwi_ {
  struct Gwion_ *const gwion;
  Ast body;
  struct ImportCK *ck;
  struct OperCK *oper; // _misc
  loc_t loc;
};

#include "import/internals.h"
static inline Tmpl* gwi_tmpl(const Gwi gwi) {
  return new_tmpl_base(gwi->gwion->mp, gwi->ck->tmpl);
}

ANN m_bool gwi_run(const Gwion gwion, m_bool (*f)(const Gwi));
#endif
