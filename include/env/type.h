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
  tflag_union    = 1 << 21,
  tflag_enum     = 1 << 22,
  tflag_error    = 1 << 23,
  tflag_ref      = 1 << 24,
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
ANN Type      array_base(Type) __attribute__((pure));
ANN Symbol array_sym(const Env env, const Type src,
                                 const m_uint depth);
ANN static inline Type array_base_simple(Type t) {
  return t->array_depth ? t->info->base_type : t;
}
ANN m_bool    type_ref(Type) __attribute__((pure));
ANN Type      actual_type(const struct Gwion_ *gwion, const Type t);
ANN static inline m_uint env_push_type(const Env env, const Type type) {
  return env_push(env, type, type->nspc);
}
ANN bool is_func(const struct Gwion_ *, const Type t);
ANN bool is_class(const struct Gwion_ *, const Type t);
ANN __attribute__((returns_nonnull)) static inline Type _class_base(Type t) {
  return t->info->base_type;
}
ANN m_uint get_depth(const Type type);
ANN void   inherit(const Type);
ANN bool type_global(const Env env, Type t);
ANN bool from_global_nspc(const Env env, const Nspc nspc);

ANN static inline Type typedef_base(Type t) {
  while (tflag(t, tflag_typedef)) t = t->info->parent;
  return t;
}

ANN static inline Func_Def closure_def(Type t) {
  t = typedef_base(t);
  return mp_vector_at(t->info->cdef->body, Section, 0)->d.func_def;
}

__attribute__((returns_nonnull)) ANN static inline Type get_gack(Type t) {
  do if (t->info->gack) return t;
  while ((t = t->info->parent));
  return t; // unreachable
}

// trait helpers
ANN static inline bool has_trait(const Type t, const Symbol trait) {
  ID_List list = t->info->traits;
  for(uint32_t i = 0; i < list->len; i++) {
    Symbol xid = *mp_vector_at(list, Symbol, i);
    if (xid == trait) return true;
  }
  return false;
}

ANN static inline Symbol miss_traits(const Type t, const Specialized *spec) {
  ID_List traits = spec->traits;
  for(uint32_t i = 0; i < traits->len; i++) {
    Symbol xid = *mp_vector_at(traits, Symbol, i);
    if (!has_trait(t, xid)) return xid;
  }
  return NULL;
}

ANN Type ref_type(const Gwion gwion, const Type t, const loc_t loc);

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
  et_closure,
  et_op,
  et_class,
  et_union,
  et_auto,
  et_none,
  et_dict,
  MAX_TYPE
} type_enum;
#endif
