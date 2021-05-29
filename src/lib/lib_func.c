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

static OP_CHECK(opck_func_call) {
  Exp_Binary* bin = (Exp_Binary*)data;
  Exp_Call call = { .func=bin->rhs, .args=bin->lhs };
  Exp e = exp_self(bin);
  e->exp_type = ae_exp_call;
  memcpy(&e->d.exp_call, &call, sizeof(Exp_Call));
  return check_exp_call1(env, &e->d.exp_call) ?: env->gwion->type[et_error];
}

static inline void fptr_instr(const Emitter emit, const Func f, const m_uint i) {
  const Instr set = emit_add_instr(emit, RegSetImm);
  set->m_val = (m_uint)f;
  set->m_val2 = -SZ_INT*i;
}

static OP_EMIT(opem_func_assign) {
  Exp_Binary* bin = (Exp_Binary*)data;
  if(bin->rhs->type->info->func->def->base->tmpl)
    fptr_instr(emit, bin->lhs->type->info->func, 2);
  (void)emit_add_instr(emit, int_r_assign);
  if(!is_fptr(emit->gwion, bin->lhs->type) && vflag(bin->rhs->type->info->func->value_ref, vflag_member)) {
    const Instr pop = emit_add_instr(emit, RegMove);
    pop->m_val = -SZ_INT;
    const Instr cpy = emit_add_instr(emit, Reg2Reg);
    cpy->m_val = -SZ_INT;
  }
  return GW_OK;
}

struct FptrInfo {
        Func  lhs;
  const Func  rhs;
  const Exp   exp;
  const loc_t pos;
};

ANN static void _fptr_tmpl_push(const Env env, const Func f) {
  const Tmpl *tmpl = f->def->base->tmpl;
  if(!tmpl)
    return;
  Type_List tl = tmpl->call;
  if(!tl)
    return;
  Specialized_List il = tmpl->list;
  while(il) {
    const Type t = known_type(env, tl->td);
    nspc_add_type(env->curr, il->xid, t);
    il = il->next;
    tl = tl->next;
  }
}

ANN static m_bool fptr_tmpl_push(const Env env, struct FptrInfo *info) {
//  if(safe_tflag(info->rhs->value_ref->from->owner_class, tflag_tmpl))
  if(!info->rhs->def->base->tmpl)
    return GW_OK;
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
  if(isa(t0, t1) > 0)
    return GW_OK;
  return t1 == env->gwion->type[et_auto] ? GW_OK:GW_ERROR;
}

ANN static m_bool fptr_args(const Env env, Func_Base *base[2]) {
  Arg_List arg0 = base[0]->args, arg1 = base[1]->args;
  while(arg0) {
    CHECK_OB(arg1);
    if(arg0->type && arg1->type)
      CHECK_BB(isa(arg0->type, arg1->type));
    else {
      Type_Decl* td[2] = { arg0->td, arg1->td };
      CHECK_BB(td_match(env, td));
    }
    arg0 = arg0->next;
    arg1 = arg1->next;
  }
  return !arg1 ? GW_OK : GW_ERROR;
}

ANN static bool fptr_effects(const Env env, struct FptrInfo *info) {
  if(!info->lhs->def->base->effects.ptr)
    return true;
  if(!info->rhs->def->base->effects.ptr) {
    gwerr_secondary("too many effects", env->name, info->pos);
    return false;
  }
  const Vector lhs = &info->lhs->def->base->effects;
  const Vector rhs = &info->rhs->def->base->effects;
  for(m_uint i = 0; i < vector_size(lhs); i++) {
    if(vector_find(rhs, vector_at(lhs, 0)) == -1) {
      gwerr_secondary("effect not handled", env->name, info->pos);
      return false;
    }
  }
  return true;
}

