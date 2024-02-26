#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "template.h"
#include "traverse.h"
#include "parse.h"
#include "operator.h"
#include "gwion.h"
#include "object.h"
#include "instr.h"
#include "import.h"
#include "default_args.h"
#include "spread.h"
#include "closure.h"

ANN static bool scan2_stmt(const Env, Stmt*);
ANN static bool scan2_stmt_list(const Env, Stmt_List);

ANN static inline bool ensure_scan2(const Env env, const Type t) {
  if (tflag(t, tflag_scan2) || !(tflag(t, tflag_cdef) || tflag(t, tflag_udef)))
    return true;
  struct EnvSet es = {.env   = env,
                      .data  = env,
                      .func  = (_envset_func)scan2_cdef,
                      .scope = env->scope->depth,
                      .flag  = tflag_scan2};
  return envset_run(&es, t);
}

ANN static bool scan2_decl(const Env env, const Exp_Decl *decl) {
  const Type t = decl->type;
  CHECK_B(ensure_scan2(env, t));
  const Var_Decl vd   = decl->var.vd;
  nspc_add_value(env->curr, vd.tag.sym, vd.value);
  return true;
}

ANN bool scan2_exp_decl(const Env env, const Exp_Decl *decl) {
  if(decl->args) CHECK_B(scan2_exp(env, decl->args));
  const bool   global = GET_FLAG(decl->var.td, global);
  const m_uint scope  = !global ? env->scope->depth : env_push_global(env);
  const bool ret    = scan2_decl(env, decl);
  if (global) env_pop(env, scope);
  return ret;
}

ANN static bool scan2_args(const Func_Def f) {
  Arg_List   args   = f->base->args;
  for(uint32_t i = 0; i < args->len; i++) {
    Arg *arg = mp_vector_at(args, Arg, i);
    const Value v   = arg->var.vd.value;
    v->from->offset = f->stack_depth;
    f->stack_depth += v->type->size;
    set_vflag(v, vflag_arg);
  }
  return true;
}

ANN static Value scan2_func_assign(const Env env, const Func_Def d,
                                   const Func f, const Value v) {
  valuefrom(env, v->from);
  SET_FLAG(v, const);
  set_vflag(v, vflag_func);
  if (!env->class_def) {
    if (!GET_FLAG(d->base, global)) set_vflag(v, vflag_fglobal);
  } else {
    if (GET_FLAG(d->base, static))
      SET_FLAG(v, static);
    else set_vflag(v, vflag_member);
    SET_ACCESS(d->base, v)
  }
  d->base->func = v->d.func_ref = f;
  return f->value_ref           = v;
}

ANN bool scan2_fptr_def(const Env env NUSED, const Fptr_Def fptr) {
  if(fptr->cdef->base.type->error) return false;
  if(GET_FLAG(fptr->cdef, global)) env_push_global(env);
  const bool ret = scan2_class_def(env, fptr->cdef);
  const Func_Def fdef = mp_vector_at(fptr->cdef->base.type->info->cdef->body, struct Section_ , 0)->d.func_def;
  if(fdef->base->func) set_fflag(fdef->base->func, fflag_fptr);
  else CHECK_B(tmpl_fptr(env, fptr, fdef));
  if(GET_FLAG(fptr->cdef, global)) env_pop(env, 0);
  return ret;
}

ANN static bool scan2_func_def_op(const Env env, const Func_Def f);
ANN bool        scan2_type_def(const Env env, const Type_Def tdef) {
  if (tdef->when) CHECK_B(scan2_exp(env, tdef->when));
  if (tflag(tdef->type, tflag_cdef))
    return scan2_class_def(env, tdef->type->info->cdef);
  if (!tdef->type->info->cdef) return true;
  return tdef->type->info->cdef ? scan2_class_def(env, tdef->type->info->cdef) : true;
}

