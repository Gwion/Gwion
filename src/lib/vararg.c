#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "import.h"
#include "vararg.h"
#include "gwion.h"

void free_vararg(MemPool p, struct Vararg_* arg) {
  xfree(arg->d);
  xfree(arg->k);
  mp_free(p, Vararg, arg);
}

INSTR(VarargTop) { GWDEBUG_EXE
  struct Vararg_* arg = *(struct Vararg_**)MEM(instr->m_val);
  if(arg) {
    if(arg->d)
      PUSH_REG(shred, SZ_INT)
    else {
      shred->pc = instr->m_val2 + 1;
      mp_free(shred->info->mp, Vararg, arg);
    }
  } else
    shred->pc = instr->m_val2 + 1;
}

INSTR(VarargIni) { GWDEBUG_EXE
  struct Vararg_* arg = mp_alloc(shred->info->mp, Vararg);
  POP_REG(shred,  instr->m_val - SZ_INT)
  arg->d = (m_bit*)xmalloc(instr->m_val);
  for(m_uint i = 0; i < instr->m_val; i += SZ_INT)
    *(m_uint*)(arg->d + i) = *(m_uint*)(shred->reg - SZ_INT + i);
  const Vector kinds = (Vector)instr->m_val2;
  arg->s = vector_size(kinds);
  arg->k = (m_uint*)xmalloc(arg->s * SZ_INT);
  for(m_uint i = 0; i < arg->s; ++i)
     *(m_uint*)(arg->k + i) = vector_at(kinds, i);
  *(struct Vararg_**)REG(-SZ_INT) = arg;
}

INSTR(VarargEnd) { GWDEBUG_EXE
  struct Vararg_* arg = *(struct Vararg_**)MEM(instr->m_val);
  PUSH_REG(shred, SZ_INT);
  arg->o += arg->k[arg->i];
  if(++arg->i < arg->s)
    shred->pc = instr->m_val2;
  else
    free_vararg(shred->info->mp, arg);
}

INSTR(VarargMember) { GWDEBUG_EXE
  const struct Vararg_* arg = *(struct Vararg_**)MEM(instr->m_val);
  for(m_uint i = 0; i < instr->m_val2; i += SZ_INT)
    *(m_uint*)REG(i) = *(m_uint*)(arg->d + arg->o + i);
  PUSH_REG(shred, instr->m_val2);
}

static OP_CHECK(at_varobj) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  return bin->rhs->type;
}

static INSTR(VarargAssign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  *(M_Object**)REG(0) = &*(M_Object*)REG(-SZ_INT);
}

static FREEARG(freearg_vararg) {
  if(instr->m_val2)
    free_vector(((Gwion)gwion)->p, (Vector)instr->m_val2);
}

GWION_IMPORT(vararg) {
  CHECK_OB((t_vararg  = gwi_mk_type(gwi, "@Vararg", SZ_INT, t_object)))
  const Type t_varobj  = gwi_mk_type(gwi, "VarObject", SZ_INT, t_vararg);
  CHECK_OB(t_varobj)
  SET_FLAG(t_varobj, abstract);
  const Type t_varloop = gwi_mk_type(gwi, "@VarLoop",  SZ_INT, NULL);
  CHECK_OB(t_varloop)
  CHECK_BB(gwi_add_type(gwi,  t_varobj))
  CHECK_BB(gwi_add_type(gwi,  t_varloop))
  CHECK_BB(gwi_class_ini(gwi, t_vararg, NULL, NULL))
  CHECK_BB(gwi_union_ini(gwi, NULL))
  CHECK_BB(gwi_union_add(gwi, "@VarLoop",  "start"))
  CHECK_BB(gwi_union_add(gwi, "@VarLoop",  "end"))
  CHECK_BB(gwi_union_add(gwi, "int",       "i"))
  CHECK_BB(gwi_union_add(gwi, "float",     "f"))
  CHECK_BB(gwi_union_add(gwi, "time",      "t"))
  CHECK_BB(gwi_union_add(gwi, "dur",       "d"))
  CHECK_BB(gwi_union_add(gwi, "complex",   "c"))
  CHECK_BB(gwi_union_add(gwi, "polar",     "p"))
  CHECK_BB(gwi_union_add(gwi, "Vec3",      "v3"))
  CHECK_BB(gwi_union_add(gwi, "Vec4",      "v4"))
  CHECK_BB(gwi_union_add(gwi, "VarObject", "o"))
  CHECK_BB(gwi_union_end(gwi, ae_flag_const))
  CHECK_BB(gwi_class_end(gwi))
  CHECK_BB(gwi_oper_ini(gwi, "VarObject", "Object", NULL))
  CHECK_BB(gwi_oper_add(gwi, at_varobj))
  CHECK_BB(gwi_oper_end(gwi, op_ref, VarargAssign))
  CHECK_BB(gwi_oper_ini(gwi, "Object", "VarObject", NULL))
  CHECK_BB(gwi_oper_add(gwi, at_varobj))
  CHECK_BB(gwi_oper_end(gwi, op_ref, VarargAssign))
  register_freearg(gwi, VarargIni, freearg_vararg);
  return GW_OK;
}