ANN static m_bool fptr_check(const Env env, struct FptrInfo *info) {
//  if(!info->lhs->def->base->tmpl != !info->rhs->def->base->tmpl)
//    return GW_ERROR;
  const Type l_type = info->lhs->value_ref->from->owner_class;
  const Type r_type = info->rhs->value_ref->from->owner_class;
  if(!r_type && l_type)
    ERR_B(info->pos, _("can't assign member function to non member function pointer"))
  else if(!l_type && r_type) {
    if(!GET_FLAG(info->rhs, global))
      ERR_B(info->pos, _("can't assign non member function to member function pointer"))
  } else if(l_type && isa(r_type, l_type) < 0)
      ERR_B(info->pos, _("can't assign member function to a pointer of an other class"))
  if(vflag(info->rhs->value_ref, vflag_member)) {
    if(!vflag(info->lhs->value_ref, vflag_member))
      ERR_B(info->pos, _("can't assign static function to member function pointer"))
  } else if(vflag(info->lhs->value_ref, vflag_member))
      ERR_B(info->pos, _("can't assign member function to static function pointer"))
  return GW_OK;
}

ANN static inline m_bool fptr_rettype(const Env env, struct FptrInfo *info) {
  Type_Decl* td[2] = { info->lhs->def->base->td,
      info->rhs->def->base->td };
  return td_match(env, td);
}

ANN static inline m_bool fptr_arity(struct FptrInfo *info) {
  return fbflag(info->lhs->def->base, fbflag_variadic) ==
         fbflag(info->rhs->def->base, fbflag_variadic);
}

ANN static Type fptr_type(const Env env, struct FptrInfo *info) {
  const Value v = info->lhs->value_ref;
  const Nspc nspc = v->from->owner;
  const m_str c = s_name(info->lhs->def->base->xid),
    stmpl = !info->rhs->def->base->tmpl ? NULL : "template";
  for(m_uint i = 0; i <= v->from->offset; ++i) {
    const Symbol sym = (!info->lhs->def->base->tmpl || i != 0) ?
        func_symbol(env, nspc->name, c, stmpl, i) : info->lhs->def->base->xid;
    if(!is_class(env->gwion, info->lhs->value_ref->type)) {
      if(!(info->lhs = nspc_lookup_func1(nspc, sym))) {
        const Value v = nspc_lookup_value1(nspc, insert_symbol(c));
        if(isa(v->type, env->gwion->type[et_function]) < 0)
          return NULL;
        info->lhs = v->type->info->func;
      }
    } else {
      DECL_OO(const Type, t, = nspc_lookup_type1(nspc, info->lhs->def->base->xid));
      info->lhs = actual_type(env->gwion, t)->info->func;
    }
    Type type = NULL;
    Func_Base *base[2] =  { info->lhs->def->base, info->rhs->def->base };
    CHECK_BO(fptr_tmpl_push(env, info));
    if (fptr_rettype(env, info) > 0 && fptr_arity(info) &&
        fptr_args(env, base) > 0 && fptr_effects(env, info))
      type = actual_type(env->gwion, info->lhs->value_ref->type) ?: info->lhs->value_ref->type;
    if(info->rhs->def->base->tmpl)
      nspc_pop_type(env->gwion->mp, env->curr);
    if(type)return type;
  }
  return NULL;
}

ANN static m_bool _check_lambda(const Env env, Exp_Lambda *l, const Func_Def def) {
//if(l->def->base->func)return GW_OK;
  const bool is_tmpl = safe_tflag(def->base->func->value_ref->from->owner_class, tflag_tmpl);
  if(is_tmpl)
    template_push_types(env, def->base->func->value_ref->from->owner_class->info->cdef->base.tmpl);
  Arg_List base = def->base->args, arg = l->def->base->args;
  while(base && arg) {
    arg->td = type2td(env->gwion, known_type(env, base->td), exp_self(l)->pos);
    base = base->next;
    arg = arg->next;
  }
  l->def->base->td = type2td(env->gwion, known_type(env, def->base->td), exp_self(l)->pos);
  if(is_tmpl)
    nspc_pop_type(env->gwion->mp, env->curr);
  if(base || arg)
    ERR_B(exp_self(l)->pos, _("argument number does not match for lambda"))
  l->def->base->flag = def->base->flag;
//  if(GET_FLAG(def->base, global) && !l->owner && def->base->func->value_ref->from->owner_class)
    UNSET_FLAG(l->def->base, global);
  l->def->base->values = env->curr->info->value;
  const m_uint scope = env->scope->depth;
//  if(GET_FLAG(def->base, global) && !l->owner && def->base->func->value_ref->from->owner_class)
//env_push(env, NULL, env->context->nspc);
  env->scope->depth = 0;
  const m_bool ret = traverse_func_def(env, l->def);
  env->scope->depth = scope;
//  if(GET_FLAG(def->base, global) && !l->owner && def->base->func->value_ref->from->owner_class)
//env_pop(env, scope);
  if(l->def->base->func) {
    if(env->curr->info->value != l->def->base->values) {
      free_scope(env->gwion->mp, env->curr->info->value);
      env->curr->info->value = l->def->base->values;
    }
  }
  return ret;
}

