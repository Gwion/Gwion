#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "emit.h"
#include "traverse.h"
#include "template.h"
#include "parse.h"
#include "specialid.h"

#include "gwi.h"
#include "gack.h"

#undef insert_symbol

M_Object new_object(MemPool p, const Type t) {
  const uint32_t offset = sizeof(struct M_Object_) + t->nspc->offset;
  const M_Object a = _mp_calloc(p, offset);
  a->ref           = 1;
  a->type_ref      = t;
  return a;
}

M_Object new_string(const struct Gwion_ *gwion, const m_str str) {
  const M_Object o = new_object(gwion->mp, gwion->type[et_string]);
  STRING(o)        = mstrdup(gwion->mp, str);
  return o;
}

ANN static void user_dtor(const M_Object o, const VM_Shred shred, const Type t) {
  const VM_Shred sh = new_vm_shred(shred->info->mp, t->nspc->dtor);
  vmcode_addref(t->nspc->dtor);
  sh->base             = shred->base;
  *(M_Object *)sh->mem = o;
  *(Type *)(sh->mem + SZ_INT) = t;
  vm_add_shred(shred->info->vm, sh);
  ++sh->info->me->ref;
}

ANN static inline void release_not_union(const m_bit *data, const VM_Shred shred, const Scope s) {
  const Map m = &s->map;
  for(m_uint i = map_size(m) + 1; --i;) {
    const Value v = (Value) VVAL(m, i-1);
    if (vflag(v, vflag_release))
      compound_release(shred, v->type, data + v->from->offset);
  }
}

ANN static void do_release(const M_Object o,
                                        const VM_Shred shred, const Type t) {
  if(!t->nspc->offset) {
    free_object(shred->info->mp, o);
    return;
  }
  if (!tflag(t, tflag_union))
    release_not_union(o->data, shred, t->nspc->info->value);
  if (tflag(t, tflag_dtor)) {
    if (t->nspc->dtor->builtin)
      ((f_xtor)t->nspc->dtor->native_func)(o, NULL, shred);
    else
      return user_dtor(o, shred, t);
  }
  return do_release(o, shred, t->info->parent);
}


INSTR(DTOR_EOC) {
  const M_Object o = *(M_Object *)MEM(0);
  const Type t     = *(Type *)MEM(SZ_INT);
  do_release(o, shred, t->info->parent);
  shred->info->me->ref = 1;
  vm_shred_exit(shred);
}

ANN void __release(const M_Object o, const VM_Shred shred) {
  do_release(o, shred, o->type_ref);
}

ANN void free_object(MemPool p, const M_Object o) {
  mp_free2(p, sizeof(struct M_Object_) + o->type_ref->nspc->offset, o);
}

static ID_CHECK(opck_this) {
  if (!env->class_def)
    ERR_O(exp_self(prim)->pos,
          _("keyword 'this' can be used only inside class definition..."))
  if(env->func) {
    if (!vflag(env->func->value_ref, vflag_member))
      ERR_O(exp_self(prim)->pos,
          _("keyword 'this' cannot be used inside static functions..."))
    if (!exp_getuse(exp_self(prim)) &&
        !strcmp(s_name(env->func->def->base->xid), "@gack"))
      return get_gack(env->class_def->info->parent); // get_gack ?
  }
  return env->class_def;
}

static ID_EMIT(opem_this) {
  if (!exp_getvar(exp_self(prim)) &&
      tflag(exp_self(prim)->type, tflag_struct)) {
    const Instr instr = emit_add_instr(emit, RegPushMemDeref);
    instr->m_val2     = emit->env->class_def->size;
    return (Instr)GW_OK;
  }
  const Instr instr = emit_add_instr(emit, RegPushMem);
  instr->m_val      = emit->status.this_offset;
  return instr;
}

GWION_IMPORT(object) {
  const Type t_object = gwi_mk_type(gwi, "Object", SZ_INT, "@Compound");
  gwi_set_global_type(gwi, t_object, et_object);
  t_object->nspc = new_nspc(gwi->gwion->mp, "Object");
  struct SpecialId_ spid = {.ck = opck_this, .em = opem_this, .is_const = 1};
  gwi_specialid(gwi, "this", &spid);
  return GW_OK;
}
