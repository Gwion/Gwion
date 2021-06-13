#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "compile.h"

#define _XOPEN_SOURCE 500
#include <string.h>
GWION_IMPORT(compile_string) {
  return compile_string(gwi->gwion, __FILE__, "1;") ? GW_OK : GW_ERROR;
}
