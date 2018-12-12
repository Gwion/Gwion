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
  struct Gwion_ *gwion;
  size_t offset;
  ae_flag flag;
  HAS_OBJ
};

ANEW ANN Value new_value(struct Gwion_* gwion, const Type type, const m_str name);
ANN void free_value(Value a);
#endif
