#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dlfcn.h>
#include "vm.h"
#include "type.h"
#include "instr.h"
#include "ugen.h"
#include "driver.h"

VM_Shred new_vm_shred(VM_Code c) {
  VM_Shred shred    = calloc(1, sizeof(struct VM_Shred_));
  shred->base       = calloc(SIZEOF_MEM, sizeof(char));
  shred->_reg       = calloc(SIZEOF_REG, sizeof(char));
  shred->reg        = shred->_reg;
  shred->mem        = shred->base;
  shred->_mem       = shred->base;
  shred->code       = c;
  shred->name       = strdup(c->name);
  vector_init(&shred->gc1);
  return shred;
}

static void vm_shred_free_args(Vector v) {
  m_uint i;
  for(i = 0; i < vector_size(v); i++)
    free((void*)vector_at(v, i));
  free_vector(v);
}

void free_vm_shred(VM_Shred shred) {
  release(shred->me, shred);
  if(strstr(shred->name, "spork~"))
    free(shred->_mem);
  else
    free(shred->base);
  free(shred->_reg);
  REM_REF(shred->code)
  free(shred->name);
  free(shred->filename);
  vector_release(&shred->gc1);
  if(shred->args)
    vm_shred_free_args(shred->args);
  free(shred);
}

m_bool init_bbq(VM* vm, DriverInfo* di, Driver* d) {
  di->func(d, vm);
  if(d->ini(vm, di) < 0)
    return -1; // LCOV_EXCL_LINE
  sp_createn(&vm->sp, di->out);
  free(vm->sp->out);
  vm->sp->out   = calloc(di->out, sizeof(SPFLOAT));
  vm->in   = calloc(di->in, sizeof(SPFLOAT));
  vm->n_in = di->in;
  vm->sp->sr = di->sr;
  sp_srand(vm->sp, time(NULL));
  return 1;
}

static Shreduler new_shreduler(VM* vm) {
  Shreduler s = (Shreduler)malloc(sizeof(struct Shreduler_));
  s->curr = s->list = NULL;
  s->vm = vm;
  s->n_shred = 1;
  return s;
}

static void free_shreduler(Shreduler s) {
  free(s);
}

void shreduler_set_loop(Shreduler s, m_bool loop) {
  s->loop = loop;
}

VM_Shred shreduler_get(Shreduler s) {
  VM_Shred shred = s->list;
  if(!shred) {
    if(!vector_size(&s->vm->shred) && !s->loop) {
      s->vm->is_running = 0;
      s->vm->wakeup();
    }
    return NULL;
  }
  if(shred->wake_time <= (s->vm->sp->pos + .5)) {
    s->list = shred->next;
    shred->next = NULL;
    shred->prev = NULL;
    if(s->list)
      s->list->prev = NULL;
    s->curr = shred;
    return shred;
  }
  return NULL;
}

static void shreduler_parent(VM_Shred out, Vector v) {
  m_uint index = vector_find(v, (vtype)out);
  vector_rem(v, index);
  if(!vector_size(v)) {
    vector_release(v);
    out->parent->child.ptr = NULL;
  }
}

static void shreduler_child(Shreduler s, Vector v) {
  m_uint i, size = vector_size(v);
  for(i = 0; i < size; i++) {
    VM_Shred child = (VM_Shred)vector_front(v);
    if(child == s->list) // 09/03/17
      s->list = NULL;
    shreduler_remove(s, child, 1);
  }
}

static void shreduler_gc(VM_Shred out) {
  m_uint i;
  for(i = 0; i < vector_size(&out->gc); i++) {
    M_Object o = (M_Object)vector_at(&out->gc, i);
    if(o)
      release(o, out);
  }
  vector_release(&out->gc);
}

static void shreduler_erase(Shreduler s, VM_Shred out) {
  vtype index;
  if(out->parent)
    shreduler_parent(out, &out->parent->child);
  if(out->child.ptr)
    shreduler_child(s, &out->child);
  index = vector_find(&s->vm->shred, (vtype)out);
  vector_rem(&s->vm->shred, index);
  if(out->gc.ptr)
    shreduler_gc(out);
}

