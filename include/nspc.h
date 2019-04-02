#ifndef __NSPC
#define __NSPC
struct NspcInfo_ {
  m_bit* class_data;
  struct Vector_    vtable;
  struct Map_      	op_map;
  struct Scope_     value;
  struct Scope_     type;
  struct Scope_     func;
  size_t offset;
  size_t class_data_size;
};

struct Nspc_ {
  m_str     name;
  Nspc parent;
  struct VM_Code_*   pre_ctor;
  struct VM_Code_*   dtor;
  struct NspcInfo_* info;
  HAS_OBJ
};

extern ANEW ANN Nspc new_nspc(MemPool p, const m_str name);

extern ANN void nspc_commit(const Nspc);
//extern ANN void nspc_rollback(const Nspc);

#define describe_lookup0(A, b)                                                 \
static inline ANN A nspc_lookup_##b##0(const Nspc n, const Symbol s){          \
  return (A)scope_lookup0(&n->info->b, (vtype)s);                              \
}

#define describe_lookup1(A, b)                                                 \
static inline ANN A nspc_lookup_##b##1(const Nspc n, const Symbol s) {         \
  const A a = (A)scope_lookup1(&n->info->b, (vtype)s);                         \
  if(!a && n->parent)                                                          \
    return nspc_lookup_##b##1(n->parent, s);                                   \
  return a;                                                                    \
}

#define describe_lookups(A, b)                                                 \
describe_lookup0(A, b)                                                         \
describe_lookup1(A, b)                                                         \

#define describe_nspc_func(A, b)                                               \
static inline ANN void nspc_add_##b(const Nspc n, const Symbol s, const A a) { \
  scope_add(&n->info->b, (vtype)s, (vtype)a);                                  \
}                                                                              \
ANN static inline void nspc_push_##b(MemPool p, const Nspc n) { scope_push(p, &n->info->b); }\
ANN inline static void nspc_pop_##b (MemPool p, const Nspc n) { scope_pop (p, &n->info->b); }\
describe_lookups(A, b)

describe_nspc_func(Value, value)
describe_nspc_func(Type, type)
describe_nspc_func(Func, func)
/* howere there is no need for lookup_func0, push_func, pop_func */
#endif
