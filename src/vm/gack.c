#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
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

ANN2(2)
static int gw_vasprintf(MemPool mp, char **str, const char *fmt, va_list args) {
  char *       base     = *str;
  const size_t base_len = base ? strlen(base) : 0;
  DECL_BB(const int, size, = fmtlen(fmt, args));
  char *ret = mp_malloc2(mp, base_len + size + 1);
  if (base) strcpy(ret, base);
  const int final_len = vsprintf(ret + base_len, fmt, args);
  if (final_len < 0) {
    mp_free2(mp, base_len + size + 1, ret);
    return -1;
  }
  if (base) mp_free2(mp, strlen(base), base);
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

ANN static void prepare_call(const VM_Shred shred, const m_uint offset) {
  const m_uint push = offset + SZ_INT + sizeof(struct frame_t);
  shred->mem += push;
  struct frame_t *frame =
      (struct frame_t *)(shred->mem - sizeof(struct frame_t));
  frame->push = push;
  frame->code = shred->code;
  frame->pc   = shred->pc;
  shred->pc   = 0;
}

ANN void gack(const VM_Shred shred, const m_uint offset) {
  const Type    t    = *(Type *)shred->reg;
  const VM_Code code = get_gack(t)->info->gack;
  if (code->builtin) {
    const m_uint sz = *(m_uint *)(shred->reg + SZ_INT);
    ((f_gack)code->native_func)(t, (shred->reg - sz), shred);
    POP_REG(shred, sz);
  } else {
    prepare_call(shred, offset);
    if (tflag(t, tflag_struct))
      *(void **)(shred->mem) = (void *)(shred->reg - t->size);
    else
      *(M_Object *)(shred->mem) = *(M_Object *)(shred->reg - SZ_INT);
    shred->code = code;
  }
  return;
}
