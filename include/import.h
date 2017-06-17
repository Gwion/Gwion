#include "env.h"
#include "dl.h"

#ifdef DEBUG_STACK
#define PUSH_MEM(a, b) \
{\
  a->mem_index += b; \
  a->mem += b;\
}
#define POP_MEM(a, b) \
{\
  a->mem_index -= b; \
  a->mem -= b;\
}
#define PUSH_REG(a, b) \
{\
  a->reg_index += b; \
  a->reg += b;\
}
#define POP_REG(a, b) \
{\
  a->reg_index -= b; \
  a->reg -= b;\
}
#else
#define PUSH_MEM(a, b) a->mem += b;
#define POP_MEM(a, b)  a->mem -= b;
#define PUSH_REG(a, b) a->reg += b;
#define POP_REG(a, b)  a->reg -= b;
#endif

#define INSTR(a) void a(VM* vm, VM_Shred shred, Instr instr)
#define MFUN(a) void a(M_Object o,  DL_Return * RETURN, VM_Shred shred)
#define SFUN(a) void a(DL_Return * RETURN, VM_Shred shred)
#define CTOR(a) void a(M_Object o, VM_Shred shred)
#define DTOR(a) void a(M_Object o, VM_Shred shred)
#define TICK(a) m_bool a(UGen u)
#define IMPORT m_bool import(Env env)

#define ALLOC_PTR(a, b, c) b* a = malloc(sizeof(b)); *a =c

typedef m_bool(*Import_fun)(Env env, DL_Func* f);

Func import_mfun(Env env, DL_Func * fun);
Func import_sfun(Env env, DL_Func * fun);

Func_Def make_dll_as_fun(DL_Func * dl_fun, m_bool is_static);
m_int import_svar(Env env, const m_str type, const m_str name,
                  const m_bool is_const, const m_bool is_ref, m_uint* addr, const m_str doc);
m_int import_mvar(Env env, const m_str type, const m_str name,
                  const m_bool is_const, const m_bool is_ref, const m_str doc);
m_bool import_class_end(Env env);
Type import_class_begin(Env env, Type type, Nspc where, const f_xtor pre_ctor, const f_xtor dtor);
Type get_type(Env env, m_str str);
m_bool import_op(Env env, Operator op, m_str l, m_str r, m_str t, f_instr f, m_bool global);
