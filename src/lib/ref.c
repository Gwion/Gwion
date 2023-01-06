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

ANN Type ref_type(const Gwion gwion, const Type t, const loc_t loc) {
  const m_str name = type2str(gwion, t, loc);
  char c[7 + strlen(name)];
  sprintf(c, "Ref:[%s]", name);
  return str2type(gwion, c, loc);
}

static m_bool ref_access(const Env env, const Exp e) {
  const m_str access = exp_access(e);
  if (!access) return GW_OK;
  env_err(env, e->pos, _("operand is %s"), access);
  return GW_ERROR;
}

static OP_CHECK(opck_ref_implicit_similar) {
  const struct Implicit *imp = (struct Implicit *)data;
  CHECK_BN(ref_access(env, imp->e));
  exp_setvar(imp->e, 1);
  return imp->t;
}

static inline Type ref_base(Type t) {
  do if(!tflag(t->info->parent, tflag_ref))
    return t;
  while((t = t->info->parent));
  return NULL;
}

static OP_EMIT(opem_ref_implicit_similar) {
  const struct Implicit *imp = (struct Implicit *)data;
  if(!tflag(imp->t, tflag_contract)) return GW_OK;
  const Env env = emit->env;
  const Type base = ref_base(imp->t);
  struct Op_Import opi    = {.op   = insert_symbol("@implicit"),
                             .lhs  = base,
                             .rhs  = imp->t,
                             .data = (m_uint)imp};
  CHECK_BB(op_emit(emit, &opi));
  emit_regmove(emit, -imp->e->type->size);
  exp_setvar(imp->e, true);
  imp->e->cast_to = NULL;
  return emit_exp(emit, imp->e);
}

static OP_CHECK(opck_implicit_ref) {
  const struct Implicit *imp = (struct Implicit *)data;
  CHECK_BN(ref_access(env, imp->e));
  exp_setvar(imp->e, 1);
  imp->e->cast_to = imp->t;
  return imp->t;
}

static OP_CHECK(opck_ref_contract_similar) {
  const struct Implicit *imp = (struct Implicit *)data;
  CHECK_BN(ref_access(env, imp->e));
  const Type base = (Type)vector_front(&imp->t->info->tuple->contains);
  struct Op_Import opi    = {.op   = insert_symbol("@implicit"),
                          .lhs  = imp->e->type,
                          .rhs  = base,
                          .data = (m_uint)imp};
  return op_check(env, &opi);
}

static OP_EMIT(opem_ref_contract_similar) {
  const struct Implicit *imp = (struct Implicit *)data;
  const Env env = emit->env;
  const Type base = (Type)vector_front(&imp->t->info->tuple->contains);
  struct Exp_ cast = {.type=base, .d={.exp_cast={.exp=imp->e}}};
  struct Op_Import opi    = {.op   = insert_symbol("$"),
                          .lhs  = imp->e->type,
                          .rhs  = base,
                          .data = (m_uint)&cast};
  CHECK_BB(op_emit(emit, &opi));
  emit_regmove(emit, -imp->e->type->size);
  exp_setvar(imp->e, true);
  imp->e->cast_to = NULL;
  return emit_exp(emit, imp->e);
}

ANN static void base2ref(Env env, const Type lhs, const Type rhs) {
  struct Op_Func   opfunc = {.ck = opck_similar_cast};
  struct Op_Import opi    = {.op   = insert_symbol("$"),
                          .lhs  = lhs,
                          .ret  = rhs,
                          .rhs  = rhs,
                          .func = &opfunc,
                          .data = eNoOp};
  add_op(env->gwion, &opi);
  opfunc.ck = opck_ref_implicit_similar;
  opfunc.em = opem_ref_implicit_similar;
  opi.op    = insert_symbol("@implicit");
  add_op(env->gwion, &opi);

  if(tflag(lhs, tflag_contract)) {
    opi.lhs = lhs->info->base_type;
    opfunc.ck = opck_ref_contract_similar;
    opfunc.em = opem_ref_contract_similar;
    opi.op    = insert_symbol("@implicit");
    add_op(env->gwion, &opi);
  }

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

ANN static void ref2ref(Env env, const Type lhs, const Type rhs) {
  struct Op_Func   opfunc = {.ck = opck_implicit_ref };
  struct Op_Import opi = {.op                      = insert_symbol("$"),
                          .lhs                     = lhs,
                          .ret                     = rhs,
                          .rhs                     = rhs,
                          .func=&opfunc, .data = eNoOp};
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
  const Type base = known_type(env, *mp_vector_at(ts->td->types, Type_Decl*, 0));
  const Type t    = new_type(env->gwion->mp, s_name(info.name), base);
  t->size = SZ_INT;
  SET_FLAG(t, abstract | ae_flag_final);
  set_tflag(t, tflag_infer);
  set_tflag(t, tflag_noret);
  set_tflag(t, tflag_scan0);
  set_tflag(t, tflag_scan1);
  set_tflag(t, tflag_scan2);
  set_tflag(t, tflag_check);
  set_tflag(t, tflag_emit);
  set_tflag(t, tflag_ref);
  const m_uint scope = env_push(env, base->info->value->from->owner_class,
                                base->info->value->from->owner);
  mk_class(env, t, (loc_t) {});
  base2ref(env, base, t);
  ref2base(env, t, base);
  ref2ref(env, t, t);
  env_pop(env, scope);
  t->info->tuple = new_tupleform(env->gwion->mp, base);
  type_addref(base);
  vector_add(&t->info->tuple->contains, (vtype)base);
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
  t_foreach->nspc->offset += SZ_INT;

  GWI_BB(gwi_struct_end(gwi))

  gwidoc(gwi, "internal `Ref` type creation.");
  GWI_BB(gwi_oper_ini(gwi, "Ref", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_ref_scan))
  GWI_BB(gwi_oper_end(gwi, "class", NULL))
  return GW_OK;
}
