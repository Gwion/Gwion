#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "gwion.h"
#include "type.h"
#include "nspc.h"
#include "instr.h"
#include "object.h"
#include "value.h"
#include "operator.h"
#include "import.h"
#include "emit.h"
#include "traverse.h"
#include "parse.h"
#include "gwi.h"

static OP_CHECK(opck_ptr_assign) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  if(bin->lhs->meta != ae_meta_var)
    ERR_N(exp_self(bin)->pos, _("left side operand is constant"));
  bin->lhs->emit_var = 1;
  Type t = bin->lhs->type;
  do if(!strcmp(t->name, get_type_name(env, bin->rhs->type->name, 1)))
    return bin->lhs->type;
  while((t = t->e->parent));
  return env->gwion->type[et_null];
}

static INSTR(instr_ptr_assign) {
  POP_REG(shred, SZ_INT)
  const M_Object o = *(M_Object*)REG(0);
  *(m_uint**)o->data = *(m_uint**)REG(-SZ_INT);
}

static OP_EMIT(opem_ptr_assign) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  if(!GET_FLAG(bin->rhs->type, nonnull))
    emit_add_instr(emit, GWOP_EXCEPT);
  return emit_add_instr(emit, instr_ptr_assign);
}

static OP_CHECK(opck_ptr_deref) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  return exp_self(unary)->type = nspc_lookup_type1(env->curr, insert_symbol(get_type_name(env, unary->exp->type->name, 1)));
}

static OP_CHECK(opck_ptr_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  DECL_ON(const Type, t, = type_decl_resolve(env, cast->td))
  if(!GET_FLAG(t, check)) {
    assert(t->e->def);
    CHECK_BN(traverse_class_def(env, t->e->def))
  }
  const Type to = known_type(env, cast->td->types->td);
  if(isa(cast->exp->type, to) > 0)
    return t;
  ERR_N(exp_self(cast)->pos, "invalid pointer cast")
}

static OP_CHECK(opck_implicit_ptr) {
  const struct Implicit* imp = (struct Implicit*)data;
  const Exp e = imp->e;
  if(!strcmp(get_type_name(env, imp->t->name, 1), e->type->name)) {
    if(e->meta == ae_meta_value)
      ERR_N(e->pos, _("can't cast constant to Ptr"));
    e->cast_to = imp->t;
    e->emit_var = 1;
    return imp->t;
  }
  return NULL;
}

static INSTR(instr_ptr_deref) {
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  if(!*(m_bit**)o->data)
    Except(shred, _("EmptyPointerException"));
  if(instr->m_val2)
    memcpy(REG(-SZ_INT), o->data, SZ_INT);
  else {
    shred->reg -= SZ_INT - instr->m_val;
    memcpy(REG(-instr->m_val), *(m_bit**)o->data, instr->m_val);
  }
}

static INSTR(Cast2Ptr) {
  const M_Object o = new_object(shred->info->mp, shred, shred->info->vm->gwion->type[et_ptr]);
  *(m_uint**)o->data = *(m_uint**)REG(-SZ_INT);
  *(M_Object*)REG(-SZ_INT) = o;
}
/*
static OP_EMIT(opem_ptr_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  const Instr instr = emit_add_instr(emit, Cast2Ptr);
  instr->m_val = (m_uint)exp_self(cast)->type;
  return GW_OK;
}
*/
static OP_EMIT(opem_ptr_deref) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  const Instr instr = emit_add_instr(emit, instr_ptr_deref);
  instr->m_val = exp_self(unary)->type->size;
  instr->m_val2 = exp_self(unary)->emit_var;
  return instr;
}

GWION_IMPORT(ptr) {
  const Type t_ptr = gwi_class_ini(gwi, "Ptr<~A~>", NULL);
  gwi->gwion->type[et_ptr] = t_ptr;
  GWI_BB(gwi_item_ini(gwi, "@internal", "@val"))
  GWI_BB(gwi_item_end(gwi, 0, NULL))
  GWI_BB(gwi_class_end(gwi))
  t_ptr->nspc->info->offset = SZ_INT; // TODO: should be an assert
  GWI_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "Ptr", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_ptr_assign))
  GWI_BB(gwi_oper_emi(gwi, opem_ptr_assign))
  GWI_BB(gwi_oper_end(gwi, ":=>", instr_ptr_assign))
  GWI_BB(gwi_oper_add(gwi, opck_implicit_ptr))
  GWI_BB(gwi_oper_end(gwi, "@implicit", Cast2Ptr))
  GWI_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "Ptr", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_ptr_cast))
  GWI_BB(gwi_oper_end(gwi, "$", Cast2Ptr))
  GWI_BB(gwi_oper_ini(gwi, NULL, "Ptr", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_ptr_deref))
  GWI_BB(gwi_oper_emi(gwi, opem_ptr_deref))
  GWI_BB(gwi_oper_end(gwi, "*", instr_ptr_deref))
  return GW_OK;
}
