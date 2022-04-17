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

ANN static Arg_List curry_arg_list(const Env env, const Exp e) {
  Arg_List args = new_mp_vector(env->gwion->mp, sizeof(Arg), 0);
  Exp next = e;
  while(next) {
    if(is_hole(env, next)) {
      char c[256];
      sprintf(c, "@%u\n", args->len);
      Arg arg = { .var_decl = { .xid = insert_symbol(c) }};
      mp_vector_add(env->gwion->mp, &args, Arg, arg);
    }
    next = next->next;
  }
  return args;
}

ANN2(1) static Func_Base *curry_base(const Env env, Exp earg, const loc_t loc) {
  Arg_List args = earg ? curry_arg_list(env, earg) : NULL;
  Func_Base *base = new_func_base(env->gwion->mp, NULL, lambda_name(env->gwion->st, loc.first), args, ae_flag_none, loc);
  base->fbflag |= fbflag_lambda;
  return base;
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
  const Exp _args = curry_call(env, earg);
  mp_free(env->gwion->mp, Exp, earg);
  const Exp new = new_exp_call(env->gwion->mp, efun, _args, efun->pos);
  Stmt stmt = mp_vector_at(slist, struct Stmt_, 0);
  stmt->stmt_type = ae_stmt_exp;
  stmt->d.stmt_exp.val = new;
  return new_stmt_code(env->gwion->mp, slist, efun->pos);
}

static OP_CHECK(opck_curry) {
  Exp_Call *call = (Exp_Call*)data;
  if(!call->args) exit(2);
  const Exp efun = call->args;
  const Exp earg = efun->next;
  efun->next = NULL;
  Func_Base *base = curry_base(env, earg, exp_self(call)->pos);
  base->fbflag |= fbflag_lambda;
  Stmt code = curry_code(env, efun, earg);
  exp_self(call)->d.exp_lambda.def = new_func_def(env->gwion->mp, base, code);
  exp_self(call)->exp_type = ae_exp_lambda;
  return check_exp(env, exp_self(call));
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
