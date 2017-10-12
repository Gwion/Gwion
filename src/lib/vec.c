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

m_bool import_vec3(Importer importer) {
  CHECK_BB(importer_class_begin(importer,  &t_vec3, NULL, NULL))
    importer_add_var(importer,  "float", "x", ae_flag_member, NULL);
  importer_add_var(importer,  "float", "y", ae_flag_member, NULL);
  importer_add_var(importer,  "float", "z", ae_flag_member, NULL);
  importer_func_begin(importer, "void", "set", (m_uint)vec3_set);
  importer_add_arg(importer, "float", "x");
  importer_add_arg(importer, "float", "y");
  importer_add_arg(importer, "float", "z");
  CHECK_BB(importer_func_end(importer, 0))
    importer_func_begin(importer, "void", "setAll", (m_uint)vec3_setAll);
  importer_add_arg(importer, "float", "x");
  CHECK_BB(importer_func_end(importer, 0))
    importer_func_begin(importer, "float", "magnitude", (m_uint)vec3_magnitude);
  CHECK_BB(importer_func_end(importer, 0))
    importer_func_begin(importer, "void", "normalize", (m_uint)vec3_normalize);
  CHECK_BB(importer_func_end(importer, 0))
    importer_func_begin(importer, "float", "interp", (m_uint)vec3_interp);
  CHECK_BB(importer_func_end(importer, 0))
    importer_func_begin(importer, "float", "interp", (m_uint)vec3_float);
  importer_add_arg(importer, "float", "delta");
  CHECK_BB(importer_func_end(importer, 0))
    importer_func_begin(importer, "float", "interp", (m_uint)vec3_dur);
  importer_add_arg(importer, "dur", "delta");
  CHECK_BB(importer_func_end(importer, 0))
    importer_func_begin(importer, "void", "update", (m_uint)vec3_update);
  importer_add_arg(importer, "float", "goal");
  CHECK_BB(importer_func_end(importer, 0))
    importer_func_begin(importer, "void", "update", (m_uint)vec3_update_slew);
  importer_add_arg(importer, "float", "goal");
  importer_add_arg(importer, "float", "slew");
  CHECK_BB(importer_func_end(importer, 0))
    importer_func_begin(importer, "void", "supdate", (m_uint)vec3_update_set);
  importer_add_arg(importer, "float", "goalAndValue");
  CHECK_BB(importer_func_end(importer, 0))
    importer_func_begin(importer, "void", "supdate", (m_uint)vec3_update_set_slew);
  importer_add_arg(importer, "float", "goalAndValue");
  importer_add_arg(importer, "float", "slew");
  CHECK_BB(importer_func_end(importer, 0))

  CHECK_BB(importer_oper_begin(importer, "Vec3", "Vec3", "Vec3"))
  CHECK_BB(importer_oper_end(importer, op_plus, vec3_add, 1))
  CHECK_BB(importer_oper_end(importer, op_minus, vec3_minus, 1))
  CHECK_BB(importer_oper_end(importer, op_times, vec3_xproduct, 1))
  CHECK_BB(importer_oper_end(importer, op_chuck, vec3_r_assign, 1))
  CHECK_BB(importer_oper_begin(importer, "Vec3", "float", "Vec3"))
  CHECK_BB(importer_oper_end(importer, op_times,  vec3_times_float, 1))
  CHECK_BB(importer_oper_end(importer, op_divide, vec3_divide_float, 1))
  CHECK_BB(importer_oper_begin(importer, "float", "Vec3", "Vec3"))
  CHECK_BB(importer_oper_end(importer, op_times,  float_times_vec3, 1))
  CHECK_BB(importer_class_end(importer))
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

m_bool import_vec4(Importer importer) {
  CHECK_BB(importer_class_begin(importer,  &t_vec4, NULL, NULL))
    importer_add_var(importer,  "float", "x", ae_flag_member, NULL);
  importer_add_var(importer,  "float", "y", ae_flag_member, NULL);
  importer_add_var(importer,  "float", "z", ae_flag_member, NULL);
  importer_add_var(importer,  "float", "w", ae_flag_member, NULL);
  importer_func_begin(importer, "void", "set", (m_uint)vec4_set);
  importer_add_arg(importer, "float", "x");
  importer_add_arg(importer, "float", "y");
  importer_add_arg(importer, "float", "z");
  importer_add_arg(importer, "float", "w");
  CHECK_BB(importer_func_end(importer, 0))
    importer_func_begin(importer, "void", "setAll", (m_uint)vec4_setAll);
  importer_add_arg(importer, "float", "x");
  CHECK_BB(importer_func_end(importer, 0))
    importer_func_begin(importer, "float", "magnitude", (m_uint)vec4_magnitude);
  CHECK_BB(importer_func_end(importer, 0))
    importer_func_begin(importer, "void", "normalize", (m_uint)vec4_normalize);
  CHECK_BB(importer_func_end(importer, 0))
  CHECK_BB(importer_oper_begin(importer, "Vec4", "Vec4", "Vec4"))
  CHECK_BB(importer_oper_end(importer, op_plus,  vec4_add, 1))
  CHECK_BB(importer_oper_end(importer, op_minus, vec4_minus, 1))
  CHECK_BB(importer_oper_end(importer, op_times, vec4_xproduct, 1))
  CHECK_BB(importer_oper_end(importer, op_chuck, vec4_r_assign, 1))
  CHECK_BB(importer_oper_begin(importer, "Vec4", "float", "Vec4"))
  CHECK_BB(importer_oper_end(importer, op_times, vec4_times_float, 1))
  CHECK_BB(importer_oper_end(importer, op_divide, vec4_divide_float, 1))
  CHECK_BB(importer_oper_begin(importer, "float", "Vec4", "Vec4"))
  CHECK_BB(importer_oper_end(importer, op_times, float_times_vec4, 1))
  CHECK_BB(importer_class_end(importer))
    return 1;
}
