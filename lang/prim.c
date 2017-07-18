#include "math.h"
#include "import.h"

struct Type_ t_int   = { "int",   SZ_INT,   NULL,    te_int};
struct Type_ t_float = { "float", SZ_FLOAT, NULL,    te_float };
struct Type_ t_dur   = { "dur",   SZ_FLOAT, NULL,    te_dur };
struct Type_ t_time  = { "time",  SZ_FLOAT, NULL,    te_time };
struct Type_ t_now   = { "@now",  SZ_FLOAT, &t_time, te_now };
struct Type_ t_complex = { "complex", SZ_COMPLEX, NULL, te_complex};
struct Type_ t_polar   = { "polar",   SZ_COMPLEX, NULL, te_polar};
struct Type_ t_vec3  = { "Vec3",  SZ_VEC3,  NULL,    te_vec3};
struct Type_ t_vec4  = { "Vec4",  SZ_VEC4,  NULL,    te_vec4};

m_int o_complex_real, o_complex_imag;
m_int o_polar_mod,    o_polar_phase;

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
    CHECK_BB(import_op(env, op_minus,         NULL,   "float", "float", float_negate,     0))
    CHECK_BB(import_op(env, op_exclamation,   NULL,   "float",   "int", float_not,        0))
    CHECK_BB(import_op(env, op_exclamation,   NULL,   "time",    "int", float_not,        0))
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

static INSTR(complex_assign) {
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  *(m_complex*)REG(0) = (**(m_complex**)REG(0) = *(m_complex*)REG(SZ_INT));
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_plus) {
  POP_REG(shred, SZ_COMPLEX * 2);
  *(m_complex*)REG(0) += *(m_complex*)REG(SZ_COMPLEX);
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_minus) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  *(m_complex*)REG(0) -= *(m_complex*)REG(SZ_INT);
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_times) {
  POP_REG(shred, SZ_COMPLEX * 2);
  *(m_complex*)REG(0) *= *(m_complex*)REG(SZ_INT);
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_divide) {
  POP_REG(shred, SZ_COMPLEX * 2);
  *(m_complex*)REG(0) /= *(m_complex*)REG(SZ_INT);
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_r_assign) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  **(m_complex**)REG(SZ_COMPLEX) = *(m_complex*)REG(0);
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_r_plus) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  *(m_complex*)REG(0) = (**(m_complex**)REG(SZ_COMPLEX) += (*(m_complex*)REG(0)));
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_r_minus) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  *(m_complex*)REG(0) = (**(m_complex**)REG(SZ_COMPLEX) -= (*(m_complex*)REG(0)));
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_r_times) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  *(m_complex*)REG(0) = (**(m_complex**)REG(SZ_COMPLEX) *= (*(m_complex*)REG(0)));
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_r_divide) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  *(m_complex*)REG(0) = (**(m_complex**)REG(SZ_COMPLEX) /= (*(m_complex*)REG(0)));
  PUSH_REG(shred, SZ_COMPLEX);
}

INSTR(complex_real) {
  POP_REG(shred, SZ_INT);
  if(instr->m_val) {
//    *(m_float**)REG(0) = &**(m_float**)REG(0); // coverity
    PUSH_REG(shred, SZ_INT);
  } else {
    *(m_float*)REG(0) = **(m_float**)REG(0);  // coverity
    PUSH_REG(shred, SZ_FLOAT);
  }
}

INSTR(complex_imag) {
  POP_REG(shred, SZ_INT);
  if(instr->m_val) {
    m_float* f = &**(m_float**)REG(0);
    *(m_float**)REG(0) = &*(f + 1);
    PUSH_REG(shred, SZ_INT);
  } else {
    m_float* f = &**(m_float**)REG(0);
    *(m_float*)REG(0) = *(f + 1);
    PUSH_REG(shred, SZ_FLOAT);
  }
}

