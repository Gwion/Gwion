#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "validate.h"
#define __NO_EMIT
#include "specialid.h"

typedef struct {
  Env env;
  uint32_t jumps;
} Validate;

ANN static bool validate_type_decl(Validate *a, Type_Decl *b);
ANN static bool validate_exp(Validate *a, Exp* b);
ANN static bool validate_stmt(Validate *a, Stmt* b);
ANN static bool validate_stmt_list(Validate *a, Stmt_List b);
ANN static bool validate_func_def(Validate *a, Func_Def b);
ANN static bool validate_type_def(Validate *a, Type_Def b);
ANN static bool validate_ast(Validate *a, Ast b);

#define CHECK_RET(a, ret) \
do {                      \
  if(!(a))                \
    ret = false;          \
} while(0) 

ANN static bool validate_array_sub(Validate *a, Array_Sub b) {
  return b->exp ? validate_exp(a, b->exp) : true;
}

ANN static bool validate_specialized(Validate *a, Specialized *b) {
  return b->td ? validate_type_decl(a, b->td) : true;
}

ANN static bool validate_specialized_list(Validate *a, Specialized_List b) {
  bool ret = true;
  for(uint32_t i = 0; i < b->len; i++) {
    Specialized * c = mp_vector_at(b, Specialized  , i);
    if(!validate_specialized(a, c))
      ret = false;
  }
  return ret;
}

ANN static bool validate_tmplarg(Validate *a, TmplArg *b) {
  bool ret = true;
  if (b->type == tmplarg_td)
    CHECK_RET(validate_type_decl(a, b->d.td), ret);
  else CHECK_RET(validate_exp(a, b->d.exp), ret);
  return ret;
}

ANN static bool validate_tmplarg_list(Validate *a, TmplArg_List b) {
  bool ret = true;
  for(uint32_t i = 0; i < b->len; i++) {
    TmplArg *c = mp_vector_at(b, TmplArg, i);
    if(!validate_tmplarg(a, c))
      ret = false;
  }
  return ret;
}

ANN static bool validate_tmpl(Validate *a, Tmpl *b) {
  bool ret = true;
  if(b->list) CHECK_RET(validate_specialized_list(a, b->list), ret);
  if(b->call) CHECK_RET(validate_tmplarg_list(a, b->call), ret);
  return ret;
}

ANN static bool validate_range(Validate *a, Range *b) {
  bool ret = true;
  if(b->start) CHECK_RET(validate_exp(a, b->start), ret);
  if(b->end) CHECK_RET(validate_exp(a , b->end), ret);
  return ret;
}

ANN static bool validate_type_decl(Validate *a, Type_Decl *b) {
  bool ret = true;
  if(b->array) CHECK_RET(validate_array_sub(a, b->array), ret);
  if(b->types) CHECK_RET(validate_tmplarg_list(a, b->types), ret);
  return ret;
}

ANN static bool validate_prim_id(Validate *a, Exp_Primary *b) {
  struct SpecialId_ *spid = specialid_get(a->env->gwion, b->d.var);
  if(spid) return true;
  if (!b->value) {// assume it's an operator
    // turns out it could be a _ in a case expression
    env_err(a->env, exp_self(b)->loc, "missing value for operator");
    return false;
  }
  return true;
}

ANN static bool validate_prim(Validate *a, Exp_Primary *b) {
  if(b->prim_type == ae_prim_id)
    return validate_prim_id(a, b);
  if(b->prim_type == ae_prim_dict ||
     b->prim_type == ae_prim_hack ||
     b->prim_type == ae_prim_interp)
    return validate_exp(a, b->d.exp);
  if(b->prim_type == ae_prim_array)
    return validate_array_sub(a, b->d.array);
  if(b->prim_type == ae_prim_range)
    return validate_range(a, b->d.range);
  return true;
}

