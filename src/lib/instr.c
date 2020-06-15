#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "array.h"
#include "shreduler_private.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "emit.h"
#include "template.h"

INSTR(DTOR_EOC) {
  const M_Object o = *(M_Object*)MEM(0);
  o->type_ref = o->type_ref->e->parent;
  _release(o, shred);
  shred->info->me->ref = 1;
  vm_shred_exit(shred);
}

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
  DECL_OO(const Value, v, = nspc_lookup_value0(nspc, sym) ?: nspc_lookup_value0(nspc, fdef->base->xid))
  if(isa(v->type, env->gwion->type[et_class]) > 0)
    return NULL;
  const Func_Def def = cpy_func_def(env->gwion->mp, v->d.func_ref->def);
  def->base->tmpl->call = cpy_type_list(env->gwion->mp, dt->tl);
  def->base->tmpl->base = dt->vt_index;
  dt->def = def;
  dt->owner = v->from->owner;
  dt->owner_class = v->from->owner_class;
  SET_FLAG(def, template);
  return def;
}

ANN static Func_Def traverse_tmpl(const Emitter emit, struct dottmpl_ *const dt, const Nspc nspc) {
  DECL_OO(const Func_Def, def, = from_base(emit->env, dt, nspc))
  CHECK_BO(traverse_dot_tmpl(emit, dt))
  return def;
}

INSTR(GTmpl) {
  struct dottmpl_ *dt = (struct dottmpl_*)instr->m_val;
  const Func f = *(Func*)REG(-SZ_INT);
  const m_str name = f->name;
  const Emitter emit = shred->info->vm->gwion->emit;
  emit->env->name = "runtime";
  m_str tmpl_name = tl2str(emit->env, dt->tl);
  for(m_uint i = 0 ; i <= f->value_ref->from->offset; ++i) {
    const Symbol sym = func_symbol(emit->env, f->value_ref->from->owner->name,
      name, tmpl_name, i);
    const Func base = nspc_lookup_func0(f->value_ref->from->owner, sym);
    if(base) {
      free_mstr(emit->gwion->mp, tmpl_name);
      assert(base->code);
      *(VM_Code*)(shred->reg -SZ_INT) = base->code;
      return;
    }
  }
  free_mstr(emit->gwion->mp, tmpl_name);
  dt->def = f->def;
  const Func_Def def = traverse_tmpl(emit, dt, f->value_ref->from->owner);
  if(!def)
    Except(shred, "MissigTmplPtrException[internal]");
  *(VM_Code*)(shred->reg -SZ_INT) = def->base->func->code;
}

INSTR(DotTmpl) {
  struct dottmpl_ *dt = (struct dottmpl_*)instr->m_val;
  const m_str name = dt->name;
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  Type t = !GET_FLAG(o->type_ref, nonnull) ? o->type_ref : o->type_ref->e->parent;
  do {
    const Emitter emit = shred->info->vm->gwion->emit;
    emit->env->name = "runtime";
    char str[instr->m_val2 + strlen(t->name) + 1];
    strcpy(str, name);
    strcpy(str + instr->m_val2, t->name);
    const Func f = nspc_lookup_func0(t->nspc, insert_symbol(emit->env->gwion->st, str));
    if(f) {
      if(!f->code)
        break;
      if(GET_FLAG(f, member))
        shred->reg += SZ_INT;
      *(VM_Code*)(shred->reg-SZ_INT) = f->code;
      return;
    } else {
      const Func_Def def = traverse_tmpl(emit, dt, t->nspc);
      if(!def)
        continue;
      const Func f = def->base->func;
      if(GET_FLAG(f, member))
        shred->reg += SZ_INT;
      *(VM_Code*)(shred->reg-SZ_INT) = f->code;
      return;
    }
  } while((t = t->e->parent));
  Except(shred, "MissigTmplException[internal]");
}

#define VAL (*(m_uint*)(byte + SZ_INT))
#define FVAL (*(m_float*)(byte + SZ_INT))
#define VAL2 (*(m_uint*)(byte + SZ_INT*2))
#define BYTE(a)  m_bit *byte = shred->code->bytecode + (shred->pc -1)* SZ_INT*3; *(m_bit*)byte = a;

INSTR(SetFunc) {
  BYTE(eRegPushImm)
  const Func f = (Func)instr->m_val;
  VAL = *(m_uint*)(shred->reg) = (m_uint)f->code;
  shred->reg += SZ_INT;
}

INSTR(SetRecurs) {
  BYTE(eRegPushImm)
  VAL = *(m_uint*)(shred->reg) = (m_uint)shred->code;
  shred->reg += SZ_INT;
}

INSTR(SetCtor) {
  BYTE(eRegSetImm)
  const Type t = (Type)instr->m_val;
  VAL = *(m_uint*)(shred->reg + SZ_INT) = (m_uint)t->nspc->pre_ctor;
  VAL2 = SZ_INT;
}

