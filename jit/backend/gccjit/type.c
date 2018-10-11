#include <stdlib.h> // exit
#include <string.h>
#include <libgccjit.h>
#include "defs.h"
#include "absyn.h"
#include "operator.h"
#include "instr.h"
#include "jitter.h"
#include "cc.h"

#define cc_get_xxx(name, type) \
type* cc_get_##name(CC cc, const m_str str) {\
  for(m_uint i = 0; i < map_size(&cc->name); ++i) {\
    if(!strcmp(str, (m_str)VKEY(&cc->name, i))) {\
      return (type*)VVAL(&cc->name, i);\
    }\
  }\
puts(__func__);\
puts(str);\
  exit(12);\
}
cc_get_xxx(type, gcc_jit_type)
cc_get_xxx(field, gcc_jit_field)

static void cc_add_type(CC cc, const m_str str, TYPE v) {
  m_str s;
  asprintf(&s, "%s", str);
  map_set(&cc->type, (vtype)s, (vtype)v);
}

static void cc_add_struct(CC cc, const m_str str, STRUCT v) {
  m_str s;
  asprintf(&s, "%s", str);
  map_set(&cc->type, (vtype)s, (vtype)v);
}

struct CCImport {
  CC cc;
  m_str owner;
};

static void cc_add_field(const struct CCImport *cci, const m_str str, const FIELD v) {
  m_str s;
  asprintf(&s, "%s:%s", cci->owner, str);
  map_set(&cci->cc->field, (vtype)s, (vtype)v);
}

static inline FIELD add_field(const struct CCImport *cci, const m_str type, const m_str name) {
  const FIELD f = gcc_jit_context_new_field(cci->cc->parent, NULL, cc_get_type(cci->cc, type), name);
  cc_add_field(cci, name, f);
  return (FIELD)f;
}

#define ADD_FIELD(a, b) add_field(cci, (a), (b))
#define AS_TYPE(a) gcc_jit_struct_as_type((a))
#define PTR_TYPE(a) gcc_jit_type_get_pointer((a))
#define SET_FIELDS(a, b) gcc_jit_struct_set_fields((STRUCT)cc_get_type(cci->cc, "struct " #a), NULL, (b), fields);

static void add_type(const CC cc, const m_str name, TYPE t) {
  char c[strlen(name) + 2];
  cc_add_type(cc, name, t);
  sprintf(c, "%s*", name);
  cc_add_type(cc, c, PTR_TYPE(t));
}

static void declare(CC cc, const m_str name, const m_str cname) {
  STRUCT s = gcc_jit_context_new_opaque_struct(cc->parent, NULL, cname);
  TYPE   t = AS_TYPE(s);
  char c[strlen(name) + 8];
  sprintf(c, "struct %s", name);
  cc_add_struct(cc, c, s);
  sprintf(c, "%s_", name);
  cc_add_type(cc, c, t);
  add_type(cc, name, PTR_TYPE(t));
}

static void import_instr_type(struct CCImport *cci) {
  cci->owner = "instr";
  FIELD fields[4] = {
    ADD_FIELD("pointer", "execute"),
    ADD_FIELD("pointer", "m_val"),
    ADD_FIELD("pointer", "m_val2"),
    ADD_FIELD("pointer", "ptr")
  };
  SET_FIELDS(instr, 4);
}

static void import_oo_type(struct CCImport* cci) {
  cci->owner = "oo";
  FIELD fields[2] = {
    ADD_FIELD("m_uint", "ref_count"),
    ADD_FIELD("m_bool", "type")
  };
  SET_FIELDS(oo, 2);
}

static void import_type_type(struct CCImport* cci) {
  cci->owner = "type";
  FIELD fields[] = {
    ADD_FIELD("m_uint", "xid"),
    ADD_FIELD("m_bool", "size"),
    ADD_FIELD("m_str", "name"),
    ADD_FIELD("pointer", "nspc"), // Nspc
    ADD_FIELD("type", "parent"), // Nspc
    ADD_FIELD("pointer", "owner"), // Nspc
    ADD_FIELD("m_uint", "array_depth"),
    ADD_FIELD("pointer", "def"), // Class_Sef
    ADD_FIELD("pointer", "d"), // union type_data
    ADD_FIELD("pointer", "p"), // union type_data
    ADD_FIELD("m_bool", "flag"),
    ADD_FIELD("struct oo", "obj")
  };
  SET_FIELDS(type, 12);
}

static void import_vector_type(struct CCImport  *cci) {
  cci->owner = "vector";
  FIELD fields[1] = { ADD_FIELD("pointer", "ptr") };
  SET_FIELDS(vector, 1);
}

