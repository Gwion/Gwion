#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "value.h"
#include "instr.h"
#include "import.h"

struct Type_ t_null    = { "@null",     SZ_INT, NULL,      te_null};
struct Type_ t_object  = { "Object",    SZ_INT, NULL,      te_object };

void NullException(VM_Shred shred, const m_str c) {
  m_uint i;
  for(i = 0; i < vector_size(&shred->gc1); i++)
    release((M_Object)vector_at(&shred->gc1, i), shred);

  err_msg(INSTR_, 0, "%s: shred[id=%" UINT_F ":%s], PC=[%" UINT_F "]",
          c, shred->xid, shred->name, shred->pc);
  release(shred->me, shred);
  shred->me = NULL;
}

M_Object new_M_Object(VM_Shred shred) {
  M_Object a = calloc(1, sizeof(struct M_Object_));
  a->ref = 1;
  if(shred)
    vector_add(&shred->gc, (vtype)a);
  return a;
}

M_Object new_String(VM_Shred shred, m_str str) {
  M_Object o = new_M_Object(shred);
  initialize_object(o, &t_string);
  STRING(o) = s_name(insert_symbol(str));
  return o;
}

m_bool initialize_object(M_Object object, Type type) {
  object->vtable = &type->info->vtable;
  object->type_ref = type;
  if(type->info->offset) {
    if(!(object->data = calloc(type->info->offset, sizeof(unsigned char))))
      CHECK_BB(err_msg(TYPE_, 0,
          "OutOfMemory: while instantiating object '%s'\n", type->name))
  }
  return 1;
}

void instantiate_object(VM * vm, VM_Shred shred, Type type) {
  M_Object object = new_M_Object(NULL);
  if(!object) Except(shred, "NullPtrException");
  initialize_object(object, type);
  *(M_Object*)REG(0) =  object;
  PUSH_REG(shred,  SZ_INT);
  return;
}

static void handle_dtor(Type t, VM_Shred shred) {
  VM_Code code = new_vm_code(t->info->dtor->instr, SZ_INT, 1, "[dtor]");
// also was filename = "[in code dtor exec]"
  VM_Shred sh = new_vm_shred(code);
  vector_init(&sh->gc);
  memcpy(sh->mem, shred->mem, SIZEOF_MEM);
  vector_pop(code->instr);
  Instr eoc = malloc(sizeof(struct Instr_));
  eoc->execute = EOC;
  vector_add(code->instr, (vtype)eoc);
  vm_add_shred(shred->vm_ref, sh);
}

void release(M_Object obj, VM_Shred shred) {
  if(!obj)
    return;
  if(!--obj->ref) {
    Type t = obj->type_ref;
    while(t) {
      m_uint i;
      Vector v = nspc_get_value(t->info);
      for(i = 0; i < vector_size(v); i++) {
        Value value = (Value)vector_at(v, i);
        if(!GET_FLAG(value, ae_flag_static) && isa(value->m_type, &t_object) > 0)
          release(*(M_Object*)(obj->data + value->offset), shred);
      }
      free_vector(v);
      if(GET_FLAG(t, ae_flag_dtor)) {
        if(t->info->dtor->native_func)
          ((f_xtor)t->info->dtor->native_func)(obj, shred);
        else
          handle_dtor(t, shred);
      }
      t = t->parent;
    }
  }
}

static DTOR(object_dtor) {
  free(o->data);
  free(o);
}

INSTR(Assign_Object) {
  M_Object tgt, src;
  POP_REG(shred, SZ_INT * 2);
  src = *(M_Object*)REG(0);
  if((tgt = **(M_Object**)REG(SZ_INT)))
    release(tgt, shred);
  release(tgt, shred);
  **(M_Object**)REG((instr->m_val ? 0 : SZ_INT)) = src;
  **(M_Object**)REG(SZ_INT) = src;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(eq_Object) {
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);
  *(m_uint*)REG(0) = (lhs == rhs);
  release(lhs, shred);
  release(rhs, shred);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(neq_Object) {
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);
  *(m_uint*)REG(0) = (lhs != rhs);
  release(lhs, shred);
  release(rhs, shred);
  PUSH_REG(shred, SZ_INT);
}

static OP_CHECK(at_object) {
  Exp_Binary* bin = (Exp_Binary*)data;
  Type l = bin->lhs->type;
  Type r = bin->rhs->type;
  if(opck_rassign(env, data) == &t_null)
    return &t_null;
  if(l != &t_null && isa(l, r) < 0) {
    if(err_msg(TYPE_, bin->pos, "'%s' @=> '%s': not allowed", l->name, r->name))
    return &t_null;
  }
  bin->rhs->emit_var = 1;
  return r;
}

static OP_CHECK(opck_object_cast) {
  Exp_Cast* cast = (Exp_Cast*)data;
  Type l = cast->exp->type;
  Type r = cast->self->type;
  return isa(l, r) > 0 ? r : &t_null;
}

static OP_CHECK(opck_implicit_null2obj) {
  struct Implicit* imp = (struct Implicit*)data;
  return imp->t;
}

m_bool import_object(Gwi gwi) {
  CHECK_BB(gwi_class_ini(gwi, &t_object, NULL, object_dtor))
  CHECK_BB(gwi_class_end(gwi))
  CHECK_BB(gwi_oper_ini(gwi, "@null", "Object", "Object"))
  CHECK_BB(gwi_oper_add(gwi, at_object))
  CHECK_BB(gwi_oper_end(gwi, op_at_chuck, Assign_Object))
  CHECK_BB(gwi_oper_ini(gwi, "Object", "Object", NULL))
  CHECK_BB(gwi_oper_add(gwi, at_object))
  CHECK_BB(gwi_oper_end(gwi, op_at_chuck, Assign_Object))
  CHECK_BB(gwi_oper_ini(gwi, "Object", "Object", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_eq,  eq_Object))
  CHECK_BB(gwi_oper_end(gwi, op_neq, neq_Object))
  CHECK_BB(gwi_oper_add(gwi, opck_object_cast))
  CHECK_BB(gwi_oper_emi(gwi, opem_basic_cast))
  CHECK_BB(gwi_oper_end(gwi, op_dollar, NULL))
  CHECK_BB(gwi_oper_ini(gwi, "@null", "Object", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_eq,  eq_Object))
  CHECK_BB(gwi_oper_end(gwi, op_neq, neq_Object))
  CHECK_BB(gwi_oper_add(gwi, opck_basic_cast))
  CHECK_BB(gwi_oper_emi(gwi, opem_basic_cast))
  CHECK_BB(gwi_oper_end(gwi, op_dollar, NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_implicit_null2obj))
  CHECK_BB(gwi_oper_end(gwi, op_implicit, NULL))
  CHECK_BB(gwi_oper_ini(gwi, "Object", "@null", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_eq, eq_Object))
  CHECK_BB(gwi_oper_end(gwi, op_neq, neq_Object))
  CHECK_BB(gwi_oper_ini(gwi, NULL, "Object", "int"))
  CHECK_BB(gwi_oper_add(gwi, opck_unary_meta))
  CHECK_BB(gwi_oper_end(gwi, op_exclamation, int_not))
  return 1;
}
