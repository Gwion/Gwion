#include <stdlib.h>
#include "map.h"
#include "emit.h"

static VM* vm;

void set_nspc_vm(VM* _vm)
{ vm = _vm; }
Value namespace_lookup_value(NameSpace namespace, S_Symbol xid, int climb)
{
  Value v = scope_lookup(namespace->value, xid, climb);
  if( climb > 0 && !v && namespace->parent )
    v = namespace_lookup_value(namespace->parent, xid, climb);
  return v;
}

void namespace_add_value(NameSpace namespace, S_Symbol xid, Value value)
{
  scope_add(namespace->value, xid, value);
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
  if( climb > 0 && !t && namespace->parent )
    t = (Type)namespace_lookup_type(namespace->parent, xid, climb);
  return t;
}

void namespace_add_type(NameSpace namespace, S_Symbol xid, Type value)
{
  scope_add(namespace->type, xid, value);
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
  scope_add(namespace->func, xid, value);
}

/*
void namespace_push_func(NameSpace namespace)
{
  scope_push(namespace->func);
}

void namespace_pop_func(NameSpace namespace)
{
  scope_pop(namespace->func);
}
*/
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
#include "func.h"
void free_NameSpace(NameSpace a)
{
  m_uint i;
  Vector v = scope_get(a->value);
Vector instr = new_Vector();
VM_Code code = new_VM_Code(instr, 0, 0, "", "");
VM_Shred s = new_VM_Shred(code);
s->vm_ref = vm;

  for(i = 0; i < vector_size(v); i++)
  {
    Value value = (Value)vector_at(v, i);
    if(isa(value->m_type, &t_class) > 0)
{
		free(value->m_type->obj);
		free(value->m_type);

}
    else if(isa(value->m_type, &t_object) > 0)
{
//exit(3);
if(value->ptr)
  release(((M_Object)value->ptr), s);

//  free_UGen(((M_Object)value->ptr)->ugen);
//  free(value->ptr);
//  release(value->ptr, NULL);
//  break;

}
      rem_ref(value->obj, value);
}

  free_Vector(v);
  free_Scope(a->value);
free_VM_Shred(s);
//free_VM_Code(code);
  v = scope_get(a->type);
  for(i = 0; i < vector_size(v); i++)
  {
    Type type = (Type)vector_at(v, i);

  if(type->info)
  {
    free_Operator_Map(type->info->operator);
    free_Vector(type->info->obj_v_table);
    free_Scope(type->info->value);
    free_Scope(type->info->type);
//free_Scope(type->info->func);
    if(type->info->pre_ctor)
      free_VM_Code(type->info->pre_ctor);
    if(type->info->dtor)
      free_VM_Code(type->info->dtor);

  Vector w = scope_get(type->info->func);
  m_uint j;
  for(j = 0; j < vector_size(w); j++)
  {
    Func func = (Func)vector_at(w, j);
    //free(func);
    //rem_ref(func->value_ref->obj, func->value_ref);
    free_VM_Code(func->code);
    free(func->value_ref->m_type->name);
    free(func->value_ref->m_type);
    free(func->value_ref->obj);
    free_Value(func->value_ref);

    if(func->def->ret_type->array_type)
    {
      free(func->def->ret_type->obj);
      free(func->def->ret_type);
    }
    if(func->def->arg_list)
      free_Arg_List(func->def->arg_list);
    free_Type_Decl(func->def->type_decl);
    free(func->def);
    free(func->name);
    free(func->obj);
    free(func);
  }
  free_Vector(w);
  free_Scope(type->info->func);
  free_Map(type->info->label);
  free(type->info->obj);
  free(type->info);
  }
  rem_ref(type->obj, type); // no op for now
  }
  free_Vector(v);
  free_Scope(a->type);

  v = scope_get(a->func);
  for(i = 0; i < vector_size(v); i++)
  {
    Func func = (Func)vector_at(v, i);
    rem_ref(func->obj, func);
  }
  free_Vector(v);
  free_Scope(a->func);

  free_Map(a->label);
  if(a->class_data)
	  free(a->class_data);
  if(a->obj_v_table)
    free_Vector(a->obj_v_table);
  if(a->pre_ctor)
    free_VM_Code(a->pre_ctor);
  if(a->dtor)
    free_VM_Code(a->dtor);
  free_Operator_Map(a->operator);
  free(a);
}
/*
Vector emit_lookup_label(Emitter emit, S_Symbol xid, int climb)
{ return (Vector)scope_lookup(emit->labels, xid, climb); }
void emit_add_label(Emitter emit, S_Symbol xid, void* value)
{ scope_add(emit->labels, xid, value); }
void emit_push_label(Emitter emit)
{ scope_push(emit->labels); }
void emit_pop_label(Emitter emit)
{ scope_pop(emit->labels); }
*/
