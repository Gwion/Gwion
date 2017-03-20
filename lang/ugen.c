#include "vm.h"
#include "type.h"
#include "dl.h"
#include "err_msg.h"
#include "import.h"
#include "lang.h"
#include "ugen.h"
#include "bbq.h"

/*#include <soundpipe.h>*/

struct Type_ t_ugen = { "UGen", SZ_INT, &t_object, te_ugen };

m_bool base_tick(UGen u)
{
  m_uint i, size = vector_size(u->ugen);
  UGen ugen;
  if(!size) {
    u->out = 0;
    return 1;
  }
  ugen = (UGen)vector_at(u->ugen, 0);
  u->out = ugen->out;
  /*  u->out = ugen ? ugen->out : 0;*/
  for(i = 1; i < size; i++) {
    ugen = (UGen)vector_at(u->ugen, i);
    /*    if(!ugen)*/
    /*      continue;*/
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

m_bool dac_tick(UGen u)
{
  m_uint  i;
  sp_data* sp = (sp_data*)u->ug;
  for(i = 0; i < sp->nchan; i++)
    sp->out[i] = 0;
  for(i = 0; i < u->n_out; i++)
    sp->out[i] += u->channel[i]->ugen->out;
  return 1;
}

m_bool adc_tick(UGen u)
{
  m_uint  i;
  m_float last = 0;
  BBQ sp = (BBQ )u->ug;
  for(i = 0; i < u->n_out; i++) {
    M_Object obj = u->channel[i];
    last += (obj->ugen->out = sp->in[i]);
  }
  last /= u->n_out;
  return 1;
}
void ref_compute(UGen u)
{
  u->tick(u);
  u->done = 1;
}

void channel_compute(UGen u)
{
//	if(u->done)
//		return;
  m_uint i;
  u->in = 0;
  for(i = 0; i < vector_size(u->ugen); i++)
    u->in += ((UGen)vector_at(u->ugen, i))->out;
//	u->done = 1;
}
// recursively compute ugen
void ugen_compute(UGen u)
{
  m_uint  i;
  m_float sum = 0;
  UGen ugen;
  if(!u || u->done)
    return;
  if(u->channel)
    for(i = 0; i < u->n_out; i++)
      ugen_compute(u->channel[i]->ugen);
  else
    for(i = 0; i < vector_size(u->ugen); i++) {
      ugen = (UGen)vector_at(u->ugen, i);
//      if(!ugen) // find a way to remove ugens
//	      break;
      if(!ugen->done)
        ugen_compute(ugen);
    }
  if(u->ref) {
    u->tick(u);
    for(i = 0; i < u->n_in; i++)
      u->ref->channel[i]->ugen->tick(u->ref->channel[i]->ugen);
    ref_compute(u->ref);
    return;
  }
  u->tick(u);
  if(u->channel) {
    /*    for(i = 0; i < u->n_out> u->n_in ? u->n_out : u->n_in; i++)*/
    for(i = 0; i < u->n_out; i++) {
      M_Object obj = u->channel[i];
      sum += obj->ugen->out;
    }
    u->last = sum / u->n_out;
  } else
    u->last = u->out;
  u->done = 1;
}

UGen new_UGen()
{
  UGen u    =  (UGen) calloc(1, sizeof(struct UGen_));
  u->ugen   = NULL;
  u->channel = NULL;
  u->to = new_Vector();
  u->ug = NULL;
  u->trig = NULL;
  u->tick = base_tick;
  u->done = 0;
  u->op = 1;
  return u;
}

void free_UGen(UGen u)
{
  free_Vector(u->to);
  if(u->ugen)
    free_Vector(u->ugen);
  else {
    m_uint i;
    for(i = 0; i < u->n_chan; i++) {
      free_UGen(u->channel[i]->ugen);
      free(u->channel[i]);
    }
//    release(u->channel[i], NULL);
//    release(u->channel[i], NULL);
    free(u->channel);
  }
  free(u);
}

M_Object new_M_UGen()
{
  M_Object o = new_M_Object();
  initialize_object(o, &t_ugen);
  o->ugen = new_UGen();
  return o;
}

m_bool assign_ugen(UGen u, m_uint n_in, m_uint n_out, m_bool trig, void* ug)
{
  u->n_chan = n_in > n_out ? n_in : n_out;
  if(u->n_chan > 1) {
    u->channel = calloc(u->n_chan, sizeof(M_Object));
    m_uint i;
    for(i = 0; i < u->n_chan; i++) {
      M_Object chan = new_M_UGen();
      assign_ugen(chan->ugen, n_in > i, n_out > i, 0, NULL);
      chan->ugen->tick = base_tick;
      chan->ugen->ref = u;
      u->channel[i] =  chan;
    }
  } else
    u->ugen = new_Vector();
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

static INSTR(ugen_connect)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "ugen connect %p %p", *(m_uint*)(shred->reg - SZ_INT * 2), *(m_uint*)(shred->reg - SZ_INT));
#endif
  m_uint i;
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_INT);

  if(!rhs->ugen->n_in) {
    shred->is_done = 1;
    shred->is_running = 0;
    err_msg(INSTR_, 0, "'%s' has no inputs", rhs->type_ref->name);
    return;
  }
  if(rhs->ugen->channel) {
    for(i = 0; i < rhs->ugen->n_out; i++) {
      M_Object obj = rhs->ugen->channel[i];
      if(lhs->ugen->n_out > 1) {
        vector_append(obj->ugen->ugen, (vtype)lhs->ugen->channel[i % lhs->ugen->n_out]->ugen);
        vector_append(lhs->ugen->channel[i%lhs->ugen->n_out]->ugen->to, (vtype)obj->ugen);
      } else {
        vector_append(obj->ugen->ugen, (vtype)lhs->ugen);
        vector_append(lhs->ugen->to, (vtype)obj->ugen);
      }
    }
  } else
    vector_append(rhs->ugen->ugen, (vtype)lhs->ugen);
  release(lhs, shred);
  release(rhs, shred);
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(ugen_disconnect)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "ugen connect %p %p", *(m_uint*)(shred->reg - SZ_INT * 2), *(m_uint*)(shred->reg - SZ_INT));
#endif
  m_uint i;
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_INT);
  if(!rhs->ugen->n_in) {
	// rhs has no inputs, do nothing
    release(lhs, shred);
    release(rhs, shred);
    return;
  }
  if(rhs->ugen->channel) {
    for(i = 0; i < rhs->ugen->n_out; i++) {
      M_Object obj = rhs->ugen->channel[i];
      UGen ugen = obj->ugen;
      vector_remove(ugen->ugen, vector_find(ugen->ugen,  (vtype)lhs->ugen));
      vector_remove(lhs->ugen->to, vector_find(lhs->ugen->to, (vtype)ugen));
    }
  } else {
    vector_remove(rhs->ugen->ugen, vector_find(rhs->ugen->ugen, (vtype)lhs->ugen));
    vector_remove(lhs->ugen->to, vector_find(lhs->ugen->to, (vtype)rhs->ugen));
  }
  release(lhs, shred);
  release(rhs, shred);
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(trig_connect)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "trig connect %p %p", *(m_uint*)(shred->reg - SZ_INT * 2), *(m_uint*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_INT);
  if(rhs->ugen->trig) {
    vector_append(rhs->ugen->trig->ugen->ugen, (vtype)lhs->ugen);
//    vector_append(lhs->ugen->to, (vtype)rhs->ugen->trig->ugen);
  }
  release(lhs, shred);
  release(rhs, shred);
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(trig_disconnect)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "trig discconnect %p %p", *(m_uint*)(shred->reg - SZ_INT * 2), *(m_uint*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_INT);
  if(rhs->ugen->trig) {
    vector_remove(rhs->ugen->trig->ugen->ugen, vector_find(rhs->ugen->trig->ugen->ugen,  (vtype)lhs->ugen));
//    vector_remove(lhs->ugen->to, vector_find(lhs->ugen->to, (vtype)rhs->ugen->trig->ugen));
  }
  release(lhs, shred);
  release(rhs, shred);
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
}

