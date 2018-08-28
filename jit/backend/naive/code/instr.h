#include "jitter.h"
#include "cc.h"
#include "code.h"
JIT_CODE(RegPop) {
  jit_add(cc, 0x48, 0x83, 0x6f, 0x10);
  jit_add(cc, (uint8_t)instr->m_val);
}

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_instr(struct Jit* j) {
//  JIT_IMPORT(RegPop);
}
