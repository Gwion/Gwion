#ifndef __TEMPLATE
#define __TEMPLATE
ANN bool template_push_types(const Env, const Tmpl *);
ANN bool template_push(const Env env, const Type t);
ANN Tmpl *mk_tmpl(const Env, const Tmpl *, const TmplArgList*);
/*
//! returns the Tmpl of a class or enum def
ANN static inline Tmpl* get_tmpl(const Type t) {
  return tflag(t, tflag_cdef)
      ?  t->info->cdef->base.tmpl
//      :  tflag(t, tflag_udef)
        : t->info->udef->tmpl;
//        : NULL;
}
*/
ANN static inline Tmpl* get_tmpl(const Type t) {
  return tflag(t, tflag_udef)
      ? t->info->udef->tmpl
      :  t->info->cdef->base.tmpl;
//      :  tflag(t, tflag_udef)
//        : NULL;
}

#define POP_RET(a)                                                             \
  {                                                                            \
    nspc_pop_type(env->gwion->mp, env->curr);                                  \
    return (a);                                                                \
  }

ANN bool const_generic_typecheck(const Env env, const Specialized *spec, const TmplArg *targ);
#endif
