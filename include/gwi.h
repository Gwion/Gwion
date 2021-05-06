#ifndef __GWI
#define __GWI

#ifdef GWION_DOC
#include "gwfmt.h"
#define gwiheader(a,...) do { lint_nl(a->lint); lint_indent(a->lint); lint(a->lint, "{-}#!+ {/}%s{0}\n", __VA_ARGS__); } while(0)
#define gwidoc(a,...)    do { lint_nl(a->lint); lint_indent(a->lint); lint(a->lint, "{-}#!- {/}%s{0}\n", __VA_ARGS__); } while(0)
#define gwinote(a,...)   do { lint_indent(a->lint); lint(a->lint, "{-}#!- {/}%s{0}\n", __VA_ARGS__); } while(0)
#else
#define gwiheader(a,...)
#define gwidoc(a,...)
#define gwinote(a,...)
#endif
struct Gwi_ {
  struct Gwion_ *const gwion;
  Ast body;
  struct ImportCK *ck;
  struct OperCK *oper; // _misc
  struct Vector_ effects;
  loc_t loc;
#ifdef GWION_DOC
  Lint *lint;
#endif
  uint tmpls;
};

#include "import/internals.h"
static inline Tmpl* gwi_tmpl(const Gwi gwi) {
  return new_tmpl_base(gwi->gwion->mp, gwi->ck->tmpl);
}

ANN void gwi_effects(const Gwi gwi, const m_str name);
ANN m_bool gwi_run(const Gwion gwion, m_bool (*f)(const Gwi));
#endif
