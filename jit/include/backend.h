struct JitBackend {
  void (*no)(JitThread, Instr);
  void (*pc)(JitThread, struct ctrl*);
  void (*ex)(JitThread);
  void (*ini)(JitThread);
  void (*end)(JitThread);
  void (*ctrl)(struct Jit*);
  void (*code)(struct Jit*);
  void (*free)(JitThread, void*); //optionnal
};

struct JitBackend* new_jit_backend();

void  free_jit_backend(struct JitBackend*);
