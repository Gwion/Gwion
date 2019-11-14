#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <math.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "emit.h"
#include "specialid.h"
#include "gwi.h"

ANN static void push_string(const VM_Shred shred, const M_Object obj, const m_str c) {
  STRING(obj) = s_name(insert_symbol(shred->info->vm->gwion->st, c));
  *(M_Object*)REG(-SZ_INT) = (M_Object)obj;
  _release(obj, shred);
}

ANN static void push_new_string(const VM_Shred shred, const m_str c) {
  const M_Object obj = new_string(shred->info->mp, shred, c);
  *(M_Object*)REG(-SZ_INT) = (M_Object)obj;
}

#define describe_string_logical(name, action)    \
static INSTR(String_##name) {\
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
static INSTR(name##String_Assign) {\
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

static INSTR(String_Assign) {
  POP_REG(shred, SZ_INT);
  const M_Object lhs = *(M_Object*)REG(-SZ_INT);
  const M_Object rhs = *(M_Object*)REG(0);
  release(lhs, shred);
  if(!rhs)
    Except(shred, "NullStringException.");
  push_string(shred, rhs, lhs ? STRING(lhs) : "");
}

describe_string_assign(Int_, m_int, SZ_INT,,
  num_digit((m_uint)labs(lhs)) + 1,
  "%" INT_F "", lhs)
describe_string_assign(Float_, m_float, SZ_FLOAT,,
  num_digit((m_uint)lhs) + 5,
  "%.4f", lhs)
describe_string_assign(Complex_, m_complex, SZ_COMPLEX,,
  num_digit((m_uint)creal(lhs)) + num_digit((m_uint)cimag(lhs)) + 16,
  "#(%.4f, %.4f)", creal(lhs), cimag(lhs))
describe_string_assign(Polar_, m_complex, SZ_COMPLEX,,
  num_digit((m_uint)creal(lhs)) + num_digit((m_uint)cimag(lhs) / M_PI) + 16,
  "#(%.4f, %.4f)", creal(lhs), cimag(lhs)/M_PI)
describe_string_assign(Vec3_, m_vec3, SZ_VEC3,,
  num_digit((m_uint)lhs.x) + num_digit((m_uint)lhs.y) + num_digit((m_uint)lhs.z) + 25,
  "#(%.4f, %.4f, %.4f)", lhs.x, lhs.y, lhs.z)
describe_string_assign(Vec4_, m_vec4, SZ_VEC4,,
  num_digit((m_uint)lhs.x) + num_digit((m_uint)lhs.y) + num_digit((m_uint)lhs.z) + num_digit((m_uint)lhs.w) + 40,
  "#(%.4f, %.4f, %.4f, %.4f)", lhs.x, lhs.y, lhs.z, lhs.w)
describe_string_assign(Object_, M_Object, SZ_INT, release(lhs, shred),
  16,
  "%p", (void*)lhs)

static INSTR(String_String) {
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
static INSTR(name##_String) {\
  POP_REG(shred, offset); \
  const type lhs = *(type*)REG(-SZ_INT);\
  const M_Object rhs = *(M_Object*)REG(offset-SZ_INT);\
  if(!rhs)                                          \
    Except(shred, "NullPtrException");              \
  char str[(len)];\
  sprintf(str, format, __VA_ARGS__);\
  push_new_string(shred, str);\
  opt; \
  release(rhs, shred);\
}
describe_string(Int, m_int, SZ_INT,
  num_digit((m_uint)lhs) + (rhs ? strlen(STRING(rhs)) : 0) + 1,,
  "%" INT_F "%s", lhs, rhs ? STRING(rhs) : "")
describe_string(Float, m_float, SZ_FLOAT,
  (num_digit((m_uint)lhs) + 5 + (rhs ? strlen(STRING(rhs)) : 0) + 1),,
  "%.4f%s", lhs, rhs ? STRING(rhs) : "")
describe_string(Complex, m_complex, SZ_COMPLEX,
  num_digit((m_uint)creal(lhs)) + num_digit((m_uint)cimag(lhs)) + (rhs ? strlen(STRING(rhs)) : 0) +  12,,
  "#(%.4f, %.4f)%s", creal(lhs), cimag(lhs), rhs ? STRING(rhs) : "")
describe_string(Polar, m_complex, SZ_COMPLEX,
  num_digit((m_uint)creal(lhs)) + num_digit((m_uint)cimag(lhs) / M_PI) + (rhs ? strlen(STRING(rhs)) : 0) +  12,,
  "%%(%.4f, %.4f*pi)%s", creal(lhs), cimag(lhs) / M_PI, rhs ? STRING(rhs) : "")
describe_string(Vec3, m_vec3, SZ_VEC3,
  (rhs ? strlen(STRING(rhs)) : 0) + 23 + num_digit((m_uint)lhs.x) +
                                      num_digit((m_uint)lhs.y) + num_digit((m_uint)lhs.z),,
  "@(%.4f, %.4f, %.4f)%s", lhs.x, lhs.y, lhs.z, rhs ? STRING(rhs) : "")
describe_string(Vec4, m_vec4, SZ_VEC4,
  (rhs ? strlen(STRING(rhs)) : 0) + 30 + num_digit((m_uint)lhs.x) +
  num_digit((m_uint)lhs.y) + num_digit((m_uint)lhs.z) + num_digit((m_uint)lhs.w),,
  "@(%.4f, %.4f, %.4f, %.4f)%s", lhs.x, lhs.y, lhs.z, lhs.w, rhs ? STRING(rhs) : "")
describe_string(Object, M_Object, SZ_INT,
  17 + (rhs ? strlen(STRING(rhs)) : 0), /*release(lhs, shred)*/,
  "%p%s", (void*)lhs, rhs ? STRING(rhs) : "")


#define describe_string_plus(name, offset, type, opt, len, format, ...) \
static INSTR(name##String_Plus) {\
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
  lhs ? strlen(STRING(lhs)) : 0, "%s", lhs ? STRING(lhs) : "")
describe_string_plus(Int_, SZ_INT, m_int,,
  num_digit((m_uint)lhs), "%"INT_F, lhs)
describe_string_plus(Float_, SZ_FLOAT, m_float,,
  num_digit((m_uint)lhs) + 6, "%.4f", lhs)
describe_string_plus(Complex_, SZ_COMPLEX, m_complex,,
  num_digit((m_uint)creal(lhs)) + num_digit((m_uint)cimag(lhs)) + 18, "#(%.4f, %.4f)",
  creal(lhs), cimag(lhs))
describe_string_plus(Polar_, SZ_COMPLEX, m_complex,,
  num_digit((m_uint)creal(lhs)) + num_digit((m_uint)cimag(lhs)) + 18, "%%(%.4f, %.4f)",
  creal(lhs), cimag(lhs) / M_PI)
describe_string_plus(Vec3_, SZ_VEC3, m_vec3,,
  num_digit((m_uint)lhs.x) + num_digit((m_uint)lhs.y) + num_digit((m_uint)lhs.z) + 20, "@(%.4f, %.4f, %.4f)",
  lhs.x, lhs.y, lhs.z)
describe_string_plus(Vec4_, SZ_VEC4, m_vec4,,
  num_digit((m_uint)lhs.x) + num_digit((m_uint)lhs.y) + num_digit((m_uint)lhs.z) + num_digit((m_uint)lhs.z) + 28, "@(%.4f, %.4f, %.4f, %.4f)",
  lhs.x, lhs.y, lhs.z, lhs.w)
describe_string_plus(Object_, SZ_INT, M_Object, release(lhs, shred),
  11, "%p", (void*)lhs)

static CTOR(string_ctor) {
  STRING(o) = "";
}

ANN Type check_prim_str(const Env, const m_str *);
ID_CHECK(check_funcpp) {
  ((Exp_Primary*)prim)->prim_type = ae_prim_str;
  ((Exp_Primary*)prim)->d.str = env->func ? env->func->name : env->class_def ?
    env->class_def->name : env->name;
  return check_prim_str(env, &prim->d.str);
}

static GACK(gack_string) {
  const M_Object obj = *(M_Object*)VALUE;
  printf("%s", obj ? STRING(obj) : "(null string)");
}
GWION_IMPORT(string) {
  const Type t_string = gwi_class_ini(gwi, "string", NULL);
  gwi_class_xtor(gwi, string_ctor, NULL);
  GWI_BB(gwi_gack(gwi, t_string, gack_string))
  gwi->gwion->type[et_string] = t_string; // use func

  gwi_item_ini(gwi, "@internal", "@data");
  GWI_BB(gwi_item_end(gwi,   ae_flag_const, NULL))
  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_oper_ini(gwi, "string",  "string", "string"))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "=>",      String_Assign))
  GWI_BB(gwi_oper_end(gwi, "+",       String_String))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "+=>", String_Plus))

  GWI_BB(gwi_oper_ini(gwi, "string",  "string", "int"))
  GWI_BB(gwi_oper_end(gwi, "==",       String_eq))
  GWI_BB(gwi_oper_end(gwi, "!=",       String_neq))

  GWI_BB(gwi_oper_ini(gwi, "int",     "string", "string"))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "=>",      Int_String_Assign))
  GWI_BB(gwi_oper_end(gwi, "+",       Int_String))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "+=>", Int_String_Plus))

  GWI_BB(gwi_oper_ini(gwi, "float",   "string", "string"))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "=>",      Float_String_Assign))
  GWI_BB(gwi_oper_end(gwi, "+",       Float_String))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "+=>", Float_String_Plus))

  GWI_BB(gwi_oper_ini(gwi, "complex", "string", "string"))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "=>",      Complex_String_Assign))
  GWI_BB(gwi_oper_end(gwi, "+",       Complex_String))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "+=>", Complex_String_Plus))

  GWI_BB(gwi_oper_ini(gwi, "polar",   "string", "string"))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "=>",      Polar_String_Assign))
  GWI_BB(gwi_oper_end(gwi, "+",       Polar_String))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "+=>", Polar_String_Plus))

  GWI_BB(gwi_oper_ini(gwi,"Vec3",     "string", "string"))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "=>",      Vec3_String_Assign))
  GWI_BB(gwi_oper_end(gwi, "+",       Vec3_String))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "+=>", Vec3_String_Plus))

  GWI_BB(gwi_oper_ini(gwi, "Vec4",    "string", "string"))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "=>",      Vec4_String_Assign))
  GWI_BB(gwi_oper_end(gwi, "+",       Vec4_String))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "+=>", Vec4_String_Plus))

  GWI_BB(gwi_oper_ini(gwi, "Object",  "string", "string"))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "=>",      Object_String_Assign))
  GWI_BB(gwi_oper_end(gwi, "+",       Object_String))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "+=>", Object_String_Plus))

  GWI_BB(gwi_oper_ini(gwi, "@null",   "string", "string"))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "=>",      Object_String_Assign))
  GWI_BB(gwi_oper_end(gwi, "+",       Object_String))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "+=>", Object_String_Plus))

//  gwi_item_ini(gwi, "string", "__func__");
//  gwi_item_end(gwi, ae_flag_const, NULL);
//  gwi_reserve(gwi, "__func__");

  struct SpecialId_ spid = { .ck=check_funcpp, .exec=RegPushMe, .is_const=1 };
  gwi_specialid(gwi, "__func__", &spid);
  return GW_OK;
}
