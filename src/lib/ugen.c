#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"
#include "ugen.h"

struct Type_ t_ugen = { "UGen", SZ_INT, &t_object, te_ugen };
m_int o_object_ugen;

m_bool base_tick(UGen u) {
  UGen ugen;
  m_uint i, size = vector_size(&u->ugen);
  if(!size) {
    u->out = 0;
    return 1;
  }
  ugen = (UGen)vector_at(&u->ugen, 0);
  u->out = ugen->out;
  for(i = 1; i < size; i++) {
    ugen = (UGen)vector_at(&u->ugen, i);
    switch(u->op) {
      case 1:
       u->out += ugen->out;
       break;
      case 2:
        u->out -= ugen->out;
       break;
      case 3:
        u->out *= ugen->out;
       break;
      case 4:
        u->out /= ugen->out;
       break;
    }
  }
  u->in = u->out;
  return 1;
}

m_bool dac_tick(UGen u) {
  m_uint  i;
  VM* vm = (VM*)u->ug;
  for(i = u->n_out + 1; --i;)
    vm->sp->out[i - 1] = UGEN(u->channel[i - 1])->out;
  return 1;
}

m_bool adc_tick(UGen u) {
  m_uint  i;
  m_float last = 0;
  VM* vm = (VM*)u->ug;
  for(i = u->n_out + 1; --i;) {
    m_uint j = i - 1;
    M_Object obj = u->channel[j];
    UGEN(obj)->last = vm->in[j];
    last += (UGEN(obj)->out = vm->in[j]);
  }
  u->last = last;
  return 1;
}

void ugen_compute(UGen u) {
  m_uint  i;
  UGen ugen;
  if(u->done)
    return;
  u->done = 1;
  if(u->channel)
    for(i = u->n_chan + 1; --i;)
      ugen_compute(UGEN(u->channel[i - 1]));
  else for(i = vector_size(&u->ugen) + 1; --i;)
      ugen_compute((UGen)vector_at(&u->ugen, i - 1));
  if(u->ref) {
    for(i = u->ref->n_chan + 1; --i;) {
      ugen = UGEN(u->ref->channel[i - 1]);
      ugen->tick(ugen);
    }
    u->ref->tick(u->ref);
    return;
  }
  u->tick(u);
  u->last = u->out;
}

UGen new_UGen() {
  UGen u = (UGen) calloc(1, sizeof(struct UGen_));
  vector_init(&u->to);
  u->tick = base_tick;
  u->op = 1;
  return u;
}

M_Object new_M_UGen() {
  M_Object o = new_M_Object(NULL);
  initialize_object(o, &t_ugen);
  UGEN(o) = new_UGen();
  return o;
}

static m_bool assign_channel(UGen u) {
  m_uint i;
  u->channel = malloc(u->n_chan * sizeof(M_Object));
  for(i = u->n_chan + 1; --i;) {
    m_uint j = i - 1;
    M_Object chan = new_M_UGen();
    assign_ugen(UGEN(chan), u->n_in > j, u->n_out > j, 0, NULL);
    UGEN(chan)->ref = u;
    u->channel[j] =  chan;
  }
  return 1;
}

