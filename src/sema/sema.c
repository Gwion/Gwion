#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "sema.h"
#define __SEMA_IMPLEMENTATION__
#include "sema_private.h"
#include "default_args.h"
#undef __SEMA_IMPLEMENTATION__

#define POISON(a, b) \
  do { \
    a->error = true; \
    b->poison = true; \
  } while (0)

#define POISON_OK(a, b, ok) \
  do { \
    POISON(a, b); \
    ok = false; \
  } while (0)


ANN static bool unique_expression(Sema *a, Exp *b, const char *ctx) {
  const bool ok = sema_exp(a, b);
  if(!b->next) return true && ok;
  gwlog_error("can't use multiple expressions", ctx,
              a->filename, b->next->loc, 0);
  return false;
}

__attribute__((warn_unused_result))
ANN static bool array_not_empty(Sema *a, Array_Sub b,
                                const char *ctx, const loc_t loc) {
  if(b->exp)
    return true;
  gwlog_error(_("must provide values/expressions for array [...]"),
                ctx, a->filename, loc, 0);
  return false;
}

__attribute__((warn_unused_result))
ANN static bool array_empty(Sema *a, Array_Sub b,
                            const char *ctx, const loc_t loc) {
  if(!b->exp)
    return true;
  gwlog_error(_("array must be empty []"),
                ctx, a->filename, loc, 0);
  return false;
}

__attribute__((warn_unused_result))
ANN static bool type_decl_array_not_empty(Sema *a, Type_Decl *b, const char *ctx) {
  const bool ok = sema_type_decl(a, b);
  if(b->array) return array_not_empty(a, b->array, ctx, b->tag.loc) && ok;
  return ok;
}

__attribute__((warn_unused_result))
ANN static bool type_decl_array_empty(Sema *a, Type_Decl *b, const char *ctx) {
  const bool ok = sema_type_decl(a, b);
  if(b->array) return array_empty(a, b->array, ctx, b->tag.loc) && ok;
  return ok;
}

ANN static bool sema_array_sub(Sema *a, Array_Sub b) {
  if(b->exp) return sema_exp(a, b->exp);
  return true;
}

ANN static bool sema_tmplarg(Sema *a, TmplArg *b) {
  if (b->type == tmplarg_td)
    return type_decl_array_empty(a, b->d.td, "in template argument");
  return sema_exp(a, b->d.exp);
}

ANN static bool sema_tmplarg_list(Sema *a, TmplArg_List b) {
  bool ok = true;
  for(uint32_t i = 0; i < b->len; i++) {
    TmplArg * c = mp_vector_at(b, TmplArg, i);
    if(!sema_tmplarg(a, c))
      ok = false;
  }
  return ok;
}

ANN static bool sema_tmpl(Sema *a, Tmpl *b) {
  if(b->call) return sema_tmplarg_list(a, b->call);
  return true;
}

ANN static bool sema_range(Sema *a, Range *b) {
  bool ok = true;
  if(b->start && !unique_expression(a, b->start, "in range start"))
    ok = false;
  if(b->end && !unique_expression(a, b->end, "in range end"))
    ok = false;
  return ok;
}

ANN static bool sema_type_decl(Sema *a, Type_Decl *b) {
  bool ok = true;
  if(b->array && !sema_array_sub(a, b->array))
    ok = false;
  if(b->types && !sema_tmplarg_list(a, b->types))
    ok = false;
  return ok;
}

ANN static bool sema_prim_id(Sema *a NUSED, Symbol *b NUSED) {
  return true;
}

ANN static bool sema_prim_num(Sema *a NUSED, m_uint *b NUSED) {
  return true;
}

ANN static bool sema_prim_float(Sema *a NUSED, m_float *b NUSED) {
  return true;
}

ANN static bool sema_prim_str(Sema *a NUSED, m_str *b NUSED) {
  return true;
}

ANN static bool sema_prim_array(Sema *a, Array_Sub *b) {
  bool ok = sema_array_sub(a, *b);
  if(!array_not_empty(a, *b,
                      "in array litteral", prim_exp(b)->loc))
    POISON_OK(a, prim_exp(b), ok);
  return ok;
}

