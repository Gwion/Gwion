#include <stdlib.h>
#include "defs.h"
#include "err_msg.h" // for gwXalloc

static void* xcheck(void* a) {
  if(!a) {
    gw_err("Out of Memory");
    exit(1);
  }
  return a;
}
ANEW void* xmalloc(m_uint sz) {
  void* a = malloc(sz);
  return xcheck(a);
}

ANEW void* xcalloc(m_uint n, m_uint sz) {
  void* a = calloc(n, sz);
  return xcheck(a);
}

ANEW void* xrealloc(void* p, m_uint sz) {
  void* a = realloc(p, sz);
  return xcheck(a);
}
