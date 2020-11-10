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

ANN void free_context(const Context, struct Gwion_*const);
ANN static inline void context_addref(const Context c) { ++c->ref; }
ANN static inline void context_remref(const Context c, struct Gwion_ *const gwion) { if(!--c->ref) free_context(c, gwion); }
ANN2(1,3) ANEW Context new_context(MemPool p, const Ast, const m_str);
ANN void load_context(const Context, const Env);
ANN void unload_context(const Context, const Env);
#endif
