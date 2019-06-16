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
#include "value.h"
#include "instr.h"
#include "object.h"
#include "import.h"
#include "operator.h"

ANN void exception(const VM_Shred shred, const m_str c) {
  gw_err("%s: shred[id=%" UINT_F ":%s], PC=[%" UINT_F "]\n",
          c, shred->tick->xid, shred->info->name, shred->pc - 1);
  vm_shred_exit(shred);
}

M_Object new_object(MemPool p, const VM_Shred shred, const Type t) {
  const M_Object a = mp_calloc(p, M_Object);
  a->ref = 1;
  a->type_ref = t;
  a->vtable = &t->nspc->info->vtable;
  if(t->nspc->info->offset)
    a->data = (m_bit*)_mp_calloc(p, t->nspc->info->offset);
  if(shred)
    vector_add(&shred->gc, (vtype)a);
  return a;
}

M_Object new_string(MemPool p, const VM_Shred shred, const m_str str) {
  const M_Object o = new_object(p, shred, t_string);
  STRING(o) = s_name(insert_symbol(shred->info->vm->gwion->st, str));
  return o;
}

M_Object new_string2(MemPool p, const VM_Shred shred, const m_str str) {
  const M_Object o = new_object(p, shred, t_string);
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
  while(t->e->parent) {
    struct scope_iter iter = { t->nspc->info->value, 0, 0 };\
    Value v;
    while(scope_iter(&iter, &v) > 0) {
      if(!GET_FLAG(v, static) && !GET_FLAG(v, pure) &&
          isa(v->type, t_object) > 0)
        release(*(M_Object*)(o->data + v->offset), shred);
    }
    if(GET_FLAG(t, dtor)) {
      if(GET_FLAG(t->nspc->dtor, builtin))
        ((f_xtor)t->nspc->dtor->native_func)(o, NULL, shred);
      else {
        handle_dtor(o, shred);
        return;
      }
    }
    t = t->e->parent;
  }
  free_object(p, o);
}

ANN void free_object(MemPool p, const M_Object o) {
  if(o->type_ref->nspc->info->offset)
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

static OP_CHECK(at_object) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  const Type l = bin->lhs->type;
  const Type r = bin->rhs->type;
  if(opck_rassign(env, data) == t_null)
    return t_null;
  if(bin->rhs->exp_type == ae_exp_decl)
    SET_FLAG(bin->rhs->d.exp_decl.td, ref);
  if(l != t_null && isa(l, r) < 0) {
    env_err(env, exp_self(bin)->pos, "'%s' @=> '%s': not allowed", l->name, r->name);
    return t_null;
  }
  bin->rhs->emit_var = 1;
  return r;
}

#define STR_FORCE ":force"
#define STRLEN_FORCE strlen(STR_FORCE)

static inline Type new_force_type(MemPool p, const Type t, const Symbol sym) {
  const Type ret = type_copy(p, t);
//  ret->name = s_name(sym);
  SET_FLAG(ret, force);
//  nspc_add_type(t->e->owner, sym, ret);
//  map_set(&t->e->owner->info->type->map, sym, ret);
  map_set(vector_front(&t->e->owner->info->type->ptr), sym, ret);
  return ret;
}

static Type get_force_type(const Env env, const Type t) {
  const size_t len = strlen(t->name);
  char name[len + STRLEN_FORCE + 2];
  strcpy(name, t->name);
  strcpy(name + len, STR_FORCE);
  const Symbol sym = insert_symbol(env->gwion->st, name);
  return nspc_lookup_type1(t->e->owner, sym) ?: new_force_type(env->gwion->mp, t, sym);
//  return nspc_lookup_type0(t->e->owner, sym) ?: new_force_type(env, t, sym);
}

static OP_CHECK(opck_object_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  const Type l = cast->exp->type;
  const Type r = exp_self(cast)->type;
  return isa(l, r) > 0 ? get_force_type(env, r) : t_null;
}

static OP_CHECK(opck_implicit_null2obj) {
  const struct Implicit* imp = (struct Implicit*)data;
  return imp->t;
}

GWION_IMPORT(object) {
  CHECK_OB((t_object  = gwi_mk_type(gwi, "Object", SZ_INT, NULL)))
  CHECK_BB(gwi_class_ini(gwi, t_object, NULL, NULL))
  CHECK_BB(gwi_class_end(gwi))
  CHECK_BB(gwi_oper_ini(gwi, "@null", "Object", "Object"))
  CHECK_BB(gwi_oper_add(gwi, at_object))
  CHECK_BB(gwi_oper_end(gwi, op_ref, ObjectAssign))
  CHECK_BB(gwi_oper_ini(gwi, "Object", "Object", NULL))
  CHECK_BB(gwi_oper_add(gwi, at_object))
  CHECK_BB(gwi_oper_end(gwi, op_ref, ObjectAssign))
  CHECK_BB(gwi_oper_ini(gwi, "Object", "Object", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_eq,  EqObject))
  CHECK_BB(gwi_oper_end(gwi, op_ne, NeqObject))
  CHECK_BB(gwi_oper_add(gwi, opck_object_cast))
  CHECK_BB(gwi_oper_emi(gwi, opem_basic_cast))
  CHECK_BB(gwi_oper_end(gwi, op_cast, NULL))
  CHECK_BB(gwi_oper_ini(gwi, "@null", "Object", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_eq,  EqObject))
  CHECK_BB(gwi_oper_end(gwi, op_ne, NeqObject))
  CHECK_BB(gwi_oper_add(gwi, opck_basic_cast))
  CHECK_BB(gwi_oper_emi(gwi, opem_basic_cast))
  CHECK_BB(gwi_oper_end(gwi, op_cast, NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_implicit_null2obj))
  CHECK_BB(gwi_oper_end(gwi, op_impl, NULL))
  CHECK_BB(gwi_oper_ini(gwi, "Object", "@null", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_eq, EqObject))
  CHECK_BB(gwi_oper_end(gwi, op_ne, NeqObject))
  CHECK_BB(gwi_oper_ini(gwi, NULL, "Object", "int"))
  CHECK_BB(gwi_oper_add(gwi, opck_unary_meta2))
  CHECK_BB(gwi_oper_end(gwi, op_not, IntNot))
  gwi_item_ini(gwi, "@null", "null");
  gwi_item_end(gwi, 0, NULL);
  gwi_reserve(gwi, "this");
  return GW_OK;
}
