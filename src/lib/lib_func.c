#include <string.h>
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

ANN static Exp uncurry(const Env env, const Exp_Binary *bin) {
  const Stmt stmt = mp_vector_at(bin->rhs->type->info->func->def->d.code->d.stmt_code.stmt_list, struct Stmt_, 0);
  const Exp ecall = stmt->d.stmt_exp.val;
  const Exp_Call *call = &ecall->d.exp_call;
  Exp args = call->args;
  Exp lhs = bin->lhs;
  Exp base = NULL, tmp = NULL;
  while(args) {
    if(args->exp_type != ae_exp_primary || args->d.prim.prim_type != ae_prim_id || *s_name(args->d.prim.d.var) != '@') {
      // we should check better => use longer name
      const Exp next = args->next;
      args->next = NULL;
      if(tmp) tmp = (tmp->next = cpy_exp(env->gwion->mp, args));
      else base = (tmp = cpy_exp(env->gwion->mp, args));
        args->next = next;
      } else {
      if(!lhs) {
        free_exp(env->gwion->mp, base);
        return NULL;
      }
      const Exp next = lhs->next;
      lhs->next = NULL;
      if(tmp) tmp = (tmp->next = cpy_exp(env->gwion->mp, lhs));
      else base = (tmp = cpy_exp(env->gwion->mp, lhs));
      lhs = lhs->next = next;
    }
    args = args->next;
  }
  if(traverse_exp(env, base) > 0) {
    free_exp(env->gwion->mp, bin->lhs);
    return base;
  }
  free_exp(env->gwion->mp, base);
  return NULL;
}

ANN static Type mk_call(const Env env, const Exp e, const Exp func, const Exp args) {
  Exp_Call    call = {.func = func, .args = args };
  e->exp_type      = ae_exp_call;
  memcpy(&e->d.exp_call, &call, sizeof(Exp_Call));
  return check_exp_call1(env, &e->d.exp_call) ?: env->gwion->type[et_error];
}

static OP_CHECK(opck_func_call) {
  Exp_Binary *bin  = (Exp_Binary *)data;
  if(!strncmp(bin->rhs->type->name, "partial:", 8)) {
    const Stmt stmt = mp_vector_at(bin->rhs->type->info->func->def->d.code->d.stmt_code.stmt_list, struct Stmt_, 0);
    const Exp_Call *call = &stmt->d.stmt_exp.val->d.exp_call;
    const Exp args = uncurry(env, bin);
    if(args) return mk_call(env, exp_self(bin), call->func, args);
  }
  return mk_call(env, exp_self(bin), bin->rhs, bin->lhs);
}

ANN static inline Exp cpy_nonext(const Env env, const Exp e) {
  const MemPool mp   = env->gwion->mp;
  const Exp     next = e->next;
  e->next            = NULL;
  const Exp ret      = cpy_exp(mp, e);
  e->next            = next;
  if (!check_exp(env, ret)) {
    free_exp(mp, ret);
    return NULL;
  }
  return ret;
}

static inline void fptr_instr(const Emitter emit, const Func f,
                              const m_uint i) {
  const Instr set = emit_add_instr(emit, RegSetImm);
  set->m_val      = (m_uint)f;
  set->m_val2     = -SZ_INT * i;
}

static OP_EMIT(opem_func_assign) {
  Exp_Binary *bin = (Exp_Binary *)data;
  if (bin->rhs->type->info->func->def->base->tmpl)
    fptr_instr(emit, bin->lhs->type->info->func, 2);
  (void)emit_add_instr(emit, int_r_assign);
  if (vflag(bin->lhs->type->info->func->value_ref, vflag_member) &&
      !is_fptr(emit->gwion, bin->lhs->type)) {
    const Instr pop = emit_add_instr(emit, RegMove);
    pop->m_val      = -SZ_INT;
    const Instr cpy = emit_add_instr(emit, Reg2Reg);
    cpy->m_val      = -SZ_INT;
  }
  return GW_OK;
}

struct FptrInfo {
  Func        lhs;
  const Func  rhs;
  const Exp   exp;
  const loc_t pos;
};

ANN static void _fptr_tmpl_push(const Env env, const Func f) {
  const Tmpl *tmpl = f->def->base->tmpl;
  if (!tmpl) return;
  Type_List tl = tmpl->call;
  if (!tl) return;
  Specialized_List sl = tmpl->list;
  for(uint32_t i = 0; i < sl->len; i++) {
    Specialized *spec = mp_vector_at(sl, Specialized, i);
    Type_Decl *td = *mp_vector_at(tl, Type_Decl*, i);
    const Type t = known_type(env, td);
    nspc_add_type(env->curr, spec->xid, t);
  }
}

