#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "import.h"
#include "emit.h"
#include "operator.h"
#include "driver.h"

#define CHECK_OP(op, check, func) _CHECK_OP(op, check, int_##func)

GWION_IMPORT(int_op) {
  CHECK_BB(gwi_oper_ini(gwi, "int", "int", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_add, int_plus))
  CHECK_BB(gwi_oper_end(gwi, op_sub, int_minus))
  CHECK_BB(gwi_oper_end(gwi, op_mul, int_mul))
  CHECK_BB(gwi_oper_end(gwi, op_div, int_div))
  return   gwi_oper_end(gwi, op_mod, int_modulo);
}

static GWION_IMPORT(int_logical) {
  CHECK_BB(gwi_oper_end(gwi, op_and,  int_and))
  CHECK_BB(gwi_oper_end(gwi, op_or,   int_or))
  CHECK_BB(gwi_oper_end(gwi, op_eq,   int_eq))
  CHECK_BB(gwi_oper_end(gwi, op_ne,   int_neq))
  CHECK_BB(gwi_oper_end(gwi, op_gt,   int_gt))
  CHECK_BB(gwi_oper_end(gwi, op_ge,   int_ge))
  CHECK_BB(gwi_oper_end(gwi, op_lt,   int_lt))
  CHECK_BB(gwi_oper_end(gwi, op_le,   int_le))
  CHECK_BB(gwi_oper_end(gwi, op_shr,  int_sr))
  CHECK_BB(gwi_oper_end(gwi, op_shl,  int_sl))
  CHECK_BB(gwi_oper_end(gwi, op_sand, int_sand))
  CHECK_BB(gwi_oper_end(gwi, op_sor,  int_sor))
  return   gwi_oper_end(gwi, op_sxor, int_xor);
}

static GWION_IMPORT(int_r) {
  CHECK_OP(chuck, rassign, r_assign)
  CHECK_OP(radd,  rassign, r_plus)
  CHECK_OP(rsub,  rassign, r_minus)
  CHECK_OP(rmul,  rassign, r_mul)
  CHECK_OP(rdiv,  rassign, r_div)
  CHECK_OP(rmod,  rassign, r_modulo)
  CHECK_OP(rsl,   rassign, r_sl)
  CHECK_OP(rsr,   rassign, r_sr)
  CHECK_OP(rsand, rassign, r_sand)
  CHECK_OP(rsor,  rassign, r_sor)
  CHECK_OP(rsxor, rassign, r_sxor)
  return GW_OK;
}

static GWION_IMPORT(int_unary) {
  CHECK_BB(gwi_oper_ini(gwi, NULL, "int", "int"))
  CHECK_BB(gwi_oper_add(gwi,  opck_unary_meta))
  CHECK_BB(gwi_oper_end(gwi,  op_sub,       int_negate))
  CHECK_BB(gwi_oper_add(gwi,  opck_unary_meta))
  CHECK_BB(gwi_oper_end(gwi,  op_not, IntNot))
  CHECK_OP(inc, unary, pre_inc)
  CHECK_OP(dec, unary, pre_dec)
  CHECK_BB(gwi_oper_end(gwi,  op_cmp, int_cmp))
  CHECK_BB(gwi_oper_ini(gwi, "int", NULL, "int"))
  CHECK_OP(inc, post, post_inc)
  CHECK_BB(gwi_oper_add(gwi, opck_post))
  CHECK_BB(gwi_oper_end(gwi, op_dec, int_post_dec))
  return GW_OK;
}

static GWION_IMPORT(int_values) {
  CHECK_BB(gwi_enum_ini(gwi, "bool"))
  CHECK_BB(gwi_enum_add(gwi, "false", 0))
  CHECK_BB(gwi_enum_add(gwi, "true", 1))
  CHECK_BB(gwi_enum_end(gwi))
  CHECK_BB(gwi_item_ini(gwi, "bool", "maybe"))
  return gwi_item_end(gwi, 0, NULL);
}

static GWION_IMPORT(int) {
  CHECK_BB(gwi_oper_ini(gwi, "int", "int", "int"))
  CHECK_BB(import_int_op(gwi))
  CHECK_BB(import_int_logical(gwi))
  CHECK_BB(import_int_r(gwi))
  CHECK_BB(import_int_unary(gwi))
  return import_int_values(gwi);
}

OP_CHECK(opck_unary_meta2) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  unary->self->meta = ae_meta_value;
  return t_int;
}

static GWION_IMPORT(values) {
  VM* vm = gwi_vm(gwi);
  ALLOC_PTR(d_zero, m_float, 0.0);
  ALLOC_PTR(sr,     m_float, (m_float)vm->bbq->si->sr);
  ALLOC_PTR(samp,   m_float, 1.0);
  ALLOC_PTR(ms,     m_float, (m_float)(*sr     / 1000.));
  ALLOC_PTR(second, m_float, (m_float)*sr);
  ALLOC_PTR(minute, m_float, (m_float)(*sr     * 60.0));
  ALLOC_PTR(hour,   m_float, (m_float)(*minute * 60.0));
  ALLOC_PTR(t_zero, m_float, 0.0);
  ALLOC_PTR(pi, m_float, (m_float)M_PI);
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
  gwi_item_ini(gwi, "@now", "now");
  gwi_item_end(gwi, 0, NULL);
  return GW_OK;
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
  emit_add_instr(emit, CastI2F);
  return GW_OK;
}

