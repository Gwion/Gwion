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

static OP_CHECK(opck_sift) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Exp lhs = bin->lhs;
  Stmt stmt = lhs->d.exp_unary.code;
  Stmt fst = mp_vector_at(stmt->d.stmt_flow.body->d.stmt_code.stmt_list, struct Stmt_, 0);
  const Symbol chuck = insert_symbol(env->gwion->st, "=>");
  Exp next = new_exp_binary(env->gwion->mp, fst->d.stmt_exp.val, chuck, bin->rhs, bin->rhs->pos);
  CHECK_BN(traverse_exp(env, next));
  fst->d.stmt_exp.val = next;
  const Exp exp = exp_self(bin);
  exp->exp_type = lhs->exp_type;
  exp->d.exp_unary.code = lhs->d.exp_unary.code;
  exp->type = lhs->type;
  exp->d.exp_unary.unary_type = lhs->d.exp_unary.unary_type;
  exp->d.exp_unary.op = lhs->d.exp_unary.op;
  return NULL;
}

static OP_CHECK(opck_ctrl) {
  Exp_Binary *bin = (Exp_Binary*)data;
  MemPool mp = env->gwion->mp;
  if(bin->lhs->exp_type == ae_exp_decl)
    ERR_N(bin->lhs->pos, _("can't use declaration to start sift `|>` operator"));
  const Symbol chuck = insert_symbol(env->gwion->st, "=>");
  Exp exp = exp_self(data);

  Exp func = cpy_exp(mp, exp);
  const Exp dot = new_exp_dot(mp, func->d.exp_binary.lhs, insert_symbol(env->gwion->st, "last"), func->pos);
  const Exp call = new_exp_call(mp, dot, NULL, func->pos);
  func->d.exp_binary.lhs = call;
  func->d.exp_binary.op = chuck;
  traverse_exp(env, func);
  struct Stmt_ one = { .d = { .stmt_exp = { .val = func }}, .stmt_type = ae_stmt_exp, .pos = func->pos };

  Exp samp = new_prim_id(mp, insert_symbol(env->gwion->st, "samp"), func->pos);
  Exp _now = new_prim_id(mp, insert_symbol(env->gwion->st, "now"), func->pos);
  Exp time = new_exp_binary(mp, samp, chuck, _now, func->pos);
  traverse_exp(env, time);
  struct Stmt_ two = { .d = { .stmt_exp = { .val = time }}, .stmt_type = ae_stmt_exp, .pos = func->pos };

  free_exp(mp, bin->lhs);
  free_exp(mp, bin->rhs);
  Stmt_List slist = new_mp_vector(mp, struct Stmt_, 2);
  mp_vector_set(slist, struct Stmt_, 0, one);
  mp_vector_set(slist, struct Stmt_, 1, two);
  const Stmt stmt = new_stmt_code(mp, slist, func->pos);

  const Exp cond = new_prim_id(mp, insert_symbol(env->gwion->st, "true"), func->pos);
  check_exp(env, cond);

  const Stmt loop = new_stmt_flow(mp, ae_stmt_while, cond, stmt, false, func->pos);
  exp->exp_type = ae_exp_unary;
  exp->d.exp_unary.unary_type = unary_code;
  exp->d.exp_unary.code = loop;
  exp->d.exp_unary.op = insert_symbol(env->gwion->st, "spork");

  return NULL;
}

GWION_IMPORT(sift) {
  const Type sift = gwi_class_ini(gwi, "Sift", "Shred");
  SET_FLAG(sift, abstract | ae_flag_final);
  GWI_BB(gwi_class_end(gwi));

  gwidoc(gwi, "This operator expands too\n"
              "#!- spork {{\n"
              "#!-   while(true) {{\n"
              "#!-     lhs.last() => rhs;\n"
              "#!-     samp => now;\n"
              "#!-   }\n"
              "#!- }");
  GWI_BB(gwi_oper_ini(gwi, "UGen", "function", "Sift"));
  GWI_BB(gwi_oper_add(gwi, opck_ctrl));
  GWI_BB(gwi_oper_end(gwi, "|>", NULL));

  GWI_BB(gwi_oper_ini(gwi, "Sift", "function", "Sift"));
  GWI_BB(gwi_oper_add(gwi, opck_sift));
  GWI_BB(gwi_oper_end(gwi, "|>", NULL));

  GWI_BB(gwi_oper_ini(gwi, "UGen", "funptr", "Sift"));
  GWI_BB(gwi_oper_add(gwi, opck_ctrl));
  GWI_BB(gwi_oper_end(gwi, "|>", NULL));

  GWI_BB(gwi_oper_ini(gwi, "Sift", "funptr", "Sift"));
  GWI_BB(gwi_oper_add(gwi, opck_sift));
  GWI_BB(gwi_oper_end(gwi, "|>", NULL));
  return GW_OK;
}
