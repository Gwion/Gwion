#include <string.h>
#include <stdlib.h>
#include "defs.h"
#include "mpool.h"
#include "map.h"
#undef mp_alloc
#undef mp_free

struct Recycle {
  struct Recycle *next;
};

struct pool {
  uint32_t  obj_sz;
  uint32_t  blk_sz;
  uint32_t  obj_id;
  int32_t   blk_id;
  uint32_t  nblk;
  struct Recycle  *next;
  uint8_t **data;
};
static struct pool master_pool;
static struct Map_ map;

ANN static void mp_set(struct pool* p, const uint32_t obj_sz, const uint32_t blk_sz) {
  const uint32_t sz = (obj_sz + 3) & 0xfffffffc;
  p->obj_sz = sz;
  p->blk_sz = blk_sz;
  p->obj_id = blk_sz - 1;
  p->blk_id = -1;
  p->nblk   = 1;
  p->next   = NULL;
  p->data   = (uint8_t**)xcalloc(1, sizeof(uint8_t*));
}

__attribute__((constructor(200)))
void mpool_ini() {
  map_init(&map);
  mp_set(&master_pool, sizeof(struct pool), 16);
}

__attribute__((destructor(200)))
void mpool_end() {
  LOOP_OPTIM
  for(m_uint i = map_size(&map) + 1; --i;)
    mp_end((struct pool*)VVAL(&map, i - 1));
  map_release(&map);
  mp_end(&master_pool);
}

static struct pool* mp_get(const uint32_t obj_sz, const uint32_t blk_sz) {
  LOOP_OPTIM
  for(m_uint i = map_size(&map) + 1; --i;) {
    if(VKEY(&map, i - 1) == obj_sz)
      return (struct pool*)VVAL(&map, i - 1);
  }
  struct pool* p = (struct pool*)mp_alloc(&master_pool);
  mp_set(p, obj_sz, blk_sz);
  map_set(&map, obj_sz, (vtype)p);
  return p;
}

struct pool* mp_ini(const uint32_t obj_sz, const uint32_t blk_sz) {
  return mp_get((obj_sz + 3) & 0xfffffffc, blk_sz);
}

void mp_end(struct pool *p) {
  for(uint32_t i = p->nblk + 1; --i;)
    free(p->data[i-1]);
  free(p->data);
}

void *mp_alloc(struct pool *p) {
  if(p->next) {
    void *recycle = p->next;
    p->next = p->next->next;
    memset(recycle, 0, p->obj_sz);
    return recycle;
  }
  if(++p->obj_id == p->blk_sz) {
    p->obj_id = 0;
    if(++p->blk_id == (int32_t)p->nblk) {
      p->nblk <<= 1;
      p->data = (uint8_t**)xrealloc(p->data, sizeof(uint8_t*)* p->nblk);
      memset(p->data + (p->nblk >> 1), 0, (p->nblk - (p->nblk >> 1)) * sizeof(uint8_t*));
    }
    p->data[p->blk_id] = calloc(p->obj_sz, p->blk_sz);
  }
  return p->data[p->blk_id] + p->obj_id * p->obj_sz;
}

void mp_free(struct pool *p, void *ptr) {
  struct Recycle* next = p->next;
  p->next = ptr;
  p->next->next = next;
}
