#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "gwion.h"
#include "memoize.h"

struct Memoize_ {
  struct Vector_ v;
  m_uint         arg_sz;
  m_uint         ret_sz;
  size_t         limit;
  size_t         curr;
};

Memoize memoize_ini(const Emitter emit, const Func f) {
  Memoize m = mp_calloc(emit->gwion->mp, Memoize);
  vector_init(&m->v);
  m->ret_sz = f->def->base->ret_type->size;
  m->arg_sz = f->def->stack_depth;
  m->limit  = f->memoize;
  return m;
}

void memoize_end(MemPool p, Memoize m) {
  for (m_uint i = 0; i < vector_size(&m->v); ++i)
    mp_free2(p, m->arg_sz + m->arg_sz, (void *)vector_at(&m->v, i));
  vector_release(&m->v);
  mp_free(p, Memoize, m);
}

ANN static inline m_bit *get_data(MemPool mp, Memoize m) {
  if (vector_size(&m->v) >= m->limit)
    return (m_bit *)vector_at(&m->v, m->curr++ % m->limit);
  m_bit *data = mp_calloc2(mp, m->arg_sz + m->ret_sz);
  vector_add(&m->v, (vtype)data);
  return data;
}

INSTR(MemoizeStore) {
  const Memoize m    = shred->code->memoize;
  m_bit *       data = get_data(shred->info->mp, m);
  memcpy(data, shred->mem, m->arg_sz);
  memcpy(data + m->arg_sz, shred->reg - m->ret_sz, m->ret_sz);
}

INSTR(MemoizeIni) {
  const m_uint  idx = *(m_uint *)MEM(instr->m_val);
  const Memoize m   = shred->code->memoize;
  if (idx < VLEN(&m->v)) {
    const m_bit *data = (m_bit *)vector_at(&m->v, idx);
    memcpy(MEM(instr->m_val + SZ_INT * 2), data, instr->m_val2);
  } else
    shred->pc = *(m_uint *)MEM(instr->m_val + SZ_INT);
}
