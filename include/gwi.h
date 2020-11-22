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

ANN static inline m_bool gwi_doc(const Gwi gwi, const m_str name, const m_str doc) {
  DECL_OB(const Value, v, = nspc_lookup_value0(gwi->gwion->env->curr, insert_symbol(gwi->gwion->st, name)))
  v->docstr = new_text(gwi->gwion->mp);
  text_add(v->docstr, doc);
}
#endif
