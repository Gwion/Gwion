#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "map.h"
#include "mpool.h"

POOL_HANDLE(Vector, 2048)

Vector new_vector() {
  Vector v = mp_alloc(Vector);
  v->ptr = calloc(MAP_CAP, SZ_INT);
  VCAP(v) = MAP_CAP;
  return v;
}

ANN void vector_init(Vector v) {
  v->ptr = calloc(MAP_CAP, SZ_INT);
  VCAP(v) = MAP_CAP;
}

ANN void vector_release(Vector v) {
  free(v->ptr);
}

ANN void vector_add(const Vector v, const vtype data) {
  if(!(VCAP(v) - VLEN(v) - OFFSET))
    v->ptr = realloc(v->ptr, (VCAP(v) *= 2) * SZ_INT);
  VPTR(v, VLEN(v)++) = (vtype)data;
}

ANN const Vector vector_copy(const Vector v) {
  Vector ret = mp_alloc(Vector);
  ret->ptr = calloc(VCAP(v), SZ_INT);
  memcpy(ret->ptr, v->ptr, VCAP(v) * SZ_INT);
  return ret;
}

ANN void vector_copy2(const restrict Vector v, const Vector ret) {
  ret->ptr = realloc(ret->ptr, VCAP(v) * SZ_INT);
  memcpy(ret->ptr, v->ptr, VCAP(v) * SZ_INT);
}

ANN const m_int vector_find(const Vector v, const vtype data) {
  vtype i;
  for(i = VLEN(v) + 1; --i;)
    if(VPTR(v, i - 1) == (vtype)data)
      return i - 1;
  return -1;
}

ANN void vector_rem(const Vector v, const vtype index) {
  vtype i;
  if(index >= VLEN(v))
    return;
  for(i = index + 1; i < VLEN(v); i++)
    VPTR(v, i - 1) = VPTR(v, i);
  if(--VLEN(v) + OFFSET < VCAP(v) / 2)
    v->ptr = realloc(v->ptr, (VCAP(v) /= 2) * SZ_INT);
}

ANN const vtype vector_pop(const Vector v) {
  vtype ret;
  if(!VLEN(v))
    return 0;
  ret = VPTR(v, VLEN(v) - 1);
  vector_rem(v, VLEN(v) - 1);
  return ret;
}

ANN void free_vector(Vector v) {
  free(v->ptr);
  mp_free(Vector, v);
}

ANN void vector_clear(Vector v) {
  v->ptr = realloc(v->ptr, (VCAP(v) = MAP_CAP) * SZ_INT);
  VLEN(v) = 0;
}
