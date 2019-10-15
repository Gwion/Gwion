#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "instr.h"
#include "nspc.h"
#include "type.h"
#include "object.h"
#include "emit.h"
#include "env.h"
#include "vm.h"
#include "gwion.h"
#include "value.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "engine.h"
#include "parser.h"
#include "lang_private.h"
#include "specialid.h"

static GACK(gack_class) {
  printf("class(%s)", actual_type(shred->info->vm->gwion, t)->name);
}

static GACK(gack_function) {
  printf("%s", t->name);
}

static GACK(gack_fptr) {
  const VM_Code code = *(VM_Code*)VALUE;
  if(code)
    printf("%s", code->name);
  else
    printf("%s", t->name);
}

static GACK(gack_void) {
  printf("(void)");
}

static GACK(gack_int) {
  printf("%li", *(m_uint*)VALUE);
}

static GACK(gack_char) {
  printf("%c", *(char*)VALUE);
}

static GACK(gack_float) {
  printf("%.4f", *(m_float*)VALUE);
}

// we where using m_complex
static GACK(gack_complex) {
  printf("#(%.4f, %.4f)", *(m_float*)VALUE, *(m_float*)(VALUE + SZ_FLOAT));
}
static GACK(gack_polar) {
  printf("%%(%.4f, %.4f*pi)", *(m_float*)VALUE, *(m_float*)(VALUE + SZ_FLOAT) / M_PI);
}
static GACK(gack_vec3) {
  printf("%%(%.4f, %.4f, %.4f)", *(m_float*)VALUE, *(m_float*)(VALUE + SZ_FLOAT), *(m_float*)(VALUE + SZ_FLOAT*2));
}
static GACK(gack_vec4) {
  printf("%%(%.4f, %.4f, %.4f, %.4f)", *(m_float*)VALUE, *(m_float*)(VALUE + SZ_FLOAT), *(m_float*)(VALUE + SZ_FLOAT*2), *(m_float*)(VALUE + SZ_FLOAT*3));
}

#define mk_class_instr(op, arg0, arg1, ...)                          \
static INSTR(instr_class_##op) {                                     \
  POP_REG(shred, SZ_INT);                                            \
  const Type l = *(Type*)(shred->reg - SZ_INT);                      \
  const Type r = *(Type*)(shred->reg);                               \
  *(m_uint*)(shred->reg - SZ_INT) = isa(arg0, arg1) > 0 __VA_ARGS__; \
}
mk_class_instr(ge, l, r)
mk_class_instr(gt, l, r, && l != r)
mk_class_instr(le, r, l)
mk_class_instr(lt, r, l, && l != r)

