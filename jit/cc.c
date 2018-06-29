#include <stdlib.h>
#include <string.h>
#include <jit/jit.h>
#include "defs.h"
#include "absyn.h"
#include "vm.h"
#include "operator.h"
#include "instr.h"
#include "shreduler_private.h"
#include "gwjit_common.h"
#include "jitter.h"
//#include "q.h"
//#include "q.h"
#include "sig.h"
#include "code.h"
#include "ctrl.h"
#include "mpool.h"


typedef struct JitQ_* JitQ;
extern VM_Code qcode(JitQ);
extern struct ctrl* qctrl(JitQ);

struct Map_ ctrlmap, codemap;
struct Vector_ sfunvec, mfunvec;
#define XTOR(type, attr)      \
__attribute__((attr(400)))    \
void jitmap_##type() {        \
  map_##type(&codemap);       \
  map_##type(&ctrlmap);       \
  vector_##type(&sfunvec);    \
  vector_##type(&mfunvec);    \
}
XTOR(init,    constructor)
XTOR(release, destructor)

JitCC* new_cc() {
  JitCC* j = (JitCC*)xmalloc(sizeof(JitCC));
  j->ctx = jit_context_create();
  j->q = NULL;
  sig_ini(&j->sig);
  j->pool = new_pool(sizeof(struct Instr_), 128);
  return j;
}

ANN void free_cc(JitCC* j) {
puts(__func__);
  sig_end(&j->sig);
//  if(j->q->top)
//    _mp_free(j->pool, j->q->top);
  mp_end(j->pool);
  xfree(j->pool);
  jit_context_destroy(j->ctx);
  xfree(j);
}

ANN static void ini(const JitCC* j) {
  JitCC* cc = (JitCC*)j;
  jit_context_build_start(cc->ctx);
  cc->q->f = jit_function_create(cc->ctx, sig(&cc->sig, "vp"));
  cc->shred = jit_value_get_param(cc->q->f, 0);
}

INSTR(execute_jit) {
  void* arg[] = { (VM_Shred)&shred };
  jit_function_apply((jit_function_t)instr->m_val, arg, NULL);
}

ANN static void to_instr(const JitCC* j){
  const Instr base = (Instr)j->q->base;
  const Instr tmp = (Instr)j->q->top;
  base->execute = execute_jit;
  base->m_val = (m_uint)j->q->f;
  base->m_val2 = (m_uint)j;
  *(Instr*)base->ptr = tmp;
  ((JitCC*)j)->q->top = NULL;
}

ANN static void end(const JitCC* j) {
  JINSN(default_return);
  jit_function_compile(j->q->f);
  jit_context_build_end(j->ctx);
  to_instr(j);
  ((JitCC*)j)->q->f = NULL;
}

ANN static void update_pc(const JitCC* j, struct ctrl* ctrl) {
  CJval pc = JCONST(nuint, ctrl_idx(ctrl));
  JSTORER(j->shred, JOFF(VM_Shred, pc), pc);
}

ANN static Instr get_instr(const JitCC* j, const Instr instr) {
  if(instr != j->q->base)
    return instr;
  const Instr ret = (Instr)_mp_alloc(j->pool);
  memcpy(ret, instr, sizeof(struct Instr_));
  ((JitCC*)j)->q->top = ret;
  return ret;
}

ANN void none(const JitCC* j, Instr byte) {
#ifdef JIT_DEV
fprintf(stderr, "[JIT] '%p' instr not found\n", (void*)(m_uint)byte->execute);
#endif
  CJval instr = JCONST(void_ptr, (jit_nint)byte);
  Jval  arg[] = { j->shred, instr };
  CALL_NATIVE((void*)(m_uint)byte->execute, "vpp", arg)
}

ANN static void handle_exception(const JitCC* j) {
  CJval vm   = JLOADR(j->shred, JOFF(VM_Shred, vm_ref), void_ptr);
  CJval tick = JLOADR(vm, JOFF(VM, shreduler), void_ptr);
  CJval curr = JLOADR(tick, JOFF(Shreduler, curr), void_ptr);
  CJval null = JCONST(void_ptr, 0);
  INIT_LABEL(lbl)
  CJval cond = JINSN(eq, curr, null);
  JINSN(branch_if_not, cond, &lbl);
  JINSN(default_return);
  JINSN(label, &lbl);
}

ANN static void refresh(const JitCC* j, const Instr byte) {
  if(j->q->f)
    end(j);
  j->q->base = byte;
  ini(j);
}

#define CHECK_CODE(c, ret) if(c->obj.ref_count == 1) { return ret; }

typedef void (*_ctrl)(const Instr, const struct ctrl*);
//ANN static m_bool jit_flow(const struct ctrl* c, JitQ q) {
ANN static m_bool jit_flow(const struct JThread* jt, JitQ q) {
  JitCC const* cc = jt->self;
  const struct ctrl* c = qctrl(q);
  const VM_Code code = qcode(q);
  const Vector v = code->instr;
  Instr byte;
  while((byte = ctrl_run(c, v))) {
//pthread_mutex_lock(&jt->mutex);
    if(!jt->self)return;
//pthread_mutex_unlock(&jt->mutex);
    CHECK_CODE(code, -1);
    const _ctrl ctrl = (_ctrl)map_get(&ctrlmap, (vtype)byte->execute);
    if(ctrl) {
      qlock(q);
      ctrl(byte, c);
      qunlock(q);
    }
  }
  return 1;
}

typedef void (*_code)(const JitCC*, Instr);
static void jit_code(struct JThread* jt, JitQ q) {
//static void jit_code(const JitCC* cc, JitQ q) {
  JitCC const* cc = jt->self;
  const VM_Code c = qcode(q);
  const struct ctrl* ctrl = qctrl(q);
  const Vector v = c->instr;
  Instr byte;
  while((byte = ctrl_run(ctrl, v))) {
printf("code run %p %p\n", jt, jt ? jt->self : 1);
//pthread_mutex_lock(&jt->mutex);
    if(!jt->self)return;
//pthread_mutex_unlock(&jt->mutex);

//    CHECK_CODE(c,);
    if(ctrl_pc(ctrl))
      refresh(cc, byte);
    update_pc(cc, ctrl);
    const Instr ins = get_instr(cc, byte);
    const _code code = (_code)map_get(&codemap, (vtype)byte->execute);
    qlock(q);
    if(code)
      code(cc, ins);
    else {
      none(cc, ins);
      if(ctrl_ex(ctrl))
        handle_exception(cc);
    }
puts("end code run");
    qunlock(q);
  }
  end(cc);
}
//#include "jitter.h"
//#include "thread.h"
//ANN void cc(JitCC const* cc, JitQ q) {
ANN void cc(struct JThread* jt, struct JitQ_* q) {
  JitCC const* cc = jt->self;
  const struct ctrl* ctrl = qctrl(q);
puts("start cc flow");
//  ((JitCC*)cc)->q = q;
  if(!ctrl_state(ctrl)) {
    if(jit_flow(jt, q) < 0)
      return;
    ctrl_done(ctrl);
  }
puts("start cc code");
if(!jt->self)return;
  jit_code(jt, q);
}
