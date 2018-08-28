#include "jit/jit.h"
#include "gwjit_common.h"
#include "jitter.h"
#include "cc.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"

JIT_CODE(VecMember) {
  CJval src = JLOADR(cc->reg, -SZ_INT, void_ptr);
  if(instr->m_val) {
    CJval val = JADDR(src, instr->m_val2);
    JSTORER(cc->reg, -SZ_INT, val);
  } else {
    CJval val = JLOADR(src, instr->m_val2, float);
    JSTORER(cc->reg, -SZ_INT, val);
    push_reg(cc, SZ_FLOAT-SZ_INT);
  }
}

#define jit_describe_vec3(name, op)\
JIT_CODE(vec3_##name) {\
  CJval reg = push_reg(cc, -SZ_VEC3);\
  CJval ax = JLOADR(reg, -SZ_VEC3,    float);\
  CJval ay = JLOADR(reg, -SZ_COMPLEX, float);\
  CJval az = JLOADR(reg, -SZ_FLOAT,   float);\
  CJval bx = JLOADR(reg, 0,           float);\
  CJval by = JLOADR(reg, SZ_FLOAT,    float);\
  CJval bz = JLOADR(reg, SZ_COMPLEX,  float);\
  CJval x  = JINSN(op, ax, bx);\
  CJval y  = JINSN(op, ay, by);\
  CJval z  = JINSN(op, az, bz);\
  JSTORER(reg, -SZ_VEC3, x);\
  JSTORER(reg, -SZ_COMPLEX, y);\
  JSTORER(reg, -SZ_FLOAT, z);\
}
jit_describe_vec3(add, add)
jit_describe_vec3(sub, sub)
jit_describe_vec3(mul, mul)
jit_describe_vec3(div, div)

#define jit_describe_float_vec3(func)\
JIT_CODE(float_##func##_vec3) {\
  CJval reg = push_reg(cc, -SZ_FLOAT);\
  CJval  f = JLOADR(reg, -SZ_VEC3,    float);\
  CJval ax = JLOADR(reg, -SZ_COMPLEX, float);\
  CJval ay = JLOADR(reg, -SZ_FLOAT,   float);\
  CJval az = JLOADR(reg, 0,           float);\
  CJval x  = JINSN(func, ax, f);\
  CJval y  = JINSN(func, ay, f);\
  CJval z  = JINSN(func, az, f);\
  JSTORER(reg, -SZ_VEC3, x);\
  JSTORER(reg, -SZ_COMPLEX, y);\
  JSTORER(reg, -SZ_FLOAT, z);\
}
jit_describe_float_vec3(add)
jit_describe_float_vec3(sub)
jit_describe_float_vec3(mul)
jit_describe_float_vec3(div)

#define jit_describe_vec3_float(func)\
JIT_CODE(vec3_##func##_float) {\
  CJval reg = push_reg(cc, -SZ_FLOAT);\
  CJval ax = JLOADR(reg, -SZ_VEC3,    float);\
  CJval ay = JLOADR(reg, -SZ_COMPLEX, float);\
  CJval az = JLOADR(reg, -SZ_FLOAT,   float);\
  CJval f  = JLOADR(reg, 0,           float);\
  CJval x  = JINSN(func, ax, f);\
  CJval y  = JINSN(func, ay, f);\
  CJval z  = JINSN(func, az, f);\
  JSTORER(reg, -SZ_VEC3, x);\
  JSTORER(reg, -SZ_COMPLEX, y);\
  JSTORER(reg, -SZ_FLOAT, z);\
}
jit_describe_vec3_float(add)
jit_describe_vec3_float(sub)
jit_describe_vec3_float(mul)
jit_describe_vec3_float(div)

#define jit_describe_vec3_assign(name, offset1, offset2)\
JIT_CODE(vec3_##name##assign) {\
  CJval reg = push_reg(cc, -SZ_INT);\
  CJval src = JLOADR(reg,  offset1, void_ptr);\
  CJval x = JLOADR(reg, offset2-SZ_VEC3,    float);\
  CJval y = JLOADR(reg, offset2-SZ_COMPLEX, float);\
  CJval z = JLOADR(reg, offset2-SZ_FLOAT,   float);\
  JSTORER(src, 0, x);\
  JSTORER(src, SZ_FLOAT, y);\
  JSTORER(src, SZ_COMPLEX, z);\
  JSTORER(reg, -SZ_VEC3, x);\
  JSTORER(reg, -SZ_COMPLEX, y);\
  JSTORER(reg, -SZ_FLOAT, z);\
}
jit_describe_vec3_assign(, -SZ_VEC3, SZ_INT)
jit_describe_vec3_assign(r_, 0, )

#define jit_describe_vec4(name, op)\
JIT_CODE(vec4_##name) {\
  CJval reg = push_reg(cc, -SZ_VEC4);\
  CJval ax = JLOADR(reg, -SZ_VEC4,    float);\
  CJval ay = JLOADR(reg, -SZ_VEC3,    float);\
  CJval az = JLOADR(reg, -SZ_COMPLEX, float);\
  CJval aw = JLOADR(reg, -SZ_FLOAT,   float);\
  CJval bx = JLOADR(reg, 0,           float);\
  CJval by = JLOADR(reg, SZ_FLOAT,    float);\
  CJval bz = JLOADR(reg, SZ_COMPLEX,  float);\
  CJval bw = JLOADR(reg, SZ_VEC3,     float);\
  CJval x  = JINSN(op, ax, bx);\
  CJval y  = JINSN(op, ay, by);\
  CJval z  = JINSN(op, az, bz);\
  CJval w  = JINSN(op, aw, bw);\
  JSTORER(reg, -SZ_VEC4, x);\
  JSTORER(reg, -SZ_VEC3, y);\
  JSTORER(reg, -SZ_COMPLEX, z);\
  JSTORER(reg, -SZ_FLOAT, w);\
}
jit_describe_vec4(add, add)
jit_describe_vec4(sub, sub)
jit_describe_vec4(mul, mul)
jit_describe_vec4(div, div)

