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

#include <string.h>
GWION_IMPORT(compile_file) {
  DECL_B(FILE *, file, = fopen("rm_me.gw", "w+"));
  if(fprintf(file, "1;") >= 0) {
    rewind(file);
    const bool ret = compile_file(gwi->gwion, __FILE__, file, NULL);
    fclose(file);
    return ret;
  }
  return false;
}
