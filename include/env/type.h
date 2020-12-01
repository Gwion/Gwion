#ifndef __TYPE
#define __TYPE

struct TypeInfo_ {
  Type parent;
  Nspc owner;
  Type owner_class;
  union {
    Union_Def udef;
    Class_Def cdef;
    Func      func;
    Type      base_type;
  };
  struct TupleForm_* tuple;
  struct VM_Code_ *gack;
  struct Context_ *ctx;
};

enum tflag {
  tflag_none    = 1 << 0,
  tflag_scan0   = 1 << 1,//
  tflag_scan1   = 1 << 2,//
  tflag_scan2   = 1 << 3,//
  tflag_check   = 1 << 4,//
  tflag_emit    = 1 << 5,//
  tflag_infer   = 1 << 6,
  tflag_empty   = 1 << 7,
  tflag_ftmpl   = 1 << 8,
  tflag_ntmpl   = 1 << 9, // do NOT need types
  tflag_ctmpl   = 1 << 10, // child template
  tflag_udef    = 1 << 11,
  tflag_cdef    = 1 << 12,
  tflag_struct  = 1 << 13,
  tflag_ctor    = 1 << 14,
  tflag_dtor    = 1 << 15,
  tflag_tmpl    = 1 << 16,
  tflag_typedef = 1 << 17,
  tflag_nonnull = 1 << 18,
  tflag_force   = 1 << 19,
} __attribute__((packed));

struct Type_ {
  m_str     name;
  Nspc      nspc;
  struct TypeInfo_ *info;
  size_t xid;
  size_t size;
  size_t array_depth;
  uint16_t ref;
  ae_flag flag;
  enum tflag tflag;
};

REF_FUNC(Type, type)
FLAG_FUNC(Type, t)

ANN2(1,3) ANEW Type new_type(MemPool, const m_uint xid, const m_str name, const Type);
ANEW ANN Type type_copy(MemPool, const Type type);
ANN m_str get_type_name(const Env, const Type t, const m_uint);
ANN Value find_value(const Type, const Symbol);
ANN m_bool isa(const Type, const Type) __attribute__((pure));
ANN m_bool isres(const Env, const Symbol, const loc_t pos);
ANN Type array_type(const Env, const Type, const m_uint);
ANN Type find_common_anc(const Type, const Type) __attribute__((pure));
ANN m_uint id_list_len(ID_List);
ANN void type_path(const m_str, const ID_List);
ANN Type typedef_base(Type) __attribute__((pure));
ANN Type array_base(Type) __attribute__((pure));
ANN m_bool type_ref(Type) __attribute__((pure));
ANN Type actual_type(const struct Gwion_* gwion, const Type t);
ANN static inline m_uint env_push_type(const Env env, const Type type) { return env_push(env, type, type->nspc); }
ANN m_bool is_fptr(const struct Gwion_*, const Type t);
ANN m_bool is_class(const struct Gwion_*, const Type t);
ANN m_uint get_depth(const Type type);
ANN void inherit(const Type);

__attribute__((returns_nonnull))
ANN static inline Type get_gack(Type t) {
  do if(t->info->gack)
    return t;
  while((t = t->info->parent));
  return t; // unreachable
}

__attribute__((returns_nonnull))
ANN Type unflag_type(const Type t);
__attribute__((returns_nonnull))
ANN Type get_type(const Type t);
ANN static inline int is_special(const Type t) {
  return tflag(t, tflag_nonnull) || tflag(t, tflag_force);
}

typedef enum {
  et_void, et_int, et_bool, et_char, et_float,
  et_null, et_compound, et_object, et_shred, et_fork, et_event, et_ugen, et_string, et_ptr, et_array, et_gack,
  et_function, et_fptr, et_vararg, et_lambda, et_class, et_union, et_undefined, et_auto,
  MAX_TYPE
} type_enum;
#endif
