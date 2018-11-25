#include <stdlib.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "nspc.h"
#include "value.h"
#include "func.h"
#include "object.h"
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
  if(value->d.ptr || (GET_FLAG(value, static) && a->class_data) ||
    (value->d.ptr && GET_FLAG(value, builtin))) {
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
  struct scope_iter iter = { &a->value, 0, 0 };
  Value v;
  while(scope_iter(&iter, &v) > 0) {
    if(isa(v->type, t_object) > 0  ||
        (isa(v->type, t_union) > 0 &&
        (GET_FLAG(v, static) ||GET_FLAG(v, global)))) {
      nspc_release_object(a, v);
    }
    REM_REF(v);
  }
  scope_release(&a->value);
}

#define describe_nspc_free(A, b) \
ANN static void nspc_free_##b(Nspc n) {\
  struct scope_iter iter = { &n->b, 0, 0 };\
  A a;\
  while(scope_iter(&iter, &a) > 0) \
    REM_REF(a);\
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
