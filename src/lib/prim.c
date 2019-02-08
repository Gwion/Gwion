#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "import.h"

#define CHECK_OP(op, check, func) _CHECK_OP(op, check, int_##func)

GWION_IMPORT(int_op) {
  CHECK_BB(gwi_oper_ini(gwi, "int", "int", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_add,         int_plus))
  CHECK_BB(gwi_oper_end(gwi, op_sub,        int_minus))
  CHECK_BB(gwi_oper_end(gwi, op_mul,        int_mul))
  CHECK_BB(gwi_oper_end(gwi, op_div,       int_div))
  return gwi_oper_end(gwi, op_mod,      int_modulo);
}

static GWION_IMPORT(int_logical) {
  CHECK_BB(gwi_oper_end(gwi, op_and,          int_and))
  CHECK_BB(gwi_oper_end(gwi, op_or,           int_or))
  CHECK_BB(gwi_oper_end(gwi, op_eq,           int_eq))
  CHECK_BB(gwi_oper_end(gwi, op_ne, 		  int_neq))
  CHECK_BB(gwi_oper_end(gwi, op_gt,           int_gt))
  CHECK_BB(gwi_oper_end(gwi, op_ge, 	      int_ge))
  CHECK_BB(gwi_oper_end(gwi, op_lt, 		  int_lt))
  CHECK_BB(gwi_oper_end(gwi, op_le, 	      int_le))
  CHECK_BB(gwi_oper_end(gwi, op_shr,  int_sr))
  CHECK_BB(gwi_oper_end(gwi, op_shl,   int_sl))
  CHECK_BB(gwi_oper_end(gwi, op_sand,        int_sand))
  CHECK_BB(gwi_oper_end(gwi, op_sor,         int_sor))
  return gwi_oper_end(gwi, op_sxor, int_xor);
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
  CHECK_OP(inc,   unary, pre_inc)
  CHECK_OP(dec, unary, pre_dec)
  CHECK_BB(gwi_oper_end(gwi,  op_cmp, int_cmp))
  CHECK_BB(gwi_oper_ini(gwi, "int", NULL, "int"))
  CHECK_OP(inc,   post,  post_inc)
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

GWION_IMPORT(int) {
  CHECK_BB(gwi_oper_ini(gwi, "int", "int", "int"))
  CHECK_BB(import_int_op(gwi))
  CHECK_BB(import_int_logical(gwi))
  CHECK_BB(import_int_r(gwi))
  CHECK_BB(import_int_unary(gwi))
  return import_int_values(gwi);
}
