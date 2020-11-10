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
  union value_data{
    m_uint* ptr;
    Func func_ref;
  } d;
  uint16_t ref;
  ae_flag flag;
  enum vflag vflag;
};

ANN void free_value(const Value, struct Gwion_*const);
ANN static inline void value_addref(const Value v) { ++v->ref; }
ANN static inline void value_remref(const Value v, struct Gwion_ *const gwion) { if(!--v->ref) free_value(v, gwion); }
static inline int vflag(const Value v, const enum vflag flag) {
  return (v->vflag & flag) == flag;
}

#ifndef __cplusplus
static inline void set_vflag(const Value v, const enum vflag flag) {
  v->vflag |= flag;
}
#else
static inline void set_vflag(const Value v, const enum vflag flag) {
  auto ff = v->vflag | flag;
  v->vflag = static_cast<enum vflag>(ff);
}
#endif

ANEW ANN Value new_value(MemPool p, const Type type, const m_str name);
ANN void valuefrom(const Env, struct ValueFrom_*);
#endif
