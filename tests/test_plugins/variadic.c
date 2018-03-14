#include "type.h"
#include "instr.h"
#include "import.h"
#include "vararg.h"

static struct Type_ t_variadic = { "Variadic", SZ_INT, &t_object };

static MFUN(m_variadic) {
  M_Object str_obj = *(M_Object*)MEM(-SZ_INT);
  m_str str = STRING(str_obj);
  struct Vararg_* arg = *(struct Vararg_**)MEM(SZ_INT);

  while(arg->i < arg->s) {
    if(*str == 'i') {
      printf("%" INT_F "\n", *(m_int*)(arg->d + arg->o));
      arg->o += SZ_INT;
    } else if(*str == 'f') {
      printf("%f\n", *(m_float*)(arg->d + arg->o));
      arg->o += SZ_FLOAT;
    } else if(*str == 'o') {
      printf("%p\n", (void*)*(M_Object*)(arg->d + arg->o));
      arg->o += SZ_INT;
    }
    arg->i++;
    str++;
  }
  POP_REG(shred, SZ_INT);
}


IMPORT {
  CHECK_BB(gwi_class_ini(gwi, &t_variadic, NULL, NULL))
  CHECK_BB(gwi_func_ini(gwi, "void", "member", m_variadic))
  CHECK_BB(gwi_func_arg(gwi, "string", "format"))
  CHECK_BB(gwi_func_end(gwi, ae_flag_variadic))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
