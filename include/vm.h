#ifndef __VM
#define __VM

#ifdef USE_DOUBLE
#undef USE_DOUBLE
#endif
#ifndef SOUNDPIPE_H
#include <soundpipe.h>
#endif

#include <pthread.h>
#include "defs.h"
#include "oo.h"
#include "map.h"
#include "map_private.h"

typedef enum {
  NATIVE_NOT  = 1 << 1,
  NATIVE_CTOR = 1 << 2,
  NATIVE_DTOR = 1 << 3,
  _NEED_THIS_ = 1 << 4
} e_func;

typedef struct VM_Code_* VM_Code;
struct VM_Code_ {
  Vector instr;
  m_str name;
  m_uint stack_depth;
  m_uint native_func;
  e_func flag;
  struct VM_Object_ obj;
};

typedef struct Shreduler_* Shreduler;
typedef struct {
  unsigned int n_in;
  SPFLOAT* in;
  sp_data* sp;
  Shreduler shreduler;
  M_Object adc, dac, blackhole;
  Emitter emit;
  struct Vector_ shred;
  struct Vector_ ugen;
  struct Vector_ plug; // in main?
  volatile m_bool is_running; // => shreduler
} VM;

typedef struct VM_Shred_* VM_Shred;
struct VM_Shred_ {
  VM_Code code;
  VM_Shred parent;
  m_bit* reg;
  m_bit* mem;
  m_bit* _reg;
  m_bit* _mem;
  m_bit* base;
  m_uint pc, next_pc, xid;
  m_str name;
  VM* vm_ref;
  VM_Shred prev, next;
  Vector args; // passed pointer from compile
  M_Object me;
  struct Vector_ child;
  struct Vector_ gc, gc1;
  m_float wake_time;
};
ANN2(4) ANEW VM_Code new_vm_code(const Vector instr, const m_uint stack_depth, const m_bool need_this, const m_str name);
ANN void free_vm_code(const VM_Code a);

ANN VM_Shred shreduler_get(const Shreduler s) __attribute__((hot));
ANN void shreduler_remove(const Shreduler s, const VM_Shred out, const m_bool erase)__attribute__((hot));
ANN void shredule(const Shreduler s, const VM_Shred shred, const m_float wake_time)__attribute__((hot));
ANN void shreduler_set_loop(const Shreduler s, const m_bool loop);

ANEW ANN VM_Shred new_vm_shred(const VM_Code code) __attribute__((hot));
__attribute__((hot))
ANN static inline void vm_shred_exit(const VM_Shred shred) { shreduler_remove(shred->vm_ref->shreduler, shred, 1); }
void free_vm_shred(const VM_Shred shred)__attribute__((hot, nonnull));

ANN void vm_run(const VM* vm) __attribute__((hot));
ANEW VM* new_vm(const m_bool loop);
ANN void free_vm(VM* vm);
ANN void vm_add_shred(const VM* vm, const VM_Shred shred)__attribute__((hot));
ANN void vm_remove(const VM* vm, const m_uint index)__attribute__((hot));
ANN m_str code_name_set(const m_str, const m_str);
ANN m_str code_name(const m_str, const m_bool);
#endif
