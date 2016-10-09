#include "defs.h"
#include "vm.h"
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "lang.h"
#include "import.h"

#include "object.h"

struct Type_ t_object     = { "Object",     sizeof(m_uint), NULL, te_object };
void NullException(VM_Shred shred)
{
  fprintf( stderr,
    "[chuck](VM): NullPointerException: shred[id=%lu:%s], PC=[%lu]\n",
    shred->xid, shred->name, shred->pc );
  // do something!
  shred->is_running = 0;
  shred->is_done = 1;
}
M_Object new_M_Object()
{
  M_Object a = calloc(1, sizeof(struct M_Object_));
  a->vtable = NULL;
  a->type_ref = NULL;
  a->size = 0;
/*  a->array = NULL;*/
  a->data = NULL;
  a->ugen = NULL;
	a->ref = 1;
  return a;
}

M_Object new_String(m_str str)
{
  M_Object o = new_M_Object();
  initialize_object(o, &t_string);
  STRING(o) = strdup(str);
  return o;
}
m_bool initialize_object(M_Object object, Type type)
{
  if(!type->info)
	{
		err_msg(TYPE_, 0, "internal error: no type->info for type '%s'", type->name);
    exit(72);
	}
  object->vtable = type->info->obj_v_table;
  object->type_ref = type;
  object->size = type->obj_size;
  if(object->size)
  {
    object->data = calloc(object->size, sizeof(unsigned char));
    if(!object->data)
      goto out_of_memory;
    memset(object->data, 0, object->size);
  }
  else
    object->data = NULL;
  return 1;

out_of_memory:
  fprintf( stderr, "[chuck](VM): OutOfMemory: while instantiating object '%s'\n", type->name);
  return -1;
}

static INSTR(parentize)
{
  M_Object o = (M_Object)instr->m_val;
  o->ref++;
  o->type_ref = o->type_ref->parent;
  vm_add_shred(shred->vm_ref, (VM_Shred)instr->m_val2);
}
void release(M_Object obj, VM_Shred shred)
{
m_uint pc = shred->pc;
  if(!obj)
    return;
  obj->ref--;
  m_uint i, top;
  if(!obj->ref)
  {
    Type t = obj->type_ref;
    while(t)
    {
#ifdef DEBUG_INSTR
  debug_msg("instr", "dtor loop %p %s", obj, t->name);
#endif
     if(t->has_destructor)
     {
        if(t->info->dtor->native_func)
          ((f_dtor)t->info->dtor->native_func)(obj, shred);
        else
        {
          shreduler_remove(shred->vm_ref->shreduler, shred, 0);
          VM_Code code = new_VM_Code(vector_copy((t->info->dtor->instr)), SZ_INT, 1, t->name , "[in code dtor exec]");
          VM_Shred sh = new_VM_Shred(code);
          sh->me = new_Shred(shred->vm_ref, sh);
          sh->base = shred->base;
          memcpy(sh->mem, shred->mem, sizeof(shred->mem));
          vector_pop(code->instr);
          Instr instr = new_Instr();
          instr->execute = parentize;
          instr->m_val   = (m_uint)obj;
          instr->m_val2  = (m_uint)shred;
          vector_append(code->instr, instr);
          Instr eoc = new_Instr();
          eoc->execute = EOC;
          vector_append(code->instr, eoc);
          shred->next_pc--;
          // TODO: where do we free code ?
          vm_add_shred(shred->vm_ref, sh);
          return;
        }
      }
      t = t->parent;
    }
  }
}

void object_dtor(M_Object o, VM_Shred shred)
{
  free(o->data);
  free(o);
  o = NULL;
}

void Assign_Object(VM * vm, VM_Shred shred, Instr instr )
{
#ifdef DEBUG_INSTR
/*  debug_msg("instr", "assign object %i %p %p", instr->m_val, *(m_uint*)(shred->reg -SZ_INT*2), *(m_uint*)(shred->reg - SZ_INT));*/
  debug_msg("instr", "assign object");
#endif
/*  M_Object src;*/
	shred->reg -= SZ_INT*2;
/*  M_Object done = *(M_Object*)shred->reg;*/
/*  M_Object done = **(M_Object**)(shred->reg + SZ_INT);*/
/*  M_Object done = *(M_Object*)(shred->reg + SZ_INT);*/
/*  if(done)*/
/*    release(done, shred);*/
	(**(m_uint**)(shred->reg + SZ_INT) = *(m_uint*)shred->reg);
  shred->reg += SZ_INT;
}

static void eq_Object(VM * vm, VM_Shred shred, Instr instr )
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "eq Object");
#endif
	shred -= SZ_INT*2;
  m_uint* lhs = *(m_uint**)shred->reg;
	m_uint* rhs = *(m_uint**)(shred->reg + SZ_INT);
  *(m_uint*)shred->reg = (*lhs == *rhs);
	shred += SZ_INT;
}

m_bool import_object(Env env)
{
  DL_Func* fun;
  CHECK_BB(add_global_type(env, &t_object))
  CHECK_BB(import_class_begin(env, &t_object, env->global_nspc, NULL, object_dtor))
	env->class_def->doc = "the base class";
  CHECK_BB(add_binary_op(env, op_at_chuck,&t_null,   &t_object, &t_object, Assign_Object, 1))
  CHECK_BB(add_binary_op(env, op_at_chuck,&t_object, &t_object, &t_object, Assign_Object, 1))
  CHECK_BB(add_binary_op(env, op_eq,      &t_object, &t_object, &t_int,    eq_Object,     1))
  add_global_value(env, "NULL",           &t_object, 1, 0);
  CHECK_BB(import_class_end(env))
}
