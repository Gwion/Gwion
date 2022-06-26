#ifndef __SPREAD
#define __SPREAD

ANN static inline bool is_spread_tmpl(const Tmpl *tmpl) {
  const Specialized *spec = mp_vector_at(tmpl->list, Specialized, tmpl->list->len - 1);
  return !strcmp(s_name(spec->xid), "...");
}

ANN m_bool spread_ast(const Env env, const Spread_Def spread, const Tmpl *tmpl);
ANN Stmt_List spread_func(const Env env, const Stmt_List body);

#endif
