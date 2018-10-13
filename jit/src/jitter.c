#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "defs.h"
#include "absyn.h"
#include "mpool.h"
#include "type.h"
//#include "operator.h"
#include "instr.h"
#include "jitter.h"
#include "ctrl.h"
#include "code.h"
#include "thread.h"
#include "backend.h"
#include "ctrl_private.h"
#include "code_private.h"



static Q new_q(const VM_Code code, Q next) {
  Q q = (Q)xcalloc(1, sizeof(struct Q_));
  q->ctrl = new_ctrl(vector_size(code->instr));
  ADD_REF((q->code = code));
  q->next = next;
  return q;
}

static void free_q(void* data) {
  Q q = (Q)data;
  REM_REF(q->code);
  free_ctrl(q->ctrl);
  xfree(q);
}
#ifdef JIT_DEV
static struct Map_ dev_map;
__attribute__((constructor(200))) static void jit_dev_ini() { map_init(&dev_map); }
__attribute__((destructor(200))) static void jit_dev_end() {
  const m_uint size = map_size(&dev_map);
  for(m_uint i = 0; i < size; ++i)
    xfree((m_str)map_at(&dev_map, i));
  map_release(&dev_map);
}
void jit_dev(f_instr f, m_str name, m_str file, const m_uint line) {
  m_str str;
  asprintf(&str, "%-24s (%s:%" INT_F ")", name, file, line);
  map_set(&dev_map, (vtype)f, (vtype)str);
}
m_str dev_get(f_instr f) {
  const m_str str = (m_str)map_get(&dev_map, (vtype)f);
  const size_t len = strlen(strchr(str, ' '));
  return strndup(str, strlen(str) - len);
}
#endif

ANN static m_bool flow(JitThread jt, Q q) {
  const Map m = &jt->j->ctrl;
  const VM_Code code = q->code;
  const Vector v = code->instr;
  Instr byte = ctrl_run(q->ctrl, v);
  do {
    pthread_testcancel();
#ifdef JIT_SKIP
    const _code code = (_code)map_get(&jt->j->code, (vtype)byte->execute);
    if(!code) {
      ctrl_set_skip(q->ctrl);
      ctrl_next(q->ctrl);
      continue;
    } else
#endif
{
      const _ctrl ctrl = (_ctrl)map_get(m, (vtype)byte->execute);
      pthread_testcancel();
      if(ctrl)
        ctrl(byte, q->ctrl);
    }
  } while((byte = ctrl_run(q->ctrl, v)));
  return 1;
}

ANN static Instr get_instr(const JitThread jt, const Instr instr) {
  if(instr != jt->base)
    return instr;
  pthread_mutex_lock(&jt->imutex);
  const Instr ret = (Instr)_mp_alloc2(jt->pool);
printf("create %p\n", ret);
//  const Instr ret = (Instr)xmalloc(sizeof(struct Instr_));
  memcpy(ret, instr, sizeof(struct Instr_));
  pthread_mutex_unlock(&jt->imutex);
  return jt->top = ret;
}

static void code(struct JitThread_* jt, Q q) {
  const VM_Code c = q->code;
  struct ctrl* ctrl = q->ctrl;
  const Vector v = c->instr;
  struct JitBackend* be = jt->j->be;
  Instr byte = jt->base = (Instr)vector_front(c->instr);
  be->ini(jt);
#ifdef JIT_DEV
  printf("[JIT]     \033[1m%s\033[0m\n", c->name);
#endif
  while((byte = ctrl_run(ctrl, v))) {
    pthread_testcancel();

    if(ctrl_pc(ctrl) && ctrl_idx(ctrl) != 1) {
      be->end(jt);
      jt->base = byte;
      be->ini(jt);
    }

    be->pc(jt, ctrl); // maybe we can get rid of that with a {set,get}_need_pc command (every call to FuncUsr)
    const Instr ins = get_instr(jt, byte);
    const _code code = (_code)map_get(&jt->j->code, (vtype)byte->execute);
#ifdef JIT_DEV
  printf("[%sJIT\033[0m](%4lu) %s%s\033[0m\n", ctrl_pc(ctrl) ? "\033[36m" : "\033[0m", ctrl_idx(ctrl), code ? "\033[32m" : "\033[31m",
    (m_str)map_get(&dev_map, (vtype)byte->execute));
#endif
    if(code)
      code(jt->cc, ins);
    else {
      be->no(jt, ins);
      if(ctrl_ex(ctrl))
        be->ex(jt);
    }
  }
  be->end(jt);
}

static void* qprocess(void* data) {
  JitThread jt = (JitThread)data;
  struct Jit* j = jt->j;
  while(1) {
    pthread_cond_wait(&j->cond, &jt->mutex);
    while(j->q) {
      if(j->done)
        break;
      pthread_mutex_lock(&j->qmutex);
      const Q q = j->q;
      j->q = q->next;
      pthread_mutex_unlock(&j->qmutex);
      pthread_cleanup_push(free_q, q);
      flow(jt, q);
      ctrl_done(q->ctrl);
      code(jt,q);
      pthread_cleanup_pop(1);
    }
    if(j->done)
      break;
    if(j->wait) {
      ++jt->done;
      pthread_barrier_wait(&jt->barrier);
      jt->done = 0;
    }
    if(j->done)
      break;
  }
  return NULL;
}

