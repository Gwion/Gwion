#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "emit.h"
#include "traverse.h"
#include "parse.h"
#include "operator.h"
#include "import.h"

OP_CHECK(opck_basic_cast) {
  const Exp_Cast *cast = (Exp_Cast *)data;
  return isa(cast->exp->type, exp_self(cast)->type) > 0
             ? exp_self(cast)->type
             : env->gwion->type[et_error];
}

OP_CHECK(opck_usr_implicit) {
  struct Implicit *imp = (struct Implicit *)data;
  /*
  // in use for refinement types
    const Value v = nspc_lookup_value1(imp->t->info->owner,
  insert_symbol("@implicit")); if(v) { Func f = v->d.func_ref; while(f) {
        if(f->def->base->ret_type == imp->t)
          break;
        f = f->next;
      }
      if(f) {
        // TODO: add call exp
        struct Exp_ call = { .exp_type=ae_exp_call,
  .d={.exp_call={.args=imp->e}}, .pos=imp->e->pos, .type=f->value_ref->type };
        struct Op_Import opi = { .op=insert_symbol("@func_check"),
          .rhs=f->value_ref->type, .pos=imp->e->pos, .data=(uintptr_t)&call };
        CHECK_NN(op_check(env, &opi));
      }
    }
  */
  return imp->t;
}

OP_CHECK(opck_const_rhs) {
  const Exp_Binary *bin    = (Exp_Binary *)data;
  const m_str       access = exp_access(bin->rhs);
  if (access)
    ERR_N(bin->rhs->pos,
          _("cannot assign '%s' on types '%s' and '%s'.\n"
            "  ...  (reason: --- right-side operand is %s.)"),
          s_name(bin->op), bin->lhs->type->name, bin->rhs->type->name, access);
  return bin->rhs->type;
}

OP_CHECK(opck_rassign) {
  const Exp_Binary *bin = (Exp_Binary *)data;
  if (opck_const_rhs(env, data) == env->gwion->type[et_error])
    return env->gwion->type[et_error];
  exp_setvar(bin->rhs, 1);
  return bin->rhs->type;
}

OP_CHECK(opck_unary_meta) {
  const Exp_Unary *unary = (Exp_Unary *)data;
  exp_setmeta(exp_self(unary), 1);
  return unary->exp->type;
}

OP_CHECK(opck_unary_meta2) {
  const Exp_Unary *unary = (Exp_Unary *)data;
  exp_setmeta(exp_self(unary), 1);
  return env->gwion->type[et_bool];
}

OP_CHECK(opck_unary) {
  const Exp_Unary *unary  = (Exp_Unary *)data;
  const m_str      access = exp_access(unary->exp);
  if (access)
    ERR_N(unary->exp->pos,
          _("unary operator '%s' cannot be used on %s data-types."),
          s_name(unary->op), access);
  exp_setvar(unary->exp, 1);
  return unary->exp->type;
}

OP_CHECK(opck_post) {
  const Exp_Postfix *post   = (Exp_Postfix *)data;
  const m_str        access = exp_access(post->exp);
  if (access)
    ERR_N(post->exp->pos,
          _("post operator '%s' cannot be used on %s data-type."),
          s_name(post->op), access);
  exp_setvar(post->exp, 1);
  return post->exp->type;
}

ANN Type check_td(const Env env, Type_Decl *td);

OP_CHECK(opck_new) {
  Exp_Unary *unary = (Exp_Unary *)data;
  DECL_ON(const Type, t, = known_type(env, unary->ctor.td));
  if (type_ref(t))
    ERR_N(unary->ctor.td->pos, _("can't use 'new' on ref type '%s'\n"), t->name);
  if (tflag(t, tflag_infer))
    ERR_N(unary->ctor.td->pos, _("can't use 'new' on '%s'\n"),
          t->name);
  if (unary->ctor.td->array) CHECK_BN(check_subscripts(env, unary->ctor.td->array, 1));
  if(unary->ctor.exp) {
    const Exp self   = exp_self(unary);
    const Exp args   = cpy_exp(env->gwion->mp, unary->ctor.exp);
    const Exp base   = new_exp_unary2(env->gwion->mp, unary->op, unary->ctor.td, unary->ctor.exp, self->pos);
base->type = t;
    const Exp func   = new_exp_dot(env->gwion->mp, base, insert_symbol("new"), self->pos);
    self->d.exp_call.func = func;
    self->d.exp_call.args = args;
    self->d.exp_call.tmpl = NULL;
    self->d.exp_call.allow_curry = false;
    self->exp_type = ae_exp_call;
    CHECK_BN(traverse_exp(env, self));
    return self->type;
//    unarytype
  }
  if (GET_FLAG(t, abstract))
    ERR_N(unary->ctor.td->pos, _("can't use 'new' on abstract type '%s'\n"),
          t->name);
  if (isa(t, env->gwion->type[et_object]) < 0)
    ERR_N(exp_self(unary)->pos, _("can't use 'new' on non-object types...\n"));
  return t;
}

OP_EMIT(opem_new) {
  const Exp_Unary *unary = (Exp_Unary *)data;
  CHECK_BB(emit_instantiate_object(emit, exp_self(unary)->type,
                                   unary->ctor.td->array, 0));
  // we don't need gc for arrays?
  // also when in rewrote exp
  if(!(unary->ctor.td->array || unary->ctor.exp))
    emit_gc(emit, -SZ_INT);
  return GW_OK;
}
