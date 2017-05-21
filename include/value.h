#include "defs.h"
#include "map.h"
#include "oo.h"

struct Value_ {
  te_type type;
  Type m_type;
  m_str name;
  m_uint* ptr;
  Func func_ref;
  m_uint offset;
  m_uint func_num_overloads;
  NameSpace owner;
  Type owner_class;
  m_str doc;
  m_uint flag;
  struct VM_Object_ obj;
};

// TODO reorder enum
enum { ae_value_none, ae_value_import, ae_value_global, ae_value_static,
  ae_value_checked, ae_value_member, ae_value_const, ae_value_enum, ae_value_arg };
Value new_Value(Type type, m_str name);
void free_Value(Value a);
