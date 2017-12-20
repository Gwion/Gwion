#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"

struct Type_ t_vararg  = { "@Vararg",   SZ_INT, &t_object, te_vararg};
struct Type_ t_varobj  = { "@VarObject", SZ_INT, &t_object, te_vararg};
struct Type_ t_varloop = { "@VarLoop",  SZ_INT, NULL,      te_vararg_loop};

struct Vararg {
  Kindof* k;      // k(ind)
  char* d;	// d(ata)
  m_uint o, i, s; // o(ffset), i(ndex)
};

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
  arg->k = arg->s ? calloc(arg->s, sizeof(Kindof)) : NULL;
  for(i = 0; i < arg->s; i++) {
    arg->k[i] = (Kindof)vector_at(kinds, i);
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
  switch(arg->k[arg->i]) {
    case Kindof_Int:
      arg->o += SZ_INT;
      break;
    case Kindof_Float:
      arg->o += SZ_FLOAT;
      break;
    case Kindof_Complex:
      arg->o += SZ_COMPLEX;
      break;
    case Kindof_Vec3:
      arg->o += SZ_VEC3;
      break;
    case Kindof_Vec4:
      arg->o += SZ_VEC4;
      break;
    case Kindof_Void:
      break;
  }
  PUSH_REG(shred, SZ_INT);
  arg->i++;
  if(arg->i == arg->s) {
    free(arg->d);
    free(arg->k);
    free(arg);
  } else {
    *(m_uint*)REG(- SZ_INT) = 0;
    shred->next_pc = instr->m_val2;
  }
}

INSTR(Vararg_Member) {
  struct Vararg* arg = *(struct Vararg**)MEM(instr->m_val);
  /**(char*)REG(0) = *(char*)(arg->d + arg->o);*/
  memcpy(REG(0), (arg->d + arg->o), instr->m_val2);
  PUSH_REG(shred, instr->m_val2);
}

Type at_varobj(Env env, void* data) {
  Exp_Binary* bin = (Exp_Binary*)data;
  return bin->rhs->type;
}

INSTR(varobj_assign) {
  POP_REG(shred, 2 * SZ_INT);
  *(M_Object**)REG(SZ_INT) = &*(M_Object*)REG(0);
  PUSH_REG(shred, SZ_INT);
}

m_bool import_vararg(Importer importer) {
  CHECK_BB(importer_add_type(importer,  &t_varobj))
  CHECK_BB(importer_add_type(importer,  &t_varloop))
  CHECK_BB(importer_class_ini(importer,  &t_vararg, NULL, NULL))
	importer_item_ini(importer, "@VarLoop",  "start");
  importer_item_end(importer, ae_flag_const, NULL);
	importer_item_ini(importer, "@VarLoop",  "end");
  importer_item_end(importer,   ae_flag_const, NULL);
	importer_item_ini(importer, "int",       "i");
  importer_item_end(importer,     ae_flag_const, NULL);
	importer_item_ini(importer, "float",     "f");
  importer_item_end(importer,     ae_flag_const, NULL);
	importer_item_ini(importer, "time",      "t");
  importer_item_end(importer,     ae_flag_const, NULL);
	importer_item_ini(importer, "dur",       "d");
  importer_item_end(importer,     ae_flag_const, NULL);
	importer_item_ini(importer, "complex",   "c");
  importer_item_end(importer,     ae_flag_const, NULL);
	importer_item_ini(importer, "polar",     "p");
  importer_item_end(importer,     ae_flag_const, NULL);
	importer_item_ini(importer, "Vec3",      "v3");
  importer_item_end(importer,    ae_flag_const, NULL);
	importer_item_ini(importer, "Vec4",      "v4");
  importer_item_end(importer,    ae_flag_const, NULL);
	importer_item_ini(importer, "@VarObject", "o");
  importer_item_end(importer,     ae_flag_const, NULL);
  CHECK_BB(importer_class_end(importer))
  CHECK_BB(importer_oper_ini(importer, "VarObject", "Object", NULL))
  CHECK_BB(importer_oper_add(importer, at_varobj))
  CHECK_BB(importer_oper_end(importer, op_at_chuck, varobj_assign, 0))
  return 1;
}
