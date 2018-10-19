struct Context_ {
  m_str       name;
  Ast         tree;
  Nspc        nspc;
  struct Map_ lbls;
  HAS_OBJ
};

ANN2(2) ANEW Context new_context(const Ast, const m_str);
ANN void load_context(const Context, const Env);
ANN void unload_context(const Context, const Env);
ANN void free_context(const Context);
