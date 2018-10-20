#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"
#include "vararg.h"
#include "mpool.h"

INSTR(VarargTop) { GWDEBUG_EXE
  struct Vararg_* arg = *(struct Vararg_**)MEM(instr->m_val);
  if(arg->d)
    PUSH_REG(shred, SZ_INT)
  else {
    shred->pc = instr->m_val2 + 1;
    mp_free(Vararg, arg);
    return;
  }
}

INSTR(VarargIni) { GWDEBUG_EXE
  struct Vararg_* arg = mp_alloc(Vararg);
  if(instr->m_val) {
    POP_REG(shred,  instr->m_val)
    arg->d = (m_bit*)xmalloc(instr->m_val);
    memcpy(arg->d, shred->reg, instr->m_val);
  } else {
    if(*(m_uint*)instr->ptr)
      POP_REG(shred, SZ_INT)
    arg->d = NULL;
  }
  const Vector kinds = (Vector)instr->m_val2;
  if(kinds) {
    arg->s = vector_size(kinds);
    if(arg->s) {
      arg->k = (m_uint*)xcalloc(arg->s, SZ_INT);
      memcpy(arg->k, kinds->ptr + OFFSET, arg->s * SZ_INT);
    }
  } else {
    arg->s = 0;
    arg->k = NULL;
  }
  arg->o = 0;
  arg->i = 0;
  *(struct Vararg_**)REG(0) = arg;
  PUSH_REG(shred,  SZ_INT);
}

INSTR(VarargEnd) { GWDEBUG_EXE
  struct Vararg_* arg = *(struct Vararg_**)MEM(instr->m_val);
  PUSH_REG(shred, SZ_INT);
  arg->o += arg->k[arg->i];
  if(++arg->i < arg->s)
    shred->pc = instr->m_val2;
  else {
    free(arg->d);
    free(arg->k);
    mp_free(Vararg, arg);
  }
}

INSTR(VarargMember) { GWDEBUG_EXE
  const struct Vararg_* arg = *(struct Vararg_**)MEM(instr->m_val);
  memcpy(REG(0), (arg->d + arg->o), instr->m_val2);
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

GWION_IMPORT(vararg) {
  CHECK_OB((t_vararg  = gwi_mk_type(gwi, "@Vararg", SZ_INT, t_object)))
  const Type t_varobj  = gwi_mk_type(gwi, "VarObject", SZ_INT, t_vararg);
  CHECK_OB(t_varobj)
  SET_FLAG(t_varobj, ae_flag_abstract);
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
  return 1;
}
