#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "traverse.h"
#include "gwi.h"

static OP_CHECK(opck_ctrl) {
  Exp_Binary *bin = (Exp_Binary*)data;
  MemPool mp = env->gwion->mp;

  const Symbol chuck = insert_symbol(env->gwion->st, "=>");
  Exp exp = exp_self(data);
  Exp func = cpy_exp(mp, exp); // set operator

  const Exp dot = new_exp_dot(mp, func->d.exp_binary.lhs, insert_symbol(env->gwion->st, "last"), func->pos);
  const Exp call = new_exp_call(mp, dot, NULL, func->pos);
  func->d.exp_binary.lhs = call;

  func->d.exp_binary.op = chuck;
  traverse_exp(env, func);

  struct Stmt_ one = { .d = { .stmt_exp = { .val = func }}, .stmt_type = ae_stmt_exp, .pos = func->pos };

  Exp samp = new_prim_id(mp, insert_symbol(env->gwion->st, "samp"), func->pos);
  Exp _now = new_prim_id(mp, insert_symbol(env->gwion->st, "now"), func->pos);
  Exp time = new_exp_binary(mp, samp, chuck, _now, func->pos);
  struct Stmt_ two = { .d = { .stmt_exp = { .val = time }}, .stmt_type = ae_stmt_exp, .pos = func->pos };
  traverse_exp(env, time);

  Stmt_List slist = new_mp_vector(mp, struct Stmt_, 2);
  mp_vector_set(slist, struct Stmt_, 0, one);
  mp_vector_set(slist, struct Stmt_, 1, two);

  const Stmt stmt = new_stmt_code(mp, slist, func->pos);
  free_exp(mp, bin->lhs);
  free_exp(mp, bin->rhs);

  const Exp cond = new_prim_id(mp, insert_symbol(env->gwion->st, "true"), func->pos);
  check_exp(env, cond);

  const Stmt loop = new_stmt_flow(mp, ae_stmt_while, cond, stmt, false, func->pos);
  exp->exp_type = ae_exp_unary;
  exp->d.exp_unary.unary_type = unary_code;
  exp->d.exp_unary.code = loop;
  exp->d.exp_unary.op = insert_symbol(env->gwion->st, "spork");
  return env->gwion->type[et_shred];
}

GWION_IMPORT(ctrl) {
  gwidoc(gwi, "This operator expands too\n"
              "#!- spork {{\n"
              "#!-   while(true) {{\n"
              "#!-     lhs.last() => rhs;\n"
              "#!-     samp => now;\n"
              "#!-   }\n"
              "#!- }");
  GWI_BB(gwi_oper_ini(gwi, "UGen", "function", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_ctrl))
  GWI_BB(gwi_oper_end(gwi, "|>", NULL))
  return GW_OK;
}
