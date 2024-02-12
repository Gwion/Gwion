#ifndef __PARSE
#define __PARSE

#include "gwion.h"
#define insert_symbol(a) insert_symbol(env->gwion->st, (a))

#undef ERR_b
#define ERR_b(a, b, ...)                                                       \
  {                                                                            \
    env_err(env, (a), (b), ##__VA_ARGS__);                                     \
    return false;                                                              \
  }


#define ERR_OK(ok, a, b, ...)                                                  \
  {                                                                            \
    env_err(env, (a), (b), ##__VA_ARGS__);                                     \
    ok = false;                                                                \
  }

#undef ERR_B
#define ERR_B(a, b, ...)                                                       \
  {                                                                            \
    env_err(env, (a), (b), ##__VA_ARGS__);                                     \
    return GW_ERROR;                                                           \
  }

#undef ERR_O
#define ERR_O(a, b, ...)                                                       \
  {                                                                            \
    env_err(env, (a), (b), ##__VA_ARGS__);                                     \
    return NULL;                                                               \
  }

#define RET_NSPC(exp)                                                          \
  ++env->scope->depth;                                                         \
  nspc_push_value(env->gwion->mp, env->curr);                                  \
  const m_bool ret = exp;                                                      \
  nspc_pop_value(env->gwion->mp, env->curr);                                   \
  --env->scope->depth;                                                         \
  return ret;

#define RET_NSPC_B(exp)                                                        \
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
          exp->poison = true;                                                  \
          ok = false;                                                          \
        }                                                                      \
      }                                                                        \
    } while ((exp = exp->next));                                               \
    return ok;                                                                 \
  }

#define describe_stmt_func(prefix, name, type, prolog, exp)                    \
  ANN static m_bool prefix##_stmt_##name(const Env env, const type stmt) {     \
    RET_NSPC(exp)                                                              \
  }

#define describe_stmt_func_b(prefix, name, type, prolog, exp)                  \
  ANN static bool prefix##_stmt_##name(const Env env, const type stmt) {       \
    RET_NSPC_B(exp)                                                            \
  }

//ANN m_bool check_stmt(const Env env, Stmt* stmt);
ANN bool check_stmt_list(const Env env, const Stmt_List);

typedef m_bool (*_exp_func)(const void *, const void *);
ANN m_bool scanx_body(const Env env, const Class_Def cdef, const _exp_func f,
                      void *data);
static inline ANN m_bool env_body(const Env env, const Class_Def cdef,
                                  const _exp_func f) {
  return scanx_body(env, cdef, f, env);
}
#define env_body(a, b, c) env_body(a, b, (_exp_func)c)

typedef bool (*_exp_func_b)(const void *, const void *);
ANN bool scanx_body_b(const Env env, const Class_Def cdef, const _exp_func_b f,
                      void *data);
static inline ANN bool env_body_b(const Env env, const Class_Def cdef,
                                  const _exp_func_b f) {
  return scanx_body_b(env, cdef, f, env);
}
#define env_body_b(a, b, c) env_body_b(a, b, (_exp_func_b)c)

ANN m_bool scanx_cdef(const Env, void *, const Type, const _exp_func f_cdef,
                      const _exp_func f_union);

#define xxx_section_b(prefix)                                                     \
static inline m_bool prefix##_section_b(const Env env, Section *section) { \
  return prefix##_section(env, section) ? GW_OK : GW_ERROR;}

#define xxx_cdef_b(prefix)                                                     \
static inline m_bool prefix##_class_def_b(const Env env, const Class_Def cdef) { \
  return prefix##_class_def(env, cdef) ? GW_OK : GW_ERROR;} \
static inline m_bool prefix##_union_def_b(const Env env, const Union_Def udef) { \
  return prefix##_union_def(env, udef) ? GW_OK : GW_ERROR;} \
  static inline m_bool prefix##_cdef(const Env env, const Type t) {            \
    return scanx_cdef(env, env, t, (_exp_func)prefix##_class_def_b,              \
                      (_exp_func)prefix##_union_def_b);                          \
  }

xxx_cdef_b(scan0);
xxx_cdef_b(scan1);
xxx_cdef_b(scan2);
xxx_cdef_b(check);
xxx_cdef_b(traverse);

#define xxx_cdef(prefix)                                                       \
  static inline m_bool prefix##_cdef(const Env env, const Type t) {            \
    return scanx_cdef(env, env, t, (_exp_func)prefix##_class_def,              \
                      (_exp_func)prefix##_union_def);                          \
  }

ANN m_bool
scanx_fdef(const Env, void *, const Func_Def, const _exp_func);
ANN bool
scanx_fdef_b(const Env, void *, const Func_Def, const _exp_func_b);

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
