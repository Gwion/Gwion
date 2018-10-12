#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "mpool.h"
#include "mpool_private.h"

#define POOL_NUMBER  ((SIZEOF_REG + SIZEOF_MEM) / SZ_INT)
#define POOL_SIZE(a) (((a) + SZ_INT -1) & 0xfffffffc)
#define POOL_IDX(a) (((a)/SZ_INT) -1)

static struct pool master_pool;
static struct pool* pools[POOL_NUMBER];

ANN static void mp_set(struct pool* p, const uint32_t obj_sz) {
  p->obj_sz = POOL_SIZE(obj_sz);
  p->obj_id = BLK - 1;
  p->blk_id = -1;
  p->nblk   = 1;
  p->next   = NULL;
  p->data   = (uint8_t**)xcalloc(1, sizeof(uint8_t*));
}

__attribute__((constructor(200)))
void mpool_ini() {
  mp_set(&master_pool, sizeof(struct pool));
}

__attribute__((destructor(200)))
void mpool_end() {
  LOOP_OPTIM
  for(m_uint i = POOL_NUMBER + 1; --i;) {
    struct pool* p = pools[i - 1];
    if(p)
      mp_end(p);
  }
  mp_end(&master_pool);
}

static struct pool* mp_get(const uint32_t obj_sz) {
  const uint32_t idx = POOL_IDX(obj_sz);
  struct pool* orig = pools[idx];
  if(orig)
    return orig;
  struct pool* p = (struct pool*)_mp_alloc2(&master_pool);
  mp_set(p, obj_sz);
  return pools[idx] = p;
}

struct pool* mp_ini(const uint32_t obj_sz) {
  return mp_get(POOL_SIZE(obj_sz));
}

void mp_end(struct pool *p) {
  for(uint32_t i = 0; i < p->nblk && p->data[i]; ++i)
    free(p->data[i]);
  free(p->data);
}

void *_mp_alloc2(struct pool *p) {
  if(p->next) {
    struct Recycle* recycle = p->next;
    p->next = p->next->next;
    memset(recycle, 0, p->obj_sz);
    return recycle;
  }
  if(++p->obj_id == BLK) {
    p->obj_id = 0;
    if(++p->blk_id == (int32_t)p->nblk) {
      p->nblk <<= 1;
      p->data = (uint8_t**)xrealloc(p->data, sizeof(uint8_t*)* p->nblk);
      for(uint32_t i = (p->nblk >> 1) + 1; i < p->nblk; ++i)
        p->data[i] = NULL;
    }
    p->data[p->blk_id] = (uint8_t*)xcalloc(BLK, p->obj_sz);
  }
  return p->data[p->blk_id] + p->obj_id * p->obj_sz;
}

void _mp_free2(struct pool *p, void *ptr) {
  struct Recycle* next = p->next;
#ifdef POOL_CHECK
  memset(ptr, 0, p->obj_sz);
#endif
  p->next = ptr;
  p->next->next = next;
}

void _mp_free(const m_uint size, void *ptr) {
  struct pool* p = mp_get(POOL_SIZE(size));
  return _mp_free2(p, ptr);
}

struct pool* new_pool(const uint32_t obj_sz) {
  struct pool* p = (struct pool*)xmalloc(sizeof(struct pool));
  mp_set(p, obj_sz);
  return p;
}

void *_mp_alloc(const m_uint size) {
  struct pool* p = mp_get(POOL_SIZE(size));
  return _mp_alloc2(p);
}

#ifndef MEM_UNSECURE
#include "err_msg.h" // for gwXalloc
static const void* xcheck(const void* a) {
  if(!a)
    exit(gw_err("Out of Memory"));
  return a;
}
#else
#define xcheck(a) (a)
#endif

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
