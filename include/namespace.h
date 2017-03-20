#include "defs.h"
#include "vm.h"
#include "operator.h"

struct NameSpace_ {
  m_str     name;
  m_str     filename;
  Scope     value;
  Scope     type;
  Scope     func;
  Map   		label; // 09/09/16
  m_uint    offset;
  Vector    obj_v_table;
  NameSpace parent;
  VM_Code   pre_ctor;
  VM_Code   dtor;
  VM_Object obj;
//  m_uint*		class_data;
  char*		class_data;
  m_uint    class_data_size;
  Map      	operator;
};

extern Value namespace_lookup_value(NameSpace namespace, S_Symbol xid, int climb);
extern void  namespace_add_value    (NameSpace namespace, S_Symbol xid, Value value);
extern void  namespace_push_value   (NameSpace namespace);
extern void  namespace_pop_value    (NameSpace namespace);

extern Type  namespace_lookup_type  (NameSpace namespace, S_Symbol xid, int climb);
extern void  namespace_add_type     (NameSpace namespace, S_Symbol xid, Type value);
extern void  namespace_push_type    (NameSpace namespace);
extern void  namespace_pop_type     (NameSpace namespace);

extern Func  namespace_lookup_func  (NameSpace namespace, S_Symbol xid, int climb);
extern void  namespace_add_func     (NameSpace namespace, S_Symbol xid, Func value);
//extern void  namespace_push_func    (NameSpace namespace);
//extern void  namespace_pop_func     (NameSpace namespace);

extern void  namespace_commit       (NameSpace namespace);
extern void  namespace_rollback     (NameSpace namespace);

extern NameSpace new_NameSpace();
extern void free_NameSpace(NameSpace a);
