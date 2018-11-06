#include <stdlib.h>
#include <math.h>
#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "import.h"
#include "emit.h"

static INSTR(float_assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  *(m_float*)REG(-SZ_FLOAT) = (**(m_float**)REG(-SZ_FLOAT) =
    *(m_float*)REG(SZ_INT - SZ_FLOAT));
}

#define describe(name, op) \
INSTR(Float##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_FLOAT); \
  *(m_float*)REG(-SZ_FLOAT) op##= *(m_float*)REG(0); \
}

static describe(Plus, +)
static describe(Minus, -)
describe(Times, *)
static describe(Divide, /)

#define describe_logical(name, op) \
static INSTR(float_##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_FLOAT * 2 - SZ_INT); \
  *(m_int*)REG(-SZ_INT) = (*(m_float*)REG(-SZ_INT) op *(m_float*)REG(SZ_FLOAT -SZ_INT)); \
}
describe_logical(and, &&)
describe_logical(or,  ||)
describe_logical(eq,  ==)
describe_logical(neq, !=)
describe_logical(gt,   >)
describe_logical(ge,  >=)
describe_logical(lt,   <)
describe_logical(le,  <=)

INSTR(float_negate) { GWDEBUG_EXE
  *(m_float*)REG(-SZ_FLOAT) = -*(m_float*)REG(-SZ_FLOAT);
}


OP_CHECK(opck_unary_meta2) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  unary->self->meta = ae_meta_value;
  return t_int;
}

INSTR(float_not) { GWDEBUG_EXE
  POP_REG(shred, SZ_FLOAT - SZ_INT)
    *(m_int*)REG(-SZ_INT) = !*(m_float*)REG(-SZ_INT);
}

static INSTR(float_r_assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  **(m_float**)REG(0) = *(m_float*)REG(-SZ_FLOAT);
}

#define describe_r(name, op) \
static INSTR(float_r_##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_INT); \
  *(m_float*)REG(-SZ_FLOAT) = (**(m_float**)REG(0) op##= (*(m_float*)REG(-SZ_FLOAT))); \
}

describe_r(plus,   +)
describe_r(minus,  -)
describe_r(mul,  *)
describe_r(div, /)

static INSTR(int_float_assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_FLOAT);
  *(m_int*)REG(-SZ_INT) = (**(m_int**)REG(-SZ_INT) = *(m_float*)REG(0));
}

#define describe_if(name, op) \
static INSTR(int_float_##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_INT); \
  *(m_float*)REG(-SZ_FLOAT) = *(m_int*)REG(-SZ_FLOAT) op *(m_float*)REG(SZ_INT-SZ_FLOAT); \
}
describe_if(plus,   +)
describe_if(minus,  -)
describe_if(mul,  *)
describe_if(div, /)

#define describe_logical_if(name, op) \
static INSTR(int_float_##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_FLOAT); \
  *(m_int*)REG(-SZ_INT) = (*(m_int*)REG(-SZ_INT) op *(m_float*)REG(0)); \
}
describe_logical_if(and, &&)
describe_logical_if(or,  ||)
describe_logical_if(eq,  ==)
describe_logical_if(neq, !=)
describe_logical_if(gt,   >)
describe_logical_if(ge,  >=)
describe_logical_if(lt,   <)
describe_logical_if(le,  <=)

#define describe_r_if(name, op) \
static INSTR(int_float_r_##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_INT * 2 - SZ_FLOAT); \
  *(m_float*)REG(-SZ_FLOAT) = (**(m_float**)REG(SZ_INT - SZ_FLOAT) op##= *(m_int*)REG(-SZ_FLOAT)); \
}
describe_r_if(assign,  )
describe_r_if(plus,   +)
describe_r_if(minus,  -)
describe_r_if(mul,  *)
describe_r_if(div, /)

static INSTR(float_int_assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT * 2 - SZ_FLOAT);
  *(m_float*)REG(-SZ_FLOAT) = (**(m_float**)REG(-SZ_FLOAT) =
    *(m_int*)REG(SZ_INT - SZ_FLOAT));
}

#define describe_fi(name, op) \
static INSTR(float_int_##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_INT); \
  *(m_float*)REG(-SZ_FLOAT) op##= *(m_int*)REG(0); \
}
describe_fi(plus,   +)
describe_fi(minus,  -)
describe_fi(mul,  *)
describe_fi(div, /)

