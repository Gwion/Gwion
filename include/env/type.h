#ifndef __TYPE
#define __TYPE

struct TypeInfo_ {
  Type parent;
  Nspc owner;
  Type owner_class;
  Class_Def def;
  union type_data {
    Func      func;
    Type      base_type;
  } d;
  struct TupleForm_* tuple;
  struct VM_Code_ *gack;
  struct Context_ *ctx;
};

struct Type_ {
  m_str     name;
  Nspc      nspc;
  struct TypeInfo_ *e;
  size_t xid;
  size_t size;
  size_t array_depth;
  HAS_OBJ
  ae_flag flag;
};

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

__attribute__((returns_nonnull))
ANN static inline Type get_gack(Type t) {
  do if(t->e->gack)
    return t;
  while((t = t->e->parent));
  return t; // unreachable
}

typedef enum {
  et_void, et_int, et_bool, et_char, et_float,
  et_null, et_compound, et_object, et_shred, et_fork, et_event, et_ugen, et_string, et_ptr, et_array, et_gack,
  et_function, et_fptr, et_vararg, et_lambda, et_class, et_union, et_undefined, et_auto,
  MAX_TYPE
} type_enum;
#endif
