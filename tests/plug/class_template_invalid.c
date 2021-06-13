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

static m_int o_map_key;
static m_int o_map_value;
#define MAP_KEY(a) *((M_Object *)(a->data + o_map_key))
#define MAP_VAL(a) *((M_Object *)(a->data + o_map_value))

GWION_IMPORT(class_template) {
  GWI_OB(gwi_class_ini(gwi, "ClassTemplate:[A,B]", NULL))
  GWI_BB(gwi_item_ini(gwi, "A[]", "key"))
  GWI_BB((o_map_key = gwi_item_end(gwi, ae_flag_none, obj, NULL)))
  GWI_BB(gwi_item_ini(gwi, "B[]", "value"))
  GWI_BB((o_map_value = gwi_item_end(gwi, ae_flag_none, obj, NULL)))
  GWI_BB(gwi_func_ini(gwi, "int", "test:[C,D]"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))
  GWI_BB(gwi_item_ini(gwi, "ClassTemplate:[:[int]Ptr,int[]]", "testObject"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, obj, NULL))
  return GW_OK;
}
