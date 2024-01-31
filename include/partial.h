#ifndef __PARTIAL_APPLICATION
#define __PARTIAL_APPLICATION

ANN Type partial_type(const Env, Exp_Call *const);
ANN void print_signature(const Func f);

ANN static inline bool func_match_inner(const Env env, Exp* e,
                                        const Type t, const bool implicit,
                                        const bool specific) {
  if (specific ? e->type == t : isa(e->type, t) > 0) // match
    return true;
  return !implicit ? false : check_implicit(env, e, t) > 0;
}

ANN static inline bool is_typed_hole(const Env env, Exp* exp) {
  return exp->exp_type == ae_exp_cast && is_hole(env, exp->d.exp_cast.exp);
}
#endif
