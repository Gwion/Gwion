#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

void gwi_body(const Gwi gwi, const Section *section) {
  const Class_Def cdef = gwi->gwion->env->class_def->info->cdef;
  if (!cdef->body) {
    cdef->body = new_mp_vector(gwi->gwion->mp, Section, 1);
    mp_vector_set(cdef->body, Section, 0, *section);
  } else {
    mp_vector_add(gwi->gwion->mp, &cdef->body, Section, (*section));
  }
}

ANN void gwi_reset(const Gwi gwi) {
  if (gwi->ck) {
    ck_clean(gwi);
    mp_free2(gwi->gwion->mp, sizeof(ImportCK), gwi->ck);
    gwi->ck = NULL;
  }
  env_reset(gwi->gwion->env);
}

ANN static m_bool run_with_doc(const Gwi gwi, m_bool (*f)(const Gwi)) {
  struct LintState ls     = {.builtin = true};
  Lint             linter = {.mp = gwi->gwion->mp, .ls = &ls};
  lint(&linter, "{-}#!+ %s{0}\n", gwi->gwion->env->name);
  gwi->lint = &linter;
  return f(gwi);
}

ANN m_bool gwi_run(const Gwion gwion, m_bool (*f)(const Gwi)) {
  const m_str name = gwion->env->name;
  //  const Context ctx = gwion->env->context;
  //  gwion->env->context = NULL;
  OperCK       oper = {};
  struct Gwi_  gwi  = {.gwion = gwion, .oper = &oper};
  const m_bool ret  = !gwion->data->cdoc ? f(&gwi) : run_with_doc(&gwi, f);
  if (ret < 0) gwi_reset(&gwi);
  gwion->env->name = name;
  //  gwion->env->context = ctx;
  return ret;
}

