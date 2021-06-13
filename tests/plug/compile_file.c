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
  DECL_OB(FILE *, file, = fopen("rm_me.gw", "w+"));
  fprintf(file, "1;");
  rewind(file);
  const m_bool ret =
      compile_file(gwi->gwion, __FILE__, file) ? GW_OK : GW_ERROR;
  fclose(file);
  return ret;
}
