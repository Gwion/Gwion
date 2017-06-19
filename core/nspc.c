#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "emit.h"
#include "func.h"
#include "vm.h"
#include "context.h"

static VM* vm;
void release(M_Object o, VM_Shred shred);
void set_nspc_vm(VM* _vm) {
  vm = _vm;
}

Value nspc_lookup_value(Nspc nspc, S_Symbol xid, m_bool climb) {
  Value v = (Value)scope_lookup(nspc->value, xid, climb);
  if(climb > 0 && !v && nspc->parent)
    v = nspc_lookup_value(nspc->parent, xid, climb);
  return v;
}

Type nspc_lookup_type(Nspc nspc, S_Symbol xid, m_bool climb) {
  Type t = (Type)scope_lookup(nspc->type, xid, climb);
  if(climb > 0 && !t && nspc->parent)
    t = (Type)nspc_lookup_type(nspc->parent, xid, climb);
  return t;
}

Func nspc_lookup_func(Nspc nspc, S_Symbol xid, m_bool climb) {
  Func t = (Func)scope_lookup(nspc->func, xid, climb);
  if(climb > 0 && !t && nspc->parent)
    t = (Func)nspc_lookup_func(nspc->parent, xid, climb);
  return t;
}

void nspc_commit(Nspc nspc) {
  scope_commit(nspc->value);
  scope_commit(nspc->func);
  scope_commit(nspc->type);
}

Nspc new_nspc(m_str name, m_str filename) {
  Nspc a = calloc(1, sizeof(struct Nspc_));
  a->name            = name;
  a->filename        = filename;
  a->value           = new_scope();
  a->type            = new_scope();
  a->func            = new_scope();
  a->label           = new_map();
  a->class_data_size = 0;
  a->offset          = 0;
  a->class_data      = NULL;
  a->parent          = NULL;
  a->pre_ctor        = NULL;
  a->dtor            = NULL;
  a->obj_v_table     = new_vector();
  a->op_map          = NULL;
  INIT_OO(a, e_nspc_obj);
  return a;
}

void free_nspc(Nspc a) {
  m_uint i;
  Vector v = scope_get(a->value);
  for(i = 0; i < vector_size(v); i++) {
    Value value = (Value)vector_at(v, i);

    if(value->m_type) {
      if(isa(value->m_type, &t_class) > 0)
       REM_REF(value->m_type)
       else if(isa(value->m_type, &t_object) > 0) {
          if(value->ptr || GET_FLAG(value, ae_flag_static)) {
            Vector instr = new_vector();
            VM_Code code = new_vm_code(instr, 0, 0, "in nspc dtor", "");
            VM_Shred s = new_vm_shred(code);
            M_Object obj = value->ptr ? (M_Object)value->ptr :
			    *(M_Object*)(a->class_data + value->offset);
            s->vm_ref = vm;
            release(obj, s);
            free_vm_shred(s);
            free_vector(instr);
          }
        } else if(isa(value->m_type, &t_func_ptr) > 0) {
  //  just catch func pointer
        } else if(isa(value->m_type, &t_function) > 0) {
          if(value->m_type != &t_function && strcmp(a->name, "global_context")) {
            /*if(value->m_type != &t_function && a != vm->env->global_context->nspc) {*/
            free(value->name);
            free(value->m_type->name);
            REM_REF(value->m_type);
          }
        }
    }
    REM_REF(value);
  }
  free_vector(v);
  free_scope(a->value);


  v = scope_get(a->func);
  for(i = 0; i < vector_size(v); i++) {
    Func func = (Func)vector_at(v, i);
    REM_REF(func);
  }
  free_vector(v);
  free_scope(a->func);

  v = scope_get(a->type);
  for(i =vector_size(v); i > 0; i--) {
    Type type = (Type)vector_at(v, i-1);
    REM_REF(type);
  }
  free_vector(v);
  free_scope(a->type);

  for(i = 0; i < map_size(a->label); i++)
    free_map((Map)map_at(a->label, i));
  free_map(a->label);
  if(a->class_data)
    free(a->class_data);
  if(a->obj_v_table)
    free_vector(a->obj_v_table);
  if(a->pre_ctor)
    free_vm_code(a->pre_ctor);
  if(a->dtor)
    free_vm_code(a->dtor);
  if(a->op_map)
    free_op_map(a->op_map);
  free(a);
}
