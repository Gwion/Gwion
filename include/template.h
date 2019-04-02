#ifndef __TEMPLATE
#define __TEMPLATE
ANN m_bool template_push_types(const Env, ID_List, Type_List);
#define POP_RET(a) { nspc_pop_type(env->gwion->p, env->curr); return (a); }
#endif
