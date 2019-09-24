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
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "engine.h"
#include "parser.h"
#include "lang_private.h"
#include "specialid.h"

static FREEARG(freearg_gack) {
  free_vector(((Gwion)gwion)->mp, (Vector)instr->m_val2);
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
  GWI_OB((t_class = gwi_mk_type(gwi, "Class", SZ_INT, NULL)))
  GWI_BB(gwi_add_type(gwi, t_class))
  GWI_OB((t_undefined = gwi_mk_type(gwi, "@Undefined", SZ_INT, NULL))) // size = SZ_INT to enable declarations
  GWI_BB(gwi_add_type(gwi, t_undefined))
  GWI_OB((t_auto = gwi_mk_type(gwi, "auto", SZ_INT, NULL))) // size = SZ_INT to enable declarations
  GWI_BB(gwi_add_type(gwi, t_auto))
  SET_FLAG(t_class, abstract);
  const Type t_void  = gwi_mk_type(gwi, "void", 0, NULL);
  GWI_BB(gwi_set_global_type(gwi, t_void, et_void))
  GWI_OB((t_null  = gwi_mk_type(gwi, "@null",  SZ_INT, NULL)))
  GWI_BB(gwi_add_type(gwi, t_null))
  GWI_OB((t_function = gwi_mk_type(gwi, "@function", SZ_INT, NULL)))
  GWI_BB(gwi_add_type(gwi, t_function))
  GWI_OB((t_fptr = gwi_mk_type(gwi, "@func_ptr", SZ_INT, t_function)))
  GWI_BB(gwi_add_type(gwi, t_fptr))
  GWI_OB((t_lambda = gwi_mk_type(gwi, "@lambda", SZ_INT, t_function)))
  GWI_BB(gwi_add_type(gwi, t_lambda))
  GWI_OB((t_gack = gwi_mk_type(gwi, "@Gack", SZ_INT, NULL)))
  GWI_BB(gwi_add_type(gwi, t_gack))
  const Type t_int = gwi_mk_type(gwi, "int", SZ_INT, NULL);
  GWI_BB(gwi_set_global_type(gwi, t_int, et_int))
  const Type t_float = gwi_mk_type(gwi, "float", SZ_FLOAT, NULL);
  GWI_BB(gwi_set_global_type(gwi, t_float, et_float))
  const Type t_dur = gwi_mk_type(gwi, "dur", SZ_FLOAT, NULL);
  GWI_BB(gwi_add_type(gwi, t_dur))
  const Type t_time = gwi_mk_type(gwi, "time", SZ_FLOAT, NULL);
  GWI_BB(gwi_add_type(gwi, t_time))
  const Type t_now = gwi_mk_type(gwi, "@now", SZ_FLOAT, t_time);
  GWI_BB(gwi_add_type(gwi, t_now))
  struct SpecialId_ spid = { .type=t_now, .exec=RegPushNow, .is_const=1 };
  gwi_specialid(gwi, "now", &spid);
  gwi_reserve(gwi, "now");
  const Type t_complex = gwi_mk_type(gwi, "complex", SZ_COMPLEX , NULL);
  gwi->gwion->type[et_complex] = t_complex;
  const Type t_polar   = gwi_mk_type(gwi, "polar", SZ_COMPLEX , NULL);
  gwi->gwion->type[et_polar] = t_polar;
  const Type t_vec3 = gwi_mk_type(gwi, "Vec3", SZ_VEC3, NULL);
  gwi->gwion->type[et_vec3] = t_vec3;
  const Type t_vec4 = gwi_mk_type(gwi, "Vec4", SZ_VEC4, NULL);
  gwi->gwion->type[et_vec4] = t_vec4;
  GWI_BB(import_object(gwi))
  GWI_OB((t_union = gwi_mk_type(gwi, "@Union", SZ_INT, t_object)))
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

  GWI_BB(gwi_oper_ini(gwi, "Class", "Class", "int"))
  GWI_BB(gwi_oper_end(gwi, "==", int_eq))
  GWI_BB(gwi_oper_end(gwi, "!=", int_neq))
  GWI_BB(gwi_oper_end(gwi, ">=", instr_class_ge))
  GWI_BB(gwi_oper_end(gwi, ">",  instr_class_gt))
  GWI_BB(gwi_oper_end(gwi, "<=", instr_class_le))
  GWI_BB(gwi_oper_end(gwi, "<",  instr_class_lt))

  register_freearg(gwi, Gack, freearg_gack);
  return GW_OK;
}

ANN m_bool type_engine_init(VM* vm, const Vector plug_dirs) {
  vm->gwion->env->name = "[builtin]";
  struct Gwi_ gwi;
  memset(&gwi, 0, sizeof(struct Gwi_));
  gwi.gwion = vm->gwion;
  gwi.loc = new_loc(vm->gwion->mp, 0);
  GWI_BB(import_core_libs(&gwi))
  vm->gwion->env->name = "[imported]";
  for(m_uint i = 0; i < vector_size(plug_dirs); ++i) {
    m_bool (*import)(Gwi) = (m_bool(*)(Gwi))vector_at(plug_dirs, i);
    if(import && import(&gwi) < 0)
      env_reset(gwi.gwion->env);
  }
  free_loc(vm->gwion->mp, gwi.loc);
  return GW_OK;
}
