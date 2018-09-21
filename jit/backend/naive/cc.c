#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "absyn.h"
#include "operator.h"
#include "instr.h"
#include "mpool.h"
#include "jitter.h"
#include "cc.h"
#include "ctrl.h"
#include "thread.h"
#include "backend.h"
#include <sys/mman.h>

typedef long(*fn)(void);


void mytest() {  }
void mytest2() { mytest(); }

static void naive_no(JitThread jt, Instr instr) {
puts("none");

  CC cc = jt->cc;

}

static void naive_ex(JitThread jt) {
  CC cc = jt->cc;

}


static void naive_end(JitThread jt) {
  CC cc = jt->cc;
//JitTest(NULL, NULL);
//  jit_add(cc, 0xc3);
  printf("%-12s %p\n", __func__, (void*)jt);
//  for(m_uint i = 0; i < cc->idx; ++i) {
//    printf("%02x\n", cc->data[i]);
//  }
  // mk file
  const m_uint len = cc->idx * sizeof(uint8_t);
  char* mem = mmap(0, 4096,
    PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if(mem == MAP_FAILED) {
    puts("mmap failed");
    exit(2);
  }
  memset(mem, 0, 4096);

// identity
//  jit_add(cc, 3, 0x48, 0x8b, 0xc7);
//  jit_add(cc, 1, 0xc3);

//jit_add(cc, 4, 0x48, 0x83, 0xec, 0x08);
/*
jit_add(cc, 2, 0xb0, 0x00);
  uintptr_t rel = (uintptr_t)mytest - (uintptr_t)(cc->data + (cc->idx - 5));
  jit_add(cc, 1, 0xe8);
  cc->data[cc->idx++] = rel >>  0;
  cc->data[cc->idx++] = rel >>  8;
  cc->data[cc->idx++] = rel >> 16;
  cc->data[cc->idx++] = rel >> 24;
*/
//e8 6c 2a fb ff
//jit_add(cc, 4, 0x48, 0x83, 0xc4, 0x08);

jit_add(cc, 1, 0xc3);
  memcpy(mem, cc->data, cc->idx);
/*
mem[0] = 0x48;
mem[1] = 0x8b;
mem[2] = 0xc7;
mem[3] = 0xc3;
*/
  if(mprotect(mem, 4096, PROT_READ | PROT_EXEC) == -1) {
    puts("mprotect failed");
    exit(2);
  }
  const Instr base = (Instr)jt->base;
  base->execute = JitExec;
  base->m_val = (m_uint)mem;
  base->m_val2 = (m_uint)jt;
  *(Instr*)base->ptr = jt->top;
  jt->top = NULL;
  jt->base = NULL;
  cc->idx = 0;
}

static void naive_pc(JitThread jt, struct ctrl* ctrl) {
  CC cc = jt->cc;
  printf("%s %lu\n", __func__, cc->idx);
}
static void naive_ctrl(struct Jit* j) {
/*
  jit_code_import_instr(j);
  jit_code_import_int(j);
  jit_code_import_float(j);
  jit_code_import_complex(j);
  jit_code_import_vec(j);
  jit_code_import_object(j);
  jit_code_import_array(j);
  jit_code_import_string(j);
  jit_code_import_event(j);
  jit_code_import_ugen  (j);
  jit_code_import_fileio(j);
  jit_code_import_ptr(j);
  jit_code_import_gack(j);
  jit_code_import_vararg(j);
  jit_code_import_func(j);
  jit_code_import_instr(j);
  jit_code_import_int(j);
  jit_code_import_float(j);
  jit_code_import_complex(j);
  jit_code_import_vec(j);
  jit_code_import_object(j);
  jit_code_import_array(j);
  jit_code_import_string(j);
  jit_code_import_event(j);
  jit_code_import_ugen  (j);
  jit_code_import_fileio(j);
  jit_code_import_ptr(j);
  jit_code_import_gack(j);
  jit_code_import_vararg(j);
  jit_code_import_func(j);
*/
}

static void naive_code(struct Jit* j) {
/*
  jit_code_import_instr(j);
  jit_code_import_int(j);
  jit_code_import_float(j);
  jit_code_import_complex(j);
  jit_code_import_vec(j);
  jit_code_import_object(j);
  jit_code_import_array(j);
  jit_code_import_string(j);
  jit_code_import_event(j);
  jit_code_import_ugen  (j);
  jit_code_import_fileio(j);
  jit_code_import_ptr(j);
  jit_code_import_gack(j);
  jit_code_import_vararg(j);
  jit_code_import_func(j);
  jit_code_import_instr(j);
  jit_code_import_int(j);
  jit_code_import_float(j);
  jit_code_import_complex(j);
  jit_code_import_vec(j);
  jit_code_import_object(j);
  jit_code_import_array(j);
  jit_code_import_string(j);
  jit_code_import_event(j);
  jit_code_import_ugen  (j);
  jit_code_import_fileio(j);
  jit_code_import_ptr(j);
  jit_code_import_gack(j);
  jit_code_import_vararg(j);
  jit_code_import_func(j);
*/
}

struct JitBackend* new_jit_backend() {
  struct JitBackend* be = (struct JitBackend*)xmalloc(sizeof(struct JitBackend));
  be->no   = naive_no;
  be->pc   = naive_pc;
  be->ex   = naive_ex;
  be->end  = naive_end;
  be->ctrl = naive_ctrl;
  be->code = naive_code;
  return be;
}

void free_jit_backend(struct JitBackend* be) {
  xfree(be);
}
#include "shreduler_private.h"
INSTR(JitExec){
puts(__func__);
  Instr byte = *(Instr*)instr->ptr;
//  fn f = (fn)instr->m_val;
  void (*f)(VM_Shred, Instr) = (void(*)(VM_Shred, Instr))instr->m_val;
//  int i = (*f)(5);
//printf("result %i %li %lu %lu\n", i, (*f)(6), shred->pc, 
//vector_size(shred->code->instr));
(*f)(shred, instr);
if(shred->pc == vector_size(shred->code->instr))
vm_shred_exit(shred);

}

void free_cc(CC cc) { xfree(cc); }
CC new_cc(){ return xcalloc(1, sizeof(struct JitCC_)); }
void free_jit_instr(JitThread jt, Instr instr){
//  munmap((void*)instr->m_val, 4096);
  _mp_free2(jt->pool, instr);
}
