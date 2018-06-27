struct Context_ {
  m_str     filename;
  Ast       tree;
  Nspc      nspc;
  Class_Def public_class_def;
  struct Map_ label;
  struct VM_Object_ obj;
};

ANN2(2) ANEW Context new_context(const Ast, const m_str);
ANN void load_context(const Context, const Env);
ANN void unload_context(const Context, const Env);
ANN void free_context(const Context);
