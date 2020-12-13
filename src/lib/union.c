#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "emit.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "specialid.h"
#include "gack.h"

#define UNION_IDX(a) (*(m_uint*)(a->data))

static GACK(gack_none) {
  INTERP_PRINTF("None")
}

static OP_CHECK(opck_any_at_union) {
  Exp_Binary *bin = (Exp_Binary*)data;
  CHECK_NN(opck_rassign(env, data, mut)) // check those two lines
  const Type lhs = bin->lhs->info->type;
  const Nspc nspc = bin->rhs->info->type->nspc;
  for(m_uint i = 0; i < nspc->info->class_data_size; i += SZ_INT) {
    if(lhs == *(Type*)(nspc->info->class_data + i))
      return bin->rhs->info->type;
  }
  return env->gwion->type[et_error]; // err_msg
}

static inline Type curr_type(const M_Object o) {
  const m_uint idx = UNION_IDX(o);
  if(!idx)
    return NULL;
  const Type curr = *(Type*)(o->type_ref->nspc->info->class_data + ((idx-1) * SZ_INT));
  return curr;
}

static INSTR(UnionSet) {
  POP_REG(shred, SZ_INT);
  const M_Object o = **(M_Object**)REG(0);
  memcpy(o->data + SZ_INT, REG(-instr->m_val), instr->m_val);
  UNION_IDX(o) = instr->m_val2;
  PUSH_REG(shred, SZ_INT-instr->m_val);
  *(M_Object*)REG(-SZ_INT) = o;
  _release(o, shred);
}

static OP_EMIT(opem_any_at_union) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Type lhs = bin->lhs->info->type;
  const Nspc nspc = bin->rhs->info->type->nspc;
  const Instr instr = emit_add_instr(emit, UnionSet);
  for(m_uint i = 0; i < nspc->info->class_data_size; i += SZ_INT) {
    if(lhs == *(Type*)(nspc->info->class_data + i)) {
      instr->m_val2 = i/SZ_INT + 1;
      instr->m_val = lhs->size;
      return instr;
    }
  }
  return NULL;
}

static OP_CHECK(opck_union_at_any) {
  CHECK_NN(opck_rassign(env, data, mut)) // check those two lines
  Exp_Binary *bin = (Exp_Binary*)data;
  const Type rhs = bin->rhs->info->type;
  const Nspc nspc = bin->lhs->info->type->nspc;
  for(m_uint i = 0; i < nspc->info->class_data_size; i += SZ_INT) {
    if(rhs == *(Type*)(nspc->info->class_data + i))
      return rhs;
  }
  return env->gwion->type[et_error]; // err_msg
}

static INSTR(UnionGet) {
  const M_Object o = *(M_Object*)REG(-SZ_INT*2);
  if(UNION_IDX(o) != instr->m_val2)
    Except(shred, "invalid union access");
  memcpy(*(m_bit**)REG(-SZ_INT), o->data + SZ_INT, instr->m_val);
  POP_REG(shred, SZ_INT*2 - instr->m_val);
  memcpy(REG(-instr->m_val), o->data + SZ_INT, instr->m_val);
  _release(o, shred);
}

static OP_EMIT(opem_union_at_any) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Type rhs = bin->rhs->info->type;
  const Nspc nspc = bin->lhs->info->type->nspc;
  // TODO: compound
  const Instr instr = emit_add_instr(emit, UnionGet);
  if(isa(rhs, emit->gwion->type[et_object]) > 0) {
    const Instr instr = emit_add_instr(emit, RegAddRef);
    instr->m_val = -SZ_INT;
  }
  for(m_uint i = 0; i < nspc->info->class_data_size; i += SZ_INT) {
    if(rhs == *(Type*)(nspc->info->class_data + i)) {
      instr->m_val2 = i/SZ_INT + 1;
      instr->m_val = rhs->size;
      return instr;
    }
  }
  return NULL;
}

