#include <stdlib.h>
#include "defs.h"
#include "mpool.h"
#include "absyn.h"
#include "operator.h"
#include "instr.h"
#include "ctrl.h"
#include "thread.h"
#include "backend.h"

static void dummy_no(JitThread jt, Instr instr) {
  printf("%-12s %p (%p)\n", __func__, (void*)jt, (void*)instr);
}
static void dummy_ex(JitThread jt) {
  printf("%-12s %p\n", __func__, (void*)jt);
}
static void dummy_pc(JitThread jt, struct ctrl* ctrl) {
  printf("%-12s %p (%p)\n", __func__, (void*)jt, (void*)ctrl);
}
static void dummy_end(JitThread jt) {
  printf("%-12s %p\n", __func__, (void*)jt);
}

static void dummy_code(struct Jit* j) {
  printf("%-12s %p\n", __func__, (void*)j);
}

static void dummy_ctrl(struct Jit* j) {
  printf("%-12s %p\n", __func__, (void*)j);
}

struct JitBackend* new_jit_backend() {
  struct JitBackend* be = (struct JitBackend*)xmalloc(sizeof(struct JitBackend));
  be->no = dummy_no;
  be->pc = dummy_pc;
  be->ex = dummy_ex;
  be->end = dummy_end;
  be->code = dummy_code;
  be->ctrl = dummy_ctrl;
  return be;
}

void free_jit_backend(struct JitBackend* be) {
  xfree(be);
}
INSTR(JitExec){}

void free_cc(void* cc) { (void)cc; }
void* new_cc(){ return NULL; }

void free_jit_instr(JitThread jt, Instr instr){
  pthread_mutex_lock(&jt->imutex);
  _mp_free2(jt->pool, instr);
  pthread_mutex_unlock(&jt->imutex);
}