ANN static bool sema_prim_range(Sema *a, Range* *b) {
  return sema_range(a, *b);
}

ANN static bool sema_prim_dict(Sema *a, Exp* *b) {
  return sema_exp(a, *b);
}

ANN static bool sema_prim_hack(Sema *a, Exp* *b) {
  return sema_exp(a, *b);
}

ANN static bool sema_prim_interp(Sema *a, Exp* *b) {
  return sema_exp(a, *b);
}

ANN static bool sema_prim_char(Sema *a NUSED, m_str *b NUSED) {
  return true;
}

ANN static bool sema_prim_nil(Sema *a NUSED, bool *b NUSED) {
  return true;
}

ANN static bool sema_prim_perform(Sema *a NUSED, Symbol *b NUSED) {
  return true;
}

ANN static bool sema_prim_locale(Sema *a NUSED, Symbol *b NUSED) {
  return true;
}

DECL_PRIM_FUNC(sema, bool, Sema *)
ANN static bool sema_prim(Sema *a, Exp_Primary *b) {
  return sema_prim_func[b->prim_type](a, &b->d);
}


ANN static bool sema_variable(Sema *a, Variable *b) {
  if(b->td) return sema_type_decl(a, b->td);
  return true;
}

ANN static bool sema_exp_decl(Sema *a, Exp_Decl *b) {
  bool ok = sema_variable(a, &b->var);
  if(b->args && !sema_exp(a, b->args))
    ok = false;
  return ok;
}

ANN static bool sema_exp_binary(Sema *a, Exp_Binary *b) {
  const bool ok = sema_exp(a, b->lhs);
  return sema_exp(a, b->rhs) && ok;
}

ANN static bool sema_exp_unary(Sema *a, Exp_Unary *b) {
  const enum unary_type type = b->unary_type;
  if(type == unary_exp) return sema_exp(a, b->exp);
  if(type == unary_td) {
    if(!type_decl_array_not_empty(a, b->ctor.td, "in `new` expression"))
      POISON(a, exp_self(b));
    if(b->ctor.exp) return sema_exp(a, b->ctor.exp);
  } else 
    return sema_stmt_list(a, &b->code);
  return !exp_self(b)->poison;
}

ANN static bool sema_exp_cast(Sema *a, Exp_Cast *b) {
  const bool ok = type_decl_array_empty(a, b->td, "in `cast` expression");
  return sema_exp(a, b->exp) && ok;
}

ANN static bool sema_exp_post(Sema *a, Exp_Postfix *b) {
  return sema_exp(a, b->exp);
}

ANN static bool sema_exp_call(Sema *a, Exp_Call *b) {
  bool ok = sema_exp(a, b->func);
  if(b->args && !sema_exp(a, b->args))
    ok = false;
  if(b->tmpl && !sema_tmpl(a, b->tmpl))
    ok = false;
  return ok;
}

ANN static bool sema_exp_array(Sema *a, Exp_Array *b) {
  const bool ok = sema_exp(a, b->base);
  CHECK_B(array_not_empty(a, b->array, "in array expression", exp_self(b)->loc));
  return sema_array_sub(a, b->array) && ok;
}

ANN static bool sema_exp_slice(Sema *a, Exp_Slice *b) {
  const bool ok = sema_exp(a, b->base);
  return sema_range(a, b->range) && ok;
}

ANN static bool sema_exp_if(Sema *a, Exp_If *b) {
  bool ok = unique_expression(a, b->cond, "in `if` expression condition");
  if(b->if_exp && !unique_expression(a, b->if_exp, "in `if` expression true branch"))
    ok = false;
  return unique_expression(a, b->else_exp, "in `in` expression false branch") && ok;
}

ANN static bool sema_exp_dot(Sema *a, Exp_Dot *b) {
  return unique_expression(a, b->base, "in dot expression");
}

ANN static bool sema_exp_lambda(Sema *a, Exp_Lambda *b) { 
  return sema_func_def(a, b->def);
}

ANN static bool sema_exp_td(Sema *a, Type_Decl *b) {
  return type_decl_array_empty(a, b, "in `type declaration` expression");
}

