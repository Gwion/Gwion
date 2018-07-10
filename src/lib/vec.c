#include <math.h>
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "import.h"

INSTR(vec_member) { GWDEBUG_EXE
  if(instr->m_val)
    *(m_float**)REG(-SZ_INT) = (m_float*)(*(m_bit**)REG(-SZ_INT) + instr->m_val2);
  else {
    *(m_float*)REG(-SZ_INT) = *(m_float*)(*(m_bit**)REG(-SZ_INT) + instr->m_val2);
    PUSH_REG(shred, SZ_FLOAT - SZ_INT);
  }
}

#define describe_vec(func, op)                                                \
ANN static void vec_##func(const m_bit* v, const int size, const m_float f) { \
  for(int i = size + 1; --i;)                                                 \
    *(m_float*)(v + (i-1) * SZ_FLOAT) op##= f;                                \
}
describe_vec(add, +)
describe_vec(sub, -)
describe_vec(mul, *)
describe_vec(div, /)

static MFUN(vec3_set) {
  m_vec3* v =  (m_vec3*)MEM(0);
  v->x = *(m_float*)MEM(SZ_INT);
  v->y = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  v->z = *(m_float*)MEM(SZ_INT + SZ_FLOAT * 2);
}

static MFUN(vec3_setAll) {
  m_vec3* v =  *(m_vec3**)MEM(0);
  v->x = *(m_float*)MEM(SZ_INT);
  v->y = *(m_float*)MEM(SZ_INT);
  v->z = *(m_float*)MEM(SZ_INT);
}

