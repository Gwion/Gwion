#ifndef __VALUE
#define __VALUE
typedef struct ValueFrom_ {
  Nspc             owner;
  Type             owner_class;
  struct Context_ *ctx;
  size_t           offset;
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
  vflag_inner    = 1 << 7, // value is in a scope
  vflag_release  = 1 << 8,
  vflag_assigned = 1 << 9,
  vflag_arg      = 1 << 10
  //  vflag_used = 1 << 3
} __attribute__((packed));

union value_data {
  m_uint            num;
  m_float           fnum;
  m_uint *          ptr;
  struct M_Object_ *obj;
  Func              func_ref;
};

struct Value_ {
  Type               type;
  m_str              name;
  ValueFrom *from;
  union value_data   d;
  uint16_t           ref;
  ae_flag            flag;
  enum vflag         vflag;
};

REF_FUNC(Value, value)
FLAG_FUNC(Value, v)

ANEW ANN Value new_value(const Env, const Type type, const m_str name, const loc_t loc);
ANN void       valuefrom(const Env, ValueFrom *);

ANN static inline void defined_here(const Value v) {
  if (v->from->filename) // TODO: check why is that from check
    gwerr_secondary(_("defined here"), v->from->filename, v->from->loc);
}

ANN static inline void valid_value(const Env env, const Symbol xid, const Value v) {
  set_vflag(v, vflag_valid);
  nspc_add_value(env->curr, xid, v);
}
#endif
