#include "vm.h"
#include "type.h"
#include "dl.h"
#include "err_msg.h"
#include "import.h"
#include "lang.h"
#include "ugen.h"

struct Type_ t_ugen = { "UGen", SZ_INT, &t_object, te_ugen };

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
      case 1: u->in = (u->out += ugen->out); return 1;
      case 2: u->in = (u->out -= ugen->out); return 1;
      case 3: u->in = (u->out *= ugen->out); return 1;
      case 4: u->in = (u->out /= ugen->out); return 1;
    }
  }
  return 1;
}

m_bool dac_tick(UGen u) {
  m_uint  i;
  sp_data* sp = (sp_data*)u->ug;
  for(i = u->n_out + 1; --i;)
    sp->out[i - 1] = u->channel[i - 1]->ugen->out;
  return 1;
}

m_bool adc_tick(UGen u) {
  m_uint  i;
  m_float last = 0;
  BBQ sp = (BBQ)u->ug;
  for(i = u->n_out +1; --i;) {
	m_uint j = i -1;
    M_Object obj = u->channel[j];
    obj->ugen->last = sp->in[j];
    last += (obj->ugen->out = sp->in[j]);
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
      ugen_compute(u->channel[i-1]->ugen);
  else for(i = vector_size(&u->ugen) + 1; --i;)
      ugen_compute((UGen)vector_at(&u->ugen, i - 1));
  if(u->ref) {
    for(i = u->ref->n_chan + 1; --i;) {
      ugen = u->ref->channel[i -1]->ugen;
      ugen->tick(ugen);
    }
    u->tick(u);
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
  o->ugen = new_UGen();
  return o;
}

m_bool assign_ugen(UGen u, m_uint n_in, m_uint n_out, m_bool trig, void* ug) {
  u->n_chan = n_in > n_out ? n_in : n_out;
  if(u->n_chan > 1) {
    u->channel = malloc(u->n_chan * sizeof(M_Object));
    m_uint i;
    for(i = u->n_chan + 1; --i;) {
      m_uint j = i -1;
      M_Object chan = new_M_UGen();
      assign_ugen(chan->ugen, n_in > j, n_out > j, 0, NULL);
      chan->ugen->tick = base_tick;
      chan->ugen->ref = u;
      u->channel[j] =  chan;
    }
  } else
    vector_init(&u->ugen);
  if(trig) {
    u->trig = new_M_UGen();
    assign_ugen(u->trig->ugen, 1, 1, 0, NULL);
    u->trig->ugen->tick = base_tick;
  }
  u->in = u->out = 0;
  u->n_in   = n_in;
  u->n_out  = n_out;
  u->ug     = ug;
  return 1;
}

static INSTR(ugen_connect) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "ugen connect %p %p", *(m_uint*)REG(- SZ_INT * 2), *(m_uint*)REG(- SZ_INT));
#endif
  m_uint i;
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);

  if(!lhs || !lhs->ugen || !rhs || !rhs->ugen) {
    release(rhs, shred);
    Except(shred, "UgenConnectException");
  }
  if(rhs->ugen->n_in) {
    if(rhs->ugen->channel) {
      for(i = rhs->ugen->n_out + 1; --i;) {
        m_uint j = i - 1;
        M_Object obj = rhs->ugen->channel[j];
        if(lhs->ugen->n_out > 1) {
          vector_add(&obj->ugen->ugen, (vtype)lhs->ugen->channel[j % lhs->ugen->n_out]->ugen);
          vector_add(&lhs->ugen->channel[j%lhs->ugen->n_out]->ugen->to, (vtype)obj->ugen);
        } else {
          vector_add(&obj->ugen->ugen, (vtype)lhs->ugen);
          vector_add(&lhs->ugen->to, (vtype)obj->ugen);
        }
      }
    } else {
      vector_add(&rhs->ugen->ugen, (vtype)lhs->ugen);
      vector_add(&lhs->ugen->to, (vtype)rhs->ugen);
    }
  }
  release(lhs, shred);
  release(rhs, shred);
  *(M_Object*)REG(0) = rhs;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(ugen_disconnect) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "ugen connect %p %p", *(m_uint*)REG(- SZ_INT * 2), *(m_uint*)REG(- SZ_INT));
