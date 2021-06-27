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
#include "gack.h"
#include "gwi.h"

static m_bool ptr_access(const Env env, const Exp e) {
  const m_str access = exp_access(e);
  if (!access) return GW_OK;
  ERR_B(e->pos, _("operand is %s"), access);
}

ANN static inline Type ptr_base(const Env env, const Type t) {
  return (Type)vector_front(&t->info->tuple->types);
  return known_type(env, t->info->cdef->base.tmpl->call->td);
}

static OP_CHECK(opck_ptr_assign) {
  const Exp_Binary *bin = (Exp_Binary *)data;
  CHECK_BN(ptr_access(env, bin->lhs));
  CHECK_BN(ptr_access(env, bin->rhs));
  exp_setvar(bin->lhs, 1);
  exp_setvar(bin->rhs, 1);
  Type t = bin->lhs->type;
  do {
    Type u = bin->rhs->type;
    do {
      const Type base = ptr_base(env, u);
      if (isa(t, base) > 0) return t;
    } while ((u = u->info->parent) && u->info->cdef->base.tmpl->call);
  } while ((t = t->info->parent));
  return env->gwion->type[et_error];
}

static OP_EMIT(opem_ptr_assign) {
  const Exp_Binary *bin = (Exp_Binary *)data;
  const Type        t   = bin->lhs->cast_to ?: bin->lhs->type;
  if (isa(t, emit->gwion->type[et_compound]) > 0)
    (void)emit_compound_addref(emit, t, -(SZ_INT + t->size), 0);
  emit_add_instr(emit, int_r_assign);
  return GW_OK;
}

static OP_CHECK(opck_ptr_deref) {
  const Exp_Unary *unary = (Exp_Unary *)data;
  return ptr_base(env, unary->exp->type);
}

static OP_CHECK(opck_ptr_cast) {
  const Exp_Cast *cast = (Exp_Cast *)data;
  if (!cast->td->types || !cast->td->types->td)
    ERR_N(exp_self(cast)->pos, "'Ptr' needs types to cast");
  DECL_ON(const Type, t, = known_type(env, cast->td));
  if (t->info->cdef && !tflag(t, tflag_check))
    CHECK_BN(ensure_traverse(env, t));
  const Type to = known_type(env, cast->td->types->td);
  exp_setvar(cast->exp, 1);
  if (isa(cast->exp->type, to) > 0) return t;
  ERR_N(exp_self(cast)->pos, "invalid pointer cast");
}

static OP_CHECK(opck_ptr_implicit) {
  const struct Implicit *imp  = (struct Implicit *)data;
  const Exp              e    = imp->e;
  const Type             base = ptr_base(env, imp->t);
  if (isa(e->type, base) > 0) {
    const m_str access = exp_access(e);
    if (access) ERR_N(e->pos, _("can't cast %s value to Ptr"), access);
    exp_setvar(e, 1);
    return imp->t;
  }
  return NULL;
}

static INSTR(instr_ptr_deref) {
  const m_bit *o = *(m_bit **)REG(-SZ_INT);
  if (!o) {
    handle(shred, _("EmptyPointerException"));
    return;
  }
  if (instr->m_val2)
    memcpy(REG(-SZ_INT), &o, SZ_INT);
  else {
    shred->reg -= SZ_INT - instr->m_val;
    memcpy(REG(-instr->m_val), o, instr->m_val);
  }
}

static INSTR(Cast2Ptr) {
  m_bit *o                = *(m_bit **)REG(-SZ_INT);
  *(m_bit **)REG(-SZ_INT) = o;
}

static OP_EMIT(opem_ptr_cast) {
  const Exp_Cast *cast  = (Exp_Cast *)data;
  const Instr     instr = emit_add_instr(emit, Cast2Ptr);
  instr->m_val          = (m_uint)exp_self(cast)->type;
  return GW_OK;
}

static OP_EMIT(opem_ptr_implicit) {
  const struct Implicit *imp   = (struct Implicit *)data;
  const Instr            instr = emit_add_instr(emit, Cast2Ptr);
  instr->m_val                 = (m_uint)imp->t;
  return GW_OK;
}

static OP_EMIT(opem_ptr_deref) {
  const Exp_Unary *unary = (Exp_Unary *)data;
  const Instr      instr = emit_add_instr(emit, instr_ptr_deref);
  instr->m_val           = exp_self(unary)->type->size;
  instr->m_val2          = exp_getvar(exp_self(unary));
  return GW_OK;
}

ANN Type scan_class(const Env env, const Type t, const Type_Decl *td);

