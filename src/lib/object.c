#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "nspc.h"
#include "value.h"
#include "instr.h"
#include "object.h"
#include "import.h"
#include "operator.h"

ANN void exception(const VM_Shred shred, const m_str c) {
  err_msg(0, "%s: shred[id=%" UINT_F ":%s], PC=[%" UINT_F "]",
          c, shred->xid, shred->name, shred->pc - 1);
  vm_shred_exit(shred);
}

M_Object new_object(const VM_Shred shred, const Type t) {
  const M_Object a = mp_alloc(M_Object);
  a->ref = 1;
  a->type_ref = t;
  if(t->nspc->offset) {
    Type type = t;
    while(!type->p)
      type = type->parent;
    a->p = type->p;
    a->data = (m_bit*)_mp_alloc2(type->p);
  }
  if(shred)
    vector_add(&shred->gc, (vtype)a);
  return a;
}

M_Object new_string(const VM_Shred shred, const m_str str) {
  const M_Object o = new_object(shred, t_string);
  STRING(o) = s_name(insert_symbol(str));
  return o;
}

M_Object new_string2(const VM_Shred shred, const m_str str) {
  const M_Object o = new_object(shred, t_string);
  STRING(o) = str;
  return o;
}

ANN void instantiate_object(const VM_Shred shred, const Type type) {
  const M_Object object = new_object(NULL, type);
  *(M_Object*)REG(0) =  object;
  PUSH_REG(shred, SZ_INT);
}

ANN static void handle_dtor(const M_Object object, const VM_Shred shred) {
  const Type t = object->type_ref;
  const VM_Shred sh = new_vm_shred(t->nspc->dtor);
  ADD_REF(t->nspc->dtor);
  memcpy(sh->base, shred->base, SIZEOF_MEM);
  memcpy(sh->reg, shred->reg, SIZEOF_REG);
  *(M_Object*)sh->mem = object;
  sh->mem += SZ_INT;
  *(M_Object*)sh->mem = object;
  vm_add_shred(shred->vm, sh);
  ++sh->me->ref;
}
__attribute__((hot))
ANN void __release(const M_Object obj, const VM_Shred shred) {
  Type t = obj->type_ref;
  while(t) {
    const Vector v = scope_get(&t->nspc->value);
    for(m_uint i = 0; i < vector_size(v); i++) {
      const Value value = (Value)vector_at(v, i);
      if(!GET_FLAG(value, static) && isa(value->type, t_object) > 0)
        release(*(M_Object*)(obj->data + value->offset), shred);
    }
    free_vector(v);
    if(GET_FLAG(t, dtor)) {
      if(t->nspc->dtor->native_func)
        ((f_xtor)t->nspc->dtor->native_func)(obj, shred);
      else {
        handle_dtor(obj, shred);
        return;
      }
    }
    t = t->parent;
  }
}

void free_object(const M_Object o) {
  if(o->data)
    _mp_free2(o->p, o->data);
  mp_free(M_Object, o);
}

static DTOR(object_dtor) { free_object(o); }
INSTR(ObjectAssign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  const M_Object src = *(M_Object*)REG(-SZ_INT);
  const M_Object tgt = **(M_Object**)REG(0);
  if(tgt) {
    --tgt->ref;
    _release(tgt, shred);
  }
  if(instr->m_val)
    *(m_bit**)REG(-SZ_INT) = REG(-SZ_INT);
  **(M_Object**)REG(0) = src;
}

#define describe_logical(name, op)               \
static INSTR(name##Object) { GWDEBUG_EXE        \
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
  if(l != t_null && isa(l, r) < 0)
    ERR_N(bin->self->pos, "'%s' @=> '%s': not allowed", l->name, r->name)
  bin->rhs->emit_var = 1;
  return r;
}

static OP_CHECK(opck_object_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  const Type l = cast->exp->type;
  const Type r = cast->self->type;
  return isa(l, r) > 0 ? r : t_null;
}

static OP_CHECK(opck_implicit_null2obj) {
  const struct Implicit* imp = (struct Implicit*)data;
  return imp->t;
}

GWION_IMPORT(object) {
  CHECK_OB((t_object  = gwi_mk_type(gwi, "Object", SZ_INT, NULL)))
  CHECK_BB(gwi_class_ini(gwi, t_object, NULL, object_dtor))
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
  CHECK_BB(gwi_oper_add(gwi, opck_unary_meta))
  CHECK_BB(gwi_oper_end(gwi, op_not, IntNot))
  gwi_item_ini(gwi, "@null", "null");
  gwi_item_end(gwi, ae_flag_global, NULL);
  return 1;
}
