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

INSTR(EOC) {
  vm_shred_exit(shred);
}
#include "gwion.h"
INSTR(DTOR_EOC) {
  // TODO: we should be able to use shred->info->mp directly
  shred->info->mp = (MemPool)instr->m_val;
  const M_Object o = *(M_Object*)MEM(0);
  o->type_ref = o->type_ref->parent;
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

ANN static Func_Def from_base(const Env env, const struct dottmpl_ *dt, const Type t) {
  const Symbol sym = func_symbol(env, t->name, s_name(dt->base->base->xid),
    "template", dt->overload);
  const Value v = nspc_lookup_value1(t->nspc, sym);
  CHECK_OO(v)
  const Func_Def base = v->d.func_ref->def;
  const Func_Def def = new_func_def(env->gwion->p, new_func_base(env->gwion->p, base->base->td, insert_symbol(env->gwion->st, v->name),
            base->base->args), base->d.code, base->flag, loc_cpy(env->gwion->p, base->pos));
  def->tmpl = new_tmpl_list(env->gwion->p, base->tmpl->list, dt->overload);
  SET_FLAG(def, template);
  return def;
}

INSTR(DotTmpl) {
  struct dottmpl_ * dt = (struct dottmpl_*)instr->m_val;
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
        dt->owner = t; //
        if(traverse_dot_tmpl(emit, dt) < 0)
          continue;
      }
      if(GET_FLAG(f->def, static))
        shred->reg -= SZ_INT;
      *(VM_Code*)shred->reg = f->code;
      shred->reg += SZ_INT;
      return;
    } else {
      const Func_Def def = from_base(emit->env, dt, t);
      if(!def)
        continue;
      dt->def = def; //
      dt->owner = t; //
      if(traverse_dot_tmpl(emit, dt) > 0) {
        if(GET_FLAG(f->def, static))
          shred->reg -= SZ_INT;
        *(VM_Code*)shred->reg = def->base->func->code;
        shred->reg += SZ_INT;
        return;
      }
    }
  } while((t = t->parent));
  Except(shred, "MissigTmplException[internal]"); //unreachable
}
