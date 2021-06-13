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

ANN static inline GwionData *gwiondata(MemPool mp) {
  struct GwionData_ *data = mp_calloc(mp, GwionData);
  MUTEX_SETUP(data->mutex);
  return data;
}

ANN GwionData *new_gwiondata(const MemPool mp) {
  GwionData *data = gwiondata(mp);
  map_init(&data->freearg);
  map_init(&data->id);
  return data;
}

ANN GwionData *cpy_gwiondata(MemPool mp, const GwionData *src) {
  GwionData *data = gwiondata(mp);
  data->freearg   = src->freearg;
  data->id        = src->id;
  data->plug      = src->plug;
  data->passes    = src->passes;
  return data;
}

ANN void free_gwiondata_cpy(const MemPool mp, GwionData *data) {
  MUTEX_CLEANUP(data->mutex);
  mp_free(mp, GwionData, data);
}

ANN void free_gwiondata(const Gwion gwion) {
  GwionData *data = gwion->data;
  map_release(&data->freearg);
  for (m_uint i = 0; i < map_size(&data->id); ++i)
    mp_free(gwion->mp, SpecialId, (struct SpecialId_ *)map_at(&data->id, i));
  map_release(&data->id);
  free_passes(gwion->mp, data->passes);
  if (data->plug.ptr) free_plug(gwion);
  free_gwiondata_cpy(gwion->mp, data);
}
