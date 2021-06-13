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
static CTOR(class_template_ctor) {
  /*char* name = strdup(o->type_ref->name);*/
  /*char* tmp = strsep(&name, "@");*/
  /*char* name1 = strsep(&name, "@");*/
  /*Type t1 = nspc_lookup_type1(o->type_ref->info->parent,
   * insert_symbol(name1));*/
  /*Type t2 = nspc_lookup_type0(shred->vm->emit->env->curr,
   * insert_symbol(name));*/
  /*free(tmp);*/
  /**(M_Object*)(o->data) = new_array(t1->size, 0, t1->array_depth);*/
  /**(M_Object*)(o->data + SZ_INT) = new_array(t2->size, 0, t2->array_depth);*/
}

GWION_IMPORT(class_template) {
  GWI_OB(gwi_class_ini(gwi, ":[A,B]ClassTemplate", NULL))
  gwi_class_xtor(gwi, class_template_ctor, NULL);
  GWI_BB(gwi_item_ini(gwi, "A[]", "key"))
  GWI_BB((o_map_key = gwi_item_end(gwi, ae_flag_none, obj, NULL)))
  GWI_BB(gwi_item_ini(gwi, "B[]", "value"))
  GWI_BB((o_map_value = gwi_item_end(gwi, ae_flag_none, obj, NULL)))
  GWI_BB(gwi_func_ini(gwi, "int", ":[C,D]test"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))
  GWI_BB(gwi_item_ini(gwi, ":[:[int]Ptr,int[]]ClassTemplate", "testObject"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, obj, NULL))
  return GW_OK;
}
