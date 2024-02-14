#ifndef __PARSE
#define __PARSE

#include "gwion.h"
#define insert_symbol(a) insert_symbol(env->gwion->st, (a))

#define POISON(ok, env)                                                        \
  do {                                                                         \
   /*if((env)->context)*/(env)->context->error = true;                         \
    ok = false;                                                                \
  } while(false)

#define POISON_NODE(ok, env, node)                                             \
  do {                                                                         \
    POISON(ok, env);                                                           \
    (node)-> poison = true;                                                    \
  } while(false)

#undef ERR_b
#define ERR_b(a, b, ...)                                                       \
  do {                                                                         \
    env_err(env, (a), (b), ##__VA_ARGS__);                                     \
    return false;                                                              \
  } while(false)


#define ERR_OK_NODE(ok, a, b, c, ...)                                               \
  do {                                                                         \
    env_err(env, (b), (c), ##__VA_ARGS__);                                     \
    POISON_NODE(ok, env, a);                                                   \
  } while(false)

#define ERR_OK(ok, a, b, ...)                                                 \
  do {                                                                         \
    env_err(env, (a), (b), ##__VA_ARGS__);                                     \
    POISON(ok, env);                                                           \
  } while(false)

#undef ERR_B
#define ERR_B(a, b, ...)                                                       \
  do {                                                                         \
    env_err(env, (a), (b), ##__VA_ARGS__);                                     \
    return GW_ERROR;                                                           \
  } while(false)

#undef ERR_O
#define ERR_O(a, b, ...)                                                       \
  do {                                                                         \
    env_err(env, (a), (b), ##__VA_ARGS__);                                     \
    return NULL;                                                               \
  } while(false)

#define RET_NSPC(exp)                                                          \
  ++env->scope->depth;                                                         \
  nspc_push_value(env->gwion->mp, env->curr);                                  \
  const bool ret = exp;                                                        \
  nspc_pop_value(env->gwion->mp, env->curr);                                   \
  --env->scope->depth;                                                         \
  return ret;

#define SET_ACCESS(a, b)                                                       \
  if (GET_FLAG(a, private))                                                    \
    SET_FLAG(b, private);                                                      \
  else if (GET_FLAG(a, protect))                                               \
    SET_FLAG(b, protect);

#define HANDLE_SECTION_FUNC(prefix, type, Arg)                                 \
  DECL_SECTION_FUNC(prefix, type, Arg)                                         \
  ANN static inline type prefix##_section(const Arg            a,              \
                                          /* const */ Section *section) {      \
    void *d = &section->d.stmt_list;                                           \
    return prefix##_section_func[section->section_type](a, *(void **)d);       \
  }

#define HANDLE_EXP_FUNC(prefix, type, Arg)                                     \
  DECL_EXP_FUNC(prefix, type, Arg)                                             \
  ANN type prefix##_exp(const Arg arg, Exp* exp) {                              \
    do CHECK_BB(prefix##_exp_func[exp->exp_type](arg, &exp->d));               \
    while ((exp = exp->next));                                                 \
    return GW_OK;                                                              \
  }

#define HANDLE_EXP_FUNC_B(prefix, type, Arg)                                   \
  DECL_EXP_FUNC(prefix, type, Arg)                                             \
  ANN type prefix##_exp(const Arg arg, Exp* exp) {                             \
    bool ok = true;                                                            \
    do {                                                                       \
      if(!exp->poison) {                                                       \
        if(!prefix##_exp_func[exp->exp_type](arg, &exp->d)) {                  \
          POISON_NODE(ok, arg, exp);                                           \
        }                                                                      \
      }                                                                        \
    } while ((exp = exp->next));                                               \
    return ok;                                                                 \
  }

#define describe_stmt_func(prefix, name, type, prolog, exp)                    \
  ANN static bool prefix##_stmt_##name(const Env env, const type stmt) {     \
    RET_NSPC(exp)                                                              \
  }

ANN bool check_stmt_list(const Env env, const Stmt_List);

ANN bool scanx_body(const Env e, const Class_Def c, const _envset_func f,
                      void *d);

static inline ANN bool env_body(const Env env, const Class_Def cdef,
                                  const _envset_func f) {
  return scanx_body(env, cdef, f, env);
}
#define env_body(a, b, c) env_body(a, b, (_envset_func)c)

ANN bool scanx_cdef(const Env, void *, const Type, const _envset_func f_cdef,
                      const _envset_func f_union);

#define xxx_cdef(prefix)                                                          \
  static inline bool prefix##_cdef(const Env env, const Type t) {                 \
    return scanx_cdef(env, env, t, (_envset_func)prefix##_class_def,              \
                      (_envset_func)prefix##_union_def);                          \
  }

xxx_cdef(scan0);
xxx_cdef(scan1);
xxx_cdef(scan2);
xxx_cdef(check);
xxx_cdef(traverse);

ANN bool
scanx_fdef(const Env, void *, const Func_Def, const _envset_func);

ANN bool check_subscripts(const Env, const Array_Sub, const bool is_decl);
ANN bool check_implicit(const Env env, Exp* e, const Type t);
ANN bool ensure_traverse(const Env env, const Type t);
ANN bool check_traverse_fdef(const Env env, const Func_Def fdef);

ANN static inline void env_weight(const Env env, const uint16_t weight) {
  if (env->func)
    env->func->weight += weight;
  else if (env->class_def)
    env->class_def->weight += weight;
  else if (env->context)
    env->context->weight += weight;
}

ANN static inline void env_inline_mult(const Env env, const float mult) {
  if (env->func) env->func->inline_mult += mult;
}

ANN static inline bool is_hole(const Env env, Exp* exp) {
  const Symbol hole = insert_symbol("_");
  if (exp->exp_type == ae_exp_primary) {
    if (exp->d.prim.prim_type == ae_prim_id) {
      if (exp->d.prim.d.var == hole) return true;
    }
  }
  return false;
}

static inline bool exp_is_zero(Exp* exp) {
  return exp->exp_type == ae_exp_primary &&
  exp->d.prim.prim_type == ae_prim_num &&
  !exp->d.prim.d.gwint.num;
}

ANN static inline bool not_upvalue(const Env env, const Value v) {
  if (unlikely(is_class(env->gwion, v->type))) return true;
  return GET_FLAG(v, global) || vflag(v, vflag_fglobal) ||
      (v->from->owner_class && isa(v->from->owner_class, env->class_def) > 0) ||
      nspc_lookup_value1(env->curr, insert_symbol(v->name));
}

ANN bool abstract_array(const Env env, const Array_Sub array);

ANN static inline bool is_static_call(const Gwion gwion, Exp* e) {
  if (e->exp_type != ae_exp_dot) return true;
  const Exp_Dot *member = &e->d.exp_dot;
  if(unlikely(!strcmp(s_name(member->xid), "new"))) return true;
  return GET_FLAG(e->type, final) ||
         GET_FLAG(member->base->type, final) ||
         is_class(gwion, member->base->type) ||
         member->base->exp_type == ae_exp_cast;
}

#define is_new(a) !strcmp(s_name((a)->base->tag.sym), "new")
#endif