static OP_CHECK(opck_union_eq_class) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Type rhs = bin->rhs->info->type->info->base_type;
  const Nspc nspc = bin->lhs->info->type->nspc;
  for(m_uint i = 0; i < nspc->info->class_data_size; i += SZ_INT) {
    if(rhs == *(Type*)(nspc->info->class_data + i))
      return env->gwion->type[et_bool];
  }
  return NULL; // err_msg
}

static INSTR(UnionEqClass) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  *(m_uint*)REG(-SZ_INT) = (UNION_IDX(o) == instr->m_val2);
  _release(o, shred);
}

static OP_EMIT(opem_union_eq_class) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Type rhs = bin->rhs->info->type->info->base_type;
  const Nspc nspc = bin->lhs->info->type->nspc;
  const Instr instr = emit_add_instr(emit, UnionEqClass);
  if(!strcmp(s_name(bin->op), "!="))
    (void)emit_add_instr(emit, IntNot);
  for(m_uint i = 0; i < nspc->info->class_data_size; i += SZ_INT) {
    if(rhs == *(Type*)(nspc->info->class_data + i)) {
      instr->m_val2 = i/SZ_INT + 1;
      return instr;
    }
  }
  return NULL;
}

static INSTR(UnionNot) {
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  const m_uint idx = UNION_IDX(o);
  if(idx) {
    const Type none = shred->info->vm->gwion->type[et_none];
    const Type curr = *(Type*)(o->type_ref->nspc->info->class_data + ((idx-1) * SZ_INT));
    *(m_uint*)REG(-SZ_INT) = curr == none;
  } else
    *(m_uint*)REG(-SZ_INT) = 1;
  _release(o, shred);
}

static DTOR(UnionDtor) {
  const m_uint idx = *(m_uint*)o->data;
  if(idx) {
    const Type t = *(Type*)(o->type_ref->nspc->info->class_data + (idx-1) * SZ_INT);
    if(isa(t, shred->info->vm->gwion->type[et_compound]) > 0)
      compound_release(shred, t, (o->data + SZ_INT));
  }
}

ANN GWION_IMPORT(union) {
  const Type t_none = gwi_mk_type(gwi, "None", 0, NULL);
  GWI_BB(gwi_set_global_type(gwi, t_none, et_none))
  GWI_BB(gwi_gack(gwi, t_none, gack_none))
  gwi_add_type(gwi, t_none);
  struct SpecialId_ spid = { .type=gwi->gwion->type[et_none], .exec=NoOp, .is_const=1 };
  gwi_specialid(gwi, "None", &spid);

  const Type t_union = gwi_class_ini(gwi, "@Union", "Object");
  gwi_class_xtor(gwi, NULL, UnionDtor);
  GWI_BB(gwi_item_ini(gwi, "int", "@index"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, NULL))
  GWI_BB(gwi_class_end(gwi))
  gwi->gwion->type[et_union] = t_union;

  GWI_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "@Union", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_any_at_union))
  GWI_BB(gwi_oper_emi(gwi, opem_any_at_union))
  GWI_BB(gwi_oper_end(gwi, "?=>", NULL))

  GWI_BB(gwi_oper_ini(gwi, "@Union", (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_union_at_any))
  GWI_BB(gwi_oper_emi(gwi, opem_union_at_any))
  GWI_BB(gwi_oper_end(gwi, "?=>", NULL))

  GWI_BB(gwi_oper_ini(gwi, "@Union", "@Class", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_union_eq_class))
  GWI_BB(gwi_oper_emi(gwi, opem_union_eq_class))
  GWI_BB(gwi_oper_end(gwi, "==", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_union_eq_class))
  GWI_BB(gwi_oper_emi(gwi, opem_union_eq_class))
  GWI_BB(gwi_oper_end(gwi, "!=", NULL))
  GWI_BB(gwi_oper_ini(gwi, NULL, "@Union", "bool"))
  GWI_BB(gwi_oper_end(gwi, "!", UnionNot))

  GWI_BB(gwi_union_ini(gwi, "Option:[A]"))
  GWI_BB(gwi_union_add(gwi, "A"))
  GWI_BB(gwi_union_add(gwi, "None"))
  GWI_BB(gwi_union_end(gwi, ae_flag_none))

  return GW_OK;
}
