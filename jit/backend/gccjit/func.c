#include <pthread.h>
#include "defs.h"
#include "map.h"
#include "vm.h"
#include "object.h"
#include "jitter.h"
#include "cc.h"
#include "libgccjit.h"

void push_reg2(CC cc, RVAL push) {
  LVAL reg = cc_deref_field(cc, cc->shred, "shred:reg");
  LVAL ret = ARRAY_ACCESS(AS_RVAL(reg), push);
  RVAL addr = gcc_jit_lvalue_get_address(ret, NULL);
  ASSIGN(cc->block, reg, addr);
}

void push_reg(CC cc, m_int val) {
  RVAL push = ICONST(GET_TYPE(m_int), val);
  push_reg2(cc, push);
}

void push_mem2(CC cc, RVAL push) {
  LVAL mem = cc_deref_field(cc, cc->shred, "shred:mem");
  LVAL ret = ARRAY_ACCESS(AS_RVAL(mem), push);
  RVAL addr = gcc_jit_lvalue_get_address(ret, NULL);
  ASSIGN(cc->block, mem, addr);
}

void push_mem(CC cc, m_int val) {
  RVAL push = ICONST(GET_TYPE(m_int), val);
  push_mem2(cc, push);
}

LVAL cc_deref_field(CC cc, RVAL val, const m_str s) {
  FIELD field = cc_get_field(cc, s);
  return gcc_jit_rvalue_dereference_field(val, NULL, field);
}

void cc_release(CC cc, RVAL obj) {
  TYPE types[2] = { GET_TYPE(object), GET_TYPE(shred) };
  RVAL args[2] = { obj, cc->shred };
  TYPE fptr = FPTR(GET_TYPE(void), 2, types, 0);
  RVAL func = PCONST(fptr, _release);
  EVAL(cc->block, CALL(func, 2, args));
}

void cc_release2(CC cc, RVAL obj) {
  TYPE types[2] = { GET_TYPE(object), GET_TYPE(shred) };
  RVAL args[2] = { obj, cc->shred };
  TYPE fptr = FPTR(GET_TYPE(void), 2, types, 0);
  RVAL func = PCONST(fptr, release);
  EVAL(cc->block, CALL(func, 2, args));
}