ANN m_bool check_lambda(const Env env, const Type t, Exp_Lambda *l) {
  const Func_Def fdef = t->info->func->def;
  if(!GET_FLAG(t->info->func->value_ref, global))
    l->owner = t->info->value->from->owner_class;
  CHECK_BB(_check_lambda(env, l, fdef));
  exp_self(l)->type = l->def->base->func->value_ref->type;
  return GW_OK;
}

ANN static m_bool fptr_do(const Env env, struct FptrInfo *info) {
  if(isa(info->exp->type, env->gwion->type[et_lambda]) < 0) {
    CHECK_BB(fptr_check(env, info));
    if(!(info->exp->type = fptr_type(env, info)))
      ERR_B(info->pos, _("no match found"))
    return GW_OK;
  }
  Exp_Lambda *l = &info->exp->d.exp_lambda;
  return check_lambda(env, actual_type(env->gwion, info->rhs->value_ref->type), l);
}

static OP_CHECK(opck_auto_fptr) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  // we'll only deal with auto fptr declaration
  if(bin->rhs->exp_type != ae_exp_decl && bin->rhs->d.exp_decl.td->xid != insert_symbol("auto"))
    return env->gwion->type[et_error];
  // create a matching signature
  // TODO: we could check first if there a matching existing one
  Func_Base *const fbase = cpy_func_base(env->gwion->mp, bin->lhs->type->info->func->def->base);
  const Fptr_Def fptr_def = new_fptr_def(env->gwion->mp, fbase);
  char name[13 + strlen(env->curr->name) +
    num_digit(bin->rhs->pos.first.line) + num_digit(bin->rhs->pos.first.column)];
  sprintf(name, "generated@%s@%u:%u", env->curr->name, bin->rhs->pos.first.line, bin->rhs->pos.first.column);
  fptr_def->base->xid = insert_symbol(name);
  const m_bool ret = traverse_fptr_def(env, fptr_def);
  const Type t = fptr_def->type;
  free_fptr_def(env->gwion->mp, fptr_def);
  type_remref(t, env->gwion);
  bin->rhs->d.exp_decl.list->self->value->type = bin->rhs->type = bin->rhs->d.exp_decl.type = t;
  exp_setvar(bin->rhs, 1);
  return ret > 0 ? t : env->gwion->type[et_error];
}

static OP_CHECK(opck_fptr_at) {
  Exp_Binary* bin = (Exp_Binary*)data;
  if(bin->rhs->exp_type == ae_exp_decl)
    UNSET_FLAG(bin->rhs->d.exp_decl.list->self->value, late);
  if(bin->rhs->type->info->func->def->base->tmpl &&
     bin->rhs->type->info->func->def->base->tmpl->call) {
    struct FptrInfo info = { bin->lhs->type->info->func, bin->rhs->type->info->parent->info->func,
      bin->lhs, exp_self(bin)->pos };
    CHECK_BN(fptr_do(env, &info));
    exp_setvar(bin->rhs, 1);
    return bin->rhs->type;
  }
  struct FptrInfo info = { bin->lhs->type->info->func, bin->rhs->type->info->func,
      bin->lhs, exp_self(bin)->pos };
  CHECK_BN(fptr_do(env, &info));
  exp_setvar(bin->rhs, 1);
  return bin->rhs->type;
}

static OP_CHECK(opck_fptr_cast) {
  Exp_Cast* cast = (Exp_Cast*)data;
  const Type t = exp_self(cast)->type;
  struct FptrInfo info = { cast->exp->type->info->func, t->info->func,
     cast->exp, exp_self(cast)->pos };
  CHECK_BN(fptr_do(env, &info));
  return t;
}

static void member_fptr(const Emitter emit) {
  const Instr instr = emit_add_instr(emit, RegMove);
  instr->m_val = -SZ_INT;
  const Instr dup = emit_add_instr(emit, Reg2Reg);
  dup->m_val = -SZ_INT;
}

