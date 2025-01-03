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
  Exp* lhs = bin->lhs;
  Stmt* stmt = stmtlist_ptr_at(lhs->d.exp_unary.code, 0);
  Stmt* fst  = stmtlist_ptr_at(stmt->d.stmt_flow.body->d.stmt_code.stmt_list, 0);
  const Symbol chuck = insert_symbol(env->gwion->st, "=>");
  Exp* next = new_exp_binary(env->gwion->mp, fst->d.stmt_exp.val, chuck, bin->rhs, bin->rhs->loc);
  CHECK_ON(traverse_exp(env, next)); // how do we free it?
  fst->d.stmt_exp.val = next;
  Exp* exp = exp_self(bin);
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
    ERR_N(bin->lhs->loc, _("can't use declaration to start sift `|>` operator"));
  const Symbol chuck = insert_symbol(env->gwion->st, "=>");
  Exp* exp = exp_self(data);

  Exp* func = cpy_exp(mp, exp);
  Exp* dot = new_exp_dot(mp, func->d.exp_binary.lhs, MK_TAG(insert_symbol(env->gwion->st, "last"), func->loc), func->loc);
  Exp* call = new_exp_call(mp, dot, NULL, func->loc);
  func->d.exp_binary.lhs = call;
  func->d.exp_binary.op = chuck;
  CHECK_ON(traverse_exp(env, func));
  Stmt one = MK_STMT_EXP(func->loc, func);

  Exp* samp = new_prim_id(mp, insert_symbol(env->gwion->st, "samp"), func->loc);
  Exp* _now = new_prim_id(mp, insert_symbol(env->gwion->st, "now"), func->loc);
  Exp* time = new_exp_binary(mp, samp, chuck, _now, func->loc);
  CHECK_ON(traverse_exp(env, time));
  Stmt two = MK_STMT_EXP(func->loc, time);
  free_exp(mp, bin->lhs);
  free_exp(mp, bin->rhs);
  StmtList *slist = new_stmtlist(mp, 2);
  stmtlist_set(slist, 0, one);
  stmtlist_set(slist, 1, two);
  Stmt* stmt = new_stmt_code(mp, slist, func->loc);

  Exp* cond = new_prim_id(mp, insert_symbol(env->gwion->st, "true"), func->loc);
  check_exp(env, cond);

  StmtList *code = new_stmtlist(mp, 1);
  stmtlist_set(code, 0, MK_STMT(ae_stmt_while, func->loc,
        .stmt_flow = {
          .cond = cond,
          .body = stmt
        }
    ));
  exp->exp_type = ae_exp_unary;
  exp->d.exp_unary.unary_type = unary_code;
  exp->d.exp_unary.code = code;
  exp->d.exp_unary.op = insert_symbol(env->gwion->st, "spork");
  return NULL;
}

GWION_IMPORT(sift) {
  const Type sift = gwi_class_ini(gwi, "Sift", "Shred");
  SET_FLAG(sift, abstract | ae_flag_final);
  GWI_B(gwi_class_end(gwi));

  gwidoc(gwi, "This operator expands too\n"
              "#!- spork {{\n"
              "#!-   while(true) {{\n"
              "#!-     lhs.last() => rhs;\n"
              "#!-     samp => now;\n"
              "#!-   }\n"
              "#!- }");
   GWI_B(gwi_oper_ini(gwi, "UGen", "Function", "Sift"));
   GWI_B(gwi_oper_add(gwi, opck_ctrl));
   GWI_B(gwi_oper_end(gwi, "|>", NULL));

   GWI_B(gwi_oper_ini(gwi, "Sift", "Function", "Sift"));
   GWI_B(gwi_oper_add(gwi, opck_sift));
   GWI_B(gwi_oper_end(gwi, "|>", NULL));

   GWI_B(gwi_oper_ini(gwi, "UGen", "Funptr", "Sift"));
   GWI_B(gwi_oper_add(gwi, opck_ctrl));
   GWI_B(gwi_oper_end(gwi, "|>", NULL));

   GWI_B(gwi_oper_ini(gwi, "Sift", "Funptr", "Sift"));
   GWI_B(gwi_oper_add(gwi, opck_sift));
   GWI_B(gwi_oper_end(gwi, "|>", NULL));
  return true;
}
