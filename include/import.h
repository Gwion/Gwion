#include "env.h"
#include "dl.h"
#include "instr.h"

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