DECL_EXP_FUNC(sema, bool, Sema*)
ANN static bool sema_exp(Sema *a, Exp* b) {
  bool ok = sema_exp_func[b->exp_type](a, &b->d);
  if(b->next && !sema_exp(a, b ->next))
    ok = false;
  return ok;
}

ANN static bool sema_stmt_exp(Sema *a, Stmt_Exp b) {
  if(b->val) return sema_exp(a,  b->val);
  return true;
}

ANN static bool sema_stmt_while(Sema *a, Stmt_Flow b) {
  const bool ok = sema_exp(a, b->cond);
  return sema_stmt(a, b->body, false) && ok;
}

ANN static bool sema_stmt_until(Sema *a, Stmt_Flow b) {
  const bool ok = sema_exp(a, b->cond);
  return sema_stmt(a, b->body, false) && ok;
}

ANN static bool sema_stmt_for(Sema *a, Stmt_For b) {
  bool ok = sema_stmt(a, b->c1, false);
  if (!b->c2 || !b->c2->d.stmt_exp.val) {
    gwlog_error(
          _("empty for loop condition..."),
          _("...(note: explicitly use 'true' if it's the intent)"),
          a->filename, stmt_self(b)->loc, 0);
    ok = false;
  } else if(!sema_stmt(a, b->c2, false))
    ok = false;
  if(b->c3 && !sema_exp(a, b->c3))
    ok = false;
  return sema_stmt(a, b->body, false) && ok;
}

ANN static bool sema_stmt_each(Sema *a, Stmt_Each b) {
  const bool ok = unique_expression(a, b->exp, "in foreach statement");
  return sema_stmt(a, b->body, false) && ok;
}

// TODO: rename loop to repeat
ANN static bool sema_stmt_loop(Sema *a, Stmt_Loop b) {
  const bool ok = unique_expression(a, b->cond, "in repeat statement");
  return sema_stmt(a, b->body, false) && ok;
}

ANN static bool sema_stmt_if(Sema *a, Stmt_If b) {
  bool ok = sema_exp(a,  b->cond);
  if(!sema_stmt(a, b->if_body, false))
    ok = false;
  if(b->else_body && !sema_stmt(a, b->else_body, false))
    ok = false;
  return ok;
}

ANN static bool sema_stmt_code(Sema *a, Stmt_Code b) {
  if(b->stmt_list) {
    const bool scope = a->scope;
    a->scope = true;
    const bool ret = sema_stmt_list(a, &b->stmt_list);
    a->scope = scope;
    return ret;
  }
  return true;
}

ANN static bool sema_stmt_break(Sema *a, Stmt_Index b) {
  return sema_stmt_index(a, b);
}

ANN static bool sema_stmt_continue(Sema *a, Stmt_Index b) {
  return sema_stmt_index(a, b);
}

ANN static bool sema_stmt_return(Sema *a, Stmt_Exp b) {
  bool ok = true;
  if(!a->func) {
    gwlog_error("'return' statement found outside function definition", NULL, a->filename, stmt_self(b)->loc, 0);
    POISON(a, stmt_self(b));
    ok = false;
  }
  if(a->in_defer) {
    gwlog_error("'return' statement in defered action", NULL, a->filename, stmt_self(b)->loc, 0);
    POISON(a, stmt_self(b));
    ok = false;
  }
  if(b->val && !unique_expression(a, b->val, "in `return` statement"))
    ok = false;
  return ok;
}

ANN static bool sema_case_list(Sema *a, Stmt_List b) {
  bool ok = true;
  for(uint32_t i = 0; i < b->len; i++) {
    Stmt* c = mp_vector_at(b, Stmt, i);
    if(!sema_stmt_case(a, &c->d.stmt_match)) {
      POISON_OK(a, c, ok);
    }
  }
  return ok;
}

ANN static bool sema_stmt_match(Sema *a, Stmt_Match b) {
  bool ok = sema_exp(a, b->cond);
  if(!sema_case_list(a, b->list))
    ok = false;
  if(b->where && !sema_stmt(a, b->where, false))
    ok = false;
  return ok;
}

