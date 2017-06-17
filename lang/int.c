#include "defs.h"
#include "import.h"

struct Type_ t_int       = { "int",        SZ_INT,   NULL, te_int};

// arithmetic
static INSTR(assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'=' %i", **(m_int**)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)shred->reg = *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'+' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg += *(m_int*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(minus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i '-' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg -= *(m_int*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(times) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'*' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg *= *(m_int*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(divide) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'/' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  if(!*(m_int*)(shred->reg + SZ_INT)) {
    Except(shred, "ZeroDivideException")
  }
  *(m_int*)shred->reg /= *(m_int*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(modulo) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'%' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  if(!*(m_int*)(shred->reg + SZ_INT)) {
    Except(shred, "ZeroDivideException")
  }
  *(m_int*)shred->reg %= *(m_int*)(shred->reg + SZ_INT);
  PUSH_REG(shred,  SZ_INT);
}

// logical
static INSTR(and) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'&&' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg && *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(or) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'||' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg || *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(eq) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'==' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg == *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(neq) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'!=' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg != *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(gt) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'>' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg > *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(ge) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'>=' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg >= *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(lt) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'<' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg < * (m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(le) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'<=' %i", *(m_int**)(shred->reg - SZ_INT * 2), *(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int**)shred->reg <= *(m_int**)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

// bitwise
static INSTR(sl) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'<<' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg << * (m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(sr) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'>>' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg >> *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}


static INSTR(sand) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'&' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg & *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(sor) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'|' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg | *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(xor) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'^' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg ^ * (m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

// unary
INSTR(negate) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) '-' %i", *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT);
  *(m_int*)shred->reg *= -1;
  PUSH_REG(shred, SZ_INT);
}

INSTR(noti) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'!' %i", *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT);
  *(m_int*)shred->reg = !*(m_int*)shred->reg;
  PUSH_REG(shred, SZ_INT);
}

INSTR(pre_inc) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(++int) %i", **(m_int**)(shred->reg - SZ_INT));
#endif
  *(m_int*)(shred->reg - SZ_INT) = (++**(m_int**)(shred->reg - SZ_INT));
}

INSTR(pre_dec) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(--int) %i", **(m_int**)(shred->reg - SZ_INT));
#endif
  *(m_int*)(shred->reg - SZ_INT) = (--**(m_int**)(shred->reg - SZ_INT));
}

INSTR(post_inc) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int++) %i", **(m_int**)(shred->reg - SZ_INT));
#endif
  *(m_int*)(shred->reg - SZ_INT) = (**(m_int**)(shred->reg - SZ_INT))++;
}

INSTR(post_dec) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int++) %i", **(m_int**)(shred->reg - SZ_INT));
#endif
  *(m_int*)(shred->reg - SZ_INT) = (**(m_int**)(shred->reg - SZ_INT))--;
}

// reverse arithmetic
static INSTR(r_assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'=>' %i", *(m_int*)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  **(m_int**)(shred->reg + SZ_INT) = *(m_int*)shred->reg;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(r_plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'+=>' %i", *(m_int*)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) += (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(r_minus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'-=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) -= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(r_times) {
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) *= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(r_divide) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'/=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  if(!*(m_int*)(shred->reg)) {
    Except(shred, "ZeroDivideException")
  }
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) /= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(r_modulo) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'%=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) %= (*(m_int*)shred->reg));
  PUSH_REG(shred,  SZ_INT);
}

// reverse logical
static INSTR(r_and) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'&&=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) && (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}
static INSTR(ror) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'||=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) || (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(req) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'==>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) == (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(rneq) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'!=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) != (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(rgt) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'>=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) > (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(rge) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'>==>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) >= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(rlt) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'<=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) < (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(rle) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'<==>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) <= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

// reverse bitwise
static INSTR(rsl) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'<<=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) <<= (*(m_int*)shred->reg));
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(rsr) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'>>=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) >>= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(rsand) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'&=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) &= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(rsor) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'|=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) |= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(rsxor) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'^=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) ^= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}


