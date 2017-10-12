#include "type.h"
#include "instr.h"
#include "import.h"

struct Type_ t_int   = { "int",   SZ_INT,   NULL,    te_int};

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

m_bool import_int(Importer importer) {
  CHECK_BB(importer_add_type(importer,  &t_int))
    CHECK_BB(importer_oper_ini(importer, "int", "int", "int"))
    CHECK_BB(importer_oper_end(importer, op_assign,       int_assign,     0))
    CHECK_BB(importer_oper_end(importer, op_plus,         int_plus,       0))
    CHECK_BB(importer_oper_end(importer, op_minus,        int_minus,      0))
    CHECK_BB(importer_oper_end(importer, op_times,        int_times,      0))
    CHECK_BB(importer_oper_end(importer, op_divide,       int_divide,     0))
    CHECK_BB(importer_oper_end(importer, op_percent,      int_modulo,     0))
    CHECK_BB(importer_oper_end(importer, op_and,          int_and,        0))
    CHECK_BB(importer_oper_end(importer, op_or,           int_or,         0))
    CHECK_BB(importer_oper_end(importer, op_eq, 			 		 int_eq,         0))
    CHECK_BB(importer_oper_end(importer, op_neq, 			 	 int_neq,        0))
    CHECK_BB(importer_oper_end(importer, op_gt, 			 	   int_gt,         0))
    CHECK_BB(importer_oper_end(importer, op_ge, 			 	   int_ge,         0))
    CHECK_BB(importer_oper_end(importer, op_lt, 			 	   int_lt,         0))
    CHECK_BB(importer_oper_end(importer, op_le, 			 	   int_le,         0))
    CHECK_BB(importer_oper_end(importer, op_shift_right,  int_sr,         0))
    CHECK_BB(importer_oper_end(importer, op_shift_left,   int_sl,         0))
    CHECK_BB(importer_oper_end(importer, op_s_and,        int_sand,       0))
    CHECK_BB(importer_oper_end(importer, op_s_or,         int_sor,        0))
    CHECK_BB(importer_oper_end(importer, op_s_xor, 			 int_xor,        0))
    CHECK_BB(importer_oper_end(importer, op_chuck,        int_r_assign,   0))
    CHECK_BB(importer_oper_end(importer, op_plus_chuck,   int_r_plus,     0))
    CHECK_BB(importer_oper_end(importer, op_minus_chuck,  int_r_minus,    0))
    CHECK_BB(importer_oper_end(importer, op_times_chuck,  int_r_times,    0))
    CHECK_BB(importer_oper_end(importer, op_divide_chuck, int_r_divide,   0))
    CHECK_BB(importer_oper_end(importer, op_modulo_chuck, int_r_modulo,   0))
    CHECK_BB(importer_oper_end(importer, op_rand,         int_r_and,      0))
    CHECK_BB(importer_oper_end(importer, op_ror,          int_ror,       0))
    CHECK_BB(importer_oper_end(importer, op_req, 			 	 int_req,        0))
    CHECK_BB(importer_oper_end(importer, op_rneq, 			 	 int_rneq,       0))
    CHECK_BB(importer_oper_end(importer, op_rgt, 			 	 int_rgt,        0))
    CHECK_BB(importer_oper_end(importer, op_rge, 			 	 int_rge,        0))
    CHECK_BB(importer_oper_end(importer, op_rlt, 			 	 int_rlt,        0))
    CHECK_BB(importer_oper_end(importer, op_rle, 			 	 int_rle,        0))
    CHECK_BB(importer_oper_end(importer, op_rsl,          int_rsl,        0))
    CHECK_BB(importer_oper_end(importer, op_rsr,          int_rsr,        0))
    CHECK_BB(importer_oper_end(importer, op_rsand,        int_rsand,      0))
    CHECK_BB(importer_oper_end(importer, op_rsor,         int_rsor,       0))
    CHECK_BB(importer_oper_end(importer, op_rsxor, 			 int_rsxor,      0))
    CHECK_BB(importer_oper_ini(importer, NULL, "int", "int"))
    CHECK_BB(importer_oper_end(importer, op_minus,       int_negate,     0))
    CHECK_BB(importer_oper_end(importer, op_exclamation, int_not,        0))
    CHECK_BB(importer_oper_end(importer, op_plusplus,    int_pre_inc,    0))
    CHECK_BB(importer_oper_end(importer, op_minusminus,  int_pre_dec,    0))
    CHECK_BB(importer_oper_ini(importer, "int", NULL, "int"))
    CHECK_BB(importer_oper_end(importer, op_plusplus,    int_post_inc,   0))
    CHECK_BB(importer_oper_end(importer, op_minusminus,  int_post_dec,   0))
    return 1;
}