ANN static m_bool fptr_tmpl_push(const Env env, struct FptrInfo *info) {
  //  if(safe_tflag(info->rhs->value_ref->from->owner_class, tflag_tmpl))
  if (!info->rhs->def->base->tmpl) return GW_OK;
  nspc_push_type(env->gwion->mp, env->curr);
  //  Type owner = info->rhs->value_ref->from->owner_class;
  //  while(owner) {
  //    owner = owner->info->value->from->owner_class;
  //  }

  //  _fptr_tmpl_push(env, info->lhs);
  _fptr_tmpl_push(env, info->rhs);
  return GW_OK;
}

static m_bool td_match(const Env env, Type_Decl *id[2]) {
  DECL_OB(const Type, t0, = known_type(env, id[0]));
  DECL_OB(const Type, t1, = known_type(env, id[1]));
  if (isa(t0, t1) > 0) return GW_OK;
  return t1 == env->gwion->type[et_auto] ? GW_OK : GW_ERROR;
}

ANN static inline bool handle_global(Func_Base *a, Func_Base *b) {
  return (!b->func->value_ref->from->owner_class &&
      (!GET_FLAG(a, global) && a->func->value_ref->from->owner_class));
}

ANN static m_bool fptr_args(const Env env, Func_Base *base[2]) {
  Arg_List args0 = base[0]->args, args1 = base[1]->args;
  const bool offset0 = handle_global(base[0], base[1]);
  const bool offset1 = handle_global(base[1], base[0]);
  const uint32_t len0 = args0 ? args0->len : 0;
  const uint32_t len1 = args1 ? args1->len : 0;
  if(len0 - offset0 != len1 - offset1)
    return GW_ERROR;
  for(uint32_t i = 0; i < len1 - offset0; i++) {
    const Arg *arg0 = (Arg*)(args0->ptr + (i + offset0) * sizeof(Arg));
    const Arg *arg1 = (Arg*)(args1->ptr + (i + offset1) * sizeof(Arg));
    if (arg0->type && arg1->type)
      CHECK_BB(isa(arg0->type, arg1->type));
    else if(!tmpl_base(base[0]->tmpl) && !tmpl_base(base[1]->tmpl)){
      Type_Decl *td[2] = {arg0->td, arg1->td};
      CHECK_BB(td_match(env, td));
    }
  }
  return GW_OK;
}

ANN static bool fptr_effects(const Env env, struct FptrInfo *info) {
  if (!info->lhs->def->base->effects.ptr) return true;
  if (!info->rhs->def->base->effects.ptr) {
    gwerr_secondary("too many effects", env->name, info->pos);
    return false;
  }
  const Vector lhs = &info->lhs->def->base->effects;
  const Vector rhs = &info->rhs->def->base->effects;
  for (m_uint i = 0; i < vector_size(lhs); i++) {
    if (vector_find(rhs, vector_at(lhs, 0)) == -1) {
      gwerr_secondary("effect not handled", env->name, info->pos);
      return false;
    }
  }
  return true;
}

ANN static m_bool fptr_check(const Env env, struct FptrInfo *info) {
  //  if(!info->lhs->def->base->tmpl != !info->rhs->def->base->tmpl)
  //    return GW_ERROR;
  if(!info->lhs)
    ERR_B(info->pos,
          _("can't resolve operator"))
  const Type l_type = info->lhs->value_ref->from->owner_class;
  const Type r_type = info->rhs->value_ref->from->owner_class;
  const Arg *l_arg = info->lhs->def->base->args ? mp_vector_at(info->lhs->def->base->args, Arg, 0) : NULL;
  const Arg *r_arg = info->rhs->def->base->args ? mp_vector_at(info->rhs->def->base->args, Arg, 0) : NULL;
// added when parsing bases template
  const Type l_this = l_arg && l_arg->var_decl.value ? l_arg->var_decl.value->type : NULL;
  const Type r_this = r_arg && r_arg->var_decl.value? r_arg->var_decl.value->type : NULL;

  if (!r_type && l_type) {
    if (/*!GET_FLAG(info->lhs, global) && */(!info->rhs->def->base->args ||
isa(l_type, r_this) < 0)
)
    ERR_B(info->pos,
          _("can't assign member function to non member function pointer"))
  } else if (!l_type && r_type) {
    if (!GET_FLAG(info->rhs, global) && (!info->lhs->def->base->args ||
isa(r_type, l_this) < 0)
)
      ERR_B(info->pos,
            _("can't assign non member function to member function pointer"))
  } else if (l_type && isa(r_type, l_type) < 0)
    ERR_B(info->pos,
          _("can't assign member function to a pointer of an other class"))
  if (vflag(info->rhs->value_ref, vflag_member)) {
    if (!vflag(info->lhs->value_ref, vflag_member) && l_type)
      ERR_B(info->pos,
            _("can't assign static function to member function pointer"))
  } else if (vflag(info->lhs->value_ref, vflag_member) && r_type)
    ERR_B(info->pos,
          _("can't assign member function to static function pointer"))
  return GW_OK;
}

