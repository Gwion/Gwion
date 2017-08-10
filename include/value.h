#include "defs.h"
#include "map.h"
#include "oo.h"

struct Value_ {
  Type m_type;
  m_str name;
  m_uint* ptr;
  Func func_ref;
  m_uint offset;
  m_uint func_num_overloads;
  Nspc owner;
  Type owner_class;
  ae_flag flag;
  struct VM_Object_ obj;
};

Value new_value(Type type, m_str name);
void free_value(Value a);
