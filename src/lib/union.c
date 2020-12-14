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

static GACK(gack_none) {
  INTERP_PRINTF("None")
}

static INSTR(UnionSet) {
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  *(m_uint*)o->data = instr->m_val;
  memcpy(o->data + SZ_INT, REG(-SZ_INT-instr->m_val2), instr->m_val2);
  *(m_bit**)REG(-SZ_INT) = o->data + SZ_INT;
}

static INSTR(UnionCheck) {
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  if(*(m_uint*)o->data != instr->m_val)
    Except(shred, _("invalid union access"))
  POP_REG(shred, SZ_INT - instr->m_val2);
  memcpy(REG(-instr->m_val2), o->data + SZ_INT, instr->m_val2);
}

static OP_EMIT(opem_union_dot) {
  const Exp_Dot *member = (Exp_Dot*)data;
  const Map map = &member->t_base->nspc->info->value->map;
  for(m_uint i = 0; i < map_size(map); ++i) {
    if(VKEY(map, i) == (m_uint)member->xid) {
      CHECK_BO(emit_exp(emit, member->base))
      const Instr instr = emit_add_instr(emit,
        !exp_getvar(exp_self(member)) ? UnionCheck : UnionSet);
      instr->m_val = i + 1;
      instr->m_val2 = ((Value)VVAL(map, i))->type->size;
      return instr;
    }
  }
  return NULL;
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

  GWI_BB(gwi_oper_ini(gwi, "@Union", (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_emi(gwi, opem_union_dot))
  GWI_BB(gwi_oper_end(gwi, "@dot", NULL))

  GWI_BB(gwi_union_ini(gwi, "Option:[A]"))
  GWI_BB(gwi_union_add(gwi, "None", "none"))
  GWI_BB(gwi_union_add(gwi, "A", "val"))
  GWI_BB(gwi_union_end(gwi, ae_flag_none))

  return GW_OK;
}
