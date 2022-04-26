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
#include "partial.h"

ANN static Arg_List partial_arg_list(const Env env, const Arg_List base, const Exp e) {
  Arg_List args = new_mp_vector(env->gwion->mp, sizeof(Arg), 0);
  Exp next = e;
  uint32_t i = 0;
  while(next) {
    if(is_hole(env, next) || is_typed_hole(env, next)) {
      char c[256];
      sprintf(c, "@%u", args->len);
      const Arg *src = mp_vector_at(base, Arg, i);
      Type_Decl *td = src->td ? cpy_type_decl(env->gwion->mp, src->td) : NULL;
      Arg arg = { .td = td, .var_decl = { .xid = insert_symbol(c) }};
      mp_vector_add(env->gwion->mp, &args, Arg, arg);
    }
    i++;
    next = next->next;
  }
  return args;
}

ANN static inline Symbol partial_name(const Env env, const pos_t pos) {
  char c[7 + 1 + num_digit(pos.line) + num_digit(pos.column) + 2];
  sprintf(c, "partial:%u:%u", pos.line, pos.column);
  return insert_symbol(c);
}

ANN2(1, 2) static inline Func_Base *partial_base(const Env env, const Func_Base *base, Exp earg, const loc_t loc) {
  Arg_List args = earg ? partial_arg_list(env, base->args, earg) : NULL;
  Func_Base *fb = new_func_base(env->gwion->mp, cpy_type_decl(env->gwion->mp, base->td), partial_name(env, loc.first), args, ae_flag_none, loc);
  return fb;
}

ANN static Exp partial_call(const Env env, Exp e) {
  Exp base = NULL, arg;
  uint32_t i = 0;
  while(e) {
    if(is_hole(env, e) || is_typed_hole(env, e)) {
      char c[256];
      sprintf(c, "@%u", i++);
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


ANN Func find_match(const Env env, Func func, const Exp exp, const bool implicit,
                                        const bool specific) {
  do {
    Exp e = exp;
    uint32_t i = 0;
    Arg_List args = func->def->base->args;
    uint32_t len = args ? args->len : 0;
    while(e) {
      if (i >= len) break;
      const Arg *arg = mp_vector_at(args, Arg, i++);
      if(!is_hole(env, e)) {
        if(!is_typed_hole(env, e)) {
          const Exp next = e->next;
          e->next = NULL;
          const Type ret = check_exp(env, e);
          e->next = next;
          CHECK_OO(ret);
      } else
          CHECK_OO((e->type = known_type(env, e->d.exp_cast.td)));
        if (!func_match_inner(env, e, arg->type, implicit, specific)) break;
      }
      e = e->next;
    }
    if (!e && len == i) return func;
  } while ((func = func->next));
  return NULL;
}

ANN Func find_match_actual(const Env env, const Func up, const Exp args) {
  Func func;
  if ((func = find_match(env, up, args, false, true)) ||
      (func = find_match(env, up, args, true, true)) ||
      (func = find_match(env, up, args, false, true)) ||
      (func = find_match(env, up, args, true, false)))
    return func;
  return NULL;
}

ANN static Func partial_match(const Env env, const Func up, const Exp args, const loc_t loc);

ANN static void print_arg(Arg_List args) {
  for(uint32_t i = 0; i < args->len; i++) {
    Arg *arg = mp_vector_at(args, Arg, i);
    gw_err("{G}%s{0} {/}%s{0}", arg->type ? arg->type->name : NULL,
           arg->var_decl.xid ? s_name(arg->var_decl.xid) : "");
    if(i < args->len - 1) gw_err(", ");
  }
}

ANN void print_signature(const Func f) {
  gw_err("  {-}(%s){0}  ", f->name);
  const Arg_List args = f->def->base->args;
  if (args)
    print_arg(args);
  else
    gw_err("{G}void{0}");
  gw_err("\n");
}

ANN void ambiguity(const Env env, Func f, const Exp args, const loc_t loc) {
  print_signature(f);
  while(f->next) {
    const Func next = partial_match(env, f->next, args, loc);
    if(next) print_signature(next);
    f = f->next;
  }
}

ANN static Func partial_match(const Env env, const Func up, const Exp args, const loc_t loc) {
  const Func f = find_match_actual(env, up, args);
  if(f) {
    if(f->next) {
      const Func next = partial_match(env, f->next, args, loc);
      if(next) {
        gwerr_basic(_("can't resolve ambiguity"), _("in this partial application"), _("use typed holes: _ $ type"), env->name, loc, 0);
        gw_err(_("\nthose functions could match:\n"));
        print_signature(f);
        ambiguity(env, next, args, loc);
        env->context->error = true;
        return NULL;
      }
    }
    return f;
  }
  return NULL;
}

ANN static Stmt partial_code(const Env env, const Exp efun, const Exp earg) {
  Stmt_List slist = new_mp_vector(env->gwion->mp, sizeof(struct Stmt_), 1);
  const Exp arg = partial_call(env, earg);
  const Exp exp = new_exp_call(env->gwion->mp, efun, arg, efun->pos);
  Stmt stmt = mp_vector_at(slist, struct Stmt_, 0);
  stmt->stmt_type = ae_stmt_return;
  stmt->d.stmt_exp.val = exp;
  return new_stmt_code(env->gwion->mp, slist, efun->pos);
}

ANN static uint32_t count_args_exp(Exp args) {
  uint32_t i = 0;
  while(args && ++i) args = args->next;
  return i;
}

ANN static uint32_t count_args_func(Func f, const uint32_t i) {
  uint32_t max = 0;
  while(f) {
    const uint32_t len =  f->def->base->args ? f->def->base->args->len : 0;
    if(len > i && len > max) max = len;
    f = f->next;
  }
  return max;
}

ANN static Exp expand(const Env env, const Func func, const Exp e, const loc_t loc) {
  const uint32_t i = count_args_exp(e);
  const uint32_t max = count_args_func(func, i);
  if(max > i) {
    Exp args = e;
    if(args) {
      while(args->next) args = args->next;
      args->next = new_prim_id(env->gwion->mp, insert_symbol("_"), loc);
      return e;
    } else return new_prim_id(env->gwion->mp, insert_symbol("_"), loc);
  }
  return NULL;
}

ANN Type partial_type(const Env env, Exp_Call *const call) {
  const Func base = call->func->type->info->func;
  if(!base) ERR_O(call->func->pos, _("can't do partiall application on a literal lambda"));
  const Func f = partial_match(env, base, call->args, call->func->pos);
  if(!f) {
    const Exp e = expand(env, call->func->type->info->func, call->args, call->func->pos);
    if(e) {
      call->args = e;
      return partial_type(env, call);
    }
    ERR_O(call->func->pos, _("no match found for partial application"));
  }
  Func_Base *const fbase = partial_base(env, f->def->base, call->args, call->func->pos);
  const Stmt code = partial_code(env, call->func, call->args);
  const Exp exp = exp_self(call);
  exp->d.exp_lambda.def = new_func_def(env->gwion->mp, fbase, code);
  exp->exp_type = ae_exp_lambda;
  CHECK_OO(traverse_func_def(env, exp->d.exp_lambda.def));
  return exp->d.exp_lambda.def->base->func->value_ref->type;
}
