#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "vararg.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "specialid.h"
#include "traverse.h"
#include "parse.h"
#include "gack.h"

void free_vararg(MemPool p, struct Vararg_ *arg) {
  if (arg->t.ptr) {
    xfree(arg->d);
    vector_release(&arg->t);
  }
  mp_free(p, Vararg, arg);
}

static DTOR(vararg_dtor) {
  struct Vararg_ *arg = *(struct Vararg_ **)o->data;
  if (*(m_uint *)(o->data + SZ_INT * 2)) {
    m_uint offset = 0;
    for (m_uint i = 0; i < vector_size(&arg->t); ++i) {
      // could be compound release
      const Type t = (Type)vector_at(&arg->t, i);
      if (isa(t, shred->info->vm->gwion->type[et_object]) > 0)
        release(*(M_Object *)(arg->d + offset), shred);
      else if (tflag(t, tflag_struct))
        struct_release(shred, t, *(m_bit **)(arg->d + offset));
      offset += t->size;
    }
  }
  free_vararg(shred->info->mp, arg);
}

static MFUN(mfun_vararg_cpy) {
  struct Vararg_ *src = *(struct Vararg_ **)o->data;
  struct Vararg_ *arg = mp_calloc(shred->info->mp, Vararg);
  vector_copy2(&src->t, &arg->t);
  arg->d = (m_bit *)xmalloc(round2szint(*(m_uint *)(o->data + SZ_INT * 2)));
  m_uint offset = 0;
  for (m_uint i = 0; i < vector_size(&arg->t); ++i) {
    const Type t = (Type)vector_at(&arg->t, *(m_uint *)(o->data + SZ_INT * 4));
    *(m_uint *)(arg->d + offset) = *(m_uint *)(src->d + offset);
    if (isa(t, shred->info->vm->gwion->type[et_object]) > 0)
      ++(*(M_Object *)(arg->d + offset))->ref;
    offset += t->size;
  }
  const M_Object obj = new_object(shred->info->mp, shred, o->type_ref);
  *(struct Vararg_ **)obj->data       = arg;
  *(m_uint *)(obj->data + SZ_INT * 2) = *(m_uint *)(o->data + SZ_INT * 2);
  *(m_uint *)(obj->data + SZ_INT * 3) = *(m_uint *)(o->data + SZ_INT * 3);
  *(m_uint *)(obj->data + SZ_INT * 4) = *(m_uint *)(o->data + SZ_INT * 4);
  *(m_uint *)(obj->data + SZ_INT * 4) = vector_size(&arg->t); // can we copy?
  *(M_Object *)RETURN                 = obj;
}

INSTR(VarargIni) {
  const M_Object  o           = new_object(shred->info->mp, shred,
                                shred->info->vm->gwion->type[et_vararg]);
  struct Vararg_ *arg         = mp_calloc(shred->info->mp, Vararg);
  *(struct Vararg_ **)o->data = arg;
  POP_REG(shred, instr->m_val - SZ_INT)
  if ((*(m_uint *)(o->data + SZ_INT * 2) = instr->m_val)) {
    arg->d             = (m_bit *)xmalloc(round2szint(instr->m_val));
    const Vector kinds = (Vector)&instr->m_val2;
    vector_copy2(kinds, &arg->t);
    m_uint offset = 0;
    for (m_uint i = 0; i < vector_size(&arg->t); ++i) {
      const Type t                 = (Type)vector_at(&arg->t, i);
      *(m_uint *)(arg->d + offset) = *(m_uint *)(shred->reg - SZ_INT + offset);
      if (isa(t, shred->info->vm->gwion->type[et_object]) > 0) {
        const M_Object obj = *(M_Object *)(arg->d + offset);
        if (obj) ++obj->ref;
      }
      offset += t->size;
    }
    *(m_uint *)(o->data + SZ_INT * 5) = vector_size(kinds);
  }
  *(M_Object *)REG(-SZ_INT) = o;
}

static INSTR(VarargEnd) {
  const M_Object  o   = *(M_Object *)REG(0);
  struct Vararg_ *arg = *(struct Vararg_ **)o->data;
  *(m_uint *)(o->data + SZ_INT * 3) +=
      arg->t.ptr
          ? ((Type)vector_at(&arg->t, *(m_uint *)(o->data + SZ_INT * 4)))->size
          : 0;
  if (++*(m_uint *)(o->data + SZ_INT * 4) ==
      *(m_uint *)(o->data + SZ_INT * 5)) {
    //  if(++*(m_uint*)(o->data + SZ_INT*4) < *(m_uint*)(o->data + SZ_INT * 5))
    //    shred->pc = instr->m_val;
    //  else
    *(m_uint *)(o->data + SZ_INT * 4) = *(m_uint *)(o->data + SZ_INT * 3) = 0;
    ++shred->pc;
  }
}

