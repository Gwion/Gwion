#include <stdlib.h>
#include "absyn.h"
#include "symbol.h"
#include "map.h"
#include "type.h"
#include "value.h"
#include "func.h"
#include "object.h"
#include "mpool.h"

POOL_HANDLE(Nspc, 1024)
extern VM* vm;

ANN Value nspc_lookup_value0(const Nspc nspc, const Symbol xid) {
  return (Value)scope_lookup0(&nspc->value, xid);
}

ANN Value nspc_lookup_value1(const Nspc nspc, const Symbol xid) {
  Value v = (Value)scope_lookup1(&nspc->value, xid);
  if(!v && nspc->parent)
    v = nspc_lookup_value1(nspc->parent, xid);
  return v;
}

ANN Value nspc_lookup_value2(const Nspc nspc, const Symbol xid) {
  return (Value)scope_lookup2(&nspc->value, xid);
}

ANN void  nspc_add_value(const Nspc nspc, const Symbol xid, const Value value) {
  scope_add(&nspc->value, xid, (vtype)value);
}
ANN void nspc_push_value(const Nspc nspc) {
  scope_push(&nspc->value);
}

ANN void nspc_pop_value(const Nspc nspc) {
  scope_pop(&nspc->value);
}

ANN Func nspc_lookup_func0(const Nspc nspc, const Symbol xid) {
  return (Func)scope_lookup0(&nspc->func, xid);
}

ANN Func nspc_lookup_func1(const Nspc nspc, const Symbol xid) {
  Func t = (Func)scope_lookup1(&nspc->func, xid);
  if(!t && nspc->parent)
    t = (Func)nspc_lookup_func1(nspc->parent, xid);
  return t;
}

ANN Func nspc_lookup_func2(const Nspc nspc, const Symbol xid) {
  return (Func)scope_lookup2(&nspc->func, xid);
}

ANN void nspc_add_func(const Nspc nspc, const Symbol xid, const Func value) {
  scope_add(&nspc->func, xid, (vtype)value);
}

ANN Type nspc_lookup_type0(const Nspc nspc, const Symbol xid) {
  return (Type)scope_lookup0(&nspc->type, xid);
}

ANN Type nspc_lookup_type1(const Nspc nspc, const Symbol xid) {
  Type t = (Type)scope_lookup1(&nspc->type, xid);
  if(!t && nspc->parent)
    t = (Type)nspc_lookup_type1(nspc->parent, xid);
  return t;
}

ANN Type nspc_lookup_type2(const Nspc nspc, const Symbol xid) {
  return (Type)scope_lookup2(&nspc->type, xid);
}

ANN void nspc_add_type(const Nspc nspc, const Symbol xid, const Type value) {
  scope_add(&nspc->type, xid, (vtype)value);
}
ANN void nspc_push_type(const Nspc nspc) {
  scope_push(&nspc->type);
}
ANN void nspc_pop_type(const Nspc nspc) {
  scope_pop(&nspc->type);
}

ANN void nspc_commit(const Nspc nspc) {
  scope_commit(&nspc->value);
  scope_commit(&nspc->func);
  scope_commit(&nspc->type);
}

ANN Vector nspc_get_value(const Nspc nspc) {
  return scope_get(&nspc->value);
}

ANN Nspc new_nspc(const m_str name) {
  Nspc a = mp_alloc(Nspc);
  a->name            = name;
  scope_init(&a->value);
  scope_init(&a->type);
  scope_init(&a->func);
  INIT_OO(a, e_nspc_obj);
  return a;
}

ANN static void nspc_release_object(const Nspc a, Value value) {
  if(value->d.ptr || (GET_FLAG(value, ae_flag_static) && a->class_data) ||
(value->d.ptr && GET_FLAG(value, ae_flag_builtin))

) {
    const VM_Code code = new_vm_code(NULL, 0, 0, "in code dtor");
    const VM_Shred s = new_vm_shred(code);
    const M_Object obj = value->d.ptr ? (M_Object)value->d.ptr :
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

ANN static void free_nspc_value_fptr(Func f) {
  while(f) {
    Func tmp = f->next;
    free_func_simple(f);
    f = tmp;
  }
}

ANN static void free_nspc_value(const Nspc a) {
  const Vector v = scope_get(&a->value);
  for(m_uint i = vector_size(v) + 1; --i;) {
    const Value value = (Value)vector_at(v, i - 1);
    if(isa(value->m_type, &t_class) > 0) {
      if(GET_FLAG(value->m_type->d.base_type, ae_flag_template)) {
        UNSET_FLAG(value->m_type->d.base_type, ae_flag_template);
        if(GET_FLAG(value->m_type->d.base_type, ae_flag_ref)) {
          if(!GET_FLAG(value->m_type->d.base_type, ae_flag_builtin)) {
            free_class_def(value->m_type->d.base_type->def);
            REM_REF(value->m_type->d.base_type)
          } else {
            if(value->m_type->d.base_type->def->tmpl)
              free_tmpl_class(value->m_type->d.base_type->def->tmpl);
            free_id_list(value->m_type->d.base_type->def->name);
            free_class_def_simple(value->m_type->d.base_type->def);
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

ANN void free_nspc(Nspc a) {
  Vector v;

  free_nspc_value(a);
  v = scope_get(&a->func);
  for(m_uint i = vector_size(v) + 1; --i;) {
    const Func func = (Func)vector_at(v, i - 1);
    REM_REF(func);
  }
  free_vector(v);
  scope_release(&a->func);

  v = scope_get(&a->type);
  for(m_uint i = vector_size(v); i > 0; i--) {
    const Type type = (Type)vector_at(v, i - 1);
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
  mp_free(Nspc, a);
}
