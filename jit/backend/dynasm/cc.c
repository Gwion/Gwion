#include <stdlib.h>
#include "defs.h"
#include "mpool.h"
#include "absyn.h"
#include "operator.h"
#include "instr.h"
#include "ctrl.h"
#include "thread.h"
#include "backend.h"

static void dynasm_no(JitThread jt, Instr instr) {
  printf("%-12s %p (%p)\n", __func__, (void*)jt, (void*)instr);
}
static void dynasm_ex(JitThread jt) {
  printf("%-12s %p\n", __func__, (void*)jt);
}
static void dynasm_pc(JitThread jt, struct ctrl* ctrl) {
  printf("%-12s %p (%p)\n", __func__, (void*)jt, (void*)ctrl);
}
static void dynasm_end(JitThread jt) {
  printf("%-12s %p\n", __func__, (void*)jt);
}

static void dynasm_code(struct Jit* j) {
  printf("%-12s %p\n", __func__, (void*)j);
}

static void dynasm_ctrl(struct Jit* j) {
  printf("%-12s %p\n", __func__, (void*)j);
}

struct JitBackend* new_jit_backend() {
  struct JitBackend* be = (struct JitBackend*)xmalloc(sizeof(struct JitBackend));
  be->no = dynasm_no;
  be->pc = dynasm_pc;
  be->ex = dynasm_ex;
  be->end = dynasm_end;
  be->code = dynasm_code;
  be->ctrl = dynasm_ctrl;
  be->free = NULL;
  return be;
}

void free_jit_backend(struct JitBackend* be) {
  xfree(be);
}
INSTR(JitExec){}

void free_cc(struct CC* cc) { /* (xfree(cc);*/ }
struct CC* new_cc(){ /* return; */}
