#include <stdlib.h>
#include <string.h>
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

M_Object new_object(MemPool p, const VM_Shred shred, const Type t) {
  const M_Object a = mp_calloc(p, M_Object);
  a->ref           = 1;
  a->type_ref      = t;
  if (t->nspc) {
    a->vtable.ptr = t->nspc->vtable.ptr;
    if (t->nspc->offset)
      a->data = (m_bit *)_mp_calloc(p, t->nspc->offset);
  }
  if (shred) vector_add(&shred->gc, (vtype)a);
  return a;
}

M_Object new_string(MemPool p, const VM_Shred shred, const m_str str) {
  const M_Object o =
      new_object(p, shred, shred->info->vm->gwion->type[et_string]);
  STRING(o) = mstrdup(p, str);
  return o;
}

M_Object new_string2(const struct Gwion_ *gwion, const VM_Shred shred,
                     const m_str str) {
  const M_Object o = new_object(gwion->mp, shred, gwion->type[et_string]);
  STRING(o)        = mstrdup(gwion->mp, str);
  return o;
}

ANN static void handle_dtor(const M_Object o, const VM_Shred shred) {
  const VM_Shred sh = new_vm_shred(shred->info->mp, o->type_ref->nspc->dtor);
  vmcode_addref(o->type_ref->nspc->dtor);
  sh->base             = shred->base;
  *(M_Object *)sh->mem = o;
  vm_add_shred(shred->info->vm, sh);
  ++sh->info->me->ref;
}

__attribute__((hot)) ANN void __release(const M_Object o,
                                        const VM_Shred shred) {
  vector_rem2(&shred->gc, (vtype)o);
  MemPool p = shred->info->mp;
  Type    t = o->type_ref;
  do {
    if (!t->nspc) continue;
    if (isa(t, shred->info->vm->gwion->type[et_union]) < 0) {
      struct scope_iter iter = {t->nspc->info->value, 0, 0};
      Value             v;
      while (scope_iter(&iter, &v) > 0) {
        if (!GET_FLAG(v, static) &&
            isa(v->type, shred->info->vm->gwion->type[et_compound]) > 0)
          compound_release(shred, v->type, o->data + v->from->offset);
      }
    }
    if (tflag(t, tflag_dtor)) {
      if (t->nspc->dtor->builtin)
        ((f_xtor)t->nspc->dtor->native_func)(o, NULL, shred);
      else {
        o->type_ref = t;
        handle_dtor(o, shred);
        return;
      }
    }
  } while ((t = t->info->parent));
  free_object(p, o);
}

ANN void free_object(MemPool p, const M_Object o) {
  if (o->type_ref->nspc && o->type_ref->nspc->offset)
    mp_free2(p, o->type_ref->nspc->offset, o->data);
  mp_free(p, M_Object, o);
}

static ID_CHECK(opck_this) {
  if (!env->class_def)
    ERR_O(exp_self(prim)->pos,
          _("keyword 'this' can be used only inside class definition..."))
  if (env->func && !vflag(env->func->value_ref, vflag_member))
    ERR_O(exp_self(prim)->pos,
          _("keyword 'this' cannot be used inside static functions..."))
  if (!exp_getuse(exp_self(prim)) && env->func &&
      !strcmp(s_name(env->func->def->base->xid), "@gack"))
    return get_gack(env->class_def->info->parent); // get_gack ?
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
  instr->m_val      = emit->this_offset;
  return instr;
}

GWION_IMPORT(object) {
  const Type t_object = gwi_mk_type(gwi, "Object", SZ_INT, "@Compound");
  gwi_set_global_type(gwi, t_object, et_object);
  struct SpecialId_ spid = {.ck = opck_this, .em = opem_this, .is_const = 1};
  gwi_specialid(gwi, "this", &spid);
  return GW_OK;
}
