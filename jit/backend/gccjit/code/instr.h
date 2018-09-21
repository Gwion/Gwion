#include "jitter.h"
#include "cc.h"
#include "code.h"

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_instr(struct Jit* j) {
//  JIT_IMPORT(RegPop);
}
