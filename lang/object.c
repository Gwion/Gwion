#include "defs.h"
#include "err_msg.h"
#include "import.h"
#include "instr.h"
#include "lang.h"
#include "type.h"
#include "vm.h"

#include "object.h"

struct Type_ t_object = { "Object", sizeof(m_uint), NULL, te_object };

void NullException(VM_Shred shred)
{
  err_msg(INSTR_, 0, "NullPointerException: shred[id=%lu:%s], PC=[%lu]\n",
          shred->xid, shred->name, shred->pc);
  shred->is_running = 0;
  shred->is_done = 1;
}

M_Object new_M_Object()
{
  M_Object a = calloc(1, sizeof(struct M_Object_));
  a->vtable = NULL;
  a->type_ref = NULL;
  a->size = 0;
  a->d.data = NULL;
  a->ugen = NULL;
  a->ref = 1;
  return a;
}

M_Object new_String(m_str str)
{
  M_Object o = new_M_Object();
  initialize_object(o, &t_string);
  STRING(o) = S_name(insert_symbol(str));
  o->ref--;
  return o;
}

m_bool initialize_object(M_Object object, Type type)
{
  if (!type->info) {
    err_msg(TYPE_, 0, "internal error: no type->info for type '%s'", type->name);
    return -1;
  }
  object->vtable = type->info->obj_v_table;
  object->type_ref = type;
  object->size = type->obj_size;
  if (object->size) {
    object->d.data = calloc(object->size, sizeof(unsigned char));
    if (!object->d.data)
      goto out_of_memory;
    memset(object->d.data, 0, object->size);
  } else
    object->d.data = NULL;
  return 1;

out_of_memory:
  err_msg(TYPE_, 0, "OutOfMemory: while instantiating object '%s'\n", type->name);
  return -1;
}

void release(M_Object obj, VM_Shred shred)
{
  if (!obj)
    return;
  if (!--obj->ref) {
    Type t = obj->type_ref;
    while (t) {
#ifdef DEBUG_INSTR
      debug_msg("instr", "dtor loop %p %s", obj, t->name);
#endif
      m_uint i;
      Vector v = scope_get(t->info->value);
      for(i = 0; i < vector_size(v); i++) {
        Value value = (Value)vector_at(v, i);
        if(isprim(value->m_type) < 0)
          release(*(M_Object*)(obj->d.data + value->offset), shred);
      }
      free_Vector(v);
      if (t->has_destructor) {
        if (t->info->dtor->native_func)
          ((f_dtor)t->info->dtor->native_func)(obj, shred);
        else {
          VM_Code code = new_VM_Code(t->info->dtor->instr, SZ_INT, 1, "[dtor]", "[in code dtor exec]");
          VM_Shred sh = new_VM_Shred(code);
          sh->me = new_Shred(shred->vm_ref, sh);
          memcpy(sh->mem, shred->mem, SIZEOF_MEM);
		  vector_pop(code->instr);
          Instr eoc = new_Instr();
          eoc->execute = EOC;
          vector_append(code->instr, (vtype)eoc);
          vm_add_shred(shred->vm_ref, sh);
        }
      }
      t = t->parent;
    }
  }
}

void object_dtor(M_Object o, VM_Shred shred)
{
  free(o->d.data);
  free(o);
}

INSTR(Assign_Object)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "assign object %p %p", instr->m_val, *(m_uint*)(shred->reg - SZ_INT * 2), **(m_uint**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object done = **(M_Object**)(shred->reg + SZ_INT);
  if(done)
    release(done, shred);
  (**(m_uint**)(shred->reg + SZ_INT) = *(m_uint*)shred->reg);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(eq_Object)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "eq Object");
#endif
  POP_REG(shred, SZ_INT * 2);
  m_uint* lhs = *(m_uint**)shred->reg;
  m_uint* rhs = *(m_uint**)(shred->reg + SZ_INT);
  *(m_uint*)shred->reg = (*lhs == *rhs);
  PUSH_REG(shred, SZ_INT);
}

struct Vararg {
  Kindof* k;      // k(ind)
  char* d;	// d(ata)
  m_uint o, i, s; // o(ffset), i(ndex)
};

