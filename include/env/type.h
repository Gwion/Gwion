#ifndef __TYPE
#define __TYPE

struct TypeInfo_ {
  Type  parent;
  Value value;
  union {
    Union_Def udef;
    Class_Def cdef;
    Func      func;
  };
  Type               base_type;
  struct TupleForm_ *tuple;
  struct VM_Code_ *  gack;
  ID_List            traits;
};

enum tflag {
  tflag_none     = 1 << 0,
  tflag_scan0    = 1 << 1, //
  tflag_scan1    = 1 << 2, //
  tflag_scan2    = 1 << 3, //
  tflag_check    = 1 << 4, //
  tflag_emit     = 1 << 5, //
  tflag_infer    = 1 << 6,
  tflag_empty    = 1 << 7,
  tflag_ftmpl    = 1 << 8,
  tflag_ntmpl    = 1 << 9, // do NOT need types
  tflag_udef     = 1 << 10,
  tflag_cdef     = 1 << 11,
  tflag_struct   = 1 << 12,
  tflag_ctor     = 1 << 13,
  tflag_dtor     = 1 << 14,
  tflag_tmpl     = 1 << 15,
  tflag_typedef  = 1 << 16,
  tflag_distinct = 1 << 17,
  tflag_noret    = 1 << 18,
  tflag_contract = 1 << 19,
  tflag_float    = 1 << 20,
} __attribute__((packed));

struct Type_ {
  m_str             name;
  Nspc              nspc;
  struct TypeInfo_ *info;
  size_t            size;
  size_t            array_depth;
  struct Vector_    effects; // pre-ctor effects
  uint16_t          ref;
  uint16_t          weight;
  ae_flag           flag;
  enum tflag        tflag;
};

REF_FUNC(Type, type)
FLAG_FUNC(Type, t)

ANN2(1, 2) ANEW Type new_type(MemPool, const m_str name, const Type);
ANEW ANN Type type_copy(MemPool, const Type type);
ANN Value     find_value(const Type, const Symbol);
ANN m_bool    isa(const Type, const Type) __attribute__((pure));
ANN m_bool    isres(const Env, const Symbol, const loc_t pos);
ANN Type      array_type(const Env, const Type, const m_uint);
ANN Type      find_common_anc(const Type, const Type) __attribute__((pure));
ANN Type      typedef_base(Type) __attribute__((pure));
ANN Type      array_base(Type) __attribute__((pure));
ANN m_bool    type_ref(Type) __attribute__((pure));
ANN Type      actual_type(const struct Gwion_ *gwion, const Type t);
ANN static inline m_uint env_push_type(const Env env, const Type type) {
  return env_push(env, type, type->nspc);
}
ANN bool is_func(const struct Gwion_ *, const Type t);
ANN bool is_fptr(const struct Gwion_ *, const Type t);
ANN bool is_class(const struct Gwion_ *, const Type t);
ANN __attribute__((returns_nonnull)) static inline Type _class_base(Type t) {
  return t->info->base_type;
}
ANN m_uint get_depth(const Type type);
ANN void   inherit(const Type);

__attribute__((returns_nonnull)) ANN static inline Type get_gack(Type t) {
  do
    if (t->info->gack) return t;
  while ((t = t->info->parent));
  return t; // unreachable
}

// trait helpers
ANN static inline bool has_trait(const Type t, const Symbol trait) {
  ID_List list = t->info->traits;
  while (list) {
    if (list->xid == trait) return true;
    list = list->next;
  }
  return false;
}

ANN static inline ID_List miss_traits(const Type t, const Specialized_List sl) {
  ID_List traits = sl->traits;
  while (traits) {
    if (!has_trait(t, traits->xid)) return traits;
    traits = traits->next;
  }
  return NULL;
}
typedef enum {
  et_void,
  et_int,
  et_bool,
  et_char,
  et_float,
  et_error,
  et_compound,
  et_object,
  et_shred,
  et_fork,
  et_event,
  et_ugen,
  et_string,
  et_ptr,
  et_array,
  et_gack,
  et_function,
  et_fptr,
  et_vararg,
  et_lambda,
  et_op,
  et_class,
  et_union,
  et_auto,
  et_none,
  et_curry,
  MAX_TYPE
} type_enum;
#endif
