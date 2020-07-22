#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "emit.h"
#include "traverse.h"
#include "parse.h"
#include "gwi.h"

static m_bool ptr_access(const Env env, const Exp e) {
  const m_str access = exp_access(e);
  if(!access)
    return GW_OK;
  ERR_B(e->pos, _("operand is %s"), access);
}

static OP_CHECK(opck_ptr_assign) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  CHECK_BO(ptr_access(env, bin->lhs))
  CHECK_BO(ptr_access(env, bin->rhs))
  exp_setvar(bin->lhs, 1);
  Type t = bin->lhs->info->type;
  do {
//    t = unflag_type(t);
    Type u = bin->rhs->info->type;
    do {
//      u = unflag_type(u);
      const m_str str = get_type_name(env, u, 1);
      if(str && !strcmp(t->name, str))
        return bin->lhs->info->type; // use rhs?
    } while((u = u->e->parent));
  } while((t = t->e->parent));
  return env->gwion->type[et_null];
}

static INSTR(instr_ptr_assign) {
  POP_REG(shred, SZ_INT)
  const M_Object o = *(M_Object*)REG(0);
  *(m_uint**)o->data = *(m_uint**)REG(-SZ_INT);
  _release(o, shred);
}

static OP_CHECK(opck_ptr_deref) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  DECL_ON(const m_str, str, = get_type_name(env, unary->exp->info->type, 1))
  return exp_self(unary)->info->type = nspc_lookup_type1(env->curr, insert_symbol(str));
}

static OP_CHECK(opck_ptr_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  if(!cast->td->types || !cast->td->types->td)
    ERR_N(exp_self(cast)->pos, "'Ptr' needs types to cast")
  DECL_ON(const Type, t, = known_type(env, cast->td)) // was type_decl_resolve
  const Type _t = get_type(t);
  if(_t->e->def && !GET_FLAG(_t, check))
    CHECK_BN(ensure_traverse(env, _t))
  const Type to = known_type(env, cast->td->types->td);
  if(isa(cast->exp->info->type, to) > 0)
    return t;
  ERR_N(exp_self(cast)->pos, "invalid pointer cast")
}

static OP_CHECK(opck_ptr_implicit) {
  const struct Implicit* imp = (struct Implicit*)data;
  const Exp e = imp->e;
  DECL_OO(const m_str, name, = get_type_name(env, imp->t, 1))
  if(!strcmp(get_type_name(env, imp->t, 1), e->info->type->name)) {
    const m_str access = exp_access(e);
    if(access)
      ERR_N(e->pos, _("can't cast %s value to Ptr"), access);
    e->info->cast_to = imp->t;
    exp_setvar(e, 1);
    const Type t = get_type(imp->t);
    if(!GET_FLAG(t, check))
      CHECK_BN(traverse_class_def(env, t->e->def))
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
  const M_Object o = new_object(shred->info->mp, shred, (Type)instr->m_val);
  *(m_uint**)o->data = *(m_uint**)REG(-SZ_INT);
  *(M_Object*)REG(-SZ_INT) = o;
}

static OP_EMIT(opem_ptr_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  const Instr instr = emit_add_instr(emit, Cast2Ptr);
  instr->m_val = (m_uint)exp_self(cast)->info->type;
  return instr;
}

static OP_EMIT(opem_ptr_implicit) {
  const struct Implicit* imp = (struct Implicit*)data;
  const Instr instr = emit_add_instr(emit, Cast2Ptr);
  instr->m_val = (m_uint)imp->t;
  return instr;
}

static OP_EMIT(opem_ptr_deref) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  const Instr instr = emit_add_instr(emit, instr_ptr_deref);
  instr->m_val = exp_self(unary)->info->type->size;
  instr->m_val2 = exp_getvar(exp_self(unary));
  return instr;
}

ANN Type scan_class(const Env env, const Type t, const Type_Decl* td);

static DTOR(ptr_object_dtor) {
  release(*(M_Object*)o->data, shred);
}

static DTOR(ptr_struct_dtor) {
  const Type t = (Type)vector_front(&o->type_ref->e->tuple->types);
  struct_release(shred, t, *(m_bit**)o->data);
}

static OP_CHECK(opck_ptr_scan) {
  struct TemplateScan *ts = (struct TemplateScan*)data;
  const Type t = (Type)scan_class(env, ts->t, ts->td);
  const Type base = known_type(env, t->e->def->base.tmpl->call->td);
  if(isa(base, env->gwion->type[et_compound]) > 0 && !t->nspc->dtor) {
    t->nspc->dtor = new_vm_code(env->gwion->mp, NULL, SZ_INT, ae_flag_member | ae_flag_builtin, "@PtrDtor");
    if(!GET_FLAG(t, struct))
      t->nspc->dtor->native_func = (m_uint)ptr_object_dtor;
    else
      t->nspc->dtor->native_func = (m_uint)ptr_struct_dtor;
    SET_FLAG(t, dtor);
  }
  return t;
}

GWION_IMPORT(ptr) {
  const Type _t_ptr = gwi_class_ini(gwi, "@Ptr", NULL);
  GWI_BB(gwi_class_end(gwi))
  SET_FLAG(_t_ptr, unary);
  GWI_BB(gwi_oper_ini(gwi, "@Ptr", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_ptr_scan))
  GWI_BB(gwi_oper_end(gwi, "@scan", NULL))
  const Type t_ptr = gwi_class_ini(gwi, "<~A~>Ptr", "@Ptr");
//  const Type t_ptr = gwi_class_ini(gwi, "<~A~>Ptr", NULL);
  gwi->gwion->type[et_ptr] = t_ptr;
  GWI_BB(gwi_item_ini(gwi, "@internal", "@val"))
  GWI_BB(gwi_item_end(gwi, 0, NULL))
  GWI_BB(gwi_class_end(gwi))
  SET_FLAG(t_ptr, unary);
  GWI_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "nonnull Ptr", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_ptr_assign))
  GWI_BB(gwi_oper_end(gwi, ":=>", instr_ptr_assign))
  GWI_BB(gwi_oper_add(gwi, opck_ptr_implicit))
  GWI_BB(gwi_oper_emi(gwi, opem_ptr_implicit))
  GWI_BB(gwi_oper_end(gwi, "@implicit", Cast2Ptr))
  GWI_BB(gwi_oper_add(gwi, opck_ptr_cast))
  GWI_BB(gwi_oper_emi(gwi, opem_ptr_cast))
  GWI_BB(gwi_oper_end(gwi, "$", Cast2Ptr))
  GWI_BB(gwi_oper_ini(gwi, NULL, "nonnull Ptr", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_ptr_deref))
  GWI_BB(gwi_oper_emi(gwi, opem_ptr_deref))
  GWI_BB(gwi_oper_end(gwi, "*", instr_ptr_deref))
  return GW_OK;
}