INSTR(Vararg_start)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "vararg start %i", instr->m_val);
#endif
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  if (!arg->d)
    shred->next_pc = instr->m_val2 + 1;
  PUSH_REG(shred, SZ_INT);
  *(m_uint*)(shred->reg - SZ_INT) = 0;
}

INSTR(Vararg_end)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  switch (arg->k[arg->i]) {
  case Kindof_Int:
    arg->o += SZ_INT;
    break;
  case Kindof_Float:
    arg->o += SZ_FLOAT;
    break;
  case Kindof_Complex:
    arg->o += SZ_COMPLEX;
    break;
  case Kindof_Vec3:
    arg->o += SZ_VEC3;
    break;
  case Kindof_Vec4:
    arg->o += SZ_VEC4;
    break;
  // can you reach this ?
  case Kindof_Void:
  case Kindof_Ptr:
    break;
  }
  PUSH_REG(shred, SZ_INT);
  arg->i++;
  if (arg->i == arg->s) {
    free(arg->d);
    free(arg->k);
    free(arg);
  } else {
    *(m_uint*)(shred->reg - SZ_INT) = 0;
    shred->next_pc = instr->m_val2;
  }
}

INSTR(Vararg_int)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(m_uint*)shred->reg = *(m_uint*)(arg->d + arg->o);
  PUSH_REG(shred, SZ_INT);
}
INSTR(Vararg_float)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(m_float*)shred->reg = *(m_float*)(arg->d + arg->o);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(Vararg_complex)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(m_complex*)shred->reg = *(m_complex*)(arg->d + arg->o);
  PUSH_REG(shred, SZ_COMPLEX);
}

INSTR(Vararg_object)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(M_Object*)shred->reg = *(M_Object*)(arg->d + arg->o);
  PUSH_REG(shred, SZ_INT);
}

INSTR(Vararg_Vec3)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(VEC3_T*)shred->reg = *(VEC3_T*)(arg->d + arg->o);
  PUSH_REG(shred, SZ_VEC3);
}

INSTR(Vararg_Vec4)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(VEC4_T*)shred->reg = *(VEC4_T*)(arg->d + arg->o);
  PUSH_REG(shred, SZ_VEC4);
}

m_bool import_object(Env env)
{
  CHECK_BB(add_global_type(env, &t_object))
  CHECK_OB(import_class_begin(env, &t_object, env->global_nspc, NULL, object_dtor))
  env->class_def->doc = "the base class";
  CHECK_BB(add_binary_op(env, op_at_chuck, &t_null, &t_object, &t_object, Assign_Object, 1, 0))
  CHECK_BB(add_binary_op(env, op_at_chuck, &t_object, &t_object, &t_object, Assign_Object, 1, 0))
  CHECK_BB(add_binary_op(env, op_eq, &t_object, &t_object, &t_int, eq_Object, 1, 0))

  add_global_value(env, "NULL", &t_object, 1, 0);

  CHECK_BB(import_class_end(env))
  CHECK_BB(add_global_type(env, &t_vararg))
  CHECK_BB(add_global_type(env, &t_varobj))
  CHECK_OB(import_class_begin(env, &t_vararg, env->global_nspc, NULL, NULL))
  import_mvar(env, "int", "start", 1, 0, "start vararg loop");
  import_mvar(env, "int", "end", 1, 0, "end vararg loop");
  import_mvar(env, "int", "i", 1, 0, "vararg int");
  import_mvar(env, "float", "f", 1, 0, "vararg float");
  import_mvar(env, "time", "t", 1, 0, "vararg time");
  import_mvar(env, "dur", "d", 1, 0, "vararg dur");
  import_mvar(env, "complex", "c", 1, 0, "vararg complex");
  import_mvar(env, "polar", "p", 1, 0, "vararg polar");
  import_mvar(env, "Vec3", "v3", 1, 0, "vararg Vec3");
  import_mvar(env, "Vec4", "v4", 1, 0, "vararg Vec4");
  import_mvar(env, "VarObject", "o", 1, 0, "vararg object");
  CHECK_BB(import_class_end(env))
  return 1;
}
