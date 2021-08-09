#ifndef __NSPC
#define __NSPC
struct NspcInfo_ {
  struct Map_    op_map;
  Scope          value;
  Scope          type;
  Scope          func;
  Scope          trait;
  struct Vector_ op_tmpl;
};

struct Nspc_ {
  struct Vector_    vtable;
  m_bit *           class_data;
  struct VM_Code_ * pre_ctor;
  struct VM_Code_ * dtor;
  Nspc              parent;
  m_str             name;
  struct NspcInfo_ *info;
  uint16_t       offset;
  uint16_t          ref;
  uint16_t       class_data_size;
};

REF_FUNC(Nspc, nspc)
extern ANEW ANN Nspc new_nspc(MemPool p, const m_str name);

extern ANN void nspc_commit(const Nspc);
// extern ANN void nspc_rollback(const Nspc);

#define describe_lookup0(A, b)                                                 \
  static inline ANN A nspc_lookup_##b##0(const Nspc n, const Symbol s) {       \
    return (A)scope_lookup0(n->info->b, (vtype)s);                             \
  }

#define describe_lookup1(A, b)                                                 \
  static inline ANN A nspc_lookup_##b##1(const Nspc n, const Symbol s) {       \
    const A a = (A)scope_lookup1(n->info->b, (vtype)s);                        \
    if (!a && n->parent) return nspc_lookup_##b##1(n->parent, s);              \
    return a;                                                                  \
  }

#define describe_lookup2(A, b)                                                 \
  static inline ANN A nspc_lookup_##b##2(const Nspc n, const Symbol s) {       \
    return (A)scope_lookup2(n->info->b, (vtype)s);                             \
  }

#define describe_lookups(A, b)                                                 \
  describe_lookup0(A, b) describe_lookup1(A, b) describe_lookup2(A, b)

#define describe_nspc_func(A, b)                                               \
  static inline ANN void nspc_add_##b(const Nspc n, const Symbol s,            \
                                      const A a) {                             \
    scope_add(n->info->b, (vtype)s, (vtype)a);                                 \
  }                                                                            \
  static inline ANN void nspc_add_##b##_front(const Nspc n, const Symbol s,    \
                                              const A a) {                     \
    map_set(&n->info->b->map, (vtype)s, (vtype)a);                             \
  }                                                                            \
  ANN static inline void nspc_push_##b(MemPool p, const Nspc n) {              \
    scope_push(p, n->info->b);                                                 \
  }                                                                            \
  ANN static inline void nspc_pop_##b(MemPool p, const Nspc n) {               \
    scope_pop(p, n->info->b);                                                  \
  }                                                                            \
  describe_lookups(A, b)

describe_nspc_func(Value, value) describe_nspc_func(Type, type)
    describe_nspc_func(Func, func) describe_nspc_func(Trait, trait)
    /* howere there is no need for lookup_func0, push_func, pop_func */
    ANN void did_you_mean_nspc(const Nspc, const char *);
ANN void did_you_mean_type(const Type, const char *);
ANN void did_you_mean_trait(Nspc nspc, const char *name);

#define DID_YOU_MEAN_LIMIT 128
#define did_you_mean_nspc(a, b)                                                \
  if (strlen(b) < DID_YOU_MEAN_LIMIT) did_you_mean_nspc(a, b);
#define did_you_mean_type(a, b)                                                \
  if (strlen(b) < DID_YOU_MEAN_LIMIT) did_you_mean_type(a, b);

ANN static inline void nspc_allocdata(MemPool mp, const Nspc nspc) {
  if (nspc->class_data_size) {
    nspc->class_data =
        (m_bit *)mp_calloc2(mp, nspc->class_data_size);
  }
}
#endif
