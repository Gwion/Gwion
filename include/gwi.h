#ifndef __GWI
#define __GWI

#include "gwfmt.h"
#define gwiheader(a, ...)                                                      \
  if (a->gwion->data->cdoc) do {                                               \
      gwfmt_nl(gwi->gwfmt);                                                      \
      gwfmt_indent(gwi->gwfmt);                                                  \
      gwfmt_util(gwi->gwfmt, (m_str)"{-}#!+ {/}%s{0}\n", __VA_ARGS__);                \
  } while (0)
#define gwidoc(a, ...)                                                         \
  if (a->gwion->data->cdoc) do {                                               \
      gwfmt_nl(a->gwfmt);                                                        \
      gwfmt_indent(a->gwfmt);                                                    \
      gwfmt_util(a->gwfmt, (m_str)"{-}#!- {/}%s{0}\n", __VA_ARGS__);                  \
  } while (0)
#define gwinote(a, ...)                                                        \
  if (a->gwion->data->cdoc) do {                                               \
      gwfmt_indent(a->gwfmt);                                                    \
      gwfmt_util(a->gwfmt, (m_str)"{-}#!- {/}%s{0}\n", __VA_ARGS__);                  \
  } while (0)

struct Gwi_ {
  struct Gwion_ *const gwion;
  Ast                  body;
  struct ImportCK *    ck;
  struct OperCK *      oper; // _misc
  struct Vector_       effects;
  loc_t                loc;
  Gwfmt *              gwfmt;
  uint8_t              tmpls;
};

#include "import/internals.h"
static inline Tmpl *gwi_tmpl(const Gwi gwi) {
  return new_tmpl(gwi->gwion->mp, gwi->ck->sl);
}

ANN void gwi_effects(const Gwi gwi, const m_str name);
ANN bool gwi_run(const Gwion gwion, bool (*f)(const Gwi));
#endif
