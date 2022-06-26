#ifndef __GWION_ENV
#define __GWION_ENV

#include "env/oo.h"
#include "env/trait.h"
#include "env/nspc.h"
#include "env/env.h"

#include "env/value.h"
#include "env/type.h"
#include "env/func.h"
#include "env/context.h"
#include "env/tuple.h"
#include "env/envset.h"

ANN2(1,4) static inline void gwerr_basic_from(const m_str msg, const m_str explain,
                                              const m_str fix, const ValueFrom *from,
                                              const uint code) {
  gwerr_basic(msg, explain, fix, from->filename, from->loc, code);
}
ANN static inline void gwerr_secondary_from(const m_str msg, const ValueFrom *from) {
  gwerr_secondary(msg, from->filename, from->loc);
}
#endif
