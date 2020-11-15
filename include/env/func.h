#ifndef __FUNC
#define __FUNC

enum fflag {
  fflag_none  = 1 << 0,
  fflag_pure  = 1 << 1,
  fflag_ftmpl = 1 << 2,
  fflag_tmpl  = 1 << 3,
  fflag_valid = 1 << 4,
} __attribute__((packed));

struct Func_ {
  m_str name;
  Func_Def def;
  struct VM_Code_* code;
  Value value_ref;
  Func next;
  size_t vt_index;
  struct Map_ upvalues;
  uint16_t ref;
  ae_flag flag;
  enum fflag fflag;
};

ANN void free_func(const Func, struct Gwion_*const);
ANN static inline void func_addref(const Func f) { ++f->ref; }
ANN static inline void func_remref(const Func f, struct Gwion_ *const gwion) { if(!--f->ref) free_func(f, gwion); }

static inline int fflag(const Func f, const enum fflag flag) {
  return (f->fflag & flag) == flag;
}

static inline int safe_fflag(const Func f, const enum fflag flag) {
  return f ? ((f->fflag & flag) == flag) : 0;
}
#ifndef __cplusplus
static inline void set_fflag(const Func f, const enum fflag flag) {
  f->fflag |= flag;
}

static inline void unset_fflag(const Func f, const enum fflag flag) {
  f->fflag &= ~flag;
}
#else
static inline void set_fflag(const Func f, const enum fflag flag) {
  const auto ff = f->fflag | flag;
  f->fflag = static_cast<enum fflag>(ff);
}

static inline void unset_fflag(const Func f, const enum fflag flag) {
  const auto ff = f->fflag & ~flag;
  f->fflag = static_cast<enum fflag>(ff);
}
#endif

ANEW ANN Func new_func(MemPool, const m_str, const Func_Def);
ANN2(1,2) Symbol func_symbol(const Env, const m_str, const m_str, const m_str, const m_uint);
ANN m_bool check_lambda(const Env, const Type, Exp_Lambda*);
ANN void builtin_func(const MemPool mp, const Func f, void* func_ptr);
#endif
