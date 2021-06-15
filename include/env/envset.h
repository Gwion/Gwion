#ifndef __ENVSET
#define __ENVSET

typedef m_bool (*envset_func)(const void *, const void *);
struct EnvSet {
  const Env         env;
  const envset_func func;
  const void *      data;
  const enum tflag  flag;
  Context           _ctx;
  m_str             _filename;
  const uint16_t    scope;
  bool              run;
};

ANN m_bool envset_run(struct EnvSet *, const Type);
ANN2(1, 3) m_bool envset_push(struct EnvSet *, const Type, const Nspc);
ANN static inline m_bool envset_pushv(struct EnvSet *es, const Value v) {
  return envset_push(es, v->from->owner_class, v->from->owner);
}
ANN2(1) void envset_pop(struct EnvSet *, const Type);

ANN static inline m_bool extend_push(const Env env, const Type t) {
  const Type owner = t->info->value->from->owner_class;
  if (owner) CHECK_BB(extend_push(env, owner));
  return env_push_type(env, t);
}

ANN static inline void extend_pop(const Env env, const Type t) {
  env_pop(env, 0);
  const Type owner = t->info->value->from->owner_class;
  if (owner) extend_pop(env, owner);
}
#endif
