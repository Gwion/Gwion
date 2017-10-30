#include "type.h"
#include "import.h"
#include "object.h"
#include "emit.h"

static struct Type_ t_class_template = { "ClassTemplate", SZ_INT, &t_object };
static m_int o_map_key;
static m_int o_map_value;
#define MAP_KEY(a) *((M_Object*)(a->data + o_map_key))
#define MAP_VAL(a) *((M_Object*)(a->data + o_map_value))
static CTOR(class_template_ctor) {
printf("name %i\n", o->type_ref->info->parent);
printf("name %i\n", o->type_ref->info->offset);
printf("name %i\n", o->type_ref->info->offset);
printf("name %s\n", o->type_ref->name);
  char* name = strdup(o->type_ref->name);
//  name = strsep(&name, "@");
  char* tmp = strsep(&name, "@");
//  char* name2 = strsep(&name1, "@");
//printf("name1 %s %s\n", name1, name);
char* name1 = strsep(&name, "@");
printf("name1 %s %s\n", name1, name);
//exit(6);

  Type t1 = nspc_lookup_type1(o->type_ref->info->parent, insert_symbol(name1));
//  Type t1 = nspc_lookup_type0(o->type_ref->info->parent, insert_symbol(name1));

//  Type t1 = nspc_lookup_type0(shred->vm_ref->emit->env->curr, insert_symbol(name1));

//  name1 = strsep(name, "@");
  Type t2 = nspc_lookup_type0(shred->vm_ref->emit->env->curr, insert_symbol(name));
printf("name1 %lu %lu\n", t1->size, t2->size);
free(tmp);
  *(M_Object*)(o->data) = new_M_Array(t1->size, 0, t1->array_depth);
  *(M_Object*)(o->data + SZ_INT) = new_M_Array(t2->size, 0, t2->array_depth);
}

static MFUN(class_template_set) {

}

IMPORT
{
  const m_str list[2] = { "A", "B" };
  importer_tmpl_ini(importer, 2, list);
  CHECK_BB(importer_class_ini(importer, &t_class_template, class_template_ctor, NULL))
  importer_tmpl_end(importer);
  CHECK_BB(importer_item_ini(importer, "A[]", "key"))
  CHECK_BB((o_map_key = importer_item_end(importer, ae_flag_member | ae_flag_template, NULL)))
  CHECK_BB(importer_item_ini(importer, "B[]", "value"))
  CHECK_BB((o_map_value = importer_item_end(importer, ae_flag_member, NULL)))
  CHECK_BB(importer_class_end(importer))

importer_func_ini(importer, "B", "set", class_template_set);
importer_func_end(importer, ae_flag_member);
  return 1;
}
