#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "nspc.h"
#include "instr.h"
#include "object.h"
#include "import.h"
#include "emit.h"
#include "operator.h"

static OP_CHECK(opck_ptr_assign) {
  const Exp_Binary* bin = (Exp_Binary*)data;
Type t = bin->lhs->type;
do {
  if(!strcmp(t->name, get_type_name(bin->rhs->type->name, 1))) {
    if(bin->lhs->meta != ae_meta_var)
      ERR_N(0, "left side operand is constant")
    bin->lhs->emit_var = 1;
    return bin->lhs->type;
  }
} while((t = t->parent));
  return t_null;
}

static INSTR(instr_ptr_assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT)
  const M_Object o = *(M_Object*)REG(0);
  *(m_uint**)o->data = *(m_uint**)REG(-SZ_INT);
}

static OP_CHECK(opck_ptr_deref) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  unary->self->type = nspc_lookup_type1(unary->exp->type->owner, insert_symbol(get_type_name(unary->exp->type->name, 1)));
  return unary->self->type;
}

static OP_CHECK(opck_implicit_ptr) {
  const struct Implicit* imp = (struct Implicit*)data;
  const Exp e = (Exp)imp->e;
  if(!strcmp(get_type_name(imp->t->name, 1), e->type->name)) {
    if(e->meta == ae_meta_value)
      ERR_N(0, "can't cast constant to Ptr");
    e->cast_to = imp->t;
    e->emit_var = 1;
    return imp->t;
  }
  return NULL;
}

static INSTR(instr_ptr_deref) { GWDEBUG_EXE
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  if(instr->m_val2)
    memcpy(REG(-SZ_INT), o->data, SZ_INT);
  else
    memcpy(REG(-SZ_INT), *(m_bit**)o->data, instr->m_val);
}

static OP_EMIT(opem_ptr_deref) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  const Instr instr = emit_add_instr(emit, instr_ptr_deref);
  instr->m_val = unary->self->type->size;
  instr->m_val2 = unary->self->emit_var;
  return GW_OK;
}

static INSTR(Cast2Ptr) { GWDEBUG_EXE
  const M_Object o = new_object(shred, t_ptr);
  *(m_uint**)o->data = *(m_uint**)REG(-SZ_INT);
  *(M_Object*)REG(-SZ_INT) = o;
}

GWION_IMPORT(ptr) {
  const m_str list[] = { "A" };
  CHECK_OB((t_ptr = gwi_mk_type(gwi, "Ptr", SZ_INT, t_object)))
  CHECK_BB(gwi_tmpl_ini(gwi, 1, list))
  CHECK_BB(gwi_class_ini(gwi, t_ptr, NULL, NULL))
  CHECK_BB(gwi_tmpl_end(gwi))
  CHECK_BB(gwi_item_ini(gwi, "int", "@val"))
  CHECK_BB(gwi_item_end(gwi, 0, NULL))
  CHECK_BB(gwi_class_end(gwi))
  CHECK_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "Ptr", NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_ptr_assign))
  CHECK_BB(gwi_oper_end(gwi, op_trig, instr_ptr_assign))
  CHECK_BB(gwi_oper_add(gwi, opck_implicit_ptr))
  CHECK_BB(gwi_oper_end(gwi, op_impl, Cast2Ptr))
  CHECK_BB(gwi_oper_ini(gwi, NULL, "Ptr", NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_ptr_deref))
  CHECK_BB(gwi_oper_emi(gwi, opem_ptr_deref))
  CHECK_BB(gwi_oper_end(gwi, op_mul, instr_ptr_deref))
  return GW_OK;
}