ANN static bool sema_stmt_case(Sema *a, Stmt_Match b) {
  bool ok = sema_exp(a, b->cond);
  if(!sema_stmt_list(a, &b->list))
    ok = false;
  if(b->when && !sema_exp(a, b->when))
    ok = false;
  return ok;
}

ANN static bool sema_stmt_index(Sema *a NUSED, Stmt_Index b NUSED) {
  return true;
}

ANN static bool sema_stmt_pp(Sema *a, Stmt_PP b) {
  if(b->pp_type == ae_pp_include)
    a->filename = b->data;
  return true;
}

ANN static bool sema_stmt_retry(Sema *a NUSED, Stmt_Exp b NUSED) {
  if(a->handling) return true;
  gwlog_error("`retry` outside of `handle` block", NULL, a->filename, stmt_self(b)->loc, 0);
  return false;
}

ANN static bool sema_handler(Sema *a, Handler *b, ID_List *tags) {
  bool ok = true;
  for(uint32_t i = 0; i < (*tags)->len; i++) {
    Tag *tag = mp_vector_at(*tags, Tag, i);
    if(!tag->sym && b->tag.sym) {
      gwlog_error("named handler after a catch-all one", NULL, a->filename, b->tag.loc, 0);
      gwlog_related("catch-all used here", a->filename, b->tag.loc);
      ok = false;
    } else if(b->tag.sym == tag->sym) {
      gwlog_error("duplicate handler tag", NULL, a->filename, b->tag.loc, 0);
      gwlog_related("handler used here", a->filename, b->tag.loc);
      ok = false;
    }
  }
  mp_vector_add(a->mp, tags, Tag, b->tag);
  return sema_stmt(a, b->stmt, false) && ok;
}

ANN static bool sema_handler_list(Sema *a, Handler_List b) {
  bool ok = true;
  MP_Vector *tags = new_mp_vector(a->mp, Tag, 0);
  for(uint32_t i = 0; i < b->len; i++) {
    Handler *handler = mp_vector_at(b, Handler, i);
    if(!sema_handler(a, handler, &tags))
      ok = false;
  }
  free_mp_vector(a->mp, Symbol, tags);
  return ok;
}

ANN static bool sema_stmt_try(Sema *a, Stmt_Try b) {
  const bool handling = a->handling;
  a->handling = true;
  bool ok = sema_stmt(a, b->stmt, false);
  if(!sema_handler_list(a, b->handler))
    ok = false;
  a->handling = handling;
  return ok;
}

ANN static bool sema_stmt_defer(Sema *a, Stmt_Defer b) {
  const bool in_defer = a ->in_defer;
  a->in_defer = true;
  return sema_stmt(a, b->stmt, false);
  a ->in_defer = in_defer;
}

ANN2(1, 2) static bool sema_spread(Sema *a, const Spread_Def spread, MP_Vector **acc) {
  char c[256]; // dangerous?
  const m_str data = spread->data;
  DECL_B(FILE *,f, = fmemopen(data, strlen(data), "r"));
  struct AstGetter_ arg =  {a->filename, f, a->st, .ppa = a->ppa};
  if(!a->tmpls) { // we are trying this to prevent too many errors
    DECL_B(Ast, ast, = parse_pos(&arg, spread->tag.loc.first));
    const bool ret = sema_ast(a, &ast);
    free_ast(a->mp, ast);
    return ret;
  }
  bool ok = true;
  for(uint32_t i = 0; i < a->tmpls->len; i++) {
    const Type_Decl *td = *mp_vector_at(a->tmpls, Type_Decl*, i);
    fseek(f, 0, SEEK_SET);

    const m_str type = tdpp(a->mp, a->st, td, true, true);
    sprintf(c, "%s=%s", s_name(spread->tag.sym), type);
    pparg_add(a->ppa, c);
    free_mstr(a->mp, type);
    for(uint32_t j = 0; j < spread->list->len; j++) {
      const Symbol sym = *mp_vector_at(spread->list, Symbol, j);
      m_str name = s_name(sym);
      char c[256]; // dangerous?
      sprintf(c, "%s=%s%i", name, name, i);
      pparg_add(a->ppa, c);
    }
    Ast ast = parse_pos(&arg, spread->tag.loc.first);
    pparg_rem(a->ppa, s_name(spread->tag.sym));
    for(uint32_t j = 0; j < spread->list->len;j++) {
      const Symbol sym = *mp_vector_at(spread->list, Symbol, j);
      m_str name = s_name(sym);
      pparg_rem(a->ppa, name);
    }
    // we should only *sema* the first one
    if(ast /*&& sema_ast(a, &ast)*/) {
      for(uint32_t i = 0; i < ast->len; i++) {
        const Section section = *mp_vector_at(ast, Section, i);
        mp_vector_add(a->mp, acc, Section, section);
      }
      free_mp_vector(a->mp, Section , ast);
    } else {
      ok = false;
      break;
    }
  }
  fclose(f);
  return ok;
}

