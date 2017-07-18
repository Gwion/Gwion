#ifndef DL_H
#define DL_H

#include <stdlib.h>
#include <complex.h>
#include "defs.h"
#include "vm.h"
#include "map_private.h"

void dl_return_push(const char* retval, VM_Shred shred, m_uint size);

typedef void (*f_xtor)(M_Object o, VM_Shred sh);
typedef void (*f_mfun)(M_Object o, char* RETURN, VM_Shred sh);
typedef void (*f_sfun)(char* RETURN, VM_Shred sh);

typedef struct {
  m_str name;
  m_str type;
} DL_Value;

typedef struct {
  m_str    name;
  m_str    type;
  m_uint   addr;
  m_uint   narg;
  DL_Value args[6];
} DL_Func;


void dl_func_init(DL_Func* fun, const m_str t, const m_str n, const m_uint addr);
void dl_func_add_arg(DL_Func* a, const m_str t, const m_str  n);
#endif
