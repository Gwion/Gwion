#ifndef __VM
#define __VM

typedef struct VM_Code_* VM_Code;
struct VM_Code_ {
  union {
    Vector instr;
    m_uint native_func;
  };
  size_t stack_depth;
  ae_flag flag;
  void* memoize;
  m_str name;
  HAS_OBJ
};

struct VM_;
struct Driver_;
typedef void (*f_bbqset)(struct Driver_*);
typedef void (*f_bbqrun)(const struct VM_*);
struct BBQ_ {
  uint64_t pos;
  m_float* in;
  m_float* out;
  volatile uint is_running;// : 1; // => shreduler
  struct SoundInfo_ *si;
  f_bbqset func;
  f_bbqrun run;
  struct Driver_* driver;
};

typedef struct Shreduler_* Shreduler;
typedef struct Emitter_   * Emitter;
typedef struct VM_ {
  Shreduler shreduler;
  struct Vector_ ugen;
  struct BBQ_* bbq;
  struct Gwion_* gwion;
  uint32_t rand[2];
} VM;

typedef struct VM_Shred_* VM_Shred;

struct ShredInfo_ {
  VM* vm;
  struct M_Object_* me;
  m_str name;
  Vector args;
};

struct ShredTick_ {
  VM_Shred self;
  struct ShredTick_ *prev;
  struct ShredTick_ *next;
  struct ShredTick_ *parent;
  struct Vector_ child;
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
ANN2(4) ANEW VM_Code new_vm_code(const Vector instr, const m_uint stack_depth, const ae_flag, const m_str name);

ANN VM_Shred shreduler_get(const Shreduler s) __attribute__((hot));
ANN void shreduler_remove(const Shreduler s, const VM_Shred out, const m_bool erase)__attribute__((hot));
ANN void shredule(const Shreduler s, const VM_Shred shred, const m_float wake_time)__attribute__((hot));
ANN void shreduler_set_loop(const Shreduler s, const m_bool loop);
ANN void shreduler_add(const Shreduler s, const VM_Shred shred);

ANEW ANN VM_Shred new_vm_shred(const VM_Code code) __attribute__((hot));
__attribute__((hot))
ANN static inline void vm_shred_exit(const VM_Shred shred) { shreduler_remove(shred->info->vm->shreduler, shred, 1); }
void free_vm_shred(const VM_Shred shred)__attribute__((hot, nonnull));

ANN void vm_run(const VM* vm) __attribute__((hot));
ANEW VM* new_vm(void);
ANN void free_vm(VM* vm);
ANN m_uint vm_add_shred(const VM* vm, const VM_Shred shred)__attribute__((hot));
ANN void vm_remove(const VM* vm, const m_uint index)__attribute__((hot));
ANN m_str code_name_set(const m_str, const m_str);
ANN m_str code_name(const m_str, const m_bool);
ANN uint32_t gw_rand(uint32_t s[2]);
ANN void gw_seed(uint32_t s[2], const uint64_t);
#endif
