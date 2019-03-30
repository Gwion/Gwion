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

INSTR(EOC) { GWDEBUG_EXE
  vm_shred_exit(shred);
}

INSTR(DTOR_EOC) { GWDEBUG_EXE
  const M_Object o = *(M_Object*)MEM(0);
  o->type_ref = o->type_ref->parent;
  o->ref = 1;
  _release(o, shred);
  vm_shred_exit(shred);
  _release(shred->info->me, shred);
}

/* branching */
INSTR(SwitchIni) {
  const Vector v = (Vector)instr->m_val;
  const m_uint size = vector_size(v);
  const Map m = (Map)instr->m_val2;
  POP_REG(shred, SZ_INT * (size));
  for(m_uint i = 0; i < size; ++i)
    map_set(m, *(vtype*)REG((i) * SZ_INT), vector_at(v, i));
  *(Map*)REG(0) = m;
}

INSTR(BranchSwitch) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  const Map map = *(Map*)REG(SZ_INT);
  shred->pc = map_get(map, *(m_uint*)REG(0)) ?: instr->m_val;
}

INSTR(AutoLoopStart) { GWDEBUG_EXE
  const M_Object o =  *(M_Object*)REG(-SZ_INT);
  if(!o)
    Except(shred, "NullPtrException");
  const m_uint idx = *(m_uint*)MEM(instr->m_val);
  const Type t = (Type)instr->m_val2;
  if(t) {
    M_Object ptr = *(M_Object*)MEM(instr->m_val + SZ_INT);
    if(!idx) {
      ptr = new_object(shred, t);
      *(M_Object*)MEM(instr->m_val + SZ_INT) = ptr;
    }
    *(m_bit**)ptr->data = m_vector_addr(ARRAY(o), idx);
  } else
    m_vector_get(ARRAY(o), idx, MEM(instr->m_val + SZ_INT));
}

INSTR(AutoLoopEnd) { GWDEBUG_EXE
  const M_Object o =  *(M_Object*)REG(-SZ_INT);
  if(++*(m_uint*)MEM(instr->m_val) >= m_vector_size(ARRAY(o))) {
    shred->pc = instr->m_val2;
    POP_REG(shred, SZ_INT);
  }
}

#ifdef OPTIMIZE
INSTR(PutArgsInMem) { GWDEBUG_EXE
  POP_REG(shred, instr->m_val)
  for(m_uint i = 0; i < instr->m_val; i += SZ_INT)
    *(m_uint*)(shred->mem + i) = *(m_uint*)(shred->reg + i);
}
#endif

INSTR(PopArrayClass) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  const M_Object obj = *(M_Object*)REG(-SZ_INT);
  const M_Object tmp = *(M_Object*)REG(0);
  ARRAY(obj) = ARRAY(tmp);
  free_object(tmp);
  ADD_REF(obj->type_ref) // add ref to typedef array type
}
#include "gwion.h"
#include "emit.h"
#include "value.h"
#include "template.h"

ANN static Func_Def from_base(const struct dottmpl_ *dt, const Type t) {
  const Symbol sym = func_symbol(t->name, s_name(dt->base->name),
    "template", dt->overload);
  const Value v = nspc_lookup_value1(t->nspc, sym);
  CHECK_OO(v)
  const Func_Def base = v->d.func_ref->def;
  const Func_Def def = new_func_def(base->td, insert_symbol(v->name),
            base->arg_list, base->d.code, base->flag);
  def->tmpl = new_tmpl_list(base->tmpl->list, dt->overload);
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
    const Func f = nspc_lookup_func1(t->nspc, insert_symbol(str));
    if(f) {
      if(!f->code) {
        dt->def = f->def;//
        dt->owner = t; //
        if(traverse_dot_tmpl(emit, dt) < 0)
          continue;
      }
      *(VM_Code*)shred->reg = f->code;
      shred->reg += SZ_INT;
      return;
    } else {
      const Func_Def def = from_base(dt, t);
      if(!def)
        continue;
      dt->def = def; //
      dt->owner = t; //
      if(traverse_dot_tmpl(emit, dt) > 0) {
        *(VM_Code*)shred->reg = def->func->code;
        shred->reg += SZ_INT;
        return;
      }
    }
  } while((t = t->parent));
  Except(shred, "MissigTmplException[internal]"); //unreachable
}
