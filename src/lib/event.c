#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "import.h"

m_int o_event_shred;

static CTOR(event_ctor) {
  EV_SHREDS(o) = new_vector();
}

static DTOR(event_dtor) {
  free_vector(EV_SHREDS(o));
}

static INSTR(Event_Wait) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  const M_Object event = *(M_Object*)REG(0);
  if(!event)
    Except(shred, "NullEventWait");
  shreduler_remove(vm->shreduler, shred, 0);
  const Vector v = EV_SHREDS(event);
  vector_add(v, (vtype)shred);
  *(m_int*)REG(0) = 1;
  PUSH_REG(shred, SZ_INT);
  release(event, shred);
}

static MFUN(event_signal) {
  const Vector v = EV_SHREDS(o);
  const VM_Shred sh = (VM_Shred)vector_front(v);
  if(sh)
    shredule(shred->vm_ref->shreduler, sh, .5);
  *(m_uint*)RETURN = sh ? 1 : 0;
  vector_rem(v, 0);
}

ANN void broadcast(const M_Object o) {
  for(m_uint i = 0; i < vector_size(EV_SHREDS(o)); i++) {
    VM_Shred sh = (VM_Shred)vector_at(EV_SHREDS(o), i);
    shredule(sh->vm_ref->shreduler, sh, .5);
  }
  vector_clear(EV_SHREDS(o));
}

static MFUN(event_broadcast) {
  broadcast(o);
}

m_bool import_event(Gwi gwi) {
  CHECK_OB((t_event = gwi_mk_type(gwi, "Event", SZ_INT, t_object )))
  CHECK_BB(gwi_class_ini(gwi,  t_event, event_ctor, event_dtor))
  CHECK_BB(gwi_item_ini(gwi, "int", "@shreds"))
  CHECK_BB((o_event_shred = gwi_item_end(gwi, ae_flag_member, NULL)))
  CHECK_BB(gwi_func_ini(gwi, "int", "signal", event_signal))
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_func_ini(gwi, "int", "broadcast", event_broadcast))
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  CHECK_BB(gwi_oper_ini(gwi, "Event", "@now", "int"))
  _CHECK_OP(chuck, rassign, Event_Wait)
  return 1;
}
