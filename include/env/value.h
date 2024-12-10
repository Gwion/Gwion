#ifndef __VALUE
#define __VALUE
typedef struct ValueFrom_ {
  Nspc             owner;
  Type             owner_class;
  struct Context_ *ctx;
  uint32_t         offset;
  m_str            filename;
  loc_t            loc;
} ValueFrom;

enum vflag {
  vflag_none     = 1 << 0,
  vflag_func     = 1 << 1,
  vflag_fglobal  = 1 << 2,
  vflag_valid    = 1 << 3,
  vflag_direct   = 1 << 4,
  vflag_builtin  = 1 << 5,
  vflag_member   = 1 << 6,
  vflag_release  = 1 << 7,
  vflag_assigned = 1 << 8,
  vflag_arg      = 1 << 9
} __attribute__((packed));

union value_data {
  m_int             num;
  m_float           fnum;
  m_uint *          ptr;
  struct M_Object_ *obj;
  Func              func_ref;
};

struct Value_ {
  Type               type;
  m_str              name;
  ValueFrom *from;
  FuncList  *used_by; // list of functions using this global
  union value_data   d;
  uint16_t           ref;
  ae_flag            flag;
  enum vflag         vflag;
};

MK_VECTOR_TYPE(Value, value);
REF_FUNC(Value, value)
FLAG_FUNC(Value, v)

ANEW ANN Value new_value(const Env, const Type type, const Tag tag);
ANN void       valuefrom(const Env, ValueFrom *);

ANN static inline void valid_value(const Env env, const Symbol xid, const Value v) {
  set_vflag(v, vflag_valid);
  nspc_add_value(env->curr, xid, v);
}

ANN static inline m_uint env_pushv(const Env env, const Value value) {
  return env_push(env, value->from->owner_class, value->from->owner);
}

#endif
