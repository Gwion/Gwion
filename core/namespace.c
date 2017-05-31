#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "emit.h"
#include "func.h"

static VM* vm;
void release(M_Object o, VM_Shred shred);
void set_nspc_vm(VM* _vm) {
  vm = _vm;
}

Value namespace_lookup_value(NameSpace namespace, S_Symbol xid, int climb) {
  Value v = (Value)scope_lookup(namespace->value, xid, climb);
  if(climb > 0 && !v && namespace->parent)
    v = namespace_lookup_value(namespace->parent, xid, climb);
  return v;
}

Type namespace_lookup_type(NameSpace namespace, S_Symbol xid, int climb) {
  Type t = (Type)scope_lookup(namespace->type, xid, climb);
  if(climb > 0 && !t && namespace->parent)
    t = (Type)namespace_lookup_type(namespace->parent, xid, climb);
  return t;
}

Func namespace_lookup_func(NameSpace namespace, S_Symbol xid, int climb) {
  Func t = (Func)scope_lookup(namespace->func, xid, climb);
  if(climb > 0 && !t && namespace->parent)
    t = (Func)namespace_lookup_func(namespace->parent, xid, climb);
  return t;
}

void namespace_commit(NameSpace namespace) {
  scope_commit(namespace->value);
  scope_commit(namespace->func);
  scope_commit(namespace->type);
}

NameSpace new_NameSpace() {
  NameSpace a = calloc(1, sizeof(struct NameSpace_));
  a->name            = NULL;
  a->filename        = NULL;
  a->value           = new_Scope();
  a->type            = new_Scope();
  a->func            = new_Scope();
  a->label           = new_Map();
  a->class_data_size = 0;
  a->offset          = 0;
  a->class_data      = NULL;
  a->parent          = NULL;
  a->pre_ctor        = NULL;
  a->dtor            = NULL;
  a->obj_v_table     = new_vector();
  a->operator        = NULL;
  INIT_OO(a, e_namespace_obj);
  return a;
}

void free_NameSpace(NameSpace a) {
  m_uint i;
  Vector v = scope_get(a->value);
  for(i = 0; i < vector_size(v); i++) {
    Value value = (Value)vector_at(v, i);
    if(isa(value->m_type, &t_class) > 0)
      REM_REF(value->m_type)
      else if(isa(value->m_type, &t_object) > 0) {
        if(value->ptr || GET_FLAG(value, ae_value_static)) {
          Vector instr = new_vector();
          VM_Code code = new_VM_Code(instr, 0, 0, "", "");
          VM_Shred s = new_VM_Shred(code);
          s->vm_ref = vm;
          release(((M_Object)value->ptr), s);
          free_VM_Shred(s);
          free_vector(instr);
        }
      } else if(isa(value->m_type, &t_func_ptr) > 0) {
//  just catch func pointer
      } else if(isa(value->m_type, &t_function) > 0) {
        if(value->m_type != &t_function && strcmp(a->name, "global_context")) {
          free(value->name);
          free(value->m_type->name);
          REM_REF(value->m_type);
        }
      }
    REM_REF(value);

  }
  free_vector(v);
  free_Scope(a->value);


  v = scope_get(a->func);
  for(i = 0; i < vector_size(v); i++) {
    Func func = (Func)vector_at(v, i);
    REM_REF(func);
  }
  free_vector(v);
  free_Scope(a->func);

  v = scope_get(a->type);
  for(i =vector_size(v); i > 0; i--) { // changed /07/04/17 for reverse order.
    Type type = (Type)vector_at(v, i-1);
    REM_REF(type);
  }
  free_vector(v);
  free_Scope(a->type);

  for(i = 0; i < map_size(a->label); i++)
    free_Map((Map)map_at(a->label, i));
  free_Map(a->label);
  if(a->class_data)
    free(a->class_data);
  if(a->obj_v_table)
    free_vector(a->obj_v_table);
  if(a->pre_ctor)
    free_VM_Code(a->pre_ctor);
  if(a->dtor)
    free_VM_Code(a->dtor);
  if(a->operator)
    free_Operator_Map(a->operator);
  free(a);
}
