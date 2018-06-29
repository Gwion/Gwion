struct JitQ_{
  VM_Code code;
  jit_function_t f;
  Instr top;
  Instr base;
  struct ctrl* ctrl;
  struct JitQ_* next;
  struct pool* pool;
  pthread_mutex_t* mutex;
};
