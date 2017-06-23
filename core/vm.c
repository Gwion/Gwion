#include <stdlib.h>
#include <dlfcn.h>
#include <time.h>
#include "vm.h"
#include "err_msg.h"
#include "oo.h"
#include "func.h"
#include "instr.h"
#include "ugen.h"
#include "driver.h"
#include "shreduler.h"

void udp_do(VM* vm);
VM_Code new_vm_code(Vector instr, m_uint stack_depth, m_bool need_this,
    m_str name, m_str filename) {
  VM_Code code           = malloc(sizeof(struct VM_Code_));
  code->instr            = instr ?  vector_copy(instr) : NULL;
  code->stack_depth      = stack_depth;
  code->need_this        = need_this;
  code->name             = strdup(name);
  code->filename         = strdup(filename);
  code->native_func      = 0;
  code->native_func_type = NATIVE_UNKNOWN;
  INIT_OO(code, e_code_obj)
  return code;
}
/// free instr should be a function
void free_vm_code(VM_Code a) {
  m_uint i;
  if(!strcmp(a->name, "[dtor]")) { // dtor from release. free only EOC
    free((void*)vector_back(a->instr));
    free_vector(a->instr);
  } else if(a->instr) {
    for(i = vector_size(a->instr) + 1; --i;) {
      Instr instr = (Instr)vector_at(a->instr, i - 1);
      if(instr->execute == Instr_Array_Init ||
          instr->execute == Instr_Array_Alloc)
        free(*(VM_Array_Info**)instr->ptr);
      else if(instr->execute == Gack) {
        m_uint j;
        Vector v = *(Vector*)instr->ptr;
        for(j = vector_size(v) + 1; --j;)
          REM_REF(((Type)vector_at(v, j - 1)));
        free_vector(v);
      } else if(instr->execute == Branch_Switch)
        free_map(*(Map*)instr->ptr);
      else if(instr->execute == Spork) {
        if(instr->m_val2)
          REM_REF(((Func)instr->m_val2))
      } else if(instr->execute == Init_Loop_Counter)
        free((m_int*)instr->m_val);
      free(instr);
    }
    free_vector(a->instr);
  }
  free(a->name);
  free(a->filename);
  free(a);
}

VM_Shred new_vm_shred(VM_Code c) {
  VM_Shred shred    = calloc(1, sizeof(struct VM_Shred_));
  shred->base       = calloc(SIZEOF_MEM, sizeof(char));
  shred->_reg       = calloc(SIZEOF_REG, sizeof(char));
  shred->reg        = shred->_reg;
  shred->mem        = shred->base;
  shred->_mem       = shred->base;
  shred->code       = c;
  shred->is_running = 1;
  shred->xid        = -1;
  shred->name       = strdup(c->name);
  vector_init(&shred->gc1);
  return shred;
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
  free(shred);
}

BBQ new_bbq(VM* vm, DriverInfo* di, Driver** driver) {
  BBQ a;
  Driver* d;

  if(!(d = di->func(vm)) || d->ini(vm, di) < 0)
    return NULL; // LCOV_EXCL_LINE
  a = malloc(sizeof(struct BBQ_));
  sp_createn(&a->sp, di->out);
  free(a->sp->out);
  a->sp->out   = calloc(di->out, sizeof(SPFLOAT));
  a->in   = calloc(di->in, sizeof(SPFLOAT));
  a->n_in = di->in;
  a->sp->sr = di->sr;
  *driver = d;
  sp_srand(a->sp, time(NULL));
  return a;
}

static void free_bbq(BBQ a) {
  sp_destroy(&a->sp);
  free(a->in);
  free(a);
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
  if(vm->env)
    REM_REF(vm->env);
  if(vm->emit)
    REM_REF(vm->emit);
  for(i = vector_size(&vm->plug) + 1; --i;)
    dlclose((void*)vector_at(&vm->plug, i - 1));
  vector_release(&vm->plug);
  vector_release(&vm->shred);
  vector_release(&vm->ugen);
  if(vm->bbq)
    free_bbq(vm->bbq);
  free_shreduler(vm->shreduler);
  free(vm);
}

void vm_add_shred(VM* vm, VM_Shred shred) {
  shred->vm_ref = vm;
  if(shred->xid == -1) {
    vector_add(&vm->shred, (vtype)shred);
  }
  shredule(vm->shreduler, shred, get_now(vm->shreduler) + .5);
}

void vm_run(VM* vm) {
  m_uint   i;
  VM_Shred shred;
  Instr    instr;
  while((shred = shreduler_get(vm->shreduler))) {
#ifdef DEBUG_VM
    debug_msg("vm", "shred [%i]: stack: {%i:%i}. pc: (%i,%i / %i)",
        shred->xid, *shred->reg, *shred->mem, shred->pc,
        shred->next_pc, vector_size(shred->code->instr));
#endif
    while(shred->is_running) {
      shred->pc = shred->next_pc;
      shred->next_pc++;
      instr = (Instr)vector_at(shred->code->instr, shred->pc);
#ifdef DEBUG_VM
      if(!instr) {
        err_msg(VM_, 0, "internal error: no instruction");
        debug_msg("vm", "shred [%i]: pc: (%i,%i / %i)", shred->xid,
                  shred->pc, shred->next_pc, vector_size(shred->code->instr));
        break;
      } else if(!instr->execute) {
        err_msg(VM_, 0, "internal error: instruction has no execute function");
        shred->is_done = 1;
        continue;
      } else
#endif
        instr->execute(vm, shred, instr);
#ifdef DEBUG_STACK
      debug_msg("stack", "shred[%i] mem[%i] reg[%i]", shred->xid,
                shred->mem - shred->_mem, shred->reg - shred->_reg);
#endif
#ifdef DEBUG_VM
      debug_msg("vm", "shred [%i]: pc: (%i,%i / %i)", shred->xid, shred->pc,
          shred->next_pc, vector_size(shred->code->instr));
#endif
      if(shred->is_done) {
        if(shreduler_remove(vm->shreduler, shred, 1) < 0) {
          goto next;
        }
      }
    }
next:
    ;
  }
  if(!vm->is_running) {
    return;
  }
  udp_do(vm);
  for(i = vector_size(&vm->ugen) + 1; --i;) {
    UGen u = (UGen)vector_at(&vm->ugen, i - 1);
    u->done = 0;
    if(u->channel) {
      m_uint j;
      for(j = u->n_chan; --j;)
        u->channel[j - 1]->ugen->done = 0;
    }
  }
  ugen_compute(vm->blackhole->ugen);
  ugen_compute(vm->dac->ugen);
}