ANN static bool validate_variable(Validate *a, Variable *b) {
  return b->td ? validate_type_decl(a, b->td) : true;
}

ANN static void set_late(const Exp_Decl *decl, const Var_Decl *var) {
  const Value v         = var->value;
  if (!decl->args &&
      (GET_FLAG(array_base_simple(v->type), abstract) || GET_FLAG(decl->var.td, late)))
    SET_FLAG(v, late);
  else UNSET_FLAG(v, late);
}

static inline bool late_array(const Type_Decl* td) {
  return !td->array || !td->array->exp;
}

ANN static bool validate_exp_decl(Validate *a, Exp_Decl *b) {
  if(b->args && !strncmp(b->args->type->name, "partial:", 8)) {
    env_err(a->env, b->args->loc, "unresolved partial");
    return false;
  }
  CHECK_B(validate_variable(a, &b->var));
  Var_Decl *vd = &b->var.vd;
  Value v = vd->value;
  if(!v) return true;
  if(is_object(a->env->gwion, v->type) && !b->args) {
    set_late(b, vd);
  if (!exp_getvar(exp_self(b)) && GET_FLAG(array_base_simple(v->type), abstract) && !GET_FLAG(b->var.td, late) &&
      /*GET_FLAG(v, late) && */late_array(b->var.td)
      && GET_FLAG(v->type, abstract)) {
    env_err(a->env, b->var.td->tag.loc, _("Type '%s' is abstract, use {+G}late{0} instead of {G+}%s{0}"),
             v->type->name, !GET_FLAG(b->var.td, const) ? "var" : "const");
    if(v->type->nspc->vtable.ptr) {
      bool has_new = false;
      const Vector vec = &v->type->nspc->vtable;
      for(m_uint i = 0; i < vector_size(vec); i++) {
        const Func f = (Func)vector_at(vec, i);
        if(is_new(f->def)) {
          if(!has_new) {
            gwlog_hint(_("maybe use a constructor?"), a->env->name, b->var.td->tag.loc);
            has_new = true;
          }
          print_signature(a->env->gwion, f);
        }
      }
    }
    return false;
  }
  }
  return true;
}

ANN static bool validate_exp_binary(Validate *a, Exp_Binary *b) {
  bool ret = true;
  CHECK_RET(validate_exp(a, b->lhs), ret);
  CHECK_RET(validate_exp(a, b->rhs), ret);
  return ret;
}

ANN static bool validate_exp_unary(Validate *a, Exp_Unary *b) {
  bool ret = true;
  const enum unary_type type = b->unary_type;
  if(type == unary_exp)
    CHECK_RET(validate_exp(a, b->exp), ret);
  else if(type == unary_td)
    CHECK_RET(validate_type_decl(a, b->ctor.td), ret);
  else
    CHECK_RET(validate_stmt_list(a, b->code), ret);
  return ret;
}

ANN static bool validate_exp_cast(Validate *a, Exp_Cast *b) {
  bool ret = true;
  CHECK_RET(validate_type_decl(a, b->td), ret);
  CHECK_RET(validate_exp(a, b->exp), ret);
  return ret;
}

ANN static bool validate_exp_post(Validate *a, Exp_Postfix *b) {
  return validate_exp(a, b->exp);
}

ANN static bool validate_exp_call(Validate *a, Exp_Call *b) {
  bool ret = true;
  CHECK_RET(validate_exp(a, b->func), ret);
  if(b->args) CHECK_RET(validate_exp(a, b->args), ret);
  if(b->tmpl) CHECK_RET(validate_tmpl(a, b->tmpl), ret);
  return ret;
}

ANN static bool validate_exp_array(Validate *a, Exp_Array *b) {
  bool ret = true;
  CHECK_RET(validate_exp(a, b->base), ret);
  CHECK_RET(validate_array_sub(a, b->array), ret);
  return ret;
}