ANN static bool scan2_range(const Env env, Range *range) {
  if (range->start) CHECK_B(scan2_exp(env, range->start));
  if (range->end) CHECK_B(scan2_exp(env, range->end));
  return true;
}

ANN static inline bool scan2_prim(const Env env, const Exp_Primary *prim) {
  if (prim->prim_type == ae_prim_hack || prim->prim_type == ae_prim_dict || prim->prim_type == ae_prim_interp)
    CHECK_B(scan2_exp(env, prim->d.exp));
  else if (prim->prim_type == ae_prim_array && prim->d.array->exp)
    return scan2_exp(env, prim->d.array->exp);
  else if (prim->prim_type == ae_prim_range)
    return scan2_range(env, prim->d.range);
  return true;
}

ANN static inline bool scan2_exp_array(const Env        env,
                                         const Exp_Array *array) {
  CHECK_B(scan2_exp(env, array->base));
  return scan2_exp(env, array->array->exp);
}

ANN static inline bool scan2_exp_slice(const Env env, const Exp_Slice *exp) {
  CHECK_B(scan2_exp(env, exp->base));
  return scan2_range(env, exp->range);
}

ANN static inline bool scan2_exp_binary(const Env         env,
                                          const Exp_Binary *bin) {
  CHECK_B(scan2_exp(env, bin->lhs));
  return scan2_exp(env, bin->rhs);
}

ANN static inline bool scan2_exp_cast(const Env env, const Exp_Cast *cast) {
  return scan2_exp(env, cast->exp);
}

ANN static inline bool scan2_exp_post(const Env          env,
                                        const Exp_Postfix *post) {
  return scan2_exp(env, post->exp);
}

ANN static inline bool scan2_exp_call(const Env       env,
                                        const Exp_Call *exp_call) {
  if (exp_call->tmpl) return true;
  CHECK_B(scan2_exp(env, exp_call->func));
  Exp* args = exp_call->args;
  return args ? scan2_exp(env, args) : true;
}

ANN static inline bool scan2_exp_dot(const Env env, const Exp_Dot *member) {
  return scan2_exp(env, member->base);
}

ANN static inline bool scan2_exp_if(const Env env, const Exp_If *exp_if) {
  CHECK_B(scan2_exp(env, exp_if->cond));
  CHECK_B(scan2_exp(env, exp_if->if_exp ?: exp_if->cond));
  return scan2_exp(env, exp_if->else_exp);
}

ANN static bool scan2_exp_unary(const Env env, const Exp_Unary *unary) {
  if (unary->unary_type == unary_exp)
    return scan2_exp(env, unary->exp);
  return true;
}

ANN static inline bool _scan2_stmt_match_case(const restrict Env env,
                                                const Stmt_Match   stmt) {
  CHECK_B(scan2_exp(env, stmt->cond));
  if (stmt->when) CHECK_B(scan2_exp(env, stmt->when));
  return scan2_stmt_list(env, stmt->list);
}

ANN static inline bool scan2_stmt_match_case(const restrict Env env,
                                               const Stmt_Match   stmt) {
    RET_NSPC(_scan2_stmt_match_case(env, stmt))}

ANN static inline bool
    _scan2_stmt_match(const restrict Env env, const Stmt_Match stmt) {
  if (stmt->where) CHECK_B(scan2_stmt(env, stmt->where));
  Stmt_List l = stmt->list;
  bool ok = true;
  for(m_uint i = 0; i < l->len; i++) {
    Stmt* stmt = mp_vector_at(l, Stmt, i);
    if(!scan2_stmt_match_case(env, &stmt->d.stmt_match))
      POISON_NODE(ok, env, stmt);
  }
  return ok;
}

ANN static inline bool scan2_handler(const restrict Env env,
                                            const Handler *handler) {
  RET_NSPC(scan2_stmt(env, handler->stmt));
}

