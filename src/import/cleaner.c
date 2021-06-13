/** @file: cleaner.c                                   *
 *  \brief: functions to clean import module           *
 *                                                     */
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "instr.h"
#include "emit.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

ANN m_bool ck_ini(const Gwi gwi, const enum importck_type t) {
  if (gwi->ck) // TODO: improve error message
    GWI_ERR_B(_("already importing"))
  gwi->ck       = mp_calloc2(gwi->gwion->mp, sizeof(ImportCK));
  gwi->ck->type = t;
  return GW_OK;
}

ANN m_bool ck_ok(const Gwi gwi, const enum importck_type t) {
  if (!gwi->ck) GWI_ERR_B(_("import not started"))
  if (gwi->ck->type == t) return GW_OK;
  // TODO: improve error message
  GWI_ERR_B(_("already importing"))
}

ANN void ck_end(const Gwi gwi) {
  mp_free2(gwi->gwion->mp, sizeof(ImportCK), gwi->ck);
  gwi->ck = NULL;
}

typedef void (*cleaner)(MemPool, ImportCK *);
static cleaner cleaners[] = {NULL,          ck_clean_udef, ck_clean_tdef,
                             NULL, //  ck_clean_oper,
                             ck_clean_item, ck_clean_fdef};

ANN void ck_clean(const Gwi gwi) {
  const cleaner clean = cleaners[gwi->ck->type];
  if (clean) clean(gwi->gwion->mp, gwi->ck);
  memset(gwi->ck, 0, sizeof(ImportCK));
}
