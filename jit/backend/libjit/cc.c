#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <jit/jit.h>
#include <jit/jit-dump.h>
#include "defs.h"
#include "absyn.h"
#include "vm.h"
#include "shreduler_private.h"
#include "mpool.h"
#include "jitter.h"
#include "cc.h"
#include "sig.h"
#include "code/func.h"
#include "thread.h"
#include "backend.h"
#include "instr.h"
#include "code.h"
#include "code_private.h"
#include "ctrl.h"
#include "ctrl_private.h"

CC new_cc() {
  CC cc = (CC)xmalloc(sizeof(struct JitCC_));
  cc->ctx = jit_context_create();
  cc->f = NULL;
  sig_ini(&cc->sig);
  map_init(&cc->vtable);
  return cc;
}

ANN void free_cc(CC cc) {
  sig_end(&cc->sig);
  map_release(&cc->vtable);
  jit_context_destroy(cc->ctx);
  xfree(cc);
}

ANN static void libjit_ini(const JitThread jt) {
  CC cc = jt->cc;
  jit_context_build_start(cc->ctx);
  cc->f = jit_function_create(cc->ctx, sig(&cc->sig, "vp", jit_abi_fastcall));
  cc->shred = jit_value_get_param(cc->f, 0);
  cc->vm = JLOADR(cc->shred, JOFF(VM_Shred, vm), void_ptr);
  cc->reg = JLOADR(cc->shred, JOFF(VM_Shred, reg), void_ptr);
}

INSTR(JitExec) {
  void (*f)(VM_Shred) = (void (*)(VM_Shred))instr->m_val;
  f(shred);
}

ANN static void to_instr(const JitThread jt){
  CC cc = jt->cc;
  const Instr base = (Instr)jt->base;
  base->execute = JitExec;
  base->m_val = (m_uint)jit_function_to_closure(cc->f);
  base->m_val2 = (m_uint)jt;
  *(Instr*)base->ptr = jt->top;
  jt->top = NULL;
  jt->base = NULL;
}

ANN static void libjit_end(const JitThread jt) {
  CC cc = jt->cc;
  jit_insn_default_return(cc->f);
  jit_function_compile(cc->f);
  jit_context_build_end(cc->ctx);
  to_instr(jt);
//jit_dump_function(stdout, cc->f, NULL);
  map_clear(&cc->vtable);
  cc->f = NULL;
}

ANN static void libjit_pc(JitThread jt, struct ctrl* ctrl) {
  CC cc = jt->cc;
  CJval pc = JCONST(nuint, ctrl_idx(ctrl));
  JSTORER(cc->shred, JOFF(VM_Shred, pc), pc);
}

ANN void libjit_no(const JitThread jt, Instr byte) {
  CC cc = jt->cc;
  CJval instr = JCONST(void_ptr, (jit_nint)byte);
  Jval arg[] = { cc->shred, instr };
  CALL_NATIVE((void*)(m_uint)byte->execute, "vpp", arg);
  push_reg(cc, 0); // sync CC->shred
}

ANN static void libjit_ex(const JitThread jt) {
  CC cc = jt->cc;
  CJval tick = JLOADR(cc->vm, JOFF(VM, shreduler), void_ptr);
  CJval curr = JLOADR(tick, JOFF(Shreduler, curr), void_ptr);
  CJval null = JCONST(void_ptr, 0);
  INIT_LABEL(lbl)
  CJval cond = JINSN(eq, curr, null);
  JINSN(branch_if_not, cond, &lbl);
  jit_insn_default_return(cc->f);
  JINSN(label, &lbl);
}

static void libjit_ctrl(struct Jit* j) {
  jit_ctrl_import_array(j);
  jit_ctrl_import_event(j);
  jit_ctrl_import_float(j);
  jit_ctrl_import_instr(j);
  jit_ctrl_import_string(j);
  jit_ctrl_import_vararg(j);
}

static void libjit_code(struct Jit* j) {
  jit_code_import_instr(j);
  jit_code_import_int(j);
  jit_code_import_float(j);
  jit_code_import_complex(j);
  jit_code_import_vec(j);
  jit_code_import_object(j);
  jit_code_import_array(j);
  jit_code_import_string(j);
  jit_code_import_event(j);
  jit_code_import_ugen  (j);
  jit_code_import_ptr(j);
  jit_code_import_gack(j);
  jit_code_import_vararg(j);
  jit_code_import_func(j);
}

struct JitBackend* new_jit_backend() {
  struct JitBackend* be = (struct JitBackend*)xmalloc(sizeof(struct JitBackend));
  be->no   = libjit_no;
  be->pc   = libjit_pc;
  be->ex   = libjit_ex;
  be->ini  = libjit_ini;
  be->end  = libjit_end;
  be->ctrl = libjit_ctrl;
  be->code = libjit_code;
  be->free = NULL;
  return be;
}

void free_jit_backend(struct JitBackend* be) {
  xfree(be);
}
