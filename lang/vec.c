#include <math.h>
#include "defs.h"
#include "import.h"

struct Type_ t_vec3 = { "Vec3", SZ_VEC3, NULL, te_vec3};
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
  RETURN->d.v_float = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

static MFUN(vec3_normalize) {
  m_vec3* v =  &**(m_vec3**)MEM(0);
  m_float mag = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
  if(mag  > 0) {
    v->x /= mag;
    v->y /= mag;
    v->z /= mag;
  }
}

static MFUN(vec3_interp) {
  m_vec3* v =  &**(m_vec3**)MEM(0);
  v->x = (v->y - v->x) * v->z + v->x;
  RETURN->d.v_float = v->x;
}

static MFUN(vec3_float) {
  m_vec3* v =  &**(m_vec3**)MEM(0);
  v->x = (v->y - v->x) * v->z * (*(m_float*)MEM(SZ_INT)) + v->x;
  RETURN->d.v_float = v->x;
}

static MFUN(vec3_dur) {
  m_vec3* v =  &**(m_vec3**)MEM(0);
  v->x = (v->y - v->x) * v->z * (*(m_float*)MEM(SZ_INT) / shred->vm_ref->bbq->sp->sr) + v->x;
  RETURN->d.v_float = v->x;
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
  *(m_float*)REG(0) = r.x;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.y;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.z;
  PUSH_REG(shred,  SZ_FLOAT);
}

static INSTR(vec3_minus) {
  m_vec3 r, * t = (m_vec3*)REG(0);
  POP_REG(shred, SZ_VEC3 * 2);
  r.x = t->x - (t + 1)->x;
  r.y = t->y - (t + 1)->y;
  r.z = t->z - (t + 1)->z;
  *(m_float*)REG(0) = r.x;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.y;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.z;
  PUSH_REG(shred,  SZ_FLOAT);
}

static INSTR(vec3_xproduct) {
  m_vec3 r, * t = (m_vec3*)REG(0);
  POP_REG(shred, SZ_VEC3 * 2);
  r.x = t->x * (t + 1)->x;
  r.y = t->y * (t + 1)->y;
  r.z = t->z * (t + 1)->z;
  *(m_float*)REG(0) = r.x;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.y;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.z;
  PUSH_REG(shred,  SZ_FLOAT);
}

static INSTR(float_times_vec3) {
  POP_REG(shred, SZ_FLOAT + SZ_VEC3);
  m_float f = *(m_float*)REG(0);
  m_vec3 r = *(m_vec3*)REG(SZ_FLOAT);
  r.x *= f;
  r.y *= f;
  r.z *= f;
  *(m_float*)REG(0) = r.x;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.y;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.z;
  PUSH_REG(shred,  SZ_FLOAT);
}
static INSTR(vec3_times_float) {
  POP_REG(shred, SZ_FLOAT + SZ_VEC3);
  m_vec3 r = *(m_vec3*)REG(0);
  m_float f = *(m_float*)REG(SZ_VEC3);
  r.x *= f;
  r.y *= f;
  r.z *= f;
  *(m_float*)REG(0) = r.x;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.y;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.z;
  PUSH_REG(shred,  SZ_FLOAT);
}

static INSTR(vec3_divide_float) {
  POP_REG(shred, SZ_FLOAT + SZ_VEC3);
  m_vec3 r = *(m_vec3*)REG(0);
  m_float f = *(m_float*)REG(SZ_VEC3);
  r.x /= f;
  r.y /= f;
  r.z /= f;
  *(m_float*)REG(0) = r.x;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.y;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.z;
  PUSH_REG(shred,  SZ_FLOAT);
}

static INSTR(vec3_r_assign) {
  POP_REG(shred, SZ_VEC3 + SZ_INT);
  m_vec3* r = *(m_vec3**)REG(SZ_VEC3);
  r->x = *(m_float*)REG(0);
  r->y = *(m_float*)REG(SZ_FLOAT);
  r->z = *(m_float*)REG(SZ_COMPLEX);
  *(m_vec3*)REG(0) = *r;
  PUSH_REG(shred, SZ_VEC3);
}

