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

void vector_init(struct Vector_* v) {
  v->ptr = calloc(MAP_CAP, sizeof(vtype));
  v->ptr[1] = MAP_CAP;
}

void vector_release(Vector v) {
  free(v->ptr);
}

void vector_add(Vector v, vtype data) {
  if(!(v->ptr[1] - v->ptr[0] - OFFSET))
    v->ptr = realloc(v->ptr, (v->ptr[1] *= 2) * sizeof(vtype));
  v->ptr[v->ptr[0]++ + OFFSET] = (vtype)data;
}

Vector vector_copy(Vector v) {
  Vector ret = malloc(sizeof(struct Vector_));
  ret->ptr = calloc(v->ptr[1], sizeof(vtype));
  memcpy(ret->ptr, v->ptr, v->ptr[1] * SZ_INT);
  return ret;
}

void vector_copy2(Vector v, Vector ret) {
  ret->ptr = realloc(ret->ptr, v->ptr[1] * sizeof(vtype));
  memcpy(ret->ptr, v->ptr, v->ptr[1] * SZ_INT);
}

m_int vector_find(Vector v, vtype data) {
  vtype i;
  for(i = v->ptr[0] + 1; --i;)
    if(v->ptr[i + OFFSET - 1] == (vtype)data)
      return i - 1;
  return -1;
}

void vector_rem(Vector v, const vtype index) {
  vtype i;
  if(index >= v->ptr[0])
    return;
  for(i = index + 1; i < v->ptr[0]; i++)
    v->ptr[i - 1 + OFFSET] = v->ptr[i + OFFSET];
  if(--v->ptr[0] + OFFSET < v->ptr[1] / 2)
    v->ptr = realloc(v->ptr, (v->ptr[1] /= 2) * sizeof(vtype));
}

vtype vector_pop(Vector v) {
  vtype ret;
  if(!v->ptr[0])
    return 0;
  ret = v->ptr[v->ptr[0] + OFFSET - 1];
  vector_rem(v, v->ptr[0] - 1);
  return ret;
}

void free_vector(Vector v) {
  free(v->ptr);
  free(v);
}

void vector_clear(Vector v) {
  v->ptr = realloc(v->ptr, (v->ptr[1] = MAP_CAP) * sizeof(vtype));
  v->ptr[0] = 0;
}
