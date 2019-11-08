#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "object.h"
#include "instr.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gack.h"

ANN void gack(const VM_Shred shred, const Instr instr) {
  Type base = (Type)instr->m_val, t = base;
  do {
    if(t->e->gack) {
      if(GET_FLAG(t->e->gack, builtin))
        ((f_gack)t->e->gack->native_func)(base, (shred->reg - t->size), shred);
      else {
        shred->mem += instr->m_val2;
        *(m_uint*)(shred->mem+ SZ_INT) = instr->m_val2 + SZ_INT;
        *(VM_Code*)(shred->mem+ SZ_INT*2) = shred->code;
        *(m_uint*)(shred->mem+ SZ_INT*3) = shred->pc;
        *(m_uint*)(shred->mem+ SZ_INT*4) = SZ_INT;
        shred->mem += SZ_INT*5;
        *(M_Object*)(shred->mem)= *(M_Object*)(shred->reg - SZ_INT);
        shred->code = t->e->gack;
        shred->pc = 0;
      }
      return;
    }
  } while((t = t->e->parent));
}
