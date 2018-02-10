#include "type.h"
#include "instr.h"
#include "import.h"

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
  m_float f = *(m_float*)REG(0);
  *(m_float*)REG(0) = (shred->wake_time += f);
  shredule(vm->shreduler, shred, f);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(Event_Wait) {
  M_Object event;
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  event = *(M_Object*)REG(0);
  if(!event)
    Except(shred, "NullEventWait");
  shreduler_remove(vm->shreduler, shred, 0);
  Vector v = EV_SHREDS(event);
  vector_add(v, (vtype)shred);
  *(m_int*)REG(0) = 1;
  PUSH_REG(shred, SZ_INT);
  release(event, shred);
}

static MFUN(event_signal) {
  VM_Shred sh;
  Vector v = EV_SHREDS(o);
  *(m_uint*)RETURN = vector_size(v);
  if(!*(m_uint*)RETURN)
    Except(shred, "NullEventSignal");
  sh = (VM_Shred)vector_front(v);
  shredule(shred->vm_ref->shreduler, sh, .5);
  vector_rem(v, 0);
}

void broadcast(M_Object o) {
  m_uint i;
  VM_Shred sh;
  for(i = 0; i < vector_size(EV_SHREDS(o)); i++) {
    sh = (VM_Shred)vector_at(EV_SHREDS(o), i);
    shredule(sh->vm_ref->shreduler, sh, .5);
  }
  vector_clear(EV_SHREDS(o));
}

static MFUN(event_broadcast) {
  broadcast(o);
}

m_bool import_event(Gwi gwi) {
  CHECK_BB(gwi_class_ini(gwi,  &t_event, event_ctor, event_dtor))
	gwi_item_ini(gwi, "int", "@shreds");
  o_event_shred = gwi_item_end(gwi, ae_flag_member, NULL);
  CHECK_BB(o_event_shred);
  gwi_func_ini(gwi, "int", "signal", event_signal);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "broadcast", event_broadcast);
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  CHECK_BB(gwi_oper_ini(gwi, "Event", "@now", "int"))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi,  op_chuck, Event_Wait))
  return 1;
}
