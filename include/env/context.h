#ifndef __CONTEXT
#define __CONTEXT
typedef struct Context_ *Context;
struct Context_ {
  Nspc     nspc;
  m_str    name;
  Ast      tree;
  Ast      extend;
  uint16_t ref;
  uint16_t weight;
  bool     error;
  bool     global;
};

REF_FUNC(Context, context)
ANN2(1, 3) ANEW Context new_context(MemPool p, const Ast, const m_str);
ANN void load_context(const Context, const Env);
ANN void unload_context(const Context, const Env);

ANN static inline void env_set_error(const Env env, const bool state) {
  if(env->context) env->context->error = state;
}
#endif
