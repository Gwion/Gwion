#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "import.h"
#include "func.h"
#include "array.h"
#include "nspc.h"
#include "shreduler_private.h"

INSTR(DTOR_EOC) {
  const M_Object o = *(M_Object*)MEM(0);
  o->type_ref = o->type_ref->e->parent;
  __release(o, shred);
  shred->info->me->ref = 1;
  vm_shred_exit(shred);
}

/* branching */
INSTR(SwitchIni) {
  const Vector v = (Vector)instr->m_val;
  const m_uint size = vector_size(v);
  for(m_uint i = 0; i < size; ++i)
    map_set((Map)instr->m_val2, *(vtype*)REG((i) * SZ_INT), vector_at(v, i));
}

INSTR(SwitchBranch) {
  POP_REG(shred, SZ_INT);
  const Map map = *(Map*)REG(SZ_INT);
  shred->pc = map_get(map, *(m_uint*)REG(0)) ?: instr->m_val;
}

#ifdef OPTIMIZE
INSTR(PutArgsInMem) {
  POP_REG(shred, instr->m_val)
  for(m_uint i = 0; i < instr->m_val; i += SZ_INT)
    *(m_uint*)(shred->mem + i) = *(m_uint*)(shred->reg + i);
}
#endif

#include "gwion.h"
#include "emit.h"
#include "value.h"
#include "template.h"

INSTR(PopArrayClass) {
  POP_REG(shred, SZ_INT);
  const M_Object obj = *(M_Object*)REG(-SZ_INT);
  const M_Object tmp = *(M_Object*)REG(0);
  ARRAY(obj) = ARRAY(tmp);
  free_object(shred->info->mp, tmp);
  ADD_REF(obj->type_ref) // add ref to typedef array type
}

ANN static Func_Def from_base(const Env env, const struct dottmpl_ *dt, const Nspc nspc) {
  const Func_Def fdef = dt->def ?: dt->base;
  const Symbol sym = func_symbol(env, nspc->name, s_name(fdef->base->xid),
    "template", dt->vt_index);
  DECL_OO(const Value, v, = nspc_lookup_value1(nspc, sym))
  const Func_Def base = v->d.func_ref->def;
  const Func_Def def = new_func_def(env->gwion->mp, new_func_base(env->gwion->mp, fdef->base->td, insert_symbol(env->gwion->st, v->name),
            fdef->base->args), fdef->d.code, fdef->flag, loc_cpy(env->gwion->mp, base->pos));
  def->base->tmpl = new_tmpl(env->gwion->mp, base->base->tmpl->list, dt->vt_index);
  SET_FLAG(def, template);
  return def;
}

INSTR(GTmpl) {
  struct dottmpl_ *dt = (struct dottmpl_*)instr->m_val;
  const Func f = *(Func*)REG(-SZ_INT);
  const m_str name = f->name;
  const Emitter emit = shred->info->vm->gwion->emit;
  emit->env->name = "runtime";
  m_str tmpl_name = tl2str(emit->env, dt->tl);
  for(m_uint i = 0 ; i <= f->value_ref->offset; ++i) {
    const Symbol sym = func_symbol(emit->env, f->value_ref->owner->name,
      name, tmpl_name, i);
    const Func base = nspc_lookup_func1(f->value_ref->owner, sym);
    if(base) {
      xfree(tmpl_name);
      assert(base->code);
      if(GET_FLAG(base->def, static))
        shred->reg -= SZ_INT;
      *(VM_Code*)(shred->reg -SZ_INT) = base->code;
      return;
    }
  }
  xfree(tmpl_name);
  dt->def = f->def;
  const Func_Def def = from_base(emit->env, dt, f->value_ref->owner);
  if(!def)
    Except(shred, "MissigTmplPtrException[internal]");
  dt->def = def;
  dt->owner = f->value_ref->owner;
  dt->owner_class = f->value_ref->owner_class;
  if(traverse_dot_tmpl(emit, dt) > 0) {
    if(GET_FLAG(def, static))
      shred->reg -= SZ_INT;
    *(VM_Code*)(shred->reg -SZ_INT) = def->base->func->code;
    return;
  }
}

INSTR(DotTmpl) {
  struct dottmpl_ *dt = (struct dottmpl_*)instr->m_val;
  const m_str name = dt->name;
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  Type t = o->type_ref;
  do {
    const Emitter emit = shred->info->vm->gwion->emit;
    emit->env->name = "runtime";
    char str[instr->m_val2 + strlen(t->name) + 1];
    strcpy(str, name);
    strcpy(str + instr->m_val2, t->name);
    const Func f = nspc_lookup_func1(t->nspc, insert_symbol(emit->env->gwion->st, str));
    if(f) {
      if(!f->code) {
        dt->def = f->def;//
        dt->owner_class = t; //
        if(traverse_dot_tmpl(emit, dt) < 0)
          continue;
      }
      if(GET_FLAG(f->def, static))
        shred->reg -= SZ_INT;
      *(VM_Code*)shred->reg = f->code;
      shred->reg += SZ_INT;
      return;
    } else {
      const Func_Def def = from_base(emit->env, dt, t->nspc);
      if(!def)
        continue;
      dt->def = def; //
      dt->owner_class = t; //
      if(traverse_dot_tmpl(emit, dt) > 0) {
        if(GET_FLAG(def, static))
          shred->reg -= SZ_INT;
        *(VM_Code*)shred->reg = def->base->func->code;
        shred->reg += SZ_INT;
        return;
      }
    }
  } while((t = t->e->parent));
  Except(shred, "MissigTmplException[internal]"); //unreachable
}