static inline int is_member(const Type from) {
  return vflag(from->info->func->value_ref, vflag_member);
}

static OP_EMIT(opem_fptr_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  if(is_member(cast->exp->type))
    member_fptr(emit);
  if(exp_self(cast)->type->info->func->def->base->tmpl)
    fptr_instr(emit, cast->exp->type->info->func, 1);
  return GW_OK;
}

static OP_CHECK(opck_fptr_impl) {
  struct Implicit *impl = (struct Implicit*)data;
  struct FptrInfo info = { impl->e->type->info->func, impl->t->info->func,
      impl->e, impl->e->pos };
  CHECK_BN(fptr_do(env, &info));
  return impl->t;
}

// smh the VM should be able to do that
static INSTR(Func2Code) {
  *(VM_Code*)REG(-SZ_INT) = (*(Func*)REG(-SZ_INT))->code;
}

static OP_EMIT(opem_fptr_impl) {
  struct Implicit *impl = (struct Implicit*)data;
  if(is_member(impl->e->type))
    member_fptr(emit);
  if(impl->t->info->func->def->base->tmpl) {
    fptr_instr(emit, ((Exp)impl->e)->type->info->func, 1);
    if(!is_fptr(emit->gwion, impl->e->type) && safe_tflag(impl->t->info->value->from->owner_class, tflag_tmpl)){
      emit_add_instr(emit, Func2Code);
    }
  }
  return GW_OK;
}

static void op_narg_err(const Env env, const Func_Def fdef, const loc_t loc) {
  if(!env->context->error) {
    gwerr_basic(_("invalid operator decay"), _("Decayed operators take two arguments"), NULL,
      env->name, loc, 0);
    if(fdef)
      gwerr_secondary("declared here", env->name, fdef->base->pos);
    env->context->error = true;
  }
}

static m_bool op_call_narg(const Env env, Exp arg,
    const loc_t loc) {
  m_uint narg = 0;
  while(arg) {
    narg++;
    arg = arg->next;
  }
  if(narg == 2)
    return GW_OK;
  op_narg_err(env, NULL, loc);
  return GW_ERROR;
}

ANN Type check_op_call(const Env env, Exp_Call *const exp) {
  CHECK_BO(op_call_narg(env, exp->args, exp->func->pos));
  const Exp base = exp_self(exp);
  const Exp op_exp = exp->func;
  base->exp_type = ae_exp_binary;
  Exp_Binary *bin = &base->d.exp_binary;
  bin->lhs = exp->args;
  bin->lhs = exp->args->next;
  exp->args->next = NULL;
  bin->op = op_exp->d.prim.d.var;
  free_exp(env->gwion->mp, op_exp);
  return check_exp(env, base);
}

static m_bool op_impl_narg(const Env env, const Func_Def fdef,
    const loc_t loc) {
  m_uint narg = 0;
  Arg_List arg = fdef->base->args;
  while(arg) {
    narg++;
    arg = arg->next;
  }
  if(narg == 2)
    return GW_OK;
  op_narg_err(env, fdef, loc);
  return GW_ERROR;
}

static inline void op_impl_ensure_types(const Env env, const Func func) {
  Arg_List arg = func->def->base->args;
  const bool owner_tmpl = safe_tflag(func->value_ref->from->owner_class, tflag_tmpl);
  const bool func_tmpl = fflag(func, fflag_tmpl);
  if(owner_tmpl)
    template_push_types(env, func->value_ref->from->owner_class->info->cdef->base.tmpl);
  if(func_tmpl)
    template_push_types(env, func->def->base->tmpl);
  while(arg) {
    if(!arg->type)
      arg->type = known_type(env, arg->td);
    arg = arg->next;
  }
  if(!func->def->base->ret_type)
    func->def->base->ret_type = known_type(env, func->def->base->td);
  if(owner_tmpl)
    nspc_pop_type(env->gwion->mp, env->curr);
  if(func_tmpl)
    nspc_pop_type(env->gwion->mp, env->curr);
}

