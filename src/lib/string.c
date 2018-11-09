#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <math.h>
#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "err_msg.h"
#include "oo.h"
#include "env.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "import.h"

ANN static void push_string(const VM_Shred shred, const M_Object obj, const m_str c) {
  STRING(obj) = s_name(insert_symbol(c));
  *(M_Object*)REG(-SZ_INT) = (M_Object)obj;
  _release(obj, shred);
}

ANN static void push_new_string(const VM_Shred shred, const m_str c) {
  const M_Object obj = new_string(shred, c);
  *(M_Object*)REG(-SZ_INT) = (M_Object)obj;
}

#define describe_string_logical(name, action)    \
static INSTR(String_##name) { GWDEBUG_EXE        \
  POP_REG(shred, SZ_INT);                        \
  const M_Object lhs = *(M_Object*)REG(-SZ_INT); \
  const M_Object rhs = *(M_Object*)REG(0);       \
  *(m_int*)REG(-SZ_INT) = action;                \
  release(lhs, shred);                           \
  release(rhs, shred);                           \
}
describe_string_logical(eq, (lhs && rhs && !strcmp(STRING(lhs), STRING(rhs))) || (!lhs && !rhs))
describe_string_logical(neq, (lhs && rhs && strcmp(STRING(lhs), STRING(rhs))) ||
    (lhs && !rhs) || (!lhs && rhs))

#define describe_string_assign(name, type, offset, opt, len, format, ...) \
static INSTR(name##String_Assign) { GWDEBUG_EXE                      \
  POP_REG(shred, offset);                                            \
  const type lhs = *(type*)REG(-SZ_INT);                             \
  const M_Object rhs = *(M_Object*)REG(offset - SZ_INT);             \
  opt;                                                               \
  if(!rhs)                                                           \
    Except(shred, "NullStringException.");                           \
  char str[(len)];                                                   \
  sprintf(str, format, __VA_ARGS__);                                 \
  push_string(shred, rhs, str);                                     \
}

static INSTR(String_Assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  const M_Object lhs = *(M_Object*)REG(-SZ_INT);
  const M_Object rhs = *(M_Object*)REG(0);
  release(lhs, shred);
  if(!rhs)
    Except(shred, "NullStringException.");
  push_string(shred, rhs, lhs ? STRING(lhs) : "");
}

describe_string_assign(Int_, m_int, SZ_INT,,
  num_digit(labs(lhs)) + 1,
  "%" INT_F "", lhs)
describe_string_assign(Float_, m_float, SZ_FLOAT,,
  num_digit((m_uint)lhs) + 5,
  "%.4f", lhs)
describe_string_assign(Complex_, m_complex, SZ_COMPLEX,,
  num_digit(creal(lhs)) + num_digit(cimag(lhs)) + 16,
  "#(%.4f, %.4f)", creal(lhs), cimag(lhs))
describe_string_assign(Polar_, m_complex, SZ_COMPLEX,,
  num_digit(creal(lhs)) + num_digit(cimag(lhs)) + 16,
  "#(%.4f, %.4f)", creal(lhs), cimag(lhs)/M_PI)
describe_string_assign(Vec3_, m_vec3, SZ_VEC3,,
  num_digit(lhs.x) + num_digit(lhs.y) + num_digit(lhs.z) + 23,
  "#(%.4f, %.4f, %.4f)", lhs.x, lhs.y, lhs.z)
describe_string_assign(Vec4_, m_vec4, SZ_VEC4,,
  num_digit(lhs.x) + num_digit(lhs.y) + num_digit(lhs.z) + num_digit(lhs.w) + 30,
  "#(%.4f, %.4f, %.4f, %.4f)", lhs.x, lhs.y, lhs.z, lhs.w)
describe_string_assign(Object_, M_Object, SZ_INT, release(lhs, shred),
  16,
  "%p", (void*)lhs)

static INSTR(String_String) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  const M_Object lhs = *(M_Object*)REG(-SZ_INT);
  const M_Object rhs = *(M_Object*)REG(0);
  char str[(lhs ? strlen(STRING(lhs)) : 0) + (rhs ? strlen(STRING(rhs)) : 0) + 1];
  sprintf(str, "%s%s", lhs ? STRING(lhs) : "", rhs ? STRING(rhs) : "");
  push_new_string(shred, str);
  release(rhs, shred);
  release(lhs, shred);
}

#define describe_string(name, type, offset, len, opt, format, ...) \
static INSTR(name##_String) { GWDEBUG_EXE \
  POP_REG(shred, offset); \
  const type lhs = *(type*)REG(-SZ_INT);\
  const M_Object rhs = *(M_Object*)REG(offset-SZ_INT);\
  char str[(len)];\
  sprintf(str, format, __VA_ARGS__);\
  push_new_string(shred, str);\
  opt; \
  release(rhs, shred);\
}
describe_string(Int, m_int, SZ_INT,
  num_digit(lhs) + (rhs ? strlen(STRING(rhs)) : 0) + 1,,
  "%" INT_F "%s", lhs, rhs ? STRING(rhs) : "")
describe_string(Float, m_float, SZ_FLOAT,
  (num_digit((m_uint)lhs) + 5 + (rhs ? strlen(STRING(rhs)) : 0) + 1),,
  "%.4f%s", lhs, rhs ? STRING(rhs) : "")
describe_string(Complex, m_complex, SZ_COMPLEX,
  num_digit(creal(lhs)) + num_digit(cimag(lhs)) + (rhs ? strlen(STRING(rhs)) : 0) +  12,,
  "#(%.4f, %.4f)%s", creal(lhs), cimag(lhs), rhs ? STRING(rhs) : "")
