#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "object.h"
#include "vararg.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "specialid.h"
#include "traverse.h"
#include "parse.h"
#include "gack.h"

void free_vararg(MemPool p, struct Vararg_* arg) {
  xfree(arg->d);
  xfree(arg->k);
  xfree(arg->t);
  mp_free(p, Vararg, arg);
}

INSTR(VarargTop) {
  struct Vararg_* arg = *(struct Vararg_**)MEM(instr->m_val);
  if(arg) {
    assert(arg->d);
    PUSH_REG(shred, SZ_INT)
  } else
    shred->pc = instr->m_val2 + 1;
}

INSTR(VarargIni) {
  struct Vararg_* arg = mp_calloc(shred->info->mp, Vararg);
  POP_REG(shred, instr->m_val - SZ_INT)
  arg->d = (m_bit*)xmalloc(round2szint(instr->m_val));
  for(m_uint i = 0; i < instr->m_val; i += SZ_INT)
    *(m_uint*)(arg->d + i) = *(m_uint*)(shred->reg - SZ_INT + i);
  const Vector kinds = (Vector)instr->m_val2;
  arg->s = vector_size(kinds);
  arg->k = (m_uint*)xmalloc(arg->s * SZ_INT);
  arg->t = (Type*)xmalloc(arg->s * SZ_INT);
  for(m_uint i = 0; i < arg->s; ++i) {
    const Type t = (Type)vector_at(kinds, i);
    *(Type*)(arg->t + i) = t;
    *(m_uint*)(arg->k + i) = t->size;
  }
  *(struct Vararg_**)REG(-SZ_INT) = arg;
}

INSTR(VarargEnd) {
  struct Vararg_* arg = *(struct Vararg_**)MEM(instr->m_val);
  PUSH_REG(shred, SZ_INT);
  arg->o += arg->k[arg->i];
  if(++arg->i < arg->s)
    shred->pc = instr->m_val2;
  else
    free_vararg(shred->info->mp, arg);
}

static OP_CHECK(opck_vararg_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  return known_type(env, cast->td);
}

static INSTR(VarargCast) {
  struct Vararg_* arg = *(struct Vararg_**)MEM(instr->m_val);
  const Type t = (Type)instr->m_val2;
  if(isa(arg->t[arg->i], t) < 0){
	  free_vararg(shred->info->mp, arg);
	  Except(shred, "InvalidVariadicAccess");
  }
  for(m_uint i = 0; i < t->size; i += SZ_INT)
    *(m_uint*)REG(i - SZ_INT) = *(m_uint*)(arg->d + arg->o + i);
  PUSH_REG(shred, t->size - SZ_INT);
}

ANN static inline Instr get_variadic(const Emitter emit) {
  return (Instr)vector_back(&emit->info->variadic);
}

ANN m_bool variadic_check(const Emitter emit, const loc_t pos) {
  const Env env = emit->env;
  if(!get_variadic(emit))
      ERR_B(pos, _("vararg.xxx used before vararg.start"))
  if(GET_FLAG(emit->env->func, empty))
    ERR_B(pos, _("vararg.xxx used after vararg.end"))
  return GW_OK;
}

static OP_EMIT(opem_vararg_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  CHECK_BO(variadic_check(emit, cast->exp->pos))
  const Instr instr = emit_add_instr(emit, VarargCast);
  instr->m_val = get_variadic(emit)->m_val;
  instr->m_val2 = (m_uint)exp_self(cast)->type;
  return instr;
}

static FREEARG(freearg_vararg) {
  if(instr->m_val2)
    free_vector(((Gwion)gwion)->mp, (Vector)instr->m_val2);
}

static ID_CHECK(idck_vararg) {
  if(env->func && GET_FLAG(env->func->def, variadic))
    return exp_self(prim)->type;
  ERR_O(exp_self(prim)->pos, _("'vararg' must be used inside variadic function"))
}

static GACK(gack_vararg) {
  INTERP_PRINTF("%p\n", *(M_Object*)VALUE);
}

ANN static inline m_uint emit_code_size(const Emitter emit) {
  return vector_size(&emit->code->instr);
}
ANN static m_bool emit_vararg_start(const Emitter emit, const m_uint offset) {
  const Instr instr = emit_add_instr(emit, VarargTop);
  instr->m_val = offset;
  instr->m_val2 = emit_code_size(emit);
  vector_set(&emit->info->variadic, vector_size(&emit->info->variadic) -1, (vtype)instr);
  return GW_OK;
}

ANN static void emit_vararg_end(const Emitter emit, const m_uint offset) {
  const Instr instr = emit_add_instr(emit, VarargEnd),
    variadic = get_variadic(emit);
  instr->m_val = offset;
  instr->m_val2 = variadic->m_val2;
  variadic->m_val2 = emit_code_size(emit);
  SET_FLAG(emit->env->func, empty);// mark vararg func as complete
}

static OP_EMIT(opem_vararg_dot) {
  const Env env = emit->env;
  const Exp_Dot *member = (Exp_Dot*)data;
  m_uint offset = emit->env->class_def ? SZ_INT : 0;
  Arg_List l = emit->env->func->def->base->args;
  const m_str str = s_name(member->xid);
  while(l) {
    offset += l->type->size;
    l = l->next;
  }
  if(!strcmp(str, "start")) {
    if(get_variadic(emit))
      ERR_O(exp_self(member)->pos, _("vararg.start already used"))
    emit_vararg_start(emit, offset);
    return (Instr)GW_OK;
  }
  assert(!strcmp(str, "end"));
  if(!get_variadic(emit))
    ERR_O(exp_self(member)->pos, _("vararg.start not used before vararg.end"))
  emit_vararg_end(emit, offset);
  return (Instr)GW_OK;
}
OP_CHECK(opck_object_dot);

GWION_IMPORT(vararg) {
  const Type t_varloop = gwi_mk_type(gwi, "@VarLoop",  SZ_INT, NULL);
  GWI_BB(gwi_set_global_type(gwi, t_varloop, et_varloop))
  const Type t_vararg  = gwi_class_spe(gwi, "@Vararg", 0);
  gwi_gack(gwi, t_vararg, gack_vararg);
  GWI_BB(gwi_union_ini(gwi, NULL, NULL))
  GWI_BB(gwi_union_add(gwi, "@VarLoop",  "start"))
  GWI_BB(gwi_union_add(gwi, "@VarLoop",  "end"))
  GWI_OB(gwi_union_end(gwi, ae_flag_const))
  GWI_BB(gwi_class_end(gwi))
  GWI_BB(gwi_oper_ini(gwi, "@Vararg", (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_object_dot))
  GWI_BB(gwi_oper_emi(gwi, opem_vararg_dot))
  GWI_BB(gwi_oper_end(gwi, "@dot", NULL))
  GWI_BB(gwi_oper_ini(gwi, "@Vararg", (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_vararg_cast))
  GWI_BB(gwi_oper_emi(gwi, opem_vararg_cast))
  GWI_BB(gwi_oper_end(gwi, "$", VarargCast))
  gwi_register_freearg(gwi, VarargIni, freearg_vararg);
  struct SpecialId_ spid = { .type=t_vararg, .exec=RegPushImm, .is_const=1, .ck=idck_vararg};
  gwi_specialid(gwi, "vararg", &spid);
  return GW_OK;
}