ANN static Stmt_List spread_to_stmt_list(Sema *a, const Spread_Def spread) {
  bool ok = true;
  Ast ast = new_mp_vector(a->mp, Section, 0);
  if(!sema_spread(a, spread, &ast)) {
    free_ast(a->mp, ast);
    return NULL;
  }
  Stmt_List stmt_list = new_mp_vector(a->mp, Stmt, 0);
  for(uint32_t i = 0; i < ast->len; i++) {
    Section *section = mp_vector_at(ast, Section, i);
    if(section->section_type != ae_section_stmt) {
      ok = false;
      break;
    }
    for(uint32_t j = 0; j < section->d.stmt_list->len; j++) {
      Stmt stmt = *mp_vector_at(section->d.stmt_list, Stmt, j);
      mp_vector_add(a->mp, &stmt_list, Stmt, stmt);
    }
  }
  if(ok) {
    free_mstr(a->mp, spread->data);
    free_id_list(a->mp, spread->list);
    free_mp_vector(a->mp, Section, ast);
    return stmt_list;
  }
  gwlog_error(_("invalid spread section"), NULL, a->filename, spread->tag.loc, 0);
  free_ast(a->mp, ast);
  return NULL;
}


ANN static bool sema_stmt_spread(Sema *a, Spread_Def b) {
  if(!a->tmpls) {
    if(a->in_variadic) return sema_spread(a, b, NULL);
    else {
      gwlog_error(_("spread statement outside of variadic environment"), NULL,
        a->filename, b->tag.loc, 0);
      return false;
    }
  }
  bool ok = true;
    DECL_B(Stmt_List, stmt_list, = spread_to_stmt_list(a, b));
    Stmt *stmt = mp_vector_at(stmt_list, Stmt, 0);
    if(!sema_stmt(a, stmt, false)) { //bin
      free_stmt_list(a->mp, stmt_list);
      return false;
    }
 if(a->stmt_list) {

    Stmt_List base = *a->stmt_list;
    const uint32_t index = ((uint8_t*)stmt_self(b) - base->ptr) / sizeof(Stmt);
    // for now we will litteraly create a new vector
    // TODO: a function to extends array len (and allocate if needed)
    MP_Vector *result = new_mp_vector(a->mp, Stmt, base->len + stmt_list->len - 1);
    // store the first part of the list in it
    // TODO: use memcpy
    // or berrer, use the above function to just start at part two
    for(uint32_t i = 0; i < index; i++) {
      Stmt stmt = *mp_vector_at(base, Stmt, i);
      mp_vector_set(result, Stmt, i, stmt);
    }
    for(uint32_t i = 0; i < stmt_list->len; i++) { 
      Stmt stmt = *mp_vector_at(stmt_list, Stmt, i);
      mp_vector_set(result, Stmt, i + index, stmt);
    }
    for(uint32_t i = index + 1; i < base->len; i++) { 
      Stmt stmt = *mp_vector_at(base, Stmt, i);
      mp_vector_set(result, Stmt, stmt_list->len + i - 1, stmt);
    }
    // we can now free the base list
    free_mp_vector(a->mp, Stmt, base);
    *a->stmt_list = result;
  } else {
    Stmt_List result = spread_to_stmt_list(a, b);
    Stmt *stmt = stmt_self(b);
    stmt->stmt_type = ae_stmt_code;
    stmt->d.stmt_code.stmt_list = result;
  }
  return ok;
}

