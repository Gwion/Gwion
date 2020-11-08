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
ANN void exception(const VM_Shred shred, const m_str c) {
  gw_err("%s: shred[id=%" UINT_F ":%s], PC=[%" UINT_F "]\n",
          c, shred->tick->xid, shred->info->name, shred->pc - 1);
  vm_shred_exit(shred);
}

M_Object new_object(MemPool p, const VM_Shred shred, const Type t) {
  const M_Object a = mp_calloc(p, M_Object);
  a->ref = 1;
  a->type_ref = t;
  if(t->nspc) {
    a->vtable = &t->nspc->info->vtable;
    if(t->nspc->info->offset)
      a->data = (m_bit*)_mp_calloc(p, t->nspc->info->offset);
  }
  if(shred)
    vector_add(&shred->gc, (vtype)a);
  return a;
}

M_Object new_string(MemPool p, const VM_Shred shred, const m_str str) {
  const M_Object o = new_object(p, shred, shred->info->vm->gwion->type[et_string]);
  STRING(o) = s_name(insert_symbol(shred->info->vm->gwion->st, str));
  return o;
}

M_Object new_string2(const struct Gwion_ *gwion, const VM_Shred shred, const m_str str) {
  const M_Object o = new_object(gwion->mp, shred, gwion->type[et_string]);
  STRING(o) = str;
  return o;
}

ANN static void handle_dtor(const M_Object o, const VM_Shred shred) {
  const VM_Shred sh = new_vm_shred(shred->info->mp, o->type_ref->nspc->dtor);
  vmcode_addref(o->type_ref->nspc->dtor);
  sh->base = shred->base;
  *(M_Object*)sh->mem = o;
  vm_add_shred(shred->info->vm, sh);
  ++sh->info->me->ref;
}

__attribute__((hot))
ANN void __release(const M_Object o, const VM_Shred shred) {
  MemPool p = shred->info->mp;
  Type t = o->type_ref;
  do {
    if(!t->nspc || is_special(t))
      continue;
    struct scope_iter iter = { t->nspc->info->value, 0, 0 };\
    Value v;
    while(scope_iter(&iter, &v) > 0) {
      if(!GET_FLAG(v, static) && !vflag(v, vflag_union) &&
          isa(v->type, shred->info->vm->gwion->type[et_object]) > 0)
        release(*(M_Object*)(o->data + v->from->offset), shred);
      else if(tflag(v->type, tflag_struct) &&
            !GET_FLAG(v, static) && !vflag(v, vflag_union) && v->type->e->tuple) {
        const TupleForm tf = v->type->e->tuple;
        for(m_uint i = 0; i < vector_size(&tf->types); ++i) {
          const m_bit *data = o->data + v->from->offset;
          const Type t = (Type)vector_at(&tf->types, i);
          if(isa(t, shred->info->vm->gwion->type[et_object]) > 0)
            release(*(M_Object*)(data + vector_at(&tf->offset, i)), shred);
        }
      }
    }
    if(tflag(t, tflag_dtor) && t->nspc->dtor) {
    // check flag for array types
      if(t->nspc->dtor->builtin)
        ((f_xtor)t->nspc->dtor->native_func)(o, NULL, shred);
      else {
        o->type_ref = t;
        handle_dtor(o, shred);
        return;
      }
    }
  } while((t = t->e->parent));
  free_object(p, o);
}

ANN void free_object(MemPool p, const M_Object o) {
  if(o->type_ref->nspc && o->type_ref->nspc->info->offset)
    mp_free2(p, o->type_ref->nspc->info->offset, o->data);
  mp_free(p, M_Object, o);
}

static ID_CHECK(opck_this) {
  if(!env->class_def)
    ERR_O(exp_self(prim)->pos, _("keyword 'this' can be used only inside class definition..."))
  if(env->func && !vflag(env->func->value_ref, vflag_member))
      ERR_O(exp_self(prim)->pos, _("keyword 'this' cannot be used inside static functions..."))
  if(env->func && !strcmp(s_name(env->func->def->base->xid), "@gack"))
    return force_type(env, get_gack(env->class_def->e->parent)); // get_gack ?
  return env->class_def;
}

static ID_EMIT(opem_this) {
  if(!exp_getvar(exp_self(prim)) && tflag(exp_self(prim)->info->type, tflag_struct)) {
    const Instr instr = emit_add_instr(emit, RegPushMemDeref);
    instr->m_val2 = emit->env->class_def->size;
    return (Instr)GW_OK;
  }
  return emit_add_instr(emit, RegPushMem);
}

GWION_IMPORT(object) {
  const Type t_object  = gwi_mk_type(gwi, "Object", SZ_INT, "@Compound");
  gwi_set_global_type(gwi, t_object, et_object);
  struct SpecialId_ spid = { .ck=opck_this, .em=opem_this, .is_const=1 };
  gwi_specialid(gwi, "this", &spid);
  return GW_OK;
}
