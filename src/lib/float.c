#include <stdlib.h>
#include <math.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"

static INSTR(float_assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)REG(0) = (**(m_float**)REG(0) = *(m_float*)REG(SZ_INT));
  PUSH_REG(shred, SZ_FLOAT);
}

#define describe(name, op) \
INSTR(float_##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_FLOAT); \
  *(m_float*)REG(-SZ_FLOAT) op##= *(m_float*)REG(0); \
}

static describe(plus, +)
static describe(minus, -)
describe(times, *)
static describe(divide, /)

#define describe_logical(name, op) \
static INSTR(float_##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_FLOAT * 2); \
  *(m_int*)REG(0) = (*(m_float*)REG(0) op *(m_float*)REG(SZ_FLOAT)); \
  PUSH_REG(shred, SZ_INT); \
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
  POP_REG(shred, SZ_FLOAT)
    *(m_float*)REG(0) = -*(m_float*)REG(0);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(float_not) { GWDEBUG_EXE
  POP_REG(shred, SZ_FLOAT)
    *(m_int*)REG(0) = !*(m_float*)REG(0);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_r_assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  **(m_float**)REG(SZ_FLOAT) = *(m_float*)REG(0);
  PUSH_REG(shred, SZ_FLOAT);
}

#define describe_r(name, op) \
static INSTR(float_r_##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_FLOAT + SZ_INT); \
  *(m_float*)REG(0) = (**(m_float**)REG(SZ_FLOAT) op##= (*(m_float*)REG(0))); \
  PUSH_REG(shred, SZ_FLOAT); \
}

describe_r(plus,   +)
describe_r(minus,  -)
describe_r(times,  *)
describe_r(divide, /)

static INSTR(int_float_assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)REG(0) = (**(m_int**)REG(0) = *(m_float*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

#define describe_if(name, op) \
static INSTR(int_float_##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_INT + SZ_FLOAT); \
  *(m_float*)REG(0) = *(m_int*)REG(0) op *(m_float*)REG(SZ_INT); \
  PUSH_REG(shred, SZ_FLOAT); \
}
describe_if(plus,   +)
describe_if(minus,  -)
describe_if(times,  *)
describe_if(divide, /)

#define describe_logical_if(name, op) \
static INSTR(int_float_##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_INT + SZ_FLOAT); \
  *(m_int*)REG(0) = (*(m_int*)REG(0) op *(m_float*)REG(SZ_INT)); \
  PUSH_REG(shred, SZ_INT); \
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
  POP_REG(shred, SZ_INT * 2); \
  *(m_float*)REG(0) = (**(m_float**)REG(SZ_INT) op##= *(m_int*)REG(0)); \
  PUSH_REG(shred, SZ_FLOAT); \
}
describe_r_if(assign,  )
describe_r_if(plus,   +)
describe_r_if(minus,  -)
describe_r_if(times,  *)
describe_r_if(divide, /)

static INSTR(float_int_assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)REG(0) = (**(m_float**)REG(0) = *(m_int*)REG(SZ_INT));
  PUSH_REG(shred, SZ_FLOAT);
}

#define describe_fi(name, op) \
static INSTR(float_int_##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_FLOAT + SZ_INT); \
  *(m_float*)REG(0) op##= *(m_int*)REG(SZ_FLOAT); \
  PUSH_REG(shred, SZ_FLOAT); \
}
describe_fi(plus,   +)
describe_fi(minus,  -)
describe_fi(times,  *)
describe_fi(divide, /)

#define describe_logical_fi(name, op) \
static INSTR(float_int_##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_FLOAT + SZ_INT); \
  *(m_int*)REG(0) = (*(m_float*)REG(0) op *(m_int*)REG(SZ_FLOAT)); \
  PUSH_REG(shred, SZ_INT); \
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
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  **(m_int**)REG(SZ_FLOAT) = *(m_float*)REG(0);
  *(m_int*)REG(0) = **(m_int**)REG(SZ_FLOAT);
  PUSH_REG(shred, SZ_INT);
}

#define describe_r_fi(name, op) \
static INSTR(float_int_r_##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_INT + SZ_FLOAT); \
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_FLOAT) op##= (*(m_float*)REG(0))); \
  PUSH_REG(shred, SZ_INT); \
}
describe_r_fi(plus,   +)
describe_r_fi(minus,  -)
describe_r_fi(times,  *)
describe_r_fi(divide, /)

static INSTR(Time_Advance) { GWDEBUG_EXE
  POP_REG(shred, SZ_FLOAT);
  const m_float f = *(m_float*)REG(-SZ_FLOAT);
  *(m_float*)REG(-SZ_FLOAT) = (shred->wake_time += f);
  shredule(shred->vm_ref->shreduler, shred, f);
}


ANN static m_bool import_values(const Gwi gwi) {
  VM* vm = gwi_vm(gwi);
  ALLOC_PTR(d_zero, m_float, 0.0);
  ALLOC_PTR(sr,     m_float, (m_float)vm->sp->sr);
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
  CHECK_BO(err_msg(TYPE_, bin->self->pos, "can't assign 'now' to 'now'"))
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
  CHECK_OB(emitter_add_instr(emit, Cast_i2f))
  return 1;
}

static OP_EMIT(opem_f2i) {
  CHECK_OB(emitter_add_instr(emit, Cast_f2i))
  return 1;
}

INSTR(Cast_i2f) { GWDEBUG_EXE
  POP_REG(shred,  SZ_INT);
  *(m_float*)REG(0) = *(m_int*)REG(0);
  PUSH_REG(shred,  SZ_FLOAT);
}


INSTR(Cast_f2i) { GWDEBUG_EXE
  POP_REG(shred,  SZ_FLOAT);
  *(m_int*)REG(0) = *(m_float*)REG(0);
  PUSH_REG(shred,  SZ_INT);
}

#define CHECK_OP(op, check, func) _CHECK_OP(op, check, float_##func)
#define CHECK_IF(op, check, func) _CHECK_OP(op, check, int_float_##func)
#define CHECK_FI(op, check, func) _CHECK_OP(op, check, float_int_##func)

ANN m_bool import_float(const Gwi gwi) {
  CHECK_BB(gwi_oper_ini(gwi, "float", "float", "float"))
  CHECK_OP(assign, assign, assign)
  CHECK_BB(gwi_oper_end(gwi, op_plus,          float_plus))
  CHECK_BB(gwi_oper_end(gwi, op_minus,         float_minus))
  CHECK_BB(gwi_oper_end(gwi, op_times,         float_times))
  CHECK_BB(gwi_oper_end(gwi, op_divide,        float_divide))
  CHECK_OP(chuck, rassign, r_assign)
  CHECK_OP(plus_chuck, rassign, r_plus)
  CHECK_OP(minus_chuck, rassign, r_minus)
  CHECK_OP(times_chuck, rassign, r_times)
  CHECK_OP(divide_chuck, rassign, r_divide)
  CHECK_BB(gwi_oper_ini(gwi, "float", "float", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_and,           float_and))
  CHECK_BB(gwi_oper_end(gwi, op_or,            float_or))
  CHECK_BB(gwi_oper_end(gwi, op_eq, 			 float_eq))
  CHECK_BB(gwi_oper_end(gwi, op_neq, 			 float_neq))
  CHECK_BB(gwi_oper_end(gwi, op_gt, 			 float_gt))
  CHECK_BB(gwi_oper_end(gwi, op_ge, 			 float_ge))
  CHECK_BB(gwi_oper_end(gwi, op_lt, 			 float_lt))
  CHECK_BB(gwi_oper_end(gwi, op_le, 			 float_le))
  CHECK_BB(gwi_oper_ini(gwi, NULL,   "float", "float"))
  CHECK_OP(minus, unary_meta, negate)
  CHECK_BB(gwi_oper_ini(gwi, NULL,   "float", "int"))
  CHECK_OP(exclamation, unary_meta, not)
  CHECK_BB(gwi_oper_ini(gwi, NULL,   "time", "int"))
  CHECK_OP(exclamation, unary_meta, not)
  CHECK_BB(gwi_oper_ini(gwi, NULL,   "dur", "int"))
  CHECK_OP(exclamation, unary_meta, not)
  CHECK_BB(gwi_oper_ini(gwi, "int", "float", "int"))
  CHECK_IF(assign, assign, assign)
  CHECK_BB(gwi_oper_end(gwi, op_and,           int_float_and))
  CHECK_BB(gwi_oper_end(gwi, op_or,            int_float_or))
  CHECK_BB(gwi_oper_end(gwi, op_eq, 			 int_float_eq))
  CHECK_BB(gwi_oper_end(gwi, op_neq, 			 int_float_neq))
  CHECK_BB(gwi_oper_end(gwi, op_gt, 			 int_float_gt))
  CHECK_BB(gwi_oper_end(gwi, op_ge, 			 int_float_ge))
  CHECK_BB(gwi_oper_end(gwi, op_lt, 			 int_float_lt))
  CHECK_BB(gwi_oper_end(gwi, op_le, 			 int_float_le))
  CHECK_BB(gwi_oper_ini(gwi, "int", "float", "float"))
  CHECK_BB(gwi_oper_end(gwi, op_plus,          int_float_plus))
  CHECK_BB(gwi_oper_end(gwi, op_times,         int_float_times))
  CHECK_BB(gwi_oper_end(gwi, op_minus,         int_float_minus))
  CHECK_BB(gwi_oper_end(gwi, op_divide,        int_float_divide))
  CHECK_IF(chuck, rassign, r_assign)
  CHECK_IF(plus_chuck, rassign, r_plus)
  CHECK_IF(minus_chuck, rassign, r_minus)
  CHECK_IF(times_chuck, rassign, r_times)
  CHECK_IF(divide_chuck, rassign, r_divide)
  CHECK_BB(gwi_oper_emi(gwi, opem_i2f))
  _CHECK_OP(dollar, basic_cast, Cast_i2f)
  _CHECK_OP(implicit, implicit_i2f, Cast_i2f)
  CHECK_BB(gwi_oper_ini(gwi, "float", "int", "float"))
  CHECK_FI(assign, assign, assign)
  CHECK_BB(gwi_oper_end(gwi, op_plus,         float_int_plus))
  CHECK_BB(gwi_oper_end(gwi, op_minus,        float_int_minus))
  CHECK_BB(gwi_oper_end(gwi, op_times,        float_int_times))
  CHECK_BB(gwi_oper_end(gwi, op_divide,       float_int_divide))
  CHECK_BB(gwi_oper_ini(gwi, "float", "int", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_and,          float_int_and))
  CHECK_BB(gwi_oper_end(gwi, op_or,           float_int_or))
  CHECK_BB(gwi_oper_end(gwi, op_eq, 			float_int_eq))
  CHECK_BB(gwi_oper_end(gwi, op_neq, 			float_int_neq))
  CHECK_BB(gwi_oper_end(gwi, op_gt, 			float_int_gt))
  CHECK_BB(gwi_oper_end(gwi, op_ge, 			float_int_ge))
  CHECK_BB(gwi_oper_end(gwi, op_lt, 			float_int_lt))
  CHECK_BB(gwi_oper_end(gwi, op_le, 			float_int_le))
  CHECK_FI(chuck, rassign, r_assign)
  CHECK_FI(plus_chuck, rassign, r_plus)
  CHECK_FI(minus_chuck, rassign, r_minus)
  CHECK_FI(times_chuck, rassign, r_times)
  CHECK_FI(divide_chuck, rassign, r_divide)
  CHECK_BB(gwi_oper_emi(gwi, opem_f2i))
  _CHECK_OP(dollar, basic_cast, Cast_f2i)
  _CHECK_OP(implicit, implicit_f2i, Cast_f2i)
  CHECK_BB(gwi_oper_ini(gwi, "dur", "dur", "dur"))
  CHECK_OP(chuck, rassign, r_assign)
  CHECK_BB(gwi_oper_end(gwi, op_plus,         float_plus))
  CHECK_BB(gwi_oper_end(gwi, op_minus,        float_minus))
  CHECK_BB(gwi_oper_end(gwi, op_times,        float_times))
  CHECK_BB(gwi_oper_ini(gwi, "dur", "dur", "float"))
  CHECK_BB(gwi_oper_end(gwi, op_divide,       float_divide))

  CHECK_BB(gwi_oper_ini(gwi, "dur", "dur", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_gt,           float_gt))
  CHECK_BB(gwi_oper_end(gwi, op_ge,           float_ge))
  CHECK_BB(gwi_oper_end(gwi, op_lt,           float_lt))
  CHECK_BB(gwi_oper_end(gwi, op_le,           float_le))

  CHECK_BB(gwi_oper_ini(gwi, "time", "time", "time"))
  CHECK_OP(chuck, rassign, r_assign)

  CHECK_BB(gwi_oper_ini(gwi, "time", "dur", "time"))
  CHECK_BB(gwi_oper_end(gwi, op_plus,         float_plus))
  CHECK_BB(gwi_oper_ini(gwi, "dur", "time", "time"))
  CHECK_OP(chuck, rassign, r_assign)
  CHECK_BB(gwi_oper_end(gwi, op_plus,         float_plus))
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
