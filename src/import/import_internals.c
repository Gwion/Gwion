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
    cdef->body = new_sectionlist(gwi->gwion->mp, 1);
    sectionlist_set(cdef->body, 0, *section);
  } else {
    sectionlist_add(gwi->gwion->mp, &cdef->body, (*section));
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

static int doc2console(Gwion gwion NUSED, const char *str) {
  return fprintf(stdout, "%s", str);
}

static int (*doc_func)(Gwion, const char *str) = doc2console;
void set_doc_func(int (*f)(Gwion, const char*)) {
  doc_func = f;
}

ANN static bool run_with_doc(const Gwi gwi, bool (*f)(const Gwi)) {
  struct GwfmtState ls     = {.builtin = true, .nindent = 4};
  gwfmt_state_init(&ls);
  text_init(&ls.text, gwi->gwion->mp);
  Gwfmt gwfmter = {.mp = gwi->gwion->mp, .ls = &ls, .st = gwi->gwion->st };
  gwfmt_indent(&gwfmter);
  gwfmt_util(&gwfmter, "{-}#!+ %s{0}\n", gwi->gwion->env->name);
  gwi->gwfmt = &gwfmter;
  const bool ret = f(gwi);
  doc_func(gwi->gwion, ls.text.str);
  free_mstr(gwi->gwion->mp, ls.text.str);
  return ret;
}

ANN bool gwi_run(const Gwion gwion, bool (*f)(const Gwi)) {
  OperCK       oper = {};
  struct Gwi_  gwi  = {.gwion = gwion, .oper = &oper};
  const bool ret  = !gwion->data->cdoc ? f(&gwi) : run_with_doc(&gwi, f);
  if (!ret) gwi_reset(&gwi);
  return ret;
}

