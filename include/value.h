struct Value_ {
  Type type;
  m_str name;
  Nspc owner;
  Type owner_class;
  m_uint offset;
  union value_data{
    m_uint* ptr;
    Func func_ref;
  } d;
  ae_flag flag;
  HAS_OBJ
};

ANEW ANN Value new_value(const Type type, const m_str name);
ANN void free_value(Value a);
