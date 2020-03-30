#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "emit.h"
#include "traverse.h"
#include "parse.h"
#include "operator.h"
#include "import.h"

OP_CHECK(opck_basic_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  return isa(cast->exp->info->type, exp_self(cast)->info->type) > 0 ?
     exp_self(cast)->info->type : env->gwion->type[et_null];
}

OP_CHECK(opck_simple_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  return cast->exp->info->cast_to =  exp_self(cast)->info->type;
}

OP_CHECK(opck_usr_implicit) {
  struct Implicit* imp = (struct Implicit*)data;
  imp->e->info->cast_to = imp->t;
  return imp->t;
}

OP_CHECK(opck_const_rhs) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  const m_str access = exp_access(bin->rhs);
  if(access)
    ERR_N(bin->rhs->pos, _("cannot assign '%s' on types '%s' and '%s'.\n"
         "  ...  (reason: --- right-side operand is %s.)"),
         s_name(bin->op), bin->lhs->info->type->name, bin->rhs->info->type->name,
         access)
  return bin->rhs->info->type;
}

OP_CHECK(opck_rhs_emit_var) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  exp_setvar(bin->rhs, 1);
  return bin->rhs->info->type;
}

OP_CHECK(opck_rassign) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  if(opck_const_rhs(env, data, mut) == env->gwion->type[et_null])
    return env->gwion->type[et_null];
  exp_setvar(bin->rhs, 1);
  return bin->rhs->info->type;
}

OP_CHECK(opck_unary_meta) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  exp_setmeta(exp_self(unary), 1);
  return unary->exp->info->type;
}

OP_CHECK(opck_unary_meta2) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  exp_setmeta(exp_self(unary), 1);
  return env->gwion->type[et_bool];
}

OP_CHECK(opck_unary) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  const m_str access = exp_access(unary->exp);
  if(access)
    ERR_N(unary->exp->pos,
          _("unary operator '%s' cannot be used on %s data-types."),
          s_name(unary->op), access);
  exp_setvar(unary->exp, 1);
  return unary->exp->info->type;
}

OP_CHECK(opck_post) {
  const Exp_Postfix* post = (Exp_Postfix*)data;
  const m_str access = exp_access(post->exp);
  if(access)
    ERR_N(post->exp->pos, _("post operator '%s' cannot be used on %s data-type."),
          s_name(post->op), access)
  exp_setvar(post->exp, 1);
  return post->exp->info->type;
}

OP_CHECK(opck_new) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  SET_FLAG(unary->td, ref);
  DECL_ON(const Type, t, = known_type(env, unary->td))
  if(isa(t, env->gwion->type[et_object]) < 0 && isa(t, env->gwion->type[et_function]) < 0)
    ERR_N(exp_self(unary)->pos, _("primitive types cannot be used as reference (@)...\n"))
  if(type_ref(t))
    ERR_N(td_pos(unary->td), _("can't use 'new' on ref type '%s'\n"), t->name)
  if(GET_FLAG(t, abstract))
    ERR_N(td_pos(unary->td), _("can't use 'new' on abstract type '%s'\n"), t->name)
  UNSET_FLAG(unary->td, ref);
  if(unary->td->array)
    CHECK_BN(check_subscripts(env, unary->td->array, 1))
  return t;
}

OP_EMIT(opem_new) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  CHECK_BO(emit_instantiate_object(emit, exp_self(unary)->info->type,
    unary->td->array, GET_FLAG(unary->td, ref)))
  return emit_add_instr(emit, GcAdd);
}
