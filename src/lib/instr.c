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
#include "func.h"
#include "array.h"
#include "nspc.h"
#include "shreduler_private.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "cpy_ast.h"

INSTR(DTOR_EOC) {
  const M_Object o = *(M_Object*)MEM(0);
  o->type_ref = o->type_ref->e->parent;
  _release(o, shred);
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
}

ANN static Func_Def from_base(const Env env, struct dottmpl_ *const dt, const Nspc nspc) {
  const Func_Def fdef = dt->def ?: dt->base;
  const Symbol sym = func_symbol(env, nspc->name, s_name(fdef->base->xid),
    "template", dt->vt_index);
  DECL_OO(const Value, v, = nspc_lookup_value0(nspc, sym))
  const Func_Def def = cpy_func_def(env->gwion->mp, v->d.func_ref->def);
  def->base->tmpl->call = dt->tl;
  def->base->tmpl->base = dt->vt_index;
  dt->def = def;
  dt->owner = v->owner;
  dt->owner_class = v->owner_class;
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
    const Func base = nspc_lookup_func0(f->value_ref->owner, sym);
    if(base) {
      free_mstr(emit->gwion->mp, tmpl_name);
      assert(base->code);
      *(VM_Code*)(shred->reg -SZ_INT) = base->code;
      return;
    }
  }
  free_mstr(emit->gwion->mp, tmpl_name);
  dt->def = f->def;
  const Func_Def def = from_base(emit->env, dt, f->value_ref->owner);
  if(!def)
    Except(shred, "MissigTmplPtrException[internal]");
  if(traverse_dot_tmpl(emit, dt) > 0)
    *(VM_Code*)(shred->reg -SZ_INT) = def->base->func->code;
  else
    Except(shred, "TemplateException");
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
    const Func f = nspc_lookup_func0(t->nspc, insert_symbol(emit->env->gwion->st, str));
    if(f) {
      *(VM_Code*)shred->reg = f->code;
      shred->reg += SZ_INT;
      return;
    } else {
      const Func_Def def = from_base(emit->env, dt, t->nspc);
      if(!def)
        continue;
      if(traverse_dot_tmpl(emit, dt) > 0) {
        *(VM_Code*)shred->reg = def->base->func->code;
        shred->reg += SZ_INT;
        return;
      } else break;
    }
  } while((t = t->e->parent));
  Except(shred, "MissigTmplException[internal]"); //unreachable
}
