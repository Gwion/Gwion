#include <stdlib.h>
#include <complex.h>
#include "defs.h"
#include "vm.h"

#define DLARG_MAX 6

typedef void (*f_xtor)(M_Object o, VM_Shred sh);
typedef void (*f_mfun)(M_Object o, char* RETURN, VM_Shred sh);
typedef void (*f_sfun)(char* RETURN, VM_Shred sh);
typedef struct Importer_* Importer;

#define MFUN(a) void a(M_Object o, char* RETURN, VM_Shred shred)
#define SFUN(a) void a(char* RETURN, VM_Shred shred)
#define CTOR(a) void a(M_Object o, VM_Shred shred)
#define DTOR(a) void a(M_Object o, VM_Shred shred)
#define TICK(a) m_bool a(UGen u)
#define IMPORT m_bool import(Importer importer)
#define ALLOC_PTR(a, b, c) b* a = (b*)malloc(sizeof(b)); *a =c

m_int importer_add_type(Importer importer, Type type);
m_int importer_add_value(Importer importer, const m_str name, Type type, const m_bool is_const, void* data);


m_int importer_class_ini(Importer importer, Type type, const f_xtor pre_ctor, const f_xtor dtor);
m_int importer_class_end(Importer importer);

m_int importer_item_ini(Importer importer, const m_str type, const m_str name);
m_int importer_item_end(Importer importer, const ae_flag flag, const m_uint* addr);

m_int importer_fptr_ini(Importer importer, const m_str type, const m_str name);
m_int importer_fptr_end(Importer importer, const ae_flag flag);

m_int importer_tmpl_ini(Importer importer, const m_uint n, const m_str *name);
m_int importer_tmpl_end(Importer importer);

m_int importer_union_ini(Importer importer, const m_str name);
m_int importer_union_add(Importer importer, const m_str type, const m_str name);
m_int importer_union_end(Importer importer, ae_flag flag);

m_int importer_enum_ini(Importer importer, const m_str type);
m_int importer_enum_add(Importer importer, const m_str name);
m_int importer_enum_end(Importer importer);

m_int importer_func_ini(Importer importer, const m_str type, const m_str name, m_uint addr);
m_int importer_func_arg(Importer importer, const m_str t, const m_str  n);
m_int importer_func_end(Importer importer, const ae_flag flag);

m_int importer_oper_ini(Importer importer, const m_str l, const m_str r, const m_str t);
m_int importer_oper_add(Importer importer, Type (*check)(Env env, void*));
m_int importer_oper_end(Importer importer, const Operator op, const f_instr f, const m_bool global);

