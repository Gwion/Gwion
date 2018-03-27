#include <stdlib.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"
#include "ugen.h"
#include "mpool.h"
POOL_HANDLE(UGen, 256)
m_int o_object_ugen;

static TICK(dac_tick) {
  const VM* vm = (VM*)u->ug;
  for(m_uint i = u->n_out + 1; --i;)
    vm->sp->out[i - 1] = UGEN(u->channel[i - 1])->out;
}

static TICK(adc_tick) {
  const VM* vm = (VM*)u->ug;
  u->last = 0;
  for(m_uint i = u->n_out + 1; --i;) {
    const m_uint j = i - 1;
    const M_Object obj = u->channel[j];
    UGEN(obj)->last = vm->in[j];
    u->last += (UGEN(obj)->out = vm->in[j]);
  }
}

__attribute__((hot, nonnull))
void ugen_compute(const UGen u) {
  if(u->ref && !u->ref->done) {
    ugen_compute(u->ref);
    return;
  }
  u->done = 1;
  if(u->channel) {
    for(m_uint i = u->n_chan + 1; --i;) {
      const UGen v = UGEN(u->channel[i - 1]);
      if(!v->done)
        ugen_compute(v);
    }
  } else {
    const m_uint size = vector_size(&u->ugen);
    if(size) {
      const UGen v = (UGen)vector_at(&u->ugen, size - 1);
      if(!v->done)
        ugen_compute(v);
      u->in = v->out;
      for(m_uint i = size; --i;) {
        const UGen v = (UGen)vector_at(&u->ugen, i - 1);
        if(!v->done)
          ugen_compute(v);
        u->op(u, v->out);
      }
    }
  }
  u->tick(u);
  u->last = u->out;
}

ANEW static UGen new_UGen() {
  UGen u = mp_alloc(UGen);
  vector_init(&u->to);
  u->tick = base_tick;
  u->op = ugop_plus;
  return u;
}

M_Object new_M_UGen() {
  const M_Object o = new_M_Object(NULL);
  initialize_object(o, t_ugen);
  UGEN(o) = new_UGen();
  return o;
}

ANN static m_bool assign_channel(const UGen u) {
  u->channel = (M_Object*)xmalloc(u->n_chan * SZ_INT);
  for(m_uint i = u->n_chan + 1; --i;) {
    const m_uint j = i - 1;
    const M_Object chan = new_M_UGen();
    assign_ugen(UGEN(chan), u->n_in > j, u->n_out > j, 0, NULL);
    UGEN(chan)->ref = u;
    u->channel[j] =  chan;
  }
  return 1;
}

__attribute__((nonnull(1)))
m_bool assign_ugen(const UGen u, const m_uint n_in, const m_uint n_out, const m_bool trig, void* ug) {
  u->n_chan = n_in > n_out ? n_in : n_out;
  u->in = u->out = 0;
  u->n_in   = n_in;
  u->n_out  = n_out;
  u->ug     = ug;
  if(u->n_chan > 1)
    CHECK_BB(assign_channel(u))
  else
    vector_init(&u->ugen);
  if(trig) {
    u->trig = new_M_UGen();
    assign_ugen(UGEN(u->trig), 1, 1, 0, NULL);
  }
  return 1;
}

ANN static void release_connect(const VM_Shred shred) {
  release(*(M_Object*)REG(0), shred);
  release(*(M_Object*)REG(SZ_INT), shred);
  *(M_Object*)REG(0) = *(M_Object*)REG(SZ_INT);
  PUSH_REG(shred, SZ_INT);
}

ANN static inline void connect(const UGen lhs, const UGen rhs) {
  vector_add(&rhs->ugen, (vtype)lhs);
  vector_add(&lhs->to,   (vtype)rhs);
}

ANN static inline void disconnect(const UGen lhs, const UGen rhs) {
  vector_rem(&rhs->ugen, vector_find(&rhs->ugen, (vtype)lhs));
  vector_rem(&lhs->to,   vector_find(&lhs->to,   (vtype)rhs));
}

ANN static m_bool connect_init(const VM_Shred shred, M_Object* lhs, M_Object* rhs) {
  POP_REG(shred, SZ_INT * 2);
  *lhs = *(M_Object*)REG(0);
  *rhs = *(M_Object*)REG(SZ_INT);
  if(!*lhs || !*rhs) {
    release(*lhs, shred);
    release(*rhs, shred);
    NullException(shred, "UgenConnectException");
    return -1;
  }
  return 1;
}

