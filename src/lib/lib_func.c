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
  ++*mut;
  return check_exp_call1(env, &e->d.exp_call) ?: env->gwion->type[et_null];
}

static inline void fptr_instr(const Emitter emit, const Func f, const m_uint i) {
  const Instr set = emit_add_instr(emit, RegSetImm);
  set->m_val = (m_uint)f;
  set->m_val2 = -SZ_INT*i;
}

static OP_EMIT(opem_func_assign) {
  Exp_Binary* bin = (Exp_Binary*)data;
  if(bin->rhs->info->type->e->d.func->def->base->tmpl)
    fptr_instr(emit, bin->lhs->info->type->e->d.func, 2);
  const Instr instr = emit_add_instr(emit, int_r_assign);
  if(!is_fptr(emit->gwion, bin->lhs->info->type) && GET_FLAG(bin->rhs->info->type->e->d.func, member)) {
    const Instr pop = emit_add_instr(emit, RegPop);
    pop->m_val = SZ_INT;
    const Instr cpy = emit_add_instr(emit, Reg2Reg);
    cpy->m_val = -SZ_INT;
  }
  return instr;
}

struct FptrInfo {
        Func  lhs;
  const Func  rhs;
  const Exp   exp;
  const loc_t pos;
};

ANN static m_bool fptr_tmpl_push(const Env env, struct FptrInfo *info) {
  if(!info->rhs->def->base->tmpl)
    return GW_OK;
  ID_List t0 = info->lhs->def->base->tmpl->list,
          t1 = info->rhs->def->base->tmpl->list;
  nspc_push_type(env->gwion->mp, env->curr);
  while(t0) {
    nspc_add_type(env->curr, t0->xid, env->gwion->type[et_undefined]);
    nspc_add_type(env->curr, t1->xid, env->gwion->type[et_undefined]);
    t0 = t0->next;
    t1 = t1->next;
  }
  return GW_OK;
}

static m_bool td_match(const Env env, Type_Decl *id[2]) {
  DECL_OB(const Type, t0, = known_type(env, id[0]))
  DECL_OB(const Type, t1, = known_type(env, id[1]))
  return isa(t0, t1);
}

ANN static m_bool fptr_args(const Env env, Func_Base *base[2]) {
  Arg_List arg0 = base[0]->args, arg1 = base[1]->args;
  while(arg0) {
    CHECK_OB(arg1)
    Type_Decl* td[2] = { arg0->td, arg1->td };
    CHECK_BB(td_match(env, td))
    arg0 = arg0->next;
    arg1 = arg1->next;
  }
  return !arg1 ? GW_OK : GW_ERROR;
}

ANN static m_bool fptr_check(const Env env, struct FptrInfo *info) {
  if(!info->lhs->def->base->tmpl != !info->rhs->def->base->tmpl)
    return GW_ERROR;
  const Type l_type = info->lhs->value_ref->from->owner_class;
  const Type r_type = info->rhs->value_ref->from->owner_class;
  if(!r_type && l_type)
    ERR_B(info->pos, _("can't assign member function to non member function pointer"))
  else if(!l_type && r_type) {
    if(!GET_FLAG(info->rhs, global))
      ERR_B(info->pos, _("can't assign non member function to member function pointer"))
  } else if(l_type && isa(r_type, l_type) < 0)
      ERR_B(info->pos, _("can't assign member function to a pointer of an other class"))
  if(GET_FLAG(info->rhs, member)) {
    if(!GET_FLAG(info->lhs, member))
      ERR_B(info->pos, _("can't assign static function to member function pointer"))
  } else if(GET_FLAG(info->lhs, member))
      ERR_B(info->pos, _("can't assign member function to static function pointer"))
  return GW_OK;
}

ANN static inline m_bool fptr_rettype(const Env env, struct FptrInfo *info) {
  Type_Decl* td[2] = { info->lhs->def->base->td,
      info->rhs->def->base->td };
  return td_match(env, td);
}

ANN static inline m_bool fptr_arity(struct FptrInfo *info) {
  return GET_FLAG(info->lhs->def, variadic) ==
         GET_FLAG(info->rhs->def, variadic);
}

ANN static Type fptr_type(const Env env, struct FptrInfo *info) {
  const Value v = info->lhs->value_ref;
  const Nspc nspc = v->from->owner;
  const m_str c = s_name(info->lhs->def->base->xid),
    stmpl = !info->rhs->def->base->tmpl ? NULL : "template";
  Type type = NULL;
  for(m_uint i = 0; i <= v->from->offset && !type; ++i) {
    const Symbol sym = (!info->lhs->def->base->tmpl || i != 0) ?
        func_symbol(env, nspc->name, c, stmpl, i) : info->lhs->def->base->xid;
    if(!is_class(env->gwion, info->lhs->value_ref->type))
      CHECK_OO((info->lhs = nspc_lookup_func1(nspc, sym)))
    else {
      DECL_OO(const Type, t, = nspc_lookup_type1(nspc, info->lhs->def->base->xid))
      info->lhs = actual_type(env->gwion, t)->e->d.func;
    }
    Func_Base *base[2] =  { info->lhs->def->base, info->rhs->def->base };
    if(fptr_tmpl_push(env, info) > 0) {
      if(fptr_rettype(env, info) > 0 &&
           fptr_arity(info) && fptr_args(env, base) > 0)
      type = actual_type(env->gwion, info->lhs->value_ref->type) ?: info->lhs->value_ref->type;
      if(info->rhs->def->base->tmpl)
        nspc_pop_type(env->gwion->mp, env->curr);
    }
  }
  return type;
}

