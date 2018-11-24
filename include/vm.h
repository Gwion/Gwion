#ifndef __VM
#define __VM

#ifdef USE_DOUBLE
#undef USE_DOUBLE
#endif

typedef struct VM_Code_* VM_Code;
struct VM_Code_ {
  Vector instr;
  m_str name;
  m_uint stack_depth;
  m_uint native_func;
  void* memoize;
  ae_flag flag;
  HAS_OBJ
};

struct BBQ_ {
  uint64_t pos;
  m_float* in;
  m_float* out;
  uint32_t sr; // int 32
  uint8_t nchan;
  uint8_t n_in;
};

typedef struct Shreduler_* Shreduler;
typedef struct Emitter_   * Emitter;
typedef struct VM_ {
  Shreduler shreduler;
  struct Vector_ ugen;
  struct Gwion_* gwion;
  struct BBQ_* bbq;
  uint32_t rand[2];
  volatile unsigned is_running : 1; // => shreduler
} VM;

typedef struct VM_Shred_* VM_Shred;
struct VM_Shred_ {
  VM_Code code;
  VM_Shred parent;
  m_bit* reg;
  m_bit* mem;
  m_bit* _reg;
  m_bit* base;
  m_uint pc, xid;
  m_str name;
  VM* vm;
  VM_Shred prev, next;
  Vector args; // passed pointer from compile
  struct M_Object_* me;
  struct Vector_ child;
  struct Vector_ gc;//, gc1;
  m_float wake_time;
};
ANN2(4) ANEW VM_Code new_vm_code(const Vector instr, const m_uint stack_depth, const m_bool need_this, const m_str name);
ANN void free_vm_code(const VM_Code a);

ANN VM_Shred shreduler_get(const Shreduler s) __attribute__((hot));
ANN void shreduler_remove(const Shreduler s, const VM_Shred out, const m_bool erase)__attribute__((hot));
ANN void shredule(const Shreduler s, const VM_Shred shred, const m_float wake_time)__attribute__((hot));
ANN void shreduler_set_loop(const Shreduler s, const m_bool loop);
ANN void shreduler_add(const Shreduler s, const VM_Shred shred);

ANEW ANN VM_Shred new_vm_shred(const VM_Code code) __attribute__((hot));
__attribute__((hot))
ANN static inline void vm_shred_exit(const VM_Shred shred) { shreduler_remove(shred->vm->shreduler, shred, 1); }
void free_vm_shred(const VM_Shred shred)__attribute__((hot, nonnull));

ANN void vm_run(const VM* vm) __attribute__((hot));
ANEW VM* new_vm(void);
ANN void free_vm(VM* vm);
ANN m_uint vm_add_shred(const VM* vm, const VM_Shred shred)__attribute__((hot));
ANN void vm_remove(const VM* vm, const m_uint index)__attribute__((hot));
ANN m_str code_name_set(const m_str, const m_str);
ANN m_str code_name(const m_str, const m_bool);
ANN uint32_t gw_rand(uint32_t s[2]);
#endif
