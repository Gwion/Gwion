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
#include "traverse.h"
#include "parse.h"

ANN static Arg_List curry_arg_list(const Env env, const Arg_List base, const Exp e) {
  Arg_List args = new_mp_vector(env->gwion->mp, sizeof(Arg), 0);
  Exp next = e;
  uint32_t i = 0;
  while(next) {
    if(is_hole(env, next)) {
      char c[256];
      sprintf(c, "@%u\n", args->len);
      Type_Decl *td = cpy_type_decl(env->gwion->mp, mp_vector_at(base, Arg, i)->td);
      Arg arg = { .td = td, .var_decl = { .xid = insert_symbol(c) }};
      mp_vector_add(env->gwion->mp, &args, Arg, arg);
    }
    i++;
    next = next->next;
  }
  return args;
}

ANN2(1) static Func_Base *curry_base(const Env env, const Func_Base *base, Exp earg, const loc_t loc) {
  Arg_List args = earg ? curry_arg_list(env, base->args, earg) : NULL;
  Func_Base *fb = new_func_base(env->gwion->mp, cpy_type_decl(env->gwion->mp, base->td), lambda_name(env->gwion->st, loc.first), args, ae_flag_none, loc);
  fb->fbflag |= fbflag_lambda;
  return fb;
}

ANN static Exp curry_call(const Env env, Exp e) {
  Exp base = NULL, arg;
  uint32_t i = 0;
  while(e) {
    if(is_hole(env, e)) {
      char c[256];
      sprintf(c, "@%u\n", i++);
      const Exp next = new_prim_id(env->gwion->mp, insert_symbol(c), e->pos);
      if(base) arg = arg->next = next;
      else arg = base = next;
    } else {
      const Exp next = cpy_exp(env->gwion->mp, e);
      if(base) arg = arg->next = next;
      else arg = base = next;
    }
    e = e->next;
  }
  return base;
}

ANN static Stmt curry_code(const Env env, const Exp efun, const Exp earg) {
  Stmt_List slist = new_mp_vector(env->gwion->mp, sizeof(struct Stmt_), 1);
  const Exp arg = curry_call(env, earg);
  const Exp exp = new_exp_call(env->gwion->mp, efun, arg, efun->pos);
  Stmt stmt = mp_vector_at(slist, struct Stmt_, 0);
  stmt->stmt_type = ae_stmt_return;
  stmt->d.stmt_exp.val = exp;
  return new_stmt_code(env->gwion->mp, slist, efun->pos);
}

ANN static Type curry_type(const Env env, const Exp exp, const Exp efun, const Exp earg) {
  Func_Base *base = curry_base(env, efun->type->info->func->def->base, earg, exp->pos);
  Stmt code = curry_code(env, efun, earg);
  exp->d.exp_lambda.def = new_func_def(env->gwion->mp, base, code);
  exp->exp_type = ae_exp_lambda;
  return check_exp(env, exp);
}

static OP_CHECK(opck_curry) {
  Exp_Call *call = (Exp_Call*)data;
  if(!call->args)
    ERR_N(exp_self(call)->pos, _("`curry` requires a function as first argument"));
  const Exp efun = call->args;
  const Exp earg = efun->next;
  efun->next = NULL;
  const Type ret = check_exp(env, efun)
    ? curry_type(env, exp_self(call), efun, earg)
    : env->gwion->type[et_error];
  mp_free(env->gwion->mp, Exp, earg);
  return ret;
}

GWION_IMPORT(curry) {
  GWI_BB(gwi_func_ini(gwi, "@function", "curry"));
  GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_none));

  const Env env = gwi->gwion->env; // no need
  const Func f = (Func)map_at(&env->curr->info->func->map, map_size(&env->curr->info->func->map) - 1);
  const struct Op_Func   opfunc = {.ck = opck_curry};
  const struct Op_Import opi    = {
      .rhs  = f->value_ref->type,
      .func = &opfunc,
      .data = (uintptr_t)f,
      .op   = insert_symbol("@func_check")};
  CHECK_BB(add_op(gwi->gwion, &opi));
  return GW_OK;
}

