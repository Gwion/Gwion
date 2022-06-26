#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "emit.h"

ANN static Func_Def traverse_tmpl(const Emitter emit, Func_Def fdef, Func_Def fbase,
                              const Nspc nspc) {
  const Env env = emit->env;
  const Symbol   sym  = func_symbol(env, nspc->name, s_name(fdef->base->xid),
                                 "template", fbase->vt_index);
  DECL_OO(const Value, v, = nspc_lookup_value0(nspc, sym)
                                ?: nspc_lookup_value0(nspc, fdef->base->xid));
  const f_xfun xfun = v->d.func_ref->def->d.dl_func_ptr;
  if (vflag(v, vflag_builtin))
    v->d.func_ref->def->d.dl_func_ptr = NULL; // why
  const Func_Def def = cpy_func_def(env->gwion->mp, v->d.func_ref->def);
  if (vflag(v, vflag_builtin)) v->d.func_ref->def->d.dl_func_ptr = xfun;
  def->base->tmpl->call = cpy_type_list(env->gwion->mp, fbase->base->tmpl->call);
  CHECK_BO(traverse_dot_tmpl(emit, def, v));
  if (vflag(v, vflag_builtin)) builtin_func(emit->gwion->mp, def->base->func, xfun);
  return def;
}

ANN static void foo(const VM_Shred shred, Type t, const Func_Def fbase, const m_str tmpl_name) {
  const Emitter emit = shred->info->vm->gwion->emit;
  emit->env->name    = "runtime";
  do {
    const Func base = fbase->base->func;
    const Symbol sym  = func_symbol(emit->env, t->name, base->name, tmpl_name, base->def->vt_index);
    const Func f = nspc_lookup_func0(t->nspc, sym);
    if (f) {
      if(!f->code) exit(15); //continue;
      if (vflag(f->value_ref, vflag_member)) shred->reg += SZ_INT;
      *(VM_Code *)(shred->reg - SZ_INT) = f->code;
      return;
    } else {
      const Func_Def def = traverse_tmpl(emit, fbase, fbase, t->nspc);
      if (!def) continue;
      const Func f = def->base->func;
      if (vflag(f->value_ref, vflag_member)) shred->reg += SZ_INT;
      *(VM_Code *)(shred->reg - SZ_INT) = f->code;
      return;
    }
  } while ((t = t->info->parent));
  handle(shred, "MissigTmplPtrException");
}

INSTR(GTmpl) {
  const Func_Def fbase = (Func_Def)instr->m_val;
  const m_str tmpl = (m_str)instr->m_val2;
  const Func       f  = *(Func *)REG(-SZ_INT);
  const Func       _f  = *(Func *)REG(-SZ_INT);
  if (!f) {
    handle(shred, "EmptyFuncPointerException");
     return;
  }
  if (f->code) {
    *(VM_Code *)(shred->reg - SZ_INT) = f->code;
    return;
  }
  const Emitter emit      = shred->info->vm->gwion->emit;
  emit->env->name         = "runtime";
  Type t = f->value_ref->from->owner_class;
  if(!t) {
    const Nspc nspc = f->value_ref->from->owner;
    for (m_uint i = 0; i <= f->value_ref->from->offset; ++i) {
      const Symbol sym  = func_symbol(emit->env, nspc->name, f->name, tmpl, i);
      const Func   base = nspc_lookup_func0(nspc, sym);
      if (base) {
        assert(base->code);
        *(VM_Code *)(shred->reg - SZ_INT) = base->code;
        return;
      }
    }
    const Func_Def def = traverse_tmpl(emit, f->def, fbase, nspc);
    if (def) {
      *(VM_Code *)(shred->reg - SZ_INT) = def->base->func->code;
      return;
    }
  } else {
    do {
      const Symbol sym  = func_symbol(emit->env, t->name, f->name, tmpl, f->def->vt_index);
      const Func f = nspc_lookup_func0(t->nspc, sym);
      if (f) {
        if (!f->code) exit(15); //continue;
        *(VM_Code *)(shred->reg - SZ_INT) = f->code;
        return;
      } else {
        const Func_Def def = traverse_tmpl(emit, _f->def, fbase, t->nspc);
        if (!def) continue;
        const Func f = def->base->func;
        *(VM_Code *)(shred->reg - SZ_INT) = f->code;
        return;
      }
    } while ((t = t->info->parent));
  }
}

INSTR(DotTmpl) {
  const Func_Def fbase = (Func_Def)instr->m_val;
  const m_str tmpl = (m_str)instr->m_val2;
  const M_Object   o    = *(M_Object *)REG(-SZ_INT); // orig
  foo(shred, o->type_ref, fbase, tmpl);
}

#define VAL  (*(m_uint *)(byte + SZ_INT))
#define FVAL (*(m_float *)(byte + SZ_INT))
#define VAL2 (*(m_uint *)(byte + SZ_INT * 2))
#define BYTE(a)                                                                \
  m_bit *byte    = shred->code->bytecode + (shred->pc - 1) * BYTECODE_SZ;       \
  *(m_uint *)byte = a;

INSTR(SetFunc) {
  m_bit *byte    = shred->code->bytecode + (shred->pc - 1) * BYTECODE_SZ;
  *(m_uint *)byte = instr->opcode;
  instr->opcode = eRegPushImm;
  const Func f = (Func)instr->m_val;
  VAL = *(m_uint *)(shred->reg) = (m_uint)f->code;
  shred->reg += SZ_INT;
}

INSTR(SetCtor) {
  BYTE(eRegSetImm)
  const Type t = (Type)instr->m_val;
  VAL = *(m_uint *)(shred->reg + SZ_INT) = (m_uint)t->nspc->pre_ctor;
  VAL2                                   = SZ_INT;
}

INSTR(fast_except) {
  struct FastExceptInfo *info = (struct FastExceptInfo *)instr->m_val2;
  if(*(m_uint*)REG(-SZ_INT)) {
    m_bit *byte    = shred->code->bytecode + (shred->pc - 1) * BYTECODE_SZ;       \
    *(m_uint *)byte = instr->opcode;
    VAL = -SZ_INT;
    instr->opcode = eNoOp;
    if(info) mp_free2(shred->info->mp, sizeof(struct FastExceptInfo), info);
    return;
  } else if(info) {
    if(info->file)
      gwerr_basic("Object not instantiated", NULL, NULL, info->file, info->loc, 0);
    if(info->file2)
      gwerr_warn("declared here", NULL, NULL, info->file2, info->loc2);
    mp_free2(shred->info->mp, sizeof(struct FastExceptInfo), info);
  }
  handle(shred, "NullPtrException");
}