#endif
  m_uint i;
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);
  if(!lhs || !lhs->ugen || !rhs || !rhs->ugen) {
    release(rhs, shred);
    Except(shred, "UgenConnectException");
  }
  if(rhs->ugen->n_in) {
    if(rhs->ugen->channel) {
      for(i = rhs->ugen->n_out + 1; --i;) {
        M_Object obj = rhs->ugen->channel[i - 1];
        UGen ugen = obj->ugen;
        vector_rem(&ugen->ugen, vector_find(&ugen->ugen, (vtype)lhs->ugen));
        vector_rem(&lhs->ugen->to, vector_find(&lhs->ugen->to, (vtype)ugen));
      }
    } else {
      vector_rem(&rhs->ugen->ugen, vector_find(&rhs->ugen->ugen, (vtype)lhs->ugen));
      vector_rem(&lhs->ugen->to, vector_find(&lhs->ugen->to, (vtype)rhs->ugen));
    }
  }
  release(lhs, shred);
  release(rhs, shred);
  *(M_Object*)REG(0) = rhs;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(trig_connect) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "trig connect %p %p", *(m_uint*)REG(- SZ_INT * 2), *(m_uint*)REG(- SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);
  if(!lhs || !lhs->ugen || !rhs || !rhs->ugen) {
    release(rhs, shred);
    Except(shred, "UgenConnectException");
  }
  if(rhs->ugen->trig) {
    vector_add(&rhs->ugen->trig->ugen->ugen, (vtype)lhs->ugen);
    vector_add(&lhs->ugen->to, (vtype)rhs->ugen->trig->ugen);
  }
  release(lhs, shred);
  release(rhs, shred);
  *(M_Object*)REG(0) = rhs;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(trig_disconnect) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "trig discconnect %p %p", *(m_uint*)REG(- SZ_INT * 2), *(m_uint*)REG(- SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);
  if(!lhs || !lhs->ugen || !rhs || !rhs->ugen) {
    release(rhs, shred);
    Except(shred, "UgenConnectException");
  }
  if(rhs->ugen->trig) {
    vector_rem(&rhs->ugen->trig->ugen->ugen, vector_find(&rhs->ugen->trig->ugen->ugen, (vtype)lhs->ugen));
    vector_rem(&lhs->ugen->to, vector_find(&lhs->ugen->to, (vtype)rhs->ugen->trig->ugen));
  }
  release(lhs, shred);
  release(rhs, shred);
  *(M_Object*)REG(0) = rhs;
  PUSH_REG(shred, SZ_INT);
}

static CTOR(ugen_ctor) {
  o->ugen = new_UGen();
  vector_add(&shred->vm_ref->ugen, (vtype)o->ugen);
}

static DTOR(ugen_dtor) {
  UGen ug = o->ugen;
  m_uint i;
  m_int j = vector_find(&shred->vm_ref->ugen, (vtype)ug);
  if(j > -1)
    vector_rem(&shred->vm_ref->ugen, j);
  for(i = vector_size(&ug->to) + 1; --i;) {
    UGen u = (UGen)vector_at(&ug->to, i - 1);
    if(u->ugen.ptr) {
      m_int index = vector_find(&u->ugen, (vtype)ug);
      if(index > -1)
        vector_rem(&u->ugen, index);
    }
  }

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
  if(ug->trig)
    release(ug->trig, shred);
  vector_release(&ug->to);
  free(ug);
}

static MFUN(ugen_channel) {
  m_int i = *(m_int*)MEM(SZ_INT);
  if(!o->ugen->channel)
    RETURN->d.v_object = !i ? o : NULL;
  else if(i < 0 || i >= o->ugen->n_chan)
    RETURN->d.v_object = NULL;
  else
    RETURN->d.v_object = o->ugen->channel[i];
}

static MFUN(ugen_get_op) {
  RETURN->d.v_uint = o->ugen->op;
}

static MFUN(ugen_set_op) {
  m_int i = *(m_int*)MEM(SZ_INT);
  if(i < 1 || i > 4)
    err_msg(INSTR_, 0, "invalid op %i", i);
  else
    o->ugen->op = i;
  RETURN->d.v_uint = o->ugen->op;
}

static MFUN(ugen_get_last) {
  RETURN->d.v_float = o->ugen->last;
}

m_bool import_ugen(Env env) {
  DL_Func fun;

  CHECK_BB(import_class_begin(env, &t_ugen, env->global_nspc, ugen_ctor, ugen_dtor))

  dl_func_init(&fun, "UGen", "chan", (m_uint)ugen_channel);
  dl_func_add_arg(&fun, "int", "arg0");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "op", (m_uint)ugen_get_op);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "op", (m_uint)ugen_set_op);
  dl_func_add_arg(&fun, "int", "arg0");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "float", "last", (m_uint)ugen_get_last);
  CHECK_BB(import_fun(env, &fun, 0))

  CHECK_BB(import_op(env, op_chuck,   "UGen", "UGen", "UGen", ugen_connect, 1))
  CHECK_BB(import_op(env, op_unchuck, "UGen", "UGen", "UGen", ugen_disconnect, 1))
  CHECK_BB(import_op(env, op_trig,    "UGen", "UGen", "UGen", trig_connect, 1))
  CHECK_BB(import_op(env, op_untrig,  "UGen", "UGen", "UGen", trig_disconnect, 1))

  CHECK_BB(import_class_end(env))
  return 1;
}
