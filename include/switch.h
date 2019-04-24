#ifndef __SWITCH
#define __SWITCH
ANN Map switch_map(const Env env);
ANN Vector switch_vec(const Env env);
ANN void switch_expset(const Env env, const Exp);
ANN Exp switch_expget(const Env env);
ANN void switch_pc(const Env env, const m_uint pc);
ANN void switch_dynpc(const Env env, const m_int val, const m_uint pc);
ANN m_bool switch_dup(const Env env, const m_int value, const loc_t pos);
ANN m_bool switch_pop(const Env env);
ANN m_bool switch_end(const Env env, const loc_t pos);
ANN m_uint switch_idx(const Env);
ANN Vector switch_vec(const Env);
ANN m_bool switch_add(const Env env, const Stmt_Switch stmt);
ANN m_bool switch_default(const Env env, const m_uint pc, const loc_t pos);
ANN m_bool switch_inside(const Env env, const loc_t pos);
ANN m_bool switch_dyn(const Env env);
ANN m_bool switch_decl(const Env env, const loc_t pos);
ANN void switch_reset(const Env env);
ANN void switch_release(const Scope);
ANN void switch_get(const Env env, const Stmt_Switch stmt);
#endif
