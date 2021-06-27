#ifndef __GWION
#define __GWION

// typedef struct Gwion_* Gwion;
#include "plug.h"
#include "driver.h"
#include "gwiondata.h"
struct Arg_;

struct Gwion_ {
  MemPool            mp;
  VM *               vm;
  SymTable *         st;
  Env                env;
  Emitter            emit;
  struct GwionData_ *data;
  Type *             type;
  struct PPArg_ *    ppa;
};

ANN m_bool gwion_ini(const Gwion, struct Arg_ *);
ANN VM * gwion_cpy(const VM *);
ANN void gwion_run(const Gwion gwion);
ANN void gwion_end(const Gwion gwion);
void     free_code_instr(const Vector v, const Gwion gwion);
ANN void gwion_end_child(const VM_Shred shred, const Gwion gwion);
ANN void push_global(const Gwion gwion, const m_str name);
ANN Nspc pop_global(const Gwion gwion);
__attribute__((returns_nonnull)) ANN static inline Value
type_value(const Gwion gwion, const Type t) {
  return (Value)nspc_lookup_value1(t->info->value->from->owner,
                                   insert_symbol(gwion->st, t->name));
}
__attribute__((returns_nonnull)) ANN static inline Type
type_class(const Gwion gwion, const Type t) {
  const Value v = nspc_lookup_value1(t->info->value->from->owner,
                                     insert_symbol(gwion->st, t->name));
  return v->type;
}

ANN void gwion_set_debug(const Gwion gwion, const bool dbg);
ANN void gwion_set_dump(const Gwion gwion, const bool dump);
ANN void gwion_set_cdoc(const Gwion gwion, const bool dbg);
#endif
