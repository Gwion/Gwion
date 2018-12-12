#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "value.h"
#include "func.h"
#include "instr.h"
#include "emit.h"
#include "object.h"
#include "import.h"
#include "nspc.h"
#include "operator.h"

ANN Type check_exp_call1(const Env, const Exp restrict,const Exp, const Exp);
ANN m_bool emit_exp_spork(const Emitter, const Exp_Unary*);

static INSTR(assign_func) { GWDEBUG_EXE
  const Func f = **(Func**)REG(-SZ_INT) = *(Func*)REG(-(SZ_INT*2+instr->m_val2));
  POP_REG(shred, instr->m_val + instr->m_val2)
  *(Func*)REG(-SZ_INT) = f; // do we need this ?
}

static OP_CHECK(opck_func_call) {
  Exp_Binary* bin = (Exp_Binary*)data;
  return check_exp_call1(env, bin->rhs, bin->lhs, bin->self);
}

ANN static Type fptr_type(Exp_Binary* bin) {
  const Func l_func = bin->lhs->type->d.func;
  const Func r_func = bin->rhs->type->d.func;
  const Nspc nspc = l_func->value_ref->owner;
  const m_str c = s_name(l_func->def->name);
  const Value v = l_func->value_ref;
  for(m_uint i = 0; i <= v->offset; ++i) {
    const Symbol sym = func_symbol(nspc, c, NULL, i);
    const Func f = nspc_lookup_func1(nspc, sym); // was lookup2
    CHECK_OO(f)
    if(compat_func(r_func->def, f->def) > 0) {
      bin->func = f;
      return r_func->value_ref->type->d.base_type;
    }
  }
  return NULL;
}

static OP_CHECK(opck_fptr_at) {
  Exp_Binary* bin = (Exp_Binary*)data;
  const Func l_func = bin->lhs->type->d.func;
  const Func_Def l_fdef = l_func->def;
  const Type l_type = l_func->value_ref->owner_class;
  const Func r_func = bin->rhs->type->d.func;
  const Func_Def r_fdef = r_func->def;
  const Type r_type = r_func->value_ref->owner_class;
  bin->rhs->emit_var = 1;
  if(!r_type && l_type)
    ERR_N(bin->self->pos, "can't assign member function to non member function pointer")
  else if(r_type && !l_type) {
    if(!GET_FLAG(r_func, global))
      ERR_N(bin->self->pos, "can't assign non member function to member function pointer")
  } else if(r_type && isa(r_type, l_type) < 0)
      ERR_N(bin->self->pos, "can't assign member function to member function pointer"
            " of an other class")
  if(GET_FLAG(r_func, member)) {
    if(!GET_FLAG(l_func, member))
      ERR_N(bin->self->pos, "can't assign static function to member function pointer")
  } else if(GET_FLAG(l_func, member))
      ERR_N(bin->self->pos, "can't assign member function to static function pointer")
  if(isa(r_fdef->ret_type, l_fdef->ret_type) < 0)
    ERR_N(bin->self->pos, "return type '%s' does not match '%s'\n\t... in pointer assignement",
         r_fdef->ret_type->name, l_fdef->ret_type->name)
  if(GET_FLAG(l_fdef, variadic) != GET_FLAG(r_fdef, variadic))
    ERR_N(bin->self->pos, "function must be of same argument kind.",
         r_fdef->ret_type->name, l_fdef->ret_type->name)
  if(isa(bin->lhs->type, t_fptr) > 0 && isa(bin->lhs->type, bin->rhs->type) > 0)
    return bin->rhs->type;
  return fptr_type(bin);
}

static OP_CHECK(opck_fptr_cast) {
  Exp_Cast* cast = (Exp_Cast*)data;
  const Type t = cast->self->type;
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

ANN Type check_exp_unary_spork(const Env env, const Stmt code);

static OP_CHECK(opck_spork) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  if(unary->exp && unary->exp->exp_type == ae_exp_call)
    return t_shred;
  else if(unary->code)
    return check_exp_unary_spork(env, unary->code);
  else
    ERR_O(unary->self->pos, "only function calls can be sporked...")
  return NULL;
}
static OP_EMIT(opem_fptr_at) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  const Instr instr = emit_add_instr(emit, assign_func);
  if(GET_FLAG(bin->rhs->type->d.func, global))
    instr->m_val = SZ_INT;
  else if(GET_FLAG(bin->rhs->type->d.func, member)) {
    if(bin->rhs->exp_type != ae_exp_decl)
      instr->m_val2 = SZ_INT;
    instr->m_val = SZ_INT*2;
  } else
    instr->m_val = SZ_INT;
  return GW_OK;
}

static OP_EMIT(opem_spork) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  return emit_exp_spork(emit, unary);
}

GWION_IMPORT(func) {
  CHECK_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "@function", NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_func_call))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, NULL))
  CHECK_BB(gwi_oper_ini(gwi, "@function", "@func_ptr", NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_fptr_at))
  CHECK_BB(gwi_oper_emi(gwi, opem_fptr_at))
  CHECK_BB(gwi_oper_end(gwi, op_ref, NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_fptr_cast))
  CHECK_BB(gwi_oper_emi(gwi, opem_basic_cast))
  CHECK_BB(gwi_oper_end(gwi, op_cast, NULL))
  CHECK_BB(gwi_oper_ini(gwi, NULL, (m_str)OP_ANY_TYPE, NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_spork))
  CHECK_BB(gwi_oper_emi(gwi, opem_spork))
  return (m_bool)gwi_oper_end(gwi, op_spork, NULL);
}
