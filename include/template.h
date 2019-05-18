#ifndef __TEMPLATE
#define __TEMPLATE
ANN m_bool template_push_types(const Env, const Tmpl*);
#define POP_RET(a) { nspc_pop_type(env->gwion->mp, env->curr); return (a); }
#endif