DECL_STMT_FUNC(sema, bool, Sema*)
ANN static bool sema_stmt(Sema *a, Stmt* b, bool in_list) {
  Stmt_List *stmt_list = a->stmt_list;
  if(!in_list) a->stmt_list = NULL;
  const bool ret = sema_stmt_func[b->stmt_type](a, &b->d);
  if(!ret) b->poison = true;
  a->stmt_list = stmt_list;
  return ret;
}

ANN static bool sema_arg(Sema *a, Arg *b, const bool no_default) {
  bool ok = sema_variable(a, &b->var);
  // NOTE: we need to check for td in case of lambda
  // maybe lambdas should have a td with 'auto'?
  if(b->var.td && b->var.td->array &&
    !array_empty(a, b->var.td->array,
                 "in argument", b->var.td->tag.loc))
      ok = false;
  if (b->exp) {
    if(no_default) {
      gwlog_error("'default' argument not allowed", NULL, a->filename, b->var.vd.tag.loc, 0);
      ok = false;
    }
    if(!unique_expression(a, b->exp, "in argument list"))
      ok = false;
  }
  return ok;
}

ANN static bool sema_arg_list(Sema *a, Arg_List b,
      bool *has_default, const bool arg_needs_sym, const bool no_default) {
  bool ok = true;
  for(uint32_t i = 0; i < b->len; i++) {
    Arg *c = mp_vector_at(b, Arg, i);
    if(!c->var.vd.tag.sym && arg_needs_sym) {
      gwlog_error("argument needs name", NULL, a->filename, c->var.vd.tag.loc, 0);
      ok = false;
    }
    if(!sema_arg(a, c, no_default))
      ok = false;
    if(c->exp) *has_default = true;
    else if(*has_default) {
      gwlog_error("missing default argument", NULL, a->filename, c->var.vd.tag.loc, 0);
      ok = false;
    }
  }
  return ok;
}

ANN static bool sema_stmt_list(Sema *a, Stmt_List *b) {
  bool ok = true;
  Stmt_List *stmt_list = a->stmt_list;
  a->stmt_list = b;
  for(uint32_t i = 0; i < (*b)->len; i++) {
    Stmt* c = mp_vector_at(*b, Stmt, i);
    if(!sema_stmt(a, c, true))
      POISON_OK(a, c, ok);
  }
  a->stmt_list = stmt_list;
  return ok;
}

ANN static bool sema_func_base(Sema *a, Func_Base *b, const bool arg_need_sym) {
  bool ok = true;
  if(b->td && !type_decl_array_empty(a, b->td, "in function return type"))
    ok = false;
  const bool no_default = GET_FLAG(b, abstract) || fbflag(b, fbflag_op);
  bool has_default = false;
  if(b->args && !sema_arg_list(a, b->args, &has_default, arg_need_sym, no_default))
    ok  = false;
  if(has_default)
    set_fbflag(b, fbflag_default);
  if(b->tmpl && !sema_tmpl(a, b->tmpl))
    ok = false;
  return ok;
}

ANN static bool fill(Sema *a, const Tmpl *tmpl) {
  bool ok = true;
  const uint32_t len = a->tmpls->len;
  for(uint32_t i = tmpl->list->len - 1; i < tmpl->call->len; i++) {
    TmplArg targ = *mp_vector_at(tmpl->call, TmplArg, i);
    if(targ.type != tmplarg_td) {
      gwlog_error("invalid const expression in variadic template",
                  "can't use expression in spread",
                  a->filename, targ.d.exp->loc, 0);
      Specialized *spec = mp_vector_at(tmpl->list, Specialized,
                                       tmpl->list->len - 1);
      gwlog_related("spread starts here", a->filename, spec->tag.loc);
      ok = false;
    }
    mp_vector_add(a->mp, &a->tmpls, Type_Decl*, targ.d.td);
  }
  if(ok) return true;
  a->tmpls->len = len;
  return false;
}

ANN static void unfill(Sema *a, const Tmpl *b) {
  a->tmpls->len -= b->call->len - (b->list->len) - 1;
}

