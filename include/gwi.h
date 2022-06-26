#ifndef __GWI
#define __GWI

#include "gwfmt.h"
#define gwiheader(a, ...)                                                      \
  if (a->gwion->data->cdoc) do {                                               \
      lint_nl(gwi->lint);                                                      \
      lint_indent(gwi->lint);                                                  \
      lint(gwi->lint, (m_str)"{-}#!+ {/}%s{0}\n", __VA_ARGS__);                \
  } while (0)
#define gwidoc(a, ...)                                                         \
  if (a->gwion->data->cdoc) do {                                               \
      lint_nl(a->lint);                                                        \
      lint_indent(a->lint);                                                    \
      lint(a->lint, (m_str)"{-}#!- {/}%s{0}\n", __VA_ARGS__);                  \
  } while (0)
#define gwinote(a, ...)                                                        \
  if (a->gwion->data->cdoc) do {                                               \
      lint_indent(a->lint);                                                    \
      lint(a->lint, (m_str)"{-}#!- {/}%s{0}\n", __VA_ARGS__);                  \
  } while (0)

struct Gwi_ {
  struct Gwion_ *const gwion;
  Ast                  body;
  struct ImportCK *    ck;
  struct OperCK *      oper; // _misc
  struct Vector_       effects;
  loc_t                loc;
  Lint *               lint;
  uint8_t              tmpls;
};

#include "import/internals.h"
static inline Tmpl *gwi_tmpl(const Gwi gwi) {
  return new_tmpl(gwi->gwion->mp, gwi->ck->sl);
}

ANN void   gwi_effects(const Gwi gwi, const m_str name);
ANN m_bool gwi_run(const Gwion gwion, m_bool (*f)(const Gwi));
#endif