#define jit_describe_float_vec4(func)\
JIT_CODE(float_##func##_vec4) {\
  CJval reg = push_reg(cc, -SZ_FLOAT);\
  CJval  f = JLOADR(reg, -SZ_VEC4,    float);\
  CJval ax = JLOADR(reg, -SZ_VEC3,    float);\
  CJval ay = JLOADR(reg, -SZ_COMPLEX, float);\
  CJval az = JLOADR(reg, -SZ_FLOAT,   float);\
  CJval aw = JLOADR(reg, 0,           float);\
  CJval x  = JINSN(func, ax, f);\
  CJval y  = JINSN(func, ay, f);\
  CJval z  = JINSN(func, az, f);\
  CJval w  = JINSN(func, aw, f);\
  JSTORER(reg, -SZ_VEC3, x);\
  JSTORER(reg, -SZ_COMPLEX, y);\
  JSTORER(reg, -SZ_FLOAT, z);\
  JSTORER(reg, -SZ_FLOAT, w);\
}
jit_describe_float_vec4(add)
jit_describe_float_vec4(sub)
jit_describe_float_vec4(mul)
jit_describe_float_vec4(div)

#define jit_describe_vec4_float(func)\
JIT_CODE(vec4_##func##_float) {\
  CJval reg = push_reg(cc, -SZ_FLOAT);\
  CJval ax = JLOADR(reg, -SZ_VEC4,    float);\
  CJval ay = JLOADR(reg, -SZ_VEC3,    float);\
  CJval az = JLOADR(reg, -SZ_COMPLEX, float);\
  CJval aw = JLOADR(reg, -SZ_FLOAT,   float);\
  CJval f  = JLOADR(reg, 0,           float);\
  CJval x  = JINSN(func, ax, f);\
  CJval y  = JINSN(func, ay, f);\
  CJval z  = JINSN(func, az, f);\
  CJval w  = JINSN(func, aw, f);\
  JSTORER(reg, -SZ_VEC4, x);\
  JSTORER(reg, -SZ_VEC3, y);\
  JSTORER(reg, -SZ_COMPLEX, z);\
  JSTORER(reg, -SZ_FLOAT, w);\
}
jit_describe_vec4_float(add)
jit_describe_vec4_float(sub)
jit_describe_vec4_float(mul)
jit_describe_vec4_float(div)

#define jit_describe_vec4_assign(name, offset1, offset2)\
JIT_CODE(vec4_##name##assign) {\
  CJval reg = push_reg(cc, -SZ_INT);\
  CJval src = JLOADR(reg,  offset1, void_ptr);\
  CJval x = JLOADR(reg, offset2-SZ_VEC4,    float);\
  CJval y = JLOADR(reg, offset2-SZ_VEC3,    float);\
  CJval z = JLOADR(reg, offset2-SZ_COMPLEX, float);\
  CJval w = JLOADR(reg, offset2-SZ_FLOAT,   float);\
  JSTORER(src, 0, x);\
  JSTORER(src, SZ_FLOAT, y);\
  JSTORER(src, SZ_COMPLEX, z);\
  JSTORER(src, SZ_COMPLEX, w);\
  JSTORER(reg, -SZ_VEC4, x);\
  JSTORER(reg, -SZ_VEC3, y);\
  JSTORER(reg, -SZ_COMPLEX, z);\
  JSTORER(reg, -SZ_FLOAT, w);\
}
jit_describe_vec4_assign(, -SZ_VEC4, SZ_INT)
jit_describe_vec4_assign(r_, 0, )

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_vec(struct Jit* j) {
  JIT_IMPORT(VecMember)
  JIT_IMPORT(vec3_assign)
  JIT_IMPORT(vec3_add)
  JIT_IMPORT(vec3_sub)
  JIT_IMPORT(vec3_mul)
  JIT_IMPORT(vec3_div)
  JIT_IMPORT(float_add_vec3)
  JIT_IMPORT(float_sub_vec3)
  JIT_IMPORT(float_mul_vec3)
  JIT_IMPORT(float_div_vec3)
  JIT_IMPORT(vec3_add_float)
  JIT_IMPORT(vec3_sub_float)
  JIT_IMPORT(vec3_mul_float)
  JIT_IMPORT(vec3_div_float)
  JIT_IMPORT(vec3_r_assign)
  JIT_IMPORT(vec4_assign)
  JIT_IMPORT(vec4_add)
  JIT_IMPORT(vec4_sub)
  JIT_IMPORT(vec4_mul)
  JIT_IMPORT(vec4_div)
  JIT_IMPORT(float_add_vec4)
  JIT_IMPORT(float_sub_vec4)
  JIT_IMPORT(float_mul_vec4)
  JIT_IMPORT(float_div_vec4)
  JIT_IMPORT(vec4_add_float)
  JIT_IMPORT(vec4_sub_float)
  JIT_IMPORT(vec4_mul_float)
  JIT_IMPORT(vec4_div_float)
  JIT_IMPORT(vec4_r_assign)
}
