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

ANN static m_bool scan2_stmt(const Env, const Stmt);
ANN static m_bool scan2_stmt_list(const Env, Stmt_List);

ANN static inline m_bool ensure_scan2(const Env env, const Type t) {
  if(tflag(t, tflag_scan2) || !(tflag(t, tflag_cdef) || tflag(t, tflag_udef)))
    return GW_OK;
  struct EnvSet es = { .env=env, .data=env, .func=(_exp_func)scan2_cdef,
    .scope=env->scope->depth, .flag=tflag_scan2 };
  return envset_run(&es, t);
}

ANN static m_bool scan2_decl(const Env env, const Exp_Decl* decl) {
  const Type t = decl->type;
  CHECK_BB(ensure_scan2(env, t));
  Var_Decl_List list = decl->list;
  do {
    const Var_Decl var = list->self;
    const Exp array = var->array ? var->array->exp : NULL;
    if(array)
      CHECK_BB(scan2_exp(env, array));
    nspc_add_value(env->curr, var->xid, var->value);
  } while((list = list->next));
  return GW_OK;
}

ANN m_bool scan2_exp_decl(const Env env, const Exp_Decl* decl) {
  const bool global = GET_FLAG(decl->td, global);
  const m_uint scope = !global ? env->scope->depth : env_push_global(env);
  const m_bool ret = scan2_decl(env, decl);
  if(global)
    env_pop(env, scope);
  return ret;
}

ANN static m_bool scan2_args(const Func_Def f) {
  Arg_List list = f->base->args;
  const bool global = GET_FLAG(f->base, global);
  do {
    const Value v = list->var_decl->value;
    v->from->offset = f->stack_depth;
    f->stack_depth += v->type->size;
    if(global)
      SET_FLAG(v, global);
  } while((list = list->next));
  return GW_OK;
}

ANN static Value scan2_func_assign(const Env env, const Func_Def d,
    const Func f, const Value v) {
  valuefrom(env, v->from, d->base->pos);
  SET_FLAG(v, const);
  set_vflag(v, vflag_func);
  if(!env->class_def) {
    if(!GET_FLAG(d->base, global))
      set_vflag(v, vflag_fglobal);
  } else {
    if(GET_FLAG(d->base, static))
      SET_FLAG(v, static);
    else
      set_vflag(v, vflag_member);
    SET_ACCESS(d->base, v)
 }
  d->base->func = v->d.func_ref = f;
  return f->value_ref = v;
}


ANN m_bool scan2_fptr_def(const Env env NUSED, const Fptr_Def fptr) {
  if(!tmpl_base(fptr->base->tmpl)) {
    const Func_Def def = fptr->type->info->func->def;
    if(def->base->args) {
      RET_NSPC(scan2_args(def))
    }
  } else
    set_tflag(fptr->type, tflag_ftmpl);
  return GW_OK;
}

ANN static m_bool scan2_func_def_op(const Env env, const Func_Def f);
ANN m_bool scan2_type_def(const Env env, const Type_Def tdef) {
  if(tdef->when)
    CHECK_BB(scan2_exp(env, tdef->when));
  if(!tdef->type->info->cdef)
    return GW_OK;
  return (!is_fptr(env->gwion, tdef->type) && tdef->type->info->cdef) ?
    scan2_class_def(env, tdef->type->info->cdef) : GW_OK;
}

ANN static m_bool scan2_range(const Env env, Range *range) {
  if(range->start)
    CHECK_BB(scan2_exp(env, range->start));
  if(range->end)
    CHECK_BB(scan2_exp(env, range->end));
  return GW_OK;
}

ANN static inline m_bool scan2_prim(const Env env, const Exp_Primary* prim) {
  if(prim->prim_type == ae_prim_hack || prim->prim_type == ae_prim_interp)
    CHECK_BB(scan2_exp(env, prim->d.exp));
/*  else if(prim->prim_type == ae_prim_id) {
    const Value v = prim_value(env, prim->d.var);
    if(v)
      v->vflag |= used;
  } */else if(prim->prim_type == ae_prim_array && prim->d.array->exp)
    return scan2_exp(env, prim->d.array->exp);
  else if(prim->prim_type == ae_prim_range)
    return scan2_range(env, prim->d.range);
  return GW_OK;
}