static DTOR(ptr_object_dtor) { release(**(M_Object **)o, shred); }

static DTOR(ptr_struct_dtor) {
  const Type   base  = *(Type *)(shred->mem + SZ_INT);
  const m_uint scope = env_push(shred->info->vm->gwion->env,
                                base->info->value->from->owner_class,
                                base->info->value->from->owner);
  const Type   t     = known_type(shred->info->vm->gwion->env,
                            base->info->cdef->base.tmpl->call->td);
  env_pop(shred->info->vm->gwion->env, scope);
  struct_release(shred, t, *(m_bit **)o);
}
#include "tmpl_info.h"
static OP_CHECK(opck_ptr_scan) {
  struct TemplateScan *ts   = (struct TemplateScan *)data;
  struct tmpl_info     info = {
      .base = ts->t, .td = ts->td, .list = ts->t->info->cdef->base.tmpl->list};
  const Type exists = tmpl_exists(env, &info);
  if (exists) return exists != env->gwion->type[et_error] ? exists : NULL;
  const Type base = known_type(env, ts->td->types->td);
  const Type t    = new_type(env->gwion->mp, s_name(info.name), base);
  t->info->parent = env->gwion->type[et_ptr];
  SET_FLAG(t, abstract | ae_flag_final);
  t->info->tuple = new_tupleform(env->gwion->mp, NULL);
  t->nspc        = new_nspc(env->gwion->mp, t->name);
  vector_add(&t->info->tuple->types, (m_uint)base);
  const m_uint scope = env_push(env, base->info->value->from->owner_class,
                                base->info->value->from->owner);
  mk_class(env, t, (loc_t) {});
  env_pop(env, scope);
  nspc_add_type_front(t->info->value->from->owner, info.name, t);
  if (isa(base, env->gwion->type[et_compound]) > 0) {
    t->nspc->dtor =
        new_vmcode(env->gwion->mp, NULL, NULL, "@PtrDtor", SZ_INT, true, false);
    if (!tflag(base, tflag_struct))
      t->nspc->dtor->native_func = (m_uint)ptr_object_dtor;
    else
      t->nspc->dtor->native_func = (m_uint)ptr_struct_dtor;
    set_tflag(t, tflag_dtor);
  }
  return t;
}

static OP_CHECK(opck_ptr_ref) {
  const Exp_Binary *bin = (Exp_Binary *)data;
  exp_setvar(bin->rhs, 1);
  return bin->rhs->type;
}

static GACK(gack_ptr) { INTERP_PRINTF("%p", *(m_str *)VALUE); }

GWION_IMPORT(ptr) {
  const Type t_ptr         = gwi_struct_ini(gwi, "Ptr:[A]");
  gwi->gwion->type[et_ptr] = t_ptr;
  GWI_BB(gwi_gack(gwi, t_ptr, gack_ptr))
  GWI_BB(gwi_item_ini(gwi, "@internal", "@val"))
  GWI_BB(gwi_item_end(gwi, 0, num, 0))
  GWI_BB(gwi_class_end(gwi))
  GWI_BB(gwi_oper_ini(gwi, "Ptr", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_ptr_scan))
  GWI_BB(gwi_oper_end(gwi, "@scan", NULL))
  GWI_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "Ptr", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_ptr_assign))
  GWI_BB(gwi_oper_emi(gwi, opem_ptr_assign))
  GWI_BB(gwi_oper_end(gwi, ":=>", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_ptr_implicit))
  GWI_BB(gwi_oper_emi(gwi, opem_ptr_implicit))
  GWI_BB(gwi_oper_end(gwi, "@implicit", Cast2Ptr))
  GWI_BB(gwi_oper_add(gwi, opck_ptr_cast))
  GWI_BB(gwi_oper_emi(gwi, opem_ptr_cast))
  GWI_BB(gwi_oper_end(gwi, "$", Cast2Ptr))
  GWI_BB(gwi_oper_ini(gwi, NULL, "Ptr", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_ptr_deref))
  GWI_BB(gwi_oper_emi(gwi, opem_ptr_deref))
  GWI_BB(gwi_oper_end(gwi, "*", instr_ptr_deref))
  GWI_BB(gwi_oper_ini(gwi, "Ptr", "Ptr", "Ptr"))
  GWI_BB(gwi_oper_add(gwi, opck_ptr_ref))
  //  GWI_BB(gwi_oper_emi(gwi, opem_ptr_ref))
  GWI_BB(gwi_oper_end(gwi, "=>", int_r_assign))
  return GW_OK;
}