void ugen_ctor(M_Object o, VM_Shred shred)
{
  o->ugen = new_UGen();
  /*  o->ugen->to = new_Vector();*/
  vector_append(shred->vm_ref->ugen, (vtype)o->ugen);
}


void ugen_dtor(M_Object o, VM_Shred shred)
{
  vector_remove(shred->vm_ref->ugen, (m_uint)vector_find(shred->vm_ref->ugen, (vtype)o->ugen));
  m_uint i;
  for(i = 0; i < vector_size(o->ugen->to); i++) {
    UGen u = (UGen)vector_at(o->ugen->to, i);
    m_int index = vector_find(u->ugen, (vtype)o->ugen);
    if(index > -1)
      vector_remove(u->ugen, index);
    else
      u->trig = NULL;
  }
  if(o->ugen->trig)
	release(o->ugen->trig, shred);
  free_UGen(o->ugen);
}

static MFUN(ugen_channel)
{
  m_int i = *(m_int*)(shred->mem + SZ_INT);
  if(!o->ugen->channel && !i)
    RETURN->d.v_object = o;
  else if(i < 0 || i >= o->ugen->n_out)
    RETURN->d.v_object = NULL;
  else {
	RETURN->d.v_object = o->ugen->channel[i];
	o->ugen->channel[i]->ref++;
  }
}

static MFUN(ugen_get_op)
{
  RETURN->d.v_uint = o->ugen->op;
}

static MFUN(ugen_set_op)
{
  m_int i = *(m_int*)(shred->mem + SZ_INT);
  if(i < 1 || i > 4)
    err_msg(INSTR_, 0, "invalid op %i", i);
  else
    o->ugen->op = i;
  RETURN->d.v_uint = o->ugen->op;
}

static MFUN(ugen_get_last)
{
  RETURN->d.v_float = o->ugen->last;
}

m_bool import_ugen(Env env)
{
  DL_Func* fun;

  CHECK_BB(add_global_type(env, &t_ugen))
  CHECK_OB(import_class_begin(env, &t_ugen, env->global_nspc, ugen_ctor, ugen_dtor))
  env->class_def->doc = "base sound class";

  fun = new_DL_Func("UGen", "chan", (m_uint)ugen_channel);
  dl_func_add_arg(fun, "int", "arg0");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "op", (m_uint)ugen_get_op);
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "op", (m_uint)ugen_set_op);
  dl_func_add_arg(fun, "int", "arg0");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("float", "last", (m_uint)ugen_get_last);
  CHECK_OB(import_mfun(env, fun))

  CHECK_BB(add_binary_op(env, op_chuck,   &t_ugen, &t_ugen, &t_ugen, ugen_connect, 1, 0))
  CHECK_BB(add_binary_op(env, op_unchuck, &t_ugen, &t_ugen, &t_ugen, ugen_disconnect, 1, 0))
  CHECK_BB(add_binary_op(env, op_trig,    &t_ugen, &t_ugen, &t_ugen, trig_connect, 1, 0))
  CHECK_BB(add_binary_op(env, op_untrig,  &t_ugen, &t_ugen, &t_ugen, trig_disconnect, 1, 0))

  CHECK_BB(import_class_end(env))
  return 1;
}
