#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "instr.h"
#include "object.h"
#include "emit.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "mpool.h"
#include "specialid.h"
#include "template.h"

ANN static m_bool mk_xtor(MemPool p, const Type type, const m_uint d, const ae_flag e) {
  VM_Code* code = e == ae_flag_ctor ? &type->nspc->pre_ctor : &type->nspc->dtor;
  const m_str name = type->name;
  *code = new_vm_code(p, NULL, SZ_INT, e | ae_flag_member | ae_flag_builtin, name);
  (*code)->native_func = (m_uint)d;
  type->flag |= e;
  return GW_OK;
}

ANN2(1,2) static inline m_bool class_parent(const Env env, const Type t) {
  Type parent = t->e->parent;
  while(parent && !GET_FLAG(parent, checked)) {
    if(t->e->def)
      CHECK_BB(traverse_class_def(env, t->e->def))
    parent = parent->e->parent;
  }
  return GW_OK;
}

ANN void inherit(const Type t) {
  const Nspc nspc = t->nspc, parent = t->e->parent->nspc;
  if(!nspc || !parent)
    return;
  nspc->info->offset = parent->info->offset;
  if(parent->info->vtable.ptr)
    vector_copy2(&parent->info->vtable, &nspc->info->vtable);
}

ANN2(1,2) static void import_class_ini(const Env env, const Type t) {
  t->nspc = new_nspc(env->gwion->mp, t->name);
  t->nspc->parent = env->curr;
  if(isa(t, env->gwion->type[et_object]) > 0)
    inherit(t);
  t->e->owner = env->curr;
  t->e->owner_class = env->class_def;
  SET_FLAG(t, checked);
  env_push_type(env, t);
}

ANN2(1) void gwi_class_xtor(const Gwi gwi, const f_xtor ctor, const f_xtor dtor) {
  const Type t = gwi->gwion->env->class_def;
  if(ctor)
    mk_xtor(gwi->gwion->mp, t, (m_uint)ctor, ae_flag_ctor);
  if(dtor)
    mk_xtor(gwi->gwion->mp, t, (m_uint)dtor, ae_flag_dtor);
}

ANN static inline void gwi_type_flag(const Type t) {
  SET_FLAG(t, scan1 | ae_flag_scan2 | ae_flag_check | ae_flag_emit);
}

ANN static Type type_finish(const Gwi gwi, const Type t) {
  gwi_add_type(gwi, t);
  CHECK_BO(class_parent(gwi->gwion->env, t))
  import_class_ini(gwi->gwion->env, t);
  return t;
}

ANN2(1,2) Type gwi_class_ini(const Gwi gwi, const m_str name, const m_str parent) {
  struct ImportCK ck = { .name=name };
  CHECK_BO(check_typename_def(gwi, &ck))
  DECL_OO(Type_Decl *,td, = str2decl(gwi, parent ?: "Object"))
  Tmpl* tmpl = ck.tmpl ? new_tmpl_base(gwi->gwion->mp, ck.tmpl) : NULL;
  if(tmpl)
    CHECK_BO(template_push_types(gwi->gwion->env, tmpl))
  const Type p = known_type(gwi->gwion->env, td); // check
  if(!p) {
    env_pop(gwi->gwion->env, 0);
    return NULL;
  }
  if(tmpl)
    nspc_pop_type(gwi->gwion->mp, gwi->gwion->env->curr);
  const Type t = new_type(gwi->gwion->mp, ++gwi->gwion->env->scope->type_xid, s_name(ck.sym), p);
  t->e->def = new_class_def(gwi->gwion->mp, 0, ck.sym, td, NULL, loc(gwi));
  t->e->def->base.tmpl = tmpl;
  t->e->def->base.type = t;
  t->e->tuple = new_tupleform(gwi->gwion->mp, p);
  t->e->parent = p;
  if(td->array)
    SET_FLAG(t, typedef);
  if(ck.tmpl)
    SET_FLAG(t, template);
  else
    gwi_type_flag(t);
  return type_finish(gwi, t);
}

ANN Type gwi_struct_ini(const Gwi gwi, const m_str name, const m_uint size) {
  CHECK_OO(str2sym(gwi, name))
  const Type t = new_type(gwi->gwion->mp, ++gwi->gwion->env->scope->type_xid, name, NULL);
  t->size = size;
  gwi_type_flag(t);
  SET_FLAG(t, struct);
  return type_finish(gwi, t);
}

ANN m_int gwi_class_end(const Gwi gwi) {
  if(!gwi->gwion->env->class_def)
    GWI_ERR_B(_("import: too many class_end called."))
  nspc_allocdata(gwi->gwion->mp, gwi->gwion->env->class_def->nspc);
  env_pop(gwi->gwion->env, 0);
  return GW_OK;
}
