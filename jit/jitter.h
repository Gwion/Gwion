struct Jitter {
//  struct JitQ_   *q;
//  struct JitCC_ *cc;
  struct pool* pool;
  pthread_t     thread;
  pthread_mutex_t mutex;
  pthread_barrier_t barrier;
  pthread_cond_t    cccond;
  pthread_cond_t    qcond;
  pthread_t         qthread;
  pthread_mutex_t   qmutex;
//  struct JitQ_      *last; // needed !
//  struct JitQ_      *qq;
  struct JThread* _cc;
/*
=> jitthread q
=> jitthread cc

*/
  m_bool init;
};
ANN void free_ctrl(struct ctrl*);

struct Jitter* new_jitter();
void jitq(struct Jitter*, VM_Code);
void free_jitter(struct Jitter* j);
void qjoin(struct Jitter*j);

/*
// free jitter.
  jit->q_cond->value = 1;
  jit->cc_cond->value = 1;
  pthread_cond_signal(cc_cond);
  pthread_cond_signal(q_cond);
  pthread_join(jit->q->mutex);
  pthread_join(jit->cc->mutex);
  mp_end(jit->pool);
  xfree(jit->pool);
  xfree(jit);
*/

static inline void jitbarrier(struct Jitter* j) {
  pthread_barrier_wait(&j->barrier);
}

static inline void qprocess(struct Jitter* j) {
puts("wait for barrier.");
  pthread_barrier_wait(&j->barrier);
puts("barrier reached");
  pthread_mutex_lock(&j->qmutex);
  while(1) {
    pthread_cond_wait(&j->qcond, &j->qmutex);
/*
     if(j->init == 2)
      break;
    do {
//      ccq(j->cc, j->qq);
      pthread_cond_signal(&j->cccond);
    } while((j->qq = j->qq->next));
*/
    pthread_mutex_lock(&j->qmutex);
//    j->last = NULL;
  }
//  if(q)
//    qclear(q);
// free_jt(jt);
}

/*
ccq
  if(j->qq && code->prio > j->q->code->prio) {
    r->next = j->q->next;
    j->q->f = j->cc->f;
    j->q->top = j->cc->top;
    j->q->base = j->cc->base;
    j->q = r;
    pthread_mutex_unlock(&j->mutex);
    return;
  }
*/

#include "jit/jit.h"
#include "mpool.h"
#include "thread.h"
#include "cc.h"
#include "q.h"
extern JitCC* new_cc();
//extern void cc(JitCC*, struct JitQ_*);
extern void cc(struct JThread* jt, struct JitQ_*);


static inline struct JitQ_* dq(struct JThread* jt, struct JitQ_* q) {
  pthread_mutex_lock(q->mutex);
  struct JitQ_* const next = q->next;
  REM_REF(q->code);
  _mp_free(q->pool, q);
  free_ctrl(q->ctrl);
  pthread_mutex_unlock(q->mutex);
  return next;
}

ANN void qclear(struct JitQ_*);
ANN void free_cc(struct JitCC_*);
static inline void ccprocess(struct JThread* jt) {
  JitCC* c = jt->self = new_cc();
  pthread_barrier_wait(jt->barrier);
//  while(jt->self) {
  while(1) {
puts("wait for cond");
//    pthread_mutex_lock(&jt->mutex);
    pthread_cond_wait(&jt->cond, &jt->mutex);
puts("cond signal reeceived");

    if(!jt->self) {
puts("quit requested");
  if(c->q) {
puts("qclear");
    qclear(c->q);
}
      break;
    }

    do cc(jt, c->q);
    while((c->q = dq(jt, c->q)));
//    while((c->q = dq(jt, c->q))&& jt->self);
puts("cc end");

    if(!jt->self) {
puts("quit requested");
//  if(c->q) {
//puts("qclear");
//    qclear(c->q);
//}
      break;
    }
//    pthread_cond_signal(&jt->cond);
/*
#ifdef JIT_ALL
    if(jt->wait) {
      pthread_barrier_wait(jt->barrier);
      jt->wait = 0;
    }
#endif
*/
  }
//  if(c->q)
//    qclear(c->q);
  free_cc(c);
}