ANN static inline m_bool scan2_exp_array(const Env env, const Exp_Array* array) {
  CHECK_BB(scan2_exp(env, array->base));
  return scan2_exp(env, array->array->exp);
}

ANN static inline m_bool scan2_exp_slice(const Env env, const Exp_Slice* exp) {
  CHECK_BB(scan2_exp(env, exp->base));
  return scan2_range(env, exp->range);
}

ANN static m_bool multi_decl(const Env env, const Exp e, const Symbol op) {
  if(e->exp_type == ae_exp_decl) {
    if(e->d.exp_decl.list->next)
      ERR_B(e->pos, _("cant '%s' from/to a multi-variable declaration."), s_name(op))
//    set_vflag(e->d.exp_decl.list->self->value, vflag_used);
  }
  return GW_OK;
}

ANN static inline m_bool scan2_exp_binary(const Env env, const Exp_Binary* bin) {
  CHECK_BB(scan2_exp(env, bin->lhs));
  CHECK_BB(scan2_exp(env, bin->rhs));
  CHECK_BB(multi_decl(env, bin->lhs, bin->op));
  return multi_decl(env, bin->rhs, bin->op);
}

ANN static inline m_bool scan2_exp_cast(const Env env, const Exp_Cast* cast) {
  return scan2_exp(env, cast->exp);
}

ANN static inline m_bool scan2_exp_post(const Env env, const Exp_Postfix* post) {
  return scan2_exp(env, post->exp);
}

ANN static inline m_bool scan2_exp_call(const Env env, const Exp_Call* exp_call) {
  if(exp_call->tmpl)
    return GW_OK;
  CHECK_BB(scan2_exp(env, exp_call->func));
  const Exp args = exp_call->args;
  return args ? scan2_exp(env, args) : GW_OK;
}

ANN static inline m_bool scan2_exp_dot(const Env env, const Exp_Dot* member) {
  return scan2_exp(env, member->base);
}

ANN static inline m_bool scan2_exp_if(const Env env, const Exp_If* exp_if) {
  CHECK_BB(scan2_exp(env, exp_if->cond));
  CHECK_BB(scan2_exp(env, exp_if->if_exp ?: exp_if->cond));
  return scan2_exp(env, exp_if->else_exp);
}

ANN static m_bool scan2_exp_unary(const Env env, const Exp_Unary * unary) {
  if(unary->unary_type == unary_code) {
    RET_NSPC(scan2_stmt(env, unary->code))
  } else if(unary->unary_type == unary_exp)
    return scan2_exp(env, unary->exp);
  return GW_OK;
}

ANN static inline m_bool _scan2_stmt_match_case(const restrict Env env, const Stmt_Match stmt) {
  CHECK_BB(scan2_exp(env, stmt->cond));
  if(stmt->when)
    CHECK_BB(scan2_exp(env, stmt->when));
  return scan2_stmt_list(env, stmt->list);
}

ANN static inline m_bool scan2_stmt_match_case(const restrict Env env, const Stmt_Match stmt) {
  RET_NSPC(_scan2_stmt_match_case(env, stmt))
}

ANN static inline m_bool _scan2_stmt_match(const restrict Env env, const Stmt_Match stmt) {
  if(stmt->where)
    CHECK_BB(scan2_stmt(env, stmt->where));
  Stmt_List list = stmt->list;
  do CHECK_BB(scan2_stmt_match_case(env, &list->stmt->d.stmt_match));
  while((list = list->next));
  return GW_OK;
}

ANN static inline m_bool scan2_handler_list(const restrict Env env, const Handler_List handler) {
  if(handler->next)
    CHECK_BB(scan2_handler_list(env, handler->next));
  RET_NSPC(scan2_stmt(env, handler->stmt))
}

ANN static inline m_bool scan2_stmt_try(const restrict Env env, const Stmt_Try stmt) {
  CHECK_BB(scan2_handler_list(env, stmt->handler));
  RET_NSPC(scan2_stmt(env, stmt->stmt))
}