#define describe_logical_fi(name, op) \
static INSTR(float_int_##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_FLOAT); \
  *(m_int*)REG(-SZ_INT) = (*(m_float*)REG(-SZ_INT) op *(m_int*)REG(SZ_FLOAT-SZ_INT)); \
}
describe_logical_fi(and, &&)
describe_logical_fi(or,  ||)
describe_logical_fi(eq,  ==)
describe_logical_fi(neq, !=)
describe_logical_fi(gt,   >)
describe_logical_fi(ge,  >=)
describe_logical_fi(lt,   <)
describe_logical_fi(le,  <=)

static INSTR(float_int_r_assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_FLOAT);
  *(m_int*)REG(-SZ_INT) = **(m_int**)REG(SZ_FLOAT-SZ_INT) =
    *(m_float*)REG(-SZ_INT);
}

#define describe_r_fi(name, op) \
static INSTR(float_int_r_##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_FLOAT); \
  *(m_int*)REG(-SZ_INT) = (**(m_int**)REG(SZ_FLOAT -SZ_INT) op##= (*(m_float*)REG(-SZ_INT))); \
}
describe_r_fi(plus,   +)
describe_r_fi(minus,  -)
describe_r_fi(mul,  *)
describe_r_fi(div, /)

static INSTR(Time_Advance) { GWDEBUG_EXE
  POP_REG(shred, SZ_FLOAT);
  const m_float f = *(m_float*)REG(-SZ_FLOAT);
  *(m_float*)REG(-SZ_FLOAT) = (shred->wake_time += f);
  shredule(shred->vm->shreduler, shred, f);
}


static GWION_IMPORT(values) {
  VM* vm = gwi_vm(gwi);
  ALLOC_PTR(d_zero, m_float, 0.0);
  ALLOC_PTR(sr,     m_float, (m_float)vm->bbq->sr);
  ALLOC_PTR(samp,   m_float, 1.0);
  ALLOC_PTR(ms,     m_float, (m_float)*sr     / 1000.);
  ALLOC_PTR(second, m_float, (m_float)*sr);
  ALLOC_PTR(minute, m_float, (m_float)*sr     * 60.0);
  ALLOC_PTR(hour,   m_float, (m_float)*minute * 60.0);
//  ALLOC_PTR(day,    m_float, (m_float)*hour   * 24.0);
  ALLOC_PTR(t_zero, m_float, 0.0);
  ALLOC_PTR(pi, m_float, M_PI);
  gwi_item_ini(gwi, "float", "samplerate");
  gwi_item_end(gwi, ae_flag_const, sr);
  gwi_item_ini(gwi, "float", "pi");
  gwi_item_end(gwi, ae_flag_const, pi);
  gwi_item_ini(gwi, "dur", "d_zero");
  gwi_item_end(gwi, ae_flag_const, d_zero);
  gwi_item_ini(gwi, "dur", "samp");
  gwi_item_end(gwi, ae_flag_const, samp);
  gwi_item_ini(gwi, "dur", "ms");
  gwi_item_end(gwi, ae_flag_const, ms);
  gwi_item_ini(gwi, "dur", "second");
  gwi_item_end(gwi, ae_flag_const, second);
  gwi_item_ini(gwi, "dur", "minute");
  gwi_item_end(gwi, ae_flag_const, minute);
  gwi_item_ini(gwi, "dur", "hour");
  gwi_item_end(gwi, ae_flag_const, hour);
  gwi_item_ini(gwi, "time", "t_zero");
  gwi_item_end(gwi, ae_flag_const, t_zero);
  return 1;
}

static OP_CHECK(opck_chuck_now) {
  Exp_Binary* bin = (Exp_Binary*)data;
  ERR_O(bin->self->pos, "can't assign 'now' to 'now'")
  return NULL;
}

static OP_CHECK(opck_implicit_f2i) {
  return t_null;
}

static OP_CHECK(opck_implicit_i2f) {
  struct Implicit* imp = (struct Implicit*)data;
  ((Exp)imp->e)->cast_to = t_float;
  return t_float;
}

static OP_EMIT(opem_i2f) {
  CHECK_OB(emitter_add_instr(emit, CastI2F))
  return 1;
}

static OP_EMIT(opem_f2i) {
  CHECK_OB(emitter_add_instr(emit, CastF2I))
  return 1;
}

INSTR(CastI2F) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT - SZ_FLOAT);
  *(m_float*)REG(-SZ_FLOAT) = *(m_int*)REG(-SZ_FLOAT);
}