static m_bool shreduler_free_shred(Shreduler s, VM_Shred out, m_bool erase) {
  if(!out->prev && !out->next && out != s->list) {
    if(erase && !out->wait && !out->child.ptr)
      free_vm_shred(out);
    return - 1;
  }
  return 1;
}

void shreduler_remove(Shreduler s, VM_Shred out, m_bool erase) {
  if(erase)
    shreduler_erase(s, out);
  s->curr = (s->curr == out) ? NULL : s->curr;
  if(shreduler_free_shred(s, out, erase) < 0)
    return;
  out->prev ? (out->prev->next = out->next) : (s->list = out->next);
  if(out->next)
    out->next->prev = out->prev;
  out->next = out->prev = NULL;
  return;
}

m_bool shredule(Shreduler s, VM_Shred shred, m_float wake_time) {
  VM_Shred curr, prev;

  shred->wake_time = (wake_time += s->vm->sp->pos);
  if(!s->list)
    s->list = shred;
  else {
    curr = s->list;
    prev = NULL;
    while(curr) {
      if(curr->wake_time > wake_time)
        break;
      prev = curr;
      curr = curr->next;
    }
    if(!prev) {
      shred->next = s->list;
      if(s->list)
        s->list->prev = shred;
      s->list = shred;
    } else {
      shred->next = prev->next;
      shred->prev = prev;
      if(prev->next)
        prev->next->prev = shred;
      prev->next = shred;
    }
  }
  if(s->curr == shred)
    s->curr = NULL;
  return 1;
}

VM* new_vm(m_bool loop) {
  VM* vm         = (VM*)calloc(1, sizeof(VM));
  vm->shreduler  = new_shreduler(vm);
  vector_init(&vm->shred);
  vector_init(&vm->ugen);
  vector_init(&vm->plug);
  shreduler_set_loop(vm->shreduler, loop < 0 ? 0 : 1);
  return vm;
}

void free_vm(VM* vm) {
  m_uint i;
  if(vm->emit)
    free_emitter(vm->emit);
  for(i = vector_size(&vm->plug) + 1; --i;)
    dlclose((void*)vector_at(&vm->plug, i - 1));
  vector_release(&vm->plug);
  vector_release(&vm->shred);
  vector_release(&vm->ugen);
  if(vm->sp)
    sp_destroy(&vm->sp);
  free(vm->in);
  free_shreduler(vm->shreduler);
  free(vm);
  free_symbols();
}

void vm_add_shred(VM* vm, VM_Shred shred) {
  shred->vm_ref = vm;
  if(!shred->me)
    shred->me = new_shred(vm, shred);
  if(!shred->xid) {
    vector_add(&vm->shred, (vtype)shred);
    shred->xid = vm->shreduler->n_shred++;
  }
  shredule(vm->shreduler, shred, .5);
}

static void vm_run_shred(VM* vm, VM_Shred shred) {
  Instr instr;
  while(vm->shreduler->curr) {
    shred->pc = shred->next_pc++;
    instr = (Instr)vector_at(shred->code->instr, shred->pc);
    instr->execute(vm, shred, instr);
#ifdef DEBUG_STACK
    debug_msg("stack", "shred[%i] mem[%i] reg[%i]", shred->xid,
              shred->mem - shred->_mem, shred->reg - shred->_reg);
#endif
    if(!shred->me)
     shreduler_remove(vm->shreduler, shred, 1);
  }
}

static void vm_ugen_init(VM* vm) {
  m_uint i;
  for(i = vector_size(&vm->ugen) + 1; --i;) {
    UGen u = (UGen)vector_at(&vm->ugen, i - 1);
    u->done = 0;
    if(u->channel) {
      m_uint j;
      for(j = u->n_chan + 1; --j;) // miss + 1
        UGEN(u->channel[j - 1])->done = 0;
    }
    if(u->trig)
      UGEN(u->trig)->done = 0;
  }
  ugen_compute(UGEN(vm->adc));
  ugen_compute(UGEN(vm->dac));
  ugen_compute(UGEN(vm->blackhole));
}

void vm_run(VM* vm) {
  VM_Shred shred;
  while((shred = shreduler_get(vm->shreduler)))
    vm_run_shred(vm, shred);
  if(!vm->is_running)
    return;
  udp_do(vm);
  vm_ugen_init(vm);
}
