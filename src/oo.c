#include <stdlib.h>
#include <stdio.h>
#include "map.h"
#include "context.h"
#include "func.h"
#include "oo.h"
#include "emit.h"
#include "object.h"

Value nspc_lookup_value(Nspc nspc, S_Symbol xid, m_bool climb) {
  Value v = (Value)scope_lookup(&nspc->value, xid, climb);
  if(climb > 0 && !v && nspc->parent)
    v = nspc_lookup_value(nspc->parent, xid, climb);
  return v;
}

Type nspc_lookup_type(Nspc nspc, S_Symbol xid, m_bool climb) {
  Type t = (Type)scope_lookup(&nspc->type, xid, climb);
  if(climb > 0 && !t && nspc->parent)
    t = (Type)nspc_lookup_type(nspc->parent, xid, climb);
  return t;
}

Func nspc_lookup_func(Nspc nspc, S_Symbol xid, m_bool climb) {
  Func t = (Func)scope_lookup(&nspc->func, xid, climb);
  if(climb > 0 && !t && nspc->parent)
    t = (Func)nspc_lookup_func(nspc->parent, xid, climb);
  return t;
}

void nspc_commit(Nspc nspc) {
  scope_commit(&nspc->value);
  scope_commit(&nspc->func);
  scope_commit(&nspc->type);
}

Nspc new_nspc(m_str name, m_str filename) {
  Nspc a = calloc(1, sizeof(struct Nspc_));
  a->name            = name;
  a->filename        = filename;
  a->class_data_size = 0;
  a->offset          = 0;
  a->class_data      = NULL;
  a->parent          = NULL;
  a->pre_ctor        = NULL;
  a->dtor            = NULL;
  vector_init(&a->obj_v_table);
  scope_init(&a->value);
  scope_init(&a->type);
  scope_init(&a->func);
  INIT_OO(a, e_nspc_obj);
  return a;
}

void free_nspc(Nspc a) {
  m_uint i;
  Vector v = scope_get(&a->value);
  for(i = vector_size(v) + 1; --i;) {
    Value value = (Value)vector_at(v, i - 1);

    if(value->m_type) {
      if(isa(value->m_type, &t_class) > 0)
        REM_REF(value->m_type)
      else if(isa(value->m_type, &t_object) > 0) {
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
      } else if(isa(value->m_type, &t_func_ptr) > 0) {
        Func f = value->func_ref;
        while(f) {
          Func tmp = f->next;
          free(f);
          f = tmp;
        }
      } else if(isa(value->m_type, &t_function) > 0) {
        if(value->m_type != &t_function && GET_FLAG(value, ae_flag_builtin))
          REM_REF(value->m_type)
        else if(GET_FLAG(value, ae_flag_template))
          REM_REF(value->func_ref)
        else
          REM_REF(value->m_type)
        }
    }
    REM_REF(value);
  }
  free_vector(v);
  scope_release(&a->value);


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

Value new_value(Type type, m_str name) {
  Value a               = (Value)calloc(1, sizeof(struct Value_));
  a->m_type             = type;
  a->name               = name;
  INIT_OO(a, e_value_obj);
  return a;
}

static void free_value(Value a) {
  if(a->ptr) {
    if(isprim(a->m_type) > 0 && !GET_FLAG(a, ae_flag_enum))
      free(a->ptr);
  }
  free(a);
}

Func new_func(m_str name, Func_Def def) {
  Func func = calloc(1, sizeof(struct Func_));
  func->name = name;
  func->def = def;
  INIT_OO(func, e_func_obj);
  return func;
}

static void free_func(Func a) {
  if(a->code) {
    if(a->def && !GET_FLAG(a, ae_flag_template)) {
      if(!GET_FLAG(a->def, ae_flag_dtor)) {
        REM_REF(a->code);
      }
    }
  }
  if(a->def && !GET_FLAG(a, ae_flag_template))
    free_func_def(a->def);
  free(a);
}

Type new_type(te_type xid, m_str name, Type parent) {
  Type type    = calloc(1, sizeof(struct Type_));
  type->xid    = xid;
  type->name   = name;
  type->parent = parent;
  INIT_OO(type, e_type_obj);
  return type;
}

static void free_type(Type a) {
  if(a->info)
    REM_REF(a->info);
  if(!GET_FLAG(a, ae_flag_builtin) || a->parent == &t_int || isa(a, &t_class) > 0
      || isa(a, &t_function) > 0 || a->array_depth)
    free(a);
}

Type type_copy(Env env, Type type) {
  Type a         = calloc(1, sizeof(struct Type_));
  a->xid         = type->xid;
  a->name        = type->name;
  a->parent      = type->parent;
  a->info        = type->info;
  a->owner       = type->owner;
  a->size        = type->size;
  a->d.actual_type = type->d.actual_type;
  a->array_depth = type->array_depth;
  if(GET_FLAG(type, ae_flag_checked))
    SET_FLAG(a, ae_flag_checked);
  a->def         = type->def;
  INIT_OO(a, e_type_obj);
  return a;
}

void rem_ref(VM_Object a, void* ptr) {
  if(!--a->ref_count) {
    switch(a->type) {
      case e_nspc_obj:
        free_nspc(ptr);
        break;
      case e_type_obj:
        free_type(ptr);
        break;
      case e_value_obj:
        free_value(ptr);
        break;
      case e_context_obj:
        free_context(ptr);
        break;
      case e_func_obj:
        free_func(ptr);
        break;
      case e_code_obj:
        free_vm_code(ptr);
        break;
    }
  }
}