ANN static m_bool import_core_libs(const Gwi gwi) {
  const Type t_class = gwi_mk_type(gwi, "@Class", SZ_INT, NULL);
  gwi->gwion->type[et_class] = t_class;
  GWI_BB(gwi_add_type(gwi, t_class))
  GWI_BB(gwi_gack(gwi, gwi->gwion->type[et_class], gack_class)) // not working yet
  gwi->gwion->type[et_class] = t_class;
  const Type t_undefined = gwi_mk_type(gwi, "@Undefined", SZ_INT, NULL);
  GWI_BB(gwi_set_global_type(gwi, t_undefined, et_undefined))
  const Type t_auto = gwi_mk_type(gwi, "auto", SZ_INT, NULL);
  GWI_BB(gwi_set_global_type(gwi, t_auto, et_auto))
  SET_FLAG(t_class, abstract);
  const Type t_void  = gwi_mk_type(gwi, "void", 0, NULL);
  GWI_BB(gwi_gack(gwi, t_void, gack_void))
  GWI_BB(gwi_set_global_type(gwi, t_void, et_void))
  const Type t_null  = gwi_mk_type(gwi, "@null",  SZ_INT, NULL);
  GWI_BB(gwi_set_global_type(gwi, t_null, et_null))
  const Type t_function = gwi_mk_type(gwi, "@function", SZ_INT, NULL);
  GWI_BB(gwi_gack(gwi, t_function, gack_function))
  GWI_BB(gwi_set_global_type(gwi, t_function, et_function))
  const Type t_fptr = gwi_mk_type(gwi, "@func_ptr", SZ_INT, "@function");
  GWI_BB(gwi_gack(gwi, t_fptr, gack_fptr))
  GWI_BB(gwi_set_global_type(gwi, t_fptr, et_fptr))
  const Type t_lambda = gwi_mk_type(gwi, "@lambda", SZ_INT, "@function");
  GWI_BB(gwi_set_global_type(gwi, t_lambda, et_lambda))
  const Type t_gack = gwi_mk_type(gwi, "@Gack", SZ_INT, NULL);
  GWI_BB(gwi_set_global_type(gwi, t_gack, et_gack))
  const Type t_int = gwi_mk_type(gwi, "int", SZ_INT, NULL);
  GWI_BB(gwi_gack(gwi, t_int, gack_int))
  GWI_BB(gwi_set_global_type(gwi, t_int, et_int))
  const Type t_char = gwi_mk_type(gwi, "char", SZ_INT, "int");
  GWI_BB(gwi_gack(gwi, t_char, gack_char))
  GWI_BB(gwi_set_global_type(gwi, t_char, et_char))
  const Type t_float = gwi_mk_type(gwi, "float", SZ_FLOAT, NULL);
  GWI_BB(gwi_gack(gwi, t_float, gack_float))
  GWI_BB(gwi_set_global_type(gwi, t_float, et_float))
  const Type t_dur = gwi_mk_type(gwi, "dur", SZ_FLOAT, NULL);
  GWI_BB(gwi_gack(gwi, t_dur, gack_float))
  GWI_BB(gwi_add_type(gwi, t_dur))
  const Type t_time = gwi_mk_type(gwi, "time", SZ_FLOAT, NULL);
  GWI_BB(gwi_gack(gwi, t_time, gack_float))
  GWI_BB(gwi_add_type(gwi, t_time))
  const Type t_now = gwi_mk_type(gwi, "@now", SZ_FLOAT, "time");
  GWI_BB(gwi_add_type(gwi, t_now))
  struct SpecialId_ spid = { .type=t_now, .exec=RegPushNow, .is_const=1 };
  gwi_specialid(gwi, "now", &spid);
  const Type t_complex = gwi_mk_type(gwi, "complex", SZ_COMPLEX , NULL);
  gwi->gwion->type[et_complex] = t_complex;
  GWI_BB(gwi_gack(gwi, t_complex, gack_complex))
  const Type t_polar   = gwi_mk_type(gwi, "polar", SZ_COMPLEX , NULL);
  gwi->gwion->type[et_polar] = t_polar;
  GWI_BB(gwi_gack(gwi, t_polar, gack_polar))
  const Type t_vec3 = gwi_mk_type(gwi, "Vec3", SZ_VEC3, NULL);
  gwi->gwion->type[et_vec3] = t_vec3;
  GWI_BB(gwi_gack(gwi, t_vec3, gack_vec3))
  const Type t_vec4 = gwi_mk_type(gwi, "Vec4", SZ_VEC4, NULL);
  gwi->gwion->type[et_vec4] = t_vec4;
  GWI_BB(gwi_gack(gwi, t_vec4, gack_vec4))
  GWI_BB(import_object(gwi))
  const Type t_union = gwi_mk_type(gwi, "@Union", SZ_INT, "Object");
  gwi->gwion->type[et_union] = t_union;
  GWI_BB(gwi_class_ini(gwi, t_union, NULL, NULL))
  GWI_BB(gwi_class_end(gwi))
  GWI_BB(import_tuple(gwi))
  GWI_BB(import_array(gwi))
  GWI_BB(import_event(gwi))
  GWI_BB(import_ugen(gwi))
  GWI_BB(import_ptr(gwi))
  GWI_BB(import_func(gwi))
  GWI_BB(gwi_oper_ini(gwi, NULL, (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_new))
  GWI_BB(gwi_oper_emi(gwi, opem_new))
  GWI_BB(gwi_oper_end(gwi, "new", NULL))
  GWI_BB(import_prim(gwi))
  GWI_BB(import_complex(gwi))
  GWI_BB(import_vec3(gwi))
  GWI_BB(import_vec4(gwi))
  GWI_BB(import_vararg(gwi))
  GWI_BB(import_string(gwi))
  GWI_BB(import_shred(gwi))
  GWI_BB(import_modules(gwi))

  GWI_BB(gwi_oper_ini(gwi, "@Class", "@Class", "int"))
  GWI_BB(gwi_oper_end(gwi, "==", int_eq))
  GWI_BB(gwi_oper_end(gwi, "!=", int_neq))
  GWI_BB(gwi_oper_end(gwi, ">=", instr_class_ge))
  GWI_BB(gwi_oper_end(gwi, ">",  instr_class_gt))
  GWI_BB(gwi_oper_end(gwi, "<=", instr_class_le))
  GWI_BB(gwi_oper_end(gwi, "<",  instr_class_lt))
  return GW_OK;
}

ANN m_bool type_engine_init(VM* vm, const Vector plug_dirs) {
  vm->gwion->env->name = "[builtin]";
  struct YYLTYPE loc = {};
  struct Gwi_ gwi = { .gwion=vm->gwion, .loc=&loc };
  CHECK_BB(import_core_libs(&gwi))
  vm->gwion->env->name = "[imported]";
  for(m_uint i = 0; i < vector_size(plug_dirs); ++i) {
    m_bool (*import)(Gwi) = (m_bool(*)(Gwi))vector_at(plug_dirs, i);
    if(import && import(&gwi) < 0)
      env_reset(gwi.gwion->env);
  }
  return GW_OK;
}