ANN static bool validate_exp_slice(Validate *a, Exp_Slice *b) {
  bool ret = true;
  CHECK_RET(validate_exp(a, b->base), ret);
  CHECK_RET(validate_range(a, b->range), ret);
  return ret;
}

ANN static bool validate_exp_if(Validate *a, Exp_If *b) {
  bool ret = true;
  CHECK_RET(validate_exp(a, b->cond), ret);
  if(b->if_exp) CHECK_RET(validate_exp(a, b->if_exp), ret);
  CHECK_RET(validate_exp(a, b->else_exp), ret);
  return ret;
}

ANN static bool validate_exp_dot(Validate *a, Exp_Dot *b) {
  return validate_exp(a, b->base);
}

ANN static bool validate_exp_lambda(Validate *a, Exp_Lambda *b) {
  return validate_func_def(a, b->def);
}

ANN static bool validate_exp_td(Validate *a, Type_Decl *b) {
  return validate_type_decl(a, b);
}

DECL_EXP_FUNC(validate, bool, Validate*)
ANN static bool validate_exp(Validate *a, Exp* b) {
  bool ret = true;
  if(!b->poison)
    CHECK_RET(validate_exp_func[b->exp_type](a, &b->d), ret);
  if(b->next) CHECK_RET(validate_exp(a, b ->next), ret);
  return ret;
}

ANN static bool validate_stmt_exp(Validate *a, Stmt_Exp b) {
  bool ret = true;
  if(b->val) CHECK_RET(validate_exp(a,  b->val), ret);
  return ret;
}

ANN static bool validate_stmt_jump(Validate *a, Stmt *stmt) {
  a->jumps++;
  const bool ret = validate_stmt(a, stmt);
  a->jumps--;
  return ret;
}

ANN static bool validate_stmt_flow(Validate *a, Stmt_Flow b) {
  bool ret = true;
  CHECK_RET(validate_exp(a, b->cond), ret);
  CHECK_RET(validate_stmt_jump(a, b->body), ret);
  return ret;
}
#define validate_stmt_while validate_stmt_flow
#define validate_stmt_until validate_stmt_flow

ANN static bool validate_stmt_for(Validate *a, Stmt_For b) {
  CHECK_B(validate_stmt(a, b->c1));
  if(b->c2) CHECK_B(validate_stmt(a, b->c2));
  if(b->c3) CHECK_B(validate_exp(a, b->c3));
  CHECK_B(validate_stmt_jump(a, b->body));
  return true;
}

ANN static bool validate_stmt_each(Validate *a, Stmt_Each b) {
  bool ret = true;
  CHECK_RET(validate_exp(a, b->exp), ret);
  CHECK_RET(validate_stmt_jump(a, b->body), ret);
  return ret;
}

ANN static bool validate_stmt_loop(Validate *a, Stmt_Loop b) {
  bool ret = true;
  CHECK_RET(validate_exp(a,  b->cond), ret);
  CHECK_RET(validate_stmt_jump(a, b->body), ret);
  return ret;
}

ANN static bool validate_stmt_if(Validate *a, Stmt_If b) {
  bool ret = true;
  CHECK_RET(validate_exp(a,  b->cond), ret);
  CHECK_RET(validate_stmt(a, b->if_body), ret);
  if(b->else_body) CHECK_RET(validate_stmt(a, b->else_body), ret);
  return ret;
}

ANN static bool validate_stmt_code(Validate *a, Stmt_Code b) {
  bool ret = true;
  if(b->stmt_list) CHECK_RET(validate_stmt_list(a, b->stmt_list), ret);
  return ret;
}

ANN static inline bool validate_stmt_index(Validate *a, const Stmt_Index stmt) {
  if (stmt->idx > a->jumps) {
    env_err(a->env, stmt_self(stmt)->loc, _("too many jumps required."));
    return false;
  }
  return true;
}

ANN static bool validate_stmt_break(Validate *a, Stmt_Index b) {
  return validate_stmt_index(a, b);
}

