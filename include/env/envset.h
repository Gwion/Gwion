#ifndef __ENVSET
#define __ENVSET

// couldbe _exp_func
typedef bool (*envset_func)(const void *, const void *);
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

ANN bool envset_run(struct EnvSet *, const Type);
ANN2(1, 3) bool envset_push(struct EnvSet *, const Type, const Nspc);
ANN static inline bool envset_pushv(struct EnvSet *es, const Value v) {
  es->_ctx         = es->env->context;
  es->_filename    = es->env->name;
  CHECK_B(envset_push(es, v->from->owner_class, v->from->owner));
  return true;
}
ANN2(1) void envset_pop(struct EnvSet *, const Type);
#endif
