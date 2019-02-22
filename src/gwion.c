#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "instr.h"
#include "emit.h"
#include "gwion.h"
#include "arg.h"

ANN void gwion_init(const Gwion gwion, const Vector args) {
  gwion->vm = new_vm();
  gwion->emit = new_emitter();
  gwion->env = new_env();
  gwion->emit->env = gwion->env;
  gwion->vm->gwion = gwion;
  gwion->env->gwion = gwion;
  plug_ini(gwion->plug, args);
}

ANN void gwion_release(const Gwion gwion) {
  plug_end(gwion);
  free_env(gwion->env);
  free_emitter(gwion->emit);
  free_vm(gwion->vm);
  free_symbols();
}
