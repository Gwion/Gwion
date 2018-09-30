typedef struct JitThread_ *JitThread;
struct JitThread_ {
  void* cc;
  pthread_mutex_t mutex;
  pthread_mutex_t imutex;
  pthread_t thread;
  Instr top, base;
  struct Jit * j;
  struct pool * pool;
};
