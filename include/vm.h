#ifndef __VM
#define __VM

typedef struct VM_Code_* VM_Code;
struct VM_Code_ {
  m_bit *bytecode;
  union {
    Vector instr;
    m_uint native_func;
  };
  size_t stack_depth;
  void* memoize;
  m_str name;
  HAS_OBJ
  ae_flag flag;
};

typedef struct Shreduler_* Shreduler;
typedef struct Emitter_   * Emitter;
typedef struct VM_Shred_* VM_Shred;

typedef struct VM_ {
  Shreduler shreduler;
  struct Vector_ ugen;
  struct BBQ_* bbq;
  struct Gwion_* gwion;
  VM_Shred cleaner_shred;
  struct VM_ *parent;
  uint32_t rand[2];
} VM;


struct ShredInfo_ {
  VM* vm;
  struct M_Object_* me;
  m_str name;
  Vector args;
  MemPool mp;
  VM_Code orig;
};

struct ShredTick_ {
  VM_Shred self;
  struct ShredTick_ *prev;
  struct ShredTick_ *next;
  struct ShredTick_ *parent;
  struct Vector_ child;
  Shreduler shreduler;
  size_t xid;
  m_float wake_time;
};

struct VM_Shred_ {
  VM_Code code;
  m_bit* reg;
  m_bit* mem;
  m_bit* base;
  size_t pc;
  struct Vector_ gc;
  struct ShredTick_ * tick;
  struct ShredInfo_ * info;
};
ANN2(1,5) ANEW VM_Code new_vm_code(MemPool p, const Vector instr, const m_uint stack_depth, const ae_flag, const m_str name);

ANN VM_Shred shreduler_get(const Shreduler s) __attribute__((hot));
ANN void shreduler_remove(const Shreduler s, const VM_Shred out, const m_bool erase)__attribute__((hot));
ANN void shredule(const Shreduler s, const VM_Shred shred, const m_float wake_time)__attribute__((hot));
ANN void shreduler_set_loop(const Shreduler s, const m_bool loop);
ANN void shreduler_ini(const Shreduler s, const VM_Shred shred);
ANN void shreduler_add(const Shreduler s, const VM_Shred shred);

ANEW ANN VM_Shred new_vm_shred(MemPool, const VM_Code code) __attribute__((hot));
ANEW ANN VM_Shred new_shred_base(const VM_Shred, const VM_Code code) __attribute__((hot));
__attribute__((hot))
ANN static inline void vm_shred_exit(const VM_Shred shred) { shreduler_remove(shred->info->vm->shreduler, shred, 1); }
void free_vm_shred(const VM_Shred shred)__attribute__((hot, nonnull));

ANN void vm_run(const VM* vm) __attribute__((hot));
ANEW VM* new_vm(MemPool, const m_bool);
ANN void vm_lock(VM const*);
ANN void vm_unlock(VM const*);
ANN m_bool vm_running(VM const*);
ANN void free_vm(VM* vm);
ANN void vm_ini_shred(const VM* vm, const VM_Shred shred)__attribute__((hot));
ANN void vm_add_shred(const VM* vm, const VM_Shred shred)__attribute__((hot));
ANN void vm_remove(const VM* vm, const m_uint index)__attribute__((hot));
ANN m_str code_name_set(MemPool p, const m_str, const m_str);
ANN m_str code_name(const m_str, const m_bool);
ANN uint32_t gw_rand(uint32_t s[2]);
ANN void gw_seed(uint32_t s[2], const uint64_t);
#endif
