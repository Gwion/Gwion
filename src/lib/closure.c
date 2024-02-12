#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "emit.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "traverse.h"
#include "template.h"
#include "parse.h"
#include "partial.h"
#include "gack.h"
#include "tmp_resolve.h"

ANN static Exp* uncurry(const Env env, const Exp_Binary *bin) {
  const Stmt* stmt = mp_vector_at(bin->rhs->type->info->func->def->d.code, Stmt, 0);
  Exp* ecall = stmt->d.stmt_exp.val;
  const Exp_Call *call = &ecall->d.exp_call;
  Exp* args = call->args;
  Exp* lhs = bin->lhs;
  Exp* base = NULL;
  Exp *tmp = NULL;
  while(args) {
    if(args->exp_type != ae_exp_primary || args->d.prim.prim_type != ae_prim_id || *s_name(args->d.prim.d.var) != '@') {
      Exp* next = args->next;
      args->next = NULL;
      if(tmp) tmp = (tmp->next = cpy_exp(env->gwion->mp, args));
      else base = (tmp = cpy_exp(env->gwion->mp, args));
      args->next = next;
    } else {
      if(!lhs) {
        free_exp(env->gwion->mp, base);
        return NULL;
      }
      Exp* next = lhs->next;
      lhs->next = NULL;
      if(tmp) tmp = (tmp->next = cpy_exp(env->gwion->mp, lhs));
      else base = (tmp = cpy_exp(env->gwion->mp, lhs));
      lhs = lhs->next = next;
    }
    args = args->next;
  }
  if(traverse_exp(env, base)) {
    free_exp(env->gwion->mp, bin->lhs);
    return base;
  }
  free_exp(env->gwion->mp, base);
  return NULL;
}

ANN static Type mk_call(const Env env, Exp* e, Exp* func, Exp* args) {
  Exp_Call call = {.func = func, .args = args };
  e->exp_type   = ae_exp_call;
  e->d.exp_call = call;
  return check_exp_call1(env, &e->d.exp_call) ?: env->gwion->type[et_error];
}

static OP_CHECK(opck_func_call) {
  Exp_Binary *bin  = (Exp_Binary *)data;
  if(!strncmp(bin->rhs->type->name, "partial:", 8)) {
    const Stmt* stmt = mp_vector_at(bin->rhs->type->info->func->def->d.code, Stmt, 0);
    const Exp_Call *call = &stmt->d.stmt_exp.val->d.exp_call;
    DECL_ON(Exp*, args, = uncurry(env, bin));
    return mk_call(env, exp_self(bin), call->func, args);
  }
  return mk_call(env, exp_self(bin), bin->rhs, bin->lhs);
}

static OP_CHECK(opck_fptr_call) {
  Exp_Binary *bin  = (Exp_Binary *)data;
  const Type t = mk_call(env, exp_self(bin), bin->rhs, bin->lhs);
  if (t == env->gwion->type[et_error])
    gw_err("{-}did you mean to use {0}{+}:=>{0}{-}?{0}\n");
  return t;
}

ANN Type upvalue_type(const Env env, Capture *cap) {
  const Value v = nspc_lookup_value1(env->curr, cap->var.tag.sym);
  if(!v) ERR_O(cap->var.tag.loc, _("non existing value")); // did_you_mean
  if(cap->is_ref && not_upvalue(env, v))
    ERR_O(cap->var.tag.loc, _("can't take ref of a scoped value"));
  cap->var.value = v;
  const Type base_type = !tflag(v->type, tflag_ref) ? v->type : (Type)vector_front(&v->type->info->tuple->contains);
  return !cap->is_ref ? base_type :  ref_type(env->gwion, base_type, cap->var.tag.loc);
}

ANN void free_captures(const VM_Shred shred, m_bit *const caps) {
  uint32_t sz = SZ_INT;
  const Capture_List captures = (*(Func_Def*)caps)->captures;
  for(m_uint i = 0; i < captures->len; i++) {
    Capture *const cap = mp_vector_at(captures, Capture, i);
    if(tflag(cap->temp->type, tflag_compound))
      compound_release(shred, cap->temp->type, caps + sz);
    sz += cap->temp->type->size;
  }
  mp_free2(shred->info->mp, sz, caps);
}

static INSTR(fptr_capture) {
  POP_REG(shred, instr->m_val);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  m_bit *caps = mp_malloc2(shred->info->mp, SZ_INT + instr->m_val);
  memcpy(caps + SZ_INT, REG(0), instr->m_val);
  *(Func_Def*)caps = (Func_Def)instr->m_val2;
  *(m_bit**)(o->data + SZ_INT) = caps;
}

