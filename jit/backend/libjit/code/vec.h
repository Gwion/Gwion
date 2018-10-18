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
JIT_CODE(Vec3##name) {\
  push_reg(cc, -SZ_VEC3);\
  CJval ax = JLOADR(cc->reg, -SZ_VEC3,    float);\
  CJval ay = JLOADR(cc->reg, -SZ_COMPLEX, float);\
  CJval az = JLOADR(cc->reg, -SZ_FLOAT,   float);\
  CJval bx = JLOADR(cc->reg, 0,           float);\
  CJval by = JLOADR(cc->reg, SZ_FLOAT,    float);\
  CJval bz = JLOADR(cc->reg, SZ_COMPLEX,  float);\
  CJval x  = JINSN(op, ax, bx);\
  CJval y  = JINSN(op, ay, by);\
  CJval z  = JINSN(op, az, bz);\
  JSTORER(cc->reg, -SZ_VEC3, x);\
  JSTORER(cc->reg, -SZ_COMPLEX, y);\
  JSTORER(cc->reg, -SZ_FLOAT, z);\
}
jit_describe_vec3(Add, add)
jit_describe_vec3(Sub, sub)
jit_describe_vec3(Mul, mul)
jit_describe_vec3(Div, div)

#define jit_describe_float_vec3(name, func)\
JIT_CODE(Float##name##Vec3) {\
  push_reg(cc, -SZ_FLOAT);\
  CJval  f = JLOADR(cc->reg, -SZ_VEC3,    float);\
  CJval ax = JLOADR(cc->reg, -SZ_COMPLEX, float);\
  CJval ay = JLOADR(cc->reg, -SZ_FLOAT,   float);\
  CJval az = JLOADR(cc->reg, 0,           float);\
  CJval x  = JINSN(func, ax, f);\
  CJval y  = JINSN(func, ay, f);\
  CJval z  = JINSN(func, az, f);\
  JSTORER(cc->reg, -SZ_VEC3, x);\
  JSTORER(cc->reg, -SZ_COMPLEX, y);\
  JSTORER(cc->reg, -SZ_FLOAT, z);\
}
jit_describe_float_vec3(Add, add)
jit_describe_float_vec3(Sub, sub)
jit_describe_float_vec3(Mul, mul)
jit_describe_float_vec3(Div, div)

#define jit_describe_vec3_float(name, func)\
JIT_CODE(Vec3##name##Float) {\
  push_reg(cc, -SZ_FLOAT);\
  CJval ax = JLOADR(cc->reg, -SZ_VEC3,    float);\
  CJval ay = JLOADR(cc->reg, -SZ_COMPLEX, float);\
  CJval az = JLOADR(cc->reg, -SZ_FLOAT,   float);\
  CJval f  = JLOADR(cc->reg, 0,           float);\
  CJval x  = JINSN(func, ax, f);\
  CJval y  = JINSN(func, ay, f);\
  CJval z  = JINSN(func, az, f);\
  JSTORER(cc->reg, -SZ_VEC3, x);\
  JSTORER(cc->reg, -SZ_COMPLEX, y);\
  JSTORER(cc->reg, -SZ_FLOAT, z);\
}
jit_describe_vec3_float(Add, add)
jit_describe_vec3_float(Sub, sub)
jit_describe_vec3_float(Mul, mul)
jit_describe_vec3_float(Div, div)

#define jit_describe_vec3_assign(name, offset1, offset2)\
JIT_CODE(Vec3##name##Assign) {\
  push_reg(cc, -SZ_INT);\
  CJval src = JLOADR(cc->reg,  offset1, void_ptr);\
  CJval x = JLOADR(cc->reg, offset2-SZ_VEC3,    float);\
  CJval y = JLOADR(cc->reg, offset2-SZ_COMPLEX, float);\
  CJval z = JLOADR(cc->reg, offset2-SZ_FLOAT,   float);\
  JSTORER(src, 0, x);\
  JSTORER(src, SZ_FLOAT, y);\
  JSTORER(src, SZ_COMPLEX, z);\
  JSTORER(cc->reg, -SZ_VEC3, x);\
  JSTORER(cc->reg, -SZ_COMPLEX, y);\
  JSTORER(cc->reg, -SZ_FLOAT, z);\
}
jit_describe_vec3_assign(, -SZ_VEC3, SZ_INT)
jit_describe_vec3_assign(R, 0, )

#define jit_describe_vec4(name, op)\
JIT_CODE(Vec4##name) {\
  push_reg(cc, -SZ_VEC4);\
  CJval ax = JLOADR(cc->reg, -SZ_VEC4,    float);\
  CJval ay = JLOADR(cc->reg, -SZ_VEC3,    float);\
  CJval az = JLOADR(cc->reg, -SZ_COMPLEX, float);\
  CJval aw = JLOADR(cc->reg, -SZ_FLOAT,   float);\
  CJval bx = JLOADR(cc->reg, 0,           float);\
  CJval by = JLOADR(cc->reg, SZ_FLOAT,    float);\
  CJval bz = JLOADR(cc->reg, SZ_COMPLEX,  float);\
  CJval bw = JLOADR(cc->reg, SZ_VEC3,     float);\
  CJval x  = JINSN(op, ax, bx);\
  CJval y  = JINSN(op, ay, by);\
  CJval z  = JINSN(op, az, bz);\
  CJval w  = JINSN(op, aw, bw);\
  JSTORER(cc->reg, -SZ_VEC4, x);\
  JSTORER(cc->reg, -SZ_VEC3, y);\
  JSTORER(cc->reg, -SZ_COMPLEX, z);\
  JSTORER(cc->reg, -SZ_FLOAT, w);\
}
jit_describe_vec4(Add, add)
jit_describe_vec4(Sub, sub)
jit_describe_vec4(Mul, mul)
jit_describe_vec4(Div, div)

