#include "defs.h"

static Operator operators[] = {
// arithmetic
  op_assign, op_plus, op_minus, op_times, op_divide, op_percent,
// logical
  op_and, op_or, op_eq, op_neq,
  op_gt, op_ge, op_lt, op_le,
// bitwise
  op_shift_left, op_shift_right,
  op_s_or, op_s_and, op_s_xor,
// unary
  op_plusplus, op_minusminus, op_exclamation, op_tilda,
  op_new, op_spork, op_typeof, op_sizeof,
// reverse arithmetic
  op_chuck, op_plus_chuck, op_minus_chuck, op_times_chuck, op_divide_chuck, op_modulo_chuck,
// reverse logical
  op_rand, op_ror, op_req, op_rneq,
  op_rgt, op_rge, op_rlt, op_rle,
// reverse bitwise
  op_rsl, op_rsr, op_rsand, op_rsor, op_rsxor,
// unchuck and others
  op_unchuck, op_rinc, op_rdec, op_runinc, op_rundec,
//at_chuck
  op_at_chuck, op_at_unchuck,
// trig
  op_trig, op_untrig
};

static m_str op_str[] = {
// arithmetic
  "=", "+", "-", "*", "/", "%",
// logical
  "&&", "||",	"==", "!=",
  ">", ">=", "<", "<=",
// bitwise
  "<<", ">>",
  "|", "&", "^",
// unary
  "++", "--", "!", "~",
  "new", "spork", "typeof", "sizeof",
// reverse arithmetic
  "=>", "+=>", "-=>", "*=>", "/=>", "%=>",
// reverse logical
  "&&=>", "||=>", "==>", "!=>",
  ">=>", ">==>", "<=<", "<==<",
// reverse bitwise
  "<<=>", ">>=>", "&=>", "|=>", "^=>",
// unchuck an others
  "=<", "++=>", "--=>", "++=<", "--=<",
// at_chuck
  "@=>", "@=<",
// trig
  "]=>", "]=<", NULL
};

static m_str op_name[] = {
// arithmetic
  "assign", "plus", "minus", "times", "divide", "modulo",
// logical
  "and", "or", "eq", "neq",
  "gt", "ge", "lt", "le",
// bitwise
  "shiftleft", "shiftright",
  "s_or", "s_and", "s_xor",
// unary
  "plusplus", "minusminus", "exclamation", "tilda",
  "new", "spork", "typeof", "sizeof",
// reverse arithmetic
  "chuck", "pluschuck", "minuschuck", "timeschuck", "dividechuck", "modulochuck",
// reverse logical
  "rand", "ror", "req", "rneq",
  "rgt", "rge", "rlt", "rle",
// reverse bitwise
  "rsl", "rsr", "rsand", "rsor", "rsxor",
// unchuck and others
  "unchuck", "rinc", "rdec", "runinc", "runcdec",
// at
  "at_chuck", "at_unchuck",
// trig
  "trig", "untrig", NULL
  // more to come
};

typedef Type (*f_type)(Env env, Expression exp);

typedef struct {
  Type lhs, rhs, ret;
  f_instr instr;
  Func func;
} M_Operator;
