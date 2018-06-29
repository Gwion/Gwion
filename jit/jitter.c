#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include "defs.h"
#include "vm.h"
#include "jitter.h"
//#include "mpool.h"

void free_cc(struct JitCC_*);
void qinit(struct Jitter*);
//void qclear(struct Jitter*);
struct Jitter* new_jitter() {
  struct Jitter* j = (struct Jitter*)xmalloc(sizeof(struct Jitter));
  j->thread = 0;
  j->init = 0;
  qinit(j);
  pthread_mutex_init(&j->mutex, NULL);
  pthread_barrier_init(&j->barrier, NULL, 2);
/*
  pthread_cond_init(&j->qcond, NULL);
  pthread_mutex_init(&j->qmutex, NULL);
  pthread_create(&j->qthread, NULL, qprocess, j);
*/
  j->_cc = new_jt(j, ccprocess, sizeof(Instr));
  return j;
}

//#include <signal.h>
ANN void free_jitter(struct Jitter* j) {
puts("free jitter");
  free_jt(j->_cc);
puts("free jitter");
  mp_end(j->pool);
  xfree(j->pool);
  xfree(j);
//return;
}
