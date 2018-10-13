#include <string.h>
#include <stdlib.h>
#include "defs.h"
#include "err_msg.h" // for gwXalloc

#ifdef GWMPOOL
#include "mpool.h"
#include "map.h"
struct Recycle {
  struct Recycle *next;
};

struct pool {
  uint32_t  obj_sz;
//  uint32_t  blk_sz;
  uint32_t  obj_id;
  int32_t   blk_id;
  uint32_t  nblk;
  struct Recycle  *next;
  uint8_t **data;
};

//static struct pool* pools[1 << MEM_STEP];

static struct pool master_pool;
static struct Map_ map;

ANN static void mp_set(struct pool* p, const uint32_t obj_sz/*, const uint32_t blk_sz*/) {
  const uint32_t sz = (obj_sz + 3) & 0xfffffffc;
  p->obj_sz = sz;
//  p->blk_sz = blk_sz;
//  p->obj_id = blk_sz - 1;
//  p->blk_sz = 256;
  p->obj_id = 256 - 1;
  p->blk_id = -1;
  p->nblk   = 1;
  p->next   = NULL;
  p->data   = (uint8_t**)xcalloc(1, sizeof(uint8_t*));
}

__attribute__((constructor(200)))
void mpool_ini() {
  map_init(&map);
  mp_set(&master_pool, sizeof(struct pool)/*, 16*/);
}

__attribute__((destructor(200)))
void mpool_end() {
  LOOP_OPTIM
  for(m_uint i = map_size(&map) + 1; --i;)
    mp_end((struct pool*)VVAL(&map, i - 1));
  map_release(&map);
  mp_end(&master_pool);
}

static struct pool* mp_get(const uint32_t obj_sz/*, const uint32_t blk_sz*/) {
  LOOP_OPTIM
  for(m_uint i = map_size(&map) + 1; --i;) {
    if(VKEY(&map, i - 1) == obj_sz)
      return (struct pool*)VVAL(&map, i - 1);
  }
  struct pool* p = (struct pool*)_mp_alloc2(&master_pool);
  mp_set(p, obj_sz/*, blk_sz*/);
  map_set(&map, obj_sz, (vtype)p);
  return p;
}

struct pool* mp_ini(const uint32_t obj_sz, const uint32_t blk_sz 
__attribute__((unused))) {
  return mp_get((obj_sz + 3) & 0xfffffffc/*, blk_sz*/);
}

void mp_end(struct pool *p) {
  for(uint32_t i = 0; i < p->nblk; ++i) {
//    if(!p->data[i])
//      break;
    free(p->data[i]);
  }
  free(p->data);
}

void *_mp_alloc2(struct pool *p) {
  if(p->next) {
    struct Recycle* recycle = p->next;
    p->next = p->next->next;
    memset(recycle, 0, p->obj_sz);
    return recycle;
  }
//  if(++p->obj_id == p->blk_sz) {
  if(++p->obj_id == 256) {
    p->obj_id = 0;
    if(++p->blk_id == (int32_t)p->nblk) {
//      p->nblk <<= 1;
      ++p->nblk;
      p->data = (uint8_t**)xrealloc(p->data, sizeof(uint8_t*)* p->nblk);
//      for(uint32_t i = p->nblk >> 1; i < p->nblk; ++i)
//        p->data[i] = NULL;
    }
//    if(!p->data[p->blk_id])
//      p->data[p->blk_id] = calloc(p->obj_sz, p->blk_sz);
      p->data[p->blk_id] = calloc(p->obj_sz, 256);
  }
  return p->data[p->blk_id] + p->obj_id * p->obj_sz;
}

void _mp_free2(struct pool *p, void *ptr) {
  struct Recycle* next = p->next;
//  memset(ptr, 0, p->obj_sz);
  p->next = ptr;
  p->next->next = next;
}

void _mp_free(const m_uint size, void *ptr) {
  struct pool* p = mp_get(size);
  return _mp_free2(p, ptr);
}
struct pool* new_pool(const uint32_t obj_sz, const uint32_t blk_sz) {
(void)blk_sz;
  struct pool* p = (struct pool*)xmalloc(sizeof(struct pool));
  mp_set(p, obj_sz/*, blk_sz*/);
  return p;
}

void *_mp_alloc(const m_uint size) {
  struct pool* p = mp_get(size);
  return _mp_alloc2(p);
}
#endif
static const void* xcheck(const void* a) {
  if(!a) {
    gw_err("Out of Memory");
    exit(1);
  }
  return a;
}

ANEW const void* xmalloc(const m_uint sz) {
  const void* a = malloc(sz);
//  const void* a = aligned_alloc(64, sz); //should be a macro getconf LEVEL1_DCACHE_LINESIZE
  return xcheck(a);
}

ANEW const void* xcalloc(const m_uint n, const m_uint sz) {
//  const void* a = aligned_alloc(64, n * sz); //should be a macro getconf LEVEL1_DCACHE_LINESIZE
//  memset(a, 0, sz);
  void* a = calloc(n, sz);
  return xcheck(a);
}

ANEW const void* xrealloc(void* p, const m_uint sz) {
  void* a = realloc(p, sz);
  return xcheck(a);
}
