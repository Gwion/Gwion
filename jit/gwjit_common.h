#ifdef USE_DOUBLE
#define jit_type_float jit_type_sys_double
#else
#define jit_type_float jit_type_sys_float
#endif

#define JIT_CALL JIT_CALL_NOTHROW
