#ifndef __GWIONDATA
#define __GWIONDATA
typedef struct GwionData_ {
  struct Map_     freearg;
  struct Map_     id;
  MUTEX_TYPE      mutex;
  struct Vector_  child;
  struct Vector_  child2;
  struct Passes_ *passes;
  struct Map_     plug;
  bool            cdoc;
} GwionData;

ANN GwionData *new_gwiondata(const MemPool);
ANN GwionData *cpy_gwiondata(MemPool, const GwionData *);
ANN void       free_gwiondata(const Gwion);
ANN void       free_gwiondata_cpy(const MemPool, GwionData *);
#endif
