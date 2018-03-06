#include <stdlib.h>
#include "absyn.h"
#include "symbol.h"
#include "map.h"
#include "type.h"
#include "value.h"
#include "func.h"
#include "object.h"

extern VM* vm;

Value nspc_lookup_value0(const Nspc nspc, const S_Symbol xid) {
  return (Value)scope_lookup0(&nspc->value, xid);
}

Value nspc_lookup_value1(const Nspc nspc, const S_Symbol xid) {
  Value v = (Value)scope_lookup1(&nspc->value, xid);
  if(!v && nspc->parent)
    v = nspc_lookup_value1(nspc->parent, xid);
  return v;
}

Value nspc_lookup_value2(const Nspc nspc, const S_Symbol xid) {
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

Func nspc_lookup_func0(const Nspc nspc, const S_Symbol xid) {
  return (Func)scope_lookup0(&nspc->func, xid);
}

Func nspc_lookup_func1(const Nspc nspc, const S_Symbol xid) {
  Func t = (Func)scope_lookup1(&nspc->func, xid);
  if(!t && nspc->parent)
    t = (Func)nspc_lookup_func1(nspc->parent, xid);
  return t;
}

Func nspc_lookup_func2(const Nspc nspc, const S_Symbol xid) {
  return (Func)scope_lookup2(&nspc->func, xid);
}

void nspc_add_func(Nspc nspc, S_Symbol xid, Func value) {
  scope_add(&nspc->func, xid, (vtype)value);
}

Type nspc_lookup_type0(const Nspc nspc, const S_Symbol xid) {
  return (Type)scope_lookup0(&nspc->type, xid);
}

Type nspc_lookup_type1(const Nspc nspc, const S_Symbol xid) {
  Type t = (Type)scope_lookup1(&nspc->type, xid);
  if(!t && nspc->parent)
    t = (Type)nspc_lookup_type1(nspc->parent, xid);
  return t;
}

Type nspc_lookup_type2(const Nspc nspc, const S_Symbol xid) {
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

Vector nspc_get_value(const Nspc nspc) {
  return scope_get(&nspc->value);
}

Nspc new_nspc(m_str name) {
  Nspc a = calloc(1, sizeof(struct Nspc_));
  a->name            = name;
  scope_init(&a->value);
  scope_init(&a->type);
  scope_init(&a->func);
  INIT_OO(a, e_nspc_obj);
  return a;
}

static void nspc_release_object(Nspc a, Value value) {
  if(value->d.ptr || (GET_FLAG(value, ae_flag_static) && a->class_data) ||
(value->d.ptr && GET_FLAG(value, ae_flag_builtin))

) {
    VM_Code code = new_vm_code(NULL, 0, 0, "in code dtor");
    VM_Shred s = new_vm_shred(code);
    M_Object obj = value->d.ptr ? (M_Object)value->d.ptr :
        *(M_Object*)(a->class_data + value->offset);
    s->vm_ref = vm;
    release(obj, s);
    free_vm_shred(s);
  }
  if(value->m_type->array_depth && !GET_FLAG(value->m_type, ae_flag_typedef))
    REM_REF(value->m_type)
  else if(GET_FLAG(value->m_type, ae_flag_builtin) && GET_FLAG(value->m_type, ae_flag_typedef))
    REM_REF(value->m_type->parent)
}

static void free_nspc_value_fptr(Func f) {
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
    if(isa(value->m_type, &t_class) > 0) {
      if(GET_FLAG(value->m_type->d.base_type, ae_flag_template)) {
        UNSET_FLAG(value->m_type->d.base_type, ae_flag_template);
        if(GET_FLAG(value->m_type->d.base_type, ae_flag_ref)) {
          if(!GET_FLAG(value->m_type->d.base_type, ae_flag_builtin)) {
            free_class_def(value->m_type->d.base_type->def);
            REM_REF(value->m_type->d.base_type)
          } else {
            if(value->m_type->d.base_type->def->tmpl)
              free(value->m_type->d.base_type->def->tmpl);
            free_id_list(value->m_type->d.base_type->def->name);
            free(value->m_type->d.base_type->def);
            SET_FLAG(value->m_type->d.base_type, ae_flag_template);
            REM_REF(value->m_type->d.base_type)
          }
        } else
          free_class_def(value->m_type->d.base_type->def);
      }
      REM_REF(value->m_type)
    } else if(isa(value->m_type, &t_union) > 0) {
      if(GET_FLAG(value, ae_flag_static))
        nspc_release_object(a, value);
      REM_REF(value->m_type)
    }
    else if(isa(value->m_type, &t_object) > 0)
      nspc_release_object(a, value);
    else if(isa(value->m_type, &t_func_ptr) > 0)
      free_nspc_value_fptr(value->d.func_ref);
    else if(isa(value->m_type, &t_function) > 0) {
      if(GET_FLAG(value, ae_flag_template)) {
        REM_REF(value->m_type)
        REM_REF(value->d.func_ref)
      }
      else
        REM_REF(value->m_type)
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
  if(a->vtable.ptr)
    vector_release(&a->vtable);
  if(a->pre_ctor)
    REM_REF(a->pre_ctor);
  if(a->dtor)
    REM_REF(a->dtor);
  if(a->op_map.ptr)
    free_op_map(&a->op_map);
  free(a);
}
