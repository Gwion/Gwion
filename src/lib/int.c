#include "type.h"
#include "instr.h"
#include "import.h"

struct Type_ t_int = { "int", SZ_INT };

static INSTR(int_assign) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)REG(0) = (**(m_int**)REG(0) = *(m_int*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

#define describe(name, op)\
static INSTR(int_##name) {\
  POP_REG(shred, SZ_INT * 2);\
  *(m_int*)REG(0) op *(m_int*)REG(SZ_INT);\
  PUSH_REG(shred, SZ_INT);\
}

describe(plus,  +=)
describe(minus, +=)
describe(times, *=)

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

#define describe_logical(name, op) \
static INSTR(int_##name) {\
  POP_REG(shred, SZ_INT * 2);\
  *(m_int*)REG(0) = (*(m_int*)REG(0) op *(m_int*)REG(SZ_INT));\
  PUSH_REG(shred, SZ_INT);\
}

describe_logical(and,  &&)
describe_logical(or,   ||)
describe_logical(eq,   ==)
describe_logical(neq,  !=)
describe_logical(gt,    >)
describe_logical(ge,   >=)
describe_logical(lt,    <)
describe_logical(le,   <=)
describe_logical(sl,   <<)
describe_logical(sr,   >>)
describe_logical(sand,  &)
describe_logical(sor,   |)
describe_logical(xor,   ^)

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

#define describe_r(name, op)\
static INSTR(int_r_##name) {\
  POP_REG(shred, SZ_INT * 2);\
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_INT) op (*(m_int*)REG(0)));\
  PUSH_REG(shred, SZ_INT);\
}


static INSTR(int_r_assign) {
  POP_REG(shred, SZ_INT * 2);
  **(m_int**)REG(SZ_INT) = *(m_int*)REG(0);
  PUSH_REG(shred, SZ_INT);
}
describe_r(plus, +=)
describe_r(minus, -=)
describe_r(times, *=)

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

describe_r(sl,   <<=)
describe_r(sr,   >>=)
describe_r(sand,  &=)
describe_r(sor,   |=)
describe_r(sxor,  ^=)

m_bool import_int(Gwi gwi) {
    CHECK_BB(gwi_oper_ini(gwi, "int", "int", "int"))
    CHECK_BB(gwi_oper_add(gwi, opck_assign))
    CHECK_BB(gwi_oper_end(gwi, op_assign,       int_assign))
    CHECK_BB(gwi_oper_end(gwi, op_plus,         int_plus))
    CHECK_BB(gwi_oper_end(gwi, op_minus,        int_minus))
    CHECK_BB(gwi_oper_end(gwi, op_times,        int_times))
    CHECK_BB(gwi_oper_end(gwi, op_divide,       int_divide))
    CHECK_BB(gwi_oper_end(gwi, op_percent,      int_modulo))
    CHECK_BB(gwi_oper_end(gwi, op_and,          int_and))
    CHECK_BB(gwi_oper_end(gwi, op_or,           int_or))
    CHECK_BB(gwi_oper_end(gwi, op_eq,           int_eq))
    CHECK_BB(gwi_oper_end(gwi, op_neq, 		  int_neq))
    CHECK_BB(gwi_oper_end(gwi, op_gt,           int_gt))
    CHECK_BB(gwi_oper_end(gwi, op_ge, 	      int_ge))
    CHECK_BB(gwi_oper_end(gwi, op_lt, 		  int_lt))
    CHECK_BB(gwi_oper_end(gwi, op_le, 	      int_le))
    CHECK_BB(gwi_oper_end(gwi, op_shift_right,  int_sr))
    CHECK_BB(gwi_oper_end(gwi, op_shift_left,   int_sl))
    CHECK_BB(gwi_oper_end(gwi, op_s_and,        int_sand))
    CHECK_BB(gwi_oper_end(gwi, op_s_or,         int_sor))
    CHECK_BB(gwi_oper_end(gwi, op_s_xor, 	      int_xor))
    CHECK_BB(gwi_oper_add(gwi, opck_rassign))
    CHECK_BB(gwi_oper_end(gwi, op_chuck,        int_r_assign))
    CHECK_BB(gwi_oper_add(gwi, opck_rassign))
    CHECK_BB(gwi_oper_end(gwi, op_plus_chuck,   int_r_plus))
    CHECK_BB(gwi_oper_add(gwi, opck_rassign))
    CHECK_BB(gwi_oper_end(gwi, op_minus_chuck,  int_r_minus))
    CHECK_BB(gwi_oper_add(gwi, opck_rassign))
    CHECK_BB(gwi_oper_end(gwi, op_times_chuck,  int_r_times))
    CHECK_BB(gwi_oper_add(gwi, opck_rassign))
    CHECK_BB(gwi_oper_end(gwi, op_divide_chuck, int_r_divide))
    CHECK_BB(gwi_oper_add(gwi, opck_rassign))
    CHECK_BB(gwi_oper_end(gwi, op_modulo_chuck, int_r_modulo))
    CHECK_BB(gwi_oper_add(gwi, opck_rassign))
    CHECK_BB(gwi_oper_end(gwi, op_rsl,          int_r_sl))
    CHECK_BB(gwi_oper_add(gwi, opck_rassign))
    CHECK_BB(gwi_oper_end(gwi, op_rsr,          int_r_sr))
    CHECK_BB(gwi_oper_add(gwi, opck_rassign))
    CHECK_BB(gwi_oper_end(gwi, op_rsand,        int_r_sand))
    CHECK_BB(gwi_oper_add(gwi, opck_rassign))
    CHECK_BB(gwi_oper_end(gwi, op_rsor,         int_r_sor))
    CHECK_BB(gwi_oper_add(gwi, opck_rassign))
    CHECK_BB(gwi_oper_end(gwi, op_rsxor, 		  int_r_sxor))
    CHECK_BB(gwi_oper_ini(gwi, NULL, "int", "int"))
    CHECK_BB(gwi_oper_add(gwi, opck_unary_meta))
    CHECK_BB(gwi_oper_end(gwi, op_minus,       int_negate))
    CHECK_BB(gwi_oper_add(gwi, opck_unary_meta))
    CHECK_BB(gwi_oper_end(gwi, op_exclamation, int_not))
    CHECK_BB(gwi_oper_add(gwi, opck_unary))
    CHECK_BB(gwi_oper_end(gwi, op_plusplus,    int_pre_inc))
    CHECK_BB(gwi_oper_add(gwi, opck_unary))
    CHECK_BB(gwi_oper_end(gwi, op_minusminus,  int_pre_dec))
    CHECK_BB(gwi_oper_ini(gwi, "int", NULL, "int"))
    CHECK_BB(gwi_oper_add(gwi, opck_post))
    CHECK_BB(gwi_oper_end(gwi, op_plusplus,    int_post_inc))
    CHECK_BB(gwi_oper_add(gwi, opck_post))
    CHECK_BB(gwi_oper_end(gwi, op_minusminus,  int_post_dec))
    return 1;
}
