#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "object.h"
#include "instr.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

static m_int o_map_key;
static m_int o_map_value;
#define MAP_KEY(a) *((M_Object*)(a->data + o_map_key))
#define MAP_VAL(a) *((M_Object*)(a->data + o_map_value))

GWION_IMPORT(class_template) {
  GWI_OB(gwi_class_ini(gwi, "<~A,B~>ClassTemplate", NULL))
    GWI_BB(gwi_item_ini(gwi, "A[]", "key"))
    GWI_BB((o_map_key = gwi_item_end(gwi, ae_flag_member | ae_flag_template, NULL)))
    GWI_BB(gwi_item_ini(gwi, "B[]", "value"))
    GWI_BB((o_map_value = gwi_item_end(gwi, ae_flag_member, NULL)))
   GWI_BB(gwi_func_ini(gwi, "int", "<~C,D~>test"))
   GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))
  GWI_BB(gwi_item_ini(gwi, "<~Ptr<~int~>,int[]~>ClassTemplate", "testObject"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, NULL))
  return GW_OK;
}
