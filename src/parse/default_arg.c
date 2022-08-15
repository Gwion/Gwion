#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "traverse.h"

ANN static Exp mk_default_args(const MemPool p, const Arg_List args, const uint32_t max) {
  Exp exp = NULL, base_exp = NULL;
  for(uint32_t i = 0; i < args->len; i++) {
    Arg *arg = mp_vector_at(args, Arg, i);
    const Exp arg_exp = new_prim_id(p, arg->var_decl.xid, arg->var_decl.pos);
    if(exp)
      exp = (exp->next = arg_exp);
    else
      base_exp = exp = arg_exp;
  }
  // now add default args
  for(uint32_t i = args->len; i < max; i++) {
    Arg *arg = mp_vector_at(args, Arg, i);
    const Exp arg_exp = cpy_exp(p, arg->exp);
    if(exp)
      exp = (exp->next = arg_exp);
    else
      base_exp = exp = arg_exp;
  }
  return base_exp;
}

ANN static Stmt_List std_code(const MemPool p, Func_Base *base, const Arg_List args, const uint32_t len) {
  const Exp efunc = new_prim_id(p, base->xid, base->pos);
  const Exp exp_arg = mk_default_args(p, args, len);
  const Exp ecall = new_exp_call(p, efunc, exp_arg, base->pos);
  Stmt_List slist = new_mp_vector(p, struct Stmt_, 1);
  mp_vector_set(slist, struct Stmt_, 0,
    ((struct Stmt_) {
      .stmt_type = ae_stmt_return, .d = { .stmt_exp = { .val = ecall }},
      .pos = base->pos
  }));
  return slist;
}

ANN static Stmt_List new_code(const Env env, Func_Base *base, const Arg_List args, const uint32_t len) {
  const MemPool p = env->gwion->mp;
  const Exp dbase  = new_prim_id(p, insert_symbol(env->gwion->st, "this"), base->pos);
  const Exp dot  = new_exp_dot(p, dbase, insert_symbol(env->gwion->st, env->class_def->name), base->pos);
  const Exp exp_args = mk_default_args(p, args, len);
  const Exp ecall = new_exp_call(p, dot, exp_args, base->pos);
  Stmt_List slist = new_mp_vector(p, struct Stmt_, 1);
  mp_vector_set(slist, struct Stmt_, 0,
    ((struct Stmt_) {
      .stmt_type = ae_stmt_exp, .d = { .stmt_exp = { .val = ecall }},
      .pos = base->pos
  }));
  return slist;
}

ANN void default_args(const Env env, const Section *s, Ast *acc) {
  const MemPool p = env->gwion->mp;
  const Func_Def base_fdef = s->d.func_def;
  Arg_List       args = base_fdef->base->args;
  uint32_t len = args->len;
  while(args->len--) {
    Arg *arg = mp_vector_at(args, Arg, args->len);
    if(!arg->exp) break;
    Func_Base *const base = cpy_func_base(p, base_fdef->base);
    Stmt_List slist = strcmp("new", s_name(base->xid))
        ? std_code(env->gwion->mp, base, args, len)
        : new_code(env, base, args, len);
    const Stmt      body  = new_stmt_code(p, slist, base->pos);
    const Func_Def  fdef  = new_func_def(p, base, body);
    scan1_func_def(env, fdef);
    scan2_func_def(env, fdef);
    Section section = MK_SECTION(func, func_def, fdef);
    mp_vector_add(p, acc, Section, section);
  }
  args->len = len;
}
