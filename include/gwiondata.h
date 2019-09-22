#ifndef __GWIONDATA
#define __GWIONDATA
typedef struct Env_* Env;
typedef m_bool (*compilation_pass)(Env, Ast);
typedef struct GwionData_ {
  struct Map_ freearg;
  MUTEX_TYPE mutex;
  struct Vector_ child;
  struct Vector_ reserved;
  struct Map_ pass_map;
  struct Vector_ pass;
} GwionData;

ANN GwionData* new_gwiondata(MemPool);
ANN void free_gwiondata(MemPool, GwionData*);
#endif