ANN static inline m_bool scan2_stmt_match(const restrict Env env, const Stmt_Match stmt) {
  CHECK_BB(scan2_exp(env, stmt->cond));
  RET_NSPC(_scan2_stmt_match(env, stmt))
}

#define scan2_exp_lambda dummy_func
#define scan2_exp_td dummy_func
HANDLE_EXP_FUNC(scan2, m_bool, Env)

#define scan2_stmt_func(name, type, prolog, exp) describe_stmt_func(scan2, name, type, prolog, exp)
scan2_stmt_func(flow, Stmt_Flow,, !(scan2_exp(env, stmt->cond) < 0 ||
    scan2_stmt(env, stmt->body) < 0) ? 1 : -1)
scan2_stmt_func(varloop, Stmt_VarLoop,, !(scan2_exp(env, stmt->exp) < 0 ||
    scan2_stmt(env, stmt->body) < 0) ? 1 : -1)
scan2_stmt_func(for, Stmt_For,, !(scan2_stmt(env, stmt->c1) < 0 ||
    scan2_stmt(env, stmt->c2) < 0 ||
    (stmt->c3 && scan2_exp(env, stmt->c3) < 0) ||
    scan2_stmt(env, stmt->body) < 0) ? 1 : -1)
scan2_stmt_func(each, Stmt_Each,, !(scan2_exp(env, stmt->exp) < 0 ||
    scan2_stmt(env, stmt->body) < 0) ? 1 : -1)
scan2_stmt_func(loop, Stmt_Loop,, !(scan2_exp(env, stmt->cond) < 0 ||
    scan2_stmt(env, stmt->body) < 0) ? 1 : -1)
scan2_stmt_func(if, Stmt_If,, !(scan2_exp(env, stmt->cond) < 0 ||
    scan2_stmt(env, stmt->if_body) < 0 ||
    (stmt->else_body && scan2_stmt(env, stmt->else_body) < 0)) ? 1 : -1)

ANN static inline m_bool scan2_stmt_code(const Env env, const Stmt_Code stmt) {
  if(stmt->stmt_list)
    { RET_NSPC(scan2_stmt_list(env, stmt->stmt_list)) }
  return GW_OK;
}

ANN static inline m_bool scan2_stmt_exp(const Env env, const Stmt_Exp stmt) {
  return stmt->val ? scan2_exp(env, stmt->val) : 1;
}

ANN m_bool scan2_union_def(const Env env NUSED, const Union_Def udef) {
  if(tmpl_base(udef->tmpl))
    return GW_OK;
  set_tflag(udef->type, tflag_scan2);
  return GW_OK;
}


#define scan2_stmt_while    scan2_stmt_flow
#define scan2_stmt_until    scan2_stmt_flow
#define scan2_stmt_continue dummy_func
#define scan2_stmt_break    dummy_func
#define scan2_stmt_return   scan2_stmt_exp
#define scan2_stmt_retry    dummy_func

ANN static m_bool scan2_stmt_pp(const Env env, const Stmt_PP stmt) {
  if(stmt->pp_type == ae_pp_include)
    env->name = stmt->data;
  return GW_OK;
}

ANN static m_bool scan2_stmt_defer(const Env env, const Stmt_Defer stmt) {
  return scan2_stmt(env, stmt->stmt);
}

DECL_STMT_FUNC(scan2, m_bool, Env)

ANN static m_bool scan2_stmt(const Env env, const Stmt stmt) {
  return scan2_stmt_func[stmt->stmt_type](env, &stmt->d);
}

ANN static m_bool scan2_stmt_list(const Env env, Stmt_List list) {
  do CHECK_BB(scan2_stmt(env, list->stmt));
  while((list = list->next));
  return GW_OK;
}

