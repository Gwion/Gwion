#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwiondata.h"

ANN GwionData* new_gwiondata(MemPool mp) {
  struct GwionData_ *data = mp_calloc(mp, GwionData);
  map_init(&data->freearg);
  vector_init(&data->reserved);
  map_init(&data->pass_map);
  vector_init(&data->pass);
  MUTEX_SETUP(data->mutex);
  return data;
}

ANN void free_gwiondata(MemPool mp, GwionData *data) {
  map_release(&data->freearg);
  vector_release(&data->reserved);
  map_release(&data->pass_map);
  vector_release(&data->pass);
  MUTEX_CLEANUP(data->mutex);
  mp_free(mp, GwionData, data);
}
