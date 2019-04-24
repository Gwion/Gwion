#ifndef __VALUE
#define __VALUE
struct Value_ {
  Type type;
  m_str name;
  Nspc owner;
  Type owner_class;
  union value_data{
    m_uint* ptr;
    Func func_ref;
  } d;
  size_t offset;
  HAS_OBJ
  ae_flag flag;
};

ANEW ANN Value new_value(MemPool p, const Type type, const m_str name);
#endif
