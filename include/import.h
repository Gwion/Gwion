#include <stdlib.h>
#include <complex.h>
#include "defs.h"
#include "vm.h"

#define DLARG_MAX 6

typedef void (*f_xtor)(M_Object o, VM_Shred sh);
typedef void (*f_mfun)(M_Object o, char* RETURN, VM_Shred sh);
typedef void (*f_sfun)(char* RETURN, VM_Shred sh);
typedef struct Importer_* Importer;
void importer_add_arg(Importer importer, const m_str t, const m_str  n);

#define MFUN(a) void a(M_Object o, char* RETURN, VM_Shred shred)
#define SFUN(a) void a(char* RETURN, VM_Shred shred)
#define CTOR(a) void a(M_Object o, VM_Shred shred)
#define DTOR(a) void a(M_Object o, VM_Shred shred)
#define TICK(a) m_bool a(UGen u)
#define IMPORT m_bool import(Importer importer)
#define ALLOC_PTR(a, b, c) b* a = malloc(sizeof(b)); *a =c

m_int importer_class_begin(Importer importer, Type type, const f_xtor pre_ctor, const f_xtor dtor);
m_int importer_add_var(Importer importer, const m_str type, const m_str name, ae_flag flag, m_uint* addr);
void importer_func_begin(Importer importer, const m_str type, const m_str name, m_uint addr);

m_int importer_add_fun(Importer importer, ae_flag flag);
m_int importer_oper_begin(Importer importer, const m_str l, const m_str r, const m_str t);
m_int importer_add_op(Importer importer, const Operator op, const f_instr f, const m_bool global);
m_int importer_class_end(Importer importer);
m_int importer_add_type(Importer importer, Type type);