ANN static bool validate_stmt_continue(Validate *a, Stmt_Index b) {
  return validate_stmt_index(a, b);
}

ANN static bool validate_stmt_return(Validate *a, Stmt_Exp b) {
  bool ret = true;
  if(b->val) CHECK_RET(validate_exp(a, b-> val), ret);
  return ret;
}

ANN static bool validate_stmt_case(Validate *a, Stmt_Match b) {
  bool ret = true;
  CHECK_RET(validate_exp(a, b->cond), ret);
  CHECK_RET(validate_stmt_list(a, b->list), ret);
  if(b->when) CHECK_RET(validate_exp(a, b->when), ret);
  return ret;
}

ANN static bool validate_case_list(Validate *a, Stmt_List b) {
  bool ret = true;
  for(uint32_t i = 0; i < b->len; i++) {
    Stmt* c = mp_vector_at(b, Stmt, i);
    if(!validate_stmt_case(a, &c->d.stmt_match))
      ret = false;
  }
  return ret;
}

ANN static bool validate_stmt_match(Validate *a, Stmt_Match b) {
  bool ret = true;
  CHECK_RET(validate_exp(a, b->cond), ret);
  CHECK_RET(validate_case_list(a, b->list), ret);
  if(b->where) CHECK_RET(validate_stmt(a, b->where), ret);
  return ret;
}

ANN static bool validate_stmt_pp(Validate *a NUSED, Stmt_PP b NUSED) {
  if(b->pp_type == ae_pp_include)
    a->env->name = b->data;
  return true;
}

#define validate_stmt_retry dummy_func

ANN static bool validate_handler(Validate *a, Handler *b) {
  return validate_stmt(a, b->stmt);
}

ANN static bool validate_handler_list(Validate *a, Handler_List b) {
  bool ret = true;
  for(uint32_t i = 0; i < b->len; i++) {
    Handler *handler = mp_vector_at(b, Handler, i);
    if(!validate_handler(a, handler))
      ret = false;
  }
  return ret;
}

ANN static bool validate_stmt_try(Validate *a, Stmt_Try b) {
  bool ret = true;
  CHECK_RET(validate_stmt(a, b->stmt), ret);
  CHECK_RET(validate_handler_list(a, b->handler), ret);
  return ret;
}

ANN static bool validate_stmt_defer(Validate *a, Stmt_Defer b) {
  return validate_stmt(a, b->stmt);
}

ANN static bool validate_stmt_spread(Validate *a NUSED, Spread_Def b NUSED) {
  return true;
}

#define validate_stmt_using dummy_func
#define validate_stmt_import dummy_func

DECL_STMT_FUNC(validate, bool, Validate*)
ANN static bool validate_stmt(Validate *a, Stmt* b) {
  if(b->poison) return false;
  if(!validate_stmt_func[b->stmt_type](a, &b->d))
    b->poison = true;
  return !b->poison;
}

ANN static bool validate_arg(Validate *a, Arg *b) {
  return validate_variable(a, &b->var);
}

ANN static bool validate_arg_list(Validate *a, Arg_List b) {
  bool ret = true;
  for(uint32_t i = 0; i < b->len; i++) {
    Arg *c = mp_vector_at(b, Arg, i);
    if(!validate_arg(a, c))
      ret = false;
  }
  return ret;
}

ANN static bool validate_variable_list(Validate *a, Variable_List b) {
  bool ret = true;
  for(uint32_t i = 0; i < b->len; i++) {
    Variable *c = mp_vector_at(b, Variable, i);
    if(!validate_variable(a, c))
      return true;
  }
  return ret;
}

ANN static bool validate_stmt_list(Validate *a, Stmt_List b) {
  bool ret = true;
  for(uint32_t i = 0; i < b->len; i++) {
    Stmt* c = mp_vector_at(b, Stmt, i);
    if(!validate_stmt(a, c))
      ret = false;
  }
  return ret;
}