ANN static inline m_bool fptr_rettype(const Env env, struct FptrInfo *info) {
  Type_Decl *td[2] = {info->lhs->def->base->td, info->rhs->def->base->td};
  return td_match(env, td);
}

ANN static inline m_bool fptr_arity(struct FptrInfo *info) {
  return fbflag(info->lhs->def->base, fbflag_variadic) ==
         fbflag(info->rhs->def->base, fbflag_variadic);
}

ANN static Type fptr_type(const Env env, struct FptrInfo *info) {
  const Value v     = info->lhs->value_ref;
  const Nspc  nspc  = v->from->owner;
  const m_str c     = s_name(info->lhs->def->base->xid),
              stmpl = !info->rhs->def->base->tmpl ? NULL : "template";
  for (m_uint i = 0; i <= v->from->offset; ++i) {
    const Symbol sym = (!info->lhs->def->base->tmpl || i != 0)
                           ? func_symbol(env, nspc->name, c, stmpl, i)
                           : info->lhs->def->base->xid;
    if (!is_class(env->gwion, info->lhs->value_ref->type)) {
      if (!(info->lhs = nspc_lookup_func1(nspc, sym))) {
        const Value v = nspc_lookup_value1(nspc, insert_symbol(c));
        if (!is_func(env->gwion, v->type)) return NULL;
        info->lhs = v->type->info->func;
      }
    } else {
      DECL_OO(const Type, t,
              = nspc_lookup_type1(nspc, info->lhs->def->base->xid));
      info->lhs = actual_type(env->gwion, t)->info->func;
    }
    Type       type    = NULL;
    Func_Base *base[2] = {info->lhs->def->base, info->rhs->def->base};
    CHECK_BO(fptr_tmpl_push(env, info));
    if (fptr_rettype(env, info) > 0 && fptr_arity(info) &&
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
//   if(l->def->base->func) return GW_OK;
  Arg_List bases = fdef->base->args;
  Arg_List args = l->def->base->args;
  // arity match
  if ((bases ? bases->len : 0) != (args ? args->len : 0))
    ERR_B(exp_self(l)->pos, _("argument number does not match for lambda"))
    if(fdef->captures) {
    // here move to arguments
    uint32_t offset = 0;
    for(uint32_t i = 0; i < fdef->captures->len; i++) {
      Capture *cap = mp_vector_at(fdef->captures, Capture, i);
      const Value v = nspc_lookup_value1(env->curr, cap->xid);
      if(!v) ERR_B(cap->pos, _("unknown value in capture"));
      offset += (!cap->is_ref ? SZ_INT : v->type->size);
      cap->orig = v;
      cap->offset = offset;
    }
  }
  const bool is_tmpl =
      safe_tflag(fdef->base->func->value_ref->from->owner_class, tflag_tmpl);
  if (is_tmpl)
    template_push_types(
        env,
        fdef->base->func->value_ref->from->owner_class->info->cdef->base.tmpl);
  if(bases) {
    for(uint32_t i = 0; i < bases->len; i++) {
      Arg *base = mp_vector_at(bases, Arg, i);
      Arg *arg  = mp_vector_at(args, Arg, i);
      arg->td = type2td(env->gwion, known_type(env, base->td), exp_self(l)->pos);
    }
  }
  l->def->base->td =
      type2td(env->gwion, known_type(env, fdef->base->td), exp_self(l)->pos);
  if (is_tmpl) nspc_pop_type(env->gwion->mp, env->curr);
  l->def->base->flag = fdef->base->flag;
  //  if(GET_FLAG(def->base, global) && !l->owner &&
  //  def->base->func->value_ref->from->owner_class)
  UNSET_FLAG(l->def->base, global);
  Upvalues upvalues = {
    .values = env->curr->info->value
  };
  l->def->base->values = &upvalues;
  const m_uint scope   = env->scope->depth;
  if(GET_FLAG(fdef->base, global) && !l->owner &&
    fdef->base->func->value_ref->from->owner_class)
   env_push(env, NULL, env->context->nspc);
  env->scope->depth = 0;
  const m_bool ret  = traverse_func_def(env, l->def);
  env->scope->depth = scope;
    if(GET_FLAG(fdef->base, global) && !l->owner &&
    fdef->base->func->value_ref->from->owner_class)
   env_pop(env, scope);

  if (l->def->base->func) {
    if (env->curr->info->value != l->def->base->values->values) {
      free_scope(env->gwion->mp, env->curr->info->value);
      env->curr->info->value = l->def->base->values->values;
    }
  }

  if(ret < 0) {
    if(args) {
      for(uint32_t i = 0; i < bases->len; i++) {
      Arg *arg  = mp_vector_at(args, Arg, i);
        free_value(arg->var_decl.value, env->gwion);
        arg->var_decl.value = NULL;
      }
    }
  }
  return ret;
}

ANN m_bool check_lambda(const Env env, const Type t, Exp_Lambda *l) {
//  if (!l->def->base->func) return GW_ERROR;
  const Func_Def fdef = t->info->func->def;
  if (!GET_FLAG(t->info->func->value_ref, global))
    l->owner = t->info->value->from->owner_class;
  CHECK_BB(_check_lambda(env, l, fdef));
  exp_self(l)->type = l->def->base->func->value_ref->type;
  return GW_OK;
}

ANN static m_bool fptr_do(const Env env, struct FptrInfo *info) {
  if (isa(info->exp->type, env->gwion->type[et_lambda]) < 0) {
    CHECK_BB(fptr_check(env, info));
    if (!(info->exp->type = fptr_type(env, info)))
      ERR_B(info->pos, _("no match found"))
    return GW_OK;
  }
  Exp_Lambda *l = &info->exp->d.exp_lambda;
  return check_lambda(env, actual_type(env->gwion, info->rhs->value_ref->type), l);
}

ANN static Type partial2auto(const Env env, const Exp_Binary *bin) {
  const Func_Def fdef = bin->lhs->d.exp_lambda.def;
  unset_fbflag(fdef->base, fbflag_lambda);
  CHECK_BN(traverse_func_def(env, fdef));
  set_fbflag(fdef->base, fbflag_lambda);
  const Type actual = fdef->base->func->value_ref->type;
  set_fbflag(fdef->base, fbflag_lambda);
  Var_Decl vd = mp_vector_at(bin->rhs->d.exp_decl.list, struct Var_Decl_, 0);
  exp_setvar(bin->rhs, true);
  return vd->value->type = bin->rhs->type = bin->rhs->d.exp_decl.type = actual;
}

static OP_CHECK(opck_auto_fptr) {
  const Exp_Binary *bin = (Exp_Binary *)data;
  // we'll only deal with auto fptr declaration
  if (bin->rhs->exp_type != ae_exp_decl &&
      bin->rhs->d.exp_decl.td->xid != insert_symbol("auto"))
    ERR_N(bin->lhs->pos, "invalid {G+}function{0} {+}@=>{0} {+G}function{0} assignment");
  if (bin->lhs->exp_type == ae_exp_td)
    ERR_N(bin->lhs->pos, "can't use {/}type decl expressions{0} in auto function pointer declarations");
  if(!bin->lhs->type->info->func)
    return partial2auto(env, bin);
  // create a matching signature
  // TODO: we could check first if there a matching existing one
  Func_Base *const fbase =
      cpy_func_base(env->gwion->mp, bin->lhs->type->info->func->def->base);
  const Fptr_Def fptr_def = new_fptr_def(env->gwion->mp, fbase);
  char name[13 + strlen(env->curr->name) + num_digit(bin->rhs->pos.first.line) +
            num_digit(bin->rhs->pos.first.column)];
  sprintf(name, "generated@%s@%u:%u", env->curr->name, bin->rhs->pos.first.line,
          bin->rhs->pos.first.column);
  fptr_def->base->xid = insert_symbol(name);
  const m_bool ret    = traverse_fptr_def(env, fptr_def);
  const Type   t      = fptr_def->type;
  free_fptr_def(env->gwion->mp, fptr_def);
//  type_remref(t, env->gwion);
  Var_Decl vd = mp_vector_at(bin->rhs->d.exp_decl.list, struct Var_Decl_, 0);
  vd->value->type = bin->rhs->type =
      bin->rhs->d.exp_decl.type                = t;
  exp_setvar(bin->rhs, 1);
  return ret > 0 ? t : env->gwion->type[et_error];
}

static OP_CHECK(opck_fptr_at) {
  Exp_Binary *bin = (Exp_Binary *)data;
  if (bin->rhs->exp_type == ae_exp_decl) {
    Var_Decl vd = mp_vector_at(bin->rhs->d.exp_decl.list, struct Var_Decl_, 0);
    UNSET_FLAG(vd->value, late);
  }
  if (bin->lhs->exp_type == ae_exp_td)
    ERR_N(bin->lhs->pos, "can't use type_decl expression");
  //    UNSET_FLAG(bin->rhs->d.exp_decl.list->self->value, late);
  if (bin->rhs->type->info->func->def->base->tmpl &&
      bin->rhs->type->info->func->def->base->tmpl->call) {
    struct FptrInfo info = {bin->lhs->type->info->func ?: bin->lhs->type->info->parent->info->func,
                            bin->rhs->type->info->parent->info->func, bin->lhs,
                            exp_self(bin)->pos};
    CHECK_BN(fptr_do(env, &info));
    exp_setvar(bin->rhs, 1);
    return bin->rhs->type;
  }
  struct FptrInfo info = {bin->lhs->type->info->func ?: bin->lhs->type->info->parent->info->func,
                          bin->rhs->type->info->func, bin->lhs,
                          exp_self(bin)->pos};
  CHECK_BN(fptr_do(env, &info));
  exp_setvar(bin->rhs, 1);
  return bin->rhs->type;
}

/*
static OP_CHECK(opck_fptr_cast) {
  Exp_Cast *      cast = (Exp_Cast *)data;
  const Type      t    = exp_self(cast)->type;
  struct FptrInfo info = {cast->exp->type->info->func, t->info->func, cast->exp,
                          exp_self(cast)->pos};
  CHECK_BN(fptr_do(env, &info));
  return t;
}
*/
static void member_fptr(const Emitter emit) {
  const Instr instr = emit_add_instr(emit, RegMove);
  instr->m_val      = -SZ_INT;
  const Instr dup   = emit_add_instr(emit, Reg2Reg);
  dup->m_val        = -SZ_INT;
}

static inline int is_member(const Type from) {
  return vflag(from->info->func->value_ref, vflag_member);
}
/*
static OP_EMIT(opem_fptr_cast) {
  const Exp_Cast *cast = (Exp_Cast *)data;
  if (is_member(cast->exp->type)) member_fptr(emit);
  if (exp_self(cast)->type->info->func->def->base->tmpl)
    fptr_instr(emit, cast->exp->type->info->func, 1);
  return GW_OK;
}
*/
static OP_CHECK(opck_fptr_impl) {
  struct Implicit *impl = (struct Implicit *)data;
  struct FptrInfo  info = {impl->e->type->info->func, impl->t->info->func,
                          impl->e, impl->e->pos};
  CHECK_BN(fptr_do(env, &info));
  return impl->t;
}

static OP_CHECK(opck_fptr_cast) {
  Exp_Cast *cast = (Exp_Cast *)data;
  const Type t = known_type(env, cast->td);
  struct FptrInfo  info = {cast->exp->type->info->func, t->info->func,
                          cast->exp, cast->td->pos};
  CHECK_BN(fptr_do(env, &info));
  return t;
}

// smh the VM should be able to do that
static INSTR(Func2Code) {
  *(VM_Code *)REG(-SZ_INT) = (*(Func *)REG(-SZ_INT))->code;
}

static OP_EMIT(opem_fptr_impl) {
  struct Implicit *impl = (struct Implicit *)data;
  if (is_member(impl->e->type)) member_fptr(emit);
  if (impl->t->info->func->def->base->tmpl) {
    fptr_instr(emit, ((Exp)impl->e)->type->info->func, 1);
    if (!is_fptr(emit->gwion, impl->e->type) &&
        safe_tflag(impl->t->info->value->from->owner_class, tflag_tmpl)) {
      emit_add_instr(emit, Func2Code);
    }
  }
  return GW_OK;
}

static void op_narg_err(const Env env, const Func_Def fdef, const loc_t loc) {
  if (!env->context->error) {
    gwerr_basic(_("invalid operator decay"),
                _("Decayed operators take two arguments"), NULL, env->name, loc,
                0);
    if (fdef) defined_here(fdef->base->func->value_ref);
    env_set_error(env);
  }
}
static m_bool op_call_narg(const Env env, Exp arg, const loc_t loc) {
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
  CHECK_BO(op_call_narg(env, exp->args, exp->func->pos));
  const Exp base   = exp_self(exp);
  const Exp op_exp = exp->func;
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
/*
  m_uint   narg = 0;
  Arg_List arg  = fdef->base->args;
  while (arg) {
    narg++;
    arg = arg->next;
  }
*/
  Arg_List arg  = fdef->base->args;
  if (!arg && arg->len == 2) return GW_OK;
  op_narg_err(env, fdef, loc);
  return GW_ERROR;
}

static inline void op_impl_ensure_types(const Env env, const Func func) {
  const bool owner_tmpl =
      safe_tflag(func->value_ref->from->owner_class, tflag_tmpl);
  if (owner_tmpl)
    template_push_types(
        env, func->value_ref->from->owner_class->info->cdef->base.tmpl);
  const bool func_tmpl = fflag(func, fflag_tmpl);
  if (func_tmpl) template_push_types(env, func->def->base->tmpl);

  Arg_List args = func->def->base->args;
  for(uint32_t i = 0; i < args->len; i++) {
    Arg *arg = mp_vector_at(args, Arg, i);
    if (!arg->type) arg->type = known_type(env, arg->td);
  }
  if (!func->def->base->ret_type)
    func->def->base->ret_type = known_type(env, func->def->base->td);
  if (owner_tmpl) nspc_pop_type(env->gwion->mp, env->curr);
  if (func_tmpl) nspc_pop_type(env->gwion->mp, env->curr);
}

#include "tmp_resolve.h"

static OP_CHECK(opck_op_impl) {
  struct Implicit *impl = (struct Implicit *)data;
  const Func       func = impl->t->info->func;
  CHECK_BN(op_impl_narg(env, func->def, impl->e->pos));
  op_impl_ensure_types(env, func);
  const Symbol lhs_sym = insert_symbol("@lhs");
  const Symbol rhs_sym = insert_symbol("@rhs");
  const Arg *arg0 = (Arg*)(func->def->base->args->ptr);
  const Arg *arg1 = (Arg*)(func->def->base->args->ptr + sizeof(Arg));
  struct Exp_  _lhs    = {
      .d        = {.prim = {.d = {.var = lhs_sym}, .prim_type = ae_prim_id}},
      .exp_type = ae_exp_primary,
      .type     = arg0->type,
      .pos      = arg0->td->pos};
  struct Exp_ _rhs = {
      .d        = {.prim = {.d = {.var = rhs_sym}, .prim_type = ae_prim_id}},
      .exp_type = ae_exp_primary,
      .type     = arg1->type,
      .pos      = arg1->td->pos};
  struct Exp_ self = {.pos = impl->e->pos};
  //  Exp_Binary _bin = { .lhs=&_lhs, .op=impl->e->d.prim.d.var, .rhs=&_rhs };//
  //  .lhs=func->def->base->args // TODO
  self.d.exp_binary.lhs = &_lhs;
  self.d.exp_binary.rhs = &_rhs;
  self.d.exp_binary.op  = impl->e->d.prim.d.var;
  struct Op_Import opi  = {.op   = impl->e->d.prim.d.var,
                          .lhs  = arg0->type,
                          .rhs  = arg1->type,
                          .data = (uintptr_t)&self.d.exp_binary,
                          .pos  = impl->e->pos};
  vector_add(&env->scope->effects, 0);
  DECL_ON(const Type, t, = op_check(env, &opi));
  CHECK_BN(isa(t, func->def->base->ret_type)); // error message?
  MP_Vector *const eff = (MP_Vector*)vector_back(&env->scope->effects);
//  if (eff && !check_effect_overload(eff, func))
//    ERR_N(impl->pos, _("`{+Y}%s{0}` has effects not present in `{+G}%s{0}`\n"),
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
        free_mp_vector(env->gwion->mp, sizeof(struct ScopeEffect), eff);
        ERR_N(impl->pos,
              _("`{+Y}%s{0}` has effects not present in `{+G}%s{0}`\n"),
              s_name(impl->e->d.prim.d.var), func->name);
      }
      return actual_type(env->gwion, func->value_ref->type);
    }
  }
  const Arg_List args = cpy_arg_list(env->gwion->mp, func->def->base->args);
  Arg *larg0 = (Arg*)(args->ptr);
  Arg *larg1 = (Arg*)(args->ptr + sizeof(Arg));
  larg0->var_decl.xid       = rhs_sym;
  larg1->var_decl.xid = rhs_sym;
  Func_Base *base =
      new_func_base(env->gwion->mp, type2td(env->gwion, t, impl->e->pos),
                    impl->e->d.prim.d.var, args, ae_flag_none, impl->e->pos);
  if (eff) {
    for (m_uint i = 0; i < eff->len; i++) {
      struct ScopeEffect *effect = mp_vector_at(eff, struct ScopeEffect, i);
      vector_add(&base->effects, (m_uint)effect->sym);
    }
    free_mp_vector(env->gwion->mp, sizeof(struct ScopeEffect), eff);
  }
  const Exp lhs =
      new_prim_id(env->gwion->mp, larg0->var_decl.xid, impl->e->pos);
  const Exp rhs =
      new_prim_id(env->gwion->mp, larg1->var_decl.xid, impl->e->pos);
  const Exp  bin = new_exp_binary(env->gwion->mp, lhs, impl->e->d.prim.d.var,
                                 rhs, impl->e->pos);
  Stmt_List slist = new_mp_vector(env->gwion->mp, sizeof(struct Stmt_), 1);
  mp_vector_set(slist, struct Stmt_, 0,
    ((struct Stmt_) {
    .stmt_type = ae_stmt_return, .d = { .stmt_exp = { .val = bin }},
    .pos = impl->e->pos
  }));
  const Stmt      code = new_stmt_code(env->gwion->mp, slist, impl->e->pos);
  const Func_Def  def  = new_func_def(env->gwion->mp, base, code);
  def->base->xid       = impl->e->d.prim.d.var;
  const m_uint scope   = env_push(env, NULL, opi.nspc);
  // we assume succes here
  /*const m_bool ret = */ traverse_func_def(env, def);
  env_pop(env, scope);
  def->base->func->value_ref->type->info->parent = env->gwion->type[et_op];
  impl->e->type         = def->base->func->value_ref->type;
  impl->e->d.prim.value = def->base->func->value_ref;
  return actual_type(env->gwion, func->value_ref->type);
}

