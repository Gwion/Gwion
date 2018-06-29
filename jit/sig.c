#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jit/jit.h>
#include <gwjit_common.h>
#include "map.h"
#include "symbol.h"

ANN static jit_type_t find(const Map m, const m_str s) {
  const m_uint size = map_size(m);
  for(m_uint i = 0; i < size; ++i)
    if(!strcmp((m_str)vector_at((Vector)m, i*2), s))
      return (jit_type_t)map_at(m, i);
  return NULL;
}

static jit_type_t get(const char c) {
  if(c == 'v')
    return jit_type_void;
  if(c == 'p')
    return jit_type_void_ptr;
  if(c == 'i')
    return jit_type_int;
  if(c == 'u')
    return jit_type_uint;
  if(c == 'I')
    return jit_type_nint;
  if(c == 'U')
    return jit_type_nuint;
  if(c == 'f')
    return jit_type_float;
  fprintf(stderr, "jit signature error: unknown type '%c'.\n", c);
  exit(1);
}

ANN jit_type_t sig(const Map m, m_str s) {
printf("request %s\n", s);
  const jit_type_t old = find(m, s);
  if(old)
    return old;
  const jit_type_t ret = get(s[0]);
  const m_str c = strdup(s);
  const m_uint len = strlen(++s);
  jit_type_t arg[len];
  for(m_uint i = 0; i < len; ++i)
    arg[i] = get(s[i]);
  jit_type_t t = jit_type_create_signature
         (jit_abi_cdecl, ret, arg, len, 1);
printf("set map_size %lu %s %p\n", m->ptr[0], c, t);
  map_set(m, (vtype)c, (vtype)t);
printf("set map_size %lu %s %p\n", m->ptr[0], c, t);
  return t;
}

ANN void sig_end(const Map m) {
  const m_uint size = map_size(m);
printf("map_size %lu %lu\n", size, m->ptr[0]);
  for(m_uint i = 0; i < size; ++i) {
const jit_type_t t = (jit_type_t)map_at(m, i);
printf("map_size %lu: key: %s %p %p\n", i, 
(m_str)vector_at((Vector)m, i*2),
(void*)map_at(m, i), t);
    xfree((m_str)vector_at((Vector)m, i*2));
//    jit_type_free((jit_type_t)map_at(m, i));
    jit_type_free(t);
  }
  map_release(m);
}
