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
#include "specialid.h"

#include "gwi.h"

#undef insert_symbol
ANN void exception(const VM_Shred shred, const m_str c) {
  gw_err("%s: shred[id=%" UINT_F ":%s], PC=[%" UINT_F "]\n",
          c, shred->tick->xid, shred->info->name, shred->pc - 1);
  vm_shred_exit(shred);
}

M_Object new_object(MemPool p, const VM_Shred shred, const Type t) {
  const M_Object a = mp_calloc(p, M_Object);
  a->ref = 1;
  a->type_ref = t;
  if(t->nspc) {
    a->vtable = &t->nspc->info->vtable;
    if(t->nspc->info->offset)
      a->data = (m_bit*)_mp_calloc(p, t->nspc->info->offset);
  }
  if(shred)
    vector_add(&shred->gc, (vtype)a);
  return a;
}

M_Object new_string(MemPool p, const VM_Shred shred, const m_str str) {
  const M_Object o = new_object(p, shred, shred->info->vm->gwion->type[et_string]);
  STRING(o) = s_name(insert_symbol(shred->info->vm->gwion->st, str));
  return o;
}

M_Object new_string2(const struct Gwion_ *gwion, const VM_Shred shred, const m_str str) {
  const M_Object o = new_object(gwion->mp, shred, gwion->type[et_string]);
  STRING(o) = str;
  return o;
}

ANN static void handle_dtor(const M_Object o, const VM_Shred shred) {
  const VM_Shred sh = new_vm_shred(shred->info->mp, o->type_ref->nspc->dtor);
  ADD_REF(o->type_ref->nspc->dtor);
  sh->base = shred->base;
  *(M_Object*)sh->mem = o;
  vm_add_shred(shred->info->vm, sh);
  ++sh->info->me->ref;
}

__attribute__((hot))
ANN void __release(const M_Object o, const VM_Shred shred) {
  MemPool p = shred->info->mp;
  Type t = o->type_ref;
  do {
    if(GET_FLAG(t, nonnull))
      t = t->e->parent;
    if(!t->nspc)
      continue; // return ?
    struct scope_iter iter = { t->nspc->info->value, 0, 0 };\
    Value v;
    while(scope_iter(&iter, &v) > 0) {
      if(!GET_FLAG(v, static) && !GET_FLAG(v, pure) &&
          isa(v->type, shred->info->vm->gwion->type[et_object]) > 0)
        release(*(M_Object*)(o->data + v->from->offset), shred);
    }
    if(GET_FLAG(t, dtor) && t->nspc->dtor) {
      if(GET_FLAG(t->nspc->dtor, builtin))
        ((f_xtor)t->nspc->dtor->native_func)(o, NULL, shred);
      else {
        o->type_ref = t;
        handle_dtor(o, shred);
        return;
      }
    }
  } while((t = t->e->parent));
  free_object(p, o);
}

ANN void free_object(MemPool p, const M_Object o) {
  if(o->type_ref->nspc && o->type_ref->nspc->info->offset)
    mp_free2(p, o->type_ref->nspc->info->offset, o->data);
  mp_free(p, M_Object, o);
}

#define describe_logical(name, op)               \
static INSTR(name##Object) {\
  POP_REG(shred, SZ_INT);                        \
  const M_Object lhs = *(M_Object*)REG(-SZ_INT); \
  const M_Object rhs = *(M_Object*)REG(0);       \
  *(m_uint*)REG(-SZ_INT) = (lhs op rhs);         \
  release(lhs, shred);                           \
  release(rhs, shred);                           \
}

describe_logical(Eq,  ==)
describe_logical(Neq, !=)

static inline m_bool nonnull_check(const Type l, const Type r) {
  return !GET_FLAG(l, nonnull) && GET_FLAG(r, nonnull);
}

static inline Type check_nonnull(const Env env, const Type l, const Type r,
      const m_str action, const loc_t pos) {
  if(GET_FLAG(r, nonnull)) {
    if(isa(l, env->gwion->type[et_null]) > 0)
      ERR_N(pos, _("can't %s '%s' to '%s'"), action, l->name, r->name);
    if(isa(l, r) < 0)
      ERR_N(pos, _("can't %s '%s' to '%s'"), action, l->name, r->name);
    return r->e->parent;
  }
/*  if(nonnull_check(l, r))
    ERR_N(pos, _("can't %s '%s' to '%s'"), action, l->name, r->name); */
  if(l != env->gwion->type[et_null] && isa(l, r) < 0)
    ERR_N(pos, _("can't %s '%s' to '%s'"), action, l->name, r->name);
  return r;
}

static OP_CHECK(at_object) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  const Type l = bin->lhs->type;
  const Type r = bin->rhs->type;
  if(opck_rassign(env, data, mut) == env->gwion->type[et_null])
    return env->gwion->type[et_null];
  if(check_nonnull(env, l, r, "assign", exp_self(bin)->pos) == env->gwion->type[et_null])
    return env->gwion->type[et_null];
  if(bin->rhs->exp_type == ae_exp_decl) {
    SET_FLAG(bin->rhs->d.exp_decl.td, ref);
    SET_FLAG(bin->rhs->d.exp_decl.list->self->value, ref);
  }
  bin->rhs->emit_var = 1;
  return r;
}

static OP_EMIT(opem_at_object) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  const Type l = bin->lhs->type;
  const Type r = bin->rhs->type;
  if(nonnull_check(l, r)) {
    const Instr instr = emit_add_instr(emit, GWOP_EXCEPT);
    instr->m_val = SZ_INT;
  }
  return emit_add_instr(emit, ObjectAssign);
}

