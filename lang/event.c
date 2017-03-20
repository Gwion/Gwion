#include "err_msg.h"
//#include "vm.h"
//#include "type.h"
//#include "dl.h"
#include "import.h"
#include "shreduler.h"
//#include "bbq.h"
#include "lang.h"

struct Type_ t_event = { "Event", SZ_INT, &t_object, te_event };

m_int o_event_shred;

void event_ctor(M_Object o, VM_Shred shred)
{
  EV_SHREDS(o) = new_Vector();
}

void event_dtor(M_Object o, VM_Shred shred)
{
  free_Vector(EV_SHREDS(o));
}

INSTR(Event_Wait)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "event wait: blocking shred %i", shred->xid);
#endif
  M_Object event;
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  event = *(M_Object*)shred->reg;
  shred->wait = event;
  shreduler_remove(vm->shreduler, shred, 0);
  Vector v = EV_SHREDS(event);
  vector_append(v, (vtype)shred);
  shred->next_pc++;
  *(m_int*)shred->reg = 1;
  PUSH_REG(shred, SZ_INT);
  release(event, shred);
}

static MFUN(event_signal)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "event signal");
#endif
  VM_Shred sh;
  Vector v = EV_SHREDS(o);
  RETURN->d.v_uint = vector_size(v);
  sh = (VM_Shred)vector_front(v);
  sh->wait = NULL;
  shredule(shred->vm_ref->shreduler, sh, get_now(shred->vm_ref->shreduler) + .5);
  vector_remove(v, 0);
}

void broadcast(M_Object o)
{
  m_uint i;
  VM_Shred sh;
  for(i = 0; i < vector_size(EV_SHREDS(o)); i++) {
    sh = (VM_Shred)vector_at(EV_SHREDS(o), i);
    sh->wait = NULL;
    shredule(sh->vm_ref->shreduler, sh, get_now(sh->vm_ref->shreduler) + .5);
  }
  vector_clear(EV_SHREDS(o));
}

static MFUN(event_broadcast)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "event signal");
#endif
  broadcast(o);
}

m_bool import_event(Env env)
{
  DL_Func* fun;
  CHECK_BB(add_global_type(env, &t_event))
  CHECK_OB(import_class_begin(env, &t_event, env->global_nspc, event_ctor, event_dtor))
  env->class_def->doc = "Process event, with precise timing";
  o_event_shred = import_mvar(env, "int", "@shreds", 0, 0, "the place for blocked shreds");
  CHECK_BB(o_event_shred);
  fun = new_DL_Func("int", "signal", (m_uint)event_signal);
  CHECK_OB(import_mfun(env, fun))
  fun = new_DL_Func("int", "broadcast", (m_uint)event_broadcast);
  CHECK_OB(import_mfun(env, fun))
  CHECK_BB(add_binary_op(env, op_chuck,        &t_event, &t_now, &t_int, Event_Wait, 1, 0))
  CHECK_BB(import_class_end(env))
  return 1;
}
