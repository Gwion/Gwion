#include <stdlib.h>
#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "symbol.h"
#include "map.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "nspc.h"
#include "value.h"
#include "func.h"
#include "object.h"
#include "mpool.h"
#include "operator.h"

extern VM* some_global_vm_soon_to_disapear;

ANN void nspc_commit(const Nspc nspc) {
  scope_commit(&nspc->value);
  scope_commit(&nspc->func);
  scope_commit(&nspc->type);
}

ANN Nspc new_nspc(const m_str name) {
  const Nspc a = mp_alloc(Nspc);
  a->name = name;
  scope_init(&a->value);
  scope_init(&a->type);
  scope_init(&a->func);
  INIT_OO(a, e_nspc_obj);
  return a;
}

ANN static void nspc_release_object(const Nspc a, Value value) {
  if(value->d.ptr || (GET_FLAG(value, ae_flag_static) && a->class_data) ||
    (value->d.ptr && GET_FLAG(value, ae_flag_builtin))) {
    const VM_Code code = new_vm_code(NULL, 0, 0, "in code dtor");
    const VM_Shred s = new_vm_shred(code);
    const M_Object obj = value->d.ptr ? (M_Object)value->d.ptr :
        *(M_Object*)(a->class_data + value->offset);
    s->vm = some_global_vm_soon_to_disapear;
    release(obj, s);
    free_vm_shred(s);
  }
}

ANN static void free_nspc_value(const Nspc a) {
  const Vector v = scope_get(&a->value);
  for(m_uint i = vector_size(v) + 1; --i;) {
    const Value value = (Value)vector_at(v, i - 1);
    if(isa(value->type, t_class) > 0)
      REM_REF(value->type)
    else if(isa(value->type, t_union) > 0) {
      if(GET_FLAG(value, ae_flag_static) ||GET_FLAG(value, ae_flag_global))
        nspc_release_object(a, value);
      if(GET_FLAG(value->type, ae_flag_op)) // only free untyped unions
        REM_REF(value->type)
    }
    else if(isa(value->type, t_function) > 0)
      REM_REF(value->type)
    else if(isa(value->type, t_object) > 0)
      nspc_release_object(a, value);
    REM_REF(value);
  }
  free_vector(v);
  scope_release(&a->value);
}

#define describe_nspc_free(A, b) \
ANN static void nspc_free_##b(Nspc n) {\
  const Vector v = scope_get(&n->b);\
  for(m_uint i = vector_size(v) + 1; --i;) {\
    const A a = (A)vector_at(v, i - 1);\
    REM_REF(a);\
  }\
  free_vector(v);\
  scope_release(&n->b);\
}
describe_nspc_free(Func, func)
describe_nspc_free(Type, type)

ANN void free_nspc(Nspc a) {
  free_nspc_value(a);
  nspc_free_func(a);
  nspc_free_type(a);

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
