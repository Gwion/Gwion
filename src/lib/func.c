#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "gwion.h"
#include "type.h"
#include "value.h"
#include "func.h"
#include "instr.h"
#include "emit.h"
#include "object.h"
#include "import.h"
#include "nspc.h"
#include "operator.h"
#include "traverse.h"
#include "parse.h"

ANN Type check_exp_call1(const Env env, const Exp_Call *exp);
ANN m_bool emit_exp_spork(const Emitter, const Exp_Unary*);

static INSTR(LambdaAssign) {
  POP_REG(shred, SZ_INT)
  *(Func*)REG(-SZ_INT) = *(Func*)REG(0);
}

static OP_CHECK(opck_func_call) {
  Exp_Binary* bin = (Exp_Binary*)data;
  if(bin->rhs->exp_type == ae_exp_decl)
    ERR_N(bin->rhs->pos, "calling fptr decl, this is forbidden.")
  Exp_Call call = { .func=bin->rhs, .args=bin->lhs };
  Exp e = exp_self(bin);
  e->exp_type = ae_exp_call;
  memcpy(&e->d.exp_call, &call, sizeof(Exp_Call));
  return check_exp_call1(env, &e->d.exp_call) ?: t_null;
}

static OP_EMIT(opem_func_assign) {
  Exp_Binary* bin = (Exp_Binary*)data;
  emit_add_instr(emit, int_r_assign);
  if((bin->lhs->type != t_lambda && isa(bin->lhs->type, t_fptr) < 0) && GET_FLAG(bin->rhs->type->d.func, member)) {
    const Instr instr = emit_add_instr(emit, LambdaAssign);
    instr->m_val = SZ_INT;
  }
  return GW_OK;
}

ANN static Type fptr_type(const Env env, Exp_Binary* bin) {
  const Func l_func = bin->lhs->type->d.func;
  const Func r_func = bin->rhs->type->d.func;
  const Nspc nspc = l_func->value_ref->owner;
  const m_str c = s_name(l_func->def->base->xid);
  const Value v = l_func->value_ref;
  for(m_uint i = 0; i <= v->offset; ++i) {
    const Symbol sym = func_symbol(env, nspc->name, c, NULL, i);
    const Func f = nspc_lookup_func1(nspc, sym); // was lookup2
    CHECK_OO(f)
    if(compat_func(r_func->def, f->def) > 0)
      return r_func->value_ref->type->d.base_type;
  }
  return NULL;
}

ANN2(1,3,4) m_bool check_lambda(const Env env, const Type owner,
    Exp_Lambda *l, const Func_Def def) {
  const m_uint scope = ((l->owner = owner)) ?
    env_push_type(env, owner) : env->scope->depth;
  Arg_List base = def->base->args, arg = l->args;
  while(base && arg) {
    arg->td = base->td;
    base = base->next;
    arg = arg->next;
  }
  if(base || arg)
    ERR_B(exp_self(l)->pos, "argument number does not match for lambda")
  l->def = new_func_def(env->gwion->mp, new_func_base(env->gwion->mp, def->base->td, l->name, l->args), l->code, def->flag,
    loc_cpy(env->gwion->mp, def->pos));
  const m_bool ret = traverse_func_def(env, l->def);
  arg = l->args;
  while(arg) {
    arg->td = NULL;
    arg = arg->next;
  }
  if(owner)
    env_pop(env, scope);
  return ret;
}

static OP_CHECK(opck_fptr_at) {
  Exp_Binary* bin = (Exp_Binary*)data;
  bin->rhs->emit_var = 1;
  if(isa(bin->lhs->type, t_lambda) > 0) {
    Exp_Lambda *l = &bin->lhs->d.exp_lambda;
    const Type owner = nspc_lookup_type1(bin->rhs->type->owner->parent,
       insert_symbol(bin->rhs->type->owner->name));
    CHECK_BO(check_lambda(env, owner, l, bin->rhs->type->d.func->def))
    return bin->rhs->type;
  }
  const Func l_func = bin->lhs->type->d.func;
  const Func_Def l_fdef = l_func->def;
  const Type l_type = l_func->value_ref->owner_class;
  const Func r_func = bin->rhs->type->d.func;
  const Func_Def r_fdef = r_func->def;
  const Type r_type = r_func->value_ref->owner_class;
  if(!r_type && l_type)
    ERR_N(exp_self(bin)->pos, "can't assign member function to non member function pointer")
  else if(r_type && !l_type) {
    if(!GET_FLAG(r_func, global))
      ERR_N(exp_self(bin)->pos, "can't assign non member function to member function pointer")
  } else if(r_type && isa(r_type, l_type) < 0)
      ERR_N(exp_self(bin)->pos, "can't assign member function to member function pointer"
            " of an other class")
  if(GET_FLAG(r_func, member)) {
    if(!GET_FLAG(l_func, member))
      ERR_N(exp_self(bin)->pos, "can't assign static function to member function pointer")
  } else if(GET_FLAG(l_func, member))
      ERR_N(exp_self(bin)->pos, "can't assign member function to static function pointer")
  if(isa(r_fdef->base->ret_type, l_fdef->base->ret_type) < 0)
    ERR_N(exp_self(bin)->pos, "return type '%s' does not match '%s'\n\t... in pointer assignement",
         r_fdef->base->ret_type->name, l_fdef->base->ret_type->name)
  if(GET_FLAG(l_fdef, variadic) != GET_FLAG(r_fdef, variadic))
    ERR_N(exp_self(bin)->pos, "function must be of same argument kind.",
         r_fdef->base->ret_type->name, l_fdef->base->ret_type->name)
  if(isa(bin->lhs->type, t_fptr) > 0 && isa(bin->lhs->type, bin->rhs->type) > 0)
    return bin->rhs->type;
  return fptr_type(env, bin);
}

