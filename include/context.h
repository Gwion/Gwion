struct Context_ {
  m_str     filename;
  Ast       tree;
  Nspc      nspc;
  Class_Def public_class_def;
  struct Map_ label;
  struct VM_Object_ obj;
};

ANN Context new_context(const Ast prog, char* filename);
ANN m_bool load_context(const Context context, const Env env);
ANN m_bool unload_context(const Context context, const Env env);
ANN void free_context(const Context context);