ANN static void do_connect(const UGen lhs, const UGen rhs) {
  if(rhs->channel) {
    for(m_uint i = rhs->n_out + 1; --i;) {
      const m_uint j = i - 1;
      const M_Object obj = rhs->channel[j];
      if(lhs->n_out > 1)
        connect(UGEN(lhs->channel[j % lhs->n_out]), UGEN(obj));
      else
        connect(lhs, UGEN(obj));
    }
  } else
    connect(lhs, rhs);
}

ANN static void do_disconnect(const UGen lhs, const UGen rhs) {
  if(rhs->channel) {
    for(m_uint i = rhs->n_out + 1; --i;)
      disconnect(lhs, UGEN(rhs->channel[i - 1]));
  } else
    disconnect(lhs, rhs);
}

static INSTR(ugen_connect) { GWDEBUG_EXE
  M_Object lhs, rhs;

  if(connect_init(shred, &lhs, &rhs) < 0)
    return;
  if(UGEN(rhs)->n_in)
    do_connect(UGEN(lhs), UGEN(rhs));
  release_connect(shred);
}

static INSTR(ugen_disconnect) { GWDEBUG_EXE
  M_Object lhs, rhs;

  if(connect_init(shred, &lhs, &rhs) < 0)
    return;
  if(UGEN(rhs)->n_in)
    do_disconnect(UGEN(lhs), UGEN(rhs));
  release_connect(shred);
}

static INSTR(trig_connect) { GWDEBUG_EXE
  M_Object lhs, rhs;

  if(connect_init(shred, &lhs, &rhs) < 0)
    return;
  if(UGEN(rhs)->trig)
    connect(UGEN(lhs), UGEN(UGEN(rhs)->trig));
  release_connect(shred);
}

static INSTR(trig_disconnect) { GWDEBUG_EXE
  M_Object lhs, rhs;

  if(connect_init(shred, &lhs, &rhs) < 0)
    return;
  if(UGEN(rhs)->trig)
    disconnect(UGEN(lhs), UGEN(UGEN(rhs)->trig));
  release_connect(shred);
}

static CTOR(ugen_ctor) {
  UGEN(o) = new_UGen();
  vector_add(&shred->vm_ref->ugen, (vtype)UGEN(o));
}

ANN static void ugen_unref(const UGen ug) {
  for(m_uint i = vector_size(&ug->to) + 1; --i;) {
    const UGen u = (UGen)vector_at(&ug->to, i - 1);
    if(u->ugen.ptr) {
      m_int index = vector_find(&u->ugen, (vtype)ug);
      if(index > -1)
        vector_rem(&u->ugen, index);
    }
  }
}

ANN static void ugen_release(const UGen ug, const VM_Shred shred) {
  if(ug->ugen.ptr) {
    for(m_uint i = vector_size(&ug->ugen) + 1; --i;) {
      UGen u = (UGen)vector_at(&ug->ugen, i - 1);
      m_int index = vector_find(&u->to, (vtype)ug);
      if(index > -1)
        vector_rem(&u->to, index);
    }
    vector_release(&ug->ugen);
  } else {
    for(m_uint i = ug->n_chan + 1; --i;)
      release(ug->channel[i - 1], shred);
    free(ug->channel);
  }
}

static DTOR(ugen_dtor) {
  const UGen ug = UGEN(o);
  const m_int j = vector_find(&shred->vm_ref->ugen, (vtype)ug);

  vector_rem(&shred->vm_ref->ugen, j);
  ugen_unref(ug);
  ugen_release(ug, shred);
  release(ug->trig, shred);
  vector_release(&ug->to);
  mp_free(UGen, ug);
}

static MFUN(ugen_channel) {
  const m_int i = *(m_int*)MEM(SZ_INT);
  if(!UGEN(o)->channel)
    *(M_Object*)RETURN = !i ? o : NULL;
  else if(i < 0 || (m_uint)i >= UGEN(o)->n_chan)
    *(M_Object*)RETURN = NULL;
  else
    *(M_Object*)RETURN = UGEN(o)->channel[i];
}

