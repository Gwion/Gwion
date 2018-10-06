typedef jit_value_t Jval;
typedef const jit_value_t CJval;
typedef struct JitCC_ {
  jit_function_t f;
  Jval shred;
  Jval vm;
  Jval reg;
  struct ctrl *ctrl;
  struct Map_ sig;
  struct Map_ label;
  struct Map_ vtable;
  jit_context_t ctx;
} JitCC;

#define JIT_CALL JIT_CALL_NOTHROW

#define INIT_LABEL(lbl) jit_label_t lbl = jit_label_undefined;

#define CALL_NATIVE(func, s, argv) \
jit_insn_call_native(cc->f, NULL, (void*)(m_uint)func, sig(&cc->sig, s, jit_abi_fastcall) , (Jval*)argv, strlen(s) - 1, JIT_CALL);\
//push_reg(cc, 0);

#define CALL_NATIVE2(func, s, argv) \
cc_call(cc, #func, s, (Jval*)argv);\
//push_reg(cc, 0);

#define JOFF(a,b) __builtin_offsetof(struct a##_, b)
#define JLOADR(a,b,c) jit_insn_load_relative(cc->f, (a), (b), jit_type_##c)
#define JADDR(a,b) jit_insn_add_relative(cc->f, (a), (b))
#define JSTORER(a,b,c) jit_insn_store_relative(cc->f, (a), (b), (c))
#define JCONST(a, b) jit_value_create_nint_constant(cc->f, jit_type_##a, (jit_nint)(b))
#define JLOADE(a,b,c) jit_insn_load_elem(cc->f, a, b, jit_type_##c)
#define JSTOREE(a,b,c) jit_insn_store_elem(cc->f, a, b, c)

#ifdef USE_DOUBLE
#define JCONSTF(a) jit_value_create_float64_constant(cc->f, jit_type_float, (m_float)a)
#else
#define JCONSTF(a) jit_value_create_float32_constant(cc->f, jit_type_float, (m_float)a)
#endif
#define JINSN(a, ...) jit_insn_##a(cc->f, ## __VA_ARGS__)

//Jval cc_call(CC cc, void* func, const m_str s, Jval *arg);
Jval cc_call(CC cc, const m_str name, const m_str s, Jval *arg);
ANN void cc_ex(const CC cc);