#include "tmp_resolve.h"
static OP_CHECK(opck_op_impl){
  struct Implicit *impl = (struct Implicit*)data;
  const Func func = impl->t->info->func;
  CHECK_BN(op_impl_narg(env, func->def, impl->e->pos));
  op_impl_ensure_types(env, func);
  const Symbol lhs_sym = insert_symbol("@lhs");
  const Symbol rhs_sym = insert_symbol("@rhs");
  struct Exp_ _lhs = { .d={ .prim={ .d={.var=lhs_sym} , .prim_type=ae_prim_id }}, .exp_type=ae_exp_primary, .type=func->def->base->args->type, .pos=func->def->base->args->td->pos };
  struct Exp_ _rhs = { .d={ .prim={ .d={.var=rhs_sym} , .prim_type=ae_prim_id }}, .exp_type=ae_exp_primary, .type=func->def->base->args->next->type, .pos=func->def->base->args->next->td->pos };
  struct Exp_ self = { .pos=impl->e->pos };
//  Exp_Binary _bin = { .lhs=&_lhs, .op=impl->e->d.prim.d.var, .rhs=&_rhs };// .lhs=func->def->base->args // TODO
  self.d.exp_binary.lhs = &_lhs;
  self.d.exp_binary.rhs = &_rhs;
  self.d.exp_binary.op = impl->e->d.prim.d.var;
  struct Op_Import opi = { .op=impl->e->d.prim.d.var, .lhs=func->def->base->args->type,
    .rhs=func->def->base->args->next->type, .data=(uintptr_t)&self.d.exp_binary, .pos=impl->e->pos };
  vector_add(&env->scope->effects, 0);
  DECL_ON(const Type, t, = op_check(env, &opi));
  CHECK_BN(isa(t, func->def->base->ret_type)); // error message?
  const m_uint _eff = vector_back(&env->scope->effects);
  if(!check_effect_overload((Vector)&_eff, func))
    ERR_N(impl->pos, _("`{+Y}%s{0}` has effects not present in `{+G}%s{0}`\n"), s_name(impl->e->d.prim.d.var), func->name);
  Value v = nspc_lookup_value0(opi.nspc, impl->e->d.prim.d.var);
  if(v) {
      const m_uint scope = env_push(env, NULL, opi.nspc);
      _lhs.next = &_rhs;
      Exp_Call call = { .args=&_lhs };
      const Func exists = (Func)find_func_match(env, v->d.func_ref, &call);
      env_pop(env, scope);
      if(exists) { // improve me
        if(_eff) {
          const M_Vector eff = (M_Vector)&_eff;
          m_vector_release(eff);
          ERR_N(impl->pos, _("`{+Y}%s{0}` has effects not present in `{+G}%s{0}`\n"), s_name(impl->e->d.prim.d.var), func->name);
        }
        return actual_type(env->gwion, func->value_ref->type);
      }
  }
  const Arg_List args = cpy_arg_list(env->gwion->mp, func->def->base->args);
  // beware shadowing ?
  args->var_decl->xid = lhs_sym;
  args->next->var_decl->xid = rhs_sym;
  Func_Base *base = new_func_base(env->gwion->mp, type2td(env->gwion, t, impl->e->pos),
       impl->e->d.prim.d.var, args, ae_flag_none, impl->e->pos);
  if(_eff) {
    const M_Vector eff = (M_Vector)&_eff;
    for (m_uint i = 0; i < m_vector_size(eff); i++) {
      struct ScopeEffect effect;
      m_vector_get(eff, i, &effect);
      vector_add(&base->effects, (m_uint)effect.sym);
    }
    m_vector_release(eff);
  }
  const Exp lhs = new_prim_id(env->gwion->mp, args->var_decl->xid, impl->e->pos);
  const Exp rhs = new_prim_id(env->gwion->mp, args->next->var_decl->xid, impl->e->pos);
  const Exp bin = new_exp_binary(env->gwion->mp, lhs, impl->e->d.prim.d.var, rhs, impl->e->pos);
  const Stmt stmt = new_stmt_exp(env->gwion->mp, ae_stmt_return, bin, impl->e->pos);
  const Stmt_List list = new_stmt_list(env->gwion->mp, stmt, NULL);
  const Stmt code = new_stmt_code(env->gwion->mp, list, impl->e->pos);
  const Func_Def def = new_func_def(env->gwion->mp, base, code);
  def->base->xid = impl->e->d.prim.d.var;
  const m_uint scope = env_push(env, NULL, opi.nspc);
  // we assume succes here
  /*const m_bool ret = */traverse_func_def(env, def);
  env_pop(env, scope);
  def->base->func->value_ref->type->info->parent = env->gwion->type[et_op];
  impl->e->type = def->base->func->value_ref->type;
  impl->e->d.prim.value = def->base->func->value_ref;
  return actual_type(env->gwion, func->value_ref->type);
}

