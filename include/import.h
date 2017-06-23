#include "env.h"
#include "dl.h"
#include "instr.h"

#define MFUN(a) void a(M_Object o,  DL_Return * RETURN, VM_Shred shred)
#define SFUN(a) void a(DL_Return * RETURN, VM_Shred shred)
#define CTOR(a) void a(M_Object o, VM_Shred shred)
#define DTOR(a) void a(M_Object o, VM_Shred shred)
#define TICK(a) m_bool a(UGen u)
#define IMPORT m_bool import(Env env)

#define ALLOC_PTR(a, b, c) b* a = malloc(sizeof(b)); *a =c

typedef m_bool(*Import_fun)(Env env, DL_Func* f);

m_int import_mfun(Env env, DL_Func * fun);
m_int import_sfun(Env env, DL_Func * fun);

m_int import_svar(Env env, const m_str type, const m_str name,
                  const m_bool is_const, const m_bool is_ref, m_uint* addr);
m_int import_mvar(Env env, const m_str type, const m_str name,
                  const m_bool is_const, const m_bool is_ref);
m_int import_class_end(Env env);
m_int import_class_begin(Env env, Type type, Nspc where, const f_xtor pre_ctor, const f_xtor dtor);
m_int import_op(Env env, Operator op, m_str l, m_str r, m_str t, f_instr f, m_bool global);
