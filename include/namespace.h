#include "defs.h"
#include "vm.h"
#include "operator.h"

struct NameSpace_ {
  m_str     name;
  m_str     filename;
  Scope     value;
  Scope     type;
  Scope     func;
  Map       label; // 09/09/16
  m_uint    offset;
  Vector    obj_v_table;
  NameSpace parent;
  VM_Code   pre_ctor;
  VM_Code   dtor;
  VM_Object obj;
  char*		class_data;
  m_uint    class_data_size;
  Map      	operator;
};

extern NameSpace new_NameSpace();
extern void free_NameSpace(NameSpace a);

extern Value namespace_lookup_value (NameSpace namespace, S_Symbol xid, int climb);
extern Type  namespace_lookup_type  (NameSpace namespace, S_Symbol xid, int climb);
extern Func  namespace_lookup_func  (NameSpace namespace, S_Symbol xid, int climb);

extern void  namespace_commit       (NameSpace namespace);
extern void  namespace_rollback       (NameSpace namespace);

static inline void  namespace_add_value    (NameSpace namespace, S_Symbol xid, Value value)
{  scope_add(namespace->value, xid, (vtype)value); }
static inline void  namespace_push_value   (NameSpace namespace)
{  scope_push(namespace->value); }
static inline void  namespace_pop_value    (NameSpace namespace)
{  scope_pop(namespace->value); }

static inline void namespace_add_func     (NameSpace namespace, S_Symbol xid, Func value)
{  scope_add(namespace->func, xid, (vtype)value); }
static inline void namespace_push_func    (NameSpace namespace)
{  scope_push(namespace->func); }
static inline void namespace_pop_func     (NameSpace namespace)
{  scope_pop(namespace->func); }

static inline void  namespace_add_type     (NameSpace namespace, S_Symbol xid, Type value)
{  scope_add(namespace->type, xid, (vtype)value); }
static inline void  namespace_push_type    (NameSpace namespace)
{  scope_push(namespace->type); }
static inline void namespace_pop_type     (NameSpace namespace)
{  scope_pop(namespace->type); }