static MFUN(ugen_get_op) {
  const f_ugop f = UGEN(o)->op;
  if(f == ugop_plus)
    *(m_uint*)RETURN = 1;
  else if(f == ugop_minus)
    *(m_uint*)RETURN = 2;
  else if(f == ugop_times)
    *(m_uint*)RETURN = 3;
  else if(f == ugop_divide)
    *(m_uint*)RETURN = 4;
}

static MFUN(ugen_set_op) {
  const m_int i = *(m_int*)MEM(SZ_INT);
  if(i == 1)
    UGEN(o)->op = ugop_plus;
  else if(i == 2)
    UGEN(o)->op = ugop_minus;
  else if(i == 3)
    UGEN(o)->op = ugop_times;
  else if(i == 4)
    UGEN(o)->op = ugop_divide;
  *(m_uint*)RETURN = i;
}

static MFUN(ugen_get_last) {
  *(m_float*)RETURN = UGEN(o)->last;
}

ANN static m_bool import_global_ugens(const Gwi gwi) {
  VM* vm = gwi_vm(gwi);

  vm->dac       = new_M_UGen();
  vm->adc       = new_M_UGen();
  vm->blackhole = new_M_UGen();

  assign_ugen(UGEN(vm->dac), 2, 2, 0, vm);
  assign_ugen(UGEN(vm->adc), 2, 2, 0, vm);
  assign_ugen(UGEN(vm->blackhole), 1, 1, 0, vm);
  UGEN(vm->dac)->tick = dac_tick;
  UGEN(vm->adc)->tick = adc_tick;
  vector_add(&vm->ugen, (vtype)UGEN(vm->blackhole));
  vector_add(&vm->ugen, (vtype)UGEN(vm->dac));
  vector_add(&vm->ugen, (vtype)UGEN(vm->adc));
  gwi_item_ini(gwi, "UGen", "adc");
  gwi_item_end(gwi, ae_flag_const, vm->adc);
  gwi_item_ini(gwi, "UGen", "dac");
  gwi_item_end(gwi, ae_flag_const, vm->dac);
  gwi_item_ini(gwi, "UGen", "blackhole");
  gwi_item_end(gwi, ae_flag_const, vm->blackhole);
  return 1;
}

static OP_CHECK(chuck_ugen) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  bin->lhs->emit_var = bin->rhs->emit_var = 0;
  return bin->rhs->type;
}

ANN m_bool import_ugen(const Gwi gwi) {
  CHECK_OB((t_ugen = gwi_mk_type(gwi, "UGen", SZ_INT, t_object)))
  CHECK_BB(gwi_class_ini(gwi,  t_ugen, ugen_ctor, ugen_dtor))
  CHECK_BB(gwi_item_ini(gwi, "int", "@ugen"))
  CHECK_BB((o_object_ugen = gwi_item_end(gwi, ae_flag_member, NULL)))

  CHECK_BB(gwi_func_ini(gwi, "UGen", "chan", ugen_channel))
  CHECK_BB(gwi_func_arg(gwi, "int", "arg0"))
  CHECK_BB(gwi_func_end(gwi, 0))

  CHECK_BB(gwi_func_ini(gwi, "int", "op", ugen_get_op))
  CHECK_BB(gwi_func_end(gwi, 0))

  CHECK_BB(gwi_func_ini(gwi, "int", "op", ugen_set_op))
  CHECK_BB(gwi_func_arg(gwi, "int", "arg0"))
  CHECK_BB(gwi_func_end(gwi, 0))

  CHECK_BB(gwi_func_ini(gwi, "float", "last", ugen_get_last))
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  CHECK_BB(gwi_oper_ini(gwi, "UGen", "UGen", "UGen"))
  CHECK_BB(gwi_oper_add(gwi, chuck_ugen))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,   ugen_connect))
  CHECK_BB(gwi_oper_add(gwi, chuck_ugen))
  CHECK_BB(gwi_oper_end(gwi, op_unchuck, ugen_disconnect))
  CHECK_BB(gwi_oper_add(gwi, chuck_ugen))
  CHECK_BB(gwi_oper_end(gwi, op_trig,    trig_connect))
  CHECK_BB(gwi_oper_add(gwi, chuck_ugen))
  CHECK_BB(gwi_oper_end(gwi, op_untrig,  trig_disconnect))

  CHECK_BB(import_global_ugens(gwi))
  return 1;
}
