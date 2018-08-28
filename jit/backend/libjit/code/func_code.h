#include "jit/jit.h"
#include "gwjit_common.h"
#include "jitter.h"
#include "cc.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"

JIT_CODE(assign_func) {
  CJval func = JLOADR(cc->reg, -SZ_INT*2-instr->m_val2, void_ptr);
  CJval ptr = JLOADR(cc->reg, -SZ_INT, void_ptr);
  JSTORER(ptr, 0, func);
  push_reg(cc, -instr->m_val -instr->m_val2);
  JSTORER(cc->reg, -SZ_INT, func);
}

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_func(struct Jit* j) {
  JIT_IMPORT(assign_func)
}
