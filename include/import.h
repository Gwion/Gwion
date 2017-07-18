#include <stdlib.h>
#include <complex.h>
#include "defs.h"
#include "vm.h"

#define DLARG_MAX 6

void dl_return_push(const char* retval, VM_Shred shred, m_uint size);

typedef void (*f_xtor)(M_Object o, VM_Shred sh);
typedef void (*f_mfun)(M_Object o, char* RETURN, VM_Shred sh);
typedef void (*f_sfun)(char* RETURN, VM_Shred sh);

typedef struct {
  m_str name;
  m_str type;
} DL_Value;

typedef struct {
  m_str    name;
  m_str    type;
  m_uint   addr;
  m_uint   narg;
  DL_Value args[DLARG_MAX];
} DL_Func;


void dl_func_init(DL_Func* fun, const m_str t, const m_str n, const m_uint addr);
void dl_func_add_arg(DL_Func* a, const m_str t, const m_str  n);

#define MFUN(a) void a(M_Object o, char* RETURN, VM_Shred shred)
#define SFUN(a) void a(char* RETURN, VM_Shred shred)
#define CTOR(a) void a(M_Object o, VM_Shred shred)
#define DTOR(a) void a(M_Object o, VM_Shred shred)
#define TICK(a) m_bool a(UGen u)
#define IMPORT m_bool import(Env env)

#define ALLOC_PTR(a, b, c) b* a = malloc(sizeof(b)); *a =c

typedef m_bool(*Import_fun)(Env env, DL_Func* f);

m_int import_class_begin(Env env, Type type, Nspc where, const f_xtor pre_ctor, const f_xtor dtor);
m_int import_var(Env env, const m_str type, const m_str name, ae_flag flag, m_uint* addr);
m_int import_fun(Env env, DL_Func * fun, ae_flag flag);
m_int import_op(Env env, const Operator op, const m_str l, const m_str r, const m_str t, const f_instr f, const m_bool global);
m_int import_class_end(Env env);
