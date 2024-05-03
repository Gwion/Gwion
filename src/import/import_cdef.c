#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "instr.h"
#include "emit.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "mpool.h"
#include "specialid.h"
#include "template.h"

ANN void mk_dtor(MemPool p, const Type t, const f_xtor d) {
  VM_Code code = t->nspc->dtor = new_vmcode(p, NULL, NULL, t->name, SZ_INT, true, false);
  code->native_func = (m_uint)d;
  set_tflag(t, tflag_dtor);
}

ANN2(1, 2) static void import_class_ini(const Env env, const Type t) {
  t->nspc         = new_nspc(env->gwion->mp, t->name);
  t->nspc->parent = env->curr;
  if (is_object(env->gwion, t))
    inherit(t);
  env_push_type(env, t);
}

ANN2(1) void add_template(const Env env, const Type t) {
  nspc_push_type(env->gwion->mp, env->curr); //
  Specialized_List sl = get_tmpl(t)->list;
  for(uint32_t i = 0; i < sl->len; i++) {
    Specialized *spec = mp_vector_at(sl, Specialized, i);
    nspc_add_type(env->curr, spec->tag.sym, env->gwion->type[et_auto]);
  }
}

ANN2(1)
void gwi_class_xtor(const Gwi gwi, const f_xtor ctor, const f_xtor dtor) {
  const Type t = gwi->gwion->env->class_def;
  if (dtor) mk_dtor(gwi->gwion->mp, t, dtor);
  if (ctor) {
    gwi_func_ini(gwi, "void", "@ctor");
    gwi_func_end(gwi, ctor, ae_flag_none);
    if(t->nspc->vtable.ptr) set_tflag(t, tflag_ctor);
  }
}

ANN static inline void gwi_type_flag(const Type t) {
  set_tflag(t,
            tflag_scan0 | tflag_scan1 | tflag_scan2 | tflag_check | tflag_emit);
}

ANN static Type type_finish(const Gwi gwi, const Type t) {
  tflag(t, tflag_compound);
  gwi_add_type(gwi, t);
  import_class_ini(gwi->gwion->env, t);
  if (t->info->cdef && get_tmpl(t)) {
    gwi->tmpls++;
    add_template(gwi->gwion->env, t);
    set_tflag(t, tflag_cdef);
  }
  if (gwi->gwion->data->cdoc && t->info->cdef) {
    gwfmt_indent(gwi->gwfmt);
    gwi->gwfmt->indent++;
    gwfmt_class_def(gwi->gwfmt, t->info->cdef);
  }
  if(t->info->cdef && t->info->cdef->base.ext &&
     t->info->cdef->base.ext->array)
    set_tflag(t, tflag_typedef);
  set_tflag(t, tflag_compound);
  return t;
}

ANN2(1, 2)
Type gwi_class_ini(const Gwi gwi, const m_str name, const m_str parent) {
  struct ImportCK ck = {.name = name};
  CHECK_O(check_typename_def(gwi, &ck));
  DECL_O(Type_Decl *, td, = gwi_str2td(gwi, parent ?: "Object"));
  Tmpl *tmpl = ck.sl ? new_tmpl(gwi->gwion->mp, ck.sl) : NULL;
  if (tmpl) CHECK_O(template_push_types(gwi->gwion->env, tmpl));
  DECL_O(const Type, base, = known_type(gwi->gwion->env, td));
  const TmplArg_List tl   = td->types;
  if (tflag(base, tflag_ntmpl)) td->types = NULL;
  const Type p = !td->types ? known_type(gwi->gwion->env, td) : NULL;
  td->types    = tl;
  if (tmpl) nspc_pop_type(gwi->gwion->mp, gwi->gwion->env->curr);
  CHECK_O(p);
  const Type t  = new_type(gwi->gwion->mp, s_name(ck.sym), p);
  t->info->cdef = new_class_def(gwi->gwion->mp, 0, MK_TAG(ck.sym, gwi->loc), td, NULL);
  t->info->cdef->base.tmpl = tmpl;
  t->info->cdef->base.type = t;
  t->info->tuple           = new_tupleform(gwi->gwion->mp, p);
  t->info->parent          = p;
  if (ck.sl)
    set_tflag(t, tflag_tmpl | tflag_ntmpl);
  else
    gwi_type_flag(t);
  return type_finish(gwi, t);
}

ANN Type gwi_struct_ini(const Gwi gwi, const m_str name) {
  struct ImportCK ck = {.name = name};
  CHECK_O(check_typename_def(gwi, &ck));
  const Type t =
      new_type(gwi->gwion->mp, s_name(ck.sym), gwi->gwion->type[et_compound]);
  t->size = 0;
  set_tflag(t, tflag_struct);
  if (!ck.sl)
    gwi_type_flag(t);
  else {
    t->info->cdef =
        new_class_def(gwi->gwion->mp, 0, MK_TAG(ck.sym, gwi->loc), NULL, NULL);
    t->info->cdef->base.type = t;
    t->info->cdef->base.tmpl = new_tmpl(gwi->gwion->mp, ck.sl);
    t->info->tuple           = new_tupleform(gwi->gwion->mp, NULL);
    t->info->parent          = NULL;
    t->info->cdef->cflag |= cflag_struct;
    set_tflag(t, tflag_tmpl | tflag_ntmpl);
  }
  return type_finish(gwi, t);
}

ANN bool gwi_class_end(const Gwi gwi) {
  if (gwi->gwion->data->cdoc && gwi->gwion->env->class_def->info->cdef) {
    gwi->gwfmt->indent--;
    gwfmt_indent(gwi->gwfmt);
    gwfmt_rbrace(gwi->gwfmt);
    gwfmt_nl(gwi->gwfmt);
  }
  if (!gwi->gwion->env->class_def)
    GWI_ERR_B(_("import: too many class_end called."))
  const Type t = gwi->gwion->env->class_def;
  if (t->nspc->class_data_size && !t->nspc->class_data)
    nspc_allocdata(gwi->gwion->mp, t->nspc);
  if (tflag(t, tflag_tmpl)) {
    --gwi->tmpls;
    nspc_pop_type(gwi->gwion->mp, gwi->gwion->env->curr);
  }
/*
// we need to use @ctor
  if (gwi->effects.ptr) {
    vector_init(&t->effects);
    vector_copy2(&gwi->effects, &t->effects);
    vector_release(&gwi->effects);
  }
*/
  env_pop(gwi->gwion->env, 0);
  return true;
}
