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
#include "lang_private.h"
#include "specialid.h"
#include "gack.h"

#undef insert_symbol
static GACK(gack_none) {
  INTERP_PRINTF("None")
}

static GACK(gack_class) {
  const Type type = actual_type(shred->info->vm->gwion, t) ?: t;
  INTERP_PRINTF("class(%s)", type->name)
}

static GACK(gack_function) {
  INTERP_PRINTF("%s", t->name)
}

static GACK(gack_gack) {
  INTERP_PRINTF("%s", *(m_str*)VALUE)
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

static GACK(gack_compound) {
  INTERP_PRINTF("%p", *(void**)VALUE);
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

static OP_CHECK(opck_basic_ctor) {
  const Exp_Call* call = (Exp_Call*)data;
  ERR_N(exp_self(call)->pos, _("can't call a non-callable value"))
}

static OP_CHECK(opck_any_at_union) {
  Exp_Binary *bin = (Exp_Binary*)data;
  CHECK_NN(opck_rassign(env, data, mut)) // check those two lines
  exp_setvar(bin->rhs, 0);
  const Type lhs = bin->lhs->info->type;
  const Nspc nspc = bin->rhs->info->type->nspc;
  for(m_uint i = 0; i < nspc->info->class_data_size; i += SZ_INT) {
    if(lhs == *(Type*)(nspc->info->class_data + i))
      return lhs;
  }
  return env->gwion->type[et_error]; // err_msg
}

static INSTR(UnionSet) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(0);
  *(m_uint*)(o->data) = instr->m_val2;
  memcpy(o->data + SZ_INT, REG(-instr->m_val), instr->m_val);
}

static OP_EMIT(opem_any_at_union) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Type lhs = bin->lhs->info->type;
  const Nspc nspc = bin->rhs->info->type->nspc;
  const Instr instr = emit_add_instr(emit, UnionSet);
  for(m_uint i = 0; i < nspc->info->class_data_size; i += SZ_INT) {
    if(lhs == *(Type*)(nspc->info->class_data + i)) {
      instr->m_val2 = i + 1;
      instr->m_val = lhs->size;
      return instr;
    }
  }
  return NULL;
}

static OP_CHECK(opck_union_at_any) {
  Exp_Binary *bin = (Exp_Binary*)data;
  CHECK_NN(opck_rassign(env, data, mut)) // check those two lines
  const Type rhs = bin->rhs->info->type;
  const Nspc nspc = bin->lhs->info->type->nspc;
  for(m_uint i = 0; i < nspc->info->class_data_size; i += SZ_INT) {
    if(rhs == *(Type*)(nspc->info->class_data + i))
      return rhs;
  }
  return env->gwion->type[et_error]; // err_msg
}

static INSTR(UnionGet) {
  const M_Object o = *(M_Object*)REG(-SZ_INT*2);
  if(*(m_uint*)(o->data) != instr->m_val2)
    Except(shred, "invalid union access");
  memcpy(*(m_bit**)REG(-SZ_INT), o->data + SZ_INT, instr->m_val);
  POP_REG(shred, SZ_INT*2 - instr->m_val);
  memcpy(REG(-instr->m_val), o->data + SZ_INT, instr->m_val);
}

static OP_EMIT(opem_union_at_any) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Type rhs = bin->rhs->info->type;
  const Nspc nspc = bin->lhs->info->type->nspc;
  const Instr instr = emit_add_instr(emit, UnionGet);
  for(m_uint i = 0; i < nspc->info->class_data_size; i += SZ_INT) {
    if(rhs == *(Type*)(nspc->info->class_data + i)) {
      instr->m_val2 = i + 1;
      instr->m_val = rhs->size;
      return instr;
    }
  }
  return NULL;
}

static OP_CHECK(opck_union_eq_class) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Type rhs = bin->rhs->info->type->info->base_type;
  const Nspc nspc = bin->lhs->info->type->nspc;
  for(m_uint i = 0; i < nspc->info->class_data_size; i += SZ_INT) {
    if(rhs == *(Type*)(nspc->info->class_data + i))
      return env->gwion->type[et_bool];
  }
  return NULL; // err_msg
}

static INSTR(UnionEqClass) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  *(m_uint*)REG(-SZ_INT) = *(m_uint*)(o->data) == instr->m_val2;

}

static OP_EMIT(opem_union_eq_class) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Type rhs = bin->rhs->info->type->info->base_type;
  const Nspc nspc = bin->lhs->info->type->nspc;
  const Instr instr = emit_add_instr(emit, UnionEqClass);
  for(m_uint i = 0; i < nspc->info->class_data_size; i += SZ_INT) {
    if(rhs == *(Type*)(nspc->info->class_data + i)) {
      instr->m_val2 = i + 1;
      return instr;
    }
  }
  return NULL;
}

DTOR(UnionDtor) {
  const m_uint idx = *(m_uint*)o->data;
  if(idx) {
    const Type t = *(Type*)(o->type_ref->nspc->info->class_data + idx * SZ_INT);
    if(isa(t, shred->info->vm->gwion->type[et_compound]) > 0)
      compound_release(shred, t, *(m_bit**)(o->data + SZ_INT));
  }
}

static ID_CHECK(idck_none) {
  struct loc_t_ loc = {};
  return str2type(env->gwion, "None", &loc);
}

static ID_EMIT(idem_none) {
  return (Instr)1;
}

