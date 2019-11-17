#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "plug.h"
#include "gwiondata.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "gwion.h"
#include "specialid.h"
#include "pass.h"

ANN GwionData* new_gwiondata(MemPool mp) {
  struct GwionData_ *data = mp_calloc(mp, GwionData);
  map_init(&data->freearg);
  map_init(&data->id);
  vector_init(&data->reserved);
  data->passes = new_passes(mp);
  MUTEX_SETUP(data->mutex);
  return data;
}

ANN void free_gwiondata(const struct Gwion_ *gwion) {
  struct GwionData_ *data = gwion->data;
  map_release(&data->freearg);
  for(m_uint i = 0; i < map_size(&data->id); ++i)
    mp_free(gwion->mp, SpecialId, (struct SpecialId_*)map_at(&data->id, i));
  map_release(&data->id);
  vector_release(&data->reserved);
  free_passes(data->passes);
  if(data->plug)
    free_plug(gwion);
  MUTEX_CLEANUP(data->mutex);
  mp_free(gwion->mp, GwionData, data);
}
