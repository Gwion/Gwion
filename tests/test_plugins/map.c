#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "instr.h"
#include "import.h"
#include "emit.h"

#include "func.h"
#include "type.h"

#define MAP_INFO(o) (*(struct map_info**)(o->data + t_array.info->offset))
#define MAP_KEY(p) ((char*)(p->data))
#define MAP_VAL(p, o) ((char*)(p->data + MAP_INFO(o)->key_size))

typedef m_bool (*f_cmp)(const char* restrict , const char* restrict , const m_uint);

struct map_info {
  Type t;
  m_uint key_size;
  m_uint val_size;
  f_cmp cmp;
};

static struct Type_ t_map = { "Map", SZ_INT };

static m_bool string_cmp(const char *restrict a, const char*restrict b, const m_uint size) {
  M_Object o = (M_Object)b;
  if(!o && !a)
    return 1;
  return o ? !strcmp(a, STRING(o)) : 1;
}

static m_bool cmp(const char *restrict a, const char*restrict b, const m_uint size) {
  return !memcmp(a, b, size);
}

static CTOR(map_ctor) {
  struct map_info* info = malloc(sizeof(struct map_info));
  info->t = array_base(o->type_ref->parent);
  Env env = shred->vm_ref->emit->env;
  Nspc curr = env->curr;
printf("type ! %p\n", info->t);
printf("type ! %s\n", o->type_ref->name);
printf("type ! %p\n", o->type_ref->parent->d.base_type);
  m_str   key_name = get_type_name(info->t->name, 1);
  m_uint depth;
  Type_Decl* key_decl = str2decl(env, key_name, &depth);
  Type key_type = type_decl_resolve(env, key_decl);
  free_type_decl(key_decl);
  info->key_size = key_type->size;
  m_str   val_name = get_type_name(info->t->name, 2);
  Type_Decl* val_decl = str2decl(env, val_name, &depth);
  Type val_type = type_decl_resolve(env, val_decl);
  free_type_decl(val_decl);
  info->val_size = val_type->size;
  MAP_INFO(o) = info;
  env->curr = curr;
  if(isa(key_type, &t_string) > 0)
    MAP_INFO(o)->cmp = string_cmp;
  else
    MAP_INFO(o)->cmp = cmp;
ADD_REF(o->type_ref);
ADD_REF(o->type_ref->parent);
}

static DTOR(map_dtor) {
  free(*(struct map_info**)(o->data + t_array.info->offset));
}


static MFUN(gw_map_get) {
  M_Vector v = ARRAY(o);
  m_uint size = m_vector_size(v);
  for(m_uint i = 0; i < size; i++) {
    M_Object p;
    m_vector_get(v, i, &p);
    if(MAP_INFO(o)->cmp(MAP_KEY(p), MEM(SZ_INT), MAP_INFO(o)->key_size)) {
      memcpy(RETURN, MAP_VAL(p, o), MAP_INFO(o)->key_size);
      return;
    }
  }
  memset(RETURN, 0, MAP_INFO(o)->key_size);
}

static MFUN(gw_map_set) {
  M_Vector v = ARRAY(o);
  m_uint size = m_vector_size(v);
  memcpy(RETURN, MEM(SZ_INT + MAP_INFO(o)->key_size), MAP_INFO(o)->val_size);
  for(m_uint i = 0; i < size; i++) {
    M_Object p;
    m_vector_get(v, i, &p);
    if(MAP_INFO(o)->cmp(MAP_KEY(p), MEM(SZ_INT), MAP_INFO(o)->key_size)) {
      memcpy(MAP_VAL(p, o), MEM(SZ_INT + MAP_INFO(o)->key_size), MAP_INFO(o)->key_size);
      return;
    }
  }
  M_Object pair = new_M_Object(NULL);
  initialize_object(pair, MAP_INFO(o)->t);
  memcpy(pair->data, MEM(SZ_INT), MAP_INFO(o)->key_size + MAP_INFO(o)->val_size);
  m_vector_add(v, (char*)&pair);
printf("size %lu\n", size);
}

//m_bool import_map(Gwi gwi) {
IMPORT {
  m_str types[] = { "A", "B" };
  CHECK_BB(gwi_tmpl_ini(gwi, 2, types))
  CHECK_BB(gwi_class_ini(gwi, &t_map, map_ctor, map_dtor))
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
  CHECK_BB(gwi_func_ini(gwi, "B", "set", gw_map_set))
  CHECK_BB(gwi_func_arg(gwi, "A", "key"))
  CHECK_BB(gwi_func_arg(gwi, "B", "val"))
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_func_ini(gwi, "B", "get", gw_map_get))
  CHECK_BB(gwi_func_arg(gwi, "A", "key"))
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
