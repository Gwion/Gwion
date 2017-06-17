#include <math.h>
#include "defs.h"
#include "shreduler.h" // for event wait
#include "import.h"

struct Type_ t_float     = { "float", SZ_FLOAT,  NULL,    te_float };
struct Type_ t_dur       = { "dur",   SZ_FLOAT,  NULL,    te_dur };
struct Type_ t_time      = { "time",  SZ_FLOAT,  NULL,    te_time };
struct Type_ t_now       = { "@now",  SZ_FLOAT,  &t_time, te_now };

// arithmetic
static INSTR(assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '=' %f %f", **(m_float**)(shred->reg - SZ_INT-SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT+SZ_FLOAT);
  *(m_float*)(shred->reg) = (**(m_float**)shred->reg = *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_float*)shred->reg += *(m_float*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(minus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '-' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_float*)shred->reg -= *(m_float*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(timesf) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '*' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_float*)shred->reg *= *(m_float*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(divide) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '/' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_float*)shred->reg /= *(m_float*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

// logical
static INSTR(and) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '&&' %f %f", *(m_float*)(shred->reg - SZ_FLOAT*2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg && *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(or) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '||' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg || *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(f_eq) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '==' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg == *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(neq) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '!=' %f %f", *(m_float*)(shred->reg - SZ_FLOAT*2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg != *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(gt) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '>' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg > *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(ge) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '>=' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg >= *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(lt) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '<' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg < * (m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(le) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '<=' %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg <= *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}


// unary
INSTR(negatef) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '-' %f", *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT)
  *(m_float*)shred->reg = -*(m_float*)shred->reg;
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(notf) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '!' %f", *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT)
  *(m_int*)shred->reg = !*(m_float*)shred->reg;
  PUSH_REG(shred, SZ_INT);
}

// reverse arithmetic
static INSTR(r_assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '=>' f f %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  **(m_float**)(shred->reg + SZ_FLOAT) = *(m_float*)shred->reg;
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(r_plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+=>' %f %f", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_FLOAT) += (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(r_minus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '-=>' %f %f", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_FLOAT) -= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(r_times) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '*=>' %f %f", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_FLOAT) *= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(r_divide) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '/=>' %f %f", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_FLOAT) /= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(Time_Advance) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "time advance %f %f", *(m_float*)(shred->reg - SZ_FLOAT*2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT * 2);
  shred->wake_time += *(m_float*)shred->reg;
  shred->is_running = 0;
  shredule(vm->shreduler, shred, shred->wake_time);
  *(m_float*)shred->reg = shred->wake_time;
  PUSH_REG(shred, SZ_FLOAT);
}

// arithmetic
static INSTR(if_assign) {
#ifdef DEBUG_INSTR
//  debug_msg("instr", "(float) '=' %i %f", **(m_int**)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)(shred->reg) = 
(**(m_int**)shred->reg = 
*(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(if_plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+' %i %f", *(m_int*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)shred->reg = *(m_int*)shred->reg + *(m_float*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(if_minus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '-' %i %f", *(m_int*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)shred->reg = *(m_int*)shred->reg + -*(m_float*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(if_timesf) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '*' %i %f", *(m_int*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)shred->reg = *(m_int*)shred->reg * *(m_float*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(if_divide) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '/' %i %f", *(m_int*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)shred->reg = *(m_int*)shred->reg / *(m_float*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_FLOAT);
}
// if logical
static INSTR(if_and) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) %i '&&' %f", *(m_int*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg && *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(if_or) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) %i '||' %f", *(m_int*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg || *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(if_eq) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float)  %i '==' %f", *(m_int*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg == *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(if_neq) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) %i '!=' %f", *(m_int*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg != *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(if_gt) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) %i '>' %f", *(m_int*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg > *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(if_ge) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) %i '>=' %f", *(m_int*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg >= *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(if_lt) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) %i '<' %f", *(m_int*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg < * (m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(if_le) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) %i '<=' %f", *(m_int*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg <= *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

// reverse arihmetic
static INSTR(ifr_assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '=>' if %i %f", *(m_int*)(shred->reg - SZ_INT * 2), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)shred->reg = (**(m_float**)(shred->reg + SZ_INT) = *(m_int*)shred->reg);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(ifr_plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+=>' %i %f", *(m_int*)(shred->reg - SZ_INT * 2), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_INT) += (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(ifr_minus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '-=>' %i %f", *(m_int*)(shred->reg - SZ_INT * 2), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_INT) -= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(ifr_times) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '*=>' %i %f", *(m_int*)(shred->reg - SZ_INT * 2), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_INT) *= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(ifr_divide) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '/=>' %f %f", *(m_int*)(shred->reg - SZ_INT * 2), **(m_float**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_INT) /= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}
// float to int
static INSTR(fi_assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '=' %f %i", **(m_float**)(shred->reg - SZ_INT*2), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)(shred->reg) = (**(m_float**)shred->reg = *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_FLOAT);
}


static INSTR(fi_plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+' %f %i", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_int*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)shred->reg += *(m_int*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(fi_minus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '-' %f %i", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_int*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)shred->reg -= *(m_int*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(fi_timesf) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '*' %f %i", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_int*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)shred->reg *= *(m_int*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(fi_divide) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '/' %f %i", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)shred->reg /= *(m_int*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}


static INSTR(fi_and) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %f '&&' %f", *(m_float*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg && *(m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}


static INSTR(fi_or) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %f '||' %i", *(m_float*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg || *(m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_eq) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float int) '==' %f %i", *(m_float*)(shred->reg - SZ_INT - SZ_FLOAT), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg == *(m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_neq) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %f '!=' %i", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg != *(m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_gt) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %f '>' %i", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg > *(m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_ge) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %f '>=' %i", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg >= *(m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_lt) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %f '<' %i", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg < * (m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_le) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(int) %f '<=' %i", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), *(m_int*)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg <= *(m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_r_assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '=>' f i %f %i", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  **(m_int**)(shred->reg + SZ_FLOAT) = *(m_float*)shred->reg;
  *(m_int*)shred->reg = **(m_int**)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_r_plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+=>' %i %f", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_FLOAT) += (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_r_minus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+=>' %f %i", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_FLOAT) += (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_r_times) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+=>' %f %i", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_FLOAT) *= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(fi_r_divide) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "(float) '+=>' %f %i", *(m_float*)(shred->reg - SZ_FLOAT - SZ_INT), **(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_FLOAT) /= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

m_bool import_float(Env env) {
  CHECK_BB(add_global_type(env, &t_float))
  CHECK_BB(add_global_type(env, &t_time))
  CHECK_BB(add_global_type(env, &t_dur))
// arithmetic
  CHECK_BB(import_op(env, op_assign,        "float", "float", "float", assign,    0))
  CHECK_BB(import_op(env, op_plus,          "float", "float", "float", plus,      0))
  CHECK_BB(import_op(env, op_minus,         "float", "float", "float", minus,     0))
  CHECK_BB(import_op(env, op_times,         "float", "float", "float", timesf,    0))
  CHECK_BB(import_op(env, op_divide,        "float", "float", "float", divide,    0))
  /*	CHECK_BB(import_op(env, op_percent,       "float", "float", "float", modulo,     0))*/
// logical
  CHECK_BB(import_op(env, op_and,           "float", "float", "int", and,         0))
  CHECK_BB(import_op(env, op_or,            "float", "float", "int", or,          0))
  CHECK_BB(import_op(env, op_eq, 			 		  "float", "float", "int", f_eq,          0))
  CHECK_BB(import_op(env, op_neq, 			 	  "float", "float", "int", neq,         0))
  CHECK_BB(import_op(env, op_gt, 			 	    "float", "float", "int", gt,          0))
  CHECK_BB(import_op(env, op_ge, 			 	    "float", "float", "int", ge,          0))
  CHECK_BB(import_op(env, op_lt, 			 	    "float", "float", "int", lt,          0))
  CHECK_BB(import_op(env, op_le, 			 	    "float", "float", "int", le,          0))
// unary
  CHECK_BB(import_op(env, op_minus,         NULL,   "float", "float", negatef,     0))
  CHECK_BB(import_op(env, op_exclamation,   NULL,   "float",   "int",  notf,        0))
  CHECK_BB(import_op(env, op_exclamation,   NULL,   "time",    "int",  notf,        0))
  CHECK_BB(import_op(env, op_exclamation,   NULL,   "dur",    "int",  notf,        0))
// reverse arithmetic
  CHECK_BB(import_op(env, op_chuck,         "float", "float", "float", r_assign,  0))
  CHECK_BB(import_op(env, op_plus_chuck,    "float", "float", "float", r_plus,    0))
  CHECK_BB(import_op(env, op_minus_chuck,   "float", "float", "float", r_minus,   0))
  CHECK_BB(import_op(env, op_times_chuck,   "float", "float", "float", r_times,   0))
  CHECK_BB(import_op(env, op_divide_chuck,  "float", "float", "float", r_divide,  0))

// int to float
  // arithmetic
  CHECK_BB(import_op(env, op_assign,        "int", "float", "int",   if_assign,   0))
  CHECK_BB(import_op(env, op_plus,          "int", "float", "float", if_plus,     0))
  CHECK_BB(import_op(env, op_minus,         "int", "float", "float", if_minus,    0))
  CHECK_BB(import_op(env, op_times,         "int", "float", "float", if_timesf,   0))
  CHECK_BB(import_op(env, op_divide,        "int", "float", "float", if_divide,   0))
  // logical
  CHECK_BB(import_op(env, op_and,           "int", "float", "int", if_and,        0))
  CHECK_BB(import_op(env, op_or,            "int", "float", "int", if_or,         0))
  CHECK_BB(import_op(env, op_eq, 			 		  "int", "float", "int", if_eq,         0))
  CHECK_BB(import_op(env, op_neq, 			 	  "int", "float", "int", if_neq,        0))
  CHECK_BB(import_op(env, op_gt, 			 	    "int", "float", "int", if_gt,         0))
  CHECK_BB(import_op(env, op_ge, 			 	    "int", "float", "int", if_ge,         0))
  CHECK_BB(import_op(env, op_lt, 			 	    "int", "float", "int", if_lt,         0))
  CHECK_BB(import_op(env, op_le, 			 	    "int", "float", "int", if_le,         0))
// reverse arithmetic
  CHECK_BB(import_op(env, op_chuck,         "int", "float", "float", ifr_assign,  0))
  CHECK_BB(import_op(env, op_plus_chuck,    "int", "float", "float", ifr_plus,    0))
  CHECK_BB(import_op(env, op_minus_chuck,   "int", "float", "float", ifr_minus,   0))
  CHECK_BB(import_op(env, op_times_chuck,   "int", "float", "float", ifr_times,   0))
  CHECK_BB(import_op(env, op_divide_chuck,  "int", "float", "float", ifr_divide,  0))

// float to int
  // arithmetic
  CHECK_BB(import_op(env, op_assign,        "float",  "int",  "float", fi_assign, 0))
  CHECK_BB(import_op(env, op_plus,          "float",  "int",  "float", fi_plus,   0))
  CHECK_BB(import_op(env, op_minus,         "float",  "int",  "float", fi_minus,  0))
  CHECK_BB(import_op(env, op_times,         "float",  "int",  "float", fi_timesf, 0))
  CHECK_BB(import_op(env, op_divide,        "float",  "int",  "float", fi_divide, 0))
  // logical
  CHECK_BB(import_op(env, op_and,           "float", "int", "int", fi_and,        0))
  CHECK_BB(import_op(env, op_or,            "float", "int", "int", fi_or,         0))
  CHECK_BB(import_op(env, op_eq, 			 		  "float", "int", "int", fi_eq,         0))
  CHECK_BB(import_op(env, op_neq, 			 	  "float", "int", "int", fi_neq,        0))
  CHECK_BB(import_op(env, op_gt, 			 	    "float", "int", "int", fi_gt,         0))
  CHECK_BB(import_op(env, op_ge, 			 	    "float", "int", "int", fi_ge,         0))
  CHECK_BB(import_op(env, op_lt, 			 	    "float", "int", "int", fi_lt,         0))
  CHECK_BB(import_op(env, op_le, 			 	    "float", "int", "int", fi_le,         0))
  // reverse arithmetic
  CHECK_BB(import_op(env, op_chuck,        "float",  "int", "int", fi_r_assign, 0))
  CHECK_BB(import_op(env, op_plus_chuck,    "float", "int", "int", fi_r_plus,    0))
  CHECK_BB(import_op(env, op_minus_chuck,   "float", "int", "int", fi_r_minus,   0))
  CHECK_BB(import_op(env, op_times_chuck,   "float", "int", "int", fi_r_times,   0))
  CHECK_BB(import_op(env, op_divide_chuck,  "float", "int", "int", fi_r_divide,  0))

  CHECK_BB(import_op(env, op_chuck,        "dur",  "dur", "dur", r_assign, 0))
  CHECK_BB(import_op(env, op_plus,         "dur",  "dur", "dur", plus, 0))
  CHECK_BB(import_op(env, op_minus,         "dur",  "dur", "dur", minus, 0))
  CHECK_BB(import_op(env, op_times,         "dur",  "dur", "dur", timesf, 0))
  CHECK_BB(import_op(env, op_times,         "dur",  "float", "dur", timesf, 0))
  CHECK_BB(import_op(env, op_divide,        "dur",  "dur", "float", divide, 0))
  CHECK_BB(import_op(env, op_divide,        "dur",  "float", "dur", divide, 0))
  /*	CHECK_BB(import_op(env, op_chuck,        "time", "dur", "dur", assign, 0))*/

  CHECK_BB(import_op(env, op_gt,           "dur",  "dur", "int", gt,   0))
  CHECK_BB(import_op(env, op_ge,           "dur",  "dur", "int", ge,   0))
  CHECK_BB(import_op(env, op_lt,           "dur",  "dur", "int", lt,   0))
  CHECK_BB(import_op(env, op_le,           "dur",  "dur", "int", le,   0))

  CHECK_BB(import_op(env, op_chuck,        "time", "time", "time", r_assign, 0))
  CHECK_BB(import_op(env, op_chuck,        "dur",  "time", "time", r_assign, 0))
  CHECK_BB(import_op(env, op_chuck,        "dur",  "@now",  "time", Time_Advance, 0))
  CHECK_BB(import_op(env, op_plus,         "time", "dur",  "time", plus,   0))
  CHECK_BB(import_op(env, op_plus,         "dur",  "time", "time", plus,   0))
  CHECK_BB(import_op(env, op_gt,           "time",  "time", "int", gt,   0))
  CHECK_BB(import_op(env, op_ge,           "time",  "time", "int", ge,   0))
  CHECK_BB(import_op(env, op_lt,           "time",  "time", "int", lt,   0))
  CHECK_BB(import_op(env, op_le,           "time",  "time", "int", le,   0))
  return 1;
}
