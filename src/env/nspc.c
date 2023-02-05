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

static inline void _free_nspc_value(const Nspc a, const Value v, Gwion gwion) {
  if(v->from->ctx && v->from->ctx->error) return; // this is quite a hack
  if (GET_FLAG(v, static) && a->class_data) {
    const m_bit *ptr = *(m_bit **)(a->class_data + v->from->offset);
    anytype_release(gwion->vm->cleaner_shred, v->type, ptr);
  } else if (vflag(v, vflag_builtin) && v->d.ptr) {
    const m_bit *ptr = (m_bit*)v->d.ptr;
    anytype_release(gwion->vm->cleaner_shred, v->type, ptr);
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

ANN static void free_operators(NspcOp *a, const Gwion gwion) {
    if (a->map.ptr) free_op_map(&a->map, gwion);
    if (a->tmpl.ptr) free_op_tmpl(&a->tmpl, gwion);
    mp_free(gwion->mp, NspcOp, a);

}
ANN void free_nspc(const Nspc a, const Gwion gwion) {
  nspc_free_value(a, gwion);
  nspc_free_func(a, gwion);
  nspc_free_trait(a, gwion);
  if(a->operators) free_operators(a->operators, gwion);
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