static JitThread* new_process(struct Jit* j) {
  JitThread* jts = (JitThread*)xmalloc(j->n * SZ_INT);
  for(m_uint i = 0; i < j->n; ++i) {
    JitThread jt = (JitThread)xmalloc(sizeof(struct JitThread_));
    jt->j = j;
    jts[i] = jt;
    jt->top = NULL;
    jt->base = NULL;
    jt->pool = new_pool(sizeof(struct Instr_));
    jt->cc = new_cc();
    pthread_barrier_init(&jt->barrier, NULL, 2);
    pthread_mutex_init(&jt->mutex, NULL);
    pthread_mutex_init(&jt->imutex, NULL);
    pthread_create(&jt->thread, NULL, qprocess, jt);
  }
  return jts;
}

static void free_process1(struct Jit* j) {
  const m_uint n = j->n;
  const JitThread* jts = j->process;
  j->done = 1;
  for(m_uint i = 0; i < n; ++i) {
    const JitThread jt = jts[i];
    pthread_cancel(jt->thread);
    pthread_join(jt->thread, NULL);
  }
}

static void free_process2(struct Jit* j) {
  const m_uint n = j->n;
  JitThread* jts = j->process;
  for(m_uint i = 0; i < n; ++i) {
    const JitThread jt = jts[i];
    mp_end(jt->pool);
    free_cc(jt->cc);
    pthread_barrier_destroy(&jt->barrier);
    pthread_mutex_destroy(&jt->mutex);
    pthread_mutex_destroy(&jt->imutex);
    xfree(jt->pool);
    xfree(jt);
  }
  xfree(jts);
}

void free_jit(struct Jit* j) {
  j->done = 1;
  pthread_cond_broadcast(&j->cond);
  free_process1(j);
  Q q = j->q;
  while(q) {
    const Q next = q->next;
    free_q(q);
    q = next;
  }
  free_process2(j);
  pthread_mutex_destroy(&j->qmutex);
  pthread_cond_destroy(&j->cond);
  free_jit_backend(j->be);
  map_release(&j->ctrl);
  map_release(&j->code);
  xfree(j);

}

void jitq(struct Jit* j, VM_Code c) {
  pthread_mutex_lock(&j->qmutex);
  j->q = new_q(c, j->q);
  pthread_mutex_unlock(&j->qmutex);
  pthread_cond_signal(&j->cond);
}

void jit_sync(struct Jit* j) {
  if(j->wait) {
    for(m_uint i = 0; i < j->n; ++i) {
      const JitThread jt = j->process[i];
//      if(jt->done)
        pthread_barrier_wait(&jt->barrier);
    }
    j->wait = 0;
  }
}

#define describe_import(name) \
void jit_##name##_import(struct Jit* jit, f_instr instr, _##name func) { \
  map_set(&jit->name, (vtype)instr, (vtype)func); \
}
describe_import(ctrl)
describe_import(code)

static void jit_ctrl_init(struct Jit* j, const Vector v) {
  j->be->ctrl(j);
  const m_uint size = vector_size(v);
  for(m_uint i = 0; i < size; ++i) {
    void(*import)(struct Jit*) = (void (*)(struct Jit*))vector_at(v, i);
    import(j);
  }
}

static void jit_code_init(struct Jit* j, const Vector v) {
  j->be->code(j);
  const m_uint size = vector_size(v);
  for(m_uint i = 0; i < size; ++i) {
    void(*import)(struct Jit*) = (void (*)(struct Jit*))vector_at(v, i);
    import(j);
  }
}

ANN static void* jit_init_process(void* data) {
  const struct JitInfo* ji = (struct JitInfo*)data;
  jit_ctrl_init(ji->j, ji->ctrl);
  jit_code_init(ji->j, ji->code);
  return NULL;
}

void jit_init_gwion(struct JitInfo* ji) {
  pthread_create(&ji->thread, NULL, jit_init_process, ji);
}

struct Jit* new_jit(const m_uint n, const m_uint wait) {
  struct Jit* j = (struct Jit*)xcalloc(1, sizeof(struct Jit));
  j->q = NULL;
  pthread_mutex_init(&j->qmutex, NULL);
  pthread_cond_init(&j->cond, NULL);
  j->n = n;
  j->wait = wait;
  j->be = new_jit_backend();
  j->process = new_process(j);
  map_init(&j->ctrl);
  map_init(&j->code);
  return j;
}

void free_jit_instr(Instr instr){
  const JitThread jt = (struct JitThread_*)instr->m_val2;
  pthread_mutex_lock(&jt->mutex);
  if(jt->j->be->free)
    jt->j->be->free(jt, (void*)instr->m_val);
   memcpy(instr, instr->ptr, sizeof(struct Instr_));
printf("to free %p\n", instr);
printf("to free %p\n", instr->ptr);
Instr i = *(Instr*)instr->ptr;
printf("to free %p\n", i);
   _mp_free2(jt->pool, instr->ptr);
//exit(2);
//  xfree(instr);
  pthread_mutex_unlock(&jt->mutex);
}
