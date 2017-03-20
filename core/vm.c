#include <stdlib.h>
#include <time.h>
#include "vm.h"

// get rid of that
#include "err_msg.h"
#include "type.h"

#include "instr.h"
#include "ugen.h"

#include "driver.h"

void stop_plug();

VM_Code new_VM_Code(Vector instr, m_uint stack_depth, m_bool need_this, m_str name, m_str filename)
{
  VM_Code code           = malloc(sizeof(struct VM_Code_));
  code->instr            = instr ?  vector_copy(instr) : NULL;
  code->stack_depth      = stack_depth;
  code->need_this        = need_this;
  code->name             = strdup(name);
  code->filename         = strdup(filename);
  code->native_func      = 0;
  code->native_func_type = NATIVE_UNKNOWN;
  return code;
}

void free_VM_Code(VM_Code a)
{
  m_uint i;
  if(!strcmp(a->name, "[dtor]")) { // dtor from release. free only EOC
    free((void*)vector_back(a->instr));
    free_Vector(a->instr);
  } else if(a->instr) {
      for(i = 0; i < vector_size(a->instr); i++) {
        Instr instr = (Instr)vector_at(a->instr, i);
        if(instr->execute == Instr_Array_Init || instr->execute == Instr_Array_Alloc ||
            instr->execute == Gack || instr->execute == Branch_Switch)
          free(instr->ptr);
      free((Instr)vector_at(a->instr, i));
    }
    free_Vector(a->instr);
  }
  free(a->name);
  free(a->filename);
  free(a);
}

VM_Shred new_VM_Shred(VM_Code c)
{
  VM_Shred shred    = calloc(1, sizeof(struct VM_Shred_));
  shred->base       = calloc(SIZEOF_MEM, sizeof(char));
  shred->_reg       = calloc(SIZEOF_REG, sizeof(char));
  shred->reg        = shred->_reg;
  shred->mem        = shred->base;
  shred->pc         = 0;
  shred->next_pc    = 0;
  shred->code       = c;
  shred->is_done    = 0;
  shred->is_running = 1;
  shred->wake_time  = 0.;
  shred->xid        = -1;
  shred->prev       = NULL;
  shred->next       = NULL;
  shred->name       = strdup(c->name);
  shred->me         = NULL;
  shred->child      = NULL;
  shred->parent     = NULL;
  shred->vm_ref     = NULL;
  shred->args       = NULL;
  shred->me         = NULL;
  shred->filename   = NULL;
  shred->child      = NULL;
#ifdef DEBUG_STACK
  shred->mem_index  = 0;
  shred->reg_index  = 0;
#endif
  return shred;
}

void free_VM_Shred(VM_Shred shred)
{
  release(shred->me, shred);
  if(strstr(shred->name, "spork~"))
    free(shred->_mem);
  else
    free(shred->base);
  free(shred->_reg);
  free_VM_Code(shred->code);
  free(shred->name);
  free(shred->filename);
  free(shred);
}

BBQ new_BBQ(VM* vm, DriverInfo* di, Driver** driver)
{
  BBQ a = malloc(sizeof(struct BBQ_));
  Driver* d;

  if(!(d = di->func(vm)))
    goto error;

  if(d->ini(vm, di) < 0) {
    goto error;
  }
  sp_createn(&a->sp, di->out);
  free(a->sp->out);
  a->sp->out   = calloc(di->out, sizeof(SPFLOAT));
  a->in   = calloc(di->in, sizeof(SPFLOAT));
  a->n_in = di->in;
  a->sp->sr = di->sr;
  *driver = d;
  sp_srand(a->sp, time(NULL));
  return a;
error:
  free(a);
  return NULL;
}

static void free_BBQ(BBQ a)
{
  sp_destroy(&a->sp);
  free(a->in);
  free(a);
}

VM* new_VM(m_bool loop)
{
  VM* vm         = (VM*)malloc(sizeof(VM));
  vm->shred      = new_Vector();
  vm->ugen       = new_Vector();
  vm->shreduler  = new_Shreduler(vm);
  vm->env        = NULL;
  vm->emit       = NULL;
  vm->is_running = 0;
  vm->bbq        = NULL;
  vm->adc        = NULL;
  vm->dac        = NULL;
  vm->blackhole  = NULL;
  vm->wakeup     = NULL;
  shreduler_set_loop(vm->shreduler, loop < 0 ? 0 : 1);
  return vm;
}

void free_VM(VM* vm)
{
  if(vm->env)
    rem_ref(vm->env->obj, vm->env);
  if(vm->emit)
    rem_ref(vm->emit->obj, vm->emit);
  stop_plug();
  free_Vector(vm->shred);
  free_Vector(vm->ugen);
  if(vm->bbq)
    free_BBQ(vm->bbq);
  free_Shreduler(vm->shreduler);
  free(vm);
}

void vm_add_shred(VM* vm, VM_Shred shred)
{
  shred->vm_ref = vm;
  if(shred->xid == -1) {
    shred->xid = vector_size(vm->shred);
    vector_append(vm->shred, (vtype)shred);
  }
  shredule(vm->shreduler, shred, get_now(vm->shreduler) + .5);
}

void vm_run(VM* vm)
{
  m_uint   i;
  VM_Shred shred;
  Instr    instr;
  while((shred = shreduler_get(vm->shreduler))) {
#ifdef DEBUG_VM
    debug_msg("vm", "shred [%i]: stack: {%i:%i}. pc: (%i,%i / %i)", shred->xid, *shred->reg,
              *shred->mem, shred->pc, shred->next_pc, vector_size(shred->code->instr));
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
      /*  else */
      debug_msg("stack", "shred[%i] mem[%i] reg[%i]", shred->xid, shred->mem_index, shred->reg_index);
#endif
#ifdef DEBUG_VM
      /*  else */
      debug_msg("vm", "shred [%i]: pc: (%i,%i / %i)", shred->xid, shred->pc, shred->next_pc, vector_size(shred->code->instr));
#endif
      if(shred->is_done) {
        if(shreduler_remove(vm->shreduler, shred, 1) < 0) {
          goto next;
//          continue;
        }
      }
    }
next:
    ;
  }
  if(!vm->is_running) {
    return;
  }
  for(i = 0; i < vector_size(vm->ugen); i++) {
    UGen u = (UGen)vector_at(vm->ugen, i);
    u->done = 0;
    if(u->channel) {
      for(int j = 0; j < u->n_in; j++)
        u->channel[j]->ugen->done = 0;
      for(int j = 0; j < u->n_out; j++)
        u->channel[j]->ugen->done = 0;
    }
  }
  ugen_compute(vm->blackhole->ugen);
  ugen_compute(vm->dac->ugen);
}

