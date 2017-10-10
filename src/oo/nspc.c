#include <stdlib.h>
#include <string.h>
#include "absyn.h"
#include "symbol.h"
#include "map.h"
#include "type.h"
#include "func.h"
#include "object.h"
#include "map_private.h"

#define OFFSET 2

vtype scope_lookup0(Scope scope, S_Symbol xid) {
  Map map = (Map)vector_back(&scope->vector);
  vtype ret = map_get(map, (vtype)xid);
  if(!ret && vector_back(&scope->vector) == vector_front(&scope->vector))
    ret = map_get(&scope->commit_map, (vtype)xid);
  return ret;
}

vtype scope_lookup1(Scope scope, S_Symbol xid) {
  m_uint i;
  vtype ret;
  for(i = vector_size(&scope->vector) + 1; --i;) {
    Map map = (Map)vector_at(&scope->vector, i - 1);
    if((ret = map_get(map, (vtype)xid)))
      break;
   }
  if(!ret)
    ret = map_get(&scope->commit_map, (vtype)xid);
  return ret;
}

vtype scope_lookup2(Scope scope, S_Symbol xid) {
  Map map = (Map)vector_front(&scope->vector);
  vtype ret = map_get(map, (vtype)xid);
  if(!ret)
    ret = map_get(&scope->commit_map, (vtype)xid);
  return ret;
}

vtype scope_lookup(Scope scope, S_Symbol xid, m_bool climb) {
  vtype ret = 0;
  if(climb == 0)
    ret = scope_lookup0(scope, xid);
  else if(climb > 0)
    ret = scope_lookup1(scope, xid);
  else
    ret = scope_lookup2(scope, xid);
  return ret;
}

void scope_add(Scope scope, S_Symbol xid, vtype value) {
  if(vector_front(&scope->vector) != vector_back(&scope->vector))
    map_set((Map)vector_back(&scope->vector), (vtype)xid, (vtype)value);
  else
    map_set(&scope->commit_map, (vtype)xid, (vtype)value);
}

void scope_commit(Scope scope) {
  map_commit((Map)vector_front(&scope->vector), &scope->commit_map);
  map_clear(&scope->commit_map);
}

void scope_push(Scope scope) {
  vector_add(&scope->vector, (vtype)new_map());
}

void scope_pop(Scope scope) {
  free_map((Map)vector_pop(&scope->vector));
}

void scope_init(Scope a) {
  vector_init((Vector)&a->commit_map);
  vector_init(&a->vector);
  scope_push(a);
}

void scope_release(Scope a) {
  free_map((Map)vector_front(&a->vector));
  vector_release(&a->vector);
  vector_release((Vector)&a->commit_map);
}

Vector scope_get(Scope s) {
  vtype i, j;
  Vector ret = new_vector();
  for(j = 0; j < vector_size(&s->vector); j++) {
    Map map = (Map)vector_at(&s->vector, j);
    for(i = 0; i < map->ptr[0]; i++)
      vector_add(ret, map->ptr[OFFSET + i * 2 + 1]);
  }
  for(i = 0; i < s->commit_map.ptr[0]; i++)
    vector_add(ret, (vtype)s->commit_map.ptr[OFFSET + i * 2 + 1]);
  return ret;
}

Value nspc_lookup_value0(Nspc nspc, S_Symbol xid) {
  return (Value)scope_lookup0(&nspc->value, xid);
}

Value nspc_lookup_value1(Nspc nspc, S_Symbol xid) {
  Value v = (Value)scope_lookup1(&nspc->value, xid);
  if(!v && nspc->parent)
    v = nspc_lookup_value1(nspc->parent, xid);
  return v;
}

Value nspc_lookup_value2(Nspc nspc, S_Symbol xid) {
  return (Value)scope_lookup2(&nspc->value, xid);
}

void  nspc_add_value(Nspc nspc, S_Symbol xid, Value value) {
  scope_add(&nspc->value, xid, (vtype)value);
}
void nspc_push_value(Nspc nspc) {
  scope_push(&nspc->value);
}

void nspc_pop_value(Nspc nspc) {
  scope_pop(&nspc->value);
}

Func nspc_lookup_func0(Nspc nspc, S_Symbol xid) {
  return (Func)scope_lookup0(&nspc->func, xid);
}

Func nspc_lookup_func1(Nspc nspc, S_Symbol xid) {
  Func t = (Func)scope_lookup1(&nspc->func, xid);
  if(!t && nspc->parent)
    t = (Func)nspc_lookup_func1(nspc->parent, xid);
  return t;
}

