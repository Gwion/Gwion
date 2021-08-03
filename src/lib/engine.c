#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "lang_private.h"
#include "specialid.h"
#include "gack.h"
#include "traverse.h"

static GACK(gack_class) {
  const Type type = actual_type(shred->info->vm->gwion, t) ?: t;
  INTERP_PRINTF("class(%s)", type->name)
}

static GACK(gack_function) { INTERP_PRINTF("%s", t->name) }

static GACK(gack_gack) { INTERP_PRINTF("%s", *(m_str *)VALUE) }

static GACK(gack_fptr) {
  const VM_Code code = *(VM_Code *)VALUE;
  if (code)
    INTERP_PRINTF("%s", code->name)
  else
    INTERP_PRINTF("%s", t->name)
}

static GACK(gack_void) { INTERP_PRINTF("void"); }

static GACK(gack_int) { INTERP_PRINTF("%" INT_F, *(m_uint *)VALUE); }

static GACK(gack_char) { INTERP_PRINTF("%c", *(char *)VALUE); }

static GACK(gack_float) { INTERP_PRINTF("%.4f", *(m_float *)VALUE); }

static GACK(gack_compound) { INTERP_PRINTF("%p", *(void **)VALUE); }
#define mk_class_instr(op, arg0, arg1, ...)                                    \
  static INSTR(instr_class_##op) {                                             \
    POP_REG(shred, SZ_INT);                                                    \
    const Type l                     = *(Type *)(shred->reg - SZ_INT);         \
    const Type r                     = *(Type *)(shred->reg);                  \
    *(m_uint *)(shred->reg - SZ_INT) = isa(arg0, arg1) > 0 __VA_ARGS__;        \
  }
mk_class_instr(ge, l, r) mk_class_instr(gt, l, r, &&l != r)
    mk_class_instr(le, r, l) mk_class_instr(lt, r, l, &&l != r)

        OP_CHECK(opck_object_dot);
OP_EMIT(opem_object_dot);

static OP_CHECK(opck_basic_ctor) {
  const Exp_Call *call = (Exp_Call *)data;
  ERR_N(exp_self(call)->pos, _("can't call a non-callable value"));
}

static OP_CHECK(opck_class_call) {
  const Exp_Binary *bin = (Exp_Binary *)data;
  Exp_Call    call = {.func = bin->rhs, .args = bin->lhs};
  Exp         e    = exp_self(bin);
  e->exp_type      = ae_exp_call;
  memcpy(&e->d.exp_call, &call, sizeof(Exp_Call));
  return check_exp_call1(env, &e->d.exp_call) ?: env->gwion->type[et_error];
}

static ID_CHECK(idck_predicate) {
  set_fflag(env->func, fflag_return);
  return exp_self(prim)->type;
}

static INSTR(PredicateCheck) {
  if (!*(m_uint *)REG(-SZ_INT)) handle(shred, "PredicateFail");
}

