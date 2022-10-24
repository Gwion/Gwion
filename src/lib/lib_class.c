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
    *(m_uint *)(shred->reg - SZ_INT) = isa(arg0, arg1) > 0 __VA_ARGS__;        \
  }
mk_class_instr(ge, l, r) mk_class_instr(gt, l, r, &&l != r)
mk_class_instr(le, r, l) mk_class_instr(lt, r, l, &&l != r)


static OP_CHECK(opck_class_call) {
  const Exp_Binary *bin = (Exp_Binary *)data;
  Exp_Call    call = {.func = bin->rhs, .args = bin->lhs};
  Exp         e    = exp_self(bin);
  e->exp_type      = ae_exp_call;
  memcpy(&e->d.exp_call, &call, sizeof(Exp_Call));
  return check_exp_call1(env, &e->d.exp_call) ?: env->gwion->type[et_error];
}

static OP_CHECK(opck_basic_ctor) {
  const Exp_Call *call = (Exp_Call *)data;
// change to *no know constructor for {+G}%s{0}*?
  ERR_N(call->func->pos, _("can't call a non-callable value"));
}

static OP_EMIT(opem_implicit_class) {
  struct Implicit *imp = (struct Implicit*)data;
  const Type t = actual_type(emit->gwion, imp->e->type);
  emit_pushimm(emit, map_size(&t->nspc->info->value->map));
  return GW_OK; emit_exp(emit, imp->e);
}

static OP_CHECK(opck_implicit_class) {
  struct Implicit *imp = (struct Implicit*)data;
  const Type t = actual_type(env->gwion, imp->e->type);
  if(tflag(t, tflag_enum)) return env->gwion->type[et_int];
  return env->gwion->type[et_error];
}

GWION_IMPORT(class) {

  gwidoc(gwi, "Operators class types.");
  GWI_BB(gwi_oper_ini(gwi, "Class", "Class", "bool"))
  GWI_BB(gwi_oper_end(gwi, "==", int_eq))
  GWI_BB(gwi_oper_end(gwi, "!=", int_neq))
  GWI_BB(gwi_oper_end(gwi, ">=", instr_class_ge))
  GWI_BB(gwi_oper_end(gwi, ">", instr_class_gt))
  GWI_BB(gwi_oper_end(gwi, "<=", instr_class_le))
  GWI_BB(gwi_oper_end(gwi, "<", instr_class_lt))

  gwidoc(gwi, "Allow binary call to constructors.");
  GWI_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "Class", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_class_call))
  GWI_BB(gwi_oper_end(gwi, "=>", NULL))

  gwidoc(gwi, "internal constructor operator.");
  GWI_BB(gwi_oper_ini(gwi, NULL, (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_basic_ctor))
  GWI_BB(gwi_oper_end(gwi, "@ctor", NULL))

  gwidoc(gwi, "Allow enum for array size");
  GWI_BB(gwi_oper_ini(gwi, "Class", "int", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_implicit_class))
  GWI_BB(gwi_oper_emi(gwi, opem_implicit_class))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))

  return GW_OK;
}