static INSTR(fptr_assign) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(0);
  *(VM_Code*)(o->data + instr->m_val) = *(VM_Code*)REG(-SZ_INT);
  m_bit *old_caps = *(m_bit**)(o->data + SZ_INT);
  if(old_caps) free_captures(shred, old_caps);
  *(VM_Code*)(o->data + instr->m_val + SZ_INT) = NULL;
  *(M_Object*)REG(-SZ_INT) = o;
}

ANN static m_bool emit_fptr_assign(const Emitter emit, const Type lhs, const Type rhs) {
  const Instr instr = emit_add_instr(emit, fptr_assign);
  if(rhs->info->cdef && get_tmpl(rhs))
    instr->m_val = SZ_INT * 2;
  if(!lhs->info->func) {
    const Func_Def fdef = lhs->info->func->def;
    const Capture_List captures = fdef->captures;
    if(captures) {
      uint32_t offset = 0;
      Exp* e = new_prim_id(emit->gwion->mp, fdef->base->tag.sym, fdef->base->tag.loc); // free me
      for(uint32_t i = 0; i < captures->len; i++) {
        Capture *const cap = mp_vector_at(captures, Capture, i);
        e->d.prim.d.var = cap->var.tag.sym;
        e->d.prim.value = cap->var.value;
        e->type = cap->var.value->type;
        exp_setvar(e, cap->is_ref);
        CHECK_BB(emit_exp(emit, e));
        if(!cap->is_ref && tflag(cap->temp->type, tflag_compound))
          emit_compound_addref(emit, cap->temp->type, cap->temp->type->size, 0);
        offset += cap->temp->type->size;
      }
      free_exp(emit->gwion->mp, e);
      const Instr instr = emit_add_instr(emit, fptr_capture);
      instr->m_val = offset;
      instr->m_val2 = (m_uint)fdef;
    }
  }
  return GW_OK;
}

static OP_EMIT(opem_fptr_assign) {
  Exp_Binary *bin = (Exp_Binary*)data;
  return emit_fptr_assign(emit, bin->lhs->type, bin->rhs->type);
}


struct FptrInfo {
  Func        lhs;
  const Func  rhs;
  Exp*   exp;
};

ANN static void _fptr_tmpl_push(const Env env, const Func f) {
  const Tmpl *tmpl = f->def->base->tmpl;
  if (!tmpl) return;
  TmplArg_List tl = tmpl->call;
  if (!tl) return;
  Specialized_List sl = tmpl->list;
  for(uint32_t i = 0; i < sl->len; i++) {
    Specialized *spec = mp_vector_at(sl, Specialized, i);
    // can it be called with consts?
    if(spec->td) continue;
    TmplArg arg = *mp_vector_at(tl, TmplArg, i);
    const Type t = known_type(env, arg.d.td);
    nspc_add_type(env->curr, spec->tag.sym, t);
  }
}

ANN static m_bool fptr_tmpl_push(const Env env, struct FptrInfo *info) {
  if (!info->rhs->def->base->tmpl) return GW_OK;
  nspc_push_type(env->gwion->mp, env->curr);
  _fptr_tmpl_push(env, info->rhs);
  return GW_OK;
}

static m_bool td_match(const Env env, Type_Decl *id[2]) {
  DECL_OB(const Type, t0, = known_type(env, id[0]));
  DECL_OB(const Type, t1, = known_type(env, id[1]));
  if (isa(t0, t1) > 0) return GW_OK;
  return t1 == env->gwion->type[et_auto] ? GW_OK : GW_ERROR;
}

ANN static m_bool fptr_args(const Env env, Func_Base *base[2]) {
  Arg_List args0 = base[0]->args, args1 = base[1]->args;
  const bool member = vflag(base[0]->func->value_ref, vflag_member);
  const uint32_t len0 = mp_vector_len(args0) + member;
  const uint32_t len1 = mp_vector_len(args1);
  if(len0 != len1) return GW_ERROR;
  if(member) {
    const Arg *arg = mp_vector_at(args1, Arg, 0);
    CHECK_BB(isa(base[0]->func->value_ref->from->owner_class, arg->type));
  }
  for(uint32_t i = member; i < len0; i++) {
    const Arg *arg0 = mp_vector_at(args0, Arg, (i - member));
    const Arg *arg1 = mp_vector_at(args1, Arg, i);
    if (arg0->type && arg1->type)
      CHECK_BB(isa(arg0->type, arg1->type));
    else if(!tmpl_base(base[0]->tmpl) && !tmpl_base(base[1]->tmpl)){
      Type_Decl *td[2] = {arg0->var.td, arg1->var.td};
      CHECK_BB(td_match(env, td));
    }
  }
  return GW_OK;
}

