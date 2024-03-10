#ifndef __GWIONDATA
#define __GWIONDATA

typedef struct Plugs {
  struct Map_ map; //! map of plugs
  struct Vector_ vec; //! dependencies
} Plugs;

typedef struct GwionData_ {
  struct Map_     freearg;
  struct Map_     id;
  gwtlock_t       mutex;
  struct Vector_  child;
  struct Vector_  child2;
  struct Passes_ *passes;
  Plugs *plugs;
  threadpool_t    *tpool;
  bool            cdoc;
  bool            errored;
  bool            color;
} GwionData;

ANN GwionData *new_gwiondata(const MemPool, const uint32_t thread_count, const uint32_t queue_size);
ANN GwionData *cpy_gwiondata(MemPool, const GwionData *);
ANN void       free_gwiondata(const Gwion);
ANN void       free_gwiondata_cpy(const MemPool, GwionData *);
#endif
