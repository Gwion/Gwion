#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "instr.h"
#include "type.h"
#include "object.h"
#include "import.h"
#include "gwi.h"
#include "lang_private.h"
#include "emit.h"
#include "env.h"
#include "vm.h"
#include "gwion.h"
#include "operator.h"
#include "engine.h"

static FREEARG(freearg_switchini) {
  free_vector(((Gwion)gwion)->p, (Vector)instr->m_val);
  free_map(((Gwion)gwion)->p, (Map)instr->m_val2);
}

static FREEARG(freearg_switchbranch) {
  free_map(((Gwion)gwion)->p, (Map)instr->m_val2);
}

static FREEARG(freearg_gack) {
  const Vector v = (Vector)instr->m_val2;
  for(m_uint i = vector_size(v) + 1; --i;)
    REM_REF(((Type)vector_at(v, i - 1)), gwion);
  free_vector(((Gwion)gwion)->p, v);
}

ANN static m_bool import_core_libs(const Gwi gwi) {
  CHECK_OB((t_undefined = gwi_mk_type(gwi, "@Undefined", SZ_INT, NULL))) // size = SZ_INT to enable declarations
  CHECK_OB((t_class = gwi_mk_type(gwi, "Class", SZ_INT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_class))
  CHECK_OB((t_auto = gwi_mk_type(gwi, "auto", SZ_INT, NULL))) // size = SZ_INT to enable declarations
  CHECK_BB(gwi_add_type(gwi, t_auto))
  SET_FLAG(t_class, abstract);
  CHECK_OB((t_void  = gwi_mk_type(gwi, "void", 0, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_void))
  CHECK_OB((t_null  = gwi_mk_type(gwi, "@null",  SZ_INT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_null))
  CHECK_OB((t_function = gwi_mk_type(gwi, "@function", SZ_INT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_function))
  CHECK_OB((t_fptr = gwi_mk_type(gwi, "@func_ptr", SZ_INT, t_function)))
  CHECK_BB(gwi_add_type(gwi, t_fptr))
  CHECK_OB((t_lambda = gwi_mk_type(gwi, "@lambda", SZ_INT, t_function)))
  CHECK_BB(gwi_add_type(gwi, t_fptr))
  CHECK_OB((t_gack = gwi_mk_type(gwi, "@Gack", SZ_INT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_gack))
  CHECK_OB((t_int = gwi_mk_type(gwi, "int", SZ_INT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_int))
  CHECK_OB((t_float = gwi_mk_type(gwi, "float", SZ_FLOAT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_float))
  CHECK_OB((t_dur = gwi_mk_type(gwi, "dur", SZ_FLOAT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_dur))
  CHECK_OB((t_time = gwi_mk_type(gwi, "time", SZ_FLOAT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_time))
  CHECK_OB((t_now = gwi_mk_type(gwi, "@now", SZ_FLOAT, t_time)))
  CHECK_BB(gwi_add_type(gwi, t_now))
  CHECK_OB((t_complex = gwi_mk_type(gwi, "complex", SZ_COMPLEX , NULL)))
  CHECK_OB((t_polar   = gwi_mk_type(gwi, "polar", SZ_COMPLEX , NULL)))
  CHECK_OB((t_vec3 = gwi_mk_type(gwi, "Vec3", SZ_VEC3, NULL)))
  CHECK_OB((t_vec4 = gwi_mk_type(gwi, "Vec4", SZ_VEC4, NULL)))
  CHECK_BB(import_object(gwi))
  CHECK_BB(import_array(gwi))
  CHECK_OB((t_union = gwi_mk_type(gwi, "@Union", SZ_INT, t_object)))
  CHECK_BB(gwi_add_type(gwi, t_union))
  CHECK_BB(import_event(gwi))
  CHECK_BB(import_ugen(gwi))
  CHECK_BB(import_ptr(gwi))
  CHECK_BB(import_func(gwi))
  CHECK_BB(gwi_oper_ini(gwi, NULL, (m_str)OP_ANY_TYPE, NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_new))
  CHECK_BB(gwi_oper_emi(gwi, opem_new))
  CHECK_BB(gwi_oper_end(gwi, op_new, NULL))
  CHECK_BB(import_prim(gwi))
  CHECK_BB(import_complex(gwi))
  CHECK_BB(import_vec3(gwi))
  CHECK_BB(import_vec4(gwi))
  CHECK_BB(import_vararg(gwi))
  CHECK_BB(import_string(gwi))
  CHECK_BB(import_shred(gwi))
  CHECK_BB(import_modules(gwi))
  register_freearg(gwi, SwitchIni, freearg_switchini);
  register_freearg(gwi, SwitchBranch, freearg_switchbranch);
  register_freearg(gwi, Gack, freearg_gack);
  return GW_OK;
}

ANN m_bool type_engine_init(VM* vm, const Vector plug_dirs) {
  vm->gwion->env->name = "[builtin]";
  struct Gwi_ gwi;
  memset(&gwi, 0, sizeof(struct Gwi_));
  gwi.gwion = vm->gwion;
  CHECK_BB(import_core_libs(&gwi))
  vm->gwion->env->name = "[imported]";
  for(m_uint i = 0; i < vector_size(plug_dirs); ++i) {
    m_bool (*import)(Gwi) = (m_bool(*)(Gwi))vector_at(plug_dirs, i);
    if(import && import(&gwi) < 0)
      env_reset(gwi.gwion->env);
  }
  return GW_OK;
}