ANN static bool fptr_effects(const Env env, struct FptrInfo *info) {
  if (!info->lhs->def->base->effects.ptr) return true;
  if (!info->rhs->def->base->effects.ptr) {
    gwerr_secondary("too many effects", env->name, info->exp->loc);
    return false;
  }
  const Vector lhs = &info->lhs->def->base->effects;
  const Vector rhs = &info->rhs->def->base->effects;
  for (m_uint i = 0; i < vector_size(lhs); i++) {
    if (vector_find(rhs, vector_at(lhs, 0)) == -1) {
      gwerr_secondary("effect not handled", env->name, info->exp->loc);
      return false;
    }
  }
  return true;
}

ANN static m_bool fptr_check(const Env env, struct FptrInfo *info) {
  //  if(!info->lhs->def->base->tmpl != !info->rhs->def->base->tmpl)
  //    return GW_ERROR;
  if(!info->lhs)
    ERR_B(info->exp->loc,
          _("can't resolve operator"))
  return GW_OK;
}

ANN static inline m_bool fptr_rettype(const Env env, struct FptrInfo *info) {
  Type_Decl *td[2] = {info->lhs->def->base->td, info->rhs->def->base->td};
  return td_match(env, td);
}

ANN static Type fptr_type(const Env env, struct FptrInfo *info) {
  const Value v     = info->lhs->value_ref;
  const Nspc  nspc  = v->from->owner;
  const m_str c     = s_name(info->lhs->def->base->tag.sym),
              stmpl = !info->rhs->def->base->tmpl ? NULL : "template";
  for (m_uint i = 0; i <= v->from->offset; ++i) {
    const Symbol sym = (!info->lhs->def->base->tmpl || i != 0)
                           ? func_symbol(env, nspc->name, c, stmpl, i)
                           : info->lhs->def->base->tag.sym;
    if (!is_class(env->gwion, info->lhs->value_ref->type)) {
      if (!(info->lhs = nspc_lookup_func1(nspc, sym))) {
        const Value v = nspc_lookup_value1(nspc, insert_symbol(c));
        if (!is_func(env->gwion, v->type)) return NULL; // is_callable
        info->lhs = v->type->info->func;
      }
    } else {
      DECL_OO(const Type, t,
              = nspc_lookup_type1(nspc, info->lhs->def->base->tag.sym));
      info->lhs = actual_type(env->gwion, t)->info->func;
    }
    Type       type    = NULL;
    Func_Base *base[2] = {info->lhs->def->base, info->rhs->def->base};
    CHECK_BO(fptr_tmpl_push(env, info));
    if (fptr_rettype(env, info) > 0 &&
        fptr_args(env, base) > 0 && fptr_effects(env, info))
      type = actual_type(env->gwion, info->lhs->value_ref->type)
                 ?: info->lhs->value_ref->type;
    if (info->rhs->def->base->tmpl) nspc_pop_type(env->gwion->mp, env->curr);
    if (type) return type;
  }
  return NULL;
}