describe_string(Polar, m_complex, SZ_COMPLEX,
  num_digit(creal(lhs)) + num_digit(cimag(lhs)) + (rhs ? strlen(STRING(rhs)) : 0) +  12,,
  "%%(%f, %f*pi)%s", creal(lhs), cimag(lhs) / M_PI, rhs ? STRING(rhs) : "")
describe_string(Vec3, m_vec3, SZ_VEC3,
  (rhs ? strlen(STRING(rhs)) : 0) + 23 + num_digit((m_uint)lhs.x) +
                                      num_digit((m_uint)lhs.y) + num_digit((m_uint)lhs.z),,
  "@(%.4f, %.4f, %.4f)%s", lhs.x, lhs.y, lhs.z, rhs ? STRING(rhs) : "")
describe_string(Vec4, m_vec4, SZ_VEC4,
  (rhs ? strlen(STRING(rhs)) : 0) + 28 + num_digit((m_uint)lhs.x) +
  num_digit((m_uint)lhs.y) + num_digit((m_uint)lhs.z) + num_digit((m_uint)lhs.w),,
  "@(%f, %f, %f, %f)%s", lhs.x, lhs.y, lhs.z, lhs.w, rhs ? STRING(rhs) : "")
describe_string(Object, M_Object, SZ_INT,
  11 + (rhs ? strlen(STRING(rhs)) : 0), release(lhs, shred),
  "%p%s", (void*)lhs, rhs ? STRING(rhs) : "")


#define describe_string_plus(name, offset, type, opt, len, format, ...) \
static INSTR(name##String_Plus) { GWDEBUG_EXE       \
  POP_REG(shred, offset);                  \
  const type lhs = *(type*)REG(-SZ_INT);                  \
  const M_Object rhs = *(M_Object*)REG(offset - SZ_INT);     \
  opt;                                              \
  if(!rhs)                                          \
    Except(shred, "NullPtrException");              \
  char c[strlen(STRING(rhs)) + (len) + 1];          \
  sprintf(c, "%s"format, STRING(rhs), __VA_ARGS__); \
  push_string(shred, rhs, c);                       \
}
describe_string_plus(,SZ_INT, M_Object, release(lhs, shred),
  strlen(STRING(rhs)), "%s", lhs ? STRING(lhs) : "")
describe_string_plus(Int_, SZ_INT, m_int,,
  num_digit(lhs), "%"INT_F, lhs)
describe_string_plus(Float_, SZ_FLOAT, m_float,,
  num_digit(lhs) + 6, "%s%.4f", STRING(rhs), lhs)
describe_string_plus(Complex_, SZ_COMPLEX, m_complex,,
  num_digit(creal(lhs)) + num_digit(cimag(lhs)) + 18, "#(%.4f, %.4f)",
  creal(lhs), cimag(lhs))
describe_string_plus(Polar_, SZ_COMPLEX, m_complex,,
  num_digit(creal(lhs)) + num_digit(cimag(lhs)) + 18, "%%(%.4f, %.4f)",
  creal(lhs), cimag(lhs) / M_PI)
describe_string_plus(Vec3_, SZ_VEC3, m_vec3,,
  num_digit(lhs.x) + num_digit(lhs.y) + num_digit(lhs.z) + 20, "@(%.4f, %.4f, %.4f)",
  lhs.x, lhs.y, lhs.z)
describe_string_plus(Vec4_, SZ_VEC4, m_vec4,,
  num_digit(lhs.x) + num_digit(lhs.y) + num_digit(lhs.z) + num_digit(lhs.z) + 28, "@(%.4f, %.4f, %.4f, %.4f)",
  lhs.x, lhs.y, lhs.z, lhs.w)
describe_string_plus(Object_, SZ_INT, M_Object, release(lhs, shred),
  11, "%p", (void*)lhs)

INSTR(RegPushStr) { GWDEBUG_EXE
  *(M_Object*)REG(0) = new_string2(shred, (m_str)instr->m_val);
  PUSH_REG(shred, SZ_INT);
}

static CTOR(string_ctor) {
  STRING(o) = "";
}

GWION_IMPORT(string) {
  t_string = gwi_mk_type(gwi, "string", SZ_INT, t_object);
  CHECK_BB(gwi_class_ini(gwi,  t_string, string_ctor, NULL))

  gwi_item_ini(gwi, "int", "@data");
  CHECK_BB(gwi_item_end(gwi,   ae_flag_const, NULL))
  CHECK_BB(gwi_class_end(gwi))

  CHECK_BB(gwi_oper_ini(gwi, "string",  "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_add,       String_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_radd, String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "string",  "string", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_eq,       String_eq))
  CHECK_BB(gwi_oper_end(gwi, op_ne,       String_neq))

  CHECK_BB(gwi_oper_ini(gwi, "int",     "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Int_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_add,       Int_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_radd, Int_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "float",   "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Float_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_add,       Float_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_radd, Float_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "complex", "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Complex_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_add,       Complex_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_radd, Complex_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "polar",   "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Polar_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_add,       Polar_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_radd, Polar_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi,"Vec3",     "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Vec3_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_add,       Vec3_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_radd, Vec3_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "Vec4",    "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Vec4_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_add,       Vec4_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_radd, Vec4_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "Object",  "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Object_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_add,       Object_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_radd, Object_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "@null",   "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Object_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_add,       Object_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_radd, Object_String_Plus))
  return 1;
}
