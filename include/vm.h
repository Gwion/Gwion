#ifndef __VM
#define __VM

typedef struct Closure_ {
  m_bit *     data;
  struct Map_ m;
  m_uint      sz;
} Closure;
ANN Closure *new_closure(MemPool mp, const m_uint sz);
ANN void     free_closure(Closure *a, const Gwion gwion);

typedef struct VM_Code_ *VM_Code;
struct VM_Code_ {
  m_bit *bytecode;
  union {
    struct Vector_ instr;
    m_uint         native_func;
  };
  Type ret_type; // could be `struct Vector_ tmpl_types;`
  union {
    void *   memoize;
    Closure *closure;
  };
  m_str            name;
  struct Map_      handlers;
  struct M_Vector_ live_values;
  uint16_t         stack_depth;
  uint16_t         ref;
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
  struct Vector_     gc;
  struct ShredTick_ *tick;
  struct ShredInfo_ *info;
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
ANN void     shredule(const Shreduler s, const VM_Shred shred,
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
void free_vm_shred(const VM_Shred shred) __attribute__((hot, nonnull));

ANN void vm_run(const VM *vm) __attribute__((hot));
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
ANN void     handle(VM_Shred shred, const m_str effect);


#define BBQ_POS_MAX 16777216

ANN void next_bbq_pos(const VM *vm);
#endif
