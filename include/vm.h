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
  char* reg;
  char* mem;
  char* _reg;
  char* _mem;
  char* base;
  m_uint pc, next_pc, xid;
  m_str name;
  VM* vm_ref;
  VM_Shred prev, next;
  Vector args; // passed pointer from compile
  M_Object me;
  struct Vector_ child;
  struct Vector_ gc, gc1;
  struct Vector_ sporks;
  m_float wake_time;
};
__attribute__((nonnull(4)))
VM_Code new_vm_code(const Vector instr, const m_uint stack_depth, const m_bool need_this, const m_str name);
ANN void free_vm_code(VM_Code a);

VM_Shred shreduler_get(Shreduler s) __attribute__((hot, nonnull));
void shreduler_remove(Shreduler s, VM_Shred out, m_bool erase)__attribute__((hot, nonnull));
VM_Shred shreduler_get(Shreduler s) __attribute__((hot, nonnull));
m_bool shredule(Shreduler s, VM_Shred shred, m_float wake_time)__attribute__((hot, nonnull));
void shreduler_set_loop(Shreduler s, m_bool loop);

VM_Shred new_vm_shred(VM_Code code) __attribute__((hot, nonnull));
__attribute__((hot, nonnull))
static inline void vm_shred_exit(VM_Shred shred) { shreduler_remove(shred->vm_ref->shreduler, shred, 1); }
void free_vm_shred(VM_Shred shred)__attribute__((hot, nonnull));

void vm_run(VM* vm) __attribute__((hot, nonnull));
VM* new_vm(m_bool loop);
void free_vm(VM* vm)__attribute__((nonnull));
void vm_add_shred(VM* vm, VM_Shred shred)__attribute__((hot, nonnull));
void vm_remove(VM* vm, m_uint index)__attribute__((hot, nonnull));
m_str code_name_set(const m_str, const m_str)__attribute__((hot, nonnull(1)));
const m_str code_name(const m_str, const m_bool)__attribute__((hot, nonnull));
#endif