ANN static bool sema_func_def(Sema *a, Func_Def b) {
  const Tmpl *tmpl = b->base->tmpl;
  const bool is_base = tmpl && tmpl_base(tmpl);
  const bool is_spread = tmpl && is_spread_tmpl(tmpl);
  const bool is_fill = !is_base && is_spread;
  const bool in_variadic = a->in_variadic;
  const bool in_defer = a->in_defer;
  a->in_variadic = in_variadic || is_spread;
  a->in_defer = in_defer;
  if(is_fill && !fill(a, tmpl)) 
    return false;
  bool ok = sema_func_base(a, b->base, !GET_FLAG(b->base, abstract));
  if(b->d.code) {
    const bool func = a->func;
    a->func = true; // we can move this out
    (void)sema_stmt_list(a, &b->d.code); // ignore code in return value
    a->func = func;
  }
  if(is_fill) unfill(a, tmpl);
  a->in_variadic = in_variadic;
  a->in_defer = in_defer;
  return ok;
}


ANN static bool sema_class_def(Sema *a, Class_Def b) {
  const Tmpl *tmpl = b->base.tmpl;
  const bool is_base = tmpl && tmpl_base(tmpl);
  const bool is_spread = tmpl && is_spread_tmpl(b->base.tmpl);
  const bool in_variadic = a->in_variadic;
  const bool is_fill = !is_base && is_spread;
  a->in_variadic = a->in_variadic || is_spread;
  if(is_fill && !fill(a, tmpl)) 
    return false;
  bool ok = sema_type_def( a, &b->base);
  // TODO: what about typedeffing an array type?
  if(b->base.ext && b->base.ext->array &&
    !array_not_empty(a, b->base.ext->array,
                     "in class extends", b->base.ext->tag.loc))
    ok = false;
  if(b->body && !sema_ast(a, &b->body))
    ok = false;
  if(is_fill) unfill(a, tmpl);
  a->in_variadic = in_variadic;
  return ok;
}

ANN static bool sema_trait_def(Sema *a, Trait_Def b) {
  if(b->body) return sema_ast(a, &b->body);
  return true;
}

ANN static bool sema_enumvalue(Sema *a NUSED, EnumValue *b NUSED) {
  return true;
  // gwint, set
}

ANN static bool sema_enum_list(Sema *a, EnumValue_List b) {
  bool ok = true;
  for(uint32_t i = 0; i < b->len; i++) {
    EnumValue *c = mp_vector_at(b, EnumValue, i);
    if(!sema_enumvalue(a, c))
      ok = false;
  }
  return ok;
}

ANN static bool sema_enum_def(Sema *a, Enum_Def b) {
  return sema_enum_list(a, b->list);
}

// TODO: rename l
ANN static bool sema_union_def(Sema *a, Union_Def b) {
  bool ok = true;
  for(uint32_t i = 0; i < b->l->len; i++) {
    Variable *c = mp_vector_at(b->l, Variable, i);
    sema_variable(a, c);
    if(!type_decl_array_empty(a, c->td, "in union member"))
      ok = false;
  }
  if(b->tmpl) {
    if(!sema_tmpl(a, b->tmpl))
      ok = false;
    if(is_spread_tmpl(b->tmpl)) {
      gwlog_error(_("unions can't be variadic"), NULL,
                  a->filename, b->tag.loc, 0);
      ok = false;
    }
  }
  return ok;
}

ANN static bool sema_fptr_def(Sema *a, Fptr_Def b) {
  return sema_func_base(a, b->base, false);
}

ANN static bool sema_type_def(Sema *a, Type_Def b) {
  bool ok = true;
  if(b->ext && !sema_type_decl(a, b->ext))
    ok = false;
  if(b->tmpl && !sema_tmpl(a, b->tmpl))
    ok = false;
  if(b->when && !unique_expression(a, b->when, "in type definition"))
    ok = false;
  return ok;
}

ANN static bool sema_extend_def(Sema *a, Extend_Def b) {
  return type_decl_array_empty(a, b->td, "in extends definition");
}

ANN static bool sema_prim_def(Sema *a NUSED, Prim_Def b NUSED) {
  return true;
}

