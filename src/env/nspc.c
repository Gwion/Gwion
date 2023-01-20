#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"

ANN void nspc_commit(const Nspc nspc) {
  scope_commit(nspc->info->value);
  scope_commit(nspc->info->func);
  scope_commit(nspc->info->type);
  scope_commit(nspc->info->trait);
}

ANN static inline void nspc_release_object(const Nspc a, Value value,
                                           Gwion gwion) {
  if ((GET_FLAG(value, static) && a->class_data) ||
      (value->d.ptr && vflag(value, vflag_builtin))) {
    const M_Object obj =
        value->d.ptr ? (M_Object)value->d.ptr
                     : *(M_Object *)(a->class_data + value->from->offset);
    release(obj, gwion->vm->cleaner_shred);
  }
}

ANN2(1, 3)
static inline void nspc_release_struct(const Nspc a, Value value, Gwion gwion) {
  if (value && ((GET_FLAG(value, static) && a->class_data) ||
                (vflag(value, vflag_builtin) && value->d.ptr))) {
    const m_bit *ptr =
        (value && value->d.ptr)
            ? (m_bit *)value->d.ptr
            : (m_bit *)(a->class_data + value->from->offset);
    for (m_uint i = 0; i < vector_size(&value->type->info->tuple->types); ++i) {
      const Type t = (Type)vector_at(&value->type->info->tuple->types, i);
      if (isa(t, gwion->type[et_object]) > 0)
        release(*(M_Object *)(ptr +
                              vector_at(&value->type->info->tuple->offset, i)),
                gwion->vm->cleaner_shred);
      else if (tflag(t, tflag_struct))
        nspc_release_struct(t->nspc, NULL, gwion);
    }
  }
}

static inline void _free_nspc_value(const Nspc a, const Value v, Gwion gwion) {
  if(v->from->ctx && v->from->ctx->error) return; // this is quite a hack
  if (tflag(v->type, tflag_compound) ) {
    if (!tflag(v->type, tflag_struct))
      nspc_release_object(a, v, gwion);
    else nspc_release_struct(a, v, gwion);
  }
  value_remref(v, gwion);
}

#define describe_nspc_free(A, b, dofree)                                       \
  ANN static void nspc_free_##b(Nspc n, Gwion gwion) {                         \
    struct scope_iter iter = {n->info->b, 0, 0};                               \
    A                 a;                                                       \
    while (scope_iter(&iter, &a) > 0) dofree;                                  \
    free_scope(gwion->mp, n->info->b);                                         \
  }

describe_nspc_free(Value, value, _free_nspc_value(n, a, gwion));
describe_nspc_free(Func, func, func_remref(a, gwion));
describe_nspc_free(Type, type, type_remref(a, gwion));
describe_nspc_free(Trait, trait, free_trait(gwion->mp, a));

ANN void free_nspc(const Nspc a, const Gwion gwion) {
  nspc_free_value(a, gwion);
  nspc_free_func(a, gwion);
  nspc_free_trait(a, gwion);
  if(a->operators) {
    if (a->operators->map.ptr) free_op_map(&a->operators->map, gwion);
    if (a->operators->tmpl.ptr) free_op_tmpl(&a->operators->tmpl, gwion);
    mp_free(gwion->mp, NspcOp, a->operators);
  }
  nspc_free_type(a, gwion);
  if (a->class_data && a->class_data_size)
    mp_free2(gwion->mp, a->class_data_size, a->class_data);
  if (a->vtable.ptr) vector_release(&a->vtable);
  mp_free(gwion->mp, NspcInfo, a->info);
  if (a->dtor) vmcode_remref(a->dtor, gwion);
  mp_free(gwion->mp, Nspc, a);
}

ANN Nspc new_nspc(MemPool p, const m_str name) {
  const Nspc a   = mp_calloc(p, Nspc);
  a->name        = name;
  a->info        = mp_calloc(p, NspcInfo);
  a->info->value = new_scope(p);
  a->info->type  = new_scope(p);
  a->info->func  = new_scope(p);
  a->info->trait = new_scope(p);
  a->ref         = 1;
  return a;
}

static nspc_add_value_t _add = _nspc_add_value;
static nspc_add_value_t _front = _nspc_add_value_front;
ANN void nspc_add_value(const Nspc n, const Symbol s, const Value a) {
  _add(n, s, a);
}
ANN void nspc_add_value_front(const Nspc n, const Symbol s, const Value a) {
  _front(n, s, a);
}

ANN void nspc_add_value_set_func(nspc_add_value_t add, nspc_add_value_t front) {
  _add = add;
  _front = front;
}

