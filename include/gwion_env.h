#ifndef __GWION_ENV
#define __GWION_ENV

#include "env/oo.h"

typedef struct Value_ *Value;
typedef struct Func_  *Func;
MK_VECTOR_TYPE(Func, func)
typedef struct Trait_ *Trait;
#include "env/nspc.h"
#include "env/env.h"

#include "env/value.h"
#include "env/type.h"
#include "env/func.h"
#include "env/trait.h"
#include "env/context.h"
#include "env/tuple.h"
#include "env/envset.h"

ANN2(1,3) static inline void gwlog_error_from(const m_str msg, const m_str explain,
                                              const ValueFrom *from,
                                              const uint code) {
  gwlog_error(msg, explain, from->filename, from->loc, code);
}
ANN static inline void gwlog_warning_from(const m_str msg, const ValueFrom *from) {
  gwlog_warning(msg, from->filename, from->loc);
}
ANN static inline void gwlog_related_from(const m_str msg, const ValueFrom *from) {
  gwlog_related(msg, from->filename, from->loc);
}

ANN static inline void declared_here(const Value v) {
  gwlog_related_from((m_str)"declared here", v->from);
}

ANN static inline void defined_here(const Value v) {
  if (v->from->filename) {// TODO: check why is that from check
    char c[256];
    c[255] = '\0';
    snprintf(c, 256, _("%.*s defined here"), 240, v->name);
    gwlog_related_from(c, v->from);
  }
}


#define is_new(a) !strcmp(s_name((a)->base->tag.sym), "new")
#endif

