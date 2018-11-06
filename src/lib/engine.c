#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "err_msg.h"
#include "type.h"
#include "value.h"
#include "object.h"
#include "import.h"
#include "gwi.h"
#include "lang_private.h"
#include "emit.h"

OP_CHECK(opck_basic_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  return cast->self->type;
}

OP_EMIT(opem_basic_cast) {
  return 1;
}

ANN static m_bool import_core_libs(const Gwi gwi) {
  CHECK_OB((t_class = gwi_mk_type(gwi, "@Class", SZ_INT, NULL)))
  CHECK_OB((t_void  = gwi_mk_type(gwi, "void", 0, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_void))
  CHECK_OB((t_null  = gwi_mk_type(gwi, "@null",  SZ_INT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_null))
  CHECK_OB((t_function = gwi_mk_type(gwi, "@function", SZ_INT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_function))
  CHECK_OB((t_fptr = gwi_mk_type(gwi, "@func_ptr", SZ_INT, t_function)))
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
  CHECK_BB(import_int(gwi))
  CHECK_BB(import_float(gwi))
  CHECK_BB(import_complex(gwi))
  CHECK_BB(import_vec3(gwi))
  CHECK_BB(import_vec4(gwi))
  CHECK_BB(import_vararg(gwi))
  CHECK_BB(import_string(gwi))
  CHECK_BB(import_shred(gwi))
  return 1;
}

ANN Env type_engine_init(VM* vm, const Vector plug_dirs) {
  const Env env = new_env();
  vm->emit->env = env;
  vm->emit->filename = "[builtin]";
  struct Gwi_ gwi;
  memset(&gwi, 0, sizeof(struct Gwi_));
  gwi.vm = vm;
  gwi.emit = vm->emit;
  gwi.env = env;
  if(import_core_libs(&gwi) < 0) {
    free_env(env);
    return NULL;
  }
  vm->emit->filename = "[imported]";
  for(m_uint i = 0; i < vector_size(plug_dirs); ++i) {
    m_bool (*import)(Gwi) = (m_bool(*)(Gwi))vector_at(plug_dirs, i);
    if(import && import(&gwi) < 0)
      env_reset(gwi.env);
  }
  return env;
}
