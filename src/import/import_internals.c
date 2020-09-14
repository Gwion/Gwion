#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "instr.h"
#include "object.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

void gwi_body(const Gwi gwi, const Ast body) {
  const Class_Def cdef = gwi->gwion->env->class_def->e->def;
  if(!cdef->body)
    cdef->body = body;
  else
    gwi->body->next = body;
  gwi->body = body;
}

ANN void gwi_reset(const Gwi gwi) {
  if(gwi->ck) {
    ck_clean(gwi);
    mp_free2(gwi->gwion->mp, sizeof(ImportCK), gwi->ck);
    gwi->ck = NULL;
  }
  env_reset(gwi->gwion->env);
}
