#ifndef __VALUE
#define __VALUE
struct ValueFrom_ {
  Nspc owner;
  Type owner_class;
  struct Context_ *ctx;
  size_t offset;
};

struct Value_ {
  Type type;
  m_str name;
  struct ValueFrom_ *from;
  union value_data{
    m_uint* ptr;
    Func func_ref;
  } d;
  HAS_OBJ
  ae_flag flag;
};

ANEW ANN Value new_value(MemPool p, const Type type, const m_str name);
ANN void valuefrom(const Env, struct ValueFrom_*);
#endif
