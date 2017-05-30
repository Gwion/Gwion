#include "env.h"
#include "dl.h"

typedef m_bool (*Import_fun)(Env env, DL_Func* f);

Func import_mfun(Env env, DL_Func * fun);
Func import_sfun(Env env, DL_Func * fun);

Func_Def make_dll_as_fun(DL_Func * dl_fun, m_bool is_static);
m_int import_svar(Env env, const m_str type, const m_str name,
  const m_bool is_const, const m_bool is_ref, m_uint* addr, const m_str doc);
m_int import_mvar(Env env, const m_str type, const m_str name,
  const m_bool is_const, const m_bool is_ref, const m_str doc );
m_bool import_class_end(Env env);
Type import_class_begin(Env env, Type type, NameSpace where, const f_xtor pre_ctor, const f_xtor dtor);
