#include "defs.h"
#include "map.h"
#include "oo.h"

struct Value_ {
  te_type type;
  Type m_type;
  m_str name;
//	void* ptr;
  m_uint* ptr;
  Func func_ref;
  m_uint offset;
  m_uint func_num_overloads;
  m_bool checked;
  m_bool is_const;
  m_bool is_member;
  m_bool is_static;
  m_bool is_context_global;
  NameSpace owner;
  Type owner_class;
  VM_Object obj;
  m_str doc;

  m_bool is_import;// use this?
//m_bool is_template;// template
};

Value new_Value(Context context, Type type, m_str name);
void free_Value(Value a);
