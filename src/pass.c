#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "gwion.h"
#include "pass.h"

static const m_str default_passes_name[] = { "check", "emit" };
static const compilation_pass default_passes[][2] = { { type_engine_check_prog, type_engine_clean_prog }, { emit_ast, NULL } };
#define NPASS sizeof(default_passes)/sizeof(default_passes[0])

ANN void pass_register(const Gwion gwion, const m_str name, const compilation_pass pass[2]) {
  compilation_pass *passes = mp_malloc2(gwion->mp, sizeof(compilation_pass)*2);
  passes[0] = pass[0];
  passes[1] = pass[1];
  const Symbol sym = insert_symbol(gwion->st, name);
  map_set(&gwion->data->passes->map, (vtype)sym, (vtype)pass);
  map_set(&gwion->data->passes->map, (vtype)sym, (vtype)passes);
}

ANN m_bool pass_set(const Gwion gwion, const Vector passes) {
  const Vector v = &gwion->data->passes->vec;
  vector_clear(v);
  for(m_uint i = 0; i < vector_size(passes); ++i) {
    const m_str name = (m_str)vector_at(passes, i);
    const Symbol sym = insert_symbol(gwion->st, name);
    const compilation_pass pass = (compilation_pass)map_get(&gwion->data->passes->map, (vtype)sym);
    if(!pass) {
      gw_err("Failed to set compilation passes, back to default\n");
      pass_default(gwion);
      return GW_ERROR;
    }
    vector_add(v, (vtype)pass);
  }
  return GW_OK;
}

ANN void pass_default(const Gwion gwion) {
  struct Vector_ v;
  vector_init(&v);
  for(m_uint i = 0; i < NPASS; ++i) {
    vector_add(&v, (vtype)default_passes_name[i]);
    pass_register(gwion, default_passes_name[i], default_passes[i]);
  }
  pass_set(gwion, &v);
  vector_release(&v);
}


ANEW ANN struct Passes_* new_passes(MemPool mp) {
  struct Passes_ *a = mp_calloc(mp, Passes);
  map_init(&a->map);
  vector_init(&a->vec);
  return a;
}

ANN void free_passes(const MemPool mp, struct Passes_ *a) {
  map_release(&a->map);
  for(m_uint i = 0; i < vector_size(&a->vec); ++i) {
    compilation_pass *passes = (compilation_pass *)vector_at(&a->vec, i);
    mp_free2(mp, sizeof(compilation_pass)*2, passes);
  }
  vector_release(&a->vec);
}
