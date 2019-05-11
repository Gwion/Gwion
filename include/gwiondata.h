#ifndef __GWIONDATA
#define __GWIONDATA

typedef struct GwionData_ {
  struct Map_ freearg;
  MUTEX_TYPE mutex;
  struct Vector_ child;
  struct Vector_ reserved;
} GwionData;

ANN GwionData* new_gwiondata(MemPool);
ANN void free_gwiondata(MemPool, GwionData*);
#endif