ANN static m_bool _check_lambda(const Env env, Exp_Lambda *l,
                                const Func_Def fdef) {
  Arg_List bases = fdef->base->args;
  Arg_List args = l->def->base->args;
  if (mp_vector_len(bases) != mp_vector_len(args))
    ERR_B(exp_self(l)->loc, _("argument number does not match for lambda"))

  if(l->def->captures) {
    // here move to arguments
    for(uint32_t i = 0; i < l->def->captures->len; i++) {
      Capture *cap = mp_vector_at(l->def->captures, Capture, i);
      const Value v = nspc_lookup_value1(env->curr, cap->var.tag.sym);
      if(!v) ERR_B(cap->var.tag.loc, _("unknown value in capture"));
      DECL_OB(const Type, t, = upvalue_type(env, cap));
      cap->temp = new_value(env, t, cap->var.tag);
      cap->var.value = v;
    }
  }

  Type owner = fdef->base->func->value_ref->from->owner_class;
  struct EnvSet es    = {.env   = env,
                      .data  = env,
                      .func  = (_envset_func)traverse_cdef,
                      .scope = env->scope->depth,
                      .flag  = tflag_scan0};
  CHECK_b(envset_pushv(&es, owner->info->value));
  while(owner) {
    const Tmpl *tmpl = get_tmpl(owner);
    if(tmpl)
      template_push_types(env, tmpl);
    owner = owner->info->value->from->owner_class;
  }
  if(bases) {
    for(uint32_t i = 0; i < bases->len; i++) {
      Arg *base = mp_vector_at(bases, Arg, i);
      Arg *arg  = mp_vector_at(args, Arg, i);
      DECL_OB(const Type, arg_type, = known_type(env, base->var.td));
      arg->var.td = type2td(env->gwion, arg_type, exp_self(l)->loc);
    }
  }
  DECL_OB(const Type, ret_type, = known_type(env, fdef->base->td));
  l->def->base->td = type2td(env->gwion, ret_type, exp_self(l)->loc);
  /*Type*/ owner = fdef->base->func->value_ref->from->owner_class;

  Upvalues upvalues = {
    .values = env->curr->info->value
  };
  if(env->class_def) env->class_def->info->values = env->curr->info->value;
  l->def->base->values = &upvalues;
  const m_uint scope   = env->scope->depth;
  env->scope->depth = 0;
  const bool shadowing = env->scope->shadowing;
  env->scope->shadowing = true;
  if(env->class_def)SET_FLAG(l->def->base, static);
  const bool ret  = traverse_func_def(env, l->def);
  env->scope->shadowing = shadowing;
  env->scope->depth = scope;

  if (l->def->base->func) {
    if (env->curr->info->value != l->def->base->values->values) {
      free_scope(env->gwion->mp, env->curr->info->value);
      env->curr->info->value = l->def->base->values->values;
    }
  }
  /*Type*/ owner = fdef->base->func->value_ref->from->owner_class->info->value->from->owner_class;
  while(owner) {
    if(get_tmpl(owner))
    nspc_pop_type(env->gwion->mp, env->curr);
    owner = owner->info->value->from->owner_class;
  }
  envset_pop(&es, owner);
  if(!ret) {
    if(args) {
      for(uint32_t i = 0; i < bases->len; i++) {
        Arg *arg  = mp_vector_at(args, Arg, i);
        free_value(arg->var.vd.value, env->gwion);
        arg->var.vd.value = NULL;
      }
    }
  }
  return ret;
}

ANN m_bool check_lambda(const Env env, const Type t, Exp_Lambda *l) {
  const Func_Def fdef = t->info->func->def;
  CHECK_BB(_check_lambda(env, l, fdef));
  exp_self(l)->type = l->def->base->func->value_ref->type;
  return GW_OK;
}

ANN static m_bool fptr_do(const Env env, struct FptrInfo *info) {
  if(info->exp->type->info->func) {
    CHECK_BB(fptr_check(env, info));
    if (!(info->exp->type = fptr_type(env, info)))
      ERR_B(info->exp->loc, _("no match found"))
    return GW_OK;
  }
  Exp_Lambda *l = &info->exp->d.exp_lambda;
  return check_lambda(env, actual_type(env->gwion, info->rhs->value_ref->type), l);
}

ANN static Type partial2auto(const Env env, const Exp_Binary *bin) {
  const Func_Def fdef = bin->lhs->d.exp_lambda.def;
  unset_fbflag(fdef->base, fbflag_lambda);
  CHECK_ON(traverse_func_def(env, fdef));
  set_fbflag(fdef->base, fbflag_lambda);
  const Type actual = fdef->base->func->value_ref->type;
  set_fbflag(fdef->base, fbflag_lambda);
  Var_Decl vd = bin->rhs->d.exp_decl.var.vd;
exp_setvar(bin->rhs, true);
  return vd.value->type = bin->rhs->type = bin->rhs->d.exp_decl.type = actual;
}