ANN static m_bool _check_lambda(const Env env, Exp_Lambda *l, const Func_Def def) {
  Arg_List base = def->base->args, arg = l->def->base->args;
  while(base && arg) {
    arg->td = base->td;
    base = base->next;
    arg = arg->next;
  }
  if(base || arg)
    ERR_B(exp_self(l)->pos, _("argument number does not match for lambda"))
  l->def->flag = def->flag;
  l->def->base->td = cpy_type_decl(env->gwion->mp, def->base->td);
  SET_FLAG(l->def, abstract); // mark as non immediate lambda
  map_set(&env->curr->info->func->map, (m_uint)l->def->base, env->scope->depth);
  const m_bool ret = check_traverse_fdef(env, l->def);
  map_remove(&env->curr->info->func->map, (m_uint)l->def->base);
  CHECK_BB(ret)
  arg = l->def->base->args;
  while(arg) {
    arg->td = NULL;
    arg = arg->next;
  }
  return GW_OK;
}

ANN m_bool check_lambda(const Env env, const Type t, Exp_Lambda *l) {
  const Func_Def fdef = t->e->d.func->def;
  struct EnvSet es = { .env=env, .data=env, .func=(_exp_func)check_cdef,
    .scope=env->scope->depth, .flag=ae_flag_check };
  l->owner = t->e->owner_class;
  CHECK_BB(envset_push(&es, l->owner, t->e->owner))
  const m_bool ret = _check_lambda(env, l, fdef);
  if(es.run)
    envset_pop(&es, l->owner);
  if(ret < 0)
    return GW_ERROR;
  exp_self(l)->info->type = l->def->base->func->value_ref->type;
  return GW_OK;
}

ANN static m_bool fptr_do(const Env env, struct FptrInfo *info) {
  if(isa(info->exp->info->type, env->gwion->type[et_lambda]) < 0) {
    m_bool nonnull = GET_FLAG(info->exp->info->type, nonnull);
    CHECK_BB(fptr_check(env, info))
    DECL_OB(const Type, t, = fptr_type(env, info))
    info->exp->info->type = !nonnull ? t : nonnul_type(env, t);
    return GW_OK;
  }
  Exp_Lambda *l = &info->exp->d.exp_lambda;
  return check_lambda(env, actual_type(env->gwion, info->rhs->value_ref->type), l);
}

static OP_CHECK(opck_auto_fptr) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  // we'll only deal with auto fptr declaration
  if(bin->rhs->exp_type != ae_exp_decl && bin->rhs->d.exp_decl.td->xid != insert_symbol("auto"))
    return env->gwion->type[et_null];
  // create a matching signature
  // TODO: we could check first if there a matching existing one
  Func_Base *const fbase = cpy_func_base(env->gwion->mp, bin->lhs->info->type->e->d.func->def->base);
  const Fptr_Def fptr_def = new_fptr_def(env->gwion->mp, fbase, bin->lhs->info->type->e->d.func->def->flag);
  char name[13 + strlen(env->curr->name) +
    num_digit(bin->rhs->pos->first.line) + num_digit(bin->rhs->pos->first.column)];
  sprintf(name, "generated@%s@%u:%u", env->curr->name, bin->rhs->pos->first.line, bin->rhs->pos->first.column);
  fptr_def->base->xid = insert_symbol(name);
  const m_bool ret = traverse_fptr_def(env, fptr_def);
  const Type t = fptr_def->type;
  free_fptr_def(env->gwion->mp, fptr_def);
  REM_REF(t, env->gwion)
  bin->rhs->d.exp_decl.list->self->value->type = bin->rhs->info->type = bin->rhs->d.exp_decl.type = t;
  exp_setvar(bin->rhs, 1);
  return ret > 0 ? t : env->gwion->type[et_null];
}

static OP_CHECK(opck_fptr_at) {
  Exp_Binary* bin = (Exp_Binary*)data;
  if(bin->rhs->info->type->e->d.func->def->base->tmpl &&
     bin->rhs->info->type->e->d.func->def->base->tmpl->call) {
    struct FptrInfo info = { bin->lhs->info->type->e->d.func, bin->rhs->info->type->e->parent->e->d.func,
      bin->lhs, exp_self(bin)->pos };
    CHECK_BO(fptr_do(env, &info))
    exp_setvar(bin->rhs, 1);
    return bin->rhs->info->type;
  }
  struct FptrInfo info = { bin->lhs->info->type->e->d.func, bin->rhs->info->type->e->d.func,
      bin->lhs, exp_self(bin)->pos };
  CHECK_BO(fptr_do(env, &info))
  exp_setvar(bin->rhs, 1);
  return bin->rhs->info->type;
}