static OP_EMIT(opem_op_impl) {
  struct Implicit *impl  = (struct Implicit *)data;
  const Func_Def   fdef  = impl->e->type->info->func->def;
  const m_bool     ret   = emit_func_def(emit, fdef);
  const Instr      instr = emit_add_instr(emit, RegSetImm);
  instr->m_val           = (m_uint)fdef->base->func->code;
  instr->m_val2          = -SZ_INT;
  return ret;
}

ANN static Type fork_type(const Env env, const Exp_Unary *unary) {
  const Type t = unary->exp->type;
  if (t == env->gwion->type[et_void]) return env->gwion->type[et_fork];
  char c[21 + strlen(t->name)];
  sprintf(c, "TypedFork:[%s]", t->name);
  const Type fork = env->gwion->type[et_fork];
  UNSET_FLAG(fork, final);
  const Type typed = str2type(env->gwion, "TypedFork", exp_self(unary)->pos);
  // ensure room for retval
  if (typed->nspc->offset == fork->nspc->offset)
    typed->nspc->offset += t->size;
  UNSET_FLAG(typed, final);
  const Type ret = str2type(env->gwion, c, exp_self(unary)->pos);
  SET_FLAG(typed, final);
  SET_FLAG(fork, final);
  return ret;
}

ANN Type upvalue_type(const Env env, Capture *cap) {
  const Value v = nspc_lookup_value1(env->curr, cap->xid);
  if(!v) ERR_O(cap->pos, _("non existing value")); // did_you_mean
  if(cap->is_ref && not_upvalue(env, v))
    ERR_O(cap->pos, _("can't take ref of a scoped value"));
  cap->orig = v;
  const Type base_type = !tflag(v->type, tflag_ref) ? v->type : (Type)vector_front(&v->type->info->tuple->contains);
  return !cap->is_ref ? base_type :  ref_type(env->gwion, base_type, cap->pos);
}

