#ifndef __CONTEXT
#define __CONTEXT
typedef struct Context_   * Context;
struct Context_ {
  m_str       name;
  Ast         tree;
  Nspc        nspc;
  struct Map_ lbls;
  m_bool error;
  m_bool global;
  uint16_t ref;
};

REF_FUNC(Context, context)
ANN2(1,3) ANEW Context new_context(MemPool p, const Ast, const m_str);
ANN void load_context(const Context, const Env);
ANN void unload_context(const Context, const Env);
#endif
