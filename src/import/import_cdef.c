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

ANN static m_bool mk_xtor(MemPool p, const Type type, const m_uint d,
                          const enum tflag e) {
  VM_Code *code = e == tflag_ctor ? &type->nspc->pre_ctor : &type->nspc->dtor;
  const m_str name     = type->name;
  *code                = new_vmcode(p, NULL, NULL, name, SZ_INT, true, false);
  (*code)->native_func = (m_uint)d;
  type->tflag |= e;
  return GW_OK;
}

ANN2(1, 2) static void import_class_ini(const Env env, const Type t) {
  t->nspc         = new_nspc(env->gwion->mp, t->name);
  t->nspc->parent = env->curr;
  if (isa(t, env->gwion->type[et_object]) > 0) inherit(t);
  env_push_type(env, t);
}

ANN2(1) void add_template(const Env env, const Type t) {
  nspc_push_type(env->gwion->mp, env->curr); //
  Specialized_List sl = t->info->cdef->base.tmpl->list;
  for(uint32_t i = 0; i < sl->len; i++) {
    Specialized *spec = mp_vector_at(sl, Specialized, i);
    nspc_add_type(env->curr, spec->xid, env->gwion->type[et_auto]);
  }
}

ANN2(1)
void gwi_class_xtor(const Gwi gwi, const f_xtor ctor, const f_xtor dtor) {
  const Type t = gwi->gwion->env->class_def;
  if (ctor) mk_xtor(gwi->gwion->mp, t, (m_uint)ctor, tflag_ctor);
  if (dtor) mk_xtor(gwi->gwion->mp, t, (m_uint)dtor, tflag_dtor);
}

ANN static inline void gwi_type_flag(const Type t) {
  set_tflag(t,
            tflag_scan0 | tflag_scan1 | tflag_scan2 | tflag_check | tflag_emit);
}

ANN static Type type_finish(const Gwi gwi, const Type t) {
  gwi_add_type(gwi, t);
  import_class_ini(gwi->gwion->env, t);
  if (t->info->cdef && t->info->cdef->base.tmpl) {
    gwi->tmpls++;
    add_template(gwi->gwion->env, t);
    set_tflag(t, tflag_cdef);
  }
  if (gwi->gwion->data->cdoc && t->info->cdef) {
    lint_indent(gwi->lint);
    gwi->lint->indent++;
    lint_class_def(gwi->lint, t->info->cdef);
  }
  if(t->info->cdef && t->info->cdef->base.ext &&
     t->info->cdef->base.ext->array)
    set_tflag(t, tflag_typedef);
  return t;
}

ANN2(1, 2)
Type gwi_class_ini(const Gwi gwi, const m_str name, const m_str parent) {
  struct ImportCK ck = {.name = name};
  CHECK_BO(check_typename_def(gwi, &ck));
  DECL_OO(Type_Decl *, td, = gwi_str2td(gwi, parent ?: "Object"));
  Tmpl *tmpl = ck.sl ? new_tmpl(gwi->gwion->mp, ck.sl) : NULL;
  if (tmpl) CHECK_BO(template_push_types(gwi->gwion->env, tmpl));
  const Type      base = find_type(gwi->gwion->env, td);
  const Type_List tl   = td->types;
  if (tflag(base, tflag_ntmpl)) td->types = NULL;
  const Type p = !td->types ? known_type(gwi->gwion->env, td) : NULL;
  td->types    = tl;
  if (tmpl) nspc_pop_type(gwi->gwion->mp, gwi->gwion->env->curr);
  CHECK_OO(p);
  const Type t  = new_type(gwi->gwion->mp, s_name(ck.sym), p);
  t->info->cdef = new_class_def(gwi->gwion->mp, 0, ck.sym, td, NULL, gwi->loc);
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
  CHECK_BO(check_typename_def(gwi, &ck));
  const Type t =
      new_type(gwi->gwion->mp, s_name(ck.sym), gwi->gwion->type[et_compound]);
  t->size = 0;
  set_tflag(t, tflag_struct);
  if (!ck.sl)
    gwi_type_flag(t);
  else {
    t->info->cdef =
        new_class_def(gwi->gwion->mp, 0, ck.sym, NULL, NULL, gwi->loc);
    t->info->cdef->base.type = t;
    t->info->cdef->base.tmpl = new_tmpl(gwi->gwion->mp, ck.sl);
    t->info->tuple           = new_tupleform(gwi->gwion->mp, NULL);
    t->info->parent          = NULL;
    t->info->cdef->cflag |= cflag_struct;
    set_tflag(t, tflag_tmpl | tflag_ntmpl);
  }
  return type_finish(gwi, t);
}

ANN m_int gwi_class_end(const Gwi gwi) {
  if (gwi->gwion->data->cdoc && gwi->gwion->env->class_def->info->cdef) {
    gwi->lint->indent--;
    lint_indent(gwi->lint);
    lint_rbrace(gwi->lint);
    lint_nl(gwi->lint);
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
  if (gwi->effects.ptr) {
    vector_init(&t->effects);
    vector_copy2(&gwi->effects, &t->effects);
    vector_release(&gwi->effects);
  }
  env_pop(gwi->gwion->env, 0);
  return GW_OK;
}
