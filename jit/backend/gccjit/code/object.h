#include "jitter.h"
#include "cc.h"
#include "code.h"

JIT_CODE(ObjectAssign) {
  push_reg(cc, -SZ_INT);
  LVAL reg  = cc_deref_field(cc, cc->shred, "shred:reg");
  RVAL oreg = CAST(AS_RVAL(reg), GET_TYPE(object*));
  LVAL src  = ARRAY_ACCESS(oreg, ICONST(GET_TYPE(m_bool), -1));
  LVAL ptgt = gcc_jit_rvalue_dereference(oreg, NULL);
  LVAL tgt  = gcc_jit_rvalue_dereference(CAST(AS_RVAL(ptgt), GET_TYPE(object*)), NULL);

// TODO improve me
cc_release2(cc, AS_RVAL(tgt));
cc_release2(cc, AS_RVAL(tgt));
  if(instr->m_val) {
    LVAL dsrc = gcc_jit_rvalue_dereference(AS_RVAL(src), NULL);
    ASSIGN(cc->block, dsrc, AS_RVAL(src));
  }
  ASSIGN(cc->block, tgt, AS_RVAL(src));
}

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_object(struct Jit* j) {
  JIT_IMPORT(ObjectAssign)
}