ANN static inline bool scan2_handler_list(const restrict Env env,
                                            const Handler_List handlers) {
  bool ok = true;
  for(uint32_t i = 0; i < handlers->len; i++) {
    Handler * handler = mp_vector_at(handlers, Handler, i);
    if(!scan2_handler(env, handler))
      POISON(ok, env);
  }
  return ok;
}

ANN static inline bool scan2_stmt_try(const restrict Env env,
                                        const Stmt_Try     stmt) {
  CHECK_B(scan2_handler_list(env, stmt->handler));
  RET_NSPC(scan2_stmt(env, stmt->stmt))
}

ANN static inline bool scan2_stmt_match(const restrict Env env,
                                          const Stmt_Match   stmt) {
  CHECK_B(scan2_exp(env, stmt->cond));
  RET_NSPC(_scan2_stmt_match(env, stmt))
}

#define scan2_exp_lambda dummy_func
#define scan2_exp_td     dummy_func
HANDLE_EXP_FUNC(scan2, bool, Env)

#define scan2_stmt_func(name, type, prolog, exp)                               \
  describe_stmt_func(scan2, name, type, prolog, exp)
scan2_stmt_func(flow, Stmt_Flow,, !(!scan2_exp(env, stmt->cond) ||
    !scan2_stmt(env, stmt->body)) ? true : false)
scan2_stmt_func(for, Stmt_For,, !(!scan2_stmt(env, stmt->c1) ||
    !scan2_stmt(env, stmt->c2) ||
    (stmt->c3 && !scan2_exp(env, stmt->c3)) ||
    !scan2_stmt(env, stmt->body)) ? true : false)
scan2_stmt_func(each, Stmt_Each,, !(!scan2_exp(env, stmt->exp) ||
    !scan2_stmt(env, stmt->body)) ? true : false)
scan2_stmt_func(loop, Stmt_Loop,, !(!scan2_exp(env, stmt->cond) ||
    !scan2_stmt(env, stmt->body)) ? true : false)
scan2_stmt_func(if, Stmt_If,, !(!scan2_exp(env, stmt->cond) ||
    !scan2_stmt(env, stmt->if_body) ||
    (stmt->else_body && !scan2_stmt(env, stmt->else_body))) ? true : false)

ANN static inline bool scan2_stmt_code(const Env env, const Stmt_Code stmt) {
  if (stmt->stmt_list) { RET_NSPC(scan2_stmt_list(env, stmt->stmt_list)) }
  return true;
}

ANN static inline bool scan2_stmt_exp(const Env env, const Stmt_Exp stmt) {
  return stmt->val ? scan2_exp(env, stmt->val) : 1;
}

ANN bool scan2_union_def(const Env env NUSED, const Union_Def udef) {
  if (tmpl_base(udef->tmpl)) return true;
  set_tflag(udef->type, tflag_scan2);
  return true;
}

#define scan2_stmt_while    scan2_stmt_flow
#define scan2_stmt_until    scan2_stmt_flow
#define scan2_stmt_continue dummy_func
#define scan2_stmt_break    dummy_func
#define scan2_stmt_return   scan2_stmt_exp
#define scan2_stmt_retry    dummy_func

ANN static bool scan2_stmt_pp(const Env env, const Stmt_PP stmt) {
  if (stmt->pp_type == ae_pp_include) env->name = stmt->data;
  return true;
}

ANN static bool scan2_stmt_defer(const Env env, const Stmt_Defer stmt) {
  return scan2_stmt(env, stmt->stmt);
}

#define scan2_stmt_spread dummy_func

DECL_STMT_FUNC(scan2, bool, Env)

ANN static bool scan2_stmt(const Env env, Stmt* stmt) {
  return scan2_stmt_func[stmt->stmt_type](env, &stmt->d);
}

ANN static bool scan2_stmt_list(const Env env, Stmt_List l) {
  bool ok = true;
  for(m_uint i = 0; i < l->len; i++) {
    Stmt* stmt = mp_vector_at(l, Stmt, i);
    if(stmt->poison) { ok = false; continue; }
    if(!scan2_stmt(env, stmt))
      POISON_NODE(ok, env, stmt);
  }
  return ok;
}