static OP_EMIT(opem_op_impl) {
  struct Implicit *impl = (struct Implicit*)data;
  const Func_Def fdef = impl->e->type->info->func->def;
  const m_bool ret = emit_func_def(emit, fdef);
  const Instr instr = emit_add_instr(emit, RegSetImm);
  instr->m_val = (m_uint)fdef->base->func->code;
  instr->m_val2 = -SZ_INT;
  return ret;
}

ANN Type check_exp_unary_spork(const Env env, const Stmt code);

ANN static void fork_exp(const Env env, const Exp_Unary* unary) {
  const Stmt stmt = new_stmt_exp(env->gwion->mp, ae_stmt_exp, unary->exp, unary->exp->pos);
  const Stmt_List list = new_stmt_list(env->gwion->mp, stmt, NULL);
  const Stmt code = new_stmt_code(env->gwion->mp, list, unary->exp->pos);
  ((Exp_Unary*)unary)->exp = NULL;
  ((Exp_Unary*)unary)->code = code;
  ((Exp_Unary*)unary)->unary_type = unary_code;
}

ANN static Type fork_type(const Env env, const Exp_Unary* unary) {
  const Type t = unary->exp->type;
  fork_exp(env, unary);
  if(t == env->gwion->type[et_void])
    return env->gwion->type[et_fork];
  char c[21 + strlen(t->name)];
  sprintf(c, "TypedFork:[%s]", t->name);
  const Type fork = env->gwion->type[et_fork];
  UNSET_FLAG(fork, final);
  const Type typed = str2type(env->gwion, "TypedFork", exp_self(unary)->pos);
  // ensure room for retval
  if(typed->nspc->info->offset == fork->nspc->info->offset)
    typed->nspc->info->offset += t->size;
  UNSET_FLAG(typed, final);
  const Type ret = str2type(env->gwion, c, exp_self(unary)->pos);
  SET_FLAG(typed, final);
  SET_FLAG(fork, final);
  return ret;
}

static OP_CHECK(opck_spork) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  if(unary->unary_type == unary_exp && unary->exp->exp_type == ae_exp_call) {
    const m_bool is_spork = unary->op == insert_symbol("spork");
    return is_spork ? env->gwion->type[et_shred] : fork_type(env, unary);
  }
  if(unary->unary_type == unary_code) {
    ++env->scope->depth;
    nspc_push_value(env->gwion->mp, env->curr);
    const m_bool ret = check_stmt(env, unary->code);
    nspc_pop_value(env->gwion->mp, env->curr);
    --env->scope->depth;
    CHECK_BN(ret);
    return env->gwion->type[unary->op == insert_symbol("spork") ? et_shred : et_fork];
  }
  ERR_O(exp_self(unary)->pos, _("only function calls can be sporked..."))
}

static OP_EMIT(opem_spork) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  return emit_exp_spork(emit, unary);
}

static FREEARG(freearg_xork) {
  vmcode_remref((VM_Code)instr->m_val, gwion);
}

static FREEARG(freearg_dottmpl) {
  struct dottmpl_ *dt = (struct dottmpl_*)instr->m_val;
  if(dt->tl)
    free_type_list(((Gwion)gwion)->mp, dt->tl);
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
  GWI_BB(gwi_oper_add(gwi, opck_fptr_cast))
  GWI_BB(gwi_oper_emi(gwi, opem_fptr_cast))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_fptr_impl))
  GWI_BB(gwi_oper_emi(gwi, opem_fptr_impl))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
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
  GWI_BB(gwi_oper_end(gwi, "@=>", NULL))
  gwi_register_freearg(gwi, SporkIni, freearg_xork);
  gwi_register_freearg(gwi, DotTmpl, freearg_dottmpl);
  return GW_OK;
}
