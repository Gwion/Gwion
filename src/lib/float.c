#include "type.h"
#include "instr.h"
#include "import.h"

struct Type_ t_float = { "float", SZ_FLOAT, NULL,    te_float };
struct Type_ t_dur   = { "dur",   SZ_FLOAT, NULL,    te_dur };
struct Type_ t_time  = { "time",  SZ_FLOAT, NULL,    te_time };
struct Type_ t_now   = { "@now",  SZ_FLOAT, &t_time, te_now };

static INSTR(float_assign) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)REG(0) = (**(m_float**)REG(0) = *(m_float*)REG(SZ_INT));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_plus) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_float*)REG(0) += *(m_float*)REG(SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_minus) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_float*)REG(0) -= *(m_float*)REG(SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(float_times) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_float*)REG(0) *= *(m_float*)REG(SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_divide) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_float*)REG(0) /= *(m_float*)REG(SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_and) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)REG(0) = (*(m_float*)REG(0) && *(m_float*)REG(SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_or) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)REG(0) = (*(m_float*)REG(0) || *(m_float*)REG(SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_eq) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)REG(0) = (*(m_float*)REG(0) == *(m_float*)REG(SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_neq)  {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)REG(0) = (*(m_float*)REG(0) != *(m_float*)REG(SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_gt) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)REG(0) = (*(m_float*)REG(0) > *(m_float*)REG(SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_ge) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)REG(0) = (*(m_float*)REG(0) >= *(m_float*)REG(SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_lt) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)REG(0) = (*(m_float*)REG(0) < * (m_float*)REG(SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_le) {
  POP_REG(shred, SZ_FLOAT * 2);
  *(m_int*)REG(0) = (*(m_float*)REG(0) <= *(m_float*)REG(SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

INSTR(float_negate) {
  POP_REG(shred, SZ_FLOAT)
    *(m_float*)REG(0) = -*(m_float*)REG(0);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(float_not) {
  POP_REG(shred, SZ_FLOAT)
    *(m_int*)REG(0) = !*(m_float*)REG(0);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_r_assign) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  **(m_float**)REG(SZ_FLOAT) = *(m_float*)REG(0);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_r_plus) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)REG(0) = (**(m_float**)REG(SZ_FLOAT) += (*(m_float*)REG(0)));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_r_minus) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)REG(0) = (**(m_float**)REG(SZ_FLOAT) -= (*(m_float*)REG(0)));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_r_times) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)REG(0) = (**(m_float**)REG(SZ_FLOAT) *= (*(m_float*)REG(0)));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_r_divide) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)REG(0) = (**(m_float**)REG(SZ_FLOAT) /= (*(m_float*)REG(0)));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(int_float_assign) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)REG(0) = (**(m_int**)REG(0) = *(m_float*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_plus) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)REG(0) = *(m_int*)REG(0) + *(m_float*)REG(SZ_INT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(int_float_minus) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)REG(0) = *(m_int*)REG(0) + -*(m_float*)REG(SZ_INT);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(int_float_timesf) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)REG(0) = *(m_int*)REG(0) * *(m_float*)REG(SZ_INT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(int_float_divide) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_float*)REG(0) = *(m_int*)REG(0) / *(m_float*)REG(SZ_INT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(int_float_and) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)REG(0) = (*(m_int*)REG(0) && *(m_float*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_or) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)REG(0) = (*(m_int*)REG(0) || *(m_float*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_eq) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)REG(0) = (*(m_int*)REG(0) == *(m_float*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_neq) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)REG(0) = (*(m_int*)REG(0) != *(m_float*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_gt) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)REG(0) = (*(m_int*)REG(0) > *(m_float*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_ge) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)REG(0) = (*(m_int*)REG(0) >= *(m_float*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_lt) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)REG(0) = (*(m_int*)REG(0) < * (m_float*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_le) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)REG(0) = (*(m_int*)REG(0) <= *(m_float*)REG(SZ_INT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(int_float_r_assign) {
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)REG(0) = (**(m_float**)REG(SZ_INT) = *(m_int*)REG(0));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(int_float_r_plus) {
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)REG(0) = (**(m_float**)REG(SZ_INT) += (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(int_float_r_minus) {
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)REG(0) = (**(m_float**)REG(SZ_INT) -= (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(int_float_r_times) {
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)REG(0) = (**(m_float**)REG(SZ_INT) *= (*(m_int*)REG(0)));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(int_float_r_divide) {
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)REG(0) = (**(m_float**)REG(SZ_INT) /= (*(m_float*)REG(0)));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_int_assign) {
  POP_REG(shred, SZ_INT * 2);
  *(m_float*)REG(0) = (**(m_float**)REG(0) = *(m_int*)REG(SZ_INT));
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_int_plus) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)REG(0) += *(m_int*)REG(SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_int_minus) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)REG(0) -= *(m_int*)REG(SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(float_int_timesf) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)REG(0) *= *(m_int*)REG(SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_int_divide) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_float*)REG(0) /= *(m_int*)REG(SZ_FLOAT);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(float_int_and) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)REG(0) = (*(m_float*)REG(0) && *(m_int*)REG(SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_or) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)REG(0) = (*(m_float*)REG(0) || *(m_int*)REG(SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_eq) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)REG(0) = (*(m_float*)REG(0) == *(m_int*)REG(SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_neq) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)REG(0) = (*(m_float*)REG(0) != *(m_int*)REG(SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_gt) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)REG(0) = (*(m_float*)REG(0) > *(m_int*)REG(SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_ge) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)REG(0) = (*(m_int*)REG(0) >= *(m_int*)REG(SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_lt) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)REG(0) = (*(m_float*)REG(0) < * (m_int*)REG(SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_le) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  *(m_int*)REG(0) = (*(m_float*)REG(0) <= *(m_int*)REG(SZ_FLOAT));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_r_assign) {
  POP_REG(shred, SZ_FLOAT + SZ_INT);
  **(m_int**)REG(SZ_FLOAT) = *(m_float*)REG(0);
  *(m_int*)REG(0) = **(m_int**)REG(SZ_FLOAT);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_r_plus) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_FLOAT) += (*(m_float*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_r_minus) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_FLOAT) += (*(m_float*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_r_times) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_FLOAT) *= (*(m_float*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

static INSTR(float_int_r_divide) {
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  *(m_int*)REG(0) = (**(m_int**)REG(SZ_FLOAT) /= (*(m_float*)REG(0)));
  PUSH_REG(shred, SZ_INT);
}

m_bool import_float(Importer importer) {
  CHECK_BB(importer_add_type(importer,  &t_float))
  CHECK_BB(importer_add_type(importer,  &t_time))
  CHECK_BB(importer_add_type(importer,  &t_dur)) 
  CHECK_BB(importer_oper_begin(importer, "float", "float", "float"))
  CHECK_BB(importer_add_op(importer, op_assign,        float_assign,      0))
  CHECK_BB(importer_add_op(importer, op_plus,          float_plus,        0))
  CHECK_BB(importer_add_op(importer, op_minus,         float_minus,       0))
  CHECK_BB(importer_add_op(importer, op_times,         float_times,       0))
  CHECK_BB(importer_add_op(importer, op_divide,        float_divide,      0))
  CHECK_BB(importer_add_op(importer, op_chuck,         float_r_assign,    0))
  CHECK_BB(importer_add_op(importer, op_plus_chuck,    float_r_plus,      0))
  CHECK_BB(importer_add_op(importer, op_minus_chuck,   float_r_minus,     0))
  CHECK_BB(importer_add_op(importer, op_times_chuck,   float_r_times,     0))
  CHECK_BB(importer_add_op(importer, op_divide_chuck,  float_r_divide,    0))
  CHECK_BB(importer_oper_begin(importer, "float", "float", "int"))
  CHECK_BB(importer_add_op(importer, op_and,           float_and,         0))
  CHECK_BB(importer_add_op(importer, op_or,            float_or,          0))
  CHECK_BB(importer_add_op(importer, op_eq, 			 		  float_eq,         0))
  CHECK_BB(importer_add_op(importer, op_neq, 			 	  float_neq,          0))
  CHECK_BB(importer_add_op(importer, op_gt, 			 	    float_gt,         0))
  CHECK_BB(importer_add_op(importer, op_ge, 			 	    float_ge,         0))
  CHECK_BB(importer_add_op(importer, op_lt, 			 	    float_lt,         0))
  CHECK_BB(importer_add_op(importer, op_le, 			 	    float_le,         0))
  CHECK_BB(importer_oper_begin(importer, NULL,   "float", "float"))
  CHECK_BB(importer_add_op(importer, op_minus,         float_negate,      0))
  CHECK_BB(importer_oper_begin(importer, NULL,   "float", "int"))
  CHECK_BB(importer_add_op(importer, op_exclamation,   float_not,         0))
  CHECK_BB(importer_oper_begin(importer, NULL,   "time", "int"))
  CHECK_BB(importer_add_op(importer, op_exclamation,   float_not,         0))
  CHECK_BB(importer_oper_begin(importer, NULL,   "dur", "int"))
  CHECK_BB(importer_add_op(importer, op_exclamation,   float_not,         0))
  CHECK_BB(importer_oper_begin(importer, "int", "float", "int"))
  CHECK_BB(importer_add_op(importer, op_assign,        int_float_assign,  0))
  CHECK_BB(importer_add_op(importer, op_and,           int_float_and,     0))
  CHECK_BB(importer_add_op(importer, op_or,            int_float_or,      0))
  CHECK_BB(importer_add_op(importer, op_eq, 			 		 int_float_eq,      0))
  CHECK_BB(importer_add_op(importer, op_neq, 			 	   int_float_neq,     0))
  CHECK_BB(importer_add_op(importer, op_gt, 			 	   int_float_gt,      0))
  CHECK_BB(importer_add_op(importer, op_ge, 			 	   int_float_ge,      0))
  CHECK_BB(importer_add_op(importer, op_lt, 			 	   int_float_lt,      0))
  CHECK_BB(importer_add_op(importer, op_le, 			 	   int_float_le,      0))
  CHECK_BB(importer_oper_begin(importer, "int", "float", "float"))
  CHECK_BB(importer_add_op(importer, op_plus,          int_float_plus,    0))
  CHECK_BB(importer_add_op(importer, op_times,         int_float_timesf,  0))
  CHECK_BB(importer_add_op(importer, op_minus,         int_float_minus,   0))
  CHECK_BB(importer_add_op(importer, op_divide,        int_float_divide,  0))
  CHECK_BB(importer_add_op(importer, op_chuck,        int_float_r_assign, 0))
  CHECK_BB(importer_add_op(importer, op_plus_chuck,   int_float_r_plus,   0))
  CHECK_BB(importer_add_op(importer, op_minus_chuck,  int_float_r_minus,  0))
  CHECK_BB(importer_add_op(importer, op_times_chuck,  int_float_r_times,  0))
  CHECK_BB(importer_add_op(importer, op_divide_chuck, int_float_r_divide, 0))
  CHECK_BB(importer_oper_begin(importer, "float", "int", "float"))
  CHECK_BB(importer_add_op(importer, op_assign,       float_int_assign,   0))
  CHECK_BB(importer_add_op(importer, op_plus,         float_int_plus,     0))
  CHECK_BB(importer_add_op(importer, op_minus,        float_int_minus,    0))
  CHECK_BB(importer_add_op(importer, op_times,        float_int_timesf,   0))
  CHECK_BB(importer_add_op(importer, op_divide,       float_int_divide,   0))
  CHECK_BB(importer_oper_begin(importer, "float", "int", "int"))
  CHECK_BB(importer_add_op(importer, op_and,          float_int_and,      0))
  CHECK_BB(importer_add_op(importer, op_or,           float_int_or,       0))
  CHECK_BB(importer_add_op(importer, op_eq, 			 		float_int_eq,       0))
  CHECK_BB(importer_add_op(importer, op_neq, 			 	  float_int_neq,      0))
  CHECK_BB(importer_add_op(importer, op_gt, 			 	  float_int_gt,       0))
  CHECK_BB(importer_add_op(importer, op_ge, 			 	  float_int_ge,       0))
  CHECK_BB(importer_add_op(importer, op_lt, 			 	  float_int_lt,       0))
  CHECK_BB(importer_add_op(importer, op_le, 			 	  float_int_le,       0))
  CHECK_BB(importer_add_op(importer, op_chuck,        float_int_r_assign, 0))
  CHECK_BB(importer_add_op(importer, op_plus_chuck,   float_int_r_plus,   0))
  CHECK_BB(importer_add_op(importer, op_minus_chuck,  float_int_r_minus,  0))
  CHECK_BB(importer_add_op(importer, op_times_chuck,  float_int_r_times,  0))
  CHECK_BB(importer_add_op(importer, op_divide_chuck, float_int_r_divide, 0))

  CHECK_BB(importer_oper_begin(importer, "dur", "dur", "dur"))
  CHECK_BB(importer_add_op(importer, op_chuck,        float_r_assign,     0))
  CHECK_BB(importer_add_op(importer, op_plus,         float_plus,         0))
  CHECK_BB(importer_add_op(importer, op_minus,        float_minus,        0))
  CHECK_BB(importer_add_op(importer, op_times,        float_times,        0))
  CHECK_BB(importer_add_op(importer, op_divide,       float_divide,       0))

  CHECK_BB(importer_oper_begin(importer, "dur", "dur", "int"))
  CHECK_BB(importer_add_op(importer, op_gt,           float_gt,           0))
  CHECK_BB(importer_add_op(importer, op_ge,           float_ge,           0))
  CHECK_BB(importer_add_op(importer, op_lt,           float_lt,           0))
  CHECK_BB(importer_add_op(importer, op_le,           float_le,           0))

  CHECK_BB(importer_oper_begin(importer, "time", "time", "time"))
  CHECK_BB(importer_add_op(importer, op_chuck,        float_r_assign,     0))

  CHECK_BB(importer_oper_begin(importer, "time", "dur", "time"))
  CHECK_BB(importer_add_op(importer, op_plus,         float_plus,         0))
  CHECK_BB(importer_oper_begin(importer, "dur", "time", "time"))
  CHECK_BB(importer_add_op(importer, op_chuck,        float_r_assign,     0))
  CHECK_BB(importer_add_op(importer, op_plus,         float_plus,         0))
  CHECK_BB(importer_oper_begin(importer,  "dur",  "@now", "time"))
  CHECK_BB(importer_add_op(importer, op_chuck,        Time_Advance,       0))

  CHECK_BB(importer_oper_begin(importer, "time", "time", "int"))
  CHECK_BB(importer_add_op(importer, op_gt,           float_gt,           0))
  return 1;
}