static OP_CHECK(opck_auto_fptr) {
  const Exp_Binary *bin = (Exp_Binary *)data;
  // we'll only deal with auto fptr declaration
  if (bin->rhs->exp_type != ae_exp_decl &&
      bin->rhs->d.exp_decl.var.td->tag.sym != insert_symbol("auto"))
    ERR_N(bin->lhs->loc, "invalid {G+}function{0} {+}:=>{0} {+G}function{0} assignment");
  if (bin->lhs->exp_type == ae_exp_td)
    ERR_N(bin->lhs->loc, "can't use {/}type decl expressions{0} in auto function pointer declarations");
//  if(!bin->lhs->type->info->func)
  if(!bin->lhs->type->info->func || !strncmp(bin->lhs->type->name, "partial:", 8))
    return partial2auto(env, bin);

  // create a matching signature
  // TODO: we could check first if there a matching existing one
  // we can maybe add it to the lhs type function namespace
  // would make it easy enough to search
  Func_Base *const fbase =
      cpy_func_base(env->gwion->mp, bin->lhs->type->info->func->def->base);
  const Fptr_Def fptr_def = new_fptr_def(env->gwion->mp, fbase);
  char name[13 + strlen(env->curr->name) + num_digit(bin->rhs->loc.first.line) +
            num_digit(bin->rhs->loc.first.column)];
  sprintf(name, "generated@%s@%u:%u", env->curr->name, bin->rhs->loc.first.line,
          bin->rhs->loc.first.column);
  fptr_def->base->tag.sym = insert_symbol(name);
  const bool ret    = traverse_fptr_def(env, fptr_def);
  const Type   t      = fptr_def->cdef->base.type;
  free_fptr_def(env->gwion->mp, fptr_def);
  Var_Decl vd = bin->rhs->d.exp_decl.var.vd;
  vd.value->type = bin->rhs->type =
      bin->rhs->d.exp_decl.type                = t;
  return ret ? t : env->gwion->type[et_error];
}

static OP_CHECK(opck_fptr_assign) {
  Exp_Binary *bin = (Exp_Binary *)data;
  const Func_Def rhs = closure_def(bin->rhs->type);
  struct FptrInfo info = {.lhs = bin->lhs->type->info->func,
                          .rhs = rhs->base->func,
                          .exp = bin->lhs};
  CHECK_BN(fptr_do(env, &info));
  return bin->rhs->type;
}

static OP_CHECK(opck_fptr_impl) {
  struct Implicit *impl = (struct Implicit *)data;
  const Func f = closure_def(impl->t)->base->func;
  struct FptrInfo  info = {.lhs = impl->e->type->info->func,
                           .rhs = f,
                           .exp = impl->e};
  CHECK_BN(fptr_do(env, &info));
  return impl->t;
}

static OP_EMIT(opem_fptr_impl) {
  struct Implicit *impl = (struct Implicit *)data;
  if(!impl->e->type->info->func->def->base->tmpl && get_tmpl(impl->t)) {
    const Instr instr = (Instr)vector_back(&emit->code->instr);
    instr->opcode = eRegPushImm;
    instr->m_val = (m_uint)impl->e->type->info->func;
  }
  const Instr instr = emit_add_instr(emit, ObjectInstantiate);
  instr->m_val2 = (m_uint)impl->t;
  return emit_fptr_assign(emit, impl->e->type, impl->t);
}

static OP_CHECK(opck_fptr_cast) {
  Exp_Cast *cast = (Exp_Cast *)data;
  const Type t = known_type(env, cast->td);
  const Func f = closure_def(t)->base->func;
  struct FptrInfo  info = {.lhs = cast->exp->type->info->func,
                           .rhs = f,
                           .exp = cast->exp};
  CHECK_BN(fptr_do(env, &info));
  return t;
}

static void op_narg_err(const Env env, const Func_Def fdef, const loc_t loc) {
  if (!env->context->error) {
    gwerr_basic(_("invalid operator decay"),
                _("Decayed operators take two arguments"), NULL, env->name, loc,
                0);
    if (fdef) defined_here(fdef->base->func->value_ref);
    env_set_error(env, true);
  }
}
static m_bool op_call_narg(const Env env, Exp* arg, const loc_t loc) {
  m_uint narg = 0;
  while (arg) {
    narg++;
    arg = arg->next;
  }
  if (narg == 2) return GW_OK;
  op_narg_err(env, NULL, loc);
  return GW_ERROR;
}

ANN Type check_op_call(const Env env, Exp_Call *const exp) {
  CHECK_BO(op_call_narg(env, exp->args, exp->func->loc));
  Exp* base   = exp_self(exp);
  Exp* op_exp = exp->func;
  base->exp_type   = ae_exp_binary;
  Exp_Binary *bin  = &base->d.exp_binary;
  bin->lhs         = exp->args;
  bin->lhs         = exp->args->next;
  exp->args->next  = NULL;
  bin->op          = op_exp->d.prim.d.var;
  free_exp(env->gwion->mp, op_exp);
  return check_exp(env, base);
}

static m_bool op_impl_narg(const Env env, const Func_Def fdef,
                           const loc_t loc) {
  if (mp_vector_len(fdef->base->args) == 2) return GW_OK;
  op_narg_err(env, fdef, loc);
  return GW_ERROR;
}

