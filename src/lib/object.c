#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"

struct Type_ t_null    = { "@null",     SZ_INT, NULL,      te_null};
struct Type_ t_object  = { "Object",    SZ_INT, NULL,      te_object };

void NullException(VM_Shred shred, const m_str c) {
  err_msg(INSTR_, 0, "%s: shred[id=%lu:%s], PC=[%lu]\n",
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
  object->vtable = &type->info->obj_v_table;
  object->type_ref = type;
  if(type->obj_size) {
    if(!(object->data = calloc(type->obj_size, sizeof(unsigned char))))
      CHECK_BB(err_msg(TYPE_, 0,
          "OutOfMemory: while instantiating object '%s'\n", type->name))
  }
  return 1;
}

static void handle_dtor(Type t, VM_Shred shred) {
  VM_Code code = new_vm_code(t->info->dtor->instr, SZ_INT, 1,
       "[dtor]", "[in code dtor exec]");
  VM_Shred sh = new_vm_shred(code);
  sh->me = new_shred(shred->vm_ref, sh);
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
#ifdef DEBUG_INSTR
      debug_msg("instr", "dtor loop %p %s", obj, t->name);
#endif
      m_uint i;
      Vector v = scope_get(&t->info->value);
      for(i = 0; i < vector_size(v); i++) {
        Value value = (Value)vector_at(v, i);
        if(!GET_FLAG(value, ae_flag_static) && isprim(value->m_type) < 0)
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
#ifdef DEBUG_INSTR
  debug_msg("instr", "assign object %lu %p %p", instr->m_val,
            *(m_uint*)REG(- SZ_INT * 2), **(m_uint**)REG(- SZ_INT));
#endif
  M_Object tgt, src;
  POP_REG(shred, SZ_INT * 2);
  src = *(M_Object*)REG(0);
  if((tgt = **(M_Object**)REG(SZ_INT)))
    release(tgt, shred);
  /*if(instr->m_val2)*/
  release(tgt, shred);
  **(M_Object**)REG((instr->m_val ? 0 : SZ_INT)) = src;
  **(M_Object**)REG(SZ_INT) = src;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(eq_Object) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "eq Object");
#endif
  POP_REG(shred, SZ_INT * 2);
  m_uint* lhs = *(m_uint**)REG(0);
  m_uint* rhs = *(m_uint**)REG(SZ_INT);
  *(m_uint*)REG(0) = (lhs == rhs);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(neq_Object) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "neq Object");
#endif
  POP_REG(shred, SZ_INT * 2);
  m_uint* lhs = *(m_uint**)REG(0);
  m_uint* rhs = *(m_uint**)REG(SZ_INT);
  *(m_uint*)REG(0) = (lhs != rhs);
  PUSH_REG(shred, SZ_INT);
}

m_bool import_object(Importer importer) {
  CHECK_BB(importer_class_begin(importer, &t_object, NULL, object_dtor))
  CHECK_BB(importer_oper_begin(importer, "@null", "Object", "Object"))
  CHECK_BB(importer_add_op(importer, op_at_chuck, Assign_Object, 1))
  CHECK_BB(importer_oper_begin(importer, "Object", "Object", "Object"))
  CHECK_BB(importer_add_op(importer, op_at_chuck, Assign_Object, 1))
  CHECK_BB(importer_oper_begin(importer, "Object", "Object", "int"))
  CHECK_BB(importer_add_op(importer, op_eq,  eq_Object, 1))
  CHECK_BB(importer_add_op(importer, op_neq, neq_Object, 1))
  CHECK_BB(importer_oper_begin(importer, "@null", "Object", "int"))
  CHECK_BB(importer_add_op(importer, op_eq,  eq_Object, 1))
  CHECK_BB(importer_add_op(importer, op_neq, neq_Object, 1))
  CHECK_BB(importer_oper_begin(importer, "Object", "@null", "int"))
  CHECK_BB(importer_add_op(importer, op_eq, eq_Object, 1))
  CHECK_BB(importer_add_op(importer, op_neq, neq_Object, 1))
  CHECK_BB(importer_oper_begin(importer, NULL, "Object", "int"))
  CHECK_BB(importer_add_op(importer, op_exclamation, int_not, 1))
  CHECK_BB(importer_class_end(importer))
  return 1;
}
