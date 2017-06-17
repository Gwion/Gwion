#include "import.h"
#include "shreduler.h"

struct Type_ t_event = { "Event", SZ_INT, &t_object, te_event };

m_int o_event_shred;

static void event_ctor(M_Object o, VM_Shred shred) {
  EV_SHREDS(o) = new_vector();
}

static void event_dtor(M_Object o, VM_Shred shred) {
  free_vector(EV_SHREDS(o));
}

INSTR(Time_Advance) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "time advance %f %f", *(m_float*)(shred->reg - SZ_FLOAT*2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  shred->wake_time += *(m_float*)shred->reg;
  shred->is_running = 0;
  shredule(vm->shreduler, shred, shred->wake_time);
  *(m_float*)shred->reg = shred->wake_time;
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(Event_Wait) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "event wait: blocking shred %i", shred->xid);
#endif
  M_Object event;
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  event = *(M_Object*)shred->reg;
  if(!event) {
    Except(shred, "NullEventWait");
  }
  shred->wait = event;
  shreduler_remove(vm->shreduler, shred, 0);
  Vector v = EV_SHREDS(event);
  vector_add(v, (vtype)shred);
  shred->next_pc++;
  *(m_int*)shred->reg = 1;
  PUSH_REG(shred, SZ_INT);
  release(event, shred);
}

static MFUN(event_signal) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "event signal");
#endif
  VM_Shred sh;
  Vector v = EV_SHREDS(o);
  RETURN->d.v_uint = vector_size(v);
  if(!RETURN->d.v_uint) {
    Except(shred, "NullEventSignal");
  }
  sh = (VM_Shred)vector_front(v);
  sh->wait = NULL;
  shredule(shred->vm_ref->shreduler, sh, get_now(shred->vm_ref->shreduler) + .5);
  vector_rem(v, 0);
}

void broadcast(M_Object o) {
  m_uint i;
  VM_Shred sh;
  for(i = 0; i < vector_size(EV_SHREDS(o)); i++) {
    sh = (VM_Shred)vector_at(EV_SHREDS(o), i);
    sh->wait = NULL;
    shredule(sh->vm_ref->shreduler, sh, get_now(sh->vm_ref->shreduler) + .5);
  }
  vector_clear(EV_SHREDS(o));
}

static MFUN(event_broadcast) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "event signal");
#endif
  broadcast(o);
}

m_bool import_event(Env env) {
  DL_Func* fun;
  CHECK_OB(import_class_begin(env, &t_event, env->global_nspc, event_ctor, event_dtor))
  o_event_shred = import_mvar(env, "int", "@shreds", 0, 0);
  CHECK_BB(o_event_shred);
  fun = new_dl_func("int", "signal", (m_uint)event_signal);
  CHECK_OB(import_mfun(env, fun))
  fun = new_dl_func("int", "broadcast", (m_uint)event_broadcast);
  CHECK_OB(import_mfun(env, fun))
  CHECK_BB(import_op(env, op_chuck, "Event", "@now", "int", Event_Wait, 1))
  CHECK_BB(import_class_end(env))
  return 1;
}
