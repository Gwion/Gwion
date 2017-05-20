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
  m_bool is_const;
  NameSpace owner;
  Type owner_class;
  VM_Object obj;
  m_str doc;
  m_uint flag;
};

// TODO reorder enum
enum { ae_value_none, ae_value_import, ae_value_global, ae_value_static, ae_value_checked, ae_value_member };
Value new_Value(Context context, Type type, m_str name);
void free_Value(Value a);