static MFUN(vec3_magnitude) {
  const m_vec3* v =  *(m_vec3**)MEM(0);
  *(m_float*)RETURN = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

static MFUN(vec3_normalize) {
  const m_vec3* v =  *(m_vec3**)MEM(0);
  const m_float mag = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
  if(mag  > 0)
    vec_div((m_bit*)v, 3, mag);
}

#define describe_vec3_x(name, arg)    \
static MFUN(vec3_##name) {            \
  m_vec3* v =  *(m_vec3**)MEM(0);   \
  v->x = (v->y - v->x) * v->z arg;    \
  *(m_float*)RETURN = v->x;           \
}
describe_vec3_x(interp, + v->x)
describe_vec3_x(float, * v->z * (*(m_float*)MEM(SZ_INT)) + v->x)
describe_vec3_x(dur, * (*(m_float*)MEM(SZ_INT) / shred->vm_ref->sp->sr) + v->x)

static MFUN(vec3_update) {
  m_vec3* v =  *(m_vec3**)MEM(0);
  v->y = *(m_float*)MEM(SZ_INT);
}

static MFUN(vec3_update_slew) {
  m_vec3* v =  *(m_vec3**)MEM(0);
  v->y = *(m_float*)MEM(SZ_INT);
  v->z = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
}

static MFUN(vec3_update_set) {
  m_vec3* v =  *(m_vec3**)MEM(0);
  v->x = *(m_float*)MEM(SZ_INT);
  v->y = *(m_float*)MEM(SZ_INT);

}

static MFUN(vec3_update_set_slew) {
  m_vec3* v =  *(m_vec3**)MEM(0);
  v->x = *(m_float*)MEM(SZ_INT);
  v->y = *(m_float*)MEM(SZ_INT);
  v->z = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
}

#define describe_vec3(name, op)           \
static INSTR(vec3_##name) { GWDEBUG_EXE   \
  POP_REG(shred, SZ_VEC3);                \
  m_vec3 r, * t = (m_vec3*)REG(-SZ_VEC3); \
  r.x = t->x op (t + 1)->x;               \
  r.y = t->y op (t + 1)->y;               \
  r.z = t->z op (t + 1)->z;               \
  *(m_vec3*)REG(-SZ_VEC3) = r;            \
}
describe_vec3(add, +)
describe_vec3(sub, -)
describe_vec3(mul, *)
describe_vec3(div, /)


#define describe_float_vec3(func)               \
static INSTR(float_##func##_vec3) { GWDEBUG_EXE \
  POP_REG(shred, SZ_FLOAT);                     \
  const m_float f = *(m_float*)REG(-SZ_VEC3);   \
  const m_vec3 r = *(m_vec3*)REG(-SZ_COMPLEX);  \
  vec_mul((m_bit*)&r, 3, f);                    \
  *(m_vec3*)REG(-SZ_VEC3) = r;                  \
}
describe_float_vec3(add)
describe_float_vec3(sub)
describe_float_vec3(mul)
describe_float_vec3(div)

#define describe_vec3_float(func)               \
static INSTR(vec3_##func##_float) { GWDEBUG_EXE \
  POP_REG(shred, SZ_FLOAT);                     \
  const m_vec3 r = *(m_vec3*)REG(-SZ_VEC3);     \
  const m_float f = *(m_float*)REG(0);          \
  vec_##func((m_bit*)&r, 3, f);                 \
  *(m_vec3*)REG(-SZ_VEC3) = r;                  \
}
describe_vec3_float(add)
describe_vec3_float(sub)
describe_vec3_float(mul)
describe_vec3_float(div)

#define describe_vec3_assign(name, offset1, offset2) \
static INSTR(vec3_##name##assign) { GWDEBUG_EXE      \
  POP_REG(shred, SZ_INT);                            \
  m_vec3* r = *(m_vec3**)REG(offset1);             \
  r->x = *(m_float*)REG(offset2-SZ_VEC3);            \
  r->y = *(m_float*)REG(offset2-SZ_COMPLEX);         \
  r->z = *(m_float*)REG(offset2-SZ_FLOAT);           \
  *(m_vec3*)REG(0) = *r;                             \
}
describe_vec3_assign(, -SZ_VEC3, SZ_INT)
describe_vec3_assign(r_, 0, )

ANN m_bool import_vec3(const Gwi gwi) {
  CHECK_BB(gwi_class_ini(gwi,  t_vec3, NULL, NULL))
	gwi_item_ini(gwi, "float", "x");
    gwi_item_end(gwi, ae_flag_member, NULL);
	gwi_item_ini(gwi, "float", "y");
  gwi_item_end(gwi, ae_flag_member, NULL);
	gwi_item_ini(gwi, "float", "z");
  gwi_item_end(gwi, ae_flag_member, NULL);
  gwi_func_ini(gwi, "void", "set", vec3_set);
  gwi_func_arg(gwi, "float", "x");
  gwi_func_arg(gwi, "float", "y");
  gwi_func_arg(gwi, "float", "z");
  CHECK_BB(gwi_func_end(gwi, 0))
    gwi_func_ini(gwi, "void", "setAll", vec3_setAll);
  gwi_func_arg(gwi, "float", "x");
  CHECK_BB(gwi_func_end(gwi, 0))
    gwi_func_ini(gwi, "float", "magnitude", vec3_magnitude);
  CHECK_BB(gwi_func_end(gwi, 0))
    gwi_func_ini(gwi, "void", "normalize", vec3_normalize);
  CHECK_BB(gwi_func_end(gwi, 0))
    gwi_func_ini(gwi, "float", "interp", vec3_interp);
  CHECK_BB(gwi_func_end(gwi, 0))
    gwi_func_ini(gwi, "float", "interp", vec3_float);
  gwi_func_arg(gwi, "float", "delta");
  CHECK_BB(gwi_func_end(gwi, 0))
    gwi_func_ini(gwi, "float", "interp", vec3_dur);
  gwi_func_arg(gwi, "dur", "delta");
  CHECK_BB(gwi_func_end(gwi, 0))
    gwi_func_ini(gwi, "void", "update", vec3_update);
  gwi_func_arg(gwi, "float", "goal");
  CHECK_BB(gwi_func_end(gwi, 0))
    gwi_func_ini(gwi, "void", "update", vec3_update_slew);
  gwi_func_arg(gwi, "float", "goal");
  gwi_func_arg(gwi, "float", "slew");
  CHECK_BB(gwi_func_end(gwi, 0))
    gwi_func_ini(gwi, "void", "supdate", vec3_update_set);
  gwi_func_arg(gwi, "float", "goalAndValue");
  CHECK_BB(gwi_func_end(gwi, 0))
    gwi_func_ini(gwi, "void", "supdate", vec3_update_set_slew);
  gwi_func_arg(gwi, "float", "goalAndValue");
  gwi_func_arg(gwi, "float", "slew");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  CHECK_BB(gwi_oper_ini(gwi, "Vec3", "Vec3", "Vec3"))
  CHECK_BB(gwi_oper_end(gwi, op_plus, vec3_add))
  CHECK_BB(gwi_oper_end(gwi, op_minus, vec3_sub))
  CHECK_BB(gwi_oper_end(gwi, op_times, vec3_mul))
  CHECK_BB(gwi_oper_end(gwi, op_divide, vec3_div))
  CHECK_BB(gwi_oper_add(gwi, opck_assign))
  CHECK_BB(gwi_oper_end(gwi, op_assign, vec3_assign))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, vec3_r_assign))
  CHECK_BB(gwi_oper_ini(gwi, "Vec3", "float", "Vec3"))
  CHECK_BB(gwi_oper_end(gwi, op_plus,  vec3_add_float))
  CHECK_BB(gwi_oper_end(gwi, op_minus, vec3_sub_float))
  CHECK_BB(gwi_oper_end(gwi, op_times,  vec3_mul_float))
  CHECK_BB(gwi_oper_end(gwi, op_divide, vec3_div_float))
  CHECK_BB(gwi_oper_ini(gwi, "float", "Vec3", "Vec3"))
  CHECK_BB(gwi_oper_end(gwi, op_plus,  float_add_vec3))
  CHECK_BB(gwi_oper_end(gwi, op_minus,  float_sub_vec3))
  CHECK_BB(gwi_oper_end(gwi, op_times,  float_mul_vec3))
  CHECK_BB(gwi_oper_end(gwi, op_divide,  float_div_vec3))
  return 1;
}

