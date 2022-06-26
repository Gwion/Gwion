#include <stdlib.h>
#include <math.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "gwi.h"
#include "emit.h"

static DTOR(basic_dtor) { xfree(UGEN(o)->module.gen.data); }

static TICK(gain_tick) { u->out = (u->in * *(m_float *)u->module.gen.data); }

static CTOR(gain_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), gain_tick,
           (m_float *)xmalloc(SZ_FLOAT), 0);
  UGEN(o)->module.gen.tick             = gain_tick;
  *(m_float *)UGEN(o)->module.gen.data = 1;
}

static MFUN(gain_get_gain) {
  *(m_float *)RETURN = *(m_float *)UGEN(o)->module.gen.data;
}

static MFUN(gain_set_gain) {
  *(m_float *)RETURN = *(m_float *)UGEN(o)->module.gen.data =
      *(m_float *)MEM(SZ_INT);
}

static GWION_IMPORT(gain) {
  GWI_OB(gwi_class_ini(gwi, "Gain", "UGen"))
  gwi_class_xtor(gwi, gain_ctor, basic_dtor);
  gwi_func_ini(gwi, "float", "gain");
  GWI_BB(gwi_func_end(gwi, gain_get_gain, ae_flag_none))
  gwi_func_ini(gwi, "float", "gain");
  gwi_func_arg(gwi, "float", "arg0");
  GWI_BB(gwi_func_end(gwi, gain_set_gain, ae_flag_none))
  return gwi_class_end(gwi);
}

static TICK(impulse_tick) {
  u->out                         = *(m_float *)u->module.gen.data;
  *(m_float *)u->module.gen.data = 0;
}

static CTOR(impulse_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), impulse_tick,
           (m_float *)xmalloc(SZ_FLOAT), 0);
  *(m_float *)UGEN(o)->module.gen.data = 0;
}

static MFUN(impulse_get_next) {
  *(m_float *)RETURN = *(m_float *)UGEN(o)->module.gen.data;
}

static MFUN(impulse_set_next) {
  *(m_float *)RETURN =
      (*(m_float *)UGEN(o)->module.gen.data = *(m_float *)MEM(SZ_INT));
}

static GWION_IMPORT(impulse) {
  GWI_OB(gwi_class_ini(gwi, "Impulse", "UGen"))
  gwi_class_xtor(gwi, impulse_ctor, basic_dtor);
  gwi_func_ini(gwi, "float", "next");
  GWI_BB(gwi_func_end(gwi, impulse_get_next, ae_flag_none))
  gwi_func_ini(gwi, "float", "next");
  gwi_func_arg(gwi, "float", "arg0");
  GWI_BB(gwi_func_end(gwi, impulse_set_next, ae_flag_none))
  return gwi_class_end(gwi);
}

static TICK(fullrect_tick) { u->out = fabs(u->in); }

static CTOR(fullrect_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), fullrect_tick,
           (m_float *)xmalloc(SZ_FLOAT), 0);
  *(m_float *)UGEN(o)->module.gen.data = 1;
}

static GWION_IMPORT(fullrect) {
  GWI_OB(gwi_class_ini(gwi, "FullRect", "UGen"))
  gwi_class_xtor(gwi, fullrect_ctor, basic_dtor);
  return gwi_class_end(gwi);
}

static TICK(halfrect_tick) {
  if (u->in > 0)
    u->out = u->in;
  else
    u->out = 0;
}

static CTOR(halfrect_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), halfrect_tick,
           (m_float *)xmalloc(SZ_FLOAT), 0);
  *(m_float *)UGEN(o)->module.gen.data = 1;
}

static GWION_IMPORT(halfrect) {
  GWI_OB(gwi_class_ini(gwi, "HalfRect", "UGen"))
  gwi_class_xtor(gwi, halfrect_ctor, basic_dtor);
  return gwi_class_end(gwi);
}

