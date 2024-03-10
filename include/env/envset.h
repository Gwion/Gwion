#ifndef __ENVSET
#define __ENVSET

typedef bool (*_envset_func)(const void *, const void *);
struct EnvSet {
  const Env         env;
  const _envset_func func;
  const void *      data;
  const enum tflag  flag;
  Context           _ctx;
  m_str             _filename;
  const uint16_t    scope;
  bool              run;
};

ANN bool envset_run(struct EnvSet *, const Type);
ANN2(1, 3) bool envset_push(struct EnvSet *, const Type, const Nspc);
ANN2(1) void envset_pop(struct EnvSet *, const Type);

ANN static inline bool envset_pushv(struct EnvSet *es, const Value v) {
  es->_ctx         = es->env->context;
  es->_filename    = es->env->name;
  return envset_push(es, v->from->owner_class, v->from->owner);
}
ANN2(1) static inline void envset_popv(struct EnvSet *es, const Value v) {
  envset_pop(es, v->from->owner_class);
}

ANN static inline bool envset_pushf(struct EnvSet *es, const Value owner) {
  CHECK_B(envset_pushv(es, owner));
  /*return*/ env_pushv(es->env, owner);
  return true;
}
ANN2(1) static inline void envset_popf(struct EnvSet *es, const Value owner) {
  env_pop(es->env, es->scope);
  envset_popv(es, owner);
}
#endif
