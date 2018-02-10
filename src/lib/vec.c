#include <math.h>
#include "type.h"
#include "instr.h"
#include "import.h"

struct Type_ t_vec3  = { "Vec3",  SZ_VEC3,  NULL,    te_vec3};
struct Type_ t_vec4  = { "Vec4",  SZ_VEC4,  NULL,    te_vec4};

INSTR(vec_member) {
  POP_REG(shred, SZ_INT);
  if(instr->m_val) {
    *(m_float**)REG(0) = &*(m_float*)(*(char**)shred->reg + instr->m_val2);
    PUSH_REG(shred, SZ_INT);
  } else {
    *(m_float*)REG(0) = *(m_float*)(*(char**)shred->reg + instr->m_val2);
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
  v->x = (v->y - v->x) * v->z * (*(m_float*)MEM(SZ_INT) / shred->vm_ref->sp->sr) + v->x;
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

m_bool import_vec3(Gwi gwi) {
  CHECK_BB(gwi_class_ini(gwi,  &t_vec3, NULL, NULL))
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
  CHECK_BB(gwi_oper_end(gwi, op_minus, vec3_minus))
  CHECK_BB(gwi_oper_end(gwi, op_times, vec3_xproduct))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, vec3_r_assign))
  CHECK_BB(gwi_oper_ini(gwi, "Vec3", "float", "Vec3"))
  CHECK_BB(gwi_oper_end(gwi, op_times,  vec3_times_float))
  CHECK_BB(gwi_oper_end(gwi, op_divide, vec3_divide_float))
  CHECK_BB(gwi_oper_ini(gwi, "float", "Vec3", "Vec3"))
  CHECK_BB(gwi_oper_end(gwi, op_times,  float_times_vec3))
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
  m_vec4* r = *(m_vec4**)REG(SZ_VEC4);
  r->x = *(m_float*)REG(0);
  r->y = *(m_float*)REG(SZ_FLOAT);
  r->z = *(m_float*)REG(SZ_FLOAT * 2);
  r->w = *(m_float*)REG(SZ_FLOAT * 3);
  PUSH_REG(shred, SZ_VEC4);
}

m_bool import_vec4(Gwi gwi) {
  CHECK_BB(gwi_class_ini(gwi,  &t_vec4, NULL, NULL))
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
  CHECK_BB(gwi_oper_end(gwi, op_minus, vec4_minus))
  CHECK_BB(gwi_oper_end(gwi, op_times, vec4_xproduct))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, vec4_r_assign))
  CHECK_BB(gwi_oper_ini(gwi, "Vec4", "float", "Vec4"))
  CHECK_BB(gwi_oper_end(gwi, op_times, vec4_times_float))
  CHECK_BB(gwi_oper_end(gwi, op_divide, vec4_divide_float))
  CHECK_BB(gwi_oper_ini(gwi, "float", "Vec4", "Vec4"))
  CHECK_BB(gwi_oper_end(gwi, op_times, float_times_vec4))
    return 1;
}
