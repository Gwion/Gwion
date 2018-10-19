#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "map.h"
#include "mpool.h"

ANN inline void vector_init(const Vector v) {
  v->ptr = (m_uint*)xcalloc(MAP_CAP, SZ_INT);
  VCAP(v) = MAP_CAP;
}

Vector new_vector() {
  const Vector v = mp_alloc(Vector);
  vector_init(v);
  return v;
}

ANN inline void vector_release(const Vector v) {
  free(v->ptr);
}

ANN void free_vector(const Vector v) {
  vector_release(v);
  mp_free(Vector, v);
}

ANN void vector_add(const Vector v, const vtype data) {
  if(VCAP(v) - VLEN(v) == OFFSET)
    v->ptr = (m_uint*)xrealloc(v->ptr, (VCAP(v) *= 2) * SZ_INT);
  VPTR(v, VLEN(v)++) = (vtype)data;
}

ANN inline void vector_copy2(const restrict Vector v, const restrict Vector ret) {
  ret->ptr = (m_uint*)xrealloc(ret->ptr, VCAP(v) * SZ_INT);
  memcpy(ret->ptr, v->ptr, VCAP(v) * SZ_INT);
}

ANN Vector vector_copy(const Vector v) {
  const Vector ret = mp_alloc(Vector);
  vector_copy2(v, ret);
  return ret;
}

ANN m_int vector_find(const Vector v, const vtype data) {
  for(vtype i = VLEN(v) + 1; --i;)
    if(VPTR(v, i - 1) == (vtype)data)
      return i - 1;
  return -1;
}

ANN void vector_rem(const Vector v, const vtype index) {
  if(index >= VLEN(v))
    return;
  if(index < VLEN(v) - 1)
    memmove(v->ptr + index + OFFSET, v->ptr + index + OFFSET + 1,
      (VLEN(v) - index - 1) * SZ_INT);
  if(--VLEN(v) + OFFSET < VCAP(v) / 2)
    v->ptr = (m_uint*)xrealloc(v->ptr, (VCAP(v) /= 2) * SZ_INT);
}

ANN void vector_rem2(const Vector v, const vtype data) {
  const m_int index = vector_find(v, data);
  vector_rem(v, index);
}

ANN vtype vector_pop(const Vector v) {
  const vtype ret = VPTR(v, VLEN(v) - 1);
  vector_rem(v, VLEN(v) - 1);
  return ret;
}

ANN void vector_clear(const Vector v) {
  v->ptr = (m_uint*)xrealloc(v->ptr, (VCAP(v) = MAP_CAP) * SZ_INT);
  VLEN(v) = 0;
}