static OP_CHECK(opck_null_fptr_at) {
  Exp_Binary* bin = (Exp_Binary*)data;
  CHECK_NN(opck_const_rhs(env, bin, mut))
  exp_setvar(bin->rhs, 1);
  return bin->rhs->info->type;
}

static OP_CHECK(opck_fptr_cast) {
  Exp_Cast* cast = (Exp_Cast*)data;
  const Type t = exp_self(cast)->info->type;
  struct FptrInfo info = { cast->exp->info->type->e->d.func, t->e->d.func,
     cast->exp, exp_self(cast)->pos };
  CHECK_BO(fptr_do(env, &info))
  cast->func = cast->exp->info->type->e->d.func;
  return t;
}

static void member_fptr(const Emitter emit) {
  const Instr instr = emit_add_instr(emit, RegPop);
  instr->m_val = SZ_INT;
  const Instr dup = emit_add_instr(emit, Reg2Reg);
  dup->m_val = -SZ_INT;
}

static int is_member(const Type from, const Type to) {
  return GET_FLAG(from->e->d.func, member) &&
    !(GET_FLAG(from, nonnull) || GET_FLAG(to, nonnull));
}

static OP_EMIT(opem_fptr_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  if(exp_self(cast)->info->type->e->d.func->def->base->tmpl)
    fptr_instr(emit, cast->exp->info->type->e->d.func, 1);
  if(is_member(cast->exp->info->type, exp_self(cast)->info->type))
    member_fptr(emit);
  return (Instr)GW_OK;
}

static OP_CHECK(opck_fptr_impl) {
  struct Implicit *impl = (struct Implicit*)data;
  struct FptrInfo info = { impl->e->info->type->e->d.func, impl->t->e->d.func,
      impl->e, impl->e->pos };
  CHECK_BO(fptr_do(env, &info))
  return ((Exp)impl->e)->info->cast_to = impl->t;
}

static OP_EMIT(opem_fptr_impl) {
  struct Implicit *impl = (struct Implicit*)data;
  if(is_member(impl->e->info->type, impl->t))
    member_fptr(emit);
  if(impl->t->e->d.func->def->base->tmpl)
    fptr_instr(emit, ((Exp)impl->e)->info->type->e->d.func, 1);
  return (Instr)GW_OK;
}

ANN Type check_exp_unary_spork(const Env env, const Stmt code);

ANN static void fork_exp(const Env env, const Exp_Unary* unary) {
  const Stmt stmt = new_stmt_exp(env->gwion->mp, ae_stmt_exp, unary->exp);
  const Stmt_List list = new_stmt_list(env->gwion->mp, stmt, NULL);
  const Stmt code = new_stmt_code(env->gwion->mp, list);
  ((Exp_Unary*)unary)->exp = NULL;
  ((Exp_Unary*)unary)->code = code;
}

ANN static Type fork_type(const Env env, const Exp_Unary* unary) {
  const Type t = unary->exp->info->type;
  fork_exp(env, unary);
  if(t == env->gwion->type[et_void])
    return env->gwion->type[et_fork];
  Type_Decl td0 = { .xid=insert_symbol(t->name), .pos=exp_self(unary)->pos };
  struct Type_List_ tl = { .td=&td0 };
  Type_Decl td = { .xid=insert_symbol("TypedFork"), .types=&tl, .pos=exp_self(unary)->pos };
  return known_type(env, &td);
}

static OP_CHECK(opck_spork) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  if(unary->exp && unary->exp->exp_type == ae_exp_call) {
    const m_bool is_spork = unary->op == insert_symbol("spork");
    return is_spork ? env->gwion->type[et_shred] : fork_type(env, unary);
  }
  if(unary->code) {
    ++env->scope->depth;
    nspc_push_value(env->gwion->mp, env->curr);
    const m_bool ret = check_stmt(env, unary->code);
    nspc_pop_value(env->gwion->mp, env->curr);
    --env->scope->depth;
    CHECK_BO(ret)
    return env->gwion->type[unary->op == insert_symbol("spork") ? et_shred : et_fork];
  }
  ERR_O(exp_self(unary)->pos, _("only function calls can be sporked..."))
}

static OP_EMIT(opem_spork) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  return emit_exp_spork(emit, unary);
}

static FREEARG(freearg_xork) {
  REM_REF((VM_Code)instr->m_val, gwion)
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
  GWI_BB(gwi_oper_ini(gwi, "@null", "@func_ptr", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_null_fptr_at))
  GWI_BB(gwi_oper_emi(gwi, opem_func_assign))
  GWI_BB(gwi_oper_end(gwi, "@=>", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_fptr_cast))
  GWI_BB(gwi_oper_emi(gwi, opem_fptr_cast))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_fptr_impl))
  GWI_BB(gwi_oper_emi(gwi, opem_fptr_impl))
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
