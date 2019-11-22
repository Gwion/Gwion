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
  m_uint arg_sz;
  m_uint ret_sz;
  struct pool *p;
  size_t limit;
  size_t curr;
  m_bool member;
  enum Kind kind;
};

static inline void memoize_return1(m_bit* tgt, const m_bit* src,
  const m_uint size NUSED) {
  *(m_uint*)tgt = *(m_uint*)src;
}

static inline void memoize_return2(m_bit* tgt, const m_bit* src,
  const m_uint size NUSED) {
  *(m_float*)tgt = *(m_float*)src;
}

static inline void memoize_return3(m_bit* tgt, const m_bit* src,
  const m_uint size) {
  memcpy(tgt, src, size);
}

static inline void memoize_return4(m_bit* tgt NUSED,
  const m_bit* src NUSED,
  const m_uint size NUSED) {}

static void(*mreturn[])(m_bit*, const m_bit*, const m_uint) =
  { memoize_return1, memoize_return2, memoize_return3, memoize_return4};

Memoize memoize_ini(const Emitter emit, const Func f, const enum Kind kind) {
  Memoize m = mp_calloc(emit->gwion->mp, Memoize);
  vector_init(&m->v);
  m->ret_sz = f->def->base->ret_type->size;
  m->kind = kind;
  if(!GET_FLAG(f, member))
    m->arg_sz = f->def->stack_depth;
  else {
    m->member = SZ_INT;
    m->arg_sz = f->def->stack_depth - SZ_INT;
  }
  m->limit = emit->info->memoize;
  m->p = new_pool((uint32_t)(m->arg_sz + m->ret_sz));
  return m;
}

void memoize_end(MemPool p, Memoize m) {
  vector_release(&m->v);
  mp_end(m->p);
  xfree(m->p);
  mp_free(p, Memoize, m);
}

static inline void memoize_set(Memoize m, const m_bit* arg) {
  m_bit* data = _mp_calloc2(m->p, 0);
  memcpy(data, arg, m->arg_sz);
  if(vector_size(&m->v) < m->limit)
    vector_add(&m->v, (vtype)data);
  else
    vector_set(&m->v, m->curr++ % m->limit, (vtype)data);
}

m_bool memoize_get(VM_Shred shred) {
  const VM_Code code = *(VM_Code*)REG(-SZ_INT);
  const Memoize m = code->memoize;
  if(!m)
    return GW_OK;
  const m_bit* arg = REG(-(SZ_INT + m->arg_sz + (m_uint)m->member));
  const m_uint size = vector_size(&m->v);
  for(m_uint i = 0; i < size; ++i) {
    m_bit* data = (m_bit*)vector_at(&m->v, i);
    if(memcmp(arg, data, m->arg_sz))
      continue;
    POP_REG(shred, SZ_INT + (m->arg_sz - m->ret_sz) + (m_uint)m->member)
    mreturn[m->kind](shred->reg-m->ret_sz, data + m->arg_sz, m->ret_sz);
    return GW_OK;
  }
  memoize_set(m, arg);
  return 0;
}

INSTR(MemoizeCall) {
  if(memoize_get(shred))
    shred->pc += instr->m_val;
}

INSTR(MemoizeStore) {
  const Memoize m = shred->code->memoize;
  const m_bit* arg = MEM(m->member);
  const m_uint size = vector_size(&m->v);
  for(m_uint i = size + 1; --i;) {
    m_bit* data = (m_bit*)vector_at(&m->v, i-1);
    if(memcmp(data, arg, m->arg_sz))
      continue;
    mreturn[m->kind](data + m->arg_sz, shred->reg-m->ret_sz, m->ret_sz);
    return;
  }
}
