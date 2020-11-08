#ifndef __ENVSET
#define __ENVSET

typedef m_bool (*envset_func)(const void*,const void*);
struct EnvSet {
  const Env env;
  const envset_func func;
  const void *data;
  const m_int scope;
  const enum tflag flag;
  m_bool run;
};

ANN m_bool envset_run(struct EnvSet*, const Type);
ANN2(1,3) m_bool envset_push(struct EnvSet*, const Type, const Nspc);
ANN2(1) void   envset_pop(struct EnvSet*, const Type);
#endif