static inline void op_impl_ensure_types(const Env env, const Func func) {
  const bool owner_tmpl =
      safe_tflag(func->value_ref->from->owner_class, tflag_tmpl);
  if (owner_tmpl)
    template_push_types(
        env, get_tmpl(func->value_ref->from->owner_class));
  const bool func_tmpl = fflag(func, fflag_tmpl);
  if (func_tmpl) template_push_types(env, func->def->base->tmpl);

  Arg_List args = func->def->base->args;
  for(uint32_t i = 0; i < args->len; i++) {
    Arg *arg = mp_vector_at(args, Arg, i);
    if (!arg->type) arg->type = known_type(env, arg->var.td);
  }
  if (!func->def->base->ret_type)
    func->def->base->ret_type = known_type(env, func->def->base->td);
  if (owner_tmpl) nspc_pop_type(env->gwion->mp, env->curr);
  if (func_tmpl) nspc_pop_type(env->gwion->mp, env->curr);
}

static OP_EMIT(opem_op_impl) {
  struct Implicit *impl = (struct Implicit *)data;
  if(!impl->e->type->info->func->code)
    emit_ensure_func(emit, impl->e->type->info->func);
  emit_pushimm(emit, (m_uint)impl->e->type->info->func->code);
  return emit_fptr_assign(emit, impl->e->type, impl->t);
}

static OP_CHECK(opck_op_impl) {
  struct Implicit *impl = (struct Implicit *)data;
  const Func       func = closure_def(impl->t)->base->func;
  CHECK_BN(op_impl_narg(env, func->def, impl->e->loc));
  op_impl_ensure_types(env, func);
  const Symbol lhs_sym = insert_symbol("@lhs");
  const Symbol rhs_sym = insert_symbol("@rhs");
  const Arg *arg0 = (Arg*)(func->def->base->args->ptr);
  const Arg *arg1 = (Arg*)(func->def->base->args->ptr + sizeof(Arg));
  Exp  _lhs    = {
      .d        = {.prim = {.d = {.var = lhs_sym}, .prim_type = ae_prim_id}},
      .exp_type = ae_exp_primary,
      .type     = arg0->type,
      .loc      = arg0->var.td->tag.loc};
  Exp _rhs = {
      .d        = {.prim = {.d = {.var = rhs_sym}, .prim_type = ae_prim_id}},
      .exp_type = ae_exp_primary,
      .type     = arg1->type,
      .loc      = arg1->var.td->tag.loc};
  Exp self = {.loc = impl->e->loc};
  self.d.exp_binary.lhs = &_lhs;
  self.d.exp_binary.rhs = &_rhs;
  self.d.exp_binary.op  = impl->e->d.prim.d.var;
  struct Op_Import opi  = {.op   = impl->e->d.prim.d.var,
                          .lhs  = arg0->type,
                          .rhs  = arg1->type,
                          .data = (uintptr_t)&self.d.exp_binary,
                          .loc  = impl->e->loc};
  vector_add(&env->scope->effects, 0);
  DECL_ON(const Type, t, = op_check(env, &opi));
  CHECK_BN(isa(t, func->def->base->ret_type)); // error message?
  MP_Vector *const eff = (MP_Vector*)vector_back(&env->scope->effects);
//  if (eff && !check_effect_overload(eff, func))
//    ERR_N(impl->loc, _("`{+Y}%s{0}` has effects not present in `{+G}%s{0}`\n"),
//          s_name(impl->e->d.prim.d.var), func->name);
  const Value v = nspc_lookup_value0(opi.nspc, impl->e->d.prim.d.var);
  if (v) {
    const m_uint scope = env_push(env, NULL, opi.nspc);
    _lhs.next          = &_rhs;
    Exp_Call   call    = {.args = &_lhs};
    const Func exists  = (Func)find_func_match(env, v->d.func_ref, &call);
    env_pop(env, scope);
    if (exists) { // improve me
      if (eff) {
        free_mp_vector(env->gwion->mp, struct ScopeEffect, eff);
        ERR_N(impl->loc,
              _("`{+Y}%s{0}` has effects not present in `{+G}%s{0}`\n"),
              s_name(impl->e->d.prim.d.var), func->name);
      }
      return func->value_ref->from->owner_class;
    }
  }
  const Arg_List args = cpy_arg_list(env->gwion->mp, func->def->base->args);
  Arg *larg0 = mp_vector_at(args, Arg, 0);
  Arg *larg1 = mp_vector_at(args, Arg, 1);
  larg0->var.vd.tag.sym = lhs_sym;
  larg1->var.vd.tag.sym = rhs_sym;
  Func_Base *base =
      new_func_base(env->gwion->mp, type2td(env->gwion, t, impl->e->loc),
                    impl->e->d.prim.d.var, args, ae_flag_none, impl->e->loc);
  if (eff) {
    for (m_uint i = 0; i < eff->len; i++) {
      struct ScopeEffect *effect = mp_vector_at(eff, struct ScopeEffect, i);
      vector_add(&base->effects, (m_uint)effect->sym);
    }
    free_mp_vector(env->gwion->mp, struct ScopeEffect, eff);
  }
  Exp* lhs =
      new_prim_id(env->gwion->mp, larg0->var.vd.tag.sym, impl->e->loc);
  Exp* rhs =
      new_prim_id(env->gwion->mp, larg1->var.vd.tag.sym, impl->e->loc);
  Exp*  bin = new_exp_binary(env->gwion->mp, lhs, impl->e->d.prim.d.var,
                                 rhs, impl->e->loc);
  Stmt_List code = new_mp_vector(env->gwion->mp, Stmt, 1);
  mp_vector_set(code, Stmt, 0, MK_STMT_RETURN(impl->e->loc, bin));
  const Func_Def  def  = new_func_def(env->gwion->mp, base, code);
  def->base->tag.sym   = impl->e->d.prim.d.var;
// use envset
// or better, some function with envset and traverse
  const m_uint scope   = env_push(env, NULL, opi.nspc);
  // we assume succes here
  /*const m_bool ret = */ traverse_func_def(env, def);
  env_pop(env, scope);
  def->base->func->value_ref->type->info->parent = env->gwion->type[et_op];
	  impl->e->type         = def->base->func->value_ref->type;
  impl->e->d.prim.value = def->base->func->value_ref;
  return opck_fptr_impl(env, impl);
}

