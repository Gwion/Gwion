#include <stdlib.h>
#include "map.h"
#include "emit.h"

Value namespace_lookup_value(NameSpace namespace, S_Symbol xid, int climb)
{
  Value v = scope_lookup(namespace->value, xid, climb);
  if( climb > 0 && !v && namespace->parent )
    v = namespace_lookup_value(namespace->parent, xid, climb);
  return v;
}

void namespace_add_value(NameSpace namespace, S_Symbol xid, Value value)
{
/*  printf("namespace->name %s\n", namespace->name);*/
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
  unsigned int i;
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
  free_Scope(a->value);
  free_Scope(a->type);
  free_Scope(a->func);
	free_Map(a->label);
  free(a->class_data);
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
