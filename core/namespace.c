#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "emit.h"
#include "func.h"

static VM* vm;
void release(M_Object o, VM_Shred shred);
void free_Arg_List(Arg_List list);
void free_Type_Decl(Type_Decl* type_decl);
void set_nspc_vm(VM* _vm)
{
  vm = _vm;
}

Value namespace_lookup_value(NameSpace namespace, S_Symbol xid, int climb)
{
  Value v = (Value)scope_lookup(namespace->value, xid, climb);
  if( climb > 0 && !v && namespace->parent)
    v = namespace_lookup_value(namespace->parent, xid, climb);
  return v;
}

void namespace_add_value(NameSpace namespace, S_Symbol xid, Value value)
{
  scope_add(namespace->value, xid, (vtype)value);
}

void namespace_push_value(NameSpace namespace)
{
  scope_push(namespace->value);
}

void namespace_pop_value(NameSpace namespace)
{
  scope_pop(namespace->value);
}

Type namespace_lookup_type(NameSpace namespace, S_Symbol xid, int climb)
{
  Type t = (Type)scope_lookup(namespace->type, xid, climb);
  if( climb > 0 && !t && namespace->parent)
    t = (Type)namespace_lookup_type(namespace->parent, xid, climb);
  return t;
}

void namespace_add_type(NameSpace namespace, S_Symbol xid, Type value)
{
  scope_add(namespace->type, xid, (vtype)value);
}

void namespace_push_type(NameSpace namespace)
{
  scope_push(namespace->type);
}

void namespace_pop_type(NameSpace namespace)
{
  scope_pop(namespace->type);
}

Func namespace_lookup_func(NameSpace namespace, S_Symbol xid, int climb)
{
  Func t = (Func)scope_lookup(namespace->func, xid, climb);
  if( climb > 0 && !t && namespace->parent)
    t = (Func)namespace_lookup_func(namespace->parent, xid, climb);
  return t;
}

void namespace_add_func(NameSpace namespace, S_Symbol xid, Func value)
{
  scope_add(namespace->func, xid, (vtype)value);
}

void namespace_commit(NameSpace namespace)
{
  scope_commit(namespace->value);
  scope_commit(namespace->func);
  scope_commit(namespace->type);
}

void namespace_rollback(NameSpace namespace)
{
  scope_rollback(namespace->value, NULL);
  scope_rollback(namespace->func,  NULL);
  scope_rollback(namespace->type,  NULL);
}

NameSpace new_NameSpace()
{
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
  a->obj             = new_VM_Object(e_namespace_obj);
  a->obj_v_table     = new_Vector();
  a->operator        = new_Operator_Map();
  return a;
}

void free_NameSpace(NameSpace a)
{
  m_uint i;
  Vector v = scope_get(a->value);
  for(i = 0; i < vector_size(v); i++) {
    Value value = (Value)vector_at(v, i);
    if(isa(value->m_type, &t_class) > 0) {
      rem_ref(value->m_type->obj, value->m_type);
    }
    else if(isa(value->m_type, &t_object) > 0) {
      if(value->ptr) {
        Vector instr = new_Vector();
        VM_Code code = new_VM_Code(instr, 0, 0, "", "");
        VM_Shred s = new_VM_Shred(code);
        s->vm_ref = vm;
        release(((M_Object)value->ptr), s);
        free_VM_Shred(s);
        free_Vector(instr);
      }
      if(value->m_type->array_type) {
        free(value->m_type->obj);
        free(value->m_type);
      }
      if(value->is_static) {
        Vector instr = new_Vector();
        VM_Code code = new_VM_Code(instr, 0, 0, "", "");
        VM_Shred s = new_VM_Shred(code);
        s->vm_ref = vm;
        release(*(M_Object*)(a->class_data + value->offset), s);
        free_VM_Shred(s);
        free_Vector(instr);
      }
    } else if(isa(value->m_type, &t_func_ptr) > 0) {
//  just catch func pointer
    } else if(isa(value->m_type, &t_function) > 0) {
      if(value->func_ref && value->func_ref->def && value->func_ref->def->is_template) {
/*
	if(value->func_ref->def->spec == ae_func_spec_dtor) {
        free(value->func_ref->def);
        value->func_ref->def = NULL;
        free_VM_Code(value->func_ref->code);
//value->owner_class->info->dtor = NULL;
continue;
}
*/
      }
      if(value->m_type != &t_function && strcmp(a->name, "global_context")) {
        free(value->name);
        free(value->m_type->name);
        rem_ref(value->m_type->obj, value->m_type);
      }
    }
    rem_ref(value->obj, value);

  }
  free_Vector(v);
  free_Scope(a->value);


  v = scope_get(a->func);
  for(i = 0; i < vector_size(v); i++) {
    Func func = (Func)vector_at(v, i);
    rem_ref(func->obj, func);
  }
  free_Vector(v);
  free_Scope(a->func);

  v = scope_get(a->type);
  for(i = 0; i < vector_size(v); i++) {
    Type type = (Type)vector_at(v, i);
      rem_ref(type->obj, type);
  }
  free_Vector(v);
  free_Scope(a->type);

  for(i = 0; i < map_size(a->label); i++)
    free_Map((Map)map_at(a->label, i));
  free_Map(a->label);
  if(a->class_data)
    free(a->class_data);
  if(a->obj_v_table)
    free_Vector(a->obj_v_table);
  if(a->pre_ctor)
    free_VM_Code(a->pre_ctor);
//  if(a->dtor && strcmp(a->dtor->filename, "[in code dtor exec]"))
  if(a->dtor)
    free_VM_Code(a->dtor);
  free_Operator_Map(a->operator);
  free(a);
}
