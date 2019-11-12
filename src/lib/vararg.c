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

void free_vararg(MemPool p, struct Vararg_* arg) {
  xfree(arg->d);
  xfree(arg->k);
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
  for(m_uint i = 0; i < arg->s; ++i)
     *(m_uint*)(arg->k + i) = vector_at(kinds, i);
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

INSTR(VarargMember) {
  struct Vararg_* arg = *(struct Vararg_**)MEM(instr->m_val);
  if(instr->m_val2 != arg->k[arg->i]) { // TODO: differnciate object and primitives
    free_vararg(shred->info->mp, arg);
    Except(shred, "InvalidVariadicAccess");
  }
  for(m_uint i = 0; i < instr->m_val2; i += SZ_INT)
    *(m_uint*)REG(i) = *(m_uint*)(arg->d + arg->o + i);
  PUSH_REG(shred, instr->m_val2);
}

static OP_CHECK(at_varobj) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  return bin->rhs->type;
}

static INSTR(VarargAssign) {
  POP_REG(shred, SZ_INT);
  *(M_Object**)REG(0) = &*(M_Object*)REG(-SZ_INT);
}

static FREEARG(freearg_vararg) {
  if(instr->m_val2)
    free_vector(((Gwion)gwion)->mp, (Vector)instr->m_val2);
}

static ID_CHECK(idck_vararg) {
  if(env->func && GET_FLAG(env->func->def, variadic))
    return env->gwion->type[et_vararg];
  ERR_O(exp_self(prim)->pos, _("'vararg' must be used inside variadic function"))
}

GWION_IMPORT(vararg) {
  const Type t_varobj  = gwi_mk_type(gwi, "VarObject", SZ_INT, "Object");
  SET_FLAG(t_varobj, abstract);
  const Type t_varloop = gwi_mk_type(gwi, "@VarLoop",  SZ_INT, NULL);
  GWI_BB(gwi_add_type(gwi,  t_varobj))
  GWI_BB(gwi_set_global_type(gwi, t_varloop, et_varloop))
  const Type t_vararg  = gwi_class_ini(gwi, "@Vararg", NULL);
  gwi->gwion->type[et_vararg] = t_vararg; // use func
  GWI_BB(gwi_union_ini(gwi, NULL, NULL))
  GWI_BB(gwi_union_add(gwi, "@VarLoop",  "start"))
  GWI_BB(gwi_union_add(gwi, "@VarLoop",  "end"))
  GWI_BB(gwi_union_add(gwi, "int",       "i"))
  GWI_BB(gwi_union_add(gwi, "float",     "f"))
  GWI_BB(gwi_union_add(gwi, "time",      "t"))
  GWI_BB(gwi_union_add(gwi, "dur",       "d"))
  GWI_BB(gwi_union_add(gwi, "complex",   "c"))
  GWI_BB(gwi_union_add(gwi, "polar",     "p"))
  GWI_BB(gwi_union_add(gwi, "Vec3",      "v3"))
  GWI_BB(gwi_union_add(gwi, "Vec4",      "v4"))
  GWI_BB(gwi_union_add(gwi, "VarObject", "o"))
  GWI_OB(gwi_union_end(gwi, ae_flag_const))
  GWI_BB(gwi_class_end(gwi))
  GWI_BB(gwi_oper_ini(gwi, "VarObject", "Object", NULL))
  GWI_BB(gwi_oper_add(gwi, at_varobj))
  GWI_BB(gwi_oper_end(gwi, "@=>", VarargAssign))
  GWI_BB(gwi_oper_ini(gwi, "Object", "VarObject", NULL))
  GWI_BB(gwi_oper_add(gwi, at_varobj))
  GWI_BB(gwi_oper_end(gwi, "@=>", VarargAssign))
  gwi_register_freearg(gwi, VarargIni, freearg_vararg);
  struct SpecialId_ spid = { .type=t_vararg, .exec=RegPushImm, .is_const=1, .ck=idck_vararg};
  gwi_specialid(gwi, "vararg", &spid);
  return GW_OK;
}
