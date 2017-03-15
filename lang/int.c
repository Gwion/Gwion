#include "defs.h"
#include "vm.h"
#include "type.h"
#include "err_msg.h"
#include "instr.h"

struct Type_ t_int       = { "int",        SZ_INT,   NULL, te_int};

// arithmetic
static INSTR(assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'=' %i", **(m_int**)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)shred->reg = *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(plus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'+' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg += *(m_int*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(minus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'-' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg -= *(m_int*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(times)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'*' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg *= *(m_int*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(divide)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'/' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg /= *(m_int*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(modulo)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'%' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg %= *(m_int*)(shred->reg + SZ_INT);
  PUSH_REG(shred,  SZ_INT);
}

// logical
static INSTR( and )
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'&&' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg && *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR( or )
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'||' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg || *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(eq)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'==' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg == *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(neq)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'!=' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg != *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(gt)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'>' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg > *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(ge)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'>=' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg >= *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(lt)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'<' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg < * (m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(le)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'<=' %i", *(m_int**)(shred->reg - SZ_INT * 2), *(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int**)shred->reg <= *(m_int**)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

// bitwise
static INSTR(sl)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'<<' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg << * (m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(sr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'>>' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg >> *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}


static INSTR(sand)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'&' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg & *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(sor)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'|' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg | *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(xor)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'^' %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)shred->reg = (*(m_int*)shred->reg ^ * (m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

// unary
INSTR(negate)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'-' %i", *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT);
  *(m_int*)shred->reg = -*(m_int*)shred->reg;
  PUSH_REG(shred, SZ_INT);
}

INSTR(not)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'!' %i", *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT);
  *(m_int*)shred->reg = !*(m_int*)shred->reg;
  PUSH_REG(shred, SZ_INT);
}

INSTR(inc)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int++) '++' %i", **(m_int**)(shred->reg - SZ_INT));
#endif
  (**(m_int**)(shred->reg - SZ_INT))++;
  *(m_int*)(shred->reg - SZ_INT) = **(m_int**)(shred->reg - SZ_INT);
}

INSTR(dec)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int++) '--' %i", **(m_int**)(shred->reg - SZ_INT));
#endif
  (**(m_int**)(shred->reg - SZ_INT))--;
  *(m_int*)(shred->reg - SZ_INT) = **(m_int**)(shred->reg - SZ_INT);
}

// reverse arithmetic
static INSTR(r_assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'=>' %i", *(m_int*)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  **(m_int**)(shred->reg + SZ_INT) = *(m_int*)shred->reg;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(r_plus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'+=>' %i", *(m_int*)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) += (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(r_minus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'-=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) -= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(r_times)
{
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) *= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(r_divide)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'/=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) /= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(r_modulo)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'%=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) %= (*(m_int*)shred->reg));
  PUSH_REG(shred,  SZ_INT);
}

// reverse logical
static INSTR(r_and)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'&&=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) && (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}
static INSTR(ror)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'||=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) || (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(req)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'==>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) == (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(rneq)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'!=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) != (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(rgt)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'>=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) > (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(rge)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'>==>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) >= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(rlt)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'<=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) < (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(rle)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'<==>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) <= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

// reverse bitwise
static INSTR(rsl)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'<<=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) <<= (*(m_int*)shred->reg));
  PUSH_REG(shred,  SZ_INT);
}

static INSTR(rsr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'>>=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) >>= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(rsand)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'&=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) &= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(rsor)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'|=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) |= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(rsxor)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i'^=>' %i", *(m_int**)(shred->reg - SZ_INT * 2), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_INT) ^= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}


