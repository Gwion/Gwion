#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "import.h"
#include "map.h"
#include "object.h"

static CTOR(event_ctor) {
  EV_SHREDS(o) = new_vector();
}

static DTOR(event_dtor) {
  free_vector(EV_SHREDS(o));
}

static INSTR(EventWait) { GWDEBUG_EXE
  POP_REG(shred, SZ_FLOAT);
  const M_Object event = *(M_Object*)REG(-SZ_INT);
  if(!event)
    Except(shred, "NullEventWait");
  shreduler_remove(shred->vm->shreduler, shred, 0);
  const Vector v = EV_SHREDS(event);
  vector_add(v, (vtype)shred);
  *(m_int*)REG(-SZ_INT) = 1;
  _release(event, shred);
}

static MFUN(event_signal) {
  const Vector v = EV_SHREDS(o);
  const VM_Shred sh = (VM_Shred)vector_front(v);
  if(sh) {
    shredule(shred->vm->shreduler, sh, .5);
    vector_rem(v, 0);
  }
}

ANN void broadcast(const M_Object o) {
  for(m_uint i = 0; i < vector_size(EV_SHREDS(o)); i++) {
    const VM_Shred sh = (VM_Shred)vector_at(EV_SHREDS(o), i);
    shredule(sh->vm->shreduler, sh, .5);
  }
  vector_clear(EV_SHREDS(o));
}

static MFUN(event_broadcast) {
  broadcast(o);
}

GWION_IMPORT(event) {
  t_event = gwi_mk_type(gwi, "Event", SZ_INT, t_object );
  CHECK_BB(gwi_class_ini(gwi,  t_event, event_ctor, event_dtor))
  CHECK_BB(gwi_item_ini(gwi, "int", "@shreds"))
  CHECK_BB(gwi_item_end(gwi, ae_flag_member, NULL))
  CHECK_BB(gwi_func_ini(gwi, "void", "signal", event_signal))
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_func_ini(gwi, "void", "broadcast", event_broadcast))
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  CHECK_BB(gwi_oper_ini(gwi, "Event", "@now", "int"))
  _CHECK_OP(chuck, rassign, EventWait)
  return 1;
}

#ifdef JIT
#include "ctrl/event.h"
#include "code/event.h"
#endif