static OP_CHECK(opck_spork) {
  const Exp_Unary *unary = (Exp_Unary *)data;
  if (unary->unary_type == unary_exp && unary->exp->exp_type == ae_exp_call) {
    const m_bool is_spork = unary->op == insert_symbol("spork");
    return is_spork ? env->gwion->type[et_shred] : fork_type(env, unary);
  }
  if (unary->unary_type == unary_code) {
    if(unary->captures) {
      uint32_t offset = !env->class_def ? 0 : SZ_INT;
      for(uint32_t i = 0; i < unary->captures->len; i++) {
        Capture *const cap = mp_vector_at(unary->captures, Capture, i);
        DECL_OO(const Type, t, = upvalue_type(env, cap));
        cap->temp = new_value(env, t, s_name(cap->xid), cap->pos);
        cap->temp->from->offset = offset;
        offset += cap->temp->type->size;
      }
    }
    Upvalues upvalues = { .values = env->curr->info->value };
    if(env->func && env->func->def->base->values)
      upvalues.parent = env->func->def->base->values;
    env->curr->info->value = new_scope(env->gwion->mp);
    if(unary->captures) {
      for(uint32_t i = 0; i < unary->captures->len; i++) {
        Capture *const cap = mp_vector_at(unary->captures, Capture, i);
        valid_value(env, cap->xid, cap->temp);
      }
    }
    const Func f = env->func;
    struct Value_ value = { .type = env->gwion->type[et_lambda]};
    if(env->class_def)
      set_vflag(&value, vflag_member);
    struct Func_Base_ fbase = { .xid=insert_symbol("in spork"), .values = &upvalues, .pos = exp_self(unary)->pos};
    set_fbflag(&fbase, fbflag_lambda);
    struct Func_Def_ fdef = { .base = &fbase};
    struct Func_ func = { .name = "in spork", .def = &fdef, .value_ref = &value};
    env->func = &func;
//    ++env->scope->depth;
//nspc_push_value(env->gwion->mp, env->curr);
    const m_bool ret = check_stmt(env, unary->code);
// nspc_push_value(env->gwion->mp, env->curr);
//    --env->scope->depth;
    env->func = f;
    free_scope(env->gwion->mp, env->curr->info->value);
    env->curr->info->value = upvalues.values;
    CHECK_BN(ret);
    return env->gwion
        ->type[unary->op == insert_symbol("spork") ? et_shred : et_fork];
  }
  ERR_O(exp_self(unary)->pos, _("only function calls can be sporked..."))
}

