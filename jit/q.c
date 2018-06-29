#include <stdlib.h>
#include <pthread.h>
#include <jit/jit.h>
#include "defs.h"
#include "absyn.h"
#include "operator.h"
#include "instr.h"
#include "jitter.h"
//#include "mpool.h"
#include "ctrl.h"
//#include "cc.h"
//#include "q.h"

ANN static void add(struct Jitter* j, VM_Code code) {
//  pthread_mutex_lock(&j->mutex);
//  pthread_mutex_lock(&j->_cc->mutex);
  struct JitQ_* r = (struct JitQ_*)_mp_alloc(j->pool);
  JitCC* cc = j->_cc->self;
  r->code = code;
  r->next = NULL;
  r->ctrl = new_ctrl(vector_size(code->instr));
  r->mutex = &j->mutex;
  r->pool = j->pool;
  ADD_REF((VM_Code)code)
/*
  if(cc->q && code->prio > cc->q->code->prio) {
    r->next = cc->q->next;
    cc->q = r;
    pthread_mutex_unlock(&j->mutex);
    pthread_mutex_unlock(&j->_cc->mutex);
    return;
  }
*/
  struct JitQ_* q = cc->q;
  if(q) {
//    do if(!q->next || code->prio > q->next->code->prio)break;
    do if(!q->next)break;
    while((q = q->next));
//    if(q->next)
//      r->next = q->next;
    q->next = r;
    pthread_mutex_unlock(&j->mutex);
//    pthread_mutex_unlock(&j->_cc->mutex);
    return;
  }
  cc->q = r;
//  pthread_mutex_unlock(&j->mutex);
//  pthread_mutex_unlock(&j->_cc->mutex);
}
/*
ANN static void* dequeue(struct Jitter* j) {
  pthread_mutex_lock(&j->_cc->mutex);
  pthread_mutex_lock(&j->mutex);
  JitCC* cc = j->_cc->self;
  struct JitQ_* const q = cc->q;
  struct JitQ_* const next = q->next;
  cc->q = next;
  pthread_mutex_unlock(&j->mutex);
  pthread_mutex_unlock(&j->_cc->mutex);
  REM_REF(q->code);
  _mp_free(j->pool, q);
  return cc->q;
}
*/

//extern void cc(struct JitCC_*, struct JitQ_*);
ANN void jitq(struct Jitter* j, VM_Code code) {
printf("%s %s\n", __func__, code->name);
  add(j, code);
  pthread_cond_signal(&j->_cc->cond);
}

void qjoin(struct Jitter* j) {
  j->_cc->wait = 1;
  pthread_barrier_wait(&j->barrier);
}

ANN void qclear(struct JitQ_* q) {
puts("clear");
  struct JitQ_* next;
  do {
    next = q->next;
    free_ctrl(q->ctrl);
    REM_REF((VM_Code)q->code);
    _mp_free(q->pool, q);
  } while((q = next));
}

ANN void qinit(struct Jitter* j) {
//  pthread_create(&j->thread, NULL, consume, j);
  j->pool = new_pool(sizeof(struct JitQ_), 128);
}

ANN VM_Code qcode(struct JitQ_* q) { return q->code; }
ANN struct ctrl* qctrl(struct JitQ_* q) { return q->ctrl; }

ANN void qlock(struct JitQ_*q) { pthread_mutex_lock(q->mutex);}
ANN void qunlock(struct JitQ_*q) { pthread_mutex_unlock(q->mutex);}