ANN static bool scan2_func_def_overload(const Env env, const Func_Def f,
                                          const Value overload) {
  if (!is_func(env->gwion, overload->type)) {
    if (!fbflag(f->base, fbflag_internal))
      ERR_B(f->base->tag.loc,
            _("function name '%s' is already used by another value"),
            overload->name);
  }
  const Func obase = overload->d.func_ref;
  if (GET_FLAG(obase->def->base, final) && (!env->class_def || (obase->value_ref->from->owner_class != env->class_def))) {
    env_err(env, f->base->tag.loc, _("can't overload final function `{G}%s{0}`"), s_name(f->base->tag.sym));
    declared_here(obase->value_ref);
    return false;
  }
  const bool base = tmpl_base(f->base->tmpl);
  const bool tmpl = fflag(obase, fflag_tmpl);
  if ((!tmpl && base) || (tmpl && !base && !f->base->tmpl))
    ERR_B(f->base->tag.loc, _("must overload template function with template"));
  if (GET_FLAG(f->base, global) != GET_FLAG(obase->def->base, global))
    ERR_B(f->base->tag.loc, _("function is declared global")); // improve me
  return true;
}

ANN static Func scan_new_func(const Env env, const Func_Def f,
                              const m_str name) {
  const Func func = new_func(env->gwion->mp, name, f);
  if (env->class_def && tflag(env->class_def, tflag_tmpl))
    set_fflag(func, fflag_ftmpl);
  if (fbflag(f->base, fbflag_lambda)) {
    if(env->class_def) env->class_def->info->values = env->curr->info->value;
    env->curr->info->value = new_scope(env->gwion->mp);
  }
  return func;
}

ANN static Type func_type(const Env env, const Func func) {
  const Type base = env->gwion->type[et_function];
  const Type t    = type_copy(env->gwion->mp, base);
  t->info->parent = base;
  t->name         = func->name;
  t->info->func   = func;
  return t;
}

ANN static void func_no_overload(const Env env, const Func f, const Value v) {
  const Type t = v->type;
  value_addref(v);
  nspc_add_value_front(env->curr, f->def->base->tag.sym, v);

  const Type newt  = type_copy(env->gwion->mp, t);
  t->info->parent  = newt;
  newt->name       = s_name(f->def->base->tag.sym);
  newt->info->func = f;
  nspc_add_type_front(env->curr, f->def->base->tag.sym, newt);
  newt->info->value = v;
}

ANN2(1, 2)
static Value func_value(const Env env, const Func f, const Value overload) {
  const Type  t = func_type(env, f);
  const Symbol sym = insert_symbol(t->name);
  const Value v = t->info->value = new_value(env, t, MK_TAG(sym, f->def->base->tag.loc));
  valuefrom(env, v->from);
  CHECK_O(scan2_func_assign(env, f->def, f, v));
  if (!overload)
    func_no_overload(env, f, v);
  else if (overload->d.func_ref) {
    f->next                    = overload->d.func_ref->next;
    overload->d.func_ref->next = f;
  }
  if (env->class_def && !GET_FLAG(f->def->base, static))
    set_vflag(v, vflag_member);
  return v;
}

