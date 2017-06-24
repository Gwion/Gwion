#include "defs.h"
#include "err_msg.h"
#include "import.h"

struct Type_ t_null    = { "@null",     SZ_INT, NULL,      te_null};
struct Type_ t_object  = { "Object",    SZ_INT, NULL,      te_object };
struct Type_ t_vararg  = { "@Vararg",   SZ_INT, &t_object, te_vararg};
struct Type_ t_varobj  = { "VarObject", SZ_INT, &t_object, te_vararg};
struct Type_ t_varloop = { "@VarLoop",  SZ_INT, NULL,      te_vararg_loop};

void NullException(VM_Shred shred, const m_str c) {
  err_msg(INSTR_, 0, "%s: shred[id=%lu:%s], PC=[%lu]\n",
          c, shred->xid, shred->name, shred->pc);
  shred->is_running = 0;
  shred->is_done = 1;
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
  object->size = type->obj_size;
  if(object->size) {
    object->d.data = calloc(object->size, sizeof(unsigned char));
    if(!object->d.data)
      goto out_of_memory;
  } else
    object->d.data = NULL;
  return 1;

out_of_memory: // LCOV_EXCL_START
  err_msg(TYPE_, 0,
      "OutOfMemory: while instantiating object '%s'\n", type->name);
  return -1;
}             // LCOV_EXCL_STOP

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
        if(isprim(value->m_type) < 0)
          release(*(M_Object*)(obj->d.data + value->offset), shred);
      }
      free_vector(v);
      if(GET_FLAG(t, ae_flag_dtor)) {
        if(t->info->dtor->native_func)
          ((f_xtor)t->info->dtor->native_func)(obj, shred);
        else {
          VM_Code code = new_vm_code(t->info->dtor->instr, SZ_INT, 1,
              "[dtor]", "[in code dtor exec]");
          VM_Shred sh = new_vm_shred(code);
          sh->me = new_shred(shred->vm_ref, sh);
          memcpy(sh->mem, shred->mem, SIZEOF_MEM);
          vector_pop(code->instr);
          Instr eoc = malloc(sizeof(struct Instr_));
          eoc->execute = EOC;
          vector_add(code->instr, (vtype)eoc);
          vm_add_shred(shred->vm_ref, sh);
        }
      }
      t = t->parent;
    }
  }
}

static CTOR(object_ctor) {}
static DTOR(object_dtor) {
  free(o->d.data);
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
//  if(instr->m_val2)
//    release(tgt,shred);
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

struct Vararg {
  Kindof* k;      // k(ind)
  char* d;	// d(ata)
  m_uint o, i, s; // o(ffset), i(ndex)
};

INSTR(Vararg_start) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "vararg start %i", instr->m_val);
#endif
  struct Vararg* arg = *(struct Vararg**)MEM(instr->m_val);
  if(!arg->d)
    shred->next_pc = instr->m_val2 + 1;
  if(!arg->s) {
    POP_REG(shred, SZ_INT); // pop vararg
    free(arg);
    return;
  }
  PUSH_REG(shred, SZ_INT);
  *(m_uint*)REG(- SZ_INT) = 0;
}

INSTR(MkVararg) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Make Vararg %i %p", instr->m_val, (void*)instr->m_val2);
#endif
  POP_REG(shred,  instr->m_val);
  m_uint i;
  Vector kinds = (Vector)instr->m_val2;
  struct Vararg* arg =calloc(1, sizeof(struct Vararg));
  if(instr->m_val) {
    arg->d = malloc(instr->m_val);
    memcpy(arg->d, shred->reg, instr->m_val);
  }  else arg->d = NULL;
  arg->s = kinds ? vector_size(kinds) : 0;
  arg->k = arg->s ? calloc(arg->s, sizeof(Kindof)) : NULL;
  for(i = 0; i < arg->s; i++) {
    arg->k[i] = (Kindof)vector_at(kinds, i);
  }
  arg->o = 0;
  arg->i = 0;
  if(kinds)
    free_vector(kinds);
  *(struct Vararg**)REG(0) = arg;
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Vararg_end) {
  struct Vararg* arg = *(struct Vararg**)MEM(instr->m_val);
  switch(arg->k[arg->i]) {
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
    case Kindof_Void: break;
  }
  PUSH_REG(shred, SZ_INT);
  arg->i++;
  if(arg->i == arg->s) {
    free(arg->d);
    free(arg->k);
    free(arg);
  } else {
    *(m_uint*)REG(- SZ_INT) = 0;
    shred->next_pc = instr->m_val2;
  }
}

