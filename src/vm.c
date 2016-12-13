
#include <stdlib.h>
#include "vm.h"

// get rid of that
#include "err_msg.h"
#include "type.h"

#include "instr.h"
#include "ugen.h"

void stop_plug();

VM_Code new_VM_Code(Vector instr, m_uint stack_depth, m_bool need_this, m_str name, m_str filename)
{
  VM_Code code           = malloc(sizeof(struct VM_Code_));
//  code->instr            = instr ?  vector_copy(instr) : new_Vector();
  code->instr            = instr ?  vector_copy(instr) : NULL;
  code->stack_depth      = stack_depth;
  code->need_this        = need_this;
  code->name             = strdup(name);
  code->filename         = strdup(filename);
  code->native_func      = 0;
  code->native_func_type = NATIVE_UNKNOWN;
//  code->gack             = new_Vector(); // this is from Code
  return code;
}

void free_VM_Code(VM_Code a)
{
  m_uint i;
  for(i = 0; i < vector_size(a->instr); i++)
    free((Instr)vector_at(a->instr, i));
  free_Vector(a->instr);
//  free(a->name);
//  free(a->filename);
  free(a->gack);
  free(a);
}

VM_Shred new_VM_Shred(VM_Code c)
{
  VM_Shred shred    = malloc(sizeof(struct VM_Shred_));
//  shred->mem        = calloc(SIZEOF_MEM, sizeof(char));
  shred->_mem        = calloc(SIZEOF_MEM, sizeof(char));
  shred->mem = shred->_mem;
  shred->reg        = calloc(SIZEOF_REG, sizeof(char));
  shred->base       = shred->mem;
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
  free_VM_Code(shred->code);
//  free(shred->_mem);
//  free(shred->mem);
//  free(shred->reg);
//  free(shred->name);
//  free(shred->filename);
  free(shred);
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
//  if(vm->env)
//    rem_ref(vm->env->obj, vm->env);
  if(vm->emit)
    rem_ref(vm->emit->obj, vm->emit);
  stop_plug();
  free_Vector(vm->shred);
  free_Vector(vm->ugen);
  sp_destroy(&vm->bbq->sp);
  free(vm->bbq->in);
  free(vm->bbq);
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
        VM_Shred sh = *(VM_Shred*)shred;
        debug_msg("vm", "shred [%i]: pc: (%i,%i / %i)", sh->xid,
                  sh->pc, sh->next_pc, vector_size(sh->code->instr));

        break;
//continue;
//        shred->is_done = 1;
      } else if(!instr->execute) {
        err_msg(VM_, 0, "internal error: instruction has no execute function");
        shred->is_done = 1;
        continue;
      } else
#endif
        instr->execute(vm, shred, instr);
      if(shred->is_done) {
// is this test good ?
        if(shreduler_remove(vm->shreduler, shred, 1) < 0)
          break;
//  shreduler_remove(vm->shreduler, shred, 1);
      }
#ifdef DEBUG_STACK
      /*  else */
      debug_msg("stack", "shred[%i] mem[%i] reg[%i]", shred->xid, shred->mem_index, shred->reg_index);
#endif
#ifdef DEBUG_VM
      /*  else */
      debug_msg("vm", "shred [%i]: stack: {%i:%i}. pc: (%i,%i / %i)", shred->xid, *shred->reg,
                *shred->mem, shred->pc, shred->next_pc, vector_size(shred->code->instr));
#endif
    }
  }
  if(!vm->is_running)
    return;
  for(i = 0; i < vector_size(vm->ugen); i++) {
    UGen u = (UGen)vector_at(vm->ugen, i);
    u->done = 0;
    if(u->channel) {
      for(int j = 0; j < u->n_in; j++)
        u->channel[j]->ugen->done = 0;
      for(int j = 0; j < u->n_out; j++) {
        u->channel[j]->ugen->done = 0;
      }
    }
  }
  ugen_compute(vm->blackhole->ugen);
  ugen_compute(vm->dac->ugen);
}

