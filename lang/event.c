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
  POP_REG(shred, SZ_FLOAT * 2);
  shred->wake_time += *(m_float*)REG(0);
  shred->is_running = 0;
  shredule(vm->shreduler, shred, shred->wake_time);
  *(m_float*)REG(0) = shred->wake_time;
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(Event_Wait) {
  M_Object event;
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  event = *(M_Object*)REG(0);
  if(!event) {
    Except(shred, "NullEventWait");
  }
  shred->wait = event;
  shreduler_remove(vm->shreduler, shred, 0);
  Vector v = EV_SHREDS(event);
  vector_add(v, (vtype)shred);
  shred->next_pc++;
  *(m_int*)REG(0) = 1;
  PUSH_REG(shred, SZ_INT);
  release(event, shred);
}

static MFUN(event_signal) {
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
  broadcast(o);
}

m_bool import_event(Env env) {
  DL_Func fun;
  CHECK_BB(import_class_begin(env, &t_event, env->global_nspc, event_ctor, event_dtor))
  o_event_shred = import_var(env, "int", "@shreds", 0, NULL);
  CHECK_BB(o_event_shred);
  dl_func_init(&fun, "int", "signal", (m_uint)event_signal);
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "int", "broadcast", (m_uint)event_broadcast);
  CHECK_BB(import_fun(env, &fun, 0))
  CHECK_BB(import_op(env, op_chuck, "Event", "@now", "int", Event_Wait, 1))
  CHECK_BB(import_class_end(env))
  return 1;
}
