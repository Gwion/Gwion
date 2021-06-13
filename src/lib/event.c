#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

static CTOR(event_ctor) { vector_init(&EV_SHREDS(o)); }

static DTOR(event_dtor) { vector_release(&EV_SHREDS(o)); }

static INSTR(EventWait) {
  POP_REG(shred, SZ_FLOAT);
  const M_Object event = *(M_Object *)REG(-SZ_INT);
  shreduler_remove(shred->tick->shreduler, shred, false);
  const Vector v = &EV_SHREDS(event);
  vector_add(v, (vtype)shred);
  *(m_int *)REG(-SZ_INT) = 1;
}

static MFUN(event_signal) {
  const Vector   v  = &EV_SHREDS(o);
  const VM_Shred sh = (VM_Shred)vector_front(v);
  if (sh) {
    shredule(sh->tick->shreduler, sh, GWION_EPSILON);
    vector_rem(v, 0);
  }
}

ANN void broadcast(const M_Object o) {
  for (m_uint i = 0; i < vector_size(&EV_SHREDS(o)); i++) {
    const VM_Shred sh = (VM_Shred)vector_at(&EV_SHREDS(o), i);
    shredule(sh->tick->shreduler, sh, GWION_EPSILON);
  }
  vector_clear(&EV_SHREDS(o));
}

static MFUN(event_broadcast) { broadcast(o); }

GWION_IMPORT(event) {
  const Type t_event = gwi_class_ini(gwi, "Event", "Object");
  gwi_class_xtor(gwi, event_ctor, event_dtor);
  gwi->gwion->type[et_event] = t_event; // use func

  GWI_BB(gwi_item_ini(gwi, "@internal", "@shreds"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
  GWI_BB(gwi_func_ini(gwi, "void", "signal"))
  GWI_BB(gwi_func_end(gwi, event_signal, ae_flag_none))
  GWI_BB(gwi_func_ini(gwi, "void", "broadcast"))
  GWI_BB(gwi_func_end(gwi, event_broadcast, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))
  GWI_BB(gwi_oper_ini(gwi, "Event", "@now", "int"))
  GWI_BB(gwi_oper_end(gwi, "=>", EventWait))
  return GW_OK;
}
