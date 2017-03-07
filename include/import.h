#include "env.h"
#include "dl.h"

typedef m_bool (*Import_fun)(Env env, DL_Func* f);

//Func import_fun(Env env, DL_Func * mfun, m_bool is_static);
//#define import_sfun(env, fun) import_fun(1, env, fun)
//#define import_fun(1, env, fun) import_sfun(env, fun)
//#define import_sfun(env, fun) import_fun(env, fun, 0)
Func import_mfun(Env env, DL_Func * fun);
Func import_sfun(Env env, DL_Func * fun);

Func_Def make_dll_as_fun(DL_Func * dl_fun, m_bool is_static);
m_int import_svar(Env env, const m_str type,
                  const m_str name, m_bool is_const, m_bool is_ref, m_uint* addr, const m_str doc );
m_int import_mvar(Env env, const m_str type,
                  const m_str name, const m_bool is_const, const m_bool is_ref, const m_str doc );
m_bool import_class_end(Env env);
Type import_class_begin(Env env, Type type, NameSpace where, f_ctor pre_ctor, f_dtor dtor);
//Type check_Decl_Expression(Env env, Decl_Expression* decl);
//m_bool import_class(Env env, Type type, DL_Func* fun, Import_fun f, f_ctor pre_ctor, f_dtor dtor);