ANN static m_bool scan2_func_def_overload(const Env env, const Func_Def f, const Value overload) {
  const m_bool fptr = is_fptr(env->gwion, overload->type);
  if(isa(overload->type, env->gwion->type[et_function]) < 0 || is_fptr(env->gwion, overload->type)) {
    if(!fbflag(f->base, fbflag_internal))
      ERR_B(f->base->pos, _("function name '%s' is already used by another value"), overload->name)
  }
  const Func obase = !fptr ? overload->d.func_ref : _class_base(overload->type)->info->func;
  if(GET_FLAG(obase->def->base, final))
    ERR_B(f->base->pos, _("can't overload final function %s"), overload->name)
  const m_bool base = tmpl_base(f->base->tmpl);
  const m_bool tmpl = fflag(obase, fflag_tmpl);
  if((!tmpl && base) || (tmpl && !base && !f->base->tmpl))
    ERR_B(f->base->pos, _("must overload template function with template"))
  return GW_OK;
}

ANN static Func scan_new_func(const Env env, const Func_Def f, const m_str name) {
  const Func func = new_func(env->gwion->mp, name, f);
  if(env->class_def && tflag(env->class_def, tflag_tmpl))
    set_fflag(func, fflag_ftmpl);
  if(fbflag(f->base, fbflag_lambda))
    env->curr->info->value = new_scope(env->gwion->mp);
  return func;
}

ANN static Type func_type(const Env env, const Func func) {
  const Type base = env->gwion->type[!fbflag(func->def->base, fbflag_lambda) ? et_function : et_lambda];
  const Type t = type_copy(env->gwion->mp, base);
  t->info->parent = base;
  t->name = func->name;
  t->info->func = func;
  return t;
}

ANN2(1,2) static Value func_value(const Env env, const Func f,
    const Value overload) {
  const Type  t = func_type(env, f);
  const Value v = t->info->value = new_value(env->gwion->mp, t, t->name);
  valuefrom(env, v->from, f->def->base->pos);
  CHECK_OO(scan2_func_assign(env, f->def, f, v));
  if(!overload) {
    value_addref(v);
    nspc_add_value_front(env->curr, f->def->base->xid, v);
  } else if(overload->d.func_ref) {
    f->next = overload->d.func_ref->next;
    overload->d.func_ref->next = f;
  }
  if(env->class_def && !GET_FLAG(f->def->base, static)) {
    t->size += SZ_INT;
    set_vflag(v, vflag_member);
  }
  return v;
}

ANN2(1, 2) static m_bool scan2_fdef_tmpl(const Env env, const Func_Def f, const Value overload) {
  const m_str name = s_name(f->base->xid);
  const Func func = scan_new_func(env, f, name);
  const Value value = func_value(env, func, overload);
  set_fflag(func, fflag_tmpl);
  set_vflag(value, vflag_valid);
  set_tflag(value->type, tflag_ftmpl); // the only types with func flag, name could be better
  Type type = env->class_def;
  Nspc nspc = env->curr;
  uint i = 0;
  do {
    const Value v = nspc_lookup_value0(nspc, f->base->xid);
    if(v) {
      Func ff = v->d.func_ref;
      if(!ff)continue;
      do {
        if(ff->def == f) {
          ++i;
          continue;
        }
        if(compat_func(ff->def, f) > 0) {
          if(ff->value_ref->from->owner == env->curr)
            ERR_B(f->base->pos, "template function '%s' already defined with those arguments in this namespace", name)
          const Symbol sym = func_symbol(env, env->curr->name, name,
            "template", ff->vt_index);
          nspc_add_value(env->curr, sym, value);
          if(!overload) {
            value_addref(value);
            nspc_add_value(env->curr, f->base->xid, value);
          }
          func->vt_index = ff->vt_index;
          return GW_OK;
        }
      } while((ff = ff->next) && ++i);
   }
  } while(type && (type = type->info->parent) && (nspc = type->nspc));
  --i;
  const Symbol sym = func_symbol(env, env->curr->name, name, "template", i);
  nspc_add_value(env->curr, sym, value);
  if(!overload) {
    value_addref(value);
    nspc_add_value(env->curr, f->base->xid, value);
    nspc_add_func(env->curr, f->base->xid, func);
  } else
    func->vt_index = ++overload->from->offset;
  return GW_OK;
}

