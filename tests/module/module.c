#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"

GWMODINI(dummy_module) {
  puts(__func__);
  return NULL;
}
GWMODEND(dummy_module) { puts(__func__); }
