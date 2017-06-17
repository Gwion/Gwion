#include <math.h>
#include "defs.h"
#include "import.h"

struct Type_ t_float     = { "float", SZ_FLOAT,  NULL,    te_float };
struct Type_ t_dur       = { "dur",   SZ_FLOAT,  NULL,    te_dur };
struct Type_ t_time      = { "time",  SZ_FLOAT,  NULL,    te_time };
struct Type_ t_now       = { "@now",  SZ_FLOAT,  &t_time, te_now };

static INSTR(float_assign) {
  POP_REG(shred, SZ_INT+SZ_FLOAT);
  *(m_float*)(shred->reg) = (**(m_float**)shred->reg = *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_plus) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_float*)shred->reg += *(m_float*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_minus) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_float*)shred->reg -= *(m_float*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(float_times) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_float*)shred->reg *= *(m_float*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_divide) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_float*)shred->reg /= *(m_float*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_and) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg && *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_or) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg || *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_eq) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg == *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_neq)  {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg != *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_gt) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg > *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_ge) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg >= *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

 static INSTR(float_lt) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg < * (m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_le) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)shred->reg = (*(m_float*)shred->reg <= *(m_float*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

INSTR(float_negate) {
  POP_REG(shred, SZ_FLOAT)
  *(m_float*)shred->reg = -*(m_float*)shred->reg;
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(float_not) {
  POP_REG(shred, SZ_FLOAT)
  *(m_int*)shred->reg = !*(m_float*)shred->reg;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_r_assign) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  **(m_float**)(shred->reg + SZ_FLOAT) = *(m_float*)shred->reg;
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_r_plus) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_FLOAT) += (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_r_minus) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_FLOAT) -= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_r_times) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_FLOAT) *= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_r_divide) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_FLOAT) /= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(int_float_assign) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)(shred->reg) = (**(m_int**)shred->reg = *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_plus) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)shred->reg = *(m_int*)shred->reg + *(m_float*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(int_float_minus) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)shred->reg = *(m_int*)shred->reg + -*(m_float*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(int_float_timesf) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)shred->reg = *(m_int*)shred->reg * *(m_float*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(int_float_divide) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)shred->reg = *(m_int*)shred->reg / *(m_float*)(shred->reg + SZ_INT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(int_float_and) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg && *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_or) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg || *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_eq) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg == *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_neq) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg != *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_gt) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg > *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_ge) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg >= *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_lt) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg < * (m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_le) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg <= *(m_float*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_r_assign) {
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)shred->reg = (**(m_float**)(shred->reg + SZ_INT) = *(m_int*)shred->reg);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(int_float_r_plus) {
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_INT) += (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(int_float_r_minus) {
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_INT) -= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(int_float_r_times) {
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_INT) *= (*(m_int*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(int_float_r_divide) {
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)(shred->reg) = (**(m_float**)(shred->reg + SZ_INT) /= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_int_assign) {
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)(shred->reg) = (**(m_float**)shred->reg = *(m_int*)(shred->reg + SZ_INT));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_int_plus) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)shred->reg += *(m_int*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_int_minus) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)shred->reg -= *(m_int*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(float_int_timesf) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)shred->reg *= *(m_int*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_int_divide) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)shred->reg /= *(m_int*)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_int_and) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg && *(m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_or) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg || *(m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_eq) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg == *(m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_neq) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg != *(m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_gt) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg > *(m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_ge) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)shred->reg = (*(m_int*)shred->reg >= *(m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_lt) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg < * (m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_le) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)shred->reg = (*(m_float*)shred->reg <= *(m_int*)(shred->reg + SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_r_assign) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  **(m_int**)(shred->reg + SZ_FLOAT) = *(m_float*)shred->reg;
  *(m_int*)shred->reg = **(m_int**)(shred->reg + SZ_FLOAT);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_r_plus) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_FLOAT) += (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_r_minus) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_FLOAT) += (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_r_times) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_FLOAT) *= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_r_divide) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)(shred->reg) = (**(m_int**)(shred->reg + SZ_FLOAT) /= (*(m_float*)shred->reg));
  PUSH_REG(shred, SZ_INT);
}

