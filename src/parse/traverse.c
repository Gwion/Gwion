#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "traverse.h"

ANN m_bool traverse_ast(const Env env, Ast *const ast) {
  CHECK_BB(scan0_ast(env, ast));
  CHECK_BB(scan1_ast(env, ast));
  CHECK_BB(scan2_ast(env, ast));
  return check_ast(env, ast);
}

ANN m_bool traverse_exp(const Env env, const Exp exp) {
  CHECK_BB(scan1_exp(env, exp));
  CHECK_BB(scan2_exp(env, exp));
  return check_exp(env, exp) ? 1 : -1;
}

ANN static m_bool _traverse_func_def(const Env env, const Func_Def fdef) {
  CHECK_BB(scan0_func_def(env, fdef));
  CHECK_BB(scan1_func_def(env, fdef));
  CHECK_BB(scan2_func_def(env, fdef));
  return check_func_def(env, fdef);
}

ANN m_bool traverse_func_def(const Env env, const Func_Def fdef) {
  const Func   former = env->func;
  const m_bool ret    = _traverse_func_def(env, fdef);
  env->func           = former;
  return ret;
}

ANN m_bool traverse_union_def(const Env env, const Union_Def def) {
  //  if(!GET_FLAG(def, scan1))
  CHECK_BB(scan1_union_def(env, def));
  //  if(!GET_FLAG(def, scan2))
  CHECK_BB(scan2_union_def(env, def));
  //  if(!GET_FLAG(def, check))
  CHECK_BB(check_union_def(env, def));
  return check_union_def(env, def);
}

ANN m_bool traverse_enum_def(const Env env, const Enum_Def def) {
  CHECK_BB(scan0_enum_def(env, def));
  CHECK_BB(scan1_enum_def(env, def));
  //  CHECK_BB(scan2_enum_def(env, def));
  return check_enum_def(env, def);
}

ANN m_bool traverse_fptr_def(const Env env, const Fptr_Def def) {
  CHECK_BB(scan0_fptr_def(env, def));
  CHECK_BB(scan1_fptr_def(env, def));
  CHECK_BB(scan2_fptr_def(env, def));
  return check_fptr_def(env, def);
}

ANN m_bool traverse_type_def(const Env env, const Type_Def def) {
  CHECK_BB(scan0_type_def(env, def));
  CHECK_BB(scan1_type_def(env, def));
  CHECK_BB(scan2_type_def(env, def));
  return check_type_def(env, def);
}

ANN m_bool traverse_class_def(const Env env, const Class_Def def) {
  const Type t = def->base.type;
  if (!tflag(t, tflag_scan1)) CHECK_BB(scan1_class_def(env, def));
  if (!tflag(t, tflag_scan2)) CHECK_BB(scan2_class_def(env, def));
  if (!tflag(t, tflag_check)) return check_class_def(env, def);
  return GW_OK;
}
