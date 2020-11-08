#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "object.h"
#include "vararg.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "specialid.h"
#include "traverse.h"
#include "parse.h"
#include "gack.h"

void free_vararg(MemPool p, struct Vararg_* arg) {
  if(arg->t.ptr) {
    xfree(arg->d);
    vector_release(&arg->t);
  }
  mp_free(p, Vararg, arg);
}

static DTOR(vararg_dtor) {
  struct Vararg_ *arg = *(struct Vararg_**)o->data;
  if(*(m_uint*)(o->data + SZ_INT*2)) {
    m_uint offset = 0;
    for(m_uint i = 0; i < vector_size(&arg->t); ++i) {
      const Type t = (Type)vector_at(&arg->t, i);
      if(isa(t, shred->info->vm->gwion->type[et_object]) > 0)
        release(*(M_Object*)(arg->d + offset), shred);
      else if(tflag(t, tflag_struct))
        struct_release(shred, t, *(m_bit**)(arg->d + offset));
      offset += t->size;
    }
  }
  free_vararg(shred->info->vm->gwion->mp, arg);
}

static MFUN(mfun_vararg_cpy) {
  struct Vararg_ *src =  *(struct Vararg_**)o->data;
  struct Vararg_* arg = mp_calloc(shred->info->mp, Vararg);
  vector_copy2(&src->t, &arg->t);
  arg->d = (m_bit*)xmalloc(round2szint(*(m_uint*)(o->data + SZ_INT*2)));
  m_uint offset = 0;
  for(m_uint i = 0; i < vector_size(&arg->t); ++i) {
    const Type t = (Type)vector_at(&arg->t, arg->i);
    *(m_uint*)(arg->d + offset) = *(m_uint*)(src->d + offset);
    if(isa(t, shred->info->vm->gwion->type[et_object]) > 0)
      ++(*(M_Object*)(arg->d + offset))->ref;
    offset += t->size;
  }
  arg->s = vector_size(&arg->t);
  arg->i = src->i;
  arg->o = src->o;
  const M_Object obj = new_object(shred->info->mp, shred, o->type_ref);
  *(struct Vararg_**)obj->data = arg;
  *(m_uint*)(obj->data + SZ_INT*2) = *(m_uint*)(o->data + SZ_INT*2);
  *(M_Object*)RETURN = obj;
}

INSTR(VarargIni) {
  const M_Object o = new_object(shred->info->mp, shred, shred->info->vm->gwion->type[et_vararg]);
  struct Vararg_* arg = mp_calloc(shred->info->mp, Vararg);
  *(struct Vararg_**)o->data = arg;
  POP_REG(shred, instr->m_val - SZ_INT)
  if((*(m_uint*)(o->data + SZ_INT * 2) = instr->m_val)) {
    arg->d = (m_bit*)xmalloc(round2szint(instr->m_val));
    const Vector kinds = (Vector)instr->m_val2;
    vector_copy2(kinds, &arg->t);
    m_uint offset = 0;
    for(m_uint i = 0; i < vector_size(&arg->t); ++i) {
      const Type t = (Type)vector_at(&arg->t, i);
      *(m_uint*)(arg->d + offset) = *(m_uint*)(shred->reg - SZ_INT + offset);
      if(isa(t, shred->info->vm->gwion->type[et_object]) > 0) {
        const M_Object obj = *(M_Object*)(arg->d + offset);
        if(obj)
          ++obj->ref;
      }
      offset += t->size;
    }
    arg->s = vector_size(kinds);
  }
  *(M_Object*)REG(-SZ_INT) = o;
}

INSTR(VarargCheck) {
  const M_Object o = *(M_Object*)(shred->reg-SZ_INT);
  struct Vararg_ *arg = *(struct Vararg_**)o->data;
  if(arg->s)
    return;
  shred->reg -= SZ_INT;
  shred->pc = instr->m_val;
}

static INSTR(VarargEnd) {
  const M_Object o = *(M_Object*)REG(0);
  struct Vararg_* arg = *(struct Vararg_**)o->data;
  arg->o += arg->t.ptr ? ((Type)vector_at(&arg->t, arg->i))->size : 0;
  if(++arg->i < arg->s)
    shred->pc = instr->m_val;
  else
    arg->i = arg->o = 0;
}

static OP_CHECK(opck_vararg_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  return known_type(env, cast->td);
}

static INSTR(VarargCast) {
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  if(!*(m_uint*)(o->data + SZ_INT))
	  Except(shred, "Using Vararg outside varloop");
  struct Vararg_* arg = *(struct Vararg_**)o->data;
  const Type t = (Type)instr->m_val;
  if(isa((Type)vector_at(&arg->t, arg->i), t) < 0)
	  Except(shred, "InvalidVariadicAccess");
  for(m_uint i = 0; i < t->size; i += SZ_INT)
    *(m_uint*)REG(i - SZ_INT) = *(m_uint*)(arg->d + arg->o + i);
}

static OP_EMIT(opem_vararg_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  const Instr instr = emit_add_instr(emit, VarargCast);
  instr->m_val = (m_uint)exp_self(cast)->info->type;
  const Instr push = emit_add_instr(emit, RegPush);
  push->m_val = exp_self(cast)->info->type->size - SZ_INT;
  return instr;
}

static FREEARG(freearg_vararg) {
  if(instr->m_val2)
    free_vector(((Gwion)gwion)->mp, (Vector)instr->m_val2);
}

static ID_CHECK(idck_vararg) {
  if(env->func && fbflag(env->func->def->base, fbflag_variadic))
    return nonnul_type(env, exp_self(prim)->info->type);
  ERR_O(exp_self(prim)->pos, _("'vararg' must be used inside variadic function"))
}

static ID_EMIT(idem_vararg) {
  const Instr instr = emit_add_instr(emit, RegPushMem);
  instr->m_val = emit->code->stack_depth - SZ_INT;
  return instr;
}

static GACK(gack_vararg) {
  INTERP_PRINTF("%p", *(M_Object*)VALUE);
}

ANN void emit_vararg_end(const Emitter emit, const m_uint pc) {
  const Instr pop = emit_add_instr(emit, RegPop);
  pop->m_val = SZ_INT;
  const Instr instr = emit_add_instr(emit, VarargEnd);
  instr->m_val = pc;
}

GWION_IMPORT(vararg) {
  const Type t_vararg  = gwi_class_ini(gwi, "Vararg", "Object");
  gwi_class_xtor(gwi, NULL, vararg_dtor);
  gwi_gack(gwi, t_vararg, gack_vararg);
  GWI_BB(gwi_item_ini(gwi, "@internal", "@data"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, NULL))
  GWI_BB(gwi_item_ini(gwi, "int", "@inLoop"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, NULL))
  GWI_BB(gwi_item_ini(gwi, "int", "@len"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, NULL))
  GWI_BB(gwi_func_ini(gwi, "Vararg", "cpy"))
  GWI_BB(gwi_func_end(gwi, mfun_vararg_cpy, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))
  SET_FLAG(t_vararg, abstract | ae_flag_final);
  gwi->gwion->type[et_vararg] = t_vararg;
  GWI_BB(gwi_oper_ini(gwi, "nonnull Vararg", (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_vararg_cast))
  GWI_BB(gwi_oper_emi(gwi, opem_vararg_cast))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
  gwi_register_freearg(gwi, VarargIni, freearg_vararg);
  struct SpecialId_ spid = { .type=t_vararg, .is_const=1, .ck=idck_vararg, .em=idem_vararg};
  gwi_specialid(gwi, "vararg", &spid);
  return GW_OK;
}