static TICK(step_tick) { u->out = *(m_float *)u->module.gen.data; }

static CTOR(step_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), step_tick,
           (m_float *)xmalloc(SZ_FLOAT), 0);
  *(m_float *)UGEN(o)->module.gen.data = 0;
}

static MFUN(step_get_next) {
  *(m_float *)RETURN = *(m_float *)UGEN(o)->module.gen.data;
}

static MFUN(step_set_next) {
  *(m_float *)RETURN = *(m_float *)UGEN(o)->module.gen.data =
      *(m_float *)(shred->mem + SZ_INT);
}

static GWION_IMPORT(step) {
  GWI_OB(gwi_class_ini(gwi, "Step", "UGen"))
  gwi_class_xtor(gwi, step_ctor, basic_dtor);
  gwi_func_ini(gwi, "float", "next");
  GWI_BB(gwi_func_end(gwi, step_get_next, ae_flag_none))
  gwi_func_ini(gwi, "float", "next");
  gwi_func_arg(gwi, "float", "arg0");
  GWI_BB(gwi_func_end(gwi, step_set_next, ae_flag_none))
  return gwi_class_end(gwi);
}

static TICK(zerox_tick) {
  m_float in                     = (u->in < 0) ? -1 : (u->in > 0);
  m_float f                      = *(m_float *)u->module.gen.data;
  u->out                         = f == in ? 1 : 0;
  *(m_float *)u->module.gen.data = in;
}

static CTOR(zerox_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), zerox_tick,
           (m_float *)xmalloc(SZ_FLOAT), 0);
  *(m_float *)UGEN(o)->module.gen.data = 1;
}

static GWION_IMPORT(zerox) {
  GWI_OB(gwi_class_ini(gwi, "ZeroX", "UGen"))
  gwi_class_xtor(gwi, zerox_ctor, basic_dtor);
  return gwi_class_end(gwi);
}

struct UUGen_ {
  M_Object self;
  VM_Code  code;
  VM_Shred shred;
  void (*prep)(struct UUGen_ *, const m_float);
};

ANN static void global_prep(struct UUGen_ *uu, const m_float in) {
  *(m_float *)uu->shred->mem = in;
}

ANN static void member_prep(struct UUGen_ *uu, const m_float in) {
  *(M_Object *)uu->shred->mem           = uu->self;
  *(m_float *)(uu->shred->mem + SZ_INT) = in;
}

static TICK(id_tick) { u->out = u->in; }

static TICK(usrugen_tick) {
  struct UUGen_ *uu = u->module.gen.data;
  uu->prep(uu, u->in);
  uu->shred->pc = 0;
  shredule(uu->shred->tick->shreduler, uu->shred, 0);
  vm_run(uu->shred->info->vm);
  uu->shred->reg -= SZ_FLOAT;
  u->out = *(m_float *)(uu->shred->reg);
}

static CTOR(usrugen_ctor) {
  struct UUGen_ *uu = mp_calloc(shred->info->mp, UUGen);
  uu->self          = o;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), id_tick, uu, 0);
}

static DTOR(usrugen_dtor) {
  struct UUGen_ *uu = UGEN(o)->module.gen.data;
  if (uu->shred) free_vm_shred(uu->shred);
  mp_free(shred->info->mp, UUGen, UGEN(o)->module.gen.data);
}

static OP_CHECK(opck_usrugen) {
  Exp_Binary *   bin = (Exp_Binary *)data;
  const Arg_List arg = bin->lhs->type->info->func->def->base->args;
  if (!arg || arg->len > 1)
    ERR_N(exp_self(bin)->pos,
          _("Tick function take one and only one argument"));
  if (isa(((Arg*)(arg->ptr))->type, env->gwion->type[et_float]) < 0)
    ERR_N(exp_self(bin)->pos,
          _("Tick functions argument must be of type float"));
  if (isa(bin->lhs->type->info->func->def->base->ret_type,
          env->gwion->type[et_float]) < 0)
    ERR_N(exp_self(bin)->pos, _("Tick function must return float"));
  if (bin->lhs->type->info->func->value_ref->from->owner_class)
    CHECK_BN(isa(bin->lhs->type->info->func->value_ref->from->owner_class,
                 bin->rhs->type));
  return bin->rhs->type;
}

