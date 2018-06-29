typedef jit_value_t Jval;
typedef const jit_value_t CJval;
#define INIT_LABEL(lbl) jit_label_t lbl = jit_label_undefined;

typedef struct JitCC_ {
  Jval shred;
  struct Map_ sig;
  jit_context_t ctx;
  struct pool* pool;
//  pthread_mutex_t* mutex;
  struct JitQ_* q;
} JitCC;

ANN void cc_ini(JitCC*);
ANN void cc_end(JitCC*);

#define CALL_NATIVE(func, s, argv) \
jit_insn_call_native(j->q->f, NULL, (void*)(m_uint)func, sig(&((struct JitCC_*)j)->sig, s) , (Jval*)argv, strlen(s) - 1, JIT_CALL);
#define JOFF(a,b) __builtin_offsetof(struct a##_, b)
#define JLOADR(a,b,c) jit_insn_load_relative(j->q->f, (a), (b), jit_type_##c)
#define JADDR(a,b) jit_insn_add_relative(j->q->f, (a), (b))
#define JSTORER(a,b,c) jit_insn_store_relative(j->q->f, (a), (b), (c))
#define JCONST(a, b) jit_value_create_nint_constant(j->q->f, jit_type_##a, (jit_nint)(b))
#ifdef USE_DOUBLE
#define JCONSTF(a) jit_value_create_float64_constant(j->q->f, jit_type_float, (m_float)a)
#else
#define JCONSTF(a) jit_value_create_float32_constant(j->q->f, jit_type_float, (m_float)a)
#endif
#define JINSN(a, ...) jit_insn_##a(j->q->f, ## __VA_ARGS__)