ANN static m_bool import_core_libs(const Gwi gwi) {
  const Type t_class = gwi_mk_type(gwi, "@Class", SZ_INT, NULL);
  set_tflag(t_class, tflag_infer);
  GWI_BB(gwi_set_global_type(gwi, t_class, et_class))
  GWI_BB(gwi_gack(gwi, t_class, gack_class))

  const Type t_undefined = gwi_mk_type(gwi, "@Undefined", SZ_INT, NULL);
  GWI_BB(gwi_set_global_type(gwi, t_undefined, et_undefined))
  const Type t_auto = gwi_mk_type(gwi, "auto", SZ_INT, NULL);
  set_tflag(t_auto, tflag_infer);
  GWI_BB(gwi_set_global_type(gwi, t_auto, et_auto))
  const Type t_void  = gwi_mk_type(gwi, "void", 0, NULL);
  GWI_BB(gwi_gack(gwi, t_void, gack_void))
  GWI_BB(gwi_set_global_type(gwi, t_void, et_void))
  const Type t_gack = gwi_mk_type(gwi, "@Gack", SZ_INT, NULL);
  GWI_BB(gwi_gack(gwi, t_gack, gack_gack))
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

  const Type t_compound = gwi_mk_type(gwi, "@Compound", 0, NULL);
  GWI_BB(gwi_gack(gwi, t_compound, gack_compound))
  GWI_BB(gwi_set_global_type(gwi, t_compound, et_compound))

  const Type t_none = gwi_mk_type(gwi, "None", 0, NULL);
  GWI_BB(gwi_gack(gwi, t_none, gack_none))
  gwi_add_type(gwi, t_none);
{
  struct SpecialId_ spid = { .ck=idck_none, .em=idem_none, .is_const=1 };
  gwi_specialid(gwi, "None", &spid);
}

  GWI_BB(import_object(gwi))

  const Type t_union = gwi_class_ini(gwi, "@Union", "Object");
  gwi_class_xtor(gwi, NULL, UnionDtor);
  GWI_BB(gwi_item_ini(gwi, "int", "@index"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, NULL))
  GWI_BB(gwi_class_end(gwi))
  GWI_BB(gwi_gack(gwi, t_union, gack_compound))
  gwi->gwion->type[et_union] = t_union;

  GWI_BB(import_prim(gwi))
  const Type t_function = gwi_mk_type(gwi, "@function", SZ_INT, NULL);
  GWI_BB(gwi_gack(gwi, t_function, gack_function))
  GWI_BB(gwi_set_global_type(gwi, t_function, et_function))
  const Type t_fptr = gwi_mk_type(gwi, "@func_ptr", SZ_INT, "@function");
  GWI_BB(gwi_gack(gwi, t_fptr, gack_fptr))
  GWI_BB(gwi_set_global_type(gwi, t_fptr, et_fptr))
  const Type t_lambda = gwi_mk_type(gwi, "@lambda", SZ_INT, "@function");
  set_tflag(t_lambda, tflag_infer);
  GWI_BB(gwi_set_global_type(gwi, t_lambda, et_lambda))

  GWI_BB(gwi_typedef_ini(gwi, "int", "@internal"))
  GWI_BB(gwi_typedef_end(gwi, ae_flag_none))

  GWI_BB(import_object_op(gwi))
  GWI_BB(import_values(gwi))

  GWI_BB(import_array(gwi))
  GWI_BB(import_event(gwi))
  GWI_BB(import_ugen(gwi))
  GWI_BB(import_ptr(gwi))
  GWI_BB(import_func(gwi))
  GWI_BB(gwi_oper_ini(gwi, NULL, (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_new))
  GWI_BB(gwi_oper_emi(gwi, opem_new))
  GWI_BB(gwi_oper_end(gwi, "new", NULL))
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

  GWI_BB(gwi_oper_ini(gwi, NULL, (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_basic_ctor))
  GWI_BB(gwi_oper_end(gwi, "@ctor", NULL))

  GWI_BB(gwi_oper_ini(gwi, "@Compound", (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_object_dot))
  GWI_BB(gwi_oper_emi(gwi, opem_object_dot))
  GWI_BB(gwi_oper_end(gwi, "@dot", NULL))

  GWI_BB(gwi_oper_ini(gwi, "@Class", (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_object_dot))
  GWI_BB(gwi_oper_emi(gwi, opem_object_dot))
  GWI_BB(gwi_oper_end(gwi, "@dot", NULL))

  GWI_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "@Union", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_any_at_union))
  GWI_BB(gwi_oper_emi(gwi, opem_any_at_union))
  GWI_BB(gwi_oper_end(gwi, "?=>", NULL))

  GWI_BB(gwi_oper_ini(gwi, "@Union", (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_union_at_any))
  GWI_BB(gwi_oper_emi(gwi, opem_union_at_any))
  GWI_BB(gwi_oper_end(gwi, "?=>", NULL))

  GWI_BB(gwi_oper_ini(gwi, "@Union", "@Class", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_union_eq_class))
  GWI_BB(gwi_oper_emi(gwi, opem_union_eq_class))
  GWI_BB(gwi_oper_end(gwi, "==", NULL))

  GWI_BB(gwi_union_ini(gwi, "Option:[A]"))
  GWI_BB(gwi_union_add(gwi, "None"))
  GWI_BB(gwi_union_add(gwi, "A"))
  GWI_BB(gwi_union_end(gwi, ae_flag_none))

  return GW_OK;
}

ANN m_bool type_engine_init(const Gwion gwion) {
  gwion->env->name = "[builtin]";
  CHECK_BB(gwi_run(gwion, import_core_libs))
  push_global(gwion, "[user]");
  return GW_OK;
}
