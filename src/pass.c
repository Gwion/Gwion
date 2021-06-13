#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "gwion.h"
#include "pass.h"
#include "traverse.h"

#define N_PASS     2
#define N_SCANPASS 4

static const m_str            default_passes_name[2] = {"check", "emit"};
static const compilation_pass default_passes[4]      = {traverse_ast, emit_ast};
static const m_str            scan_passes_name[4] = {"scan0", "scan1", "scan2",
                                          "type_check"};
static const compilation_pass scan_passes[4] = {scan0_ast, scan1_ast, scan2_ast,
                                                check_ast};

ANN void pass_register(const Gwion gwion, const m_str name,
                       const compilation_pass pass) {
  const Symbol sym = insert_symbol(gwion->st, name);
  map_set(&gwion->data->passes->map, (vtype)sym, (vtype)pass);
}

ANN m_bool pass_set(const Gwion gwion, const Vector passes) {
  const Vector v = &gwion->data->passes->vec;
  vector_clear(v);
  for (m_uint i = 0; i < vector_size(passes); ++i) {
    const m_str            name = (m_str)vector_at(passes, i);
    const Symbol           sym  = insert_symbol(gwion->st, name);
    const compilation_pass pass =
        (compilation_pass)map_get(&gwion->data->passes->map, (vtype)sym);
    if (!pass) {
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
  for (m_uint i = 0; i < N_PASS; ++i)
    vector_add(&v, (vtype)default_passes_name[i]);
  pass_set(gwion, &v);
  vector_release(&v);
}

ANEW ANN struct Passes_ *new_passes(const Gwion gwion) {
  struct Passes_ *a = gwion->data->passes = mp_calloc(gwion->mp, Passes);
  map_init(&a->map);
  for (m_uint i = 0; i < N_PASS; ++i)
    pass_register(gwion, default_passes_name[i], default_passes[i]);
  for (m_uint i = 0; i < N_SCANPASS; ++i)
    pass_register(gwion, scan_passes_name[i], scan_passes[i]);
  vector_init(&a->vec);
  return a;
}

ANN void free_passes(const MemPool mp, struct Passes_ *a) {
  map_release(&a->map);
  vector_release(&a->vec);
  mp_free(mp, Passes, a);
}
