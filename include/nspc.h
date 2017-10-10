#include "defs.h"
#include "vm.h"
#include "operator.h"
#include "oo.h"
#include "map_private.h"

struct Nspc_ {
  m_str     name;
  m_str     filename;
  m_uint    offset;
  Nspc parent;
  VM_Code   pre_ctor;
  VM_Code   dtor;
  char*		class_data;
  m_uint    class_data_size;
  struct Vector_    obj_v_table;
  struct Map_      	op_map;
  struct Scope_     value;
  struct Scope_     type;
  struct Scope_     func;
  struct VM_Object_ obj;
};

extern Nspc new_nspc(m_str name, m_str filename);
extern void free_nspc(Nspc a);

extern Value nspc_lookup_value0(Nspc nspc, S_Symbol xid);
extern Value nspc_lookup_value1(Nspc nspc, S_Symbol xid);
extern Value nspc_lookup_value2(Nspc nspc, S_Symbol xid);
extern Type  nspc_lookup_type0(Nspc nspc, S_Symbol xid);
extern Type  nspc_lookup_type1(Nspc nspc, S_Symbol xid);
extern Type  nspc_lookup_type2(Nspc nspc, S_Symbol xid);
extern Func  nspc_lookup_func0(Nspc nspc, S_Symbol xid);
extern Func  nspc_lookup_func1(Nspc nspc, S_Symbol xid);
extern Func  nspc_lookup_func2(Nspc nspc, S_Symbol xid);

extern Vector nspc_get_value(Nspc nspc);

extern void  nspc_commit(Nspc nspc);
extern void  nspc_rollback(Nspc nspc);

void nspc_add_value(Nspc nspc, S_Symbol xid, Value value);
void nspc_push_value(Nspc nspc);
void nspc_pop_value(Nspc nspc);

void nspc_add_func(Nspc nspc, S_Symbol xid, Func value);
void nspc_push_func(Nspc nspc);
void nspc_pop_func(Nspc nspc);

void nspc_add_type(Nspc nspc, S_Symbol xid, Type value);
void nspc_push_type(Nspc nspc);
void nspc_pop_type(Nspc nspc);
