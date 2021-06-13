#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "emit.h"
#include "traverse.h"
#include "template.h"
#include "parse.h"
#include "specialid.h"
#include "gwi.h"
#include "tmpl_info.h"

static m_bool ref_access(const Env env, const Exp e) {
  const m_str access = exp_access(e);
  if (!access) return GW_OK;
  env_err(env, e->pos, _("operand is %s"), access);
  return GW_ERROR;
}

static OP_CHECK(opck_implicit_similar) {
  const struct Implicit *imp = (struct Implicit *)data;
  CHECK_BN(ref_access(env, imp->e));
  exp_setvar(imp->e, 1);
  return imp->t;
}

static OP_CHECK(opck_cast_similar) {
  const Exp_Cast *cast = (Exp_Cast *)data;
  return exp_self(cast)->type;
}

ANN static void base2ref(Env env, const Type lhs, const Type rhs) {
  struct Op_Func   opfunc = {.ck = opck_cast_similar};
  struct Op_Import opi    = {.op   = insert_symbol("$"),
                          .lhs  = lhs,
                          .ret  = rhs,
                          .rhs  = rhs,
                          .func = &opfunc,
                          .data = eNoOp};
  add_op(env->gwion, &opi);
  opfunc.ck = opck_implicit_similar;
  opi.op    = insert_symbol("@implicit");
  add_op(env->gwion, &opi);
}

ANN static void ref2base(Env env, const Type lhs, const Type rhs) {
  struct Op_Import opi = {.op                      = insert_symbol("$"),
                          .lhs                     = lhs,
                          .ret                     = rhs,
                          .rhs                     = rhs,
                          /*.func=&opfunc,*/ .data = eNoOp};
  add_op(env->gwion, &opi);
  opi.op = insert_symbol("@implicit");
  add_op(env->gwion, &opi);
}

static OP_CHECK(opck_ref_scan) {
  struct TemplateScan *ts   = (struct TemplateScan *)data;
  struct tmpl_info     info = {
      .base = ts->t, .td = ts->td, .list = ts->t->info->cdef->base.tmpl->list};
  const Type exists = tmpl_exists(env, &info);
  if (exists) return exists != env->gwion->type[et_error] ? exists : NULL;
  const Type base = known_type(env, ts->td->types->td);
  const Type t    = new_type(env->gwion->mp, s_name(info.name), base);
  SET_FLAG(t, abstract | ae_flag_final);
  set_tflag(t, tflag_infer);
  set_tflag(t, tflag_noret);
  const m_uint scope = env_push(env, base->info->value->from->owner_class,
                                base->info->value->from->owner);
  mk_class(env, t, (loc_t) {});
  base2ref(env, base, t);
  ref2base(env, t, base);
  env_pop(env, scope);
  nspc_add_type_front(t->info->value->from->owner, info.name, t);
  return t;
}

GWION_IMPORT(ref) {
  gwidoc(gwi, "Ref: take a reference from a variable.");
  gwinote(gwi, "used just as the variable it reference.");
  gwinote(gwi, "can only be used as argument.");
  gwinote(gwi, "and cannot be returned.");
  const Type t_foreach = gwi_struct_ini(gwi, "Ref:[A]");
  set_tflag(t_foreach, tflag_infer);

  gwinote(gwi, "a pointer to the referenced variable.");
  GWI_BB(gwi_item_ini(gwi, "@internal", "val"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))

  GWI_BB(gwi_struct_end(gwi))

  gwidoc(gwi, "internal `Ref` type creation.");
  GWI_BB(gwi_oper_ini(gwi, "Ref", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_ref_scan))
  GWI_BB(gwi_oper_end(gwi, "@scan", NULL))
  return GW_OK;
}
