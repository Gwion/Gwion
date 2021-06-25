#ifndef __VALUE
#define __VALUE
struct ValueFrom_ {
  Nspc             owner;
  Type             owner_class;
  struct Context_ *ctx;
  size_t           offset;
  m_str            filename;
  loc_t            loc;
};

enum vflag {
  vflag_none    = 1 << 0,
  vflag_func    = 1 << 1,
  vflag_fglobal = 1 << 2,
  vflag_valid   = 1 << 3,
  vflag_direct  = 1 << 4,
  vflag_builtin = 1 << 5,
  vflag_member  = 1 << 6,
  vflag_closed  = 1 << 7,
  vflag_inner   = 1 << 8 // value is in a scope
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
  struct ValueFrom_ *from;
  union value_data   d;
  uint16_t           ref;
  ae_flag            flag;
  enum vflag         vflag;
};

REF_FUNC(Value, value)
FLAG_FUNC(Value, v)

ANEW ANN Value new_value(MemPool p, const Type type, const m_str name);
ANN void       valuefrom(const Env, struct ValueFrom_ *, const loc_t loc);

ANN static inline void defined_here(const Value v) {
  if (v->from->filename) // TODO: check why is that from check
    gwerr_secondary(_("defined here"), v->from->filename, v->from->loc);
}
#endif
