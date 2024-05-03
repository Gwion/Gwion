#ifndef __VM
#define __VM

typedef struct VM_Code_ *VM_Code;
struct VM_Code_ {
  union {
    struct {
      m_bit *bytecode;
      struct Vector_ instr;
    };
    struct {
      m_uint         native_func;
      MP_Vector *types;
    };
  };
  Type ret_type; // could be `struct Vector_ tmpl_types;`
  void *   memoize;
  m_str            name;
  struct Map_      handlers;
  struct M_Vector_ live_values;
  uint16_t         stack_depth;
  uint16_t         ref;
//  bool             is_prepared;
  bool             builtin;
  bool             callback;
  bool             is_memoize;
};

typedef struct frame_t {
  uint32_t push, pc;
  VM_Code  code;
} frame_t;

typedef struct Shreduler_ *Shreduler;
typedef struct Emitter_ *  Emitter;
typedef struct VM_Shred_ * VM_Shred;

typedef struct VMDebugger_ {
  struct VM_ *vm;
  //  struct VM_Shred *shred;
  struct Map_ map; // code, Vector<lines>
  bool        step;
} VMDebugger;

typedef struct VM_ {
  Shreduler      shreduler;
  struct Vector_ ugen;
  struct BBQ_ *  bbq;
  struct Gwion_ *gwion;
  VM_Shred       cleaner_shred;
  struct VM_ *   parent;
  uint32_t       rand[2];
  VMDebugger *   dbg;
} VM;

typedef struct ShredDebugger_ {
  struct Vector_ breakpoint;
  struct Vector_ line;
  struct Scope_  scope;
} ShredDebugger;

struct ShredInfo_ {
  VM *              vm;
  MemPool           mp;
  struct M_Object_ *me;
  VM_Code           orig;
  struct Vector_    frame;
  struct Vector_    args;
  struct Vector_    line;
  ShredDebugger *   dbg;
};

struct ShredTick_ {
  VM_Shred           self;
  struct ShredTick_ *prev;
  struct ShredTick_ *next;
  struct ShredTick_ *parent;
  struct Vector_     child;
  Shreduler          shreduler;
  size_t             xid;
  m_float            wake_time;
};

struct VM_Shred_ {
  VM_Code            code;
  m_bit *            reg;
  m_bit *            mem;
  m_bit *            base;
  size_t             pc;
  struct ShredTick_ *tick;
  struct ShredInfo_ *info;
  gwtlock_t          mutex;
};
REF_FUNC(VM_Code, vmcode)
ANN2(1, 4)
ANEW VM_Code     new_vmcode(MemPool p, const Vector instr,
                            const M_Vector live_values, const m_str name,
                            const uint16_t stack_depth, const bool builtin, const bool dump);
ANN ANEW VM_Code vmcode_callback(MemPool p, const VM_Code code);

ANN VM_Shred shreduler_get(const Shreduler s) __attribute__((hot));
ANN void     shreduler_remove(const Shreduler s, const VM_Shred out,
                              const bool erase) __attribute__((hot));
ANN void shredule(const Shreduler s, const VM_Shred shred,
                      const m_float wake_time);
ANN void     shredule_time(const Shreduler s, const VM_Shred shred,
                      const m_float wake_time) __attribute__((hot));
ANN void     shreduler_set_loop(const Shreduler s, const bool loop);
ANN void     shreduler_ini(const Shreduler s, const VM_Shred shred);
ANN void     shreduler_add(const Shreduler s, const VM_Shred shred);

ANEW ANN VM_Shred new_vm_shred(MemPool, const VM_Code code)
    __attribute__((hot));
ANEW ANN VM_Shred new_shred_base(const VM_Shred, const VM_Code code)
    __attribute__((hot));
__attribute__((hot)) ANN static inline void
vm_shred_exit(const VM_Shred shred) {
  shreduler_remove(shred->tick->shreduler, shred, true);
}
ANN void vm_clean(const VM* vm, const Gwion gwion);
void free_vm_shred(const VM_Shred shred) __attribute__((hot, nonnull));

void vm_prepare(const VM *vm, m_bit*) __attribute__((hot));
ANN static inline void vm_run(const VM *vm) {
  vm_prepare(vm, NULL);
}
void vm_force_run(const VM *vm);
ANN void vm_run_audio(const VM *vm);
ANEW VM * new_vm(MemPool, const bool);
ANN void  vm_lock(VM const *);
ANN void  vm_unlock(VM const *);
ANN bool  vm_running(VM const *);
ANN void  free_vm(VM *vm);
ANN void  vm_ini_shred(const VM *vm, const VM_Shred shred) __attribute__((hot));
ANN void  vm_add_shred(const VM *vm, const VM_Shred shred) __attribute__((hot));
ANN bool  vm_remove(const VM *vm, const m_uint index) __attribute__((hot));
ANN m_str code_name_set(MemPool p, const m_str, const m_str);
ANN m_str code_name(const m_str, const bool);
ANN uint32_t gw_rand(uint32_t s[2]);
ANN void     gw_seed(uint32_t s[2], const uint64_t);
ANN bool     handle(VM_Shred shred, const m_str effect);
#define xfun_handle(shred, effect) {\
  shred->mem -= ((Instr)vector_at(&shred->code->instr, shred->pc-1))->m_val2; \
  handle(shred, effect); \
}

ANN bool unwind(const VM_Shred shred, const Symbol effect, const m_uint size);

#define BBQ_POS_MAX 16777216

ANN void next_bbq_pos(const VM *vm);

#define MEM_STEP   16
#define SIZEOF_MEM (0x1 << MEM_STEP)
#define SIZEOF_REG (0x1 << 14)

#endif