INSTR(vec3_x) {
  if(instr->m_val) {
    POP_REG(shred, SZ_INT);
    m_vec3* v = *(m_vec3**)REG(0);
    *(m_float**)REG(0) = &v->x;
    PUSH_REG(shred,  SZ_INT);
  } else {
    POP_REG(shred, SZ_INT);
    m_vec3* v = *(m_vec3**)REG(0);
    *(m_float*)REG(0) = v->x;
    PUSH_REG(shred,  SZ_FLOAT);
  }
}

INSTR(vec3_y) {
  if(instr->m_val) {
    POP_REG(shred, SZ_INT);
    m_vec3* v = *(m_vec3**)REG(0);
    *(m_float**)REG(0) = &v->y;
    PUSH_REG(shred,  SZ_INT);
  } else {
    POP_REG(shred, SZ_INT);
    m_vec3* v = *(m_vec3**)REG(0);
    *(m_float*)REG(0) = v->y;
    PUSH_REG(shred,  SZ_FLOAT);
  }
}

INSTR(vec3_z) {
  if(instr->m_val) {
    POP_REG(shred, SZ_INT);
    m_vec3* v = *(m_vec3**)REG(0);
    *(m_float**)REG(0) = &v->z;
    PUSH_REG(shred,  SZ_INT);
  } else {
    POP_REG(shred, SZ_INT);
    m_vec3* v = *(m_vec3**)REG(0);
    *(m_float*)REG(0) = v->z;
    PUSH_REG(shred,  SZ_FLOAT);
  }
}

INSTR(vec4_x) {
  if(instr->m_val) {
    POP_REG(shred, SZ_INT);
    m_vec4* v = *(m_vec4**)REG(0);
    *(m_float**)REG(0) = &v->x;
    PUSH_REG(shred,  SZ_INT);
  } else {
    POP_REG(shred, SZ_INT);
    m_vec4* v = *(m_vec4**)REG(0);
    *(m_float*)REG(0) = v->x;
    PUSH_REG(shred,  SZ_FLOAT);
  }
}

INSTR(vec4_y) {
  if(instr->m_val) {
    POP_REG(shred, SZ_INT);
    m_vec4* v = *(m_vec4**)REG(0);
    *(m_float**)REG(0) = &v->y;
    PUSH_REG(shred,  SZ_INT);
  } else {
    POP_REG(shred, SZ_INT);
    m_vec4* v = *(m_vec4**)REG(0);
    *(m_float*)REG(0) = v->y;
    PUSH_REG(shred,  SZ_FLOAT);
  }
}

INSTR(vec4_z) {
  if(instr->m_val) {
    POP_REG(shred, SZ_INT);
    m_vec4* v = *(m_vec4**)REG(0);
    *(m_float**)REG(0) = &v->z;
    PUSH_REG(shred,  SZ_INT);
  } else {
    POP_REG(shred, SZ_INT);
    m_vec4* v = *(m_vec4**)REG(0);
    *(m_float*)REG(0) = v->z;
    PUSH_REG(shred,  SZ_FLOAT);
  }
}

INSTR(vec4_w) {
  if(instr->m_val) {
    POP_REG(shred, SZ_INT);
    m_vec4* v = *(m_vec4**)REG(0);
    *(m_float**)REG(0) = &v->w;
    PUSH_REG(shred,  SZ_INT);
  } else {
    POP_REG(shred, SZ_INT);
    m_vec4* v = *(m_vec4**)REG(0);
    *(m_float*)REG(0) = v->w;
    PUSH_REG(shred,  SZ_FLOAT);
  }
}

