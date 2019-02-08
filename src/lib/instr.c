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
  _release(shred->me, shred);
  vm_shred_exit(shred);
}

INSTR(EOC2) { GWDEBUG_EXE
  shred->pc = 0;
  shreduler_remove(shred->vm->shreduler, shred, 0);
}

INSTR(RegPushMe) { GWDEBUG_EXE
  *(M_Object*)REG(0) = shred->me;
  PUSH_REG(shred, SZ_INT);
}

INSTR(RegPushMaybe) { GWDEBUG_EXE
  *(m_uint*)REG(0) = gw_rand(shred->vm->rand) > (UINT32_MAX / 2);
  PUSH_REG(shred, SZ_INT);
}

/* branching */
INSTR(SwitchIni) {
  const Vector v = (Vector)instr->m_val;
  const m_uint size = vector_size(v);
  const Map m = (Map)instr->m_val2;
  POP_REG(shred, SZ_INT * (size - 1));
  for(m_uint i = 0; i < size; ++i)
    map_set(m, *(vtype*)REG((i-1) * SZ_INT), vector_at(v, i));
  *(Map*)REG(-SZ_INT) = m;
}

INSTR(BranchSwitch) { GWDEBUG_EXE
  const m_uint offset = *(m_uint*)instr->ptr;
  POP_REG(shred, SZ_INT + offset);
  const Map map = !offset ?(Map)instr->m_val2 : *(Map*)REG(0);
  shred->pc = map_get(map, *(m_uint*)REG(offset));
  if(!shred->pc)
    shred->pc = instr->m_val;
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
  m_uint* idx = (m_uint*)MEM(instr->m_val);
  ++*idx;
  const M_Object o =  *(M_Object*)REG(-SZ_INT);
  if(*idx >= m_vector_size(ARRAY(o))) {
    shred->pc = instr->m_val2;
    POP_REG(shred, SZ_INT);
  }
}

#ifdef OPTIMIZE
INSTR(PutArgsInMem) { GWDEBUG_EXE
  POP_REG(shred, instr->m_val)
  memcpy(shred->mem, shred->reg, instr->m_val);
}

INSTR(ConstPropSet) { GWDEBUG_EXE
  if(*(m_uint*)instr->ptr >= 2) {
    *(m_int**)(shred->reg) = (m_int*)(shred->mem + instr->m_val);
    PUSH_REG(shred, SZ_INT);
    return;
  }
  *(m_int*)(shred->mem + instr->m_val) = instr->m_val2;
  if(*(m_bool*)instr->ptr)
    *(m_int**)(shred->reg) = (m_int*)(shred->mem + instr->m_val);
  else
    *(m_int*)(shred->reg) = instr->m_val2;
  PUSH_REG(shred, SZ_INT);
  *(m_uint*)instr->ptr = 2 + *(m_uint*)instr->ptr;
}

INSTR(ConstPropGet) { GWDEBUG_EXE
  if(!*(m_uint*)instr->ptr)
    memcpy(REG(0), MEM(instr->m_val2), SZ_INT);
  else
    memcpy(REG(0), &instr->m_val, SZ_INT);
  PUSH_REG(shred, SZ_INT);
}
#endif

Type_List tmpl_tl(const Env env, const m_str name) {
//  const Value v = f->value_ref;
m_str start = strchr(name, '<');
m_str end = strchr(name, '@');
char c[strlen(name)];
strcpy(c, start + 1);
c[strlen(start) - strlen(end) - 2] = '\0';
m_uint depth;
  return str2tl(env, c, &depth);
}


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
INSTR(DotTmpl) {
  const m_str name = (m_str)instr->m_val;
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  Type t = o->type_ref;
  do {
    char str[instr->m_val2 + strlen(t->name) + 1];
    strcpy(str, name);
    strcpy(str + instr->m_val2, t->name);
    const Value value = nspc_lookup_value1(t->nspc, insert_symbol(str));
    const Func f = nspc_lookup_func1(t->nspc, insert_symbol(str));
    if(f) {
      if(!f->code) {
      const Emitter emit = shred->vm->gwion->emit;
emit->env->name = "runtime";
  const Value v = f->value_ref;
m_str start = strchr(name, '<');
m_str end = strchr(name, '@');
char c[instr->m_val2];
strcpy(c, start + 1);
c[strlen(start) - strlen(end) - 2] = '\0';
m_uint depth;
const Type_List tl = str2tl(emit->env, c, &depth);
assert(v);
assert(v->d.func_ref->def = f->def);
env_push_type(emit->env, v->owner_class);
if(template_push_types(emit->env, f->def->tmpl->list, tl) > 0)
//  if(traverse_func_def(emit->env, f->value_ref->d.func_ref->def) > 0)
  if(traverse_func_def(emit->env, f->def) > 0)
emit_func_def(emit, f->def);
assert(f->code);
assert(f->code = f->def->func->code);
nspc_pop_type(emit->env->curr);
env_pop(emit->env, 0);
free_type_list(tl);
f->def->func->code->stack_depth -= SZ_INT;
}
      *(VM_Code*)shred->reg = f->code;
      shred->reg += SZ_INT;
      return;
    } else {
      const Emitter emit = shred->vm->gwion->emit;
emit->env->name = "runtime";
//m_str start = strchr(name, '<');
m_str start = name;
m_str end = strchr(name, '<');
char c[instr->m_val2];
strcpy(c, start);
c[strlen(start) - strlen(end)] = '\0';
const Symbol sym = func_symbol(o->type_ref->name, c, "template", 
*(m_uint*)instr->ptr);
    const Value v = nspc_lookup_value1(o->type_ref->nspc, sym);
      const Func_Def base = v->d.func_ref->def; 
      const Func_Def def = new_func_def(base->td, insert_symbol(v->name),
                base->arg_list, base->d.code, base->flag);
      def->tmpl = new_tmpl_list(base->tmpl->list, *(m_int*)instr->ptr);
      SET_FLAG(def, template);
  Type_List tl = tmpl_tl(emit->env, name);
  env_push_type(emit->env, v->owner_class);
  if(template_push_types(emit->env, def->tmpl->list, tl) > 0)
    if(traverse_func_def(emit->env, def) > 0) {
      emit_func_def(emit, def);
    nspc_pop_type(emit->env->curr);
  }
env_pop(emit->env, 0);
def->func->code->stack_depth -= SZ_INT;
      *(VM_Code*)shred->reg = def->func->code;
      shred->reg += SZ_INT;
      return;



}
  } while((t = t->parent));
// should not be reached
  Except(shred, "MissigTmplException[internal]");
}
