#include <string.h>
#include <stdlib.h>
#include "defs.h"
#include "mpool.h"
#undef mp_alloc
#undef mp_free

typedef struct Recycle {
    struct Recycle *next;
} Recycle;

ANN void mp_ini(pool *p, const uint32_t obj_sz, const uint32_t blk_sz) {
  p->obj_sz = obj_sz;
  p->blk_sz = blk_sz;
  p->obj_id = blk_sz - 1;
  p->blk_id = -1;
  p->nblk = 1;
  p->data = calloc(1, sizeof(uint8_t*));
}

ANN void mp_end(pool *p) {
  for(uint32_t i = p->nblk + 1; --i;)
    free(p->data[i-1]);
  free(p->data);
}

ANN void *mp_alloc(pool *p) {
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
      p->data = realloc(p->data, sizeof(uint8_t*)* p->nblk);
            for(uint32_t i = p->nblk >> 1; i < p->nblk; ++i)
                p->data[i] = NULL;
//      memset(p->data + (p->nblk >> 1), 0, p->nblk - (p->nblk >> 1));
    }
    p->data[p->blk_id] = calloc(p->obj_sz, p->blk_sz);
  }
  return p->data[p->blk_id] + p->obj_id * p->obj_sz;
}

ANN void mp_free(pool *p, void *ptr) {
  Recycle* next = p->next;
  p->next = ptr;
  p->next->next = next;
}