ANN2(1, 2)
static bool scan2_fdef_tmpl(const Env env, const Func_Def f,
                              const Value overload) {
  const m_str name  = s_name(f->base->tag.sym);
  const Func  func  = scan_new_func(env, f, name);
  DECL_B(const Value, value, = func_value(env, func, overload));
  set_fflag(func, fflag_tmpl);
  set_vflag(value, vflag_valid);
  set_tflag(value->type,
            tflag_ftmpl); // the only types with func flag, name could be better
  Type type = env->class_def;
  Nspc nspc = env->curr;
  uint i    = 0;
  do {
    const Value v = nspc_lookup_value0(nspc, f->base->tag.sym);
    if (v) {
      Func ff = v->d.func_ref;
      if (!ff) continue;
      do {
        if (ff->def == f) {
          ++i;
          continue;
        }
        if (compat_func(ff->def, f)) {
          if (ff->value_ref->from->owner == env->curr)
            ERR_B(f->base->tag.loc,
                  "template function '%s' already defined with those arguments "
                  "in this namespace",
                  name);
          const Symbol sym =
              func_symbol(env, env->curr->name, name, "template", ff->def->vt_index);
          nspc_add_value(env->curr, sym, value);
          if (!overload) nspc_add_value(env->curr, f->base->tag.sym, value);
          nspc_add_func(env->curr, sym, func);
          func->def->vt_index = ff->def->vt_index;
          return true;
        }
      } while ((ff = ff->next) && ++i);
    }
  } while (type && (type = type->info->parent) && (nspc = type->nspc));
  --i;
  const Symbol sym = func_symbol(env, env->curr->name, name, "template", i);
  nspc_add_value(env->curr, sym, value);
  nspc_add_func(env->curr, sym, func);
  if (!overload) nspc_add_value(env->curr, f->base->tag.sym, value);
  else func->def->vt_index = ++overload->from->offset;
  return true;
}

ANN static bool scan2_func_def_op(const Env env, const Func_Def f) {
  const m_str      str    = s_name(f->base->tag.sym);
  struct Op_Func   opfunc = {.ck = strcmp(str, "@implicit") ? 0
                                                            : opck_usr_implicit};
  struct Op_Import opi    = {.ret  = f->base->ret_type,
                          .loc  = f->base->tag.loc,
                          .data = (uintptr_t)f->base->func,
                          .func = &opfunc};
  func_operator(f, &opi);
  CHECK_B(add_op(env->gwion, &opi));
  operator_set_func(&opi);
  return true;
}

ANN static bool scan2_func_def_code(const Env env, const Func_Def f) {
  const Func former = env->func;
  env->func         = f->base->func;
  const bool ctor = is_ctor(f);
  if(!ctor) {
    env->scope->depth++;
    nspc_push_value(env->gwion->mp, env->curr);
  }
  const bool ret = scan2_stmt_list(env, f->d.code); // scope depth?
  if(!ctor) {
    nspc_pop_value(env->gwion->mp, env->curr);
    env->scope->depth--;
  }
  env->func = former;
  return ret;
}

ANN static void scan2_func_def_flag(const Env env, const Func_Def f) {
  set_fflag(f->base->func, fflag_pure);
  if (f->base->tag.sym == insert_symbol("@dtor"))
    set_tflag(env->class_def, tflag_dtor);
}


ANN static m_str func_tmpl_name(const Env env, const Func_Def f) {
  const m_str      name = s_name(f->base->tag.sym);
  m_str tmpl_name = tl2str(env->gwion, f->base->tmpl->call, f->base->tag.loc);
  const Symbol sym = func_symbol(env, env->curr->name, name, tmpl_name,
                                 (m_uint)f->vt_index);
  return s_name(sym);
}

ANN2(1, 2, 4)
static Value func_create(const Env env, const Func_Def f, const Value overload,
                         const m_str name) {
  DECL_B(const Func, func, = scan_new_func(env, f, name));
  nspc_add_func(env->curr, insert_symbol(func->name), func);
  DECL_B(const Value, v, = func_value(env, func, overload));
  scan2_func_def_flag(env, f);
  nspc_add_value(env->curr, insert_symbol(func->name), v);
  return v;
}

ANN2(1, 2)
static m_str func_name(const Env env, const Func_Def f, const Value v) {
  if (!f->base->tmpl) {
    const Symbol sym = func_symbol(env, env->curr->name, s_name(f->base->tag.sym),
                                   NULL, v ? ++v->from->offset : 0);
    return s_name(sym);
  }
  return f->base->func ? f->base->func->name : func_tmpl_name(env, f);
}