static OP_CHECK(opck_op_cast) {
  Exp_Cast *cast = (Exp_Cast*)data;
  struct Implicit impl = { .e = cast->exp, .t = known_type(env, cast->td) };
  return opck_op_impl(env, &impl);
}

static OP_CHECK(opck_func_partial) {
  Exp_Call *call = (Exp_Call*)data;
  return partial_type(env, call) ?: env->gwion->type[et_error];
}

static OP_CHECK(opck_class_partial) {
  Exp_Call *call = (Exp_Call*)data;
  struct Op_Import opi = {.op   = insert_symbol("@partial"),
                          .lhs  = actual_type(env->gwion, call->func->type),
                          .loc  = call->func->loc,
                          .data = (uintptr_t)data};
   return op_check(env, &opi);
}

static FREEARG(freearg_gtmpl) {
  free_mstr(((Gwion)gwion)->mp, (m_str)instr->m_val2);
}
static FREEARG(freearg_dottmpl) {
  struct dottmpl_ *dt = (struct dottmpl_*) instr->m_val2;
  free_mstr(((Gwion)gwion)->mp, dt->tmpl_name);
}

#include "tmpl_info.h"
#include "parse.h"
#include "traverse.h"
#include "gwi.h"

ANN static bool is_base(const Env env, const TmplArg_List tl) {
  for(uint32_t i = 0; i < tl->len; i++) {
    // can call with const happen?
    TmplArg arg = *mp_vector_at(tl, TmplArg, i);
    if(unlikely(arg.type == tmplarg_exp)) continue;
    if(known_type(env, arg.d.td) == env->gwion->type[et_auto])
      return true;
  }
  return false;
}

static OP_CHECK(opck_closure_scan) {
  struct TemplateScan *ts   = (struct TemplateScan *)data;
  struct tmpl_info     info = {
      .base = ts->t, .td = ts->td, .list = get_tmpl(ts->t)->list};
  const Type exists = tmpl_exists(env, &info);
  if (exists) return exists != env->gwion->type[et_error] ? exists : NULL;
  const Func_Base *base = closure_def(ts->t)->base;
  const Arg_List args = base->args ? cpy_arg_list(env->gwion->mp, base->args) : NULL;
  Func_Base *const fbase = new_func_base(env->gwion->mp, cpy_type_decl(env->gwion->mp, base->td), info.name, args, ae_flag_none, base->tag.loc);
  fbase->tmpl = cpy_tmpl(env->gwion->mp, base->tmpl);
  if(!is_base(env, ts->td->types))
    fbase->tmpl->call = cpy_tmplarg_list(env->gwion->mp, ts->td->types);
  const Fptr_Def fdef = new_fptr_def(env->gwion->mp, cpy_func_base(env->gwion->mp, fbase));
  fdef->base->tag.sym = info.name;
  struct EnvSet es    = {.env   = env,
                      .data  = env,
                      .func  = (_envset_func)traverse_cdef,
                      .scope = env->scope->depth,
                      .flag  = tflag_scan0};
  const Type    owner = ts->t;
  CHECK_O(envset_pushv(&es, owner->info->value));
  const bool ret = traverse_fptr_def(env, fdef);
  const Type t = ret ? fdef->cdef->base.type : NULL;
  envset_pop(&es, owner->info->value->from->owner_class);
  free_fptr_def(env->gwion->mp, fdef); // clean?
  if(t) set_tflag(t, tflag_emit);
  return t;
}