// import function
m_bool import_int(Env env)
{
  CHECK_BB(add_global_type(env, &t_int))
// arithmetic
  CHECK_BB(add_binary_op(env, op_assign,        &t_int, &t_int, &t_int, assign,     1, 0))
  CHECK_BB(add_binary_op(env, op_plus,          &t_int, &t_int, &t_int, plus,       1, 0))
  CHECK_BB(add_binary_op(env, op_minus,         &t_int, &t_int, &t_int, minus,      1, 0))
  CHECK_BB(add_binary_op(env, op_times,         &t_int, &t_int, &t_int, times,      1, 0))
  CHECK_BB(add_binary_op(env, op_divide,        &t_int, &t_int, &t_int, divide,     1, 0))
  CHECK_BB(add_binary_op(env, op_percent,       &t_int, &t_int, &t_int, modulo,     1, 0))
// logical
  CHECK_BB(add_binary_op(env, op_and,           &t_int, &t_int, &t_int, and,        1, 0))
  CHECK_BB(add_binary_op(env, op_or,            &t_int, &t_int, &t_int, or,         1, 0))
  CHECK_BB(add_binary_op(env, op_eq, 			 		  &t_int, &t_int, &t_int, eq,         1, 0))
  CHECK_BB(add_binary_op(env, op_neq, 			 	  &t_int, &t_int, &t_int, neq,        1, 0))
  CHECK_BB(add_binary_op(env, op_neq, 			 	  &t_object, &t_null, &t_int, neq, 1, 0))
  CHECK_BB(add_binary_op(env, op_gt, 			 	    &t_int, &t_int, &t_int, gt,         1, 0))
  CHECK_BB(add_binary_op(env, op_ge, 			 	    &t_int, &t_int, &t_int, ge,         1, 0))
  CHECK_BB(add_binary_op(env, op_lt, 			 	    &t_int, &t_int, &t_int, lt,         1, 0))
  CHECK_BB(add_binary_op(env, op_le, 			 	    &t_int, &t_int, &t_int, le,         1, 0))
// bitwise
  CHECK_BB(add_binary_op(env, op_shift_right,   &t_int, &t_int, &t_int, sr,         1, 0))
  CHECK_BB(add_binary_op(env, op_shift_left,    &t_int, &t_int, &t_int, sl,         1, 0))
  CHECK_BB(add_binary_op(env, op_s_and,         &t_int, &t_int, &t_int, sand,       1, 0))
  CHECK_BB(add_binary_op(env, op_s_or,          &t_int, &t_int, &t_int, sor,        1, 0))
  CHECK_BB(add_binary_op(env, op_s_xor, 			  &t_int, &t_int, &t_int, xor,        1, 0))
// unary
  CHECK_BB(add_binary_op(env, op_minus,         NULL,   &t_int, &t_int, negate,     1, 0))
  CHECK_BB(add_binary_op(env, op_exclamation,   NULL,   &t_int, &t_int, not,        1, 0))
  CHECK_BB(add_binary_op(env, op_plusplus,      &t_int, NULL,   &t_int, inc,        1, 0))
  CHECK_BB(add_binary_op(env, op_plusplus,      NULL,   &t_int, &t_int, inc,        1, 0))
  CHECK_BB(add_binary_op(env, op_minusminus,    &t_int, NULL,   &t_int, dec,        1, 0))
  CHECK_BB(add_binary_op(env, op_minusminus,    NULL,   &t_int, &t_int, dec,        1, 0))
// reverse arithmetic
  CHECK_BB(add_binary_op(env, op_chuck,         &t_int, &t_int, &t_int, r_assign,   1, 0))
  CHECK_BB(add_binary_op(env, op_plus_chuck,    &t_int, &t_int, &t_int, r_plus,     1, 0))
  CHECK_BB(add_binary_op(env, op_minus_chuck,   &t_int, &t_int, &t_int, r_minus,    1, 0))
  CHECK_BB(add_binary_op(env, op_times_chuck,   &t_int, &t_int, &t_int, r_times,    1, 0))
  CHECK_BB(add_binary_op(env, op_divide_chuck,  &t_int, &t_int, &t_int, r_divide,   1, 0))
  CHECK_BB(add_binary_op(env, op_modulo_chuck,  &t_int, &t_int, &t_int, r_modulo,   1, 0))
// reverse logical
  CHECK_BB(add_binary_op(env, op_rand,          &t_int, &t_int, &t_int, r_and,      1, 0))
  CHECK_BB(add_binary_op(env, op_ror,           &t_int, &t_int, &t_int,  ror,       1, 0))
  CHECK_BB(add_binary_op(env, op_req, 			 	  &t_int, &t_int, &t_int, req,        1, 0))
  CHECK_BB(add_binary_op(env, op_rneq, 			 	  &t_int, &t_int, &t_int, rneq,       1, 0))
  CHECK_BB(add_binary_op(env, op_rgt, 			 	  &t_int, &t_int, &t_int, rgt,        1, 0))
  CHECK_BB(add_binary_op(env, op_rge, 			 	  &t_int, &t_int, &t_int, rge,        1, 0))
  CHECK_BB(add_binary_op(env, op_rlt, 			 	  &t_int, &t_int, &t_int, rlt,        1, 0))
  CHECK_BB(add_binary_op(env, op_rle, 			 	  &t_int, &t_int, &t_int, rle,        1, 0))
// TODO: reverse bitwise
  CHECK_BB(add_binary_op(env, op_rsl,           &t_int, &t_int, &t_int, rsl,        1, 0))
  CHECK_BB(add_binary_op(env, op_rsr,           &t_int, &t_int, &t_int, rsr,        1, 0))
  CHECK_BB(add_binary_op(env, op_rsand,         &t_int, &t_int, &t_int, rsand,      1, 0))
  CHECK_BB(add_binary_op(env, op_rsor,          &t_int, &t_int, &t_int, rsor,       1, 0))
  CHECK_BB(add_binary_op(env, op_rsxor, 			  &t_int, &t_int, &t_int, rsxor,      1, 0))
  t_int.doc       = "integral number";
  return 1;
}
