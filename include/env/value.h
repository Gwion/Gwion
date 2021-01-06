#ifndef __VALUE
#define __VALUE
struct ValueFrom_ {
  Nspc owner;
  Type owner_class;
  struct Context_ *ctx;
  size_t offset;
};

enum vflag {
  vflag_none  = 1 << 0,
  vflag_func  = 1 << 1,
  vflag_union = 1 << 2,
  vflag_enum  = 1 << 3,
  vflag_freeme  = 1 << 4,
  vflag_fglobal  = 1 << 5,
  vflag_valid    = 1 << 6,
  vflag_direct    = 1 << 7,
  vflag_builtin    = 1 << 8,
  vflag_member   = 1 << 9,
  vflag_closed   = 1 << 10
//  vflag_used = 1 << 3
} __attribute__((packed));

struct Value_ {
  Type type;
  m_str name;
  struct ValueFrom_ *from;
  union __attribute__((transparent_union)) value_data {
    m_uint num;
    m_float fnum;
    m_uint* ptr;
    struct M_Object_ *obj;
    Func func_ref;
  } d;
  uint16_t ref;
  ae_flag flag;
  enum vflag vflag;
};

REF_FUNC(Value, value)
FLAG_FUNC(Value, v)

ANEW ANN Value new_value(MemPool p, const Type type, const m_str name);
ANN void valuefrom(const Env, struct ValueFrom_*);
#endif
