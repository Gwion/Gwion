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
  m_float* out = ((VM*)u->ug)->sp->out;
  m_uint i = 0;
  do out[i] = UGEN(u->channel[i])->in;
  while(++i < u->n_out);
}

static TICK(adc_tick) {
  const m_float* in = ((VM*)u->ug)->in;
  m_uint i = 0;
  do UGEN(u->channel[i])->out = in[i];
  while(++i < u->n_out);
}

#define COMPUTE(a) if(!(a)->done)ugen_compute(a);
__attribute__((hot))
ANN void ugen_compute(const UGen u) {
  u->done = 1;
  u->last = u->out;
  if(!u->channel) {
    const m_uint size = vector_size(&u->from);
    if(size) {
      const UGen v = (UGen)vector_front(&u->from);
      COMPUTE(v)
      u->in = v->out;
      for(m_uint i = 1; i < size; ++i) {
        const UGen v = (UGen)vector_at(&u->from, i);
        COMPUTE(v)
        u->op(u, v->out);
      }
    }
    if(u->ref) {
      COMPUTE(u->ref)
      return;
    }
  } else {
    m_uint i = 0;
    do COMPUTE(UGEN(u->channel[i]))
    while(++i < u->n_chan);
  }
  u->tick(u);
}

ANEW static UGen new_UGen() {
  const UGen u = mp_alloc(UGen);
  u->op = ugop_plus;
  return u;
}

ANEW static M_Object new_M_UGen() {
  const M_Object o = new_M_Object(NULL);
  initialize_object(o, t_ugen);
  UGEN(o) = new_UGen();
  return o;
}

ANN void assign_channel(const UGen u) {
  u->channel = (M_Object*)xmalloc(u->n_chan * SZ_INT);
  for(m_uint i = u->n_chan + 1; --i;) {
    const m_uint j = i - 1;
    const M_Object chan = new_M_UGen();
    assign_ugen(UGEN(chan), u->n_in > j, u->n_out > j, NULL);
    UGEN(chan)->ref = u;
    u->channel[j] =  chan;
  }
}

ANN void assign_trig(const UGen u) {
  u->trig = new_M_UGen();
  UGEN(u->trig)->ref = u;
  assign_ugen(UGEN(u->trig), 1, 1, NULL);
}

ANN2(1) void assign_ugen(const UGen u, const m_uint n_in, const m_uint n_out, void* ug) {
  u->n_chan = n_in > n_out ? n_in : n_out;
  u->in = u->out = 0;
  u->n_in   = n_in;
  u->n_out  = n_out;
  u->ug     = ug;
  if(u->n_chan == 1) {
    vector_init(&u->from);
    vector_init(&u->to);
  } else
    assign_channel(u);
}

ANN static void release_connect(const VM_Shred shred) {
  release(*(M_Object*)REG(0), shred);
  release(*(M_Object*)REG(SZ_INT), shred);
  *(M_Object*)REG(0) = *(M_Object*)REG(SZ_INT);
  PUSH_REG(shred, SZ_INT);
}

ANN static inline void connect(const UGen lhs, const UGen rhs) {
  vector_add(&rhs->from, (vtype)lhs);
  vector_add(&lhs->to,   (vtype)rhs);
}

ANN static inline void disconnect(const UGen lhs, const UGen rhs) {
  vector_rem(&rhs->from, vector_find(&rhs->from, (vtype)lhs));
  vector_rem(&lhs->to,   vector_find(&lhs->to,   (vtype)rhs));
}

ANN static m_bool connect_init(const VM_Shred shred, M_Object* lhs, M_Object* rhs) {
  POP_REG(shred, SZ_INT * 2);
  *lhs = *(M_Object*)REG(0);
  *rhs = *(M_Object*)REG(SZ_INT);
  if(!*lhs || !*rhs) {
    NullException(shred, "UgenConnectException");
    return -1;
  }
  return 1;
}

