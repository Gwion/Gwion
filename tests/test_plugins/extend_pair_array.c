#include <stdlib.h>
#include "type.h"
#include "instr.h"
#include "import.h"
#include "emit.h"

#include "func.h"

#define MAP_INFO(o) *(struct map_info**)(o->data + t_array.info->offset)
struct map_info {
  Type t;
  m_uint key_size;
  m_uint val_size;
};

static struct Type_ t_pair_array_ext = { "PairArrayExt", SZ_INT };

static MFUN(gw_pair_set) {
  m_uint i, size = m_vector_size(ARRAY(o));
//  for(i = size + 1; --i) {
//    M_Object pair = (M_Object)i_vector_at(i);
}

static CTOR(pair_array_ext_ctor) {

puts("!!! builtin template ctor !!!");
  struct map_info* info = malloc(sizeof(struct map_info));
  info->t = array_base(o->type_ref);
// now to get inner type sizes
// use str2list ?
// or str2decl
Env env = shred->vm_ref->emit->env;
Nspc curr = env->curr;

m_str   key_name = get_type_name(info->t->name, 1);
m_uint depth;
Type_Decl* key_decl = str2decl(env, key_name, &depth);
Type key_type = type_decl_resolve(env, key_decl);
//ID_List key_list = new_id_list(insert_symbol(key_name), 0);
//Type    key_type = find_type(shred->vm_ref->emit->env, key_list);
//free_id_list(key_list);
free_type_decl(key_decl);
info->key_size = key_type->size;
m_str   val_name = get_type_name(info->t->name, 2);

Type_Decl* val_decl = str2decl(env, key_name, &depth);
Type val_type = type_decl_resolve(env, val_decl);
free_type_decl(val_decl);
info->val_size = val_type->size;
  MAP_INFO(o) = info;

env->curr = curr;
}

static DTOR(pair_array_ext_dtor) {
  free(*(struct map_info**)(o->data + t_array.info->offset));
}
IMPORT {
  m_str types[] = { "A", "B" };
  CHECK_BB(gwi_tmpl_ini(gwi, 2, types))
  CHECK_BB(gwi_class_ini(gwi, &t_pair_array_ext, pair_array_ext_ctor, pair_array_ext_dtor))
  CHECK_BB(gwi_tmpl_end(gwi))
  Type_Decl* td  = new_type_decl(new_id_list(insert_symbol("Pair"), 0), 0, 0);
  Type_Decl* td0 = new_type_decl(new_id_list(insert_symbol("A"), 0), 0, 0);
  Type_Decl* td1 = new_type_decl(new_id_list(insert_symbol("B"), 0), 0, 0);
  Type_List tl1  = new_type_list(td1, NULL, 0);
  Type_List tl0  = new_type_list(td0, tl1, 0);
  td->types = tl0;
  Exp e = new_exp_prim_int(0, 0);
  Array_Sub array = new_array_sub(e, 0);
  add_type_decl_array(td, array, 0);
  CHECK_BB(gwi_class_ext(gwi, td))
  CHECK_BB(gwi_item_ini(gwi, "int", "@map_info"))
  CHECK_BB(gwi_item_end(gwi, 0, NULL));
  CHECK_BB(gwi_func_ini(gwi, "B", "set", gw_pair_set))
  CHECK_BB(gwi_func_arg(gwi, "A", "key"))
  CHECK_BB(gwi_func_arg(gwi, "B", "val"))
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
