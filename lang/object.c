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

struct Vararg
{
  Kindof* k; // k(ind)
  char* d;   // d(ata)
  m_uint o, i, s;  // o(ffset), i(ndex)
};

INSTR(Vararg_start)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "vararg start %i", instr->m_val);
#endif
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  if(!arg->d)
    shred->next_pc = instr->m_val2 + 1;
  *(m_uint*)(shred->reg - SZ_INT) = 0;
}

INSTR(Vararg_end)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(m_uint*)(shred->reg -SZ_INT) = 0;
  switch(arg->k[arg->i])
  {
     case Kindof_Int:
        arg->o += SZ_INT;
        break;
     case Kindof_Float:
        arg->o += SZ_FLOAT;
        break;
     case Kindof_Complex:
        arg->o += SZ_COMPLEX;
        break;
  }
  arg->i++;
  if(arg->i == arg->s)
  {
    free(arg->d);
    free(arg->k);
    free(arg);
    shred->next_pc = shred->next_pc;  
  }
   else
   {
    *(m_uint*)(shred->reg -SZ_INT) = 0;
    shred->next_pc = instr->m_val2;   
   }
}

INSTR(Vararg_int)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(m_uint*)shred->reg = *(m_uint*)(arg->d + arg->o);
  shred->reg += SZ_INT;
}
INSTR(Vararg_float)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(m_float*)shred->reg = *(m_float*)(arg->d + arg->o);
  shred->reg += SZ_FLOAT;
}

INSTR(Vararg_complex)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(complex*)shred->reg = *(complex*)(arg->d + arg->o);
  shred->reg += SZ_COMPLEX;
}

INSTR(Vararg_object)
{
  struct Vararg* arg = *(struct Vararg**)(shred->mem + instr->m_val);
  *(M_Object*)shred->reg = *(M_Object*)(arg->d + arg->o);
  shred->reg += SZ_INT;
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
  CHECK_BB(add_global_type(env, &t_vararg))
  CHECK_BB(add_global_type(env, &t_varobj))
  CHECK_BB(import_class_begin(env, &t_vararg, env->global_nspc, NULL, NULL))
/*  fun = new_DL_Func("void", "start", (m_uint)Vararg_start);*/
/*  CHECK_OB(import_mfun(env, fun))*/
/*  fun = new_DL_Func("void", "end", (m_uint)Vararg_end);*/
/*  CHECK_OB(import_mfun(env, fun))*/
  import_mvar(env, "int", "start", 1, 0, "start vararg loop");
  import_mvar(env, "int", "end",   1, 0, "end vararg loop");
  import_mvar(env, "int",   "i",   1, 0, "vararg int");
  import_mvar(env, "float", "f",   1, 0, "vararg float");
  import_mvar(env, "complex", "c", 1, 0, "vararg complex");
  import_mvar(env, "VarObject", "o",  1, 0, "vararg object");
  CHECK_BB(import_class_end(env))
  return 1;
}
