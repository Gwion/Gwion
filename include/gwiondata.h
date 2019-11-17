#ifndef __GWIONDATA
#define __GWIONDATA
typedef struct GwionData_ {
  struct Map_ freearg;
  struct Map_ id;
  MUTEX_TYPE mutex;
  struct Vector_ child;
  struct Vector_ child2;
  struct Vector_ reserved;
  struct Passes_  *passes;
  PlugInfo* plug;
} GwionData;

ANN GwionData* new_gwiondata(MemPool);
ANN void free_gwiondata(const struct Gwion_*);
#endif
