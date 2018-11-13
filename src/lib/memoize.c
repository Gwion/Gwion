#include <string.h>
#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "mpool.h"
#include "oo.h"
#include "vm.h"
#include "type.h"
#include "instr.h"
#include "func.h"
#include "memoize.h"

struct Memoize_ {
  struct Vector_ v;
  m_uint arg_sz;
  m_uint ret_sz;
  struct pool *p;
  m_bool member;
  enum Kind kind;
};

static inline void memoize_return1(m_bit* tgt, const m_bit* src,
  const m_uint size __attribute__((unused))) {
  *(m_uint*)tgt = *(m_uint*)src;
}

static inline void memoize_return2(m_bit* tgt, const m_bit* src,
  const m_uint size __attribute__((unused))) {
  *(m_float*)tgt = *(m_float*)src;
}

static inline void memoize_return3(m_bit* tgt, const m_bit* src,
  const m_uint size) {
  memcpy(tgt, src, size);
}

static inline void memoize_return4(m_bit* tgt __attribute__((unused)),
  const m_bit* src __attribute__((unused)),
  const m_uint size __attribute__((unused))) {}

static void(*mreturn[])(m_bit*, const m_bit*, const m_uint) =
  { memoize_return1, memoize_return2, memoize_return3, memoize_return4};

Memoize memoize_ini(const Func f, const enum Kind kind) {
  Memoize m = mp_alloc(Memoize);
  vector_init(&m->v);
  m->ret_sz = f->def->ret_type->size;
  m->kind = kind;
  if(!GET_FLAG(f, ae_flag_member))
    m->arg_sz = f->def->stack_depth;
  else {
    m->member = SZ_INT;
    m->arg_sz = f->def->stack_depth - SZ_INT;
  }
  m->p = new_pool((uint32_t)(m->arg_sz + m->ret_sz));
  return m;
}

void memoize_end(Memoize m) {
  vector_release(&m->v);
  mp_end(m->p);
  free(m->p);
  mp_free(Memoize, m);
}

static inline void memoize_set(Memoize m, const m_bit* arg) {
  m_bit* data = _mp_alloc2(m->p);
  memcpy(data, arg, m->arg_sz);
  vector_add(&m->v, (vtype)data);
}

m_bool memoize_get(VM_Shred shred) {
  const VM_Code code = *(VM_Code*)REG(-SZ_INT * 2);
  const Memoize m = code->memoize;
  const m_bit* arg = REG(-(SZ_INT*2 + m->arg_sz + (m_uint)m->member));
  const m_uint size = vector_size(&m->v);
  for(m_uint i = 0; i < size; ++i) {
    m_bit* data = (m_bit*)vector_at(&m->v, i);
    if(memcmp(arg, data, m->arg_sz))
      continue;
    POP_REG(shred, SZ_INT*2 + (m->arg_sz - m->ret_sz) + (m_uint)m->member)
    mreturn[m->kind](shred->reg-m->ret_sz, data + m->arg_sz, m->ret_sz);
    return 1;
  }
  memoize_set(m, arg);
  return 0;
}

INSTR(MemoizeCall) {
  if(memoize_get(shred))
    ++shred->pc;
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