static OP_CHECK(opck_fptr_cast) {
  Exp_Cast* cast = (Exp_Cast*)data;
  const Type t = exp_self(cast)->type;
  const Value v = nspc_lookup_value1(env->curr, cast->exp->d.exp_primary.d.var);
  CHECK_OO(v)
  const Func f = isa(v->type, t_fptr) > 0 ?
            v->type->d.func :
            nspc_lookup_func1(env->curr, insert_symbol(v->name));
  CHECK_OO(f)
  CHECK_BO(compat_func(t->d.func->def, f->def))
  cast->func = f;
  return t;
}

static OP_EMIT(opem_fptr_cast) {
  CHECK_BB(opem_basic_cast(emit, data))
  Exp_Cast* cast = (Exp_Cast*)data;
  if(GET_FLAG(cast->exp->type->d.func, member)) {
    const Instr instr = emit_add_instr(emit, RegPop);
    instr->m_val = SZ_INT*2;
    const Instr dup = emit_add_instr(emit, Reg2Reg);
    dup->m_val2 = SZ_INT;
  }
  return GW_OK;
}

ANN Type check_exp_unary_spork(const Env env, const Stmt code);

static OP_CHECK(opck_spork) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  if(unary->op == op_fork && !unary->fork_ok)
    ERR_O(exp_self(unary)->pos, "forks must be stored in a value:\n"
        "fork xxx @=> Fork f")
  if(unary->exp && unary->exp->exp_type == ae_exp_call)
    return unary->op == op_spork ? t_shred : t_fork;
  else if(unary->code) {
    ++env->scope->depth;        \
    nspc_push_value(env->gwion->mp, env->curr); \
    const m_bool ret = check_stmt(env, unary->code);
    nspc_pop_value(env->gwion->mp, env->curr);  \
    --env->scope->depth;
    CHECK_BO(ret)
    return unary->op == op_spork ? t_shred : t_fork;
  } else
    ERR_O(exp_self(unary)->pos, "only function calls can be sporked...")
  return NULL;
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
  free_type_list(((Gwion)gwion)->mp, dt->tl);
  mp_free(((Gwion)gwion)->mp, dottmpl, dt);
}

GWION_IMPORT(func) {
  CHECK_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "@function", NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_func_call))
  gwi_oper_mut(gwi, 1);
  CHECK_BB(gwi_oper_end(gwi, op_chuck, NULL))
  CHECK_BB(gwi_oper_ini(gwi, "@function", "@func_ptr", NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_fptr_at))
  CHECK_BB(gwi_oper_emi(gwi, opem_func_assign))
  CHECK_BB(gwi_oper_end(gwi, op_ref, NULL /*FuncAssign*/))
  CHECK_BB(gwi_oper_add(gwi, opck_fptr_cast))
  CHECK_BB(gwi_oper_emi(gwi, opem_fptr_cast))
  CHECK_BB(gwi_oper_end(gwi, op_cast, NULL))
  CHECK_BB(gwi_oper_ini(gwi, NULL, (m_str)OP_ANY_TYPE, NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_spork))
  CHECK_BB(gwi_oper_emi(gwi, opem_spork))
  CHECK_BB(gwi_oper_end(gwi, op_spork, NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_spork))
  CHECK_BB(gwi_oper_emi(gwi, opem_spork))
  CHECK_BB(gwi_oper_end(gwi, op_fork, NULL))
  register_freearg(gwi, SporkIni, freearg_xork);
  register_freearg(gwi, ForkIni, freearg_xork);
  register_freearg(gwi, DotTmpl, freearg_dottmpl);
  return GW_OK;
}
