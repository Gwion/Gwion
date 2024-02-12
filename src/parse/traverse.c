#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "traverse.h"

ANN bool traverse_ast(const Env env, Ast *const ast) {
  CHECK_B(scan0_ast(env, ast));
  CHECK_B(scan1_ast(env, ast));
  CHECK_B(scan2_ast(env, ast));
  CHECK_B(check_ast(env, ast));
  return true;
}

ANN bool traverse_exp(const Env env, Exp* exp) {
  CHECK_B(scan1_exp(env, exp));
  CHECK_B(scan2_exp(env, exp));
  CHECK_B(check_exp(env, exp));
  return true;
}

ANN static bool _traverse_func_def(const Env env, const Func_Def fdef) {
  CHECK_B(scan0_func_def(env, fdef));
  CHECK_B(scan1_func_def(env, fdef));
  CHECK_B(scan2_func_def(env, fdef));
  CHECK_B(check_func_def(env, fdef));
  return true;
}

ANN bool traverse_func_def(const Env env, const Func_Def fdef) {
  const Func   former = env->func;
  const bool ret    = _traverse_func_def(env, fdef);
  env->func           = former;
  return ret;
}

ANN bool traverse_union_def(const Env env, const Union_Def def) {
  //  if(!GET_FLAG(def, scan1))
  CHECK_B(scan1_union_def(env, def));
  //  if(!GET_FLAG(def, scan2))
  CHECK_B(scan2_union_def(env, def));
  //  if(!GET_FLAG(def, check))
  CHECK_B(check_union_def(env, def));
  return true;
}

ANN bool traverse_enum_def(const Env env, const Enum_Def def) {
  CHECK_B(scan0_enum_def(env, def));
  CHECK_B(scan1_enum_def(env, def));
  //  CHECK_B(scan2_enum_def(env, def));
  //  CHECK_B(check_enum_def(env, def));
  return true;
}

ANN bool traverse_fptr_def(const Env env, const Fptr_Def def) {
  CHECK_B(scan0_fptr_def(env, def));
  CHECK_B(scan1_fptr_def(env, def));
  CHECK_B(scan2_fptr_def(env, def));
  CHECK_B(check_fptr_def(env, def));
  return true;
}

ANN bool traverse_type_def(const Env env, const Type_Def def) {
  CHECK_B(scan0_type_def(env, def));
  CHECK_B(scan1_type_def(env, def));
  CHECK_B(scan2_type_def(env, def));
  CHECK_B(check_type_def(env, def));
  return true;
}

ANN bool traverse_class_def(const Env env, const Class_Def def) {
  const Type t = def->base.type;
  if (!tflag(t, tflag_scan1)) CHECK_B(scan1_class_def(env, def));
  if (!tflag(t, tflag_scan2)) CHECK_B(scan2_class_def(env, def));
  if (!tflag(t, tflag_check)) CHECK_B(check_class_def(env, def));
  return true;
}
