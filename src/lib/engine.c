#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "emit.h"
#include "vm.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "engine.h"
#include "parser.h"
#include "lang_private.h"
#include "specialid.h"
#include "gack.h"

static GACK(gack_class) {
  const Type type = actual_type(shred->info->vm->gwion, t) ?: t;
  INTERP_PRINTF("class(%s)", type->name)
}

static GACK(gack_function) {
  INTERP_PRINTF("%s", t->name)
}

static GACK(gack_fptr) {
  const VM_Code code = *(VM_Code*)VALUE;
  if(code)
    INTERP_PRINTF("%s", code->name)
  else
    INTERP_PRINTF("%s", t->name)
}

static GACK(gack_void) {
 INTERP_PRINTF("void");
}

static GACK(gack_int) {
  INTERP_PRINTF("%"INT_F, *(m_uint*)VALUE);
}

static GACK(gack_char) {
  INTERP_PRINTF("%c", *(char*)VALUE);
}

static GACK(gack_float) {
  INTERP_PRINTF("%.4f", *(m_float*)VALUE);
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

OP_CHECK(opck_object_dot);
OP_EMIT(opem_object_dot);
ANN static m_bool import_core_libs(const Gwi gwi) {
  const Type t_class = gwi_mk_type(gwi, "@Class", SZ_INT, NULL);
  gwi->gwion->type[et_class] = t_class;
  GWI_BB(gwi_add_type(gwi, t_class))
  GWI_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_object_dot))
  GWI_BB(gwi_oper_emi(gwi, opem_object_dot))
  GWI_BB(gwi_oper_end(gwi, "@dot", NULL))
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

  GWI_BB(gwi_typedef_ini(gwi, "int", "@internal"))
  GWI_BB(gwi_typedef_end(gwi, ae_flag_none))

  GWI_BB(import_object(gwi))

// TODO: check me
  const Type t_union = gwi_class_ini(gwi, "@Union", NULL);
  gwi->gwion->type[et_union] = t_union;
  GWI_BB(gwi_class_end(gwi))

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

ANN m_bool type_engine_init(const Gwion gwion, const Vector plug_dirs) {
  gwion->env->name = "[builtin]";
  struct loc_t loc = {};
  OperCK oper = {};
  struct Gwi_ gwi = { .gwion=gwion, .loc=&loc, .oper=&oper };
  CHECK_BB(import_core_libs(&gwi))
  push_global(gwion, "[plugins]");
  gwion->env->name = "[imported]";
  for(m_uint i = 0; i < vector_size(plug_dirs); ++i) {
    m_bool (*import)(Gwi) = (m_bool(*)(Gwi))vector_at(plug_dirs, i);
    if(import && import(&gwi) < 0)
      gwi_reset(&gwi);
  }
  push_global(gwion, "[user]");
  return GW_OK;
}
