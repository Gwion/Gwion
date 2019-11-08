#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "gwiondata.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "emit.h"
#include "specialid.h"

ANN GwionData* new_gwiondata(MemPool mp) {
  struct GwionData_ *data = mp_calloc(mp, GwionData);
  map_init(&data->freearg);
  map_init(&data->id);
  vector_init(&data->reserved);
  map_init(&data->pass_map);
  vector_init(&data->pass);
  MUTEX_SETUP(data->mutex);
  return data;
}

ANN void free_gwiondata(MemPool mp, GwionData *data) {
  map_release(&data->freearg);
  for(m_uint i = 0; i < map_size(&data->id); ++i)
    mp_free(mp, SpecialId, (struct SpecialId_*)map_at(&data->id, i));
  map_release(&data->id);
  vector_release(&data->reserved);
  map_release(&data->pass_map);
  vector_release(&data->pass);
  MUTEX_CLEANUP(data->mutex);
  mp_free(mp, GwionData, data);
}
