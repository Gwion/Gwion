struct JThread {
  pthread_cond_t   cond;
  pthread_t        thread;
  pthread_mutex_t  mutex;
  pthread_barrier_t * barrier;
  struct pool*    pool;
  void             *self;
  m_bool wait;
};

static inline struct JThread* new_jt(
  struct Jitter* j, void (*func)(void*), size_t size) {
// could be = INITIALIZER
  struct JThread* jt = (struct JThread*)xmalloc(sizeof(struct JThread));
  pthread_cond_init(&jt->cond, NULL);
  pthread_mutex_init(&jt->mutex, NULL);
  jt->pool = new_pool(size, 256);
  jt->self = NULL;
  jt->barrier = &j->barrier;
  jt->wait = 0;
  pthread_create(&jt->thread, NULL, func, jt);
  return jt;
}

static inline void free_jt(struct JThread* jt) {
puts("signal cond");
//pthread_mutex_lock(&jt->mutex);
//pthread_cond_//wait(&jt->cond, &jt->mutex);
  jt->self = NULL;
// pthread_mutex_unlock(&jt->mutex);
const int ret =   pthread_cond_signal(&jt->cond);
printf("cond signaled %i\n", ret);
  pthread_join(jt->thread, NULL);
//  pthread_detach(jt->thread);
  pthread_mutex_destroy(&jt->mutex);
  pthread_cond_destroy(&jt->cond);
  mp_end(jt->pool);
  xfree(jt->pool);
  xfree(jt);
}

static inline void* jt_alloc(struct JThread* jt) {
  return _mp_alloc(jt->pool);
}

static inline void* jt_self(struct JThread* jt) {
  return jt->self;
}

static inline void jt_wait(struct JThread* jt) {
  pthread_cond_wait(&jt->cond, &jt->mutex);
}

static inline void jt_signal(struct JThread* jt) {
  pthread_cond_signal(&jt->cond);
}

#define describe_jt_mutex(func)      \
static inline void jt_##func(struct JThread* jt) { \
  pthread_mutex_##func(&jt->mutex);  \
}
describe_jt_mutex(lock)
describe_jt_mutex(unlock)