DECL_SECTION_FUNC(sema, bool, Sema*)
ANN static bool sema_section(Sema *a, Section *b) {
  if(b->section_type != ae_section_stmt)
    return sema_section_func[b->section_type](a, *(void**)&b->d);
  return sema_section_func[b->section_type](a, &b->d);
}

ANN void sema_default_args(const Sema *a, const Section *s, Ast *acc) {
  Func_Base *const fb = s->d.func_def->base;
  Arg_List       args = fb->args;
  uint32_t len = args->len;
  while(args->len--) {
    const Arg *arg = mp_vector_at(args, Arg, args->len);
    if(!arg->exp) break;
    default_args(a, fb, acc, len);
  }
  args->len = len;
}

ANN static bool sema_ast(Sema *a, Ast *b) {
  bool ok = true;
  Ast acc = new_mp_vector(a->mp, Section, 0);
  for(uint32_t i = 0; i < (*b)->len; i++) {
    Section *c = mp_vector_at((*b), Section, i);
    if(!sema_section(a, c)) {
      POISON_OK(a, c, ok);
      continue;
    }
    if (c->section_type == ae_section_func &&
        fbflag(c->d.func_def->base, fbflag_default))
        mp_vector_add(a->mp, &acc, Section, *c);
  }
  for(uint32_t i = 0; i < acc->len; i++) {
    Section *section = mp_vector_at(acc, Section, i);
    sema_default_args(a, section, b);
  }
  free_mp_vector(a->mp, Section, acc);
  return ok;
}

ANN static bool ext_fill(const Gwion gwion, MP_Vector **vec, const Tmpl *tmpl) {
  for(uint32_t i = tmpl->list->len - 1; i < tmpl->call->len; i++) {
    TmplArg targ = *mp_vector_at(tmpl->call, TmplArg, i);
    if(targ.type != tmplarg_td) {
      gwlog_error("invalid const expression in variadic template",
                  "can't use expression in spread",
                  gwion->env->name, targ.d.exp->loc, 0);
      Specialized *spec = mp_vector_at(tmpl->list, Specialized,
                                       tmpl->list->len - 1);
      gwlog_related("spread starts here", gwion->env->name, spec->tag.loc);
      return false;
    }
    mp_vector_add(gwion->mp, vec, Type_Decl*, targ.d.td);
  }
  return true;
}

ANN static bool spread_class_vector(const Env env, const Type t, MP_Vector **vec) {
  if(t->info->value->from->owner_class)
    CHECK_B(spread_class_vector(env, t->info->value->from->owner_class, vec));
  if(!tflag(t, tflag_cdef)) return true;
  Tmpl *tmpl = t->info->cdef->base.tmpl;
  // TODO: unallow spread tmpl outside of class/struct/func
  if(!tmpl || !is_spread_tmpl(tmpl)) return true;
  return ext_fill(env->gwion, vec, tmpl);
}

ANN bool sema_variadic_func(Env a, Func_Def fdef) {
   Sema sema = { .filename = a->name, .mp = a->gwion->mp, .st = a->gwion->st, .tmpls = new_mp_vector(a->gwion->mp, Type_Decl*, 0),
    .ppa = a->gwion->ppa };
  if(a->class_def)
    spread_class_vector(a, a->class_def, &sema.tmpls);
  bool ret = sema_func_def(&sema, fdef);
  free_mp_vector(a->gwion->mp, Type_Decl*, sema.tmpls);
  return ret;
}


ANN bool sema_variadic_class(Env a, Class_Def cdef) {
   Sema sema = { .filename = a->name, .mp = a->gwion->mp, .st = a->gwion->st, .tmpls = new_mp_vector(a->gwion->mp, Type_Decl*, 0),
    .ppa = a->gwion->ppa};
   if(a->class_def)
     spread_class_vector(a, a->class_def, &sema.tmpls);
  bool ret = sema_class_def(&sema, cdef);
  free_mp_vector(a->gwion->mp, Type_Decl*, sema.tmpls);
  return ret;
}

ANN bool sema_pass(Env a, Ast *b) {
  Sema sema = { .filename = a->name, .mp = a->gwion->mp, .st = a->gwion->st,
    .ppa = a->gwion->ppa};
  /*return*/ sema_ast(&sema, b);
  return !sema.error;
}