m_bool import_vec3(Env env) {
  DL_Func fun;

  CHECK_BB(import_class_begin(env, &t_vec3, env->global_nspc, NULL, NULL))
  import_var(env, "float", "x",   0, NULL);
  import_var(env, "float", "y",   0, NULL);
  import_var(env, "float", "z",   0, NULL);
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

struct Type_ t_vec4 = { "Vec4", SZ_VEC4, NULL, te_vec4};

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
  RETURN->d.v_float = sqrt(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}

static MFUN(vec4_normalize) {
  m_vec4* v =  &**(m_vec4**)MEM(0);
  m_float mag = sqrt(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
  if(mag  > 0) {
    v->x /= mag;
    v->y /= mag;
    v->z /= mag;
    v->w /= mag;
  }
}

static INSTR(vec4_add) {
  m_vec4 r, * t = (m_vec4*)REG(0);
  POP_REG(shred, SZ_VEC4 * 2);
  r.x = t->x + (t + 1)->x;
  r.y = t->y + (t + 1)->y;
  r.z = t->z + (t + 1)->z;
  r.w = t->w + (t + 1)->w;
  *(m_float*)REG(0) = r.x;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.y;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.z;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.w;
  PUSH_REG(shred,  SZ_FLOAT);
}

static INSTR(vec4_minus) {
  m_vec4 r, * t = (m_vec4*)REG(0);
  POP_REG(shred, SZ_VEC4 * 2);
  r.x = t->x - (t + 1)->x;
  r.y = t->y - (t + 1)->y;
  r.z = t->z - (t + 1)->z;
  r.w = t->w - (t + 1)->w;
  *(m_float*)REG(0) = r.x;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.y;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.z;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.w;
  PUSH_REG(shred,  SZ_FLOAT);
}

static INSTR(vec4_xproduct) {
  m_vec4 r, * t = (m_vec4*)REG(0);
  POP_REG(shred, SZ_VEC4 * 2);
  r.x = t->x * (t + 1)->x;
  r.y = t->y * (t + 1)->y;
  r.z = t->z * (t + 1)->z;
  r.w = t->w * (t + 1)->w;
  *(m_float*)REG(0) = r.x;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.y;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.z;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.w;
  PUSH_REG(shred,  SZ_FLOAT);
}

static INSTR(float_times_vec4) {
  POP_REG(shred, SZ_FLOAT + SZ_VEC4);
  m_float f = *(m_float*)REG(0);
  m_vec4 r = *(m_vec4*)REG(SZ_FLOAT);
  r.x *= f;
  r.y *= f;
  r.z *= f;
  *(m_float*)REG(0) = r.x;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.y;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.z;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.w;
  PUSH_REG(shred,  SZ_FLOAT);
}

static INSTR(vec4_times_float) {
  POP_REG(shred, SZ_FLOAT + SZ_VEC4);
  m_vec4 r = *(m_vec4*)REG(0);
  m_float f = *(m_float*)REG(SZ_VEC4);
  r.x *= f;
  r.y *= f;
  r.z *= f;
  r.w *= f;
  *(m_float*)REG(0) = r.x;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.y;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.z;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.w;
  PUSH_REG(shred,  SZ_FLOAT);
}

static INSTR(vec4_divide_float) {
  POP_REG(shred, SZ_FLOAT + SZ_VEC4);
  m_vec4 r = *(m_vec4*)REG(0);
  m_float f = *(m_float*)REG(SZ_VEC4);
  r.x /= f;
  r.y /= f;
  r.z /= f;
  r.w /= f;
  *(m_float*)REG(0) = r.x;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.y;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.z;
  PUSH_REG(shred,  SZ_FLOAT);
  *(m_float*)REG(0) = r.w;
  PUSH_REG(shred,  SZ_FLOAT);
}

static INSTR(vec4_r_assign) {
  POP_REG(shred, SZ_VEC4 + SZ_INT);
  m_vec4* r = *(m_vec4**)REG(SZ_VEC4);
  r->x = *(m_float*)REG(0);
  r->y = *(m_float*)REG(SZ_FLOAT);
  r->z = *(m_float*)REG(SZ_FLOAT * 2);
  r->w = *(m_float*)REG(SZ_FLOAT * 3);
  *(m_vec4*)REG(0) = *r;
  PUSH_REG(shred, SZ_VEC4);
}

m_bool import_vec4(Env env) {
  DL_Func fun;
  CHECK_BB(import_class_begin(env, &t_vec4, env->global_nspc, NULL, NULL))
  import_var(env, "float", "x",   0, NULL);
  import_var(env, "float", "y",   0, NULL);
  import_var(env, "float", "z",   0, NULL);
  import_var(env, "float", "w",   0, NULL);
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