typedef ANN void (*f_connect)(const UGen lhs, const UGen rhs);
ANN static void _do_(const f_connect f, const UGen lhs, const UGen rhs) {
  const m_uint l_max = lhs->channel  ? lhs->n_out : 1;
  const m_uint r_max = rhs->channel  ? rhs->n_in  : 1;
  const m_uint max   = l_max < r_max ? r_max : l_max;
  m_uint i = 0;
  do {
    const UGen l = lhs->channel ? UGEN(lhs->channel[i % l_max]) : lhs;
    const UGen r = rhs->channel ? UGEN(rhs->channel[i % r_max]) : rhs;
    f(l, r);
  }while(++i < max);
}

#define describe_ugen_connect(func) \
static INSTR(ugen_##func) { GWDEBUG_EXE \
  M_Object lhs, rhs; \
  if(connect_init(shred, &lhs, &rhs) > 0) {\
    _do_(func, UGEN(lhs), UGEN(rhs)); }\
  release_connect(shred);\
}
#define describe_trig_connect(func) \
static INSTR(trig_##func) { GWDEBUG_EXE \
  M_Object lhs, rhs; \
  if(connect_init(shred, &lhs, &rhs) > 0 && UGEN(rhs)->trig) {\
    _do_(func, UGEN(lhs), UGEN(UGEN(rhs)->trig)); } \
  release_connect(shred);\
}

describe_ugen_connect(connect)
describe_ugen_connect(disconnect)
describe_trig_connect(connect)
describe_trig_connect(disconnect)

static CTOR(ugen_ctor) {
  UGEN(o) = new_UGen();
  vector_add(&shred->vm_ref->ugen, (vtype)UGEN(o));
}

#define describe_release_func(src, tgt)                  \
ANN static void release_##src(const UGen ug) {           \
  for(m_uint i = vector_size(&ug->src) + 1; --i;) {      \
    const UGen u = (UGen)vector_at(&ug->src, i - 1);     \
    const m_int index = vector_find(&u->tgt, (vtype)ug); \
    vector_rem(&u->tgt, index);                          \
  }                                                      \
  vector_release(&ug->src);                              \
}
describe_release_func(from, to)
describe_release_func(to, from)

ANN static void release_mono(const UGen ug) {
  release_to(ug);
  release_from(ug);
}

ANN static void release_multi(const UGen ug, const VM_Shred shred) {
  for(m_uint i = ug->n_chan + 1; --i;)
    release(ug->channel[i - 1], shred);
  free(ug->channel);
}

static DTOR(ugen_dtor) {
  const UGen ug = UGEN(o);
  const m_int j = vector_find(&shred->vm_ref->ugen, (vtype)ug);

  vector_rem(&shred->vm_ref->ugen, j);
  if(!ug->channel)
    release_mono(ug);
  else
    release_multi(ug, shred);
  release(ug->trig, shred);
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
  assign_ugen(UGEN(vm->dac), 2, 2, vm);
  UGEN(vm->dac)->tick = dac_tick;
  vector_add(&vm->ugen, (vtype)UGEN(vm->dac));
  gwi_item_ini(gwi, "UGen", "dac");
  gwi_item_end(gwi, ae_flag_const, vm->dac);

  vm->adc       = new_M_UGen();
  assign_ugen(UGEN(vm->adc), 2, 2, vm);
  UGEN(vm->adc)->tick = adc_tick;
  vector_add(&vm->ugen, (vtype)UGEN(vm->adc));
  gwi_item_ini(gwi, "UGen", "adc");
  gwi_item_end(gwi, ae_flag_const, vm->adc);

  vm->blackhole = new_M_UGen();
  assign_ugen(UGEN(vm->blackhole), 1, 1, vm);
  UGEN(vm->blackhole)->ref = UGEN(vm->dac);
  vector_add(&vm->ugen, (vtype)UGEN(vm->blackhole));
  gwi_item_ini(gwi, "UGen", "blackhole");
  gwi_item_end(gwi, ae_flag_const, vm->blackhole);
  return 1;
}

static OP_CHECK(opck_chuck_ugen) {
  const Exp_Binary* bin = (Exp_Binary*)data;
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
  _CHECK_OP(chuck, chuck_ugen, ugen_connect)
  _CHECK_OP(unchuck, chuck_ugen, ugen_disconnect)
  _CHECK_OP(trig, chuck_ugen, trig_connect)
  _CHECK_OP(untrig, chuck_ugen, trig_disconnect)
  CHECK_BB(import_global_ugens(gwi))
  return 1;
}