m_bool import_float(Env env) {
  CHECK_BB(env_add_type(env, &t_float))
  CHECK_BB(env_add_type(env, &t_time))
  CHECK_BB(env_add_type(env, &t_dur))
  CHECK_BB(import_op(env, op_assign,        "float", "float", "float", float_assign,    0))
  CHECK_BB(import_op(env, op_plus,          "float", "float", "float", float_plus,      0))
  CHECK_BB(import_op(env, op_minus,         "float", "float", "float", float_minus,     0))
  CHECK_BB(import_op(env, op_times,         "float", "float", "float", float_times,    0))
  CHECK_BB(import_op(env, op_divide,        "float", "float", "float", float_divide,    0))
  CHECK_BB(import_op(env, op_and,           "float", "float", "int", float_and,         0))
  CHECK_BB(import_op(env, op_or,            "float", "float", "int", float_or,          0))
  CHECK_BB(import_op(env, op_eq, 			 		  "float", "float", "int", float_eq,          0))
  CHECK_BB(import_op(env, op_neq, 			 	  "float", "float", "int", float_neq,         0))
  CHECK_BB(import_op(env, op_gt, 			 	    "float", "float", "int", float_gt,          0))
  CHECK_BB(import_op(env, op_ge, 			 	    "float", "float", "int", float_ge,          0))
  CHECK_BB(import_op(env, op_lt, 			 	    "float", "float", "int", float_lt,          0))
  CHECK_BB(import_op(env, op_le, 			 	    "float", "float", "int", float_le,          0))
  CHECK_BB(import_op(env, op_minus,         NULL,   "float", "float",float_negate,     0))
  CHECK_BB(import_op(env, op_exclamation,   NULL,   "float",   "int",float_not,        0))
  CHECK_BB(import_op(env, op_exclamation,   NULL,   "time",    "int",float_not,        0))
  CHECK_BB(import_op(env, op_exclamation,   NULL,   "dur",    "int", float_not,        0))
  CHECK_BB(import_op(env, op_chuck,         "float", "float", "float", float_r_assign,  0))
  CHECK_BB(import_op(env, op_plus_chuck,    "float", "float", "float", float_r_plus,    0))
  CHECK_BB(import_op(env, op_minus_chuck,   "float", "float", "float", float_r_minus,   0))
  CHECK_BB(import_op(env, op_times_chuck,   "float", "float", "float", float_r_times,   0))
  CHECK_BB(import_op(env, op_divide_chuck,  "float", "float", "float", float_r_divide,  0))
  CHECK_BB(import_op(env, op_assign,        "int", "float", "int",   int_float_assign,   0))
  CHECK_BB(import_op(env, op_plus,          "int", "float", "float", int_float_plus,     0))
  CHECK_BB(import_op(env, op_minus,         "int", "float", "float", int_float_minus,    0))
  CHECK_BB(import_op(env, op_times,         "int", "float", "float", int_float_timesf,   0))
  CHECK_BB(import_op(env, op_divide,        "int", "float", "float", int_float_divide,   0))
  CHECK_BB(import_op(env, op_and,           "int", "float", "int", int_float_and,        0))
  CHECK_BB(import_op(env, op_or,            "int", "float", "int", int_float_or,         0))
  CHECK_BB(import_op(env, op_eq, 			 		  "int", "float", "int", int_float_eq,         0))
  CHECK_BB(import_op(env, op_neq, 			 	  "int", "float", "int", int_float_neq,        0))
  CHECK_BB(import_op(env, op_gt, 			 	    "int", "float", "int", int_float_gt,         0))
  CHECK_BB(import_op(env, op_ge, 			 	    "int", "float", "int", int_float_ge,         0))
  CHECK_BB(import_op(env, op_lt, 			 	    "int", "float", "int", int_float_lt,         0))
  CHECK_BB(import_op(env, op_le, 			 	    "int", "float", "int", int_float_le,         0))
  CHECK_BB(import_op(env, op_chuck,         "int", "float", "float", int_float_r_assign,  0))
  CHECK_BB(import_op(env, op_plus_chuck,    "int", "float", "float", int_float_r_plus,    0))
  CHECK_BB(import_op(env, op_minus_chuck,   "int", "float", "float", int_float_r_minus,   0))
  CHECK_BB(import_op(env, op_times_chuck,   "int", "float", "float", int_float_r_times,   0))
  CHECK_BB(import_op(env, op_divide_chuck,  "int", "float", "float", int_float_r_divide,  0))
  CHECK_BB(import_op(env, op_assign,        "float",  "int",  "float", float_int_assign, 0))
  CHECK_BB(import_op(env, op_plus,          "float",  "int",  "float", float_int_plus,   0))
  CHECK_BB(import_op(env, op_minus,         "float",  "int",  "float", float_int_minus,  0))
  CHECK_BB(import_op(env, op_times,         "float",  "int",  "float", float_int_timesf, 0))
  CHECK_BB(import_op(env, op_divide,        "float",  "int",  "float", float_int_divide, 0))
  CHECK_BB(import_op(env, op_and,           "float", "int", "int", float_int_and,        0))
  CHECK_BB(import_op(env, op_or,            "float", "int", "int", float_int_or,         0))
  CHECK_BB(import_op(env, op_eq, 			 		  "float", "int", "int", float_int_eq,         0))
  CHECK_BB(import_op(env, op_neq, 			 	  "float", "int", "int", float_int_neq,        0))
  CHECK_BB(import_op(env, op_gt, 			 	    "float", "int", "int", float_int_gt,         0))
  CHECK_BB(import_op(env, op_ge, 			 	    "float", "int", "int", float_int_ge,         0))
  CHECK_BB(import_op(env, op_lt, 			 	    "float", "int", "int", float_int_lt,         0))
  CHECK_BB(import_op(env, op_le, 			 	    "float", "int", "int", float_int_le,         0))
  CHECK_BB(import_op(env, op_chuck,        "float",  "int", "int", float_int_r_assign, 0))
  CHECK_BB(import_op(env, op_plus_chuck,    "float", "int", "int", float_int_r_plus,    0))
  CHECK_BB(import_op(env, op_minus_chuck,   "float", "int", "int", float_int_r_minus,   0))
  CHECK_BB(import_op(env, op_times_chuck,   "float", "int", "int", float_int_r_times,   0))
  CHECK_BB(import_op(env, op_divide_chuck,  "float", "int", "int", float_int_r_divide,  0))

  CHECK_BB(import_op(env, op_chuck,        "dur",  "dur", "dur",    float_r_assign, 0))
  CHECK_BB(import_op(env, op_plus,         "dur",  "dur", "dur",    float_plus, 0))
  CHECK_BB(import_op(env, op_minus,         "dur",  "dur", "dur",   float_minus, 0))
  CHECK_BB(import_op(env, op_times,         "dur",  "dur", "dur",   float_times, 0))
  CHECK_BB(import_op(env, op_times,         "dur",  "float", "dur", float_times, 0))
  CHECK_BB(import_op(env, op_divide,        "dur",  "dur", "float", float_divide, 0))
  CHECK_BB(import_op(env, op_divide,        "dur",  "float", "dur", float_divide, 0))

  CHECK_BB(import_op(env, op_gt,           "dur",  "dur", "int", float_gt,   0))
  CHECK_BB(import_op(env, op_ge,           "dur",  "dur", "int", float_ge,   0))
  CHECK_BB(import_op(env, op_lt,           "dur",  "dur", "int", float_lt,   0))
  CHECK_BB(import_op(env, op_le,           "dur",  "dur", "int", float_le,   0))

  CHECK_BB(import_op(env, op_chuck,        "time", "time", "time", float_r_assign, 0))
  CHECK_BB(import_op(env, op_chuck,        "dur",  "time", "time", float_r_assign, 0))
  CHECK_BB(import_op(env, op_chuck,        "dur",  "@now",  "time", Time_Advance, 0))
  CHECK_BB(import_op(env, op_plus,         "time", "dur",  "time", float_plus,   0))
  CHECK_BB(import_op(env, op_plus,         "dur",  "time", "time", float_plus,   0))
  CHECK_BB(import_op(env, op_gt,           "time",  "time", "int", float_gt,   0))
  CHECK_BB(import_op(env, op_ge,           "time",  "time", "int", float_ge,   0))
  CHECK_BB(import_op(env, op_lt,           "time",  "time", "int", float_lt,   0))
  CHECK_BB(import_op(env, op_le,           "time",  "time", "int", float_le,   0))
  return 1;
}