static OP_CHECK(opck_vararg_cast) {
  const Exp_Cast *cast = (Exp_Cast *)data;
  return known_type(env, cast->td);
}

static INSTR(VarargCast) {
  const M_Object o = *(M_Object *)REG(-SZ_INT);
  if (!*(m_uint *)(o->data + SZ_INT)) {
    handle(shred, "Using Vararg outside varloop");
    return;
  }
  struct Vararg_ *arg = *(struct Vararg_ **)o->data;
  const Type      t   = (Type)instr->m_val,
             u = (Type)vector_at(&arg->t, *(m_uint *)(o->data + SZ_INT * 4));
  if (isa(u, t) > 0 || (u == shred->info->vm->gwion->type[et_error] &&
                        isa(t, shred->info->vm->gwion->type[et_object]) > 0)) {
    for (m_uint i = 0; i < t->size; i += SZ_INT)
      *(m_uint *)REG(i - SZ_INT) =
          *(m_uint *)(arg->d + *(m_uint *)(o->data + SZ_INT * 3) + i);
  } else
    handle(shred, "InvalidVariadicAccess");
}

static OP_EMIT(opem_vararg_cast) {
  const Exp_Cast *cast  = (Exp_Cast *)data;
  const Instr     instr = emit_add_instr(emit, VarargCast);
  instr->m_val          = (m_uint)exp_self(cast)->type;
  const Instr push      = emit_add_instr(emit, RegMove);
  push->m_val           = exp_self(cast)->type->size - SZ_INT;
  return GW_OK;
}

static FREEARG(freearg_vararg) {
  if (instr->m_val2) vector_release((Vector)&instr->m_val2);
}

static ID_CHECK(idck_vararg) {
  if (env->func && fbflag(env->func->def->base, fbflag_variadic))
    return exp_self(prim)->type;
  ERR_O(exp_self(prim)->pos,
        _("'vararg' must be used inside variadic function"))
}

static ID_EMIT(idem_vararg) {
  const Instr instr = emit_add_instr(emit, RegPushMem);
  instr->m_val      = emit->vararg_offset + emit->code->stack_depth - SZ_INT;
  return instr;
}

static GACK(gack_vararg) { INTERP_PRINTF("%p", *(M_Object *)VALUE); }

ANN void emit_vararg_end(const Emitter emit, const m_uint pc) {
  const Instr pop = emit_add_instr(emit, RegMove);
  pop->m_val      = -SZ_INT;
  (void)emit_add_instr(emit, VarargEnd);
  const Instr instr = emit_add_instr(emit, Goto);
  instr->m_val      = pc;
}

GWION_IMPORT(vararg) {
  const Type t_vararg = gwi_class_ini(gwi, "Vararg", "Object");
  gwi_class_xtor(gwi, NULL, vararg_dtor);
  gwi_gack(gwi, t_vararg, gack_vararg);
  GWI_BB(gwi_item_ini(gwi, "@internal", "@data"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
  GWI_BB(gwi_item_ini(gwi, "int", "@inLoop"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
  GWI_BB(gwi_item_ini(gwi, "int", "@len"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
  GWI_BB(gwi_item_ini(gwi, "int", "@o"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
  GWI_BB(gwi_item_ini(gwi, "int", "@i"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
  GWI_BB(gwi_item_ini(gwi, "int", "@s"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
  GWI_BB(gwi_func_ini(gwi, "Vararg", "cpy"))
  GWI_BB(gwi_func_end(gwi, mfun_vararg_cpy, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))
  SET_FLAG(t_vararg, abstract | ae_flag_final);
  gwi->gwion->type[et_vararg] = t_vararg;
  GWI_BB(gwi_oper_ini(gwi, "Vararg", (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_vararg_cast))
  GWI_BB(gwi_oper_emi(gwi, opem_vararg_cast))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
  gwi_register_freearg(gwi, VarargIni, freearg_vararg);
  struct SpecialId_ spid = {
      .type = t_vararg, .is_const = 1, .ck = idck_vararg, .em = idem_vararg};
  gwi_specialid(gwi, "vararg", &spid);
  return GW_OK;
}