ANN2(1, 2)
bool scan2_fdef_std(const Env env, const Func_Def f, const Value overload) {
  const m_str name = func_name(env, f, overload);
  if (!name) return false;
  const Func base = f->base->func;
  if (!base)
    CHECK_B(func_create(env, f, overload, name));
  if (f->base->args) CHECK_B(scan2_args(f));
  if (!f->builtin && f->d.code) (void)scan2_func_def_code(env, f);
  if (!base) {
    if (fbflag(f->base, fbflag_op)) CHECK_B(scan2_func_def_op(env, f));
    set_vflag(f->base->func->value_ref, vflag_valid);
  }
  if (f->base->tmpl) set_fflag(f->base->func, fflag_tmpl);
  return true;
}

//! use function from parent class as next.
ANN static void upfunction(const Env env, const Func_Base *fb) {
  const Value v = find_value(env->class_def->info->parent, fb->tag.sym);
  if (!v) return;
  Func func = fb->func;
  while (func->next && func->next->value_ref->from->owner == env->curr)
    func = func->next;
  func->next = v->d.func_ref;
}

ANN bool scan2_fdef(const Env env, const Func_Def fdef) {
  const Value overload = nspc_lookup_value2(env->curr, fdef->base->tag.sym);
  if (overload) CHECK_B(scan2_func_def_overload(env, fdef, overload));
  CHECK_B((!tmpl_base(fdef->base->tmpl) ? scan2_fdef_std : scan2_fdef_tmpl)(
      env, fdef, overload));
  if (env->class_def && !fdef->base->func->next) upfunction(env, fdef->base);
  return true;
}

__attribute__((returns_nonnull)) static ANN Func_Def
scan2_cpy_fdef(const Env env, const Func_Def fdef) {
  const Func_Def f  = cpy_func_def(env->gwion->mp, fdef);
  f->base->ret_type = fdef->base->ret_type;
  scan0_func_def(env, f);
  scan1_func_def(env, f);
  return f;
}

static inline int is_cpy(const Func_Def fdef) {
  return GET_FLAG(fdef->base, global) ||
         (fdef->base->tmpl && !fdef->base->tmpl->call);
}

ANN bool _scan2_func_def(const Env env, const Func_Def fdef) {
  if (tmpl_base(fdef->base->tmpl) && fbflag(fdef->base, fbflag_op))
    return true;
  if(is_new(fdef)) {
    if(!env->class_def)
      ERR_B(fdef->base->tag.loc, _("{G+}new{0} operator must be set inside {C+}class{0}"));
    SET_FLAG(env->class_def, abstract);
    if(!fdef->base->ret_type)
//      fdef->base->ret_type = env->class_def;
      fdef->base->ret_type = env->gwion->type[et_auto];
  }
  if (GET_FLAG(fdef->base, global) && !env->class_def) env->context->global = 1;
  const Func_Def f = !is_cpy(fdef) ? fdef : scan2_cpy_fdef(env, fdef);
  const m_uint   scope =
      !GET_FLAG(f->base, global) ? env->scope->depth : env_push_global(env);
  f->stack_depth = (env->class_def && !GET_FLAG(f->base, static) &&
                    !GET_FLAG(f->base, global))
                       ? SZ_INT
                       : 0;
  const bool ret = scanx_fdef(env, env, f, (_envset_func)scan2_fdef);
  if (GET_FLAG(f->base, global)) env_pop(env, scope);
  CHECK_B(ret);
  fdef->base->func = f->base->func; // only needed if 'is_cpy()'
  return true;
}

ANN bool scan2_func_def(const Env env, const Func_Def fdef) {
  const uint16_t depth = env->scope->depth;
  env->scope->depth = 0;
  const bool ret = _scan2_func_def(env, fdef);
  env->scope->depth = depth;
  return ret;
}

