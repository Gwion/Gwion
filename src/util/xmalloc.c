#include <stdlib.h>
#include "defs.h"
#include "err_msg.h" // for gwXalloc

static const void* xcheck(const void* a) {
  if(!a) {
    gw_err("Out of Memory");
    exit(1);
  }
  return a;
}

ANEW const void* xmalloc(const m_uint sz) {
  const void* a = malloc(sz);
  return xcheck(a);
}

ANEW const void* xcalloc(const m_uint n, const m_uint sz) {
  void* a = calloc(n, sz);
  return xcheck(a);
}

ANEW const void* xrealloc(void* p, const m_uint sz) {
  void* a = realloc(p, sz);
  return xcheck(a);
}
