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

ANN static void free_nspc_value(const Nspc a, Gwion gwion) {
  struct scope_iter iter = {a->info->value, 0, 0};
  Value             v;
  while (scope_iter(&iter, &v) > 0) {
    if (isa(v->type, gwion->type[et_object]) > 0)
      nspc_release_object(a, v, gwion);
    else if (tflag(v->type, tflag_struct))
      nspc_release_struct(a, v, gwion);
    value_remref(v, gwion);
  }
  free_scope(gwion->mp, a->info->value);
}

#define describe_nspc_free(A, b)                                               \
  ANN static void nspc_free_##b(Nspc n, Gwion gwion) {                         \
    struct scope_iter iter = {n->info->b, 0, 0};                               \
    A                 a;                                                       \
    while (scope_iter(&iter, &a) > 0) b##_remref(a, gwion);                    \
    free_scope(gwion->mp, n->info->b);                                         \
  }

describe_nspc_free(Func, func) describe_nspc_free(Type, type) ANN
    static void nspc_free_trait(Nspc n, Gwion gwion) {
  struct scope_iter iter = {n->info->trait, 0, 0};
  Trait             a;
  while (scope_iter(&iter, &a) > 0) free_trait(gwion->mp, a);
  free_scope(gwion->mp, n->info->trait);
}

ANN void free_nspc(const Nspc a, const Gwion gwion) {
  free_nspc_value(a, gwion);
  nspc_free_func(a, gwion);
  nspc_free_trait(a, gwion);
  if (a->info->op_map.ptr) free_op_map(&a->info->op_map, gwion);
  if (a->info->op_tmpl.ptr) free_op_tmpl(&a->info->op_tmpl, gwion);
  nspc_free_type(a, gwion);
  if (a->class_data && a->class_data_size)
    mp_free2(gwion->mp, a->class_data_size, a->class_data);
  if (a->vtable.ptr) vector_release(&a->vtable);
  mp_free(gwion->mp, NspcInfo, a->info);
  if (a->pre_ctor) vmcode_remref(a->pre_ctor, gwion);
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