static INSTR(polar_plus) {
  POP_REG(shred, SZ_COMPLEX * 2);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = *(m_complex*)REG(SZ_COMPLEX);
  m_float re = creal(a) * cos(cimag(a)) + creal(b) * cos(cimag(b));
  m_float im = creal(a) * sin(cimag(a)) + creal(b) * sin(cimag(b));
  *(m_complex*)REG(0) = hypot(re, im) + atan2(im, re) * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_minus) {
  POP_REG(shred, SZ_COMPLEX * 2);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = *(m_complex*)REG(SZ_COMPLEX);
  m_float re = creal(a) * cos(cimag(a)) - creal(b) * cos(cimag(b));
  m_float im = creal(a) * sin(cimag(a)) - creal(b) * sin(cimag(b));
  *(m_complex*)REG(0) = hypot(re, im) + atan2(im, re) * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_times) {
  POP_REG(shred, SZ_COMPLEX * 2);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = *(m_complex*)REG(SZ_COMPLEX);
  m_float mag   = creal(a) * creal(b);
  m_float phase = cimag(a) + cimag(b);
  *(m_complex*)REG(0) = mag  + phase * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_divide) {
  POP_REG(shred, SZ_COMPLEX * 2);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = *(m_complex*)REG(SZ_COMPLEX);
  m_float mag   = creal(a) / creal(b);
  m_float phase = cimag(a) - cimag(b);
  *(m_complex*)REG(0) = mag  + phase * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_plus_r) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = **(m_complex**)REG(SZ_COMPLEX);
  m_float re = creal(a) * cos(cimag(a)) + creal(b) * cos(cimag(b));
  m_float im = creal(a) * sin(cimag(a)) + sin(cimag(b));
  **(m_complex**)REG(SZ_COMPLEX) = hypot(re, im) + atan2(im, re) * I;
  *(m_complex*)REG(0) = hypot(re, im) + atan2(im, re) * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_minus_r) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = **(m_complex**)REG(SZ_COMPLEX);
  m_float re, im;
  re = creal(a) * cos(cimag(a)) - creal(b) * cos(cimag(b));
  im = creal(a) * sin(cimag(a)) - sin(cimag(b));
  **(m_complex**)REG(SZ_COMPLEX) = hypot(re, im) + atan2(im, re) * I;
  *(m_complex*)REG(0) = hypot(re, im) + atan2(im, re) * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_times_r) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = **(m_complex**)REG(SZ_COMPLEX);
  m_float mag   = creal(a) * creal(b);
  m_float phase = cimag(a) + cimag(b);
  **(m_complex**)REG(SZ_COMPLEX) = mag  + phase * I;
  *(m_complex*)REG(0) = mag  + phase * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_divide_r) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = **(m_complex**)REG(SZ_COMPLEX);
  m_float mag   = creal(a) / creal(b);
  m_float phase = cimag(a) - cimag(b);
  **(m_complex**)REG(SZ_COMPLEX) = mag  + phase * I;
  *(m_complex*)REG(0) = mag  + phase * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

m_bool import_complex(Env env) {
  CHECK_BB(import_class_begin(env, &t_complex, env->global_nspc, NULL, NULL))
  o_complex_real = import_var(env, "float", "re",   ae_flag_member, NULL);
  CHECK_BB(o_complex_real)
  o_complex_imag = import_var(env, "float", "im",   ae_flag_member, NULL);
  CHECK_BB(o_complex_imag)
  CHECK_BB(import_class_end(env))

  CHECK_BB(import_class_begin(env, &t_polar, env->global_nspc, NULL, NULL))
  o_polar_mod = import_var(env, "float", "mod",   ae_flag_member, NULL);
  CHECK_BB(o_polar_mod)
  o_polar_phase = import_var(env, "float", "phase",   ae_flag_member, NULL);
  CHECK_BB(o_polar_phase)
  CHECK_BB(import_class_end(env))
  CHECK_BB(import_op(env, op_assign,        "complex", "complex", "complex", complex_assign,     0))
  CHECK_BB(import_op(env, op_plus,          "complex", "complex", "complex", complex_plus,       0))
  CHECK_BB(import_op(env, op_minus,         "complex", "complex", "complex", complex_minus,      0))
  CHECK_BB(import_op(env, op_times,         "complex", "complex", "complex", complex_times,      0))
  CHECK_BB(import_op(env, op_divide,        "complex", "complex", "complex", complex_divide,     0))
  CHECK_BB(import_op(env, op_chuck,         "complex", "complex", "complex", complex_r_assign,   0))
  CHECK_BB(import_op(env, op_plus_chuck,    "complex", "complex", "complex", complex_r_plus,     0))
  CHECK_BB(import_op(env, op_minus_chuck,   "complex", "complex", "complex", complex_r_minus,    0))
  CHECK_BB(import_op(env, op_times_chuck,   "complex", "complex", "complex", complex_r_times,    0))
  CHECK_BB(import_op(env, op_divide_chuck,  "complex", "complex", "complex", complex_r_divide,   0))
  CHECK_BB(import_op(env, op_assign,        "polar", "polar", "polar", complex_assign,           0))
  CHECK_BB(import_op(env, op_chuck,         "polar", "polar", "polar", complex_r_assign,         0))
  CHECK_BB(import_op(env, op_plus,          "polar", "polar", "polar", polar_plus,       0))
  CHECK_BB(import_op(env, op_minus,         "polar", "polar", "polar", polar_minus,      0))
  CHECK_BB(import_op(env, op_times,         "polar", "polar", "polar", polar_times,      0))
  CHECK_BB(import_op(env, op_divide,        "polar", "polar", "polar", polar_divide,     0))
  CHECK_BB(import_op(env, op_plus_chuck,    "polar", "polar", "polar", polar_plus_r,     0))
  CHECK_BB(import_op(env, op_minus_chuck,   "polar", "polar", "polar", polar_minus_r,    0))
  CHECK_BB(import_op(env, op_times_chuck,   "polar", "polar", "polar", polar_times_r,    0))
  CHECK_BB(import_op(env, op_divide_chuck,  "polar", "polar", "polar", polar_divide_r,   0))
  return 1;
}
INSTR(vec_member) {
  if(instr->m_val) {
    POP_REG(shred, SZ_INT);
    *(m_float**)REG(0) = &*(m_float*)(shred->reg + instr->m_val2 * SZ_FLOAT);
    PUSH_REG(shred, SZ_INT);
  } else {
    POP_REG(shred, SZ_INT);
    *(m_float*)REG(0) = *(m_float*)(shred->reg + instr->m_val2 * SZ_FLOAT);
    PUSH_REG(shred, SZ_FLOAT);
  }
}