ANN static bool validate_func_base(Validate *a, Func_Base *b) {
  bool ret = true;
  if(b->td)   CHECK_RET(validate_type_decl(a, b->td), ret);
  if(b->args) CHECK_RET(validate_arg_list(a, b->args), ret);
  if(b->tmpl) CHECK_RET(validate_tmpl(a, b->tmpl), ret);
  return ret;
}

ANN static bool validate_func_def(Validate *a, Func_Def b) {
  bool ret = true;
  CHECK_RET(validate_func_base(a, b->base), ret);
  if(!b->builtin && b->d.code)
    CHECK_RET(validate_stmt_list(a, b->d.code), ret);
  return ret;
}

ANN static bool validate_class_def(Validate *a, Class_Def b) {
  bool ret = true;
  CHECK_RET(validate_type_def(a, &b->base), ret);
  if(b->body) return validate_ast(a, b->body);
  return ret;
}

ANN static bool validate_trait_def(Validate *a, Trait_Def b) {
  bool ret = true;
  if(b->body) CHECK_RET(validate_ast(a, b->body), ret);
  return ret;
}

#define validate_enum_def dummy_func

ANN static bool validate_union_def(Validate *a, Union_Def b) {
  bool ret = true;
  CHECK_RET(validate_variable_list(a, b->l), ret);
  if(b->tmpl) CHECK_RET(validate_tmpl(a, b->tmpl), ret);
  return ret;
}

ANN static bool validate_fptr_def(Validate *a, Fptr_Def b) {
  return validate_func_base(a, b->base);
}

ANN static bool validate_type_def(Validate *a, Type_Def b) {
  bool ret = true;
  if(b->ext)  CHECK_RET(validate_type_decl(a, b->ext), ret);
  if(b->tmpl) CHECK_RET(validate_tmpl(a, b->tmpl), ret);
  if(b->when) CHECK_RET(validate_exp(a, b->when), ret);
  return ret;
}

ANN static bool validate_extend_def(Validate *a, Extend_Def b) {
  return validate_type_decl(a, b->td);
}

#define validate_prim_def dummy_func

DECL_SECTION_FUNC(validate, bool, Validate*)
ANN static bool validate_section(Validate *a, Section *b) {
  if(b->poison) return false;
  if(!validate_section_func[b->section_type](a, *(void**)&b->d))
    b->poison = true;
  return !b->poison;
}

ANN static bool validate_ast(Validate *a, Ast b) {
  bool ret = true;
  for(uint32_t i = 0; i < b->len; i++) {
    Section *c = mp_vector_at(b, Section, i);
    if(!validate_section(a, c))
      ret = false;
  }
  return ret;
}

ANN bool exp_validation(Env env, Exp* b) {
  Validate a = { .env = env };
  return validate_exp(&a , b);
}

ANN bool func_def_validation(Env env, Func_Def b) {
  Validate a = { .env = env };
  return validate_func_def(&a , b);
}

ANN bool union_def_validation(Env env, Union_Def b) {
  Validate a = { .env = env };
  return validate_union_def(&a , b);
}

ANN bool type_def_validation(Env env, Type_Def b) {
  Validate a = { .env = env };
  return validate_type_def(&a , b);
}

ANN bool enum_def_validation(Env env, Enum_Def b) {
  Validate a = { .env = env };
  return validate_enum_def(&a , b);
}

ANN bool fptr_def_validation(Env env, Fptr_Def b) {
  Validate a = { .env = env };
  return validate_fptr_def(&a , b);
}

ANN bool class_def_validation(Env env, Class_Def b) {
  Validate a = { .env = env };
  return validate_class_def(&a , b);
}

ANN bool ast_validation(Env env, Ast *ast) {
  Validate a = { .env = env };
  return validate_ast(&a , *ast);
}