ANN static m_bool scan2_func_def_op(const Env env, const Func_Def f) {
  const m_str str = s_name(f->base->xid);
  struct Op_Func opfunc = { .ck=strcmp(str, "@implicit") ? 0 : opck_usr_implicit };
  struct Op_Import opi = { .ret=f->base->ret_type, .pos=f->base->pos,
      .data=(uintptr_t)f->base->func, .func=&opfunc };
  func_operator(f, &opi);
  CHECK_BB(add_op(env->gwion, &opi));
  operator_set_func(&opi);
  return GW_OK;
}

ANN static m_bool scan2_func_def_code(const Env env, const Func_Def f) {
  const Func former = env->func;
  env->func = f->base->func;
  CHECK_BB(scan2_stmt_code(env, &f->d.code->d.stmt_code));
  env->func = former;
  return GW_OK;
}

ANN static void scan2_func_def_flag(const Env env, const Func_Def f) {
  set_fflag(f->base->func, fflag_pure);
  if(f->base->xid == insert_symbol("@dtor"))
    set_tflag(env->class_def, tflag_dtor);
}

ANN static m_str func_tmpl_name(const Env env, const Func_Def f) {
  const m_str name = s_name(f->base->xid);
  struct Vector_ v;
  Specialized_List id = f->base->tmpl->list;
  m_uint tlen = 0;
  vector_init(&v);
  do {
    const Type t = nspc_lookup_type0(env->curr, id->xid);
    if(!t)return NULL;
    vector_add(&v, (vtype)t);
    tlen += strlen(t->name);
  } while((id = id->next) && ++tlen);
  char tmpl_name[tlen + 2];
  m_str str = tmpl_name;
  for(m_uint i = 0; i < vector_size(&v); ++i) {
    const m_str s = ((Type)vector_at(&v, i))->name;
    strcpy(str, s);
    str += strlen(s);
    if(i + 1 < vector_size(&v))
      *str++ = ',';
  }
  tmpl_name[tlen+1] = '\0';
  vector_release(&v);
  const Symbol sym = func_symbol(env, env->curr->name, name, tmpl_name, (m_uint)f->base->tmpl->base);
  return s_name(sym);
}


ANN2(1,2,4) static Value func_create(const Env env, const Func_Def f,
     const Value overload, const m_str name) {
  const Func func = scan_new_func(env, f, name);
  nspc_add_func(env->curr, insert_symbol(func->name), func);
  const Value v = func_value(env, func, overload);
  scan2_func_def_flag(env, f);
  nspc_add_value(env->curr, insert_symbol(func->name), v);
  return v;
}

ANN2(1,2) static m_str func_name(const Env env, const Func_Def f, const Value v) {
  if(!f->base->tmpl) {
    const Symbol sym  = func_symbol(env, env->curr->name,
        s_name(f->base->xid), NULL, v ? ++v->from->offset : 0);
    return s_name(sym);
  }
  return f->base->func ? f->base->func->name : func_tmpl_name(env, f);
}

ANN2(1,2) m_bool scan2_fdef_std(const Env env, const Func_Def f, const Value overload) {
  const m_str name = func_name(env, f, overload ?: NULL);
  if(!name)return GW_ERROR;
  const Func base = f->base->func;
  if(!base)
    CHECK_OB(func_create(env, f, overload, name));
  else
    f->base->func = base;
  if(f->base->args)
    CHECK_BB(scan2_args(f));
  if(f->d.code)
    CHECK_BB(scan2_func_def_code(env, f));
  if(!base) {
    if(fbflag(f->base, fbflag_op))
      CHECK_BB(scan2_func_def_op(env, f));
    set_vflag(f->base->func->value_ref, vflag_valid);
  }
  return GW_OK;
}

//! use function from parent class as next.
ANN static void upfunction(const Env env, const Func_Base *fb) {
  const Value v = find_value(env->class_def->info->parent, fb->xid);
  if(!v)
    return;
  Func func = fb->func;
  while(func->next && func->next->value_ref->from->owner == env->curr)
    func = func->next;
  func->next = v->d.func_ref;
}

