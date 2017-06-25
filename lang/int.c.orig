#include "defs.h"
#include "import.h"

struct Type_ t_int       = { "int",        SZ_INT,   NULL, te_int};

static INSTR(int_assign) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(0) = *(m_int*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_plus) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) += *(m_int*)REG(SZ_INT);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_minus) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) -= *(m_int*)REG(SZ_INT);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_times) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) *= *(m_int*)REG(SZ_INT);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_divide) {
  POP_REG(shred, SZ_INT * 2);
  if(!*(m_int*)REG(SZ_INT)) {
    Except(shred, "ZeroDivideException")
  }
  *(m_int*)REG(0) /= *(m_int*)REG(SZ_INT);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_modulo) {
  POP_REG(shred, SZ_INT * 2);
  if(!*(m_int*)REG(SZ_INT)) {
    Except(shred, "ZeroDivideException")
  }
  *(m_int*)REG(0) %= *(m_int*)REG(SZ_INT);
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(int_and) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (*(m_int*)REG(0) && *(m_int*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_or) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (*(m_int*)REG(0) || *(m_int*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_eq) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (*(m_int*)REG(0) == *(m_int*)REG(SZ_INT));
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(int_neq) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (*(m_int*)REG(0) != *(m_int*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_gt) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (*(m_int*)REG(0) > *(m_int*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_ge) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (*(m_int*)REG(0) >= *(m_int*)REG(SZ_INT));
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(int_lt) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (*(m_int*)REG(0) < * (m_int*)REG(SZ_INT));
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(int_le) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (*(m_int**)REG(0) <= *(m_int**)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_sl) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (*(m_int*)REG(0) << * (m_int*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_sr) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (*(m_int*)REG(0) >> *(m_int*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_sand) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (*(m_int*)REG(0) & *(m_int*)REG(SZ_INT));
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(int_sor) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (*(m_int*)REG(0) | *(m_int*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_xor) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (*(m_int*)REG(0) ^ * (m_int*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_negate) {
  POP_REG(shred, SZ_INT);
  *(m_int*)REG(0) *= -1;
  PUSH_REG(shred, SZ_INT);
}

INSTR(int_not) {
  POP_REG(shred, SZ_INT);
  *(m_int*)REG(0) = !*(m_int*)REG(0);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_pre_inc) {
  *(m_int*)REG(- SZ_INT) = (++**(m_int**)REG(- SZ_INT));
}

static INSTR(int_pre_dec) {
  *(m_int*)REG(- SZ_INT) = (--**(m_int**)REG(- SZ_INT));
}

static INSTR(int_post_inc) {
  *(m_int*)REG(- SZ_INT) = (**(m_int**)REG(- SZ_INT))++;
}

static INSTR(int_post_dec) {
  *(m_int*)REG(- SZ_INT) = (**(m_int**)REG(- SZ_INT))--;
}

static INSTR(int_r_assign) {
  POP_REG(shred, SZ_INT * 2);
  **(m_int**)REG(SZ_INT) = *(m_int*)REG(0);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_r_plus) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) += (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_r_minus) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) -= (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_r_times) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) *= (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_r_divide) {
  POP_REG(shred, SZ_INT * 2);
  if(!*(m_int*)REG(0))
    Except(shred, "ZeroDivideException")
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) /= (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_r_modulo) {
  POP_REG(shred, SZ_INT * 2);
  if(!*(m_int*)REG(0))
    Except(shred, "ZeroDivideException")
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) %= (*(m_int*)REG(0)));
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(int_r_and) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) && (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_ror) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) || (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_req) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) == (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_rneq) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) != (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_rgt) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) > (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_rge) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) >= (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_rlt) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) < (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_rle) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) <= (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_rsl) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) <<= (*(m_int*)REG(0)));
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(int_rsr) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) >>= (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_rsand) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) &= (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_rsor) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) |= (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_rsxor) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) ^= (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

m_bool import_int(Env env) {
  CHECK_BB(env_add_type(env, &t_int))
  CHECK_BB(import_op(env, op_assign,        "int", "int", "int", int_assign,     0))
  CHECK_BB(import_op(env, op_plus,          "int", "int", "int", int_plus,       0))
  CHECK_BB(import_op(env, op_minus,         "int", "int", "int", int_minus,      0))
  CHECK_BB(import_op(env, op_times,         "int", "int", "int", int_times,      0))
  CHECK_BB(import_op(env, op_divide,        "int", "int", "int", int_divide,     0))
  CHECK_BB(import_op(env, op_percent,       "int", "int", "int", int_modulo,     0))
  CHECK_BB(import_op(env, op_and,           "int", "int", "int", int_and,        0))
  CHECK_BB(import_op(env, op_or,            "int", "int", "int", int_or,         0))
  CHECK_BB(import_op(env, op_eq, 			 		  "int", "int", "int", int_eq,         0))
  CHECK_BB(import_op(env, op_neq, 			 	  "int", "int", "int", int_neq,        0))
  CHECK_BB(import_op(env, op_gt, 			 	    "int", "int", "int", int_gt,         0))
  CHECK_BB(import_op(env, op_ge, 			 	    "int", "int", "int", int_ge,         0))
  CHECK_BB(import_op(env, op_lt, 			 	    "int", "int", "int", int_lt,         0))
  CHECK_BB(import_op(env, op_le, 			 	    "int", "int", "int", int_le,         0))
  CHECK_BB(import_op(env, op_shift_right,   "int", "int", "int", int_sr,         0))
  CHECK_BB(import_op(env, op_shift_left,    "int", "int", "int", int_sl,         0))
  CHECK_BB(import_op(env, op_s_and,         "int", "int", "int", int_sand,       0))
  CHECK_BB(import_op(env, op_s_or,          "int", "int", "int", int_sor,        0))
  CHECK_BB(import_op(env, op_s_xor, 			  "int", "int", "int", int_xor,        0))
  CHECK_BB(import_op(env, op_minus,         NULL,   "int", "int", int_negate,     0))
  CHECK_BB(import_op(env, op_exclamation,   NULL,   "int", "int", int_not,        0))
  CHECK_BB(import_op(env, op_plusplus,      "int", NULL,   "int", int_post_inc,   0))
  CHECK_BB(import_op(env, op_plusplus,      NULL,   "int", "int", int_pre_inc,    0))
  CHECK_BB(import_op(env, op_minusminus,    "int", NULL,   "int", int_post_dec,   0))
  CHECK_BB(import_op(env, op_minusminus,    NULL,   "int", "int", int_pre_dec,    0))
  CHECK_BB(import_op(env, op_chuck,         "int", "int", "int", int_r_assign,   0))
  CHECK_BB(import_op(env, op_plus_chuck,    "int", "int", "int", int_r_plus,     0))
  CHECK_BB(import_op(env, op_minus_chuck,   "int", "int", "int", int_r_minus,    0))
  CHECK_BB(import_op(env, op_times_chuck,   "int", "int", "int", int_r_times,    0))
  CHECK_BB(import_op(env, op_divide_chuck,  "int", "int", "int", int_r_divide,   0))
  CHECK_BB(import_op(env, op_modulo_chuck,  "int", "int", "int", int_r_modulo,   0))
  CHECK_BB(import_op(env, op_rand,          "int", "int", "int", int_r_and,      0))
  CHECK_BB(import_op(env, op_ror,           "int", "int", "int", int_ror,       0))
  CHECK_BB(import_op(env, op_req, 			 	  "int", "int", "int", int_req,        0))
  CHECK_BB(import_op(env, op_rneq, 			 	  "int", "int", "int", int_rneq,       0))
  CHECK_BB(import_op(env, op_rgt, 			 	  "int", "int", "int", int_rgt,        0))
  CHECK_BB(import_op(env, op_rge, 			 	  "int", "int", "int", int_rge,        0))
  CHECK_BB(import_op(env, op_rlt, 			 	  "int", "int", "int", int_rlt,        0))
  CHECK_BB(import_op(env, op_rle, 			 	  "int", "int", "int", int_rle,        0))
  CHECK_BB(import_op(env, op_rsl,           "int", "int", "int", int_rsl,        0))
  CHECK_BB(import_op(env, op_rsr,           "int", "int", "int", int_rsr,        0))
  CHECK_BB(import_op(env, op_rsand,         "int", "int", "int", int_rsand,      0))
  CHECK_BB(import_op(env, op_rsor,          "int", "int", "int", int_rsor,       0))
  CHECK_BB(import_op(env, op_rsxor, 			  "int", "int", "int", int_rsxor,      0))
  return 1;
}
