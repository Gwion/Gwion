#include "defs.h"
#include "vm.h"
#include "operator.h"
#include "oo.h"
#include "map_private.h"

struct Nspc_ {
  m_str     name;
  m_uint    offset;
  Nspc parent;
  VM_Code   pre_ctor;
  VM_Code   dtor;
  char*		class_data;
  m_uint    class_data_size;
  struct Vector_    vtable;
  struct Map_      	op_map;
  struct Scope_     value;
  struct Scope_     type;
  struct Scope_     func;
  struct VM_Object_ obj;
};

extern Nspc new_nspc(const m_str name);
extern void free_nspc(Nspc a);

extern Value nspc_lookup_value0(const Nspc nspc, const S_Symbol xid) ANN;
extern Value nspc_lookup_value1(const Nspc nspc, const S_Symbol xid) ANN;
extern Value nspc_lookup_value2(const Nspc nspc, const S_Symbol xid) ANN;
extern Type  nspc_lookup_type0(const Nspc nspc, const S_Symbol xid)  ANN;
extern Type  nspc_lookup_type1(const Nspc nspc, const S_Symbol xid)  ANN;
extern Type  nspc_lookup_type2(const Nspc nspc, const S_Symbol xid)  ANN;
extern Func  nspc_lookup_func0(const Nspc nspc, const S_Symbol xid)  ANN;
extern Func  nspc_lookup_func1(const Nspc nspc, const S_Symbol xid)  ANN;
extern Func  nspc_lookup_func2(const Nspc nspc, const S_Symbol xid)  ANN;

extern Vector nspc_get_value(const Nspc);

extern void  nspc_commit(const Nspc) ANN;
extern void  nspc_rollback(const Nspc) ANN;

void nspc_add_value(const Nspc, const S_Symbol, const Value) ANN;
void nspc_push_value(const Nspc) ANN;
void nspc_pop_value(const Nspc) ANN;

void nspc_add_func(const Nspc, const S_Symbol, const Func) ANN;
void nspc_push_func(const Nspc) ANN;
void nspc_pop_func(const Nspc) ANN;

void nspc_add_type(const Nspc, const S_Symbol, const Type) ANN;
void nspc_push_type(const Nspc) ANN;
void nspc_pop_type(const Nspc) ANN;
