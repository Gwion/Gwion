#include "gwion_util.h"
#include "gwiondata.h"

ANN GwionData* new_gwiondata(MemPool mp) {
  struct GwionData_ *data = mp_alloc(mp, GwionData);
  map_init(&data->freearg);
  vector_init(&data->reserved);
  MUTEX_SETUP(data->mutex);
  return data;
}

ANN void free_gwiondata(MemPool mp, GwionData *data) {
  map_release(&data->freearg);
  vector_release(&data->reserved);
  MUTEX_CLEANUP(data->mutex);
  mp_free(mp, GwionData, data);
}
