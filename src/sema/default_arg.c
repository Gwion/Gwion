#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "traverse.h"
#include "sema_private.h"
ANN2(1,2) static Exp* base_args(const MemPool p, const ArgList *args, Exp* next, const uint32_t min) {
  for(uint32_t i = min; i--;) {
    const Arg  arg = arglist_at(args, i);
    Exp* exp = new_prim_id(p, arg.var.vd.tag.sym, arg.var.vd.tag.loc);
    exp->next = next;
    next = exp;
  }
  return next;
}

ANN static Exp* additional_args(const MemPool p, const ArgList *args, const uint32_t max) {
  Exp* next = NULL;
  for(uint32_t i = max; i-- > args->len;) {
    const Arg  arg = arglist_at(args, i);
    Exp* exp = cpy_exp(p, arg.exp);
    exp->next = next;
    next = exp;
  }
  return next;
}

ANN static Exp* mk_args(const MemPool p, const ArgList *args, const uint32_t max) {
  Exp* next = additional_args(p, args, max);
  return base_args(p, args, next, args->len);
}

ANN static StmtList *code(const MemPool p, Exp* func, const ArgList *lst,
       const uint32_t max, const ae_stmt_t type) {
  Exp* arg = mk_args(p, lst, max);
  Exp* call = new_exp_call(p, func, arg, func->loc);
  StmtList *code = new_stmtlist(p, 1);
  stmtlist_set(code, 0, MK_STMT(type, func->loc, .stmt_exp = { .val = call}));
  return code;
}

ANN static StmtList *std_code(const Sema *sema, Func_Base *base, const uint32_t max) {
  const MemPool p = sema->mp;
  Exp* func = new_prim_id(p, base->tag.sym, base->tag.loc);
  return code(p, func, base->args, max, ae_stmt_return);
}

ANN static StmtList *new_code(const Sema *sema, Func_Base *base, const uint32_t max) {
  const MemPool p = sema->mp;
  SymTable *st = sema->st;
  Exp* dbase  = new_prim_id(p, insert_symbol(st, "this"), base->tag.loc);
  const Symbol sym = insert_symbol(st, "new");
  Exp* func  = new_exp_dot(p, dbase, MK_TAG(sym, base->tag.loc), base->tag.loc);
  return code(p, func, base->args, max, ae_stmt_exp);
}


ANN Func_Def default_args(const Sema *sema, Func_Base *fb, Ast *acc, uint32_t max) {
  Func_Base *const base = cpy_func_base(sema->mp, fb);
  StmtList *code = strcmp(s_name(base->tag.sym), "new")
      ? std_code(sema, fb, max)
      : new_code(sema, fb, max);
  const Func_Def  fdef  = new_func_def(sema->mp, base, code);
  Section section = MK_SECTION(func, func_def, fdef, fb->tag.loc);
  sectionlist_add(sema->mp, acc, section);
  return fdef;
}
