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
  NameSpace owner;
  Type owner_class;
  m_str doc;
  m_uint flag;
  struct VM_Object_ obj;
};

// TODO reorder enum
enum {
  ae_value_none,
  ae_value_import    = 1 << 1,
  ae_value_global    = 1 << 2,
  ae_value_static    = 1 << 3,
  ae_value_checked   = 1 << 4,
  ae_value_member    = 1 << 5,
  ae_value_const     = 1 << 6,
  ae_value_enum      = 1 << 7,
  ae_value_arg       = 1 << 8
};

Value new_value(Type type, m_str name);