static void vec_times(char* v, int size, m_float f) {
  int i;
  for(i = size + 1; --i;)
    *(m_float*)(v + (i-1) * SZ_FLOAT) *= f;
}

static void vec_divide(char* v, int size, m_float f) {
  int i;
  for(i = size + 1; --i;)
    *(m_float*)(v + (i-1) * SZ_FLOAT) /= f;
}

static MFUN(vec3_set) {
  m_vec3* v =  &*(m_vec3*)MEM(0);
  v->x = *(m_float*)MEM(SZ_INT);
  v->y = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  v->z = *(m_float*)MEM(SZ_INT + SZ_FLOAT * 2);
}

static MFUN(vec3_setAll) {
  m_vec3* v =  &**(m_vec3**)MEM(0);
  v->x = *(m_float*)MEM(SZ_INT);
  v->y = *(m_float*)MEM(SZ_INT);
  v->z = *(m_float*)MEM(SZ_INT);
}

static MFUN(vec3_magnitude) {
  m_vec3* v =  &**(m_vec3**)MEM(0);
  *(m_float*)RETURN = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

static MFUN(vec3_normalize) {
  m_vec3* v =  &**(m_vec3**)MEM(0);
  m_float mag = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
  if(mag  > 0)
    vec_divide((char*)v, 3, mag);
}

static MFUN(vec3_interp) {
  m_vec3* v =  &**(m_vec3**)MEM(0);
  v->x = (v->y - v->x) * v->z + v->x;
  *(m_float*)RETURN = v->x;
}

static MFUN(vec3_float) {
  m_vec3* v =  &**(m_vec3**)MEM(0);
  v->x = (v->y - v->x) * v->z * (*(m_float*)MEM(SZ_INT)) + v->x;
  *(m_float*)RETURN = v->x;
}

static MFUN(vec3_dur) {
  m_vec3* v =  &**(m_vec3**)MEM(0);
  v->x = (v->y - v->x) * v->z * (*(m_float*)MEM(SZ_INT) / shred->vm_ref->bbq->sp->sr) + v->x;
  *(m_float*)RETURN = v->x;
}

static MFUN(vec3_update) {
  m_vec3* v =  &**(m_vec3**)MEM(0);
  v->y = *(m_float*)MEM(SZ_INT);
}

static MFUN(vec3_update_slew) {
  m_vec3* v =  &**(m_vec3**)MEM(0);
  v->y = *(m_float*)MEM(SZ_INT);
  v->z = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
}

static MFUN(vec3_update_set) {
  m_vec3* v =  &**(m_vec3**)MEM(0);
  v->x = *(m_float*)MEM(SZ_INT);
  v->y = *(m_float*)MEM(SZ_INT);

}

static MFUN(vec3_update_set_slew) {
  m_vec3* v =  &**(m_vec3**)MEM(0);
  v->x = *(m_float*)MEM(SZ_INT);
  v->y = *(m_float*)MEM(SZ_INT);
  v->z = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
}

static INSTR(vec3_add) {
  m_vec3 r, * t = (m_vec3*)REG(0);
  POP_REG(shred, SZ_VEC3 * 2);
  r.x = t->x + (t + 1)->x;
  r.y = t->y + (t + 1)->y;
  r.z = t->z + (t + 1)->z;
  *(m_vec3*)REG(0) = r;
  PUSH_REG(shred,  SZ_VEC3);
}

static INSTR(vec3_minus) {
  m_vec3 r, * t = (m_vec3*)REG(0);
  POP_REG(shred, SZ_VEC3 * 2);
  r.x = t->x - (t + 1)->x;
  r.y = t->y - (t + 1)->y;
  r.z = t->z - (t + 1)->z;
  *(m_vec3*)REG(0) = r;
  PUSH_REG(shred,  SZ_VEC3);
}

static INSTR(vec3_xproduct) {
  m_vec3 r, * t = (m_vec3*)REG(0);
  POP_REG(shred, SZ_VEC3 * 2);
  r.x = t->x * (t + 1)->x;
  r.y = t->y * (t + 1)->y;
  r.z = t->z * (t + 1)->z;
  *(m_vec3*)REG(0) = r;
  PUSH_REG(shred,  SZ_VEC3);
}

static INSTR(float_times_vec3) {
  POP_REG(shred, SZ_FLOAT + SZ_VEC3);
  m_float f = *(m_float*)REG(0);
  m_vec3 r = *(m_vec3*)REG(SZ_FLOAT);
  vec_times((char*)&r, 3, f);
  *(m_vec3*)REG(0) = r;
  PUSH_REG(shred,  SZ_VEC3);
}

static INSTR(vec3_times_float) {
  POP_REG(shred, SZ_FLOAT + SZ_VEC3);
  m_vec3 r = *(m_vec3*)REG(0);
  m_float f = *(m_float*)REG(SZ_VEC3);
  vec_times((char*)&r, 3, f);
  *(m_vec3*)REG(0) = r;
  PUSH_REG(shred,  SZ_VEC3);
}

static INSTR(vec3_divide_float) {
  POP_REG(shred, SZ_FLOAT + SZ_VEC3);
  m_vec3 r = *(m_vec3*)REG(0);
  m_float f = *(m_float*)REG(SZ_VEC3);
  vec_divide((char*)&r, 3, f);
  *(m_vec3*)REG(0) = r;
  PUSH_REG(shred,  SZ_VEC3);
}

static INSTR(vec3_r_assign) {
  POP_REG(shred, SZ_VEC3 + SZ_INT);
  m_vec3 r = **(m_vec3**)REG(SZ_VEC3);
  r.x = *(m_float*)REG(0);
  r.y = *(m_float*)REG(SZ_FLOAT);
  r.z = *(m_float*)REG(SZ_COMPLEX);
  *(m_vec3*)REG(0) = r;
  PUSH_REG(shred, SZ_VEC3);
}

m_bool import_vec3(Env env) {
  DL_Func fun;

  CHECK_BB(import_class_begin(env, &t_vec3, env->global_nspc, NULL, NULL))
    import_var(env, "float", "x", ae_flag_member, NULL);
  import_var(env, "float", "y", ae_flag_member, NULL);
  import_var(env, "float", "z", ae_flag_member, NULL);
  dl_func_init(&fun, "void", "set", (m_uint)vec3_set);
  dl_func_add_arg(&fun, "float", "x");
  dl_func_add_arg(&fun, "float", "y");
  dl_func_add_arg(&fun, "float", "z");
  CHECK_BB(import_fun(env, &fun, 0))
    dl_func_init(&fun, "void", "setAll", (m_uint)vec3_setAll);
  dl_func_add_arg(&fun, "float", "x");
  CHECK_BB(import_fun(env, &fun, 0))
    dl_func_init(&fun, "float", "magnitude", (m_uint)vec3_magnitude);
  CHECK_BB(import_fun(env, &fun, 0))
    dl_func_init(&fun, "void", "normalize", (m_uint)vec3_normalize);
  CHECK_BB(import_fun(env, &fun, 0))
    dl_func_init(&fun, "float", "interp", (m_uint)vec3_interp);
  CHECK_BB(import_fun(env, &fun, 0))
    dl_func_init(&fun, "float", "interp", (m_uint)vec3_float);
  dl_func_add_arg(&fun, "float", "delta");
  CHECK_BB(import_fun(env, &fun, 0))
    dl_func_init(&fun, "float", "interp", (m_uint)vec3_dur);
  dl_func_add_arg(&fun, "dur", "delta");
  CHECK_BB(import_fun(env, &fun, 0))
    dl_func_init(&fun, "void", "update", (m_uint)vec3_update);
  dl_func_add_arg(&fun, "float", "goal");
  CHECK_BB(import_fun(env, &fun, 0))
    dl_func_init(&fun, "void", "update", (m_uint)vec3_update_slew);
  dl_func_add_arg(&fun, "float", "goal");
  dl_func_add_arg(&fun, "float", "slew");
  CHECK_BB(import_fun(env, &fun, 0))
    dl_func_init(&fun, "void", "supdate", (m_uint)vec3_update_set);
  dl_func_add_arg(&fun, "float", "goalAndValue");
  CHECK_BB(import_fun(env, &fun, 0))
    dl_func_init(&fun, "void", "supdate", (m_uint)vec3_update_set_slew);
  dl_func_add_arg(&fun, "float", "goalAndValue");
  dl_func_add_arg(&fun, "float", "slew");
  CHECK_BB(import_fun(env, &fun, 0))

    CHECK_BB(import_op(env, op_plus,   "Vec3",   "Vec3", "Vec3",  vec3_add, 1))
    CHECK_BB(import_op(env, op_minus,  "Vec3",   "Vec3", "Vec3",  vec3_minus, 1))
    CHECK_BB(import_op(env, op_times,  "Vec3",   "Vec3", "Vec3",  vec3_xproduct, 1))
    CHECK_BB(import_op(env, op_times,  "Vec3",   "float", "Vec3", vec3_times_float, 1))
    CHECK_BB(import_op(env, op_divide, "Vec3",   "float", "Vec3", vec3_divide_float, 1))
    CHECK_BB(import_op(env, op_times,  "float",  "Vec3", "Vec3",  float_times_vec3, 1))
    CHECK_BB(import_op(env, op_chuck,  "Vec3",   "Vec3", "Vec3",  vec3_r_assign, 1))
    CHECK_BB(import_class_end(env))
    return 1;
}

static MFUN(vec4_set) {
  m_vec4* v =  &**(m_vec4**)MEM(0);
  v->x = *(m_float*)MEM(SZ_INT);
  v->y = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  v->z = *(m_float*)MEM(SZ_INT + SZ_FLOAT * 2);
  v->w = *(m_float*)MEM(SZ_INT + SZ_FLOAT * 3);
}

static MFUN(vec4_setAll) {
  m_vec4* v =  &**(m_vec4**)MEM(0);
  v->x = *(m_float*)MEM(SZ_INT);
  v->y = *(m_float*)MEM(SZ_INT);
  v->z = *(m_float*)MEM(SZ_INT);
  v->w = *(m_float*)MEM(SZ_INT);
}

static MFUN(vec4_magnitude) {
  m_vec4* v =  &**(m_vec4**)MEM(0);
  *(m_float*)RETURN = sqrt(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}

static MFUN(vec4_normalize) {
  m_vec4* v =  &**(m_vec4**)MEM(0);
  m_float mag = sqrt(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
  if(mag  > 0)
    vec_divide((char*)v, 4, mag);
}

static INSTR(vec4_add) {
  m_vec4 r, * t = (m_vec4*)REG(0);
  POP_REG(shred, SZ_VEC4 * 2);
  r.x = t->x + (t + 1)->x;
  r.y = t->y + (t + 1)->y;
  r.z = t->z + (t + 1)->z;
  r.w = t->w + (t + 1)->w;
  *(m_vec4*)REG(0) = r;
  PUSH_REG(shred,  SZ_VEC4);
}

static INSTR(vec4_minus) {
  m_vec4 r, * t = (m_vec4*)REG(0);
  POP_REG(shred, SZ_VEC4 * 2);
  r.x = t->x - (t + 1)->x;
  r.y = t->y - (t + 1)->y;
  r.z = t->z - (t + 1)->z;
  r.w = t->w - (t + 1)->w;
  *(m_vec4*)REG(0) = r;
  PUSH_REG(shred,  SZ_VEC4);
}

static INSTR(vec4_xproduct) {
  m_vec4 r, * t = (m_vec4*)REG(0);
  POP_REG(shred, SZ_VEC4 * 2);
  r.x = t->x * (t + 1)->x;
  r.y = t->y * (t + 1)->y;
  r.z = t->z * (t + 1)->z;
  r.w = t->w * (t + 1)->w;
  *(m_vec4*)REG(0) = r;
  PUSH_REG(shred,  SZ_VEC4);
}

static INSTR(float_times_vec4) {
  POP_REG(shred, SZ_FLOAT + SZ_VEC4);
  m_float f = *(m_float*)REG(0);
  m_vec4 r = *(m_vec4*)REG(SZ_FLOAT);
  vec_times((char*)&r, 4, f);
  *(m_vec4*)REG(0) = r;
  PUSH_REG(shred,  SZ_VEC4);
}

static INSTR(vec4_times_float) {
  POP_REG(shred, SZ_FLOAT + SZ_VEC4);
  m_vec4 r = *(m_vec4*)REG(0);
  m_float f = *(m_float*)REG(SZ_VEC4);
  vec_times((char*)&r, 4, f);
  *(m_vec4*)REG(0) = r;
  PUSH_REG(shred,  SZ_VEC4);
}

static INSTR(vec4_divide_float) {
  POP_REG(shred, SZ_FLOAT + SZ_VEC4);
  m_vec4 r = *(m_vec4*)REG(0);
  m_float f = *(m_float*)REG(SZ_VEC4);
  vec_divide((char*)&r, 4, f);
  *(m_vec4*)REG(0) = r;
  PUSH_REG(shred,  SZ_VEC4);
}

static INSTR(vec4_r_assign) {
  POP_REG(shred, SZ_VEC4 + SZ_INT);
  m_vec4 r = **(m_vec4**)REG(SZ_VEC4);
  r.x = *(m_float*)REG(0);
  r.y = *(m_float*)REG(SZ_FLOAT);
  r.z = *(m_float*)REG(SZ_FLOAT * 2);
  r.w = *(m_float*)REG(SZ_FLOAT * 3);
  *(m_vec4*)REG(0) = r;
  PUSH_REG(shred, SZ_VEC4);
}

m_bool import_vec4(Env env) {
  DL_Func fun;
  CHECK_BB(import_class_begin(env, &t_vec4, env->global_nspc, NULL, NULL))
    import_var(env, "float", "x", ae_flag_member, NULL);
  import_var(env, "float", "y", ae_flag_member, NULL);
  import_var(env, "float", "z", ae_flag_member, NULL);
  import_var(env, "float", "w", ae_flag_member, NULL);
  dl_func_init(&fun, "void", "set", (m_uint)vec4_set);
  dl_func_add_arg(&fun, "float", "x");
  dl_func_add_arg(&fun, "float", "y");
  dl_func_add_arg(&fun, "float", "z");
  dl_func_add_arg(&fun, "float", "w");
  CHECK_BB(import_fun(env, &fun, 0))
    dl_func_init(&fun, "void", "setAll", (m_uint)vec4_setAll);
  dl_func_add_arg(&fun, "float", "x");
  CHECK_BB(import_fun(env, &fun, 0))
    dl_func_init(&fun, "float", "magnitude", (m_uint)vec4_magnitude);
  CHECK_BB(import_fun(env, &fun, 0))
    dl_func_init(&fun, "void", "normalize", (m_uint)vec4_normalize);
  CHECK_BB(import_fun(env, &fun, 0))
    CHECK_BB(import_op(env, op_plus,   "Vec4",   "Vec4",  "Vec4",  vec4_add, 1))
    CHECK_BB(import_op(env, op_minus,  "Vec4",   "Vec4",  "Vec4",  vec4_minus, 1))
    CHECK_BB(import_op(env, op_times,  "Vec4",   "Vec4",  "Vec4",  vec4_xproduct, 1))
    CHECK_BB(import_op(env, op_times,  "Vec4",   "float", "Vec4",  vec4_times_float, 1))
    CHECK_BB(import_op(env, op_divide, "Vec4",   "float", "Vec4",  vec4_divide_float, 1))
    CHECK_BB(import_op(env, op_times,  "float",  "Vec4",  "Vec4",  float_times_vec4, 1))
    CHECK_BB(import_op(env, op_chuck,  "Vec4",   "Vec4",  "Vec4",  vec4_r_assign, 1))
    CHECK_BB(import_class_end(env))
    return 1;
}
