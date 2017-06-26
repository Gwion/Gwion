#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "emit.h"
#include "func.h"
#include "vm.h"
#include "context.h"
#include "oo.h"

static VM* vm;
void release(M_Object o, VM_Shred shred);
void set_nspc_vm(VM* _vm) {
  vm = _vm;
}

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
          //  just catch func pointer
        } else if(isa(value->m_type, &t_function) > 0) {
          if(value->m_type != &t_function && GET_FLAG(value, ae_flag_builtin))
            REM_REF(value->m_type)
            else if(GET_FLAG(value, ae_flag_template))
              REM_REF(value->func_ref)
              else
                REM_REF(value->m_type)
//continue;
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
    free_vm_code(a->pre_ctor);
  if(a->dtor)
    free_vm_code(a->dtor);
  if(a->op_map.ptr)
    free_op_map(&a->op_map);
  free(a);
}
