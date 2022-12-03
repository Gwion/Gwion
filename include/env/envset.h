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
//  es->_ctx         = es->env->context;
//  es->_filename    = es->env->name;
  CHECK_BB(envset_push(es, v->from->owner_class, v->from->owner));
//  es->env->context = v->from->ctx;
//  es->env->name    = v->from->filename;
  return GW_OK;
}
ANN2(1) void envset_pop(struct EnvSet *, const Type);
#endif