static void import_object_type(struct CCImport *cci) {
  cci->owner = "object";
  FIELD fields[4] = {
    ADD_FIELD("m_bit*", "data"),
    ADD_FIELD("m_uint", "ref"),
    ADD_FIELD("type", "type_ref"),
    ADD_FIELD("pointer", "p") // pool !
  };
  SET_FIELDS(object, 4);
}

static void import_shred_type(struct CCImport *cci) {
  cci->owner = strdup("shred");
  FIELD fields[17] = {
    ADD_FIELD("pointer", "code"), // code
    ADD_FIELD("shred",   "parent"),
    ADD_FIELD("m_bit*",  "reg"),
    ADD_FIELD("m_bit*",  "mem"),
    ADD_FIELD("m_bit*", "_reg"),
    ADD_FIELD("m_bit*", "base"),
    ADD_FIELD("m_uint", "pc"),
    ADD_FIELD("m_uint", "xid"),
    ADD_FIELD("m_str",  "name"),
    ADD_FIELD("vm", "vm"),
    ADD_FIELD("shred", "prev"),
    ADD_FIELD("shred", "next"),
    ADD_FIELD("vector", "args"),
    ADD_FIELD("object", "me"),
    ADD_FIELD("vector_", "child"),
    ADD_FIELD("vector_", "gc"),
    ADD_FIELD("m_float", "wake_time")
  };
  SET_FIELDS(shred, 17);
}

static void import_shreduler_type(struct CCImport *cci) {
  cci->owner = "shreduler";
  FIELD fields[4] = {
    ADD_FIELD("vm", "vm"),
    ADD_FIELD("shred", "list"),
    ADD_FIELD("shred", "curr"),
    ADD_FIELD("m_bool", "loop")
  };
  SET_FIELDS(shreduler, 4);
}

static void import_vm_type(struct CCImport* cci) {
  cci->owner = "vm";
  FIELD fields[12] = {
    ADD_FIELD("shreduler", "shreduler"),
    ADD_FIELD("object",    "adc"),
    ADD_FIELD("object",    "dac"),
    ADD_FIELD("object",    "blackhole"),
    ADD_FIELD("pointer",   "emit"),      // Emitter !
    ADD_FIELD("vector_",   "shred"),
    ADD_FIELD("vector_",   "ugen"),
    ADD_FIELD("pointer",   "scan"),      // Scanner !
    ADD_FIELD("pointer",   "in"),        // m_float* !
    ADD_FIELD("pointer",   "sp"),        // sp_data* !
    ADD_FIELD("m_uint",    "n_in")
    // is_running
  };
  SET_FIELDS(vm, 11);
}

#ifdef USE_DOUBLE
#define FLOAT_TYPE GCC_JIT_TYPE_DOUBLE
#else
#define FLOAT_TYPE GCC_JIT_TYPE_FLOAT
#endif

#define GET_BASE_TYPE(a) gcc_jit_context_get_type(cc->parent, GCC_JIT_TYPE_##a)
void import_core_type(CC cc) {
  add_type(cc, "void", GET_BASE_TYPE(VOID));
  add_type(cc, "uint", GET_BASE_TYPE(UNSIGNED_INT));
  add_type(cc, "m_bool", GET_BASE_TYPE(INT));
  add_type(cc, "pointer", GET_BASE_TYPE(VOID_PTR));
  add_type(cc, "m_str", PTR_TYPE(GET_BASE_TYPE(CHAR)));
  add_type(cc, "m_bit", GET_BASE_TYPE(UNSIGNED_CHAR));
  add_type(cc, "m_int", gcc_jit_context_get_int_type(cc->parent, SZ_INT, 1));
  add_type(cc, "m_uint", gcc_jit_context_get_int_type(cc->parent, SZ_INT, 0));
  add_type(cc, "m_float", gcc_jit_context_get_type(cc->parent, FLOAT_TYPE));
}

void import_other_type(CC cc) {
  declare(cc, "instr", "Instr");
  declare(cc, "vector", "Vector");
  declare(cc, "oo", "VM_Object");
  declare(cc, "type", "Type");
  declare(cc, "object", "M_Object");
  declare(cc, "shred", "VM_Shred");
  declare(cc, "shreduler", "Shreduler");
  declare(cc, "vm", "vm");
  struct CCImport cci = { cc, NULL };
  import_instr_type(&cci);
  import_vector_type(&cci);
  import_oo_type(&cci);
  import_type_type(&cci);
  import_object_type(&cci);
  import_shred_type(&cci);
  import_shreduler_type(&cci);
  import_vm_type(&cci);
}