Func nspc_lookup_func2(Nspc nspc, S_Symbol xid) {
  return (Func)scope_lookup2(&nspc->func, xid);
}

void nspc_add_func(Nspc nspc, S_Symbol xid, Func value) {
  scope_add(&nspc->func, xid, (vtype)value);
}

Type nspc_lookup_type0(Nspc nspc, S_Symbol xid) {
  return (Type)scope_lookup0(&nspc->type, xid);
}

Type nspc_lookup_type1(Nspc  nspc, S_Symbol xid) {
  Type t = (Type)scope_lookup1(&nspc->type, xid);
  if(!t && nspc->parent)
    t = (Type)nspc_lookup_type1(nspc->parent, xid);
  return t;
}

Type nspc_lookup_type2(Nspc nspc, S_Symbol xid) {
  return (Type)scope_lookup2(&nspc->type, xid);
}

void nspc_add_type(Nspc nspc, S_Symbol xid, Type value) {
  scope_add(&nspc->type, xid, (vtype)value);
}
void nspc_push_type(Nspc nspc) {
  scope_push(&nspc->type);
}
void nspc_pop_type(Nspc nspc) {
  scope_pop(&nspc->type);
}

void nspc_commit(Nspc nspc) {
  scope_commit(&nspc->value);
  scope_commit(&nspc->func);
  scope_commit(&nspc->type);
}

Vector nspc_get_value(Nspc nspc) {
  return scope_get(&nspc->value);
}

Nspc new_nspc(m_str name, m_str filename) {
  Nspc a = calloc(1, sizeof(struct Nspc_));
  a->name            = name;
  a->filename        = filename;
  vector_init(&a->obj_v_table);
  scope_init(&a->value);
  scope_init(&a->type);
  scope_init(&a->func);
  INIT_OO(a, e_nspc_obj);
  return a;
}

static void nspc_release_object(Nspc a, Value value) {
  if(value->ptr || (GET_FLAG(value, ae_flag_static) && a->class_data)) {
    VM_Code code = new_vm_code(NULL, 0, 0, "in nspc dtor", "");
    VM_Shred s = new_vm_shred(code);
    M_Object obj = value->ptr ? (M_Object)value->ptr :
        *(M_Object*)(a->class_data + value->offset);
    s->vm_ref = vm;
    release(obj, s);
    free_vm_shred(s);
  }
  if(value->m_type->array_depth)
    REM_REF(value->m_type);
}

static void free_nspc_value_func(Func f) {
  while(f) {
    Func tmp = f->next;
    free(f);
    f = tmp;
  }
}

static void free_nspc_value(Nspc a) {
  m_uint i;
  Vector v = scope_get(&a->value);
  for(i = vector_size(v) + 1; --i;) {
    Value value = (Value)vector_at(v, i - 1);

    if(value->m_type) {
      if(isa(value->m_type, &t_class) > 0)
        REM_REF(value->m_type)
      else if(isa(value->m_type, &t_object) > 0)
        nspc_release_object(a, value);
      else if(isa(value->m_type, &t_func_ptr) > 0) {
        free_nspc_value_func(value->func_ref);
      } else if(isa(value->m_type, &t_function) > 0) {
        if(GET_FLAG(value, ae_flag_template))
          REM_REF(value->func_ref)
        else
          REM_REF(value->m_type)
        }
    }
    REM_REF(value);
  }
  free_vector(v);
  scope_release(&a->value);
}

void free_nspc(Nspc a) {
  m_uint i;
  Vector v;

  free_nspc_value(a);
  v = scope_get(&a->func);
  for(i = vector_size(v) + 1; --i;) {
    Func func = (Func)vector_at(v, i - 1);
    REM_REF(func);
  }
  free_vector(v);
  scope_release(&a->func);

  v = scope_get(&a->type);
  for(i = vector_size(v); i > 0; i--) {
    Type type = (Type)vector_at(v, i - 1);
    REM_REF(type);
  }
  free_vector(v);
  scope_release(&a->type);

  if(a->class_data)
    free(a->class_data);
  if(a->obj_v_table.ptr)
    vector_release(&a->obj_v_table);
  if(a->pre_ctor)
    REM_REF(a->pre_ctor);
  if(a->dtor)
    REM_REF(a->dtor);
  if(a->op_map.ptr)
    free_op_map(&a->op_map);
  free(a);
}
