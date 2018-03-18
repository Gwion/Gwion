struct Context_ {
  m_str     filename;
  Ast       tree;
  Nspc      nspc;
  Class_Def public_class_def;
  struct Map_ label;
  struct VM_Object_ obj;
};

__attribute__((nonnull(2)))
ANEW const Context new_context(const Ast, const m_str);
ANN const m_bool load_context(const Context, const Env);
ANN const m_bool unload_context(const Context, const Env);
ANN void free_context(const Context);