INSTR(CastF2I) { GWDEBUG_EXE
  POP_REG(shred, SZ_FLOAT - SZ_INT);
  *(m_int*)REG(-SZ_INT) = *(m_float*)REG(-SZ_INT);
}

#define CHECK_OP(op, check, func) _CHECK_OP(op, check, float_##func)
#define CHECK_IF(op, check, func) _CHECK_OP(op, check, int_float_##func)
#define CHECK_FI(op, check, func) _CHECK_OP(op, check, float_int_##func)

GWION_IMPORT(float) {
  CHECK_BB(gwi_oper_ini(gwi, "float", "float", "float"))
  CHECK_OP(assign, assign, assign)
  CHECK_BB(gwi_oper_end(gwi, op_add,          FloatPlus))
  CHECK_BB(gwi_oper_end(gwi, op_sub,         FloatMinus))
  CHECK_BB(gwi_oper_end(gwi, op_mul,         FloatTimes))
  CHECK_BB(gwi_oper_end(gwi, op_div,        FloatDivide))
  CHECK_OP(chuck, rassign, r_assign)
  CHECK_OP(radd, rassign, r_plus)
  CHECK_OP(rsub, rassign, r_minus)
  CHECK_OP(rmul, rassign, r_mul)
  CHECK_OP(rdiv, rassign, r_div)
  CHECK_BB(gwi_oper_ini(gwi, "float", "float", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_and,           float_and))
  CHECK_BB(gwi_oper_end(gwi, op_or,            float_or))
  CHECK_BB(gwi_oper_end(gwi, op_eq, 			 float_eq))
  CHECK_BB(gwi_oper_end(gwi, op_ne, 			 float_neq))
  CHECK_BB(gwi_oper_end(gwi, op_gt, 			 float_gt))
  CHECK_BB(gwi_oper_end(gwi, op_ge, 			 float_ge))
  CHECK_BB(gwi_oper_end(gwi, op_lt, 			 float_lt))
  CHECK_BB(gwi_oper_end(gwi, op_le, 			 float_le))
  CHECK_BB(gwi_oper_ini(gwi, NULL,   "float", "float"))
  CHECK_OP(sub, unary_meta, negate)
  CHECK_BB(gwi_oper_ini(gwi, NULL,   "float", "int"))
  CHECK_OP(not, unary_meta2, not)
  CHECK_BB(gwi_oper_ini(gwi, NULL,   "time", "int"))
  CHECK_OP(not, unary_meta, not)
  CHECK_BB(gwi_oper_ini(gwi, NULL,   "dur", "int"))
  CHECK_OP(not, unary_meta, not)
  CHECK_BB(gwi_oper_ini(gwi, "int", "float", "int"))
  CHECK_IF(assign, assign, assign)
  CHECK_BB(gwi_oper_end(gwi, op_and,           int_float_and))
  CHECK_BB(gwi_oper_end(gwi, op_or,            int_float_or))
  CHECK_BB(gwi_oper_end(gwi, op_eq, 			 int_float_eq))
  CHECK_BB(gwi_oper_end(gwi, op_ne, 			 int_float_neq))
  CHECK_BB(gwi_oper_end(gwi, op_gt, 			 int_float_gt))
  CHECK_BB(gwi_oper_end(gwi, op_ge, 			 int_float_ge))
  CHECK_BB(gwi_oper_end(gwi, op_lt, 			 int_float_lt))
  CHECK_BB(gwi_oper_end(gwi, op_le, 			 int_float_le))
  CHECK_BB(gwi_oper_ini(gwi, "int", "float", "float"))
  CHECK_BB(gwi_oper_end(gwi, op_add,          int_float_plus))
  CHECK_BB(gwi_oper_end(gwi, op_mul,         int_float_mul))
  CHECK_BB(gwi_oper_end(gwi, op_sub,         int_float_minus))
  CHECK_BB(gwi_oper_end(gwi, op_div,        int_float_div))
  CHECK_IF(chuck, rassign, r_assign)
  CHECK_IF(radd, rassign, r_plus)
  CHECK_IF(rsub, rassign, r_minus)
  CHECK_IF(rmul, rassign, r_mul)
  CHECK_IF(rdiv, rassign, r_div)
  CHECK_BB(gwi_oper_emi(gwi, opem_i2f))
  _CHECK_OP(cast, basic_cast, CastI2F)
  _CHECK_OP(impl, implicit_i2f, CastI2F)
  CHECK_BB(gwi_oper_ini(gwi, "float", "int", "float"))
  CHECK_FI(assign, assign, assign)
  CHECK_BB(gwi_oper_end(gwi, op_add,         float_int_plus))
  CHECK_BB(gwi_oper_end(gwi, op_sub,        float_int_minus))
  CHECK_BB(gwi_oper_end(gwi, op_mul,        float_int_mul))
  CHECK_BB(gwi_oper_end(gwi, op_div,       float_int_div))
  CHECK_BB(gwi_oper_ini(gwi, "float", "int", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_and,          float_int_and))
  CHECK_BB(gwi_oper_end(gwi, op_or,           float_int_or))
  CHECK_BB(gwi_oper_end(gwi, op_eq, 			float_int_eq))
  CHECK_BB(gwi_oper_end(gwi, op_ne, 			float_int_neq))
  CHECK_BB(gwi_oper_end(gwi, op_gt, 			float_int_gt))
  CHECK_BB(gwi_oper_end(gwi, op_ge, 			float_int_ge))
  CHECK_BB(gwi_oper_end(gwi, op_lt, 			float_int_lt))
  CHECK_BB(gwi_oper_end(gwi, op_le, 			float_int_le))
  CHECK_FI(chuck, rassign, r_assign)
  CHECK_FI(radd, rassign, r_plus)
  CHECK_FI(rsub, rassign, r_minus)
  CHECK_FI(rmul, rassign, r_mul)
  CHECK_FI(rdiv, rassign, r_div)
  CHECK_BB(gwi_oper_emi(gwi, opem_f2i))
  _CHECK_OP(cast, basic_cast, CastF2I)
  _CHECK_OP(impl, implicit_f2i, CastF2I)
  CHECK_BB(gwi_oper_ini(gwi, "dur", "dur", "dur"))
  CHECK_OP(chuck, rassign, r_assign)
  CHECK_BB(gwi_oper_end(gwi, op_add,         FloatPlus))
  CHECK_BB(gwi_oper_end(gwi, op_sub,        FloatMinus))
  CHECK_BB(gwi_oper_end(gwi, op_mul,        FloatTimes))
  CHECK_BB(gwi_oper_ini(gwi, "dur", "dur", "float"))
  CHECK_BB(gwi_oper_end(gwi, op_div,       FloatDivide))

  CHECK_BB(gwi_oper_ini(gwi, "dur", "dur", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_gt,           float_gt))
  CHECK_BB(gwi_oper_end(gwi, op_ge,           float_ge))
  CHECK_BB(gwi_oper_end(gwi, op_lt,           float_lt))
  CHECK_BB(gwi_oper_end(gwi, op_le,           float_le))

  CHECK_BB(gwi_oper_ini(gwi, "time", "time", "time"))
  CHECK_OP(chuck, rassign, r_assign)

  CHECK_BB(gwi_oper_ini(gwi, "time", "dur", "time"))
  CHECK_BB(gwi_oper_end(gwi, op_add,         FloatPlus))
  CHECK_BB(gwi_oper_ini(gwi, "dur", "time", "time"))
  CHECK_OP(chuck, rassign, r_assign)
  CHECK_BB(gwi_oper_end(gwi, op_add,         FloatPlus))
  CHECK_BB(gwi_oper_ini(gwi,  "dur",  "@now", "time"))
  _CHECK_OP(chuck, rassign, Time_Advance)
  CHECK_BB(gwi_oper_ini(gwi,  "@now",  "@now", NULL))
  _CHECK_OP(chuck, chuck_now, NULL)
  CHECK_BB(gwi_oper_ini(gwi, "time", "time", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_gt,           float_gt))
  CHECK_BB(gwi_oper_end(gwi, op_ge, 	      float_ge))
  CHECK_BB(gwi_oper_end(gwi, op_lt, 		  float_lt))
  CHECK_BB(gwi_oper_end(gwi, op_le,           float_le))

  CHECK_BB(import_values(gwi))
  return 1;
}