#define jit_describe_float_vec4(name, func)\
JIT_CODE(Float##name##Vec4) {\
  push_reg(cc, -SZ_FLOAT);\
  CJval  f = JLOADR(cc->reg, -SZ_VEC4,    float);\
  CJval ax = JLOADR(cc->reg, -SZ_VEC3,    float);\
  CJval ay = JLOADR(cc->reg, -SZ_COMPLEX, float);\
  CJval az = JLOADR(cc->reg, -SZ_FLOAT,   float);\
  CJval aw = JLOADR(cc->reg, 0,           float);\
  CJval x  = JINSN(func, ax, f);\
  CJval y  = JINSN(func, ay, f);\
  CJval z  = JINSN(func, az, f);\
  CJval w  = JINSN(func, aw, f);\
  JSTORER(cc->reg, -SZ_VEC3, x);\
  JSTORER(cc->reg, -SZ_COMPLEX, y);\
  JSTORER(cc->reg, -SZ_FLOAT, z);\
  JSTORER(cc->reg, -SZ_FLOAT, w);\
}
jit_describe_float_vec4(Add, add)
jit_describe_float_vec4(Sub, add)
jit_describe_float_vec4(Mul, add)
jit_describe_float_vec4(Div, add)

#define jit_describe_vec4_float(name, func)\
JIT_CODE(Vec4##name##Float) {\
  push_reg(cc, -SZ_FLOAT);\
  CJval ax = JLOADR(cc->reg, -SZ_VEC4,    float);\
  CJval ay = JLOADR(cc->reg, -SZ_VEC3,    float);\
  CJval az = JLOADR(cc->reg, -SZ_COMPLEX, float);\
  CJval aw = JLOADR(cc->reg, -SZ_FLOAT,   float);\
  CJval f  = JLOADR(cc->reg, 0,           float);\
  CJval x  = JINSN(func, ax, f);\
  CJval y  = JINSN(func, ay, f);\
  CJval z  = JINSN(func, az, f);\
  CJval w  = JINSN(func, aw, f);\
  JSTORER(cc->reg, -SZ_VEC4, x);\
  JSTORER(cc->reg, -SZ_VEC3, y);\
  JSTORER(cc->reg, -SZ_COMPLEX, z);\
  JSTORER(cc->reg, -SZ_FLOAT, w);\
}
jit_describe_vec4_float(Add, add)
jit_describe_vec4_float(Sub, sub)
jit_describe_vec4_float(Mul, mul)
jit_describe_vec4_float(Div, div)

#define jit_describe_vec4_assign(name, offset1, offset2)\
JIT_CODE(Vec4##name##Assign) {\
  push_reg(cc, -SZ_INT);\
  CJval src = JLOADR(cc->reg,  offset1, void_ptr);\
  CJval x = JLOADR(cc->reg, offset2-SZ_VEC4,    float);\
  CJval y = JLOADR(cc->reg, offset2-SZ_VEC3,    float);\
  CJval z = JLOADR(cc->reg, offset2-SZ_COMPLEX, float);\
  CJval w = JLOADR(cc->reg, offset2-SZ_FLOAT,   float);\
  JSTORER(src, 0, x);\
  JSTORER(src, SZ_FLOAT, y);\
  JSTORER(src, SZ_COMPLEX, z);\
  JSTORER(src, SZ_COMPLEX, w);\
  JSTORER(cc->reg, -SZ_VEC4, x);\
  JSTORER(cc->reg, -SZ_VEC3, y);\
  JSTORER(cc->reg, -SZ_COMPLEX, z);\
  JSTORER(cc->reg, -SZ_FLOAT, w);\
}
jit_describe_vec4_assign(, -SZ_VEC4, SZ_INT)
jit_describe_vec4_assign(R, 0, )

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_vec(struct Jit* j) {
  JIT_IMPORT(VecMember)
  JIT_IMPORT(Vec3Assign)
  JIT_IMPORT(Vec3Add)
  JIT_IMPORT(Vec3Sub)
  JIT_IMPORT(Vec3Mul)
  JIT_IMPORT(Vec3Div)
  JIT_IMPORT(FloatAddVec3)
  JIT_IMPORT(FloatSubVec3)
  JIT_IMPORT(FloatMulVec3)
  JIT_IMPORT(FloatDivVec3)
  JIT_IMPORT(Vec3AddFloat)
  JIT_IMPORT(Vec3SubFloat)
  JIT_IMPORT(Vec3MulFloat)
  JIT_IMPORT(Vec3DivFloat)
  JIT_IMPORT(Vec3RAssign)
  JIT_IMPORT(Vec4Assign)
  JIT_IMPORT(Vec4Add)
  JIT_IMPORT(Vec4Sub)
  JIT_IMPORT(Vec4Mul)
  JIT_IMPORT(Vec4Div)
  JIT_IMPORT(FloatAddVec4)
  JIT_IMPORT(FloatSubVec4)
  JIT_IMPORT(FloatMulVec4)
  JIT_IMPORT(FloatDivVec4)
  JIT_IMPORT(Vec4AddFloat)
  JIT_IMPORT(Vec4SubFloat)
  JIT_IMPORT(Vec4MulFloat)
  JIT_IMPORT(Vec4DivFloat)
  JIT_IMPORT(Vec4RAssign)
}
