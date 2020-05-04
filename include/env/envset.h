#ifndef __ENVSET
#define __ENVSET

typedef m_bool (*envset_func)(const void*,const void*);
struct EnvSet {
  const Env env;
  const envset_func func;
  const void *data;
  const m_int scope;
  const ae_flag flag;
  m_bool run;
};

ANN m_bool envset_run(struct EnvSet *es, const Type t);
ANN m_bool envset_push(struct EnvSet *es, const Type t);
ANN void   envset_pop(struct EnvSet *es, const Type t);
#endif
