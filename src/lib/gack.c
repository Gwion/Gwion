#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
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

ANN2(1) static int fmtlen(const char *fmt, va_list args) {
  va_list tmpa;
  va_copy(tmpa, args);
  const int size = vsnprintf(NULL, 0, fmt, tmpa);
  va_end(tmpa);
  return size;
}

ANN2(2) static int gw_vasprintf(MemPool mp, char **str, const char *fmt, va_list args) {
  char *base = *str;
  const size_t base_len = base ? strlen(base) : 0;
  DECL_BB(const int, size, = fmtlen(fmt, args))
  char *ret = mp_malloc2(mp, base_len + size + 1);
  if(base)
    strcpy(ret, base);
  const int final_len = vsprintf(ret + base_len, fmt, args);
  if(final_len < 0) {
    mp_free2(mp, base_len + size + 1, ret);
    return -1;
  }
  if(base)
    mp_free2(mp, strlen(base), base);
  *str = ret;
  return final_len;
}

ANN2(2) int gw_asprintf(MemPool mp, char **str, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  const int ret = gw_vasprintf(mp, str, fmt, args);
  va_end(args);
  return ret;
}

__attribute__((returns_nonnull))
ANN static inline VM_Code get_gack(Type t) {
  do if(t->e->gack)
    return t->e->gack;
  while((t = t->e->parent));
  return t->e->gack; // unreachable
}

ANN static void prepare_call(const VM_Shred shred, const m_uint offset) {
  shred->mem += offset;
  *(m_uint*)(shred->mem  + SZ_INT) = offset + SZ_INT;
  *(VM_Code*)(shred->mem + SZ_INT*2) = shred->code;
  *(m_uint*)(shred->mem  + SZ_INT*3) = shred->pc;
  *(m_uint*)(shred->mem  + SZ_INT*4) = SZ_INT;
  shred->mem += SZ_INT*5;
  *(M_Object*)(shred->mem)= *(M_Object*)(shred->reg - SZ_INT);
  shred->pc = 0;
}

ANN void gack(const VM_Shred shred, const m_uint offset) {
  const Type t = *(Type*)shred->reg;
  const VM_Code code = get_gack(t);
  if(GET_FLAG(code, builtin)) {
    ((f_gack)code->native_func)(t, (shred->reg - t->size), shred);
    POP_REG(shred, t->size);
  } else {
    prepare_call(shred, offset);
    shred->code = code;
    POP_REG(shred, SZ_INT*2);
  }
  return;
}

