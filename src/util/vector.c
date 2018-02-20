#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "map.h"

Vector new_vector() {
  Vector v = malloc(sizeof(struct Vector_));
  v->ptr = calloc(MAP_CAP, sizeof(vtype));
  v->ptr[1] = MAP_CAP;
  return v;
}

__attribute__((nonnull))
void vector_init(Vector v) {
  v->ptr = calloc(MAP_CAP, sizeof(vtype));
  v->ptr[1] = MAP_CAP;
}

__attribute__((nonnull))
void vector_release(Vector v) {
  free(v->ptr);
}

__attribute__((nonnull))
void vector_add(const Vector v, const vtype data) {
  if(!(v->ptr[1] - v->ptr[0] - OFFSET))
    v->ptr = realloc(v->ptr, (v->ptr[1] *= 2) * sizeof(vtype));
  v->ptr[v->ptr[0]++ + OFFSET] = (vtype)data;
}

__attribute__((nonnull))
const Vector vector_copy(const Vector v) {
  Vector ret = malloc(sizeof(struct Vector_));
  ret->ptr = calloc(v->ptr[1], sizeof(vtype));
  memcpy(ret->ptr, v->ptr, v->ptr[1] * SZ_INT);
  return ret;
}

__attribute__((nonnull))
void vector_copy2(const restrict Vector v, const Vector ret) {
  ret->ptr = realloc(ret->ptr, v->ptr[1] * sizeof(vtype));
  memcpy(ret->ptr, v->ptr, v->ptr[1] * SZ_INT);
}

__attribute__((nonnull))
const m_int vector_find(const Vector v, const vtype data) {
  vtype i;
  for(i = v->ptr[0] + 1; --i;)
    if(v->ptr[i + OFFSET - 1] == (vtype)data)
      return i - 1;
  return -1;
}

__attribute__((nonnull))
void vector_rem(const Vector v, const vtype index) {
  vtype i;
  if(index >= v->ptr[0])
    return;
  for(i = index + 1; i < v->ptr[0]; i++)
    v->ptr[i - 1 + OFFSET] = v->ptr[i + OFFSET];
  if(--v->ptr[0] + OFFSET < v->ptr[1] / 2)
    v->ptr = realloc(v->ptr, (v->ptr[1] /= 2) * sizeof(vtype));
}

__attribute__((nonnull))
const vtype vector_pop(const Vector v) {
  vtype ret;
  if(!v->ptr[0])
    return 0;
  ret = v->ptr[v->ptr[0] + OFFSET - 1];
  vector_rem(v, v->ptr[0] - 1);
  return ret;
}

__attribute__((nonnull))
void free_vector(Vector v) {
  free(v->ptr);
  free(v);
}

__attribute__((nonnull))
void vector_clear(Vector v) {
  v->ptr = realloc(v->ptr, (v->ptr[1] = MAP_CAP) * sizeof(vtype));
  v->ptr[0] = 0;
}