#define STR_FORCE ":force"
#define STRLEN_FORCE strlen(STR_FORCE)

static inline Type new_force_type(MemPool p, const Type t, const Symbol sym) {
  const Type ret = type_copy(p, t);
  if(ret->nspc)
    ADD_REF(ret->nspc)
  ret->name = s_name(sym);
  ret->flag = t->flag | ae_flag_force;
  nspc_add_type_front(t->e->owner, sym, ret);
  return ret;
 }

static Type get_force_type(const Env env, const Type t) {
  const size_t len = strlen(t->name);
  char name[len + STRLEN_FORCE + 2];
  strcpy(name, t->name);
  strcpy(name + len, STR_FORCE);
  const Symbol sym = insert_symbol(env->gwion->st, name);
  return nspc_lookup_type1(t->e->owner, sym) ?: new_force_type(env->gwion->mp, t, sym);
}

static OP_CHECK(opck_object_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  const Type l = cast->exp->type;
  const Type r = exp_self(cast)->type;
  if(check_nonnull(env, l, r, "cast", exp_self(cast)->pos) == env->gwion->type[et_null])
    return env->gwion->type[et_null];
  return get_force_type(env, r);
}

static OP_EMIT(opem_object_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  const Type l = cast->exp->type;
  const Type r = exp_self(cast)->type;
  if(nonnull_check(l, r))
    emit_add_instr(emit, GWOP_EXCEPT);
  return (Instr)GW_OK;
}

static OP_CHECK(opck_implicit_null2obj) {
  const struct Implicit* imp = (struct Implicit*)data;
  const Type l = imp->e->type;
  const Type r = imp->t;
  if(check_nonnull(env, l, r, "implicitly cast", imp->e->pos) == env->gwion->type[et_null])
    return env->gwion->type[et_null];
  imp->e->cast_to = r;
  return imp->t;
}

static OP_EMIT(opem_implicit_null2obj) {
  const struct Implicit* imp = (struct Implicit*)data;
  const Type l = imp->e->type;
  const Type r = imp->t;
  if(nonnull_check(l, r))
    emit_add_instr(emit, GWOP_EXCEPT);
  return (Instr)GW_OK;
}

static ID_CHECK(check_this) {
  if(!env->class_def)
    ERR_O(exp_self(prim)->pos, _("keyword 'this' can be used only inside class definition..."))
  if(env->func && !GET_FLAG(env->func, member))
    ERR_O(exp_self(prim)->pos, _("keyword 'this' cannot be used inside static functions..."))
  return env->class_def;
}

static GACK(gack_object) {
  printf("%p", *(M_Object*)VALUE);
}

GWION_IMPORT(object) {
const Type t_object  = gwi_mk_type(gwi, "Object", SZ_INT, NULL);
gwi_add_type(gwi, t_object);
  GWI_BB(gwi_gack(gwi, t_object, gack_object))
//assert(GET_FLAG(t_object, checked));
  SET_FLAG(t_object, checked); // should be set by gwi_add_type
  gwi->gwion->type[et_object] = t_object;

  GWI_BB(gwi_oper_cond(gwi, "Object", BranchEqInt, BranchNeqInt))
  GWI_BB(gwi_oper_ini(gwi, "@null", "Object", "Object"))
  GWI_BB(gwi_oper_add(gwi, at_object))
  GWI_BB(gwi_oper_end(gwi, "@=>", ObjectAssign))
  GWI_BB(gwi_oper_ini(gwi, "Object", "Object", NULL))
  GWI_BB(gwi_oper_add(gwi, at_object))
  GWI_BB(gwi_oper_emi(gwi, opem_at_object))
  GWI_BB(gwi_oper_end(gwi, "@=>", ObjectAssign))
  GWI_BB(gwi_oper_ini(gwi, "Object", "Object", "int"))
  GWI_BB(gwi_oper_end(gwi, "==",  EqObject))
  GWI_BB(gwi_oper_end(gwi, "!=", NeqObject))
  GWI_BB(gwi_oper_add(gwi, opck_object_cast))
  GWI_BB(gwi_oper_emi(gwi, opem_object_cast))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_implicit_null2obj))
  GWI_BB(gwi_oper_emi(gwi, opem_implicit_null2obj))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
  GWI_BB(gwi_oper_ini(gwi, "@null", "Object", "int"))
  GWI_BB(gwi_oper_end(gwi, "==",  EqObject))
  GWI_BB(gwi_oper_end(gwi, "!=", NeqObject))
  GWI_BB(gwi_oper_add(gwi, opck_object_cast))
  GWI_BB(gwi_oper_emi(gwi, opem_object_cast))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_implicit_null2obj))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
  GWI_BB(gwi_oper_ini(gwi, "Object", "@null", "int"))
  GWI_BB(gwi_oper_end(gwi, "==", EqObject))
  GWI_BB(gwi_oper_end(gwi, "!=", NeqObject))
  GWI_BB(gwi_oper_ini(gwi, NULL, "Object", "bool"))
  GWI_BB(gwi_oper_add(gwi, opck_unary_meta2))
  GWI_BB(gwi_oper_end(gwi, "!", IntNot))
  gwi_item_ini(gwi, "@null", "null");
  gwi_item_end(gwi, 0, NULL);
  struct SpecialId_ spid = { .ck=check_this, .exec=RegPushMem, .is_const=1 };
  gwi_specialid(gwi, "this", &spid);
  return GW_OK;
}
