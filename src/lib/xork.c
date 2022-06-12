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
#include "parse.h"

ANN static Type fork_type(const Env env, const Exp_Unary *unary) {
  const Type t = unary->exp->type;
  if (t == env->gwion->type[et_void]) return env->gwion->type[et_fork];
  char c[21 + strlen(t->name)];
  sprintf(c, "TypedFork:[%s]", t->name);
  const Type fork = env->gwion->type[et_fork];
  UNSET_FLAG(fork, final);
  const Type typed = str2type(env->gwion, "TypedFork", exp_self(unary)->pos);
  if (typed->nspc->offset == fork->nspc->offset)
    typed->nspc->offset += t->size;
  UNSET_FLAG(typed, final);
  const Type ret = str2type(env->gwion, c, exp_self(unary)->pos);
  SET_FLAG(typed, final);
  SET_FLAG(fork, final);
  return ret;
}

ANN Type upvalue_type(const Env env, Capture *cap);
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
    struct Value_ value = { .type = env->gwion->type[et_function]};
    if(env->class_def)
      set_vflag(&value, vflag_member);
    struct Func_Base_ fbase = { .xid=insert_symbol("in spork"), .values = &upvalues, .fbflag = fbflag_lambda, .pos = exp_self(unary)->pos};
    struct Func_Def_ fdef = { .base = &fbase};
    struct Func_ func = { .name = "in spork", .def = &fdef, .value_ref = &value};
    env->func = &func;
    const m_bool ret = check_stmt(env, unary->code);
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

static FREEARG(freearg_xork) { vmcode_remref((VM_Code)instr->m_val, gwion); }

GWION_IMPORT(xork) {
  GWI_BB(gwi_oper_ini(gwi, NULL, (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_spork))
  GWI_BB(gwi_oper_emi(gwi, opem_spork))
  GWI_BB(gwi_oper_end(gwi, "spork", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_spork))
  GWI_BB(gwi_oper_emi(gwi, opem_spork))
  GWI_BB(gwi_oper_end(gwi, "fork", NULL))
  gwi_register_freearg(gwi, SporkIni, freearg_xork);
  return GW_OK;
}