m_bool assign_ugen(UGen u, m_uint n_in, m_uint n_out, m_bool trig, void* ug) {
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

static void release_connect(VM_Shred shred) {
  release(*(M_Object*)REG(0), shred);
  release(*(M_Object*)REG(SZ_INT), shred);
  *(M_Object*)REG(0) = *(M_Object*)REG(SZ_INT);
  PUSH_REG(shred, SZ_INT);
}

static void connect(UGen lhs, UGen rhs) {
  vector_add(&rhs->ugen, (vtype)lhs);
  vector_add(&lhs->to,   (vtype)rhs);
}
  
static void disconnect(UGen lhs, UGen rhs) {
  vector_rem(&rhs->ugen, vector_find(&rhs->ugen, (vtype)lhs));
  vector_rem(&lhs->to,   vector_find(&lhs->to,   (vtype)rhs));
}

static m_bool connect_init(VM_Shred shred, M_Object* lhs, M_Object* rhs) {
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

static void do_connect(UGen lhs, UGen rhs) {
  if(rhs->channel) {
    m_uint i;
    for(i = rhs->n_out + 1; --i;) {
      m_uint j = i - 1;
      M_Object obj = rhs->channel[j];
      if(lhs->n_out > 1)
        connect(UGEN(lhs->channel[j % lhs->n_out]), UGEN(obj));
      else
        connect(lhs, UGEN(obj));
    }
  } else
    connect(lhs, rhs);
}

static void do_disconnect(UGen lhs, UGen rhs) {
  m_uint i;
  if(rhs->channel) {
    for(i = rhs->n_out + 1; --i;)
      disconnect(lhs, UGEN(rhs->channel[i - 1]));
  } else
    disconnect(lhs, rhs);
} 

static INSTR(ugen_connect) {
  M_Object lhs, rhs;

  if(connect_init(shred, &lhs, &rhs) < 0)
    return;
  if(UGEN(rhs)->n_in)
    do_connect(UGEN(lhs), UGEN(rhs));
  release_connect(shred);
}

static INSTR(ugen_disconnect) {
  M_Object lhs, rhs;
  if(connect_init(shred, &lhs, &rhs) < 0)
    return;
  if(UGEN(rhs)->n_in)
    do_disconnect(UGEN(lhs), UGEN(rhs));
  release_connect(shred);
} 

static INSTR(trig_connect) {
  M_Object lhs, rhs;

  if(connect_init(shred, &lhs, &rhs) < 0)
    return;
  if(UGEN(rhs)->trig)
    connect(UGEN(lhs), UGEN(UGEN(rhs)->trig));
  release_connect(shred);
}

static INSTR(trig_disconnect) {
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

static void ugen_unref(UGen ug, VM_Shred shred) {
  m_uint i;
  for(i = vector_size(&ug->to) + 1; --i;) {
    UGen u = (UGen)vector_at(&ug->to, i - 1);
    if(u->ugen.ptr) {
      m_int index = vector_find(&u->ugen, (vtype)ug);
      if(index > -1)
        vector_rem(&u->ugen, index);
    }
  }
}

static void ugen_release(UGen ug, VM_Shred shred) {
  m_uint i;
  if(ug->ugen.ptr) {
    for(i = vector_size(&ug->ugen) + 1; --i;) {
      UGen u = (UGen)vector_at(&ug->ugen, i - 1);
      m_int index = vector_find(&u->to, (vtype)ug);
      if(index > -1)
        vector_rem(&u->to, index);
    }
    vector_release(&ug->ugen);
  } else {
    for(i = ug->n_chan + 1; --i;)
      release(ug->channel[i - 1], shred);
    free(ug->channel);
  }
}

static DTOR(ugen_dtor) {
  UGen ug = UGEN(o);
  m_int j = vector_find(&shred->vm_ref->ugen, (vtype)ug);
  
  vector_rem(&shred->vm_ref->ugen, j);
  ugen_unref(ug, shred);
  ugen_release(ug, shred);
  release(ug->trig, shred);
  vector_release(&ug->to);
  free(ug);
}

static MFUN(ugen_channel) {
  m_int i = *(m_int*)MEM(SZ_INT);
  if(!UGEN(o)->channel)
    *(M_Object*)RETURN = !i ? o : NULL;
  else if(i < 0 || i >= UGEN(o)->n_chan)
    *(M_Object*)RETURN = NULL;
  else
    *(M_Object*)RETURN = UGEN(o)->channel[i];
}

static MFUN(ugen_get_op) {
  *(m_uint*)RETURN = UGEN(o)->op;
}

static MFUN(ugen_set_op) {
  m_int i = *(m_int*)MEM(SZ_INT);
  if(i < 1 || i > 4)
    err_msg(INSTR_, 0, "invalid op %i", i);
  else
    UGEN(o)->op = i;
  *(m_uint*)RETURN = UGEN(o)->op;
}

static MFUN(ugen_get_last) {
  *(m_float*)RETURN = UGEN(o)->last;
}

static m_bool import_global_ugens(Importer importer) {
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

  importer_add_value(importer, "adc",        &t_ugen, 1, vm->adc);
  importer_add_value(importer, "dac",        &t_ugen, 1, vm->dac);
  importer_add_value(importer, "blackhole",  &t_ugen, 1, vm->blackhole);
  return 1;
}

m_bool import_ugen(Importer importer) {
  CHECK_BB(importer_class_begin(importer,  &t_ugen, ugen_ctor, ugen_dtor))

  o_object_ugen = importer_add_var(importer,  "int", "@ugen", ae_flag_member, NULL);
  CHECK_BB(o_object_ugen)

  importer_func_begin(importer, "UGen", "chan", (m_uint)ugen_channel);
  importer_add_arg(importer, "int", "arg0");
  CHECK_BB(importer_add_fun(importer, 0))

  importer_func_begin(importer, "int", "op", (m_uint)ugen_get_op);
  CHECK_BB(importer_add_fun(importer, 0))

  importer_func_begin(importer, "int", "op", (m_uint)ugen_set_op);
  importer_add_arg(importer, "int", "arg0");
  CHECK_BB(importer_add_fun(importer, 0))

  importer_func_begin(importer, "float", "last", (m_uint)ugen_get_last);
  CHECK_BB(importer_add_fun(importer, 0))

  CHECK_BB(importer_oper_begin(importer, "UGen", "UGen", "UGen"))
  CHECK_BB(importer_add_op(importer, op_chuck,   ugen_connect, 1))
  CHECK_BB(importer_add_op(importer, op_unchuck, ugen_disconnect, 1))
  CHECK_BB(importer_add_op(importer, op_trig,    trig_connect, 1))
  CHECK_BB(importer_add_op(importer, op_untrig,  trig_disconnect, 1))

  CHECK_BB(importer_class_end(importer))

  CHECK_BB(import_global_ugens(importer))
  return 1;
}