ANN static m_bool import_core_libs(const Gwi gwi) {
  gwidoc(gwi, "one type to rule them all.");
  const Type t_class = gwi_mk_type(gwi, "Class", SZ_INT, NULL);
  set_tflag(t_class, tflag_infer);
  GWI_BB(gwi_set_global_type(gwi, t_class, et_class))
  GWI_BB(gwi_gack(gwi, t_class, gack_class))

  gwidoc(gwi, "this type is infered.");
  const Type t_auto = gwi_mk_type(gwi, "auto", SZ_INT, NULL);
  set_tflag(t_auto, tflag_infer);
  GWI_BB(gwi_set_global_type(gwi, t_auto, et_auto))

  gwidoc(gwi, "a void type.");
  const Type t_void = gwi_mk_type(gwi, "void", 0, NULL);
  GWI_BB(gwi_gack(gwi, t_void, gack_void))
  GWI_BB(gwi_set_global_type(gwi, t_void, et_void))

  gwidoc(gwi, "a type for *pretty print*.");
  const Type t_gack = gwi_mk_type(gwi, "@Gack", SZ_INT, NULL);
  GWI_BB(gwi_gack(gwi, t_gack, gack_gack))
  GWI_BB(gwi_set_global_type(gwi, t_gack, et_gack))

  gwidoc(gwi, "integer type.");
  const Type t_int = gwi_mk_type(gwi, "int", SZ_INT, NULL);
  GWI_BB(gwi_gack(gwi, t_int, gack_int))
  GWI_BB(gwi_set_global_type(gwi, t_int, et_int))

  gwidoc(gwi, "character type.");
  const Type t_char = gwi_mk_type(gwi, "char", SZ_INT, "int");
  GWI_BB(gwi_gack(gwi, t_char, gack_char))
  GWI_BB(gwi_set_global_type(gwi, t_char, et_char))

  gwidoc(gwi, "float type.");
  const Type t_float = gwi_mk_type(gwi, "float", SZ_FLOAT, NULL);
  GWI_BB(gwi_gack(gwi, t_float, gack_float))
  GWI_BB(gwi_set_global_type(gwi, t_float, et_float))
  set_tflag(t_float, tflag_float);

  gwidoc(gwi, "represent duration.");
  const Type t_dur = gwi_mk_type(gwi, "dur", SZ_FLOAT, NULL);
  GWI_BB(gwi_gack(gwi, t_dur, gack_float))
  GWI_BB(gwi_add_type(gwi, t_dur))
  set_tflag(t_dur, tflag_float);

  gwidoc(gwi, "represent time.");
  const Type t_time = gwi_mk_type(gwi, "time", SZ_FLOAT, NULL);
  GWI_BB(gwi_gack(gwi, t_time, gack_float))
  GWI_BB(gwi_add_type(gwi, t_time))
  set_tflag(t_time, tflag_float);

  gwidoc(gwi, "internal time for `{/}now{0}{-}`.");
  const Type t_now = gwi_mk_type(gwi, "@now", SZ_FLOAT, "time");
  GWI_BB(gwi_add_type(gwi, t_now))
  struct SpecialId_ spid = {.type = t_now, .exec = RegPushNow, .is_const = 1};
  gwi_specialid(gwi, "now", &spid);

  gwidoc(gwi, "internal predicate representation.");
  struct SpecialId_ predicate = {.type     = t_void,
                                 .ck       = idck_predicate,
                                 .exec     = PredicateCheck,
                                 .is_const = 1};
  gwi_specialid(gwi, "@predicate", &predicate);

  gwidoc(gwi, "internal base of all objects and structures.");
  const Type t_compound = gwi_mk_type(gwi, "@Compound", SZ_INT, NULL);
  GWI_BB(gwi_gack(gwi, t_compound, gack_compound))
  GWI_BB(gwi_set_global_type(gwi, t_compound, et_compound))

  GWI_BB(import_object(gwi))

  GWI_BB(import_prim(gwi))
  gwidoc(gwi, "the base of all functions.");
  const Type t_function = gwi_mk_type(gwi, "@function", SZ_INT, NULL);
  GWI_BB(gwi_gack(gwi, t_function, gack_function))
  GWI_BB(gwi_set_global_type(gwi, t_function, et_function))

  gwidoc(gwi, "the base of function pointers.");
  const Type t_fptr = gwi_mk_type(gwi, "@func_ptr", SZ_INT, "@function");
  GWI_BB(gwi_gack(gwi, t_fptr, gack_fptr))
  GWI_BB(gwi_set_global_type(gwi, t_fptr, et_fptr))

  gwidoc(gwi, "the base of decayed operators.");
  const Type t_op = gwi_mk_type(gwi, "@op", SZ_INT, "@function");
  GWI_BB(gwi_set_global_type(gwi, t_op, et_op))

  gwidoc(gwi, "the base of lamdbas.");
  const Type t_lambda = gwi_mk_type(gwi, "@lambda", SZ_INT, "@function");
  set_tflag(t_lambda, tflag_infer);
  GWI_BB(gwi_set_global_type(gwi, t_lambda, et_lambda))

  gwidoc(gwi, "Mark function as curried.");
  const Type t_curry = gwi_mk_type(gwi, "@Curry", 0, NULL);
  GWI_BB(gwi_set_global_type(gwi, t_curry, et_curry))

  gwidoc(gwi, "type for internal pointer data.");
  GWI_BB(gwi_typedef_ini(gwi, "int", "@internal"))
  GWI_BB(gwi_typedef_end(gwi, ae_flag_none))

  GWI_BB(import_object_op(gwi))
  GWI_BB(import_values(gwi))
  GWI_BB(import_union(gwi))

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
  GWI_BB(import_ref(gwi))

  gwidoc(gwi, "Operators class types.");
  GWI_BB(gwi_oper_ini(gwi, "Class", "Class", "int"))
  GWI_BB(gwi_oper_end(gwi, "==", int_eq))
  GWI_BB(gwi_oper_end(gwi, "!=", int_neq))
  GWI_BB(gwi_oper_end(gwi, ">=", instr_class_ge))
  GWI_BB(gwi_oper_end(gwi, ">", instr_class_gt))
  GWI_BB(gwi_oper_end(gwi, "<=", instr_class_le))
  GWI_BB(gwi_oper_end(gwi, "<", instr_class_lt))

  gwidoc(gwi, "internal constructor operator.");
  GWI_BB(gwi_oper_ini(gwi, NULL, (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_basic_ctor))
  GWI_BB(gwi_oper_end(gwi, "@ctor", NULL))

  gwidoc(gwi, "allow member access.");
  GWI_BB(gwi_oper_ini(gwi, "@Compound", (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_object_dot))
  GWI_BB(gwi_oper_emi(gwi, opem_object_dot))
  GWI_BB(gwi_oper_end(gwi, "@dot", NULL))

  gwidoc(gwi, "allow static access.");
  GWI_BB(gwi_oper_ini(gwi, "Class", (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_object_dot))
  GWI_BB(gwi_oper_emi(gwi, opem_object_dot))
  GWI_BB(gwi_oper_end(gwi, "@dot", NULL))

  gwidoc(gwi, "Allow binary call to constructors.");
  GWI_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "Class", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_class_call))
  GWI_BB(gwi_oper_end(gwi, "=>", NULL))

gwi_enum_ini(gwi, "@hidden_enum");
gwi_enum_add(gwi, "@hidden_enum", 0);
gwi_enum_end(gwi);
  return GW_OK;
}

ANN m_bool type_engine_init(const Gwion gwion) {
  gwion->env->name = "[builtin]";
  CHECK_BB(gwi_run(gwion, import_core_libs));
  push_global(gwion, "[user]");
  return GW_OK;
}