static CTOR(fptr_ctor) {
  *(VM_Code*)o->data = ((Func)vector_at(&o->type_ref->nspc->vtable, 1))->code;
}

ANN bool tmpl_fptr(const Env env, const Fptr_Def fptr, const Func_Def fdef) {
  fptr->cdef->base.type->nspc->offset += SZ_INT * 3;
  env_push_type(env, fptr->cdef->base.type);
  CHECK_b(traverse_func_def(env, fdef));
  builtin_func(env->gwion, fdef->base->func, fptr_ctor);
  set_tflag(fdef->base->func->value_ref->type, tflag_ftmpl);
  env_pop(env, 0);
  return true;
}

static DTOR(fptr_dtor) {
  m_bit *const caps = *(m_bit**)(o->data + SZ_INT);
  if(caps) free_captures(shred, caps);
}

static MFUN(fptr_default) {
  xfun_handle(shred, "EmptyFunctionPointer");
}

static GACK(gack_function) { INTERP_PRINTF("%s", t->name); }

GWION_IMPORT(func) {
  gwidoc(gwi, "the base of all functions.");
  const Type t_function = gwi_mk_type(gwi, "function", SZ_INT, NULL);
  GWI_BB(gwi_gack(gwi, t_function, gack_function))
  GWI_BB(gwi_set_global_type(gwi, t_function, et_function))

  gwidoc(gwi, "the base of decayed operators.");
  const Type t_op = gwi_mk_type(gwi, "operator", SZ_INT, "function");
  GWI_BB(gwi_set_global_type(gwi, t_op, et_op))

  gwidoc(gwi, "the base of function pointers.");
  const Type t_closure = gwi_class_ini(gwi, "funptr", "Object");
  t_closure->nspc->offset = SZ_INT*3;
  gwi_class_xtor(gwi, fptr_ctor, fptr_dtor);
  GWI_BB(gwi_set_global_type(gwi, t_closure, et_closure))
  GWI_BB(gwi_func_ini(gwi, "void", "default"));
  GWI_BB(gwi_func_end(gwi, fptr_default, ae_flag_none));
  gwi_class_end(gwi);

  GWI_BB(gwi_oper_ini(gwi, "funptr", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_closure_scan))
  GWI_BB(gwi_oper_end(gwi, "class", NULL))

  GWI_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "function", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_func_call))
  GWI_BB(gwi_oper_end(gwi, "=>", NULL))
  GWI_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "funptr", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_fptr_call))
  GWI_BB(gwi_oper_end(gwi, "=>", NULL))
  GWI_BB(gwi_oper_ini(gwi, "function", "funptr", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_fptr_assign))
  GWI_BB(gwi_oper_emi(gwi, opem_fptr_assign))
  GWI_BB(gwi_oper_end(gwi, ":=>", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_fptr_impl))
  GWI_BB(gwi_oper_emi(gwi, opem_fptr_impl))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_fptr_cast))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
  GWI_BB(gwi_oper_ini(gwi, "operator", "funptr", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_op_impl))
  GWI_BB(gwi_oper_emi(gwi, opem_op_impl))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_op_cast))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
  GWI_BB(gwi_oper_ini(gwi, "function", "function", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_auto_fptr))
  GWI_BB(gwi_oper_end(gwi, ":=>", int_r_assign))
  GWI_BB(gwi_oper_ini(gwi, "function", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_func_partial))
  GWI_BB(gwi_oper_end(gwi, "@partial", NULL))
  GWI_BB(gwi_oper_ini(gwi, "Class", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_class_partial))
  GWI_BB(gwi_oper_end(gwi, "@partial", NULL))

  gwi_register_freearg(gwi, GTmpl, freearg_gtmpl);
  gwi_register_freearg(gwi, DotTmpl, freearg_dottmpl);
  return GW_OK;
}
