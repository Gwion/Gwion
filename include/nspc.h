#include "vm.h"
#include "operator.h"

struct Nspc_ {
  m_str     name;
  m_uint    offset;
  Nspc parent;
  VM_Code   pre_ctor;
  VM_Code   dtor;
  m_bit* class_data;
  m_uint    class_data_size;
  struct Vector_    vtable;
  struct Map_      	op_map;
  struct Scope_     value;
  struct Scope_     type;
  struct Scope_     func;
  HAS_OBJ
};

extern ANEW ANN Nspc new_nspc(const m_str name);
extern ANN void free_nspc(const Nspc a);

extern ANN void nspc_commit(const Nspc);
//extern ANN void nspc_rollback(const Nspc);

#define describe_lookup0(A, b)                                                 \
static inline ANN A nspc_lookup_##b##0(const Nspc n, const Symbol s){          \
  return (A)scope_lookup0(&n->b, s);                                           \
}

#define describe_lookup1(A, b)                                                 \
static inline ANN A nspc_lookup_##b##1(const Nspc n, const Symbol s) {         \
  const A a = (A)scope_lookup1(&n->b, s);                                      \
  if(!a && n->parent)                                                          \
    return nspc_lookup_##b##1(n->parent, s);                                   \
  return a;                                                                    \
}

#define describe_lookup2(A, b)                                                 \
static inline ANN A nspc_lookup_##b##2(const Nspc n, const Symbol s) {         \
  return (A)scope_lookup2(&n->b, s);                                           \
}

#define describe_lookups(A, b)                                                 \
describe_lookup0(A, b)                                                         \
describe_lookup1(A, b)                                                         \
describe_lookup2(A, b)

#define describe_nspc_func(A, b)                                               \
static inline ANN void nspc_add_##b(const Nspc n, const Symbol s, const A a) { \
  scope_add(&n->b, s, (vtype)a);                                               \
}                                                                              \
ANN static inline void nspc_push_##b(const Nspc n) { scope_push(&n->b); }      \
ANN inline static void nspc_pop_##b (const Nspc n) { scope_pop (&n->b); }      \
describe_lookups(A, b)

describe_nspc_func(Value, value)
describe_nspc_func(Type, type)
describe_nspc_func(Func, func)
/* howere there is no need for lookup_func0, push_func, pop_func */
