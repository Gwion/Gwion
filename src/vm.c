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
#include "context.h"

Env new_env() {
  Env env = malloc(sizeof(struct Env_));
  env->global_context = new_context(NULL, "global_context");
  env->context = env->global_context;
  env->global_nspc = new_nspc("global_nspc", "global_nspc");
//  env->user_nspc = NULL;
  vector_init(&env->breaks);
  vector_init(&env->conts);
  vector_init(&env->contexts);
  vector_init(&env->class_stack);
  vector_init(&env->nspc_stack);
  map_init(&env->known_ctx);
  env->type_xid = te_last; // ????????
  env->do_type_xid = 0;
  env_reset(env);
  return env;
}

void env_reset(Env env) {
  vector_clear(&env->nspc_stack);
  vector_add(&env->nspc_stack, (vtype)env->global_nspc);

//  if(env->user_nspc)
//    vector_add(env->nspc_stack, (vtype)env->user_nspc);

  vector_clear(&env->class_stack);
  vector_add(&env->class_stack, (vtype)NULL);

//  if(env->user_nspc)
//    env->curr = env->user_nspc;
//  else
  env->curr = env->global_nspc;
  env->class_def = NULL;
  env->func = NULL;
  env->class_scope = 0;
}

void free_env(Env a) {
  m_uint i;
  free(a->global_context->tree);
  for(i = 0; i < map_size(&a->known_ctx); i++) {
    Context ctx = (Context)map_at(&a->known_ctx, i);
    REM_REF(ctx);
  }
  vector_release(&a->contexts);
  map_release(&a->known_ctx);

  for(i = 0; i < vector_size(&a->nspc_stack); i++) {
    Nspc  nspc = (Nspc)vector_pop(&a->nspc_stack);
    REM_REF(nspc);
  }
  vector_release(&a->nspc_stack);
  vector_release(&a->class_stack);
  vector_release(&a->breaks);
  vector_release(&a->conts);
  free(a);
}

m_bool env_add_type(Env env, Type type) {
  if(type->name[0] != '@')
    CHECK_BB(name_valid(type->name));
  Type v_type = type_copy(env, &t_class);
  v_type->d.actual_type = type;
  INIT_OO(type, e_type_obj);
  SET_FLAG(type, ae_flag_builtin);
  nspc_add_type(env->curr, insert_symbol(type->name), type);
  Value v = new_value(v_type, type->name);
  SET_FLAG(v, ae_flag_checked | ae_flag_const | ae_flag_global | ae_flag_builtin);
  nspc_add_value(env->curr, insert_symbol(type->name), v);
  type->owner = env->curr;
  if(env->do_type_xid) {
    env->type_xid++;
    type->xid = env->type_xid;
  }
  return 1;
}

m_bool env_push_class(Env env, Type type) {
  vector_add(&env->nspc_stack, (vtype)env->curr);
  env->curr = type->info;
  vector_add(&env->class_stack, (vtype)env->class_def);
  env->class_def = type;
  env->class_scope = 0;
  return 1;
}

m_bool env_pop_class(Env env) {
  env->class_def = (Type)vector_pop(&env->class_stack);
  env->curr = (Nspc)vector_pop(&env->nspc_stack);
  return 1;
}
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

m_bool init_bbq(VM* vm, DriverInfo* di, Driver** driver) {
  Driver* d;

  if(!(d = di->func(vm)) || d->ini(vm, di) < 0)
    return -1; // LCOV_EXCL_LINE
  sp_createn(&vm->sp, di->out);
  free(vm->sp->out);
  vm->sp->out   = calloc(di->out, sizeof(SPFLOAT));
  vm->in   = calloc(di->in, sizeof(SPFLOAT));
  vm->n_in = di->in;
  vm->sp->sr = di->sr;
  *driver = d;
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
#ifdef DEBUG_SHREDULER
  debug_msg("clock", "get");
#endif
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

void shreduler_remove(Shreduler s, VM_Shred out, m_bool erase) {
#ifdef DEBUG_SHREDULER
  debug_msg("clock", "%s %i", erase ? "erase" : "wait", out ? out->xid : -1);
#endif
  m_uint i, size = out->child.ptr ? vector_size(&out->child) : 0;
  if(erase) {
    vtype index;
    if(out->parent) {
      index = vector_find(&out->parent->child, (vtype)out);
      vector_rem(&out->parent->child, index);
      if(!vector_size(&out->parent->child)) {
        vector_release(&out->parent->child);
        out->parent->child.ptr = NULL;
      }
    }
    if(out->child.ptr) {
      for(i = 0; i < size; i++) {
        VM_Shred child = (VM_Shred)vector_front(&out->child);
        child->prev = NULL;
        child->next = NULL;
        if(child == s->list) // 09/03/17
          s->list = NULL;
        shreduler_remove(s, child, 1);
      }
    }
    index = vector_find(&s->vm->shred, (vtype)out);
    vector_rem(&s->vm->shred, index);
// GC
    if(out->gc.ptr) {
      for(i = 0; i < vector_size(&out->gc); i++) {
        M_Object o = (M_Object)vector_at(&out->gc, i);
        if(o)
          release(o, out);
      }
      vector_release(&out->gc);
    }
  }
  s->curr = (s->curr == out) ? s->curr : NULL;
  if(!out->prev && !out->next && out != s->list) {
//    if(!out->wait && !out->child && erase)
    if(erase && !out->wait && !out->child.ptr && !strstr(out->code->name, "class ")) // if fails in ctor. creates leak
      free_vm_shred(out);
    return;
  }
  out->prev ? (out->prev->next = out->next) : (s->list = out->next);
  if(out->next)
    out->next->prev = out->prev;
  out->next = out->prev = NULL;
  return;
}

m_bool shredule(Shreduler s, VM_Shred shred, m_float wake_time) {
#ifdef DEBUG_SHREDULER
  debug_msg("clock", "shredule shred[%i] at %f", shred->xid, wake_time);
#endif
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
  sp_destroy(&vm->sp);
  free(vm->in);
  free_shreduler(vm->shreduler);
  free(vm);
}

void vm_add_shred(VM* vm, VM_Shred shred) {
  shred->vm_ref = vm;
  if(!shred->xid) {
    vector_add(&vm->shred, (vtype)shred);
    shred->xid = vm->shreduler->n_shred++;
  }
  shredule(vm->shreduler, shred, .5);
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
    while(vm->shreduler->curr) {
      shred->pc = shred->next_pc++;
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
      if(!shred->me) {
       vm->shreduler->curr = NULL;
       shreduler_remove(vm->shreduler, shred, 1);
       break;
      }
    }
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
      for(j = u->n_chan; --j;) // miss + 1
        UGEN(u->channel[j - 1])->done = 0;
    }
  }
  ugen_compute(UGEN(vm->blackhole));
  ugen_compute(UGEN(vm->dac));
}

