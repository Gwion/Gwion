#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"
#include "vararg.h"

struct Type_ t_vararg  = { "@Vararg",   SZ_INT, &t_object, te_vararg};
struct Type_ t_varobj  = { "VarObject", SZ_INT, &t_object, te_vararg};
struct Type_ t_varloop = { "@VarLoop",  SZ_INT, NULL,      te_vararg_loop};

INSTR(Vararg_start) {
  struct Vararg* arg = *(struct Vararg**)MEM(instr->m_val);
  if(!arg->d)
    shred->next_pc = instr->m_val2 + 1;
  if(!arg->s) {
    if(arg->k)
      POP_REG(shred, SZ_INT); // pop vararg
    free(arg);
    return;
  }
  PUSH_REG(shred, SZ_INT);
  *(m_uint*)REG(- SZ_INT) = 0;
}

INSTR(MkVararg) {
  POP_REG(shred,  instr->m_val);
  m_uint i;
  Vector kinds = (Vector)instr->m_val2;
  struct Vararg* arg = calloc(1, sizeof(struct Vararg));
  if(instr->m_val) {
    arg->d = malloc(instr->m_val);
    memcpy(arg->d, shred->reg, instr->m_val);
  }  else arg->d = NULL;
  arg->s = kinds ? vector_size(kinds) : 0;
  arg->k = arg->s ? calloc(arg->s, sizeof(m_uint)) : NULL;
  for(i = 0; i < arg->s; i++) {
    arg->k[i] = vector_at(kinds, i);
  }
  arg->o = 0;
  arg->i = 0;
  if(kinds)
    free_vector(kinds);
  *(struct Vararg**)REG(0) = arg;
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Vararg_end) {
  struct Vararg* arg = *(struct Vararg**)MEM(instr->m_val);
  arg->o += arg->k[arg->i];
  PUSH_REG(shred, SZ_INT);
  arg->i++;
  if(arg->i == arg->s) {
    free(arg->d);
    free(arg->k);
    free(arg);
  } else {
    *(m_uint*)REG(-SZ_INT) = 0;
    shred->next_pc = instr->m_val2;
  }
}

INSTR(Vararg_Member) {
  struct Vararg* arg = *(struct Vararg**)MEM(instr->m_val);
  memcpy(REG(0), (arg->d + arg->o), instr->m_val2);
  PUSH_REG(shred, instr->m_val2);
}

static OP_CHECK(at_varobj) {
  Exp_Binary* bin = (Exp_Binary*)data;
  return bin->rhs->type;
}

INSTR(varobj_assign) {
  POP_REG(shred, 2 * SZ_INT);
  *(M_Object**)REG(SZ_INT) = &*(M_Object*)REG(0);
  PUSH_REG(shred, SZ_INT);
}

m_bool import_vararg(Importer importer) {
  SET_FLAG(&t_varobj, ae_flag_abstract);
  CHECK_BB(importer_add_type(importer,  &t_varobj))
  CHECK_BB(importer_add_type(importer,  &t_varloop))
  CHECK_BB(importer_class_ini(importer,  &t_vararg, NULL, NULL))
  CHECK_BB(importer_item_ini(importer, "@VarLoop",  "start"))
  CHECK_BB(importer_item_end(importer, ae_flag_const, NULL))
  CHECK_BB(importer_item_ini(importer, "@VarLoop",  "end"))
  CHECK_BB(importer_item_end(importer, ae_flag_const, NULL))
  CHECK_BB(importer_item_ini(importer, "int",       "i"))
  CHECK_BB(importer_item_end(importer, ae_flag_const, NULL))
  CHECK_BB(importer_item_ini(importer, "float",     "f"))
  CHECK_BB(importer_item_end(importer,     ae_flag_const, NULL))
  CHECK_BB(importer_item_ini(importer, "time",      "t"))
  CHECK_BB(importer_item_end(importer,     ae_flag_const, NULL))
  CHECK_BB(importer_item_ini(importer, "dur",       "d"))
  CHECK_BB(importer_item_end(importer,     ae_flag_const, NULL))
  CHECK_BB(importer_item_ini(importer, "complex",   "c"))
  CHECK_BB(importer_item_end(importer,     ae_flag_const, NULL))
  CHECK_BB(importer_item_ini(importer, "polar",     "p"))
  CHECK_BB(importer_item_end(importer,     ae_flag_const, NULL))
  CHECK_BB(importer_item_ini(importer, "Vec3",      "v3"))
  CHECK_BB(importer_item_end(importer,    ae_flag_const, NULL))
  CHECK_BB(importer_item_ini(importer, "Vec4",      "v4"))
  CHECK_BB(importer_item_end(importer,    ae_flag_const, NULL))
  CHECK_BB(importer_item_ini(importer, "VarObject", "o"))
  CHECK_BB(importer_item_end(importer,     ae_flag_const | ae_flag_ref, NULL))
  CHECK_BB(importer_class_end(importer))
  CHECK_BB(importer_oper_ini(importer, "VarObject", "Object", NULL))
  CHECK_BB(importer_oper_add(importer, at_varobj))
  CHECK_BB(importer_oper_end(importer, op_at_chuck, varobj_assign))
  CHECK_BB(importer_oper_ini(importer, "Object", "VarObject", NULL))
  CHECK_BB(importer_oper_add(importer, at_varobj))
  CHECK_BB(importer_oper_end(importer, op_at_chuck, varobj_assign))
  return 1;
}
