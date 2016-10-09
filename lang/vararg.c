#include "defs.h"
#include "vm.h"
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "lang.h"
#include "import.h"

#include "object.h"

struct Vararg
{
  Kindof* k; // k(ind)
  char* d;   // d(ata)
  m_uint o, i, s;  // o(ffset), i(ndex)
};

INSTR(Vararg_start)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "vararg start %i", instr->m_val);
#endif
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  if(!arg->d)
    shred->next_pc = instr->m_val2 + 1;
  *(m_uint*)(shred->reg - SZ_INT) = 0;
}

INSTR(Vararg_end)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(m_uint*)(shred->reg -SZ_INT) = 0;
  switch(arg->k[arg->i])
  {
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
  }
  arg->i++;
  if(arg->i == arg->s)
  {
    free(arg->d);
    free(arg->k);
    free(arg);
    shred->next_pc = shred->next_pc;
  }
   else
   {
    *(m_uint*)(shred->reg -SZ_INT) = 0;
    shred->next_pc = instr->m_val2;
   }
}

INSTR(Vararg_int)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(m_uint*)shred->reg = *(m_uint*)(arg->d + arg->o);
  shred->reg += SZ_INT;
}

INSTR(Vararg_float)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(m_float*)shred->reg = *(m_float*)(arg->d + arg->o);
  shred->reg += SZ_FLOAT;
}

INSTR(Vararg_complex)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(complex*)shred->reg = *(complex*)(arg->d + arg->o);
  shred->reg += SZ_COMPLEX;
}

INSTR(Vararg_Vec3)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(VEC3_T*)shred->reg = *(VEC3_T*)(arg->d + arg->o);
  shred->reg += SZ_VEC3;
}

INSTR(Vararg_Vec4)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(VEC4_T*)shred->reg = *(VEC4_T*)(arg->d + arg->o);
  shred->reg += SZ_VEC4;
}

INSTR(Vararg_object)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(M_Object*)shred->reg = *(M_Object*)(arg->d + arg->o);
  shred->reg += SZ_INT;
}

m_bool import_vararg(Env env)
{
  CHECK_BB(add_global_type(env, &t_vararg))
  CHECK_BB(add_global_type(env, &t_varobj))
  CHECK_BB(import_class_begin(env, &t_vararg, env->global_nspc, NULL, NULL))
  import_mvar(env, "int",       "start", 1, 0, "start vararg loop");
  import_mvar(env, "int",       "end",   1, 0, "end vararg loop");
  import_mvar(env, "int",       "i",     1, 0, "vararg int");
  import_mvar(env, "float",     "f",     1, 0, "vararg float");
  import_mvar(env, "time",      "t",     1, 0, "vararg time");
  import_mvar(env, "dur",       "d",     1, 0, "vararg dur");
  import_mvar(env, "complex",   "c",     1, 0, "vararg complex");
  import_mvar(env, "polar",     "p",     1, 0, "vararg polar");
  import_mvar(env, "vec3",      "v3",    1, 0, "vararg vec3");
  import_mvar(env, "vec4",      "v4",    1, 0, "vararg vec4");
  import_mvar(env, "VarObject", "o",     1, 0, "vararg object. can cast to any object type.");
  CHECK_BB(import_class_end(env))
  return 1;
}