static INSTR(UURet) { shreduler_remove(shred->tick->shreduler, shred, false); }

ANN static void code_prepare(const VM_Code code) {
  m_bit *byte = code->bytecode;
  for (m_uint i = 0; i < vector_size(&code->instr); ++i) {
    if (*(m_bit *)(byte + i * BYTECODE_SZ) == eFuncReturn) {
      *(m_bit *)(byte + i * BYTECODE_SZ)                  = eOP_MAX;
      *(f_instr *)(byte + (i * BYTECODE_SZ) + SZ_INT * 2) = UURet;
    }
  }
}

static MFUN(default_tick) {
  struct UUGen_ *uu = UGEN(o)->module.gen.data;
  if (uu->shred) {
    free_vm_shred(uu->shred);
    uu->shred = NULL;
  }
  UGEN(o)->module.gen.tick = id_tick;
}

static INSTR(UsrUGenTick) {
  const m_uint offset = !instr->m_val ? SZ_INT : 0;
  shred->reg -= SZ_INT * 2 - offset;
  const M_Object o = *(M_Object *)(shred->reg + SZ_INT - offset);
  if (!o) {
    handle(shred, "NullPtrException");
    return;
  }
  struct UUGen_ *uu = UGEN(o)->module.gen.data;
  if (uu->shred) {
    free_vm_shred(uu->shred);
    return;
  }
  UGEN(o)->module.gen.tick = usrugen_tick;
  const VM_Code code       = *(VM_Code *)(shred->reg - offset);
  if (!code) {
    handle(shred, "NullTickException");
    return;
  }
  uu->shred = new_vm_shred(shred->info->mp, *(VM_Code *)(shred->reg - offset));
  vmcode_addref(*(VM_Code *)(shred->reg - offset));
  uu->shred->info->vm = shred->info->vm;
  code_prepare(vmcode_callback(shred->info->mp, uu->shred->code));
  shreduler_ini(uu->shred->tick->shreduler, uu->shred);
  uu->prep                           = instr->m_val ? member_prep : global_prep;
  *(M_Object *)(shred->reg - SZ_INT) = o;
}

static OP_EMIT(opem_usrugen) {
  Exp_Binary *bin   = (Exp_Binary *)data;
  const Instr instr = emit_add_instr(emit, UsrUGenTick);
  instr->m_val = !!bin->lhs->type->info->func->value_ref->from->owner_class;
  return GW_OK;
}

static GWION_IMPORT(usrugen) {
  GWI_OB(gwi_class_ini(gwi, "UsrUGen", "UGen"))
  gwi_class_xtor(gwi, usrugen_ctor, usrugen_dtor);
  GWI_BB(gwi_func_ini(gwi, "int", "default_tick"))
  GWI_BB(gwi_func_end(gwi, default_tick, 0))
  GWI_BB(gwi_class_end(gwi))
  GWI_BB(gwi_oper_ini(gwi, "function", "UsrUGen", "UsrUGen"))
  GWI_BB(gwi_oper_add(gwi, opck_usrugen))
  GWI_BB(gwi_oper_emi(gwi, opem_usrugen))
  GWI_BB(gwi_oper_end(gwi, "~=>", NULL))
  return GW_OK;
}

GWION_IMPORT(modules) {
  GWI_BB(import_gain(gwi))
  GWI_BB(import_impulse(gwi))
  GWI_BB(import_fullrect(gwi))
  GWI_BB(import_halfrect(gwi))
  GWI_BB(import_step(gwi))
  GWI_BB(import_zerox(gwi))
  return import_usrugen(gwi);
}
