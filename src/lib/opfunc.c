#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "value.h"
#include "instr.h"
#include "object.h"
#include "import.h"
#include "emit.h"
#include "traverse.h"
#include "parse.h"

static inline m_str access(ae_Exp_Meta meta) {
  return meta == ae_meta_value ? "non-mutable" : "protected";
}

OP_CHECK(opck_basic_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  return exp_self(cast)->type;
}

OP_EMIT(opem_basic_cast) {
  return GW_OK;
}

OP_CHECK(opck_const_rhs) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  if(bin->rhs->meta != ae_meta_var)
    ERR_N(exp_self(bin)->pos, "cannot assign '%s' on types '%s' and '%s'.\n"
         "\t...\t(reason: --- right-side operand is %s.)",
         op2str(bin->op), bin->lhs->type->name, bin->rhs->type->name,
         access(bin->rhs->meta))
  return bin->rhs->type;
}

OP_CHECK(opck_rhs_emit_var) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  bin->rhs->emit_var = 1;
  return bin->rhs->type;
}

OP_CHECK(opck_rassign) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  if(opck_const_rhs(env, data) == t_null)
    return t_null;
  bin->rhs->emit_var = 1;
  return bin->rhs->type;
}

OP_CHECK(opck_unary_meta) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  exp_self(unary)->meta = ae_meta_value;
  return unary->exp->type;
}

OP_CHECK(opck_unary_meta2) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  exp_self(unary)->meta = ae_meta_value;
  return t_int;
}

OP_CHECK(opck_unary_meta2_uniq) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  CHECK_OO(opck_unary_meta2(env, data))
  if(unary->exp->next)
    ERR_N(exp_self(unary)->pos, "fuck!!")
  return t_int;
}

OP_CHECK(opck_unary) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  if(unary->exp->meta != ae_meta_var)
    ERR_N(unary->exp->pos,
          "unary operator '%s' cannot be used on %s data-types.",
          op2str(unary->op), access(unary->exp->meta))
  unary->exp->emit_var = 1;
  exp_self(unary)->meta = ae_meta_value;
  return unary->exp->type;
}

OP_CHECK(opck_post) {
  const Exp_Postfix* post = (Exp_Postfix*)data;
  if(post->exp->meta != ae_meta_var)
    ERR_N(post->exp->pos, "post operator '%s' cannot be used on %s data-type.",
          op2str(post->op), access(post->exp->meta))
  post->exp->emit_var = 1;
  exp_self(post)->meta = ae_meta_value;
  return post->exp->type;
}

ANN m_bool check_exp_array_subscripts(const Env env, const Exp exp);
OP_CHECK(opck_new) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  SET_FLAG(unary->td, ref);
  const Type t = known_type(env, unary->td);
  UNSET_FLAG(unary->td, ref);
  CHECK_OO(t)
  if(unary->td->array)
    CHECK_BO(check_exp_array_subscripts(env, unary->td->array->exp))
  return t;
}

OP_EMIT(opem_new) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  CHECK_BB(emit_instantiate_object(emit, exp_self(unary)->type,
    unary->td->array, GET_FLAG(unary->td, ref)))
  CHECK_OB(emit_add_instr(emit, GcAdd))
  return GW_OK;
}
