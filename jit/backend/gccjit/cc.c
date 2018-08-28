#include <stdlib.h>
#include "defs.h"
#include "absyn.h"
#include "operator.h"
#include "instr.h"
#include "jitter.h"
#include "cc.h"
#include "ctrl.h"
#include "thread.h"
#include "backend.h"

static void gccjit_no(JitThread jt, Instr instr) {
  CC cc = jt->cc;
  gcc_jit_rvalue *jshred = gcc_jit_context_new_rvalue_from_ptr(cc->ctx, pointer_type, shred);
  gcc_jit_rvalue *jinstr = gcc_jit_context_new_rvalue_from_ptr(cc->ctx, pointer_type, instr);
  gcc_jit_rvalue *args[] = { jshred, jinstr };
  gcc_jit_block_add_eval (
    cc->block, NULL,
      gcc_jit_context_new_call_trough_ptr(
      cc->ctx,
      NULL,
      instr->execute,
      2, args));
//  printf("%-12s %p (%p)\n", __func__, (void*)jt, (void*)instr);
}
static void gccjit_ex(JitThread jt) {
//  printf("%-12s %p\n", __func__, (void*)jt);
}
static void gccjit_pc(JitThread jt, struct ctrl* ctrl) {
//  printf("%-12s %p (%p)\n", __func__, (void*)jt, (void*)ctrl);
}
static void gccjit_end(JitThread jt) {
//  printf("%-12s %p\n", __func__, (void*)jt);
}

static void gccjit_ctrl(JitThread jt) {
//  printf("%-12s %p\n", __func__, (void*)jt);
}

static void gccjit_code(JitThread jt) {
//  printf("%-12s %p\n", __func__, (void*)jt);
}

struct JitBackend* new_jit_backend() {
  struct JitBackend* be = (struct JitBackend*)xmalloc(sizeof(struct JitBackend));
  be->no   = gccjit_no;
  be->pc   = gccjit_pc;
  be->ex   = gccjit_ex;
  be->end  = gccjit_end;
  be->ctrl = gccjit_ctrl;
  be->code = gccjit_code;
  return be;
}

void free_jit_backend(struct JitBackend* be) {
  xfree(be);
}
INSTR(JitExec){}

void free_cc(CC cc) { (void)cc; }
CC new_cc(){ return NULL; }
void free_jit_instr(JitThread jt, Instr instr) {}