#define scan2_enum_def  dummy_func
#define scan2_trait_def dummy_func
#define scan2_extend_def dummy_func
#define scan2_prim_def dummy_func
HANDLE_SECTION_FUNC(scan2, bool, Env)

ANN static bool scan2_parent(const Env env, const Class_Def cdef) {
  if (cdef->base.ext->array && cdef->base.ext->array->exp)
    CHECK_B(scan2_exp(env, cdef->base.ext->array->exp));
  return true;
}

ANN static bool cdef_parent(const Env env, const Class_Def cdef) {
  const bool tmpl = !!cdef->base.tmpl;
  if (tmpl) CHECK_B(template_push_types(env, cdef->base.tmpl));
  const bool ret = scan2_parent(env, cdef);
  if (tmpl) nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}

ANN bool scan2_class_body(const Env env, const Class_Def c) {
  const Tmpl *tmpl = c->base.tmpl;
  bool ok = true;
  if(tmpl && tmplarg_ntypes(tmpl->list) != tmpl->list->len) {
    for(uint32_t i = 0; i < tmpl->list->len; i++) {
      const TmplArg targ = *mp_vector_at(tmpl->call, TmplArg, i);
      if(unlikely(targ.type != tmplarg_td)) {
        if(!scan2_exp(env, targ.d.exp))
          POISON_NODE(ok, env, targ.d.exp);
      }
    }
  }
//  return scan2_ast(env, &c->body) && ok;
  scan2_ast(env, &c->body);
  return ok;
}

ANN bool scan2_class_def(const Env env, const Class_Def cdef) {
  if (tmpl_base(cdef->base.tmpl)) return true;
  const Type      t = cdef->base.type;
  const Class_Def c = t->info->cdef;
  if (tflag(t, tflag_scan2)) return true;
  set_tflag(t, tflag_scan2);
  if (c->base.ext) CHECK_B(cdef_parent(env, c));
  if (c->body) {
    const m_uint scope = env_push_type(env, t);
    const Tmpl *tmpl = cdef->base.tmpl;
    if(tmpl && tmpl->call && tmpl->call != (TmplArg_List)1 && tmpl->list)
      template_push_types(env, tmpl);
    const bool ret = scan2_class_body(env, c);
    if(tmpl && tmpl->call && tmpl->call != (TmplArg_List)1 && tmpl->list)
      nspc_pop_type(env->gwion->mp, env->curr);
    env_pop(env, scope);
    return ret;
  }
  return true;
}

ANN void scan2_default_args(const Env env, const Section *s, Ast *acc) {
  Func_Base *const fb = s->d.func_def->base;
  Arg_List       args = fb->args;
  uint32_t len = args->len;
  while(args->len--) {
    const Arg *arg = mp_vector_at(args, Arg, args->len);
    if(!arg->exp) break;
    const Func_Def fdef = default_args(env, fb, acc, len);
    scan1_func_def(env, fdef);
    scan2_func_def(env, fdef);
  }
  args->len = len;
}

ANN bool scan2_ast(const Env env, Ast *ast) {
  Ast a = *ast;
  Ast acc = new_mp_vector(env->gwion->mp, Section, 0);
  bool ok = true;
  for(m_uint i = 0; i < a->len; i++) {
    Section *section = mp_vector_at(a, Section, i);
    if(section->poison) { ok = false; continue;}
    if(!scan2_section(env, section)) {
      POISON_SECTION(ok, env, section);
      continue;
    }
    if (section->section_type == ae_section_func &&
        fbflag(section->d.func_def->base, fbflag_default))
      mp_vector_add(env->gwion->mp, &acc, Section, *section);
  }
  if(ok) {
    for(uint32_t i = 0; i < acc->len; i++) {
      Section *section = mp_vector_at(acc, Section, i);
      scan2_default_args(env, section, ast);
    }
  }
  free_mp_vector(env->gwion->mp, Section, acc);
  return ok;
}