ANN m_bool scan2_fdef(const Env env, const Func_Def fdef) {
  const Value overload = nspc_lookup_value2(env->curr, fdef->base->xid);
  if(overload)
    CHECK_BB(scan2_func_def_overload(env, fdef, overload));
  CHECK_BB((!tmpl_base(fdef->base->tmpl) ? scan2_fdef_std : scan2_fdef_tmpl)(env, fdef, overload));
  if(env->class_def)
    upfunction(env, fdef->base);
  return GW_OK;
}

__attribute__((returns_nonnull))
static ANN Func_Def scan2_cpy_fdef(const Env env, const Func_Def fdef) {
  const Func_Def f = cpy_func_def(env->gwion->mp, fdef);
  f->base->ret_type = fdef->base->ret_type;
  Arg_List a = f->base->args, b = fdef->base->args;
  while(a) {
    a->var_decl->value = b->var_decl->value;
    a->type = b->type;
    a = a->next;
    b = b->next;
  }
  scan1_func_def(env, f);
  return f;
}

static inline int is_cpy(const Func_Def fdef) {
  return  GET_FLAG(fdef->base, global) ||
    (fdef->base->tmpl && !fdef->base->tmpl->call);
}

ANN m_bool scan2_func_def(const Env env, const Func_Def fdef) {
  if(GET_FLAG(fdef->base, global) && !env->class_def)
    env->context->global = 1;
  const Func_Def f = !is_cpy(fdef) ?
    fdef : scan2_cpy_fdef(env, fdef);
  const m_uint scope = !GET_FLAG(f->base, global) ? env->scope->depth : env_push_global(env);
  f->stack_depth = (env->class_def && !GET_FLAG(f->base, static) && !GET_FLAG(f->base, global)) ? SZ_INT : 0;
  if(fbflag(f->base, fbflag_variadic))
    f->stack_depth += SZ_INT;
  const m_bool ret = scanx_fdef(env, env, f, (_exp_func)scan2_fdef);
  if(GET_FLAG(f->base, global))
    env_pop(env, scope);
  CHECK_BB(ret);
  fdef->base->func = f->base->func; // only needed if 'is_cpy()'
  return GW_OK;
}


ANN static m_bool scan2_extend_def(const Env env, const Extend_Def xdef) {
  CHECK_BB(ensure_scan2(env, xdef->t));
  CHECK_BB(extend_push(env, xdef->t));
  const m_bool ret = scan2_ast(env, xdef->body);
  extend_pop(env, xdef->t);
  return ret;
}

#define scan2_enum_def dummy_func
#define scan2_trait_def dummy_func

HANDLE_SECTION_FUNC(scan2, m_bool, Env)

ANN static m_bool scan2_parent(const Env env, const Class_Def cdef) {
  const Type parent = cdef->base.type->info->parent;
  CHECK_BB(ensure_scan2(env, parent));
  if(cdef->base.ext->array && cdef->base.ext->array->exp)
    CHECK_BB(scan2_exp(env, cdef->base.ext->array->exp));
  return GW_OK;
}

ANN static m_bool cdef_parent(const Env env, const Class_Def cdef) {
  const bool tmpl = !!cdef->base.tmpl;
  if(tmpl)
    CHECK_BB(template_push_types(env, cdef->base.tmpl));
  const m_bool ret = scan2_parent(env, cdef);
  if(tmpl)
    nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}

ANN m_bool scan2_class_def(const Env env, const Class_Def cdef) {
  if(tmpl_base(cdef->base.tmpl))
    return GW_OK;
  const Type t = cdef->base.type;
  if(tflag(t, tflag_scan2))
    return GW_OK;
  if(t->info->value->from->owner_class)
    CHECK_BB(ensure_scan2(env, t->info->value->from->owner_class));
  set_tflag(t, tflag_scan2);
  if(cdef->base.ext)
    CHECK_BB(cdef_parent(env, cdef));
  if(cdef->body)
    CHECK_BB(env_body(env, cdef, scan2_section));
  return GW_OK;
}

ANN m_bool scan2_ast(const Env env, Ast ast) {
  do CHECK_BB(scan2_section(env, ast->section));
  while((ast = ast->next));
  return GW_OK;
}
