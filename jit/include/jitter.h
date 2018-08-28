typedef struct JitCC_* CC;
struct JitInfo {
  struct Jit* j;
  Vector ctrl;
  Vector code;
  Vector mfun;
  pthread_t thread;
};

typedef struct Q_ *Q;
struct Q_ {
  VM_Code code;
  struct ctrl* ctrl;
  Q       next;
};

struct Jit {
  Q q;
  struct JitThread_** process;
  struct JitBackend* be;
  pthread_mutex_t qmutex;
  pthread_mutex_t* vmmutex;
  pthread_cond_t cond;
  pthread_barrier_t barrier;
  struct Map_ ctrl;
  struct Map_ code;
  unsigned int n;
  unsigned int jdone;
  m_bool wait;
  m_bool done;
  m_bool init;
} __attribute__((packed));

struct Jit* new_jit(const m_uint, const m_uint);
void jitq(struct Jit*, VM_Code);
void jit_sync(struct Jit*);
void free_jit(struct Jit*);
void free_jit_instr(struct JitThread_*,Instr instr);
void jit_init_gwion(struct JitInfo *);
CC new_cc(void);
void free_cc(CC);
