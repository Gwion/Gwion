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
#include "gwion.h"

ANN void nspc_commit(const Nspc nspc) {
  scope_commit(&nspc->info->value);
  scope_commit(&nspc->info->func);
  scope_commit(&nspc->info->type);
}

ANN static void nspc_release_object(const Nspc a, Value value) {
//  if(value->d.ptr || (GET_FLAG(value, static) && a->info->class_data) ||
  if((GET_FLAG(value, static) && a->info->class_data) ||
    (value->d.ptr && GET_FLAG(value, builtin))) {
    const VM_Code code = new_vm_code(NULL, 0, ae_flag_builtin, "in code dtor");
    const VM_Shred s = new_vm_shred(code);
    const M_Object obj = value->d.ptr ? (M_Object)value->d.ptr :
        *(M_Object*)(a->info->class_data + value->offset);
    s->info->vm = value->gwion->vm;
    release(obj, s);
    free_vm_shred(s);
  }
}

ANN static void free_nspc_value(const Nspc a) {
  struct scope_iter iter = { &a->info->value, 0, 0 };
  Value v;
  while(scope_iter(&iter, &v) > 0) {
    if(isa(v->type, t_object) > 0  ||
        (isa(v->type, t_union) > 0 &&
        (GET_FLAG(v, static) || GET_FLAG(v, global)))) {
      nspc_release_object(a, v);
    }
    REM_REF(v);
  }
  scope_release(&a->info->value);
}

#define describe_nspc_free(A, b) \
ANN static void nspc_free_##b(Nspc n) {\
  struct scope_iter iter = { &n->info->b, 0, 0 };\
  A a;\
  while(scope_iter(&iter, &a) > 0) \
    REM_REF(a);\
  scope_release(&n->info->b);\
}

describe_nspc_free(Func, func)
describe_nspc_free(Type, type)

ANN static void free_nspc(Nspc a) {
  nspc_free_func(a);
  nspc_free_type(a);
  free_nspc_value(a);

  if(a->info->class_data)
    free(a->info->class_data);
  if(a->info->vtable.ptr)
    vector_release(&a->info->vtable);
  if(a->info->op_map.ptr)
    free_op_map(&a->info->op_map);
  mp_free(NspcInfo, a->info);
  if(a->pre_ctor)
    REM_REF(a->pre_ctor);
  if(a->dtor)
    REM_REF(a->dtor);
  mp_free(Nspc, a);
}

ANN Nspc new_nspc(const m_str name) {
  const Nspc a = mp_alloc(Nspc);
  a->name = name;
  a->info = mp_alloc(NspcInfo);
  scope_init(&a->info->value);
  scope_init(&a->info->type);
  scope_init(&a->info->func);
  INIT_OO(a, free_nspc);
  return a;
}