INSTR(Vararg_int) {
  struct Vararg* arg = *(struct Vararg**)MEM(instr->m_val);
  *(m_uint*)REG(0) = *(m_uint*)(arg->d + arg->o);
  PUSH_REG(shred, SZ_INT);
}
INSTR(Vararg_float) {
  struct Vararg* arg = *(struct Vararg**)MEM(instr->m_val);
  *(m_float*)REG(0) = *(m_float*)(arg->d + arg->o);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(Vararg_complex) {
  struct Vararg* arg = *(struct Vararg**)MEM(instr->m_val);
  *(m_complex*)REG(0) = *(m_complex*)(arg->d + arg->o);
  PUSH_REG(shred, SZ_COMPLEX);
}

INSTR(Vararg_object) {
  struct Vararg* arg = *(struct Vararg**)MEM(instr->m_val);
  *(M_Object*)REG(0) = *(M_Object*)(arg->d + arg->o);
  PUSH_REG(shred, SZ_INT);
}

INSTR(Vararg_Vec3) {
  struct Vararg* arg = *(struct Vararg**)MEM(instr->m_val);
  *(m_vec3*)REG(0) = *(m_vec3*)(arg->d + arg->o);
  PUSH_REG(shred, SZ_VEC3);
}

INSTR(Vararg_Vec4) {
  struct Vararg* arg = *(struct Vararg**)MEM(instr->m_val);
  *(m_vec4*)REG(0) = *(m_vec4*)(arg->d + arg->o);
  PUSH_REG(shred, SZ_VEC4);
}

m_bool import_object(Env env) {
  CHECK_BB(import_class_begin(env, &t_object, env->global_nspc, object_ctor, object_dtor))
  CHECK_BB(import_op(env, op_at_chuck, "@null", "Object", "Object", Assign_Object, 1))
  CHECK_BB(import_op(env, op_at_chuck, "Object", "Object", "Object", Assign_Object, 1))
  CHECK_BB(import_op(env, op_eq,  "Object", "Object", "int",  eq_Object, 1))
  CHECK_BB(import_op(env, op_neq, "Object", "Object", "int", neq_Object, 1))
  CHECK_BB(import_op(env, op_eq,  "@null",   "Object", "int",  eq_Object, 1))
  CHECK_BB(import_op(env, op_neq, "@null",   "Object", "int", neq_Object, 1))
  CHECK_BB(import_op(env, op_eq,  "Object", "@null", "int",  eq_Object, 1))
  CHECK_BB(import_op(env, op_neq, "Object", "@null", "int", neq_Object, 1))
  CHECK_BB(import_op(env, op_exclamation,   NULL,   "Object", "int", int_not,        1))
  CHECK_BB(import_class_end(env))
  CHECK_BB(env_add_type(env, &t_varobj))
  CHECK_BB(env_add_type(env, &t_varloop))
  CHECK_BB(import_class_begin(env, &t_vararg, env->global_nspc, NULL, NULL))
  import_var(env, "@VarLoop",  "start", ae_flag_const, NULL);
  import_var(env, "@VarLoop",  "end",   ae_flag_const, NULL);
  import_var(env, "int",       "i",     ae_flag_const, NULL);
  import_var(env, "float",     "f",     ae_flag_const, NULL);
  import_var(env, "time",      "t",     ae_flag_const, NULL);
  import_var(env, "dur",       "d",     ae_flag_const, NULL);
  import_var(env, "complex",   "c",     ae_flag_const, NULL);
  import_var(env, "polar",     "p",     ae_flag_const, NULL);
  import_var(env, "Vec3",      "v3",    ae_flag_const, NULL);
  import_var(env, "Vec4",      "v4",    ae_flag_const, NULL);
  import_var(env, "VarObject", "o",     ae_flag_const, NULL);
  CHECK_BB(import_class_end(env))
  return 1;
}
