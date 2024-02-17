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
#include "gack.h"
#include "traverse.h"

#define mk_class_instr(op, arg0, arg1, ...)                                    \
  static INSTR(instr_class_##op) {                                             \
    POP_REG(shred, SZ_INT);                                                    \
    const Type l                     = *(Type *)(shred->reg - SZ_INT);         \
    const Type r                     = *(Type *)(shred->reg);                  \
    *(m_uint *)(shred->reg - SZ_INT) = isa(arg0, arg1) __VA_ARGS__;            \
  }
mk_class_instr(ge, l, r) mk_class_instr(gt, l, r, &&l != r)
mk_class_instr(le, r, l) mk_class_instr(lt, r, l, &&l != r)


static OP_CHECK(opck_class_call) {
  const Exp_Binary *bin = (Exp_Binary *)data;
  Exp_Call    call = {.func = bin->rhs, .args = bin->lhs};
  Exp*         e    = exp_self(bin);
  e->exp_type      = ae_exp_call;
  memcpy(&e->d.exp_call, &call, sizeof(Exp_Call));
  return check_exp_call1(env, &e->d.exp_call) ?: env->gwion->type[et_error];
}

static OP_CHECK(opck_basic_ctor) {
  const Exp_Call *call = (Exp_Call *)data;
// change to *no know constructor for {+G}%s{0}*?
  ERR_N(call->func->loc, _("can't call a non-callable value"));
}
/*
static OP_EMIT(opem_implicit_class) {
  struct Implicit *imp = (struct Implicit*)data;
  const Type t = actual_type(emit->gwion, imp->e->type);
  emit_pushimm(emit, map_size(&t->nspc->info->value->map));
  return true;
}

static OP_CHECK(opck_implicit_class) {
  struct Implicit *imp = (struct Implicit*)data;
  const Type t = actual_type(env->gwion, imp->e->type);
  if(isa(t, env->gwion->type[et_enum])) return imp->e->type;
  return env->gwion->type[et_error];
}
*/
GWION_IMPORT(class) {

  gwidoc(gwi, "Operators class types.");
   GWI_B(gwi_oper_ini(gwi, "Class", "Class", "bool"))
   GWI_B(gwi_oper_end(gwi, "==", int_eq))
   GWI_B(gwi_oper_end(gwi, "!=", int_neq))
   GWI_B(gwi_oper_end(gwi, ">=", instr_class_ge))
   GWI_B(gwi_oper_end(gwi, ">", instr_class_gt))
   GWI_B(gwi_oper_end(gwi, "<=", instr_class_le))
   GWI_B(gwi_oper_end(gwi, "<", instr_class_lt))

  gwidoc(gwi, "Allow binary call to constructors.");
   GWI_B(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "Class", NULL))
   GWI_B(gwi_oper_add(gwi, opck_class_call))
   GWI_B(gwi_oper_end(gwi, "=>", NULL))

  gwidoc(gwi, "internal constructor operator.");
   GWI_B(gwi_oper_ini(gwi, NULL, (m_str)OP_ANY_TYPE, NULL))
   GWI_B(gwi_oper_add(gwi, opck_basic_ctor))
   GWI_B(gwi_oper_end(gwi, "call_type", NULL))
/*
  gwidoc(gwi, "Allow enum for array size");
   GWI_B(gwi_oper_ini(gwi, "Class", "int", NULL))
   GWI_B(gwi_oper_add(gwi, opck_implicit_class))
   GWI_B(gwi_oper_emi(gwi, opem_implicit_class))
   GWI_B(gwi_oper_end(gwi, "@implicit", NULL))
*/
  return true;
}
