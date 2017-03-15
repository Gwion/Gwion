#include <math.h>
#include "defs.h"
#include "vm.h"
#include "type.h"
#include "err_msg.h"
#include "instr.h"

struct Type_ t_float     = { "float",      sizeof(m_float),  NULL,  te_float };
// arithmetic
static INSTR(assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '=' %f %f", **(m_float**)(shred->reg - SZ_FLOAT + SZ_INT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)(shred->reg) = (**(m_float**)shred->reg = *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(plus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_float*)shred->reg += *(m_float*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(minus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '-' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_float*)shred->reg -= *(m_float*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(timesf)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '*' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_float*)shred->reg *= *(m_float*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(divide)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '/' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_float*)shred->reg /= *(m_float*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

// logical
static INSTR( and )
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '&&' %f %f", *(m_float*)(shred->reg - SZ_INT * 2), *(m_float*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg && *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR( or )
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '||' %f %f", *(m_float*)(shred->reg - SZ_INT * 2), *(m_float*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg || *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(eq)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '==' %f %f", *(m_float*)(shred->reg - SZ_INT * 2), *(m_float*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg == *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(neq)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '!=' %f %f", *(m_float*)(shred->reg - SZ_INT * 2), *(m_float*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg != *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(gt)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '>' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg > *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(ge)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '>=' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg >= *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(lt)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '<' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg < * (m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(le)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '<=' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg <= *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}


// unary
INSTR(negatef)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '-' %f", *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT)
  *(m_float*)shred->reg = -*(m_float*)shred->reg;
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(notf)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '!' %f", *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT)
  *(m_int*)shred->reg = !*(m_float*)shred->reg;
  PUSH_REG(shred, SZ_INT);
}

// reverse arithmetic
static INSTR(r_assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '=>' f f %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  **(m_float**)(shred->reg + SZ_FLOAT) = *(m_float*)shred->reg;
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(r_plus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+=>' %f %f", *(m_float*)(shred->reg - SZ_FLOAT + SZ_INT), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_FLOAT) += (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(r_minus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '-=>' %f %f", *(m_float*)(shred->reg - SZ_FLOAT + SZ_INT), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_FLOAT) -= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(r_times)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '*=>' %f %f", *(m_float*)(shred->reg - SZ_FLOAT + SZ_INT), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_FLOAT) *= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(r_divide)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '/=>' %f %f", *(m_float*)(shred->reg - SZ_FLOAT + SZ_INT), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_FLOAT) /= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(Time_Advance)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "time advance %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  shred->wake_time += *(m_float*)shred->reg;
  shred->is_running = 0;
  shredule(vm->shreduler, shred, shred->wake_time);
  *(m_float*)shred->reg = shred->wake_time;
  PUSH_REG(shred, SZ_FLOAT);
}

// arithmetic
static INSTR(if_assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '=' %i %f", **(m_int**)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)(shred->reg) = (**(m_int**)shred->reg = *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(if_plus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+' %i %f", *(m_int*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)shred->reg = *(m_int*)shred->reg + *(m_float*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(if_minus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '-' %i %f", *(m_int*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)shred->reg = *(m_int*)shred->reg + -*(m_float*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(if_timesf)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '*' %i %f", *(m_int*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)shred->reg = *(m_int*)shred->reg * *(m_float*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(if_divide)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '/' %i %f", *(m_int*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)shred->reg = *(m_int*)shred->reg / *(m_float*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_FLOAT);
}
// if logical
static INSTR(if_and)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) %i '&&' %f", *(m_int*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg && *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(if_or)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) %i '||' %f", *(m_int*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg || *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(if_eq)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float)  %i '==' %f", *(m_int*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg == *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(if_neq)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) %i '!=' %f", *(m_int*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg != *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(if_gt)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) %i '>' %f", *(m_int*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg > *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(if_ge)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) %i '>=' %f", *(m_int*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg >= *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(if_lt)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) %i '<' %f", *(m_int*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg < * (m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(if_le)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) %i '<=' %f", *(m_int*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg <= *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

// reverse arihmetic
static INSTR(ifr_assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '=>' if %i %f", *(m_int*)(shred->reg - SZ_INT * 2), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)shred->reg = (**(m_float**)(shred->reg + SZ_INT) = *(m_int*)shred->reg);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(ifr_plus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+=>' %i %f", *(m_int*)(shred->reg - SZ_INT * 2), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_INT) += (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(ifr_minus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '-=>' %i %f", *(m_int*)(shred->reg - SZ_INT * 2), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_INT) -= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(ifr_times)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '*=>' %i %f", *(m_int*)(shred->reg - SZ_INT * 2), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_INT) *= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(ifr_divide)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '/=>' %f %f", *(m_int*)(shred->reg - SZ_INT * 2), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_INT) /= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}
// float to int
static INSTR(fi_assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '=' %i %f", **(m_int**)(shred->reg - SZ_INT*2), *(m_float*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)(shred->reg) = (**(m_float**)shred->reg = *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_FLOAT);
}


static INSTR(fi_plus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+' %f %i", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_int*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)shred->reg += *(m_int*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(fi_minus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '-' %f %i", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_int*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)shred->reg -= *(m_int*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(fi_timesf)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '*' %f %i", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_int*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)shred->reg *= *(m_int*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(fi_divide)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '/' %f %i", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)shred->reg /= *(m_int*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}


static INSTR(fi_and)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i '&&' %f", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg && *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}


static INSTR(fi_or)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %i '||' %f", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg || *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_eq)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) '==' %i %i", *(m_int*)(shred->reg - SZ_INT * 2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg == *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_neq)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %f '!=' %i", *(m_int*)(shred->reg - SZ_FLOAT - SZ_INT), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg != *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_gt)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %f '>' %i", *(m_int*)(shred->reg - SZ_FLOAT - SZ_INT), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg > *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_ge)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %f '>=' %i", *(m_int*)(shred->reg - SZ_FLOAT - SZ_INT), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg >= *(m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_lt)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %f '<' %i", *(m_int*)(shred->reg - SZ_FLOAT - SZ_INT), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg < * (m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_le)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %f '<=' %i", *(m_int**)(shred->reg - SZ_FLOAT - SZ_INT), *(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg <= *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_r_assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '=>' f i %f %i", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  **(m_int**)(shred->reg + SZ_FLOAT) = *(m_float*)shred->reg;
  *(m_int*)shred->reg = **(m_int**)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_r_plus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+=>' %i %f", *(m_int*)(shred->reg - SZ_FLOAT - SZ_INT), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_FLOAT) += (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_r_minus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+=>' %i %f", *(m_int*)(shred->reg - SZ_FLOAT - SZ_INT), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_FLOAT) += (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_r_times)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+=>' %i %f", *(m_int*)(shred->reg - SZ_FLOAT - SZ_INT), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_FLOAT) *= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_r_divide)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+=>' %i %f", *(m_int*)(shred->reg - SZ_FLOAT - SZ_INT), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_FLOAT) /= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

m_bool import_float(Env env)
{
  CHECK_BB(add_global_type(env, &t_float))
// arithmetic
  CHECK_BB(add_binary_op(env, op_assign,        &t_float, &t_float, &t_float, assign,    0, 0))
  CHECK_BB(add_binary_op(env, op_plus,          &t_float, &t_float, &t_float, plus,      0, 0))
  CHECK_BB(add_binary_op(env, op_minus,         &t_float, &t_float, &t_float, minus,     0, 0))
  CHECK_BB(add_binary_op(env, op_times,         &t_float, &t_float, &t_float, timesf,    0, 0))
  CHECK_BB(add_binary_op(env, op_divide,        &t_float, &t_float, &t_float, divide,    0, 0))
  /*	CHECK_BB(add_binary_op(env, op_percent,       &t_float, &t_float, &t_float, modulo,     0, 0))*/
// logical
  CHECK_BB(add_binary_op(env, op_and,           &t_float, &t_float, &t_int, and,         0, 0))
  CHECK_BB(add_binary_op(env, op_or,            &t_float, &t_float, &t_int, or,          0, 0))
  CHECK_BB(add_binary_op(env, op_eq, 			 		  &t_float, &t_float, &t_int, eq,          0, 0))
  CHECK_BB(add_binary_op(env, op_neq, 			 	  &t_float, &t_float, &t_int, neq,         0, 0))
  CHECK_BB(add_binary_op(env, op_gt, 			 	    &t_float, &t_float, &t_int, gt,          0, 0))
  CHECK_BB(add_binary_op(env, op_ge, 			 	    &t_float, &t_float, &t_int, ge,          0, 0))
  CHECK_BB(add_binary_op(env, op_lt, 			 	    &t_float, &t_float, &t_int, lt,          0, 0))
  CHECK_BB(add_binary_op(env, op_le, 			 	    &t_float, &t_float, &t_int, le,          0, 0))
// unary
  CHECK_BB(add_binary_op(env, op_minus,         NULL,   &t_float, &t_float, negatef,     0, 0))
  CHECK_BB(add_binary_op(env, op_exclamation,   NULL,   &t_int,   &t_float, notf,        0, 0))
// reverse arithmetic
  CHECK_BB(add_binary_op(env, op_chuck,         &t_float, &t_float, &t_float, r_assign,  0, 0))
  CHECK_BB(add_binary_op(env, op_plus_chuck,    &t_float, &t_float, &t_float, r_plus,    0, 0))
  CHECK_BB(add_binary_op(env, op_minus_chuck,   &t_float, &t_float, &t_float, r_minus,   0, 0))
  CHECK_BB(add_binary_op(env, op_times_chuck,   &t_float, &t_float, &t_float, r_times,   0, 0))
  CHECK_BB(add_binary_op(env, op_divide_chuck,  &t_float, &t_float, &t_float, r_divide,  0, 0))

// int to float
  // arithmetic
  CHECK_BB(add_binary_op(env, op_assign,        &t_int, &t_float, &t_int,   if_assign,   0, 0))
  CHECK_BB(add_binary_op(env, op_plus,          &t_int, &t_float, &t_float, if_plus,     0, 0))
  CHECK_BB(add_binary_op(env, op_minus,         &t_int, &t_float, &t_float, if_minus,    0, 0))
  CHECK_BB(add_binary_op(env, op_times,         &t_int, &t_float, &t_float, if_timesf,   0, 0))
  CHECK_BB(add_binary_op(env, op_divide,        &t_int, &t_float, &t_float, if_divide,   0, 0))
  // logical
  CHECK_BB(add_binary_op(env, op_and,           &t_int, &t_float, &t_int, if_and,        0, 0))
  CHECK_BB(add_binary_op(env, op_or,            &t_int, &t_float, &t_int, if_or,         0, 0))
  CHECK_BB(add_binary_op(env, op_eq, 			 		  &t_int, &t_float, &t_int, if_eq,         0, 0))
  CHECK_BB(add_binary_op(env, op_neq, 			 	  &t_int, &t_float, &t_int, if_neq,        0, 0))
  CHECK_BB(add_binary_op(env, op_gt, 			 	    &t_int, &t_float, &t_int, if_gt,         0, 0))
  CHECK_BB(add_binary_op(env, op_ge, 			 	    &t_int, &t_float, &t_int, if_ge,         0, 0))
  CHECK_BB(add_binary_op(env, op_lt, 			 	    &t_int, &t_float, &t_int, if_lt,         0, 0))
  CHECK_BB(add_binary_op(env, op_le, 			 	    &t_int, &t_float, &t_int, if_le,         0, 0))
// reverse arithmetic
  CHECK_BB(add_binary_op(env, op_chuck,         &t_int, &t_float, &t_float, ifr_assign,  0, 0))
  CHECK_BB(add_binary_op(env, op_plus_chuck,    &t_int, &t_float, &t_float, ifr_plus,    0, 0))
  CHECK_BB(add_binary_op(env, op_minus_chuck,   &t_int, &t_float, &t_float, ifr_minus,   0, 0))
  CHECK_BB(add_binary_op(env, op_times_chuck,   &t_int, &t_float, &t_float, ifr_times,   0, 0))
  CHECK_BB(add_binary_op(env, op_divide_chuck,  &t_int, &t_float, &t_float, ifr_divide,  0, 0))

// float to int
  // arithmetic
  CHECK_BB(add_binary_op(env, op_assign,        &t_float,  &t_int,  &t_float, fi_assign, 0, 0))
  CHECK_BB(add_binary_op(env, op_plus,          &t_float,  &t_int,  &t_float, fi_plus,   0, 0))
  CHECK_BB(add_binary_op(env, op_minus,         &t_float,  &t_int,  &t_float, fi_minus,  0, 0))
  CHECK_BB(add_binary_op(env, op_times,         &t_float,  &t_int,  &t_float, fi_timesf, 0, 0))
  CHECK_BB(add_binary_op(env, op_divide,        &t_float,  &t_int,  &t_float, fi_divide, 0, 0))
  // logical
  CHECK_BB(add_binary_op(env, op_and,           &t_float, &t_int, &t_int, fi_and,        0, 0))
  CHECK_BB(add_binary_op(env, op_or,            &t_float, &t_int, &t_int, fi_or,         0, 0))
  CHECK_BB(add_binary_op(env, op_eq, 			 		  &t_float, &t_int, &t_int, fi_eq,         0, 0))
  CHECK_BB(add_binary_op(env, op_neq, 			 	  &t_float, &t_int, &t_int, fi_neq,        0, 0))
  CHECK_BB(add_binary_op(env, op_gt, 			 	    &t_float, &t_int, &t_int, fi_gt,         0, 0))
  CHECK_BB(add_binary_op(env, op_ge, 			 	    &t_float, &t_int, &t_int, fi_ge,         0, 0))
  CHECK_BB(add_binary_op(env, op_lt, 			 	    &t_float, &t_int, &t_int, fi_lt,         0, 0))
  CHECK_BB(add_binary_op(env, op_le, 			 	    &t_float, &t_int, &t_int, fi_le,         0, 0))
  // reverse arithmetic
  CHECK_BB(add_binary_op(env, op_chuck,        &t_float,  &t_int, &t_int, fi_r_assign, 0, 0))
  CHECK_BB(add_binary_op(env, op_plus_chuck,    &t_float, &t_int, &t_int, fi_r_plus,    0, 0))
  CHECK_BB(add_binary_op(env, op_minus_chuck,   &t_float, &t_int, &t_int, fi_r_minus,   0, 0))
  CHECK_BB(add_binary_op(env, op_times_chuck,   &t_float, &t_int, &t_int, fi_r_times,   0, 0))
  CHECK_BB(add_binary_op(env, op_divide_chuck,  &t_float, &t_int, &t_int, fi_r_divide,  0, 0))
  t_float.doc     = "floating point number";

  CHECK_BB(add_global_type(env, &t_dur))
  CHECK_BB(add_binary_op(env, op_chuck,        &t_dur,  &t_dur, &t_dur, r_assign, 0, 0))
  CHECK_BB(add_binary_op(env, op_plus,         &t_dur,  &t_dur, &t_dur, plus, 0, 0))
  CHECK_BB(add_binary_op(env, op_minus,         &t_dur,  &t_dur, &t_dur, minus, 0, 0))
  CHECK_BB(add_binary_op(env, op_times,         &t_dur,  &t_dur, &t_dur, timesf, 0, 0))
  CHECK_BB(add_binary_op(env, op_times,         &t_dur,  &t_float, &t_dur, timesf, 0, 0))
  CHECK_BB(add_binary_op(env, op_divide,        &t_dur,  &t_dur, &t_float, divide, 0, 0))
  CHECK_BB(add_binary_op(env, op_divide,        &t_dur,  &t_float, &t_dur, divide, 0, 0))
  /*	CHECK_BB(add_binary_op(env, op_chuck,        &t_time, &t_dur, &t_dur, assign, 0, 0))*/

  CHECK_BB(add_binary_op(env, op_gt,           &t_dur,  &t_dur, &t_int, gt,   0, 0))
  CHECK_BB(add_binary_op(env, op_ge,           &t_dur,  &t_dur, &t_int, ge,   0, 0))
  CHECK_BB(add_binary_op(env, op_lt,           &t_dur,  &t_dur, &t_int, lt,   0, 0))
  CHECK_BB(add_binary_op(env, op_le,           &t_dur,  &t_dur, &t_int, le,   0, 0))
  t_dur.doc       = "duration expression";

  CHECK_BB(add_global_type(env, &t_time))
  CHECK_BB(add_binary_op(env, op_chuck,        &t_time, &t_time, &t_time, r_assign, 0, 0))
  CHECK_BB(add_binary_op(env, op_chuck,        &t_dur,  &t_time, &t_time, r_assign, 0, 0))
  CHECK_BB(add_binary_op(env, op_chuck,        &t_dur,  &t_now,  &t_time, Time_Advance, 0, 0))
  CHECK_BB(add_binary_op(env, op_plus,         &t_time, &t_dur,  &t_time, plus,   0, 0))
  CHECK_BB(add_binary_op(env, op_plus,         &t_dur,  &t_time, &t_time, plus,   0, 0))
  CHECK_BB(add_binary_op(env, op_gt,           &t_time,  &t_time, &t_time, gt,   0, 0))
  CHECK_BB(add_binary_op(env, op_ge,           &t_time,  &t_time, &t_time, ge,   0, 0))
  CHECK_BB(add_binary_op(env, op_lt,           &t_time,  &t_time, &t_time, lt,   0, 0))
  CHECK_BB(add_binary_op(env, op_le,           &t_time,  &t_time, &t_time, le,   0, 0))
  add_global_value_double(env, "t_zero",       &t_time, 0.);
  t_time.doc      = "time expression";
  return 1;
}
