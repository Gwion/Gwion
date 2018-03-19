#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"
#include "vararg.h"
#include "mpool.h"

POOL_HANDLE(Vararg, 4)

INSTR(Vararg_start) { GWDEBUG_EXE
  struct Vararg_* arg = *(struct Vararg_**)MEM(instr->m_val);
  if(!arg->d)
    shred->next_pc = instr->m_val2 + 1;
  if(!arg->s) {
    if(arg->k)
      POP_REG(shred, SZ_INT); // pop vararg
    mp_free(Vararg, arg);
    return;
  }
  PUSH_REG(shred, SZ_INT);
  *(m_uint*)REG(- SZ_INT) = 0;
}

INSTR(MkVararg) { GWDEBUG_EXE
  POP_REG(shred,  instr->m_val);
  m_uint i;
  Vector kinds = (Vector)instr->m_val2;
  struct Vararg_* arg = mp_alloc(Vararg);
  if(instr->m_val) {
    arg->d = xmalloc(instr->m_val);
    memcpy(arg->d, shred->reg, instr->m_val);
  }  else arg->d = NULL;
  arg->s = kinds ? vector_size(kinds) : 0;
  arg->k = arg->s ? xcalloc(arg->s, SZ_INT) : NULL;
  for(i = 0; i < arg->s; i++) {
    arg->k[i] = vector_at(kinds, i);
  }
  arg->o = 0;
  arg->i = 0;
  if(kinds)
    free_vector(kinds);
  *(struct Vararg_**)REG(0) = arg;
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Vararg_end) { GWDEBUG_EXE
  struct Vararg_* arg = *(struct Vararg_**)MEM(instr->m_val);
  arg->o += arg->k[arg->i];
  PUSH_REG(shred, SZ_INT);
  arg->i++;
  if(arg->i == arg->s) {
    free(arg->d);
    free(arg->k);
    mp_free(Vararg, arg);
  } else {
    *(m_uint*)REG(-SZ_INT) = 0;
    shred->next_pc = instr->m_val2;
  }
}

INSTR(Vararg_Member) { GWDEBUG_EXE
  struct Vararg_* arg = *(struct Vararg_**)MEM(instr->m_val);
  memcpy(REG(0), (arg->d + arg->o), instr->m_val2);
  PUSH_REG(shred, instr->m_val2);
}

static OP_CHECK(at_varobj) {
  Exp_Binary* bin = (Exp_Binary*)data;
  return bin->rhs->type;
}

INSTR(varobj_assign) { GWDEBUG_EXE
  POP_REG(shred, 2 * SZ_INT);
  *(M_Object**)REG(SZ_INT) = &*(M_Object*)REG(0);
  PUSH_REG(shred, SZ_INT);
}

m_bool import_vararg(Gwi gwi) {
  Type t_varobj, t_varloop;
  CHECK_OB((t_vararg  = gwi_mk_type(gwi, "@Vararg", SZ_INT, t_object)))
  CHECK_OB((t_varobj  = gwi_mk_type(gwi, "VarObject", SZ_INT, t_vararg)))
  CHECK_OB((t_varloop = gwi_mk_type(gwi, "@VarLoop",  SZ_INT, NULL)))
  SET_FLAG(t_varobj, ae_flag_abstract);
  CHECK_BB(gwi_add_type(gwi,  t_varobj))
  CHECK_BB(gwi_add_type(gwi,  t_varloop))
  CHECK_BB(gwi_class_ini(gwi, t_vararg, NULL, NULL))
  CHECK_BB(gwi_item_ini(gwi, "@VarLoop",  "start"))
  CHECK_BB(gwi_item_end(gwi, ae_flag_const, NULL))
  CHECK_BB(gwi_item_ini(gwi, "@VarLoop",  "end"))
  CHECK_BB(gwi_item_end(gwi, ae_flag_const, NULL))
  CHECK_BB(gwi_item_ini(gwi, "int",       "i"))
  CHECK_BB(gwi_item_end(gwi, ae_flag_const, NULL))
  CHECK_BB(gwi_item_ini(gwi, "float",     "f"))
  CHECK_BB(gwi_item_end(gwi,     ae_flag_const, NULL))
  CHECK_BB(gwi_item_ini(gwi, "time",      "t"))
  CHECK_BB(gwi_item_end(gwi,     ae_flag_const, NULL))
  CHECK_BB(gwi_item_ini(gwi, "dur",       "d"))
  CHECK_BB(gwi_item_end(gwi,     ae_flag_const, NULL))
  CHECK_BB(gwi_item_ini(gwi, "complex",   "c"))
  CHECK_BB(gwi_item_end(gwi,     ae_flag_const, NULL))
  CHECK_BB(gwi_item_ini(gwi, "polar",     "p"))
  CHECK_BB(gwi_item_end(gwi,     ae_flag_const, NULL))
  CHECK_BB(gwi_item_ini(gwi, "Vec3",      "v3"))
  CHECK_BB(gwi_item_end(gwi,    ae_flag_const, NULL))
  CHECK_BB(gwi_item_ini(gwi, "Vec4",      "v4"))
  CHECK_BB(gwi_item_end(gwi,    ae_flag_const, NULL))
  CHECK_BB(gwi_item_ini(gwi, "VarObject", "o"))
  CHECK_BB(gwi_item_end(gwi,     ae_flag_const | ae_flag_ref, NULL))
  CHECK_BB(gwi_class_end(gwi))
  CHECK_BB(gwi_oper_ini(gwi, "VarObject", "Object", NULL))
  CHECK_BB(gwi_oper_add(gwi, at_varobj))
  CHECK_BB(gwi_oper_end(gwi, op_at_chuck, varobj_assign))
  CHECK_BB(gwi_oper_ini(gwi, "Object", "VarObject", NULL))
  CHECK_BB(gwi_oper_add(gwi, at_varobj))
  CHECK_BB(gwi_oper_end(gwi, op_at_chuck, varobj_assign))
  return 1;
}
