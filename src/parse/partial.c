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

ANN static ArgList *partial_arg_list(const Env env, const ArgList *base, Exp* e) {
  ArgList *args = new_arglist(env->gwion->mp, 0);
  Exp* next = e;
  uint32_t i = 0;
  while(next) {
    if(is_hole(env, next) || is_typed_hole(env, next)) {
      char c[256];
      sprintf(c, "@%u", args->len);
      const Arg src = arglist_at(base, i);
      Type_Decl *td = src.var.td ? cpy_type_decl(env->gwion->mp, src.var.td) : NULL;
      Arg arg = { .var = MK_VAR(td, (Var_Decl){ .tag = MK_TAG(insert_symbol(c), src.var.vd.tag.loc)}), 
        .loc = src.var.vd.tag.loc
      };
      arglist_add(env->gwion->mp, &args, arg);
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

ANN2(1, 2) static inline Func_Base *partial_base(const Env env, const Func_Base *base, Exp* earg, const loc_t loc) {
  ArgList *args = earg ? partial_arg_list(env, base->args, earg) : NULL;
  Func_Base *fb = new_func_base(env->gwion->mp, base->td ? cpy_type_decl(env->gwion->mp, base->td) : NULL, partial_name(env, loc.first), args, ae_flag_none, loc);
  return fb;
}

ANN static Exp* partial_exp(const Env env, ArgList *args, Exp* e, const uint i) {
  if(is_hole(env, e) || is_typed_hole(env, e)) {
    char c[256];
    sprintf(c, "@%u", i);
    Exp* exp = new_prim_id(env->gwion->mp, insert_symbol(c), e->loc);
    exp->type = known_type(env, arglist_at(args, i).var.td);
    exp->d.prim.value = new_value(env, exp->type, MK_TAG(insert_symbol(c), e->loc));
    valid_value(env, insert_symbol(c), exp->d.prim.value);
    return exp;
  }
  Exp* next = e->next;
  e->next = NULL;
  Exp* exp = cpy_exp(env->gwion->mp, e);
  exp->type = e->type;
  e->next = next;
  return exp;
}

ANN2(1) static Exp* partial_call(const Env env, ArgList *args, Exp* e) {
  Exp* base = NULL;
  Exp* arg;
  uint32_t i = 0;
  while(e) {
    Exp* exp = partial_exp(env, args, e, i++);
    if(base) arg = arg->next = exp;
    else arg = base = exp;
    e = e->next;
  }
  return base;
}


ANN Func find_match(const Env env, Func func, Exp* exp, const bool implicit,
                                        const bool specific) {
  do {
    Exp* e = exp;
    uint32_t i = 0;
    ArgList *args = func->def->base->args;
    uint32_t len = arglist_len(args);
    while(e) {
      if (i >= len) break;
      const Arg arg = arglist_at(args, i++);
      if(!is_hole(env, e)) {
        if(!is_typed_hole(env, e)) {
          Exp* next = e->next;
          e->next = NULL;
          const Type ret = check_exp(env, e);
          e->next = next;
          CHECK_O(ret);
      } else
          CHECK_O((e->type = known_type(env, e->d.exp_cast.td)));
        if (!func_match_inner(env, e, arg.type, implicit, specific)) break;
      }
      e = e->next;
    }
    if (!e && len == i) return func;
  } while ((func = func->next));
  return NULL;
}

ANN Func find_match_actual(const Env env, const Func up, Exp* args) {
  return find_match(env, up, args, false, true)  ?:
         find_match(env, up, args, true,  true)  ?:
         find_match(env, up, args, false, true)  ?:
         find_match(env, up, args, true,  false) ?:
         NULL;
}

ANN static Func partial_match(const Env env, const Func up, Exp* args, const loc_t loc);

ANN void ambiguity(const Env env, Func f, Exp* args, const loc_t loc) {
  print_signature(env->gwion, f);
  while(f->next) {
    const Func next = partial_match(env, f->next, args, loc);
    if(next) print_signature(env->gwion, next);
    f = f->next;
  }
}

ANN static Func partial_match(const Env env, const Func up, Exp* args, const loc_t loc) {
  const Func f = find_match_actual(env, up, args);
  if(f) {
    const Type t = f->value_ref->from->owner_class;
    if(f->next) {
      const Func next = partial_match(env, f->next, args, loc);
      if(next) {
        const Type tnext = next->value_ref->from->owner_class;
        if(!t || !tnext || !isa(t, tnext)) {
          gwlog_error(_("can't resolve ambiguity"), _("in this partial application"), env->name, loc, 0);
          gwlog_hint(_("use typed holes: `_ $ type`"), env->name, loc);
          gw_err(_("\nthose functions could match:\n"));
          print_signature(env->gwion, f);
          ambiguity(env, next, args, loc);
          env_set_error(env,  true);
          return NULL;
        }
      }
    }
    return f;
  }
  return NULL;
}

ANN static StmtList *partial_code(const Env env, ArgList *args, Exp* efun, Exp* earg) {
  Exp* arg = partial_call(env, args, earg);
  Exp* exp = new_exp_call(env->gwion->mp, efun, arg, efun->loc);
  StmtList *code = new_stmtlist(env->gwion->mp, 1);
  stmtlist_set(code, 0, MK_STMT_RETURN(efun->loc, exp));
  return code;
}

ANN static uint32_t count_args_exp(Exp* args) {
  uint32_t i = 0;
  do i++;
  while ((args = args->next));
  return i;
}

ANN static uint32_t count_args_func(Func f, const uint32_t i) {
  uint32_t max = 0;
  do {
    const uint32_t len =  arglist_len(f->def->base->args);
    if(len > i && len > max) max = len;
  } while ((f = f->next));
  return max;
}

ANN static Exp* expand(const Env env, const Func func, Exp* e, const loc_t loc) {
  const uint32_t i = count_args_exp(e);
  const uint32_t max = count_args_func(func, i);
  if(max > i) {
    Exp* args = e;
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
  if(!base) ERR_O(call->func->loc, _("can't do partial application on a literal lambda"));
  const Func f = partial_match(env, base, call->args, call->func->loc);
  if(!f) {
    Exp* e = expand(env, call->func->type->info->func, call->args, call->func->loc);
    if(e) {
      call->args = e;
      return partial_type(env, call);
    }
    ERR_O(call->func->loc, _("no match found for partial application"));
  }
  nspc_push_value(env->gwion->mp, env->curr);
  Func_Base *const fbase = partial_base(env, f->def->base, call->args, call->func->loc);
  StmtList *code = partial_code(env, f->def->base->args, call->func, call->args);
  Exp* exp = exp_self(call);
  exp->d.exp_lambda.def = new_func_def(env->gwion->mp, fbase, code);
  exp->exp_type = ae_exp_lambda;
  const bool ret = traverse_func_def(env, exp->d.exp_lambda.def);
  nspc_pop_value(env->gwion->mp, env->curr);
  return ret ? exp->d.exp_lambda.def->base->func->value_ref->type : NULL;
}