static MFUN(vec4_set) {
  m_vec4* v =  *(m_vec4**)MEM(0);
  v->x = *(m_float*)MEM(SZ_INT);
  v->y = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  v->z = *(m_float*)MEM(SZ_INT + SZ_FLOAT * 2);
  v->w = *(m_float*)MEM(SZ_INT + SZ_FLOAT * 3);
}

static MFUN(vec4_setAll) {
  m_vec4* v =  *(m_vec4**)MEM(0);
  v->x = *(m_float*)MEM(SZ_INT);
  v->y = *(m_float*)MEM(SZ_INT);
  v->z = *(m_float*)MEM(SZ_INT);
  v->w = *(m_float*)MEM(SZ_INT);
}

static MFUN(vec4_magnitude) {
  m_vec4* v =  *(m_vec4**)MEM(0);
  *(m_float*)RETURN = sqrt(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}

static MFUN(vec4_normalize) {
  m_vec4* v =  *(m_vec4**)MEM(0);
  m_float mag = sqrt(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
  if(mag  > 0)
    vec_div((m_bit*)v, 4, mag);
}

#define describe_vec4(name, op)           \
static INSTR(vec4_##name) { GWDEBUG_EXE   \
  POP_REG(shred, SZ_VEC4);                \
  m_vec4 r, * t = (m_vec4*)REG(-SZ_VEC4); \
  r.x = t->x op (t + 1)->x;               \
  r.y = t->y op (t + 1)->y;               \
  r.z = t->z op (t + 1)->z;               \
  r.w = t->w op (t + 1)->w;               \
  *(m_vec4*)REG(-SZ_VEC4) = r;            \
}
describe_vec4(add, +)
describe_vec4(sub, -)
describe_vec4(mul, *)
describe_vec4(div, /)

#define describe_float_vec4(func)               \
static INSTR(float_##func##_vec4) { GWDEBUG_EXE \
  POP_REG(shred, SZ_FLOAT);                     \
  m_float f = *(m_float*)REG(-SZ_VEC4);         \
  m_vec4 r = *(m_vec4*)REG(-SZ_VEC3);           \
  vec_##func((m_bit*)&r, 4, f);                 \
  *(m_vec4*)REG(-SZ_VEC4) = r;                  \
}
describe_float_vec4(add)
describe_float_vec4(sub)
describe_float_vec4(mul)
describe_float_vec4(div)

#define describe_vec4_float(func)               \
static INSTR(vec4_##func##_float) { GWDEBUG_EXE \
  POP_REG(shred, SZ_FLOAT);                     \
  m_vec4 r = *(m_vec4*)REG(-SZ_VEC4);           \
  m_float f = *(m_float*)REG(0);                \
  vec_##func((m_bit*)&r, 4, f);                 \
  *(m_vec4*)REG(-SZ_VEC4) = r;                  \
}
describe_vec4_float(add)
describe_vec4_float(sub)
describe_vec4_float(mul)
describe_vec4_float(div)

#define describe_vec4_assign(name, offset1, offset2) \
static INSTR(vec4_##name##assign) { GWDEBUG_EXE      \
  POP_REG(shred, SZ_INT);                            \
  m_vec4* r = *(m_vec4**)REG(offset1);             \
  r->x = *(m_float*)REG(offset2-SZ_VEC4);            \
  r->y = *(m_float*)REG(offset2-SZ_VEC3);            \
  r->z = *(m_float*)REG(offset2-SZ_COMPLEX);         \
  r->w = *(m_float*)REG(offset2-SZ_FLOAT);           \
  *(m_vec4*)REG(0) = *r;                             \
}
describe_vec4_assign(, -SZ_VEC4, SZ_INT)
describe_vec4_assign(r_, 0, )

m_bool import_vec4(Gwi gwi) {
  CHECK_BB(gwi_class_ini(gwi,  t_vec4, NULL, NULL))
	gwi_item_ini(gwi, "float", "x");
    gwi_item_end(gwi, ae_flag_member, NULL);
	gwi_item_ini(gwi, "float", "y");
  gwi_item_end(gwi, ae_flag_member, NULL);
	gwi_item_ini(gwi, "float", "z");
  gwi_item_end(gwi, ae_flag_member, NULL);
	gwi_item_ini(gwi, "float", "w");
  gwi_item_end(gwi, ae_flag_member, NULL);
  gwi_func_ini(gwi, "void", "set", vec4_set);
  gwi_func_arg(gwi, "float", "x");
  gwi_func_arg(gwi, "float", "y");
  gwi_func_arg(gwi, "float", "z");
  gwi_func_arg(gwi, "float", "w");
  CHECK_BB(gwi_func_end(gwi, 0))
    gwi_func_ini(gwi, "void", "setAll", vec4_setAll);
  gwi_func_arg(gwi, "float", "x");
  CHECK_BB(gwi_func_end(gwi, 0))
    gwi_func_ini(gwi, "float", "magnitude", vec4_magnitude);
  CHECK_BB(gwi_func_end(gwi, 0))
    gwi_func_ini(gwi, "void", "normalize", vec4_normalize);
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  CHECK_BB(gwi_oper_ini(gwi, "Vec4", "Vec4", "Vec4"))
  CHECK_BB(gwi_oper_end(gwi, op_plus,  vec4_add))
  CHECK_BB(gwi_oper_end(gwi, op_minus, vec4_sub))
  CHECK_BB(gwi_oper_end(gwi, op_times, vec4_mul))
  CHECK_BB(gwi_oper_end(gwi, op_divide, vec4_div))
  CHECK_BB(gwi_oper_add(gwi, opck_assign))
  CHECK_BB(gwi_oper_end(gwi, op_assign, vec4_assign))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, vec4_r_assign))
  CHECK_BB(gwi_oper_ini(gwi, "Vec4", "float", "Vec4"))
  CHECK_BB(gwi_oper_end(gwi, op_plus, vec4_add_float))
  CHECK_BB(gwi_oper_end(gwi, op_minus, vec4_sub_float))
  CHECK_BB(gwi_oper_end(gwi, op_times, vec4_mul_float))
  CHECK_BB(gwi_oper_end(gwi, op_divide, vec4_div_float))
  CHECK_BB(gwi_oper_ini(gwi, "float", "Vec4", "Vec4"))
  CHECK_BB(gwi_oper_end(gwi, op_plus, float_add_vec4))
  CHECK_BB(gwi_oper_end(gwi, op_minus, float_sub_vec4))
  CHECK_BB(gwi_oper_end(gwi, op_times, float_mul_vec4))
  CHECK_BB(gwi_oper_end(gwi, op_divide, float_div_vec4))
    return 1;
}

#ifdef JIT
#include "code/vec.h"
#endif