static OP_EMIT(opem_spork) {
  const Exp_Unary *unary = (Exp_Unary *)data;
  return emit_exp_spork(emit, unary);
}

static OP_CHECK(opck_func_partial) {
  Exp_Call *call = (Exp_Call*)data;
  return partial_type(env, call);
}

static OP_CHECK(opck_class_partial) {
  Exp_Call *call = (Exp_Call*)data;
  struct Op_Import opi = {.op   = insert_symbol("@partial"),
                          .lhs  = actual_type(env->gwion, call->func->type),
                          .pos  = call->func->pos,
                          .data = (uintptr_t)data};
   return op_check(env, &opi);
}

static FREEARG(freearg_xork) { vmcode_remref((VM_Code)instr->m_val, gwion); }

static FREEARG(freearg_dottmpl) {
  struct dottmpl_ *dt = (struct dottmpl_ *)instr->m_val;
  if (dt->tl) free_type_list(((Gwion)gwion)->mp, dt->tl);
  mp_free(((Gwion)gwion)->mp, dottmpl, dt);
}

GWION_IMPORT(func) {
  GWI_BB(gwi_oper_cond(gwi, "@func_ptr", BranchEqInt, BranchNeqInt))
  GWI_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "@function", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_func_call))
  GWI_BB(gwi_oper_end(gwi, "=>", NULL))
  GWI_BB(gwi_oper_ini(gwi, NULL, "@func_ptr", "bool"))
  GWI_BB(gwi_oper_end(gwi, "!", IntNot))
  GWI_BB(gwi_oper_ini(gwi, "@function", "@func_ptr", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_fptr_at))
  GWI_BB(gwi_oper_emi(gwi, opem_func_assign))
  GWI_BB(gwi_oper_end(gwi, "@=>", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_fptr_impl))
  GWI_BB(gwi_oper_emi(gwi, opem_fptr_impl))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_fptr_cast))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
  GWI_BB(gwi_oper_ini(gwi, "@op", "@func_ptr", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_op_impl))
  GWI_BB(gwi_oper_emi(gwi, opem_op_impl))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
  GWI_BB(gwi_oper_ini(gwi, NULL, (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_spork))
  GWI_BB(gwi_oper_emi(gwi, opem_spork))
  GWI_BB(gwi_oper_end(gwi, "spork", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_spork))
  GWI_BB(gwi_oper_emi(gwi, opem_spork))
  GWI_BB(gwi_oper_end(gwi, "fork", NULL))
  GWI_BB(gwi_oper_ini(gwi, "@function", "@function", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_auto_fptr))
  GWI_BB(gwi_oper_end(gwi, "@=>", int_r_assign))
  GWI_BB(gwi_oper_ini(gwi, "@function", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_func_partial))
  GWI_BB(gwi_oper_end(gwi, "@partial", NULL))
  GWI_BB(gwi_oper_ini(gwi, "Class", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_class_partial))
  GWI_BB(gwi_oper_end(gwi, "@partial", NULL))
  gwi_register_freearg(gwi, SporkIni, freearg_xork);
  gwi_register_freearg(gwi, DotTmpl, freearg_dottmpl);
  return GW_OK;
}