static OP_EMIT(opem_f2i) {
  emit_add_instr(emit, CastF2I);
  return GW_OK;
}

#define CHECK_FF(op, check, func) _CHECK_OP(op, check, float_##func)
#define CHECK_IF(op, check, func) _CHECK_OP(op, check, int_float_##func)
#define CHECK_FI(op, check, func) _CHECK_OP(op, check, float_int_##func)

static GWION_IMPORT(intfloat) {
  CHECK_BB(gwi_oper_ini(gwi, "int", "float", "int"))
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
  return GW_OK;
}

static GWION_IMPORT(floatint) {
  CHECK_BB(gwi_oper_ini(gwi, "float", "int", "float"))
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
  return GW_OK;
}

static GWION_IMPORT(dur) {
  CHECK_BB(gwi_oper_ini(gwi, "dur", "dur", "dur"))
  CHECK_FF(chuck, rassign, r_assign)
  CHECK_BB(gwi_oper_end(gwi, op_add,         FloatPlus))
  CHECK_BB(gwi_oper_end(gwi, op_sub,        FloatMinus))
  CHECK_BB(gwi_oper_end(gwi, op_mul,        FloatTimes))
  CHECK_BB(gwi_oper_ini(gwi, "dur", "dur", "float"))
  CHECK_BB(gwi_oper_end(gwi, op_div,       FloatDivide))

  CHECK_BB(gwi_oper_ini(gwi, "dur", "dur", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_gt,           float_gt))
  CHECK_BB(gwi_oper_end(gwi, op_ge,           float_ge))
  CHECK_BB(gwi_oper_end(gwi, op_lt,           float_lt))
  return gwi_oper_end(gwi, op_le,           float_le);
}

static GWION_IMPORT(time) {
  CHECK_BB(gwi_oper_ini(gwi, "time", "time", "time"))
  CHECK_FF(chuck, rassign, r_assign)
  CHECK_BB(gwi_oper_ini(gwi, "time", "dur", "time"))
  CHECK_BB(gwi_oper_end(gwi, op_add,         FloatPlus))
  CHECK_BB(gwi_oper_ini(gwi, "dur", "time", "time"))
  CHECK_FF(chuck, rassign, r_assign)
  CHECK_BB(gwi_oper_end(gwi, op_add,         FloatPlus))
  CHECK_BB(gwi_oper_ini(gwi,  "dur",  "@now", "time"))
  _CHECK_OP(chuck, rassign, Time_Advance)
  CHECK_BB(gwi_oper_ini(gwi,  "@now",  "@now", NULL))
  _CHECK_OP(chuck, chuck_now, NULL)
  CHECK_BB(gwi_oper_ini(gwi, "time", "time", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_gt,           float_gt))
  CHECK_BB(gwi_oper_end(gwi, op_ge, 	      float_ge))
  CHECK_BB(gwi_oper_end(gwi, op_lt, 		  float_lt))
  return gwi_oper_end(gwi, op_le,           float_le);
}

static GWION_IMPORT(float) {
  CHECK_BB(gwi_oper_ini(gwi, "float", "float", "float"))
  CHECK_BB(gwi_oper_end(gwi, op_add,          FloatPlus))
  CHECK_BB(gwi_oper_end(gwi, op_sub,         FloatMinus))
  CHECK_BB(gwi_oper_end(gwi, op_mul,         FloatTimes))
  CHECK_BB(gwi_oper_end(gwi, op_div,        FloatDivide))
  CHECK_FF(chuck, rassign, r_assign)
  CHECK_FF(radd, rassign, r_plus)
  CHECK_FF(rsub, rassign, r_minus)
  CHECK_FF(rmul, rassign, r_mul)
  CHECK_FF(rdiv, rassign, r_div)
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
  CHECK_FF(sub, unary_meta, negate)
  CHECK_BB(gwi_oper_ini(gwi, NULL,   "float", "int"))
  CHECK_FF(not, unary_meta2, not)
  CHECK_BB(gwi_oper_ini(gwi, NULL,   "time", "int"))
  CHECK_FF(not, unary_meta, not)
  CHECK_BB(gwi_oper_ini(gwi, NULL,   "dur", "int"))
  CHECK_FF(not, unary_meta, not)
  return GW_OK;
}

GWION_IMPORT(prim) {
  CHECK_BB(import_int(gwi))
  CHECK_BB(import_float(gwi))
  CHECK_BB(import_intfloat(gwi))
  CHECK_BB(import_floatint(gwi))
  CHECK_BB(import_dur(gwi))
  CHECK_BB(import_time(gwi))
  return import_values(gwi);
}