// import function
m_bool import_int(Env env) {
  CHECK_BB(add_global_type(env, &t_int))
// arithmetic
  CHECK_BB(import_op(env, op_assign,        "int", "int", "int", assign,     1))
  CHECK_BB(import_op(env, op_plus,          "int", "int", "int", plus,       1))
  CHECK_BB(import_op(env, op_minus,         "int", "int", "int", minus,      1))
  CHECK_BB(import_op(env, op_times,         "int", "int", "int", times,      1))
  CHECK_BB(import_op(env, op_divide,        "int", "int", "int", divide,     1))
  CHECK_BB(import_op(env, op_percent,       "int", "int", "int", modulo,     1))
// logical
  CHECK_BB(import_op(env, op_and,           "int", "int", "int", and,        1))
  CHECK_BB(import_op(env, op_or,            "int", "int", "int", or,         1))
  CHECK_BB(import_op(env, op_eq, 			 		  "int", "int", "int", eq,         1))
  CHECK_BB(import_op(env, op_neq, 			 	  "int", "int", "int", neq,        1))
  CHECK_BB(import_op(env, op_gt, 			 	    "int", "int", "int", gt,         1))
  CHECK_BB(import_op(env, op_ge, 			 	    "int", "int", "int", ge,         1))
  CHECK_BB(import_op(env, op_lt, 			 	    "int", "int", "int", lt,         1))
  CHECK_BB(import_op(env, op_le, 			 	    "int", "int", "int", le,         1))
// bitwise
  CHECK_BB(import_op(env, op_shift_right,   "int", "int", "int", sr,         1))
  CHECK_BB(import_op(env, op_shift_left,    "int", "int", "int", sl,         1))
  CHECK_BB(import_op(env, op_s_and,         "int", "int", "int", sand,       1))
  CHECK_BB(import_op(env, op_s_or,          "int", "int", "int", sor,        1))
  CHECK_BB(import_op(env, op_s_xor, 			  "int", "int", "int", xor,        1))
// unary
  CHECK_BB(import_op(env, op_minus,         NULL,   "int", "int", negate,     1))
  CHECK_BB(import_op(env, op_exclamation,   NULL,   "int", "int", noti,        1))
  CHECK_BB(import_op(env, op_plusplus,      "int", NULL,   "int", post_inc,   1))
  CHECK_BB(import_op(env, op_plusplus,      NULL,   "int", "int", pre_inc,    1))
  CHECK_BB(import_op(env, op_minusminus,    "int", NULL,   "int", post_dec,   1))
  CHECK_BB(import_op(env, op_minusminus,    NULL,   "int", "int", pre_dec,    1))
// reverse arithmetic
  CHECK_BB(import_op(env, op_chuck,         "int", "int", "int", r_assign,   1))
  CHECK_BB(import_op(env, op_plus_chuck,    "int", "int", "int", r_plus,     1))
  CHECK_BB(import_op(env, op_minus_chuck,   "int", "int", "int", r_minus,    1))
  CHECK_BB(import_op(env, op_times_chuck,   "int", "int", "int", r_times,    1))
  CHECK_BB(import_op(env, op_divide_chuck,  "int", "int", "int", r_divide,   1))
  CHECK_BB(import_op(env, op_modulo_chuck,  "int", "int", "int", r_modulo,   1))
// reverse logical
  CHECK_BB(import_op(env, op_rand,          "int", "int", "int", r_and,      1))
  CHECK_BB(import_op(env, op_ror,           "int", "int", "int",  ror,       1))
  CHECK_BB(import_op(env, op_req, 			 	  "int", "int", "int", req,        1))
  CHECK_BB(import_op(env, op_rneq, 			 	  "int", "int", "int", rneq,       1))
  CHECK_BB(import_op(env, op_rgt, 			 	  "int", "int", "int", rgt,        1))
  CHECK_BB(import_op(env, op_rge, 			 	  "int", "int", "int", rge,        1))
  CHECK_BB(import_op(env, op_rlt, 			 	  "int", "int", "int", rlt,        1))
  CHECK_BB(import_op(env, op_rle, 			 	  "int", "int", "int", rle,        1))
// reverse bitwise
  CHECK_BB(import_op(env, op_rsl,           "int", "int", "int", rsl,        1))
  CHECK_BB(import_op(env, op_rsr,           "int", "int", "int", rsr,        1))
  CHECK_BB(import_op(env, op_rsand,         "int", "int", "int", rsand,      1))
  CHECK_BB(import_op(env, op_rsor,          "int", "int", "int", rsor,       1))
  CHECK_BB(import_op(env, op_rsxor, 			  "int", "int", "int", rsxor,      1))
  return 1;
}
