#include "defs.h"
#include "err_msg.h"
#include "vm.h"
#include "instr.h"
#include "import.h"

struct Type_ t_null    = { "@null",     SZ_INT, NULL,      te_null};
struct Type_ t_object  = { "Object",    SZ_INT, NULL,      te_object };
struct Type_ t_vararg  = { "@Vararg",   SZ_INT, &t_object, te_vararg};
struct Type_ t_varobj  = { "VarObject", SZ_INT, &t_object, te_vararg};
struct Type_ t_varloop = { "@VarLoop",  SZ_INT, NULL,      te_vararg_loop};

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
          release(*(M_Object*)(obj->data + value->offset), shred);
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
  if(instr->m_val2)
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
  struct Vararg* arg = calloc(1, sizeof(struct Vararg));
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
    case Kindof_Void:
      break;
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

struct M_Vector_ {
  char*  ptr;   // data
  m_uint len;   // number of elements * size
  m_uint size;  // size of objects
  m_uint depth;
  m_uint cap;
};


struct Type_ t_array  = { "@Array", SZ_INT, &t_object, te_array };

m_int o_object_array;

DTOR(array_dtor) {
  if(o->type_ref->d.array_type) {// maybe unnecessary. preferably check array depth
    if(ARRAY(o)->depth > 1 || isa(o->type_ref->d.array_type, &t_object) > 0) {
      m_uint i;
      for(i = 0; i < ARRAY(o)->len * SZ_INT; i += SZ_INT)
        release(*(M_Object*)(ARRAY(o)->ptr + i), shred);
    }
  }
  free(ARRAY(o)->ptr);
  REM_REF(o->type_ref);
}

M_Object new_M_Array(m_uint size, m_uint length, m_uint depth) {
  m_uint cap = 1;
  M_Object a = new_M_Object(NULL);
  initialize_object(a, &t_array);
  while(cap < length)
    cap *= 2;
  ARRAY(a)  	  = malloc(sizeof(struct M_Vector_));
  ARRAY(a)->ptr   = calloc(cap, size);
  ARRAY(a)->cap   = cap;
  ARRAY(a)->size  = size;
  ARRAY(a)->len   = length;
  ARRAY(a)->depth = depth;
  return a;
}

m_uint m_vector_size(M_Vector v) {
  return v->len;
}

static inline m_uint m_vector_depth(M_Vector v) {
  return v->depth;
}

static inline m_uint m_vector_cap(M_Vector v) {
  return v->cap;
}

m_uint  i_vector_at(M_Vector v, m_uint i) {
  return *(m_uint*)(v->ptr + i * v->size);
}

m_float f_vector_at(M_Vector v, m_uint i) {
  return *(m_float*)(v->ptr + i * v->size);
}

m_complex c_vector_at(M_Vector v, m_uint i) {
  return *(m_complex*)(v->ptr + i * v->size);
}

m_vec3 v3_vector_at(M_Vector v, m_uint i) {
  return *(m_vec3*)(v->ptr + i * v->size);
}

m_vec4 v4_vector_at(M_Vector v, m_uint i) {
  return *(m_vec4*)(v->ptr + i * v->size);
}

#define CHECK_VEC_SIZE(v)   if(++v->len >= v->cap) { \
    v->cap *=2;                                   \
    v->ptr = realloc(v->ptr, v->cap * v->size);   \
  }                                               \

void i_vector_add(M_Vector v, m_uint i) {
  CHECK_VEC_SIZE(v)
  *(m_uint*)(v->ptr + (v->len - 1)*v->size) = i;
}

void f_vector_add(M_Vector v, m_float f) {
  CHECK_VEC_SIZE(v)
  *(m_float*)(v->ptr + (v->len - 1)*v->size) = f;
}

void c_vector_add(M_Vector v, m_complex c) {
  CHECK_VEC_SIZE(v)
  *(m_complex*)(v->ptr + (v->len - 1)*v->size) = c;
}

void v3_vector_add(M_Vector v, m_vec3 c) {
  CHECK_VEC_SIZE(v)
  *(m_vec3*)(v->ptr + (v->len - 1)*v->size) = c;
}

void v4_vector_add(M_Vector v, m_vec4 c) {
  CHECK_VEC_SIZE(v)
  *(m_vec4*)(v->ptr + (v->len - 1)*v->size) = c;
}

void i_vector_set(M_Vector v, m_uint i, m_uint data) {
  *(m_uint*)(v->ptr + i * v->size) = data;
}

void f_vector_set(M_Vector v, m_uint i, m_float data) {
  *(m_float*)(v->ptr + i * v->size) = data;
}

void c_vector_set(M_Vector v, m_uint i, m_complex data) {
  *(m_complex*)(v->ptr + i * v->size) = data;
}

void v3_vector_set(M_Vector v, m_uint i, m_vec3 data) {
  *(m_vec3*)(v->ptr + i * v->size) = data;
}

void v4_vector_set(M_Vector v, m_uint i, m_vec4 data) {
  *(m_vec4*)(v->ptr + i * v->size) = data;
}

void m_vector_rem(M_Vector v, m_uint index) {
  char c[--v->len * v->size];
  if(index)
    memcpy(c, v->ptr, index * v->size);
  ++index;
  memcpy(c + (index - 1) * v->size, v->ptr + index * v->size, (v->cap - index)*v->size);
  if(v->len > 2 && v->len < v->cap / 2) {
    v->cap /= 2;
    v->ptr = realloc(v->ptr, v->cap * v->size);
  }
  memcpy(v->ptr, c, v->cap * v->size);
}
/*
void m_vector_insert(M_Vector v, m_uint index, char* data)
{
  char c[++v->len*v->size];
  if(index)
    memcpy(c, v->ptr, index*v->size);
  memcpy(c+(index*v->size), data, v->size);
  ++index;
  if(v->len > 2 && v->len < v->cap/2)
    v->cap /= 2;
  memcpy(c + (index) * v->size, v->ptr + index * v->size, (v->cap - index)*v->size);
  if(v->len > 2 && v->len < v->cap/2)
    v->ptr = realloc(v->ptr, v->cap*v->size);
  memcpy(v->ptr, c, v->cap * v->size);
}
*/

MFUN(vm_vector_rem) {
  m_int index = *(m_int*)(shred + SZ_INT);
  M_Vector v = ARRAY(o);
  if(index < 0 || index >= v->len)
    return;
  m_vector_rem(v, index);
}
/*
MFUN(vm_vector_insert_i) {
  m_int index = *(m_int*)(shred + SZ_INT);
  m_int data  = *(m_int*)(shred + SZ_INT*2);
  M_Vector v = o->d.array;
  if(index < 0 || index >= v->len)
	return;
  m_vector_rem(v, index);
}

MFUN(vm_vector_insert_f) {
  m_int index = *(m_int*)(shred + SZ_INT);
  m_float data  = *(m_float*)(shred + SZ_INT*2);
  M_Vector v = o->d.array;
  if(index < 0 || index >= v->len)
	return;
  m_vector_rem(v, index);
}
*/
m_uint*  i_vector_addr(M_Vector v, m_uint i) {
  return &*(m_uint*)(v->ptr + i * v->size);
}

m_float*  f_vector_addr(M_Vector v, m_uint i) {
  return &*(m_float*)(v->ptr + i * v->size);
}

m_complex*  c_vector_addr(M_Vector v, m_uint i) {
  return &*(m_complex*)(v->ptr + i * v->size);
}

m_vec3*  v3_vector_addr(M_Vector v, m_uint i) {
  return &*(m_vec3*)(v->ptr + i * v->size);
}

m_vec4*  v4_vector_addr(M_Vector v, m_uint i) {
  return &*(m_vec4*)(v->ptr + i * v->size);
}

MFUN(vm_vector_size) {
  *(m_uint*)RETURN = o ? m_vector_size(ARRAY(o)) : - 1;
}

MFUN(vm_vector_depth) {
  *(m_uint*)RETURN = o ? m_vector_depth(ARRAY(o)) : - 1;
}

MFUN(vm_vector_cap) {
  *(m_uint*)RETURN = o ? m_vector_cap(ARRAY(o)) : - 1;
}

INSTR(Array_Append) {
  POP_REG(shred, SZ_INT);
  M_Object o = NULL;
  if(instr->m_val == Kindof_Int) {
    POP_REG(shred, SZ_INT);
    o = *(M_Object*)REG(0);
    i_vector_add(ARRAY(o), *(m_uint*)REG(SZ_INT));
  } else if(instr->m_val == Kindof_Float) {
    POP_REG(shred, SZ_FLOAT);
    o = *(M_Object*)REG(0);
    f_vector_add(ARRAY(o), *(m_float*)REG(SZ_INT));
  } else if(instr->m_val == Kindof_Complex) {
    POP_REG(shred, SZ_COMPLEX);
    o = *(M_Object*)REG(0);
    c_vector_add(ARRAY(o), *(m_complex*)REG(SZ_INT));
  } else if(instr->m_val == Kindof_Vec3) {
    POP_REG(shred, SZ_VEC3);
    o = *(M_Object*)REG(0);
    v3_vector_add(ARRAY(o), *(m_vec3*)REG(SZ_INT));
  } else if(instr->m_val == Kindof_Vec4) {
    POP_REG(shred, SZ_VEC4);
    o = *(M_Object*)REG(0);
    v4_vector_add(ARRAY(o), *(m_vec4*)REG(SZ_INT));
  }
  release(o, shred);
  *(M_Object*)REG(0) = o;
  PUSH_REG(shred, SZ_INT);
}

m_bool import_array(Env env) {
  DL_Func fun;
  CHECK_BB(import_class_begin(env, &t_array, env->global_nspc, NULL, array_dtor))

  o_object_array = import_var(env, "int", "@array", ae_flag_member, NULL);
  CHECK_BB(o_object_array)

  dl_func_init(&fun, "int", "size", (m_uint)vm_vector_size);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "depth", (m_uint)vm_vector_depth);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "cap", (m_uint)vm_vector_cap);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "remove", (m_uint)vm_vector_rem);
  dl_func_add_arg(&fun, "int", "index");
  CHECK_BB(import_fun(env, &fun, 0))

  CHECK_BB(import_class_end(env))
  return 1;
}
#include <math.h>

struct Type_ t_string = { "string", SZ_INT, &t_object, te_string };
m_int o_string_data;

static void push_string(VM_Shred shred, M_Object obj, m_str c) {
  STRING(obj) = s_name(insert_symbol(c));
  *(M_Object*)REG(0) = (M_Object)obj;
  PUSH_REG(shred, SZ_INT);
  release(obj, shred);
}

static void push_new_string(VM_Shred shred, m_str c) {
  M_Object obj = new_String(shred, c);
  *(M_Object*)REG(0) = (M_Object)obj;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(String_Assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "string => string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_INT);
  if(rhs && rhs->data) { // assigning with chuck a empty ref
    release(lhs, shred);
    release(rhs, shred);
    STRING(rhs) = lhs ? STRING(lhs) : NULL;
  } else
    **(M_Object**)REG(SZ_INT) = lhs;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(Int_String_Assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "int '=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  m_int lhs = *(m_int*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_INT);
  if(!rhs)
    Except(shred, "NullStringException.");
  char str[num_digit(labs(lhs)) + strlen(STRING(rhs)) + 2];
  sprintf(str, "%li", lhs);
  push_string(shred, rhs, str);
}

static INSTR(Float_String_Assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "float '=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  m_float lhs = *(m_float*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_FLOAT);
  char str[num_digit((m_uint)lhs) + 6];
  if(!rhs)
    Except(shred, "NullStringException.");
  sprintf(str, ".4%f", lhs);
  push_string(shred, rhs, str);
}

static INSTR(Complex_String_Assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Complex '=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_complex lhs = *(m_complex*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_COMPLEX);
  char str[num_digit(creal(lhs)) + num_digit(cimag(lhs)) + 16];
  if(!rhs)
    Except(shred, "NullStringException.");
  sprintf(str, "#(%.4f, %.4f)", creal(lhs), cimag(lhs));
  push_string(shred, rhs, str);
}

static INSTR(Polar_String_Assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Polar '=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_complex lhs = *(m_complex*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_COMPLEX);
  char str[num_digit(creal(lhs)) + num_digit(cimag(lhs)) + 16];
  if(!rhs)
    Except(shred, "NullStringException.");
  sprintf(str, "#(%f, %f)",  creal(lhs),
          cimag(lhs) / M_PI);
  push_string(shred, rhs, str);
}

static INSTR(Vec3_String_Assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Vec3 '=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_VEC3);
  m_vec3 lhs = *(m_vec3*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_VEC3);
  char str[num_digit(lhs.x) + num_digit(lhs.y) + num_digit(lhs.z) + 23];
  if(!rhs)
    Except(shred, "NullStringException.");
  sprintf(str, "#(%.4f, %.4f, %.4f)", lhs.x, lhs.y, lhs.z);
  push_string(shred, rhs, str);
}

static INSTR(Vec4_String_Assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Vec4 '=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_VEC4);
  m_vec4 lhs = *(m_vec4*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_VEC4);
  char str[num_digit(lhs.x) + num_digit(lhs.y) + num_digit(lhs.z) + 30];
  if(!rhs)
    Except(shred, "NullStringException.");
  sprintf(str, "#(%.4f, %.4f, %.4f, %.4f)", lhs.x, lhs.y, lhs.z, lhs.w);
  push_string(shred, rhs, str);
}

static INSTR(Object_String_Assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Object '=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_INT);
  if(!rhs)
    Except(shred, "NullStringException");
  char str[12];
  str[11] = '\0';
  sprintf(str, "0x%08lu", (uintptr_t)lhs);
  push_string(shred, rhs, str);
  release(lhs, shred);
}

static INSTR(String_String) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "string '+' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);
  char str[(lhs ? strlen(STRING(lhs)) : 0) + (rhs ? strlen(STRING(rhs)) : 0) + 1];
  sprintf(str, "%s%s", lhs ? STRING(lhs) : NULL, rhs ? STRING(rhs) : NULL);
  push_new_string(shred, str);
  release(rhs, shred);
  release(lhs, shred);
}

static INSTR(Int_String) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "int '+' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  m_int lhs = *(m_int*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);
  char str[num_digit(lhs) + (rhs ? strlen(STRING(rhs)) : 0) + 1];
  sprintf(str, "%li%s", lhs, rhs ? STRING(rhs) : NULL);
  push_new_string(shred, str);
  release(rhs, shred);
}

static INSTR(Float_String) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "float '+' string");
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  m_float lhs = *(m_float*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_FLOAT);
  char str[num_digit((m_uint)lhs) + 5 + (rhs ? strlen(STRING(rhs)) : 0) + 1];
  sprintf(str, "%.4f%s", lhs, rhs ? STRING(rhs) : NULL);
  push_new_string(shred, str);
  release(rhs, shred);
}

static INSTR(Complex_String) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "complex '+' string");
#endif
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_complex  lhs = *(m_complex*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_COMPLEX);
  char str[num_digit(creal(lhs)) + num_digit(cimag(lhs)) + (rhs ? strlen(STRING(rhs)) : 0) +  12];
  sprintf(str, "#(%.4f, %.4f)%s", creal(lhs), cimag(lhs), rhs ? STRING(rhs) : NULL);
  push_new_string(shred, str);
  release(rhs, shred);
}

static INSTR(Polar_String) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "polar '+' string");
#endif
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_complex  lhs = *(m_complex*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_COMPLEX);
  char str[num_digit(creal(lhs)) + num_digit(cimag(lhs)) + (rhs ? strlen(STRING(rhs)) : 0) +  12];
  sprintf(str, "%%(%f, %f)%s", creal(lhs), cimag(lhs) / M_PI, rhs ? STRING(rhs) : NULL);
  push_new_string(shred, str);
  release(rhs, shred);
}

static INSTR(Vec3_String) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Vec3 '+' string");
#endif
  POP_REG(shred, SZ_INT + SZ_VEC3);
  m_vec3  lhs = *(m_vec3*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_VEC3);
  char str[(rhs ? strlen(STRING(rhs)) : 0) + 23 + num_digit((m_uint)lhs.x) +
                                      num_digit((m_uint)lhs.y) + num_digit((m_uint)lhs.z) ];
  sprintf(str, "@(%.4f, %.4f, %.4f)%s", lhs.x, lhs.y, lhs.z, rhs ? STRING(rhs) : NULL);
  push_new_string(shred, str);
  release(rhs, shred);
}

static INSTR(Vec4_String) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Vec4 '+' string");
#endif
  POP_REG(shred, SZ_INT + SZ_VEC4);
  m_vec4  lhs = *(m_vec4*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_VEC4);
  char str[(rhs ? strlen(STRING(rhs)) : 0) + 28 + num_digit((m_uint)lhs.x) +
                                      num_digit((m_uint)lhs.y) + num_digit((m_uint)lhs.z) + num_digit((m_uint)lhs.w)];
  sprintf(str, "@(%f, %f, %f, %f)%s", lhs.x, lhs.y, lhs.z, lhs.w, rhs ? STRING(rhs) : NULL);
  push_new_string(shred, str);
  release(rhs, shred);
}

static INSTR(Object_String) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Object '+' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);
  char str[11 + (rhs ? strlen(STRING(rhs)) : 0)];
  sprintf(str, "0x%08lu%s", (uintptr_t)lhs, rhs ? STRING(rhs) : NULL);
  push_new_string(shred, str);
  release(rhs, shred);
  release(lhs, shred);
}

static INSTR(String_Plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "string '+=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_INT);
  if(!rhs) { // TODO release
    rhs = lhs;
    *(M_Object*)REG(0) = rhs;
    release(lhs, shred);
    PUSH_REG(shred, SZ_INT);
    return;
  }
  m_uint l_len = strlen(STRING(lhs));
  m_uint r_len = strlen(STRING(rhs));
  char c[l_len + r_len + 1];
  sprintf(c, "%s%s", STRING(rhs), lhs ? STRING(lhs) : NULL);
  push_string(shred, rhs, c);
  release(lhs, shred);
}

static INSTR(Int_String_Plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "int '+=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  m_int lhs = *(m_int*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_INT);
  if(!rhs)
    Except(shred, "NullStringException.");
  m_uint len = strlen(STRING(rhs)) + 1;
  char c[len + num_digit(lhs) + 1];
  sprintf(c, "%s%li", STRING(rhs), lhs);
  push_string(shred, rhs, c);
}

static INSTR(Float_String_Plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "float '+=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  m_float lhs = *(m_float*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_FLOAT);
  m_uint len = (rhs ? STRING(rhs) ? strlen(STRING(rhs)) : 0 : 0) + 1 + 7;
  if(!rhs)
    Except(shred, "NullStringException");
  char c[len + num_digit(lhs) + 6];
  sprintf(c, "%s%.4f", STRING(rhs), lhs);
  push_string(shred, rhs, c);
}

static INSTR(Complex_String_Plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "complex '+=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_float lhs = *(m_float*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_COMPLEX);
  if(!rhs)
    Except(shred, "NullStringException")
  m_uint len = strlen(STRING(rhs)) + 1 + 5 + 13;
  char c[len + num_digit(creal(lhs)) + num_digit(cimag(lhs)) + 18];
  sprintf(c, "%s#(%.4f, %.4f)", STRING(rhs), creal(lhs), cimag(lhs));
  push_string(shred, rhs, c);
}

static INSTR(Polar_String_Plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "polar '+=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_float lhs = *(m_float*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_COMPLEX);
  if(!rhs)
    Except(shred, "NullStringException")
  m_uint len = strlen(STRING(rhs)) + 1 + 5 + 13;
  char c[len + num_digit(creal(lhs)) + num_digit(cimag(lhs)) + 18];
  sprintf(c, "%s%%(%.4f, %.4f)", STRING(rhs), creal(lhs), cimag(lhs) / M_PI);
  push_string(shred, rhs, c);
}

static INSTR(Vec3_String_Plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Vec3 '+=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_VEC3);
  m_vec3 lhs = *(m_vec3*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_VEC3);
  if(!rhs)
    Except(shred, "NullStringException")
  char c[num_digit(lhs.x) + num_digit(lhs.y) + num_digit(lhs.z) + strlen(STRING(rhs)) + 20];
  sprintf(c, "%s#(%.4f, %.4f, %.4f)", STRING(rhs), lhs.x, lhs.y, lhs.z);
  push_string(shred, rhs, c);
}

static INSTR(Vec4_String_Plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Vec4 '+=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_VEC4);
  m_vec4 lhs = *(m_vec4*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_VEC4);
  if(!rhs)
    Except(shred, "NullStringException")
  char c[num_digit(lhs.x) + num_digit(lhs.y) + num_digit(lhs.z) + num_digit(lhs.z) + strlen(STRING(rhs)) + 28];
  sprintf(c, "%s#(%.4f, %.4f, %.4f, %.4f)", STRING(rhs), lhs.x, lhs.y, lhs.z, lhs.w);
  push_string(shred, rhs, c);
}

static INSTR(Object_String_Plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Object '+=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_INT);
  if(!rhs) {
    if(lhs)
      release(rhs, shred);
    Except(shred, "NullStringException")
  }
  m_uint len = strlen(STRING(rhs)) + 11;
  char c[len + 1];
  c[len] = '\0';
  sprintf(c, "%s0x%08lu", STRING(rhs), (uintptr_t)lhs);
  push_string(shred, rhs, c);
  release(lhs, shred);
}

INSTR(Reg_Push_Str) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "push string %s", (m_str)instr->m_val);
#endif
  *(M_Object*)REG(0) = new_String(shred, (m_str)instr->m_val);
  PUSH_REG(shred, SZ_INT);
}

static CTOR(string_ctor) {
  STRING(o) = "this is the default string";
}

static MFUN(string_len) {
  *(m_uint*)RETURN = strlen(STRING(o));
}

static MFUN(string_upper) {
  m_uint i;
  char c[strlen(STRING(o)) + 1];
  strcpy(c, STRING(o));
  for(i = 0; i < strlen(c); i++)
    if(c[i]  >= 'a' && c[i] <= 'z')
      c[i] += 'A' - 'a';
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_lower) {
  m_uint i;
  char c[strlen(STRING(o)) + 1];
  strcpy(c, STRING(o));
  for(i = 0; i < strlen(c); i++)
    if(c[i]  >= 'A' && c[i] <= 'Z')
      c[i] -= 'A' - 'a';
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_ltrim) {
  m_uint i = 0;
  m_str str = STRING(o);
  while(str[i] == ' ')
    i++;
  char c[strlen(str) - i + 1];
  strcpy(c, STRING(o) + i);
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_rtrim) {
  m_str str = STRING(o);
  m_uint len = strlen(str) - 1;
  while(str[len] == ' ')
    len--;
  char c[len + 2];
  strncpy(c, str, len + 1);
  c[len + 1] = '\0';
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_trim) {
  m_str str = STRING(o);
  m_int i, start = 0, end = 0, len = 0;
  while(str[len] != '\0')
    len++;
  for(i = 0; i < len; i++) {
    if(str[i] == ' ')
      start++;
    else break;
  }
  for(i = len - 1; i >= 0; i--) {
    if(str[i] == ' ')
      end++;
    else break;
  }
  if(len - start - end <= 0) {
    *(m_uint*)RETURN = 0;
    return;
  }
  char c[len - start - end + 1];
  for(i = start; i < len - end; i++)
    c[i - start] = str[i];
  c[len - start - end ] = '\0';
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_charAt) {
  m_str str = STRING(o);
  m_int i = *(m_int*)MEM(SZ_INT), len = 0;
  while(str[len] != '\0')
    len++;
  if(i < 0 || i >= len)
    *(m_uint*)RETURN = -1;
  else
    *(m_uint*)RETURN = str[i];
}

static MFUN(string_setCharAt) {
  m_str str = STRING(o);
  m_int i = *(m_int*)MEM(SZ_INT), len = 0;
  m_int c = *(m_int*)MEM(SZ_INT * 2);
  while(str[len] != '\0')
    len++;
  if(i < 0 || i >= len)
    *(m_uint*)RETURN = -1;
  else {
    str[i] = c;
    STRING(o) = s_name(insert_symbol(str));
    *(m_uint*)RETURN = c;
  }
}

static MFUN(string_substring) {
  m_uint i, len = 0;
  m_int  index = *(m_int*)MEM(SZ_INT);
  m_str str = STRING(o);
  while(str[len] != '\0')
    len++;
  if(!len || index > len || (len - index + 1) <= 0) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char c[len - index + 1];
  memset(c, 0, len - index + 1);
  for(i = index; i < len; i++)
    c[i - index] = str[i];
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_substringN) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, (STRING(o)));
  m_int i, len = 0, index = *(m_int*)MEM(SZ_INT);
  m_int end = *(m_int*)MEM(SZ_INT * 2);
  while(str[len] != '\0')
    len++;
  if(end > len) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  len -= end;
  char c[len - index + 1];
  for(i = index; i < len; i++)
    c[i - index] = str[i];
  c[i - index] = '\0';
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_insert) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int i, len = 0, len_insert = 0, index = *(m_int*)MEM(SZ_INT);
  M_Object arg = *(M_Object*)MEM(SZ_INT * 2);

  if(!arg) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char insert[strlen(STRING(arg)) + 1];
  strcpy(insert, STRING(arg));
  while(str[len] != '\0')
    len++;
  len_insert =  strlen(insert);
  char c[len + len_insert + 1];
  for(i = 0; i < index; i++)
    c[i] = str[i];
  for(i = 0; i < len_insert; i++)
    c[i + index] = insert[i];
  for(i = index; i < len; i++)
    c[i + len_insert] = str[i];
  c[len + len_insert] = '\0';
  release(arg, shred);
  *(M_Object*)RETURN = new_String(shred, c);;
}

static MFUN(string_replace) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int i, len = 0, len_insert = 0, index = *(m_int*)MEM(SZ_INT);
  M_Object arg = *(M_Object*)MEM(SZ_INT * 2);
  if(!arg) {
    *(M_Object*)RETURN = o;
    return;
  }
  char insert[strlen(STRING(arg)) + 1];
  strcpy(insert, STRING(arg));
  len = strlen(str);
  len_insert =  strlen(insert);
  if(index >= len  || index < 0 || len < 0 || (index + len_insert + 1) <= 0) {
    release(arg, shred);
    *(M_Object*)RETURN = NULL;
    return;
  }
  char c[index + len_insert + 1];
  for(i = 0; i < index; i++)
    c[i] = str[i];
  for(i = 0; i < len_insert; i++)
    c[i + index] = insert[i];
  c[index + len_insert] = '\0';
  release(arg, shred);
  *(M_Object*)RETURN = new_String(shred, c);;
}

static MFUN(string_replaceN) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int i, len = 0, index = *(m_int*)MEM(SZ_INT);
  M_Object arg = *(M_Object*)MEM(SZ_INT * 3);
  m_int _len = *(m_int*)MEM(SZ_INT * 2);
  if(!arg || index > strlen(STRING(o)) || _len > strlen(STRING(arg))) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char insert[strlen(STRING(arg)) + 1];
  memset(insert, 0, len + 1);
  strcpy(insert, STRING(arg));
  len = strlen(str);
  str[len] = '\0';
  if(index > len)
    index = len - 1;
  char c[len + _len];
  memset(c, 0, len + _len);
  for(i = 0; i < index; i++)
    c[i] = str[i];
  for(i = 0; i < _len; i++)
    c[i + index] = insert[i];
  for(i = index + _len; i < len; i++)
    c[i] = str[i];
  c[len + _len - 1] = '\0';
  release(arg, shred);
  *(M_Object*)RETURN = new_String(shred, c);;
}

static MFUN(string_find) {
  m_str str = STRING(o);
  m_int i = 0, ret = -1;
  char arg = *(m_int*)MEM(SZ_INT);
  while(str[i] != '\0') {
    if(str[i] == arg) {
      ret = i;
      break;
    }
    i++;
  }
  *(m_uint*)RETURN = ret;
}

static MFUN(string_findStart) {
  m_str str = STRING(o);
  char pos = *(m_int*)MEM(SZ_INT);
  char arg = *(m_int*)MEM(SZ_INT * 2);
  m_int i = pos, ret = -1;
  if(!strlen(str)) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  while(str[i] != '\0') {
    if(str[i] == arg) {
      ret = i;
      break;
    }
    i++;
  }
  *(m_uint*)RETURN = ret;
}

static MFUN(string_findStr) {
  if(!strlen(STRING(o))) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int ret = -1;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  if(!obj) {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_str arg = STRING(obj);
  m_int len  = strlen(str);
  m_int i = 0;
  m_int arg_len = strlen(arg);
  while(i < len) {
    if(!strncmp(str + i, arg, arg_len)) {
      ret = i;
      break;
    }
    i++;
  }
  release(obj, shred);
  *(m_uint*)RETURN = ret;
}

static MFUN(string_findStrStart) {
  if(!strlen(STRING(o))) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int ret = -1;
  m_int start = *(m_int*)MEM(SZ_INT);
  M_Object obj = *(M_Object*)MEM(SZ_INT * 2);
  if(!obj) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  m_str arg = STRING(obj);
  m_int len  = strlen(str);
  m_int i = start;
  m_int arg_len = strlen(arg);
  while(i < len) {
    if(!strncmp(str + i, arg, arg_len)) {
      ret = i;
      break;
    }
    i++;
  }
  release(obj, shred);
  *(m_uint*)RETURN = ret;
}

static MFUN(string_rfind) {
  m_str str = STRING(o);
  m_int i = strlen(str) - 1, ret = -1;
  char arg = *(m_int*)MEM(SZ_INT);
  while(i > -1 && str[i] != '\0') {
    if(str[i] == arg) {
      ret = i;
      break;
    }
    i--;
  }
  *(m_uint*)RETURN = ret;
}

static MFUN(string_rfindStart) {
  if(!strlen(STRING(o))) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  char pos = *(m_int*)MEM(SZ_INT);
  char arg = *(m_int*)MEM(SZ_INT * 2);
  m_int i = pos, ret = -1;
  while(i > 0 && str[i] != '\0') {
    if(str[i] == arg) {
      ret = i;
      break;
    }
    i--;
  }
  *(m_uint*)RETURN = ret;
}

static MFUN(string_rfindStr) {
  if(!strlen(STRING(o))) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int ret = -1;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_str arg = STRING(o);
  m_int len  = strlen(str);
  m_int i = len - 1;
  m_int arg_len = strlen(arg);
  while(i) {
    if(!strncmp(str + i, arg, arg_len)) {
      ret = i;
      break;
    }
    i--;
  }
  release(obj, shred);
  *(m_uint*)RETURN = ret;
}

static MFUN(string_rfindStrStart) {
  if(!strlen(STRING(o))) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int ret = -1;
  m_int start = *(m_int*)MEM(SZ_INT);
  M_Object obj = *(M_Object*)MEM(SZ_INT * 2);
  if(!obj) {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_str arg = STRING(obj);

  m_int i = start;
  m_int arg_len = strlen(arg);
  while(i > -1) {
    if(!strncmp(str + i, arg, arg_len)) {
      ret = i;
      break;
    }
    i--;
  }
  release(obj, shred);
  *(m_uint*)RETURN = ret;
}

static MFUN(string_erase) {
  m_str str = STRING(o);
  m_int i;
  m_int start = *(m_int*)MEM(SZ_INT);
  m_int rem = *(m_int*)MEM(SZ_INT * 2);
  m_int len = strlen(str);
  m_int size = len - rem + 1;
  if(start >= len || size <= 0) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char c[size];
  c[size - 1] = '\0';
  for(i = 0; i < start; i++)
    c[i] = str[i];
  for(i = start + rem; i < len; i++)
    c[i - rem] = str[i];
  STRING(o) = s_name(insert_symbol(c));
}

static MFUN(string_toInt) {
  *(m_uint*)RETURN = atoi(STRING(o));
}

static MFUN(string_toFloat) {
  *(m_float*)RETURN = atof(STRING(o));
}

m_bool import_string(Env env) {
  DL_Func fun;

  CHECK_BB(import_class_begin(env, &t_string, env->global_nspc, string_ctor, NULL))

  o_string_data = import_var(env, "int", "@data",   ae_flag_const, NULL);
  CHECK_BB(o_string_data)

  dl_func_init(&fun, "int", "size", (m_uint)string_len);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "string", "upper", (m_uint)string_upper);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "string", "lower", (m_uint)string_lower);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "string", "ltrim", (m_uint)string_ltrim);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "string", "rtrim", (m_uint)string_rtrim);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "string", "trim", (m_uint)string_trim);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "charAt", (m_uint)string_charAt);
  dl_func_add_arg(&fun, "int", "pos");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "charAt", (m_uint)string_setCharAt);
  dl_func_add_arg(&fun, "int", "pos");
  dl_func_add_arg(&fun, "int", "char");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "string", "substring", (m_uint)string_substring);
  dl_func_add_arg(&fun, "int", "start");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "string", "substring", (m_uint)string_substringN);
  dl_func_add_arg(&fun, "int", "start");
  dl_func_add_arg(&fun, "int", "end");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "string", "insert", (m_uint)string_insert);
  dl_func_add_arg(&fun, "int", "pos");
  dl_func_add_arg(&fun, "string", "string");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "string", "replace", (m_uint)string_replace);
  dl_func_add_arg(&fun, "int", "pos");
  dl_func_add_arg(&fun, "string", "string");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "string", "replace", (m_uint)string_replaceN);
  dl_func_add_arg(&fun, "int", "pos");
  dl_func_add_arg(&fun, "int", "n");
  dl_func_add_arg(&fun, "string", "string");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "find", (m_uint)string_find);
  dl_func_add_arg(&fun, "int", "char");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "find", (m_uint)string_findStart);
  dl_func_add_arg(&fun, "int", "pos");
  dl_func_add_arg(&fun, "int", "char");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "find", (m_uint)string_findStr);
  dl_func_add_arg(&fun, "string", "str");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "find", (m_uint)string_findStrStart);
  dl_func_add_arg(&fun, "int", "pos");
  dl_func_add_arg(&fun, "string", "str");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "rfind", (m_uint)string_rfind);
  dl_func_add_arg(&fun, "int", "char");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "rfind", (m_uint)string_rfindStart);
  dl_func_add_arg(&fun, "int", "pos");
  dl_func_add_arg(&fun, "int", "char");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "rfind", (m_uint)string_rfindStr);
  dl_func_add_arg(&fun, "string", "str");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "rfind", (m_uint)string_rfindStrStart);
  dl_func_add_arg(&fun, "int", "pos");
  dl_func_add_arg(&fun, "string", "str");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "void",   "erase", (m_uint)string_erase);
  dl_func_add_arg(&fun, "int", "start");
  dl_func_add_arg(&fun, "int", "length");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "toInt", (m_uint)string_toInt);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "float", "toFloat", (m_uint)string_toFloat);
  CHECK_BB(import_fun(env, &fun, 0))

  CHECK_BB(import_op(env, op_chuck, "string",  "string", "string", String_Assign, 1))
  CHECK_BB(import_op(env, op_chuck, "int",     "string", "string", Int_String_Assign, 1))
  CHECK_BB(import_op(env, op_chuck, "float",   "string", "string", Float_String_Assign, 1))
  CHECK_BB(import_op(env, op_chuck, "complex", "string", "string", Complex_String_Assign, 1))
  CHECK_BB(import_op(env, op_chuck, "polar",   "string", "string", Polar_String_Assign, 1))
  CHECK_BB(import_op(env, op_chuck, "Vec3",    "string", "string", Vec3_String_Assign, 1))
  CHECK_BB(import_op(env, op_chuck, "Vec4",    "string", "string", Vec4_String_Assign, 1))
  CHECK_BB(import_op(env, op_chuck, "Object",  "string", "string", Object_String_Assign, 1))
  CHECK_BB(import_op(env, op_chuck, "@null",    "string", "string", Object_String_Assign, 1))

  CHECK_BB(import_op(env, op_plus, "string",  "string", "string", String_String, 1))
  CHECK_BB(import_op(env, op_plus, "int",     "string", "string", Int_String, 1))
  CHECK_BB(import_op(env, op_plus, "float",   "string", "string", Float_String, 1))
  CHECK_BB(import_op(env, op_plus, "complex", "string", "string", Complex_String, 1))
  CHECK_BB(import_op(env, op_plus, "polar",   "string", "string", Polar_String, 1))
  CHECK_BB(import_op(env, op_plus, "Vec3",    "string", "string", Vec3_String, 1))
  CHECK_BB(import_op(env, op_plus, "Vec4",    "string", "string", Vec4_String, 1))
  CHECK_BB(import_op(env, op_plus, "Object",  "string", "string", Object_String, 1))
  CHECK_BB(import_op(env, op_plus, "@null",  "string", "string", Object_String, 1))

  CHECK_BB(import_op(env, op_plus_chuck, "string",  "string", "string", String_Plus, 1))
  CHECK_BB(import_op(env, op_plus_chuck, "int",     "string", "string", Int_String_Plus, 1))
  CHECK_BB(import_op(env, op_plus_chuck, "float",   "string", "string", Float_String_Plus, 1))
  CHECK_BB(import_op(env, op_plus_chuck, "complex", "string", "string", Complex_String_Plus, 1))
  CHECK_BB(import_op(env, op_plus_chuck, "polar",   "string", "string", Polar_String_Plus, 1))
  CHECK_BB(import_op(env, op_plus_chuck, "Vec3",    "string", "string", Vec3_String_Plus, 1))
  CHECK_BB(import_op(env, op_plus_chuck, "Vec4",    "string", "string", Vec4_String_Plus, 1))
  CHECK_BB(import_op(env, op_plus_chuck, "Object",  "string", "string", Object_String_Plus, 1))
  CHECK_BB(import_op(env, op_plus_chuck, "@null",    "string", "string", Object_String_Plus, 1))
  CHECK_BB(import_class_end(env))
  return 1;
}
#include <libgen.h>
#include "shreduler.h"

struct Type_ t_shred      = { "Shred",      sizeof(m_uint), &t_object, te_shred};
m_int o_shred_me;

M_Object new_shred(VM* vm, VM_Shred shred) {
  M_Object obj = new_M_Object(NULL);
  initialize_object(obj, &t_shred);
  ME(obj) = shred;
  return obj;
}

static MFUN(vm_shred_exit) {
  m_uint i;
  VM_Shred  s = ME(o);
  for(i = 0; i < vector_size(&shred->gc1); i++)
    release((M_Object)vector_at(&shred->gc1, i), shred);
  release(o, shred);
  s->me = NULL;
}

static MFUN(vm_shred_id) {
  VM_Shred  s = ME(o);
  *(m_uint*)RETURN = s ? s->xid : -1;
}

static MFUN(vm_shred_is_running) {
  *(m_uint*)RETURN = ME(o) ? 1 : 0;
}

static MFUN(vm_shred_is_done) {
  *(m_uint*)RETURN = ME(o) ? 0 : 1;
}

static MFUN(shred_yield) {
  VM_Shred  s = ME(o);
  Shreduler sh = shred->vm_ref->shreduler;
  shreduler_remove(sh, s, 0);
  shredule(sh, s, .5);
}

static SFUN(vm_shred_from_id) {
  VM_Shred s = (VM_Shred)vector_at(&shred->vm_ref->shred, *(m_uint*)MEM(SZ_INT) - 1);
  if(!s)
    *(m_uint*)RETURN = 0;
  else {
    *(m_uint*)RETURN = (m_uint)s->me;
    s->me->ref++;
    vector_add(&shred->gc, (vtype) s->me);
  }
}

static MFUN(shred_args) {
  VM_Shred  s = ME(o);
  *(m_uint*)RETURN = s->args ? vector_size(s->args) : 0;
}

static MFUN(shred_arg) {
  m_str str;
  VM_Shred  s = ME(o);
  if(!s->args) {
    *(m_uint*)RETURN = 0;
    return;
  }
  str = (m_str)vector_at(s->args, *(m_uint*)MEM(SZ_INT));
  *(m_uint*)RETURN = str ? (m_uint)new_String(shred, str) : 0;
}

static MFUN(shred_path) {
  VM_Shred  s = ME(o);
  *(m_uint*)RETURN = (m_uint)new_String(shred, s->code->filename);
}

static MFUN(shred_dir) {
  VM_Shred  s = ME(o);
  char c[strlen(s->code->filename) + 1];
  memset(c, 0, strlen(s->code->filename) + 1);
  strncpy(c, s->code->filename, strlen(s->code->filename));
  *(m_uint*)RETURN = (m_uint)new_String(shred, dirname(c));
}

static DTOR(shred_dtor) {
  release(o, shred);
}

m_bool import_shred(Env env) {
  DL_Func fun;

  CHECK_BB(import_class_begin(env, &t_shred, env->global_nspc, NULL, shred_dtor))

  o_shred_me = import_var(env, "int", "@me", ae_flag_member, NULL);
  CHECK_BB(o_shred_me)

  dl_func_init(&fun, "void", "exit", (m_uint)vm_shred_exit);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "running", (m_uint)vm_shred_is_running);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "done", (m_uint)vm_shred_is_done);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "id", (m_uint)vm_shred_id);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "Shred", "fromId", (m_uint)vm_shred_from_id);
  dl_func_add_arg(&fun, "int", "arg1");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "void", "yield", (m_uint)shred_yield);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "args", (m_uint)shred_args);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "string", "arg", (m_uint)shred_arg);
  dl_func_add_arg(&fun, "int", "n");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "string", "path", (m_uint)shred_path);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "string", "dir", (m_uint)shred_dir);
  CHECK_BB(import_fun(env, &fun, 0))

  CHECK_BB(import_class_end(env))
  return 1;
}

struct Type_ t_event = { "Event", SZ_INT, &t_object, te_event };

m_int o_event_shred;

static void event_ctor(M_Object o, VM_Shred shred) {
  EV_SHREDS(o) = new_vector();
}

static void event_dtor(M_Object o, VM_Shred shred) {
  free_vector(EV_SHREDS(o));
}

INSTR(Time_Advance) {
  POP_REG(shred, SZ_FLOAT * 2);
  m_float f = *(m_float*)REG(0);
  *(m_float*)REG(0) = (shred->wake_time += f);
  shredule(vm->shreduler, shred, f);
  PUSH_REG(shred, SZ_FLOAT);
}

static INSTR(Event_Wait) {
  M_Object event;
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  event = *(M_Object*)REG(0);
  if(!event)
    Except(shred, "NullEventWait");
  shred->wait = event;
  shreduler_remove(vm->shreduler, shred, 0);
  Vector v = EV_SHREDS(event);
  vector_add(v, (vtype)shred);
  shred->next_pc++;
  *(m_int*)REG(0) = 1;
  PUSH_REG(shred, SZ_INT);
  release(event, shred);
}

static MFUN(event_signal) {
  VM_Shred sh;
  Vector v = EV_SHREDS(o);
  *(m_uint*)RETURN = vector_size(v);
  if(!*(m_uint*)RETURN)
    Except(shred, "NullEventSignal");
  sh = (VM_Shred)vector_front(v);
  sh->wait = NULL;
  shredule(shred->vm_ref->shreduler, sh, .5);
  vector_rem(v, 0);
}

void broadcast(M_Object o) {
  m_uint i;
  VM_Shred sh;
  for(i = 0; i < vector_size(EV_SHREDS(o)); i++) {
    sh = (VM_Shred)vector_at(EV_SHREDS(o), i);
    sh->wait = NULL;
    shredule(sh->vm_ref->shreduler, sh, .5);
  }
  vector_clear(EV_SHREDS(o));
}

static MFUN(event_broadcast) {
  broadcast(o);
}

m_bool import_event(Env env) {
  DL_Func fun;
  CHECK_BB(import_class_begin(env, &t_event, env->global_nspc, event_ctor, event_dtor))
  o_event_shred = import_var(env, "int", "@shreds", ae_flag_member, NULL);
  CHECK_BB(o_event_shred);
  dl_func_init(&fun, "int", "signal", (m_uint)event_signal);
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "int", "broadcast", (m_uint)event_broadcast);
  CHECK_BB(import_fun(env, &fun, 0))
  CHECK_BB(import_op(env, op_chuck, "Event", "@now", "int", Event_Wait, 1))
  CHECK_BB(import_class_end(env))
  return 1;
}
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

struct Type_ t_fileio  = { "FileIO", SZ_INT, &t_event,  te_fileio };
struct Type_ t_cout    = { "@Cout",  SZ_INT, &t_fileio, te_fileio };
struct Type_ t_cerr    = { "@Cerr",  SZ_INT, &t_fileio, te_fileio };
struct Type_ t_cin     = { "@Cin",   SZ_INT, &t_fileio, te_fileio };

static M_Object gw_cin, gw_cout, gw_cerr;
m_int o_fileio_file;

#define CHECK_FIO(o)   if(!o || !IO_FILE(o)) { err_msg(INSTR_, 0, "trying to write an empty file."); Except(shred, "EmptyFileException"); }
CTOR(fileio_ctor) {
  IO_FILE(o)  = NULL;
}

DTOR(fileio_dtor) {
  if(IO_FILE(o))
    fclose(IO_FILE(o));
}

DTOR(static_fileio_dtor) {
  IO_FILE(o) = NULL;
}

INSTR(int_to_file) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "int to file");
#endif
  POP_REG(shred, SZ_INT)
  M_Object o = **(M_Object**)REG(0);
  release(o, shred);
  CHECK_FIO(o)
  fprintf(IO_FILE(o), "%li", *(m_int*)REG(- SZ_INT));
  *(M_Object*)REG(- SZ_INT) = o;
}

INSTR(float_to_file) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "float to file");
#endif
  POP_REG(shred, SZ_INT)
  M_Object o = **(M_Object**)REG(0);
  o = **(M_Object**)REG(0);
  release(o, shred);
  CHECK_FIO(o)
  fprintf(IO_FILE(o), "%f", *(m_float*)REG(- SZ_FLOAT));
  PUSH_REG(shred, SZ_FLOAT);
  *(M_Object*)REG(- SZ_INT) = o;
}

INSTR(string_to_file) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "string to file");
#endif
  POP_REG(shred, SZ_INT)
  M_Object o = **(M_Object**)REG(0);
  M_Object lhs = *(M_Object*)REG(- SZ_INT);
  release(o, shred);
  release(lhs, shred);
  CHECK_FIO(o)
  fprintf(IO_FILE(o), "%s", lhs ? STRING(lhs) : NULL);
  *(M_Object*)REG(- SZ_INT) = o;
}

INSTR(object_to_file) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "string to file");
#endif
  POP_REG(shred, SZ_INT)
  M_Object o = **(M_Object**)REG(0);
  M_Object lhs = *(M_Object*)REG(- SZ_INT);
  release(o, shred);
  release(lhs, shred);
  CHECK_FIO(o)
  fprintf(IO_FILE(o), "%p", (void*)lhs);
  *(M_Object*)REG(- SZ_INT) = o;
}

INSTR(file_to_int) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "file => int");
#endif
  POP_REG(shred, SZ_INT)
  int ret;
  M_Object o = *(M_Object*)REG(- SZ_INT);
  if(!o) {
    Except(shred, "EmptyFileException");
  }
  release(o, shred);
  if(IO_FILE(o)) {
    if(fscanf(IO_FILE(o), "%i", &ret) < 0) {
      Except(shred, "FileReadException");                                     // LCOV_EXCL_LINE
    }
    *(m_uint*)REG(- SZ_INT) = (**(m_uint**)REG(0) = ret);
  } else {
    release(o, shred);
    Except(shred, "EmptyFileException");
  }
}

INSTR(file_to_float) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "file => float");
#endif
  POP_REG(shred, SZ_INT)
  /*  m_float ret;*/
  float ret;
  M_Object o = *(M_Object*)REG(- SZ_INT);
  if(!o) {
    Except(shred, "EmptyFileException");
  }
  release(o, shred);
  if(IO_FILE(o)) {
    if(fscanf(IO_FILE(o), "%f", &ret) < 0) {
      Except(shred, "FileReadException");                                     // LCOV_EXCL_LINE
    }
    *(m_float*)REG(- SZ_FLOAT) = (**(m_float**)REG(0) = ret);
  } else {
    release(o, shred);
    Except(shred, "EmptyFileException");
  }
}
/*
m_bool inputAvailable(FILE* f)
{
  int fd = fileno(f);
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
//  FD_SET(STDIN_FILENO, &fds);
//  select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
  FD_SET(fd, &fds);
  select(fd + 1, &fds, NULL, NULL, &tv);
  return (FD_ISSET(0, &fds));
}
*/
INSTR(file_to_string) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "file => string");
#endif
  POP_REG(shred, SZ_INT)
  M_Object o    = *(M_Object*)REG(- SZ_INT);
  M_Object s    = **(M_Object**)REG(0);
  if(!o) {
    release(s, shred);
    Except(shred, "EmptyFileException");
  }
  if(!s) {
    release(o, shred);
    Except(shred, "EmptyStringException");
  }
  if(IO_FILE(o)) {
    char c[1025];
    if(fscanf(IO_FILE(o), "%1024s", c) < 0) {
      Except(shred, "FileReadException");                                     // LCOV_EXCL_LINE
    }
    STRING(s) = s_name(insert_symbol(c));
    *(M_Object*)REG(- SZ_INT) = s;
  }
  release(o, shred);
  release(s, shred);
}

MFUN(file_nl) {
  *(m_uint*)RETURN = fwrite("\n",  strlen("\n"), 1, IO_FILE(o));
}

MFUN(file_open) {
  M_Object lhs = *(M_Object*)MEM(SZ_INT * 2);
  M_Object rhs = *(M_Object*)MEM(SZ_INT);
  if(!lhs || !rhs)
    Except(shred, "invalid arguments to FileIO.open()");
  m_str filename = STRING(rhs);
  m_str mode = STRING(lhs);
  release(rhs, shred);
  release(lhs, shred);
  if(IO_FILE(o)) {
    fclose(IO_FILE(o));
    IO_FILE(o) = NULL;
  }
  IO_FILE(o) = fopen(filename, mode);
  *(m_uint*)RETURN = IO_FILE(o) ? 1 : 0;
}

MFUN(file_close) {
  if(IO_FILE(o)) {
    fclose(IO_FILE(o));
    IO_FILE(o) = NULL;
  }
  *(m_uint*)RETURN = !IO_FILE(o) ? 1 : 0;
}

SFUN(file_remove) {
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  if(!obj)
    return;
  release(obj, shred);
  *(m_uint*)RETURN = remove(STRING(*(M_Object*)MEM(SZ_INT)));
}

SFUN(file_list) {
  m_uint i;
  struct dirent **namelist;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_str str;
  if(!obj)
    return;
  str = STRING(obj);
  release(obj, shred);
  if(!str)
    return;
  m_int n = scandir(str, &namelist, NULL, alphasort);
  if(n < 0) {
    *(m_uint*)RETURN = 0;
    return;
  }
  Type t = new_array_type(shred->vm_ref->emit->env, 1, &t_string, shred->vm_ref->emit->env->curr);
  M_Object ret = new_M_Array(SZ_INT, n, 1);
  vector_add(&shred->gc, (vtype)ret);
  ret->type_ref = t;
  for(i = 0; i < n; i++) {
    M_Object string = new_String(NULL, namelist[i]->d_name);
    i_vector_set(ARRAY(ret), i, (m_uint)string);
    free(namelist[i]);
  }
  free(namelist);
  *(m_uint*)RETURN = (m_uint)ret;
}

m_bool import_fileio(Env env) {
  DL_Func fun;

  CHECK_BB(import_class_begin(env, &t_fileio, env->global_nspc, fileio_ctor, fileio_dtor))

  // import vars
  o_fileio_file = import_var(env, "int", "@file", ae_flag_member, NULL);
  CHECK_BB(o_fileio_file)

  // import funcs
  dl_func_init(&fun, "int", "nl", (m_uint)file_nl);
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "int", "open", (m_uint)file_open);
  dl_func_add_arg(&fun, "string", "filename");
  dl_func_add_arg(&fun, "string", "mode");
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "int", "close", (m_uint)file_close);
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "int", "remove", (m_uint)file_remove);
  dl_func_add_arg(&fun, "string", "filename");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))
  dl_func_init(&fun, "string[]", "list", (m_uint)file_list);
  dl_func_add_arg(&fun, "string", "filename");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  // import operators
  CHECK_BB(import_op(env, op_chuck, "int",    "FileIO", "FileIO", int_to_file, 1))
  CHECK_BB(import_op(env, op_chuck, "float",  "FileIO", "FileIO", float_to_file, 1))
  CHECK_BB(import_op(env, op_chuck, "string", "FileIO", "FileIO", string_to_file, 1))
  CHECK_BB(import_op(env, op_chuck, "Object", "FileIO", "FileIO", object_to_file, 1))
  CHECK_BB(import_op(env, op_chuck, "@null",  "FileIO", "FileIO", object_to_file, 1))
  CHECK_BB(import_op(env, op_chuck, "FileIO", "string", "string", file_to_string, 1))
  CHECK_BB(import_op(env, op_chuck, "FileIO", "int",    "int",    file_to_int, 1))
  CHECK_BB(import_op(env, op_chuck, "FileIO", "float",  "float",  file_to_float, 1))
  CHECK_BB(import_class_end(env))

  CHECK_BB(import_class_begin(env, &t_cout, env->global_nspc, NULL, static_fileio_dtor))
  CHECK_BB(import_class_end(env))

  CHECK_BB(import_class_begin(env, &t_cerr, env->global_nspc, NULL, static_fileio_dtor))
  CHECK_BB(import_class_end(env))

  CHECK_BB(import_class_begin(env, &t_cin, env->global_nspc, NULL, static_fileio_dtor))
  CHECK_BB(import_class_end(env))

  gw_cin = new_M_Object(NULL);
  initialize_object(gw_cin, &t_cin);
  EV_SHREDS(gw_cin) = new_vector();
  gw_cout = new_M_Object(NULL);
  initialize_object(gw_cout, &t_cout);
  IO_FILE(gw_cout) = stdout;
  EV_SHREDS(gw_cout) = new_vector();
  gw_cerr = new_M_Object(NULL);
  initialize_object(gw_cerr, &t_cerr);
  IO_FILE(gw_cerr) = stderr;
  EV_SHREDS(gw_cerr) = new_vector();
  env_add_value(env, "cin",  &t_fileio, 1, gw_cin);
  env_add_value(env, "cout", &t_fileio, 1, gw_cout);
  env_add_value(env, "cerr", &t_fileio, 1, gw_cerr);
  return 1;
}

#include "lang.h"
#include "ugen.h"

struct Type_ t_ugen = { "UGen", SZ_INT, &t_object, te_ugen };
m_int o_object_ugen;

m_bool base_tick(UGen u) {
  UGen ugen;
  m_uint i, size = vector_size(&u->ugen);
  if(!size) {
    u->out = 0;
    return 1;
  }
  ugen = (UGen)vector_at(&u->ugen, 0);
  u->out = ugen->out;
  for(i = 1; i < size; i++) {
    ugen = (UGen)vector_at(&u->ugen, i);
    switch(u->op) {
      case 1:
        u->in = (u->out += ugen->out);
        return 1;
      case 2:
        u->in = (u->out -= ugen->out);
        return 1;
      case 3:
        u->in = (u->out *= ugen->out);
        return 1;
      case 4:
        u->in = (u->out /= ugen->out);
        return 1;
    }
  }
  return 1;
}

m_bool dac_tick(UGen u) {
  m_uint  i;
  VM* vm = (VM*)u->ug;
  for(i = u->n_out + 1; --i;)
    vm->sp->out[i - 1] = UGEN(u->channel[i - 1])->out;
  return 1;
}

m_bool adc_tick(UGen u) {
  m_uint  i;
  m_float last = 0;
  VM* vm = (VM*)u->ug;
  for(i = u->n_out + 1; --i;) {
    m_uint j = i - 1;
    M_Object obj = u->channel[j];
    UGEN(obj)->last = vm->in[j];
    last += (UGEN(obj)->out = vm->in[j]);
  }
  u->last = last;
  return 1;
}

void ugen_compute(UGen u) {
  m_uint  i;
  UGen ugen;
  if(u->done)
    return;
  u->done = 1;
  if(u->channel)
    for(i = u->n_chan + 1; --i;)
      ugen_compute(UGEN(u->channel[i - 1]));
  else for(i = vector_size(&u->ugen) + 1; --i;)
      ugen_compute((UGen)vector_at(&u->ugen, i - 1));
  if(u->ref) {
    for(i = u->ref->n_chan + 1; --i;) {
      ugen = UGEN(u->ref->channel[i - 1]);
      ugen->tick(ugen);
    }
    u->tick(u);
    return;
  }
  u->tick(u);
  u->last = u->out;
}

UGen new_UGen() {
  UGen u = (UGen) calloc(1, sizeof(struct UGen_));
  vector_init(&u->to);
  u->tick = base_tick;
  u->op = 1;
  return u;
}

M_Object new_M_UGen() {
  M_Object o = new_M_Object(NULL);
  initialize_object(o, &t_ugen);
  UGEN(o) = new_UGen();
  return o;
}

m_bool assign_ugen(UGen u, m_uint n_in, m_uint n_out, m_bool trig, void* ug) {
  u->n_chan = n_in > n_out ? n_in : n_out;
  if(u->n_chan > 1) {
    u->channel = malloc(u->n_chan * sizeof(M_Object));
    m_uint i;
    for(i = u->n_chan + 1; --i;) {
      m_uint j = i - 1;
      M_Object chan = new_M_UGen();
      assign_ugen(UGEN(chan), n_in > j, n_out > j, 0, NULL);
      UGEN(chan)->tick = base_tick;
      UGEN(chan)->ref = u;
      u->channel[j] =  chan;
    }
  } else
    vector_init(&u->ugen);
  if(trig) {
    u->trig = new_M_UGen();
    assign_ugen(UGEN(u->trig), 1, 1, 0, NULL);
    UGEN(u->trig)->tick = base_tick;
  }
  u->in = u->out = 0;
  u->n_in   = n_in;
  u->n_out  = n_out;
  u->ug     = ug;
  return 1;
}

static INSTR(ugen_connect) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "ugen connect %p %p", *(m_uint*)REG(- SZ_INT * 2), *(m_uint*)REG(- SZ_INT));
#endif
  m_uint i;
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);

  if(!lhs || !UGEN(lhs) || !rhs || !UGEN(rhs)) {
    release(rhs, shred);
    Except(shred, "UgenConnectException");
  }
  if(UGEN(rhs)->n_in) {
    if(UGEN(rhs)->channel) {
      for(i = UGEN(rhs)->n_out + 1; --i;) {
        m_uint j = i - 1;
        M_Object obj = UGEN(rhs)->channel[j];
        if(UGEN(lhs)->n_out > 1) {
          vector_add(&UGEN(obj)->ugen, (vtype)UGEN(UGEN(lhs)->channel[j % UGEN(lhs)->n_out]));
          vector_add(&UGEN(UGEN(lhs)->channel[j % UGEN(lhs)->n_out])->to, (vtype)UGEN(obj));
        } else {
          vector_add(&UGEN(obj)->ugen, (vtype)UGEN(lhs));
          vector_add(&UGEN(lhs)->to, (vtype)UGEN(obj));
        }
      }
    } else {
      vector_add(&UGEN(rhs)->ugen, (vtype)UGEN(lhs));
      vector_add(&UGEN(lhs)->to, (vtype)UGEN(rhs));
    }
  }
  release(lhs, shred);
  release(rhs, shred);
  *(M_Object*)REG(0) = rhs;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(ugen_disconnect) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "ugen connect %p %p", *(m_uint*)REG(- SZ_INT * 2), *(m_uint*)REG(- SZ_INT));
#endif
  m_uint i;
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);
  if(!lhs || !UGEN(lhs) || !rhs || !UGEN(rhs)) {
    release(rhs, shred);
    Except(shred, "UgenConnectException");
  }
  if(UGEN(rhs)->n_in) {
    if(UGEN(rhs)->channel) {
      for(i = UGEN(rhs)->n_out + 1; --i;) {
        M_Object obj = UGEN(rhs)->channel[i - 1];
        UGen ugen = UGEN(obj);
        vector_rem(&ugen->ugen, vector_find(&ugen->ugen, (vtype)UGEN(lhs)));
        vector_rem(&UGEN(lhs)->to, vector_find(&UGEN(lhs)->to, (vtype)ugen));
      }
    } else {
      vector_rem(&UGEN(rhs)->ugen, vector_find(&UGEN(rhs)->ugen, (vtype)UGEN(lhs)));
      vector_rem(&UGEN(lhs)->to, vector_find(&UGEN(lhs)->to, (vtype)UGEN(rhs)));
    }
  }
  release(lhs, shred);
  release(rhs, shred);
  *(M_Object*)REG(0) = rhs;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(trig_connect) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "trig connect %p %p", *(m_uint*)REG(- SZ_INT * 2), *(m_uint*)REG(- SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);
  if(!lhs || !UGEN(lhs) || !rhs || !UGEN(rhs)) {
    release(rhs, shred);
    Except(shred, "UgenConnectException");
  }
  if(UGEN(rhs)->trig) {
    vector_add(&UGEN(UGEN(rhs)->trig)->ugen, (vtype)UGEN(lhs));
    vector_add(&UGEN(lhs)->to, (vtype)UGEN(UGEN(rhs)->trig));
  }
  release(lhs, shred);
  release(rhs, shred);
  *(M_Object*)REG(0) = rhs;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(trig_disconnect) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "trig discconnect %p %p", *(m_uint*)REG(- SZ_INT * 2), *(m_uint*)REG(- SZ_INT));
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);
  if(!lhs || !UGEN(lhs) || !rhs || !UGEN(rhs)) {
    release(rhs, shred);
    Except(shred, "UgenConnectException");
  }
  if(UGEN(rhs)->trig) {
    vector_rem(&UGEN(UGEN(rhs)->trig)->ugen, vector_find(&UGEN(UGEN(rhs)->trig)->ugen, (vtype)UGEN(lhs)));
    vector_rem(&UGEN(lhs)->to, vector_find(&UGEN(lhs)->to, (vtype)UGEN(UGEN(rhs)->trig)));
  }
  release(lhs, shred);
  release(rhs, shred);
  *(M_Object*)REG(0) = rhs;
  PUSH_REG(shred, SZ_INT);
}

static CTOR(ugen_ctor) {
  UGEN(o) = new_UGen();
  vector_add(&shred->vm_ref->ugen, (vtype)UGEN(o));
}

static DTOR(ugen_dtor) {
  UGen ug = UGEN(o);
  m_uint i;
  m_int j = vector_find(&shred->vm_ref->ugen, (vtype)ug);
  if(j > -1)
    vector_rem(&shred->vm_ref->ugen, j);
  for(i = vector_size(&ug->to) + 1; --i;) {
    UGen u = (UGen)vector_at(&ug->to, i - 1);
    if(u->ugen.ptr) {
      m_int index = vector_find(&u->ugen, (vtype)ug);
      if(index > -1)
        vector_rem(&u->ugen, index);
    }
  }

  if(ug->ugen.ptr) {
    for(i = vector_size(&ug->ugen) + 1; --i;) {
      UGen u = (UGen)vector_at(&ug->ugen, i - 1);
      m_int index = vector_find(&u->to, (vtype)ug);
      if(index > -1)
        vector_rem(&u->to, index);
    }
    vector_release(&ug->ugen);
  } else {
    for(i = ug->n_chan + 1; --i;)
      release(ug->channel[i - 1], shred);
    free(ug->channel);
  }
  if(ug->trig)
    release(ug->trig, shred);
  vector_release(&ug->to);
  free(ug);
}

static MFUN(ugen_channel) {
  m_int i = *(m_int*)MEM(SZ_INT);
  if(!UGEN(o)->channel)
    *(M_Object*)RETURN = !i ? o : NULL;
  else if(i < 0 || i >= UGEN(o)->n_chan)
    *(M_Object*)RETURN = NULL;
  else
    *(M_Object*)RETURN = UGEN(o)->channel[i];
}

static MFUN(ugen_get_op) {
  *(m_uint*)RETURN = UGEN(o)->op;
}

static MFUN(ugen_set_op) {
  m_int i = *(m_int*)MEM(SZ_INT);
  if(i < 1 || i > 4)
    err_msg(INSTR_, 0, "invalid op %i", i);
  else
    UGEN(o)->op = i;
  *(m_uint*)RETURN = UGEN(o)->op;
}

static MFUN(ugen_get_last) {
  *(m_float*)RETURN = UGEN(o)->last;
}

m_bool import_ugen(Env env) {
  DL_Func fun;

  CHECK_BB(import_class_begin(env, &t_ugen, env->global_nspc, ugen_ctor, ugen_dtor))

  o_object_ugen = import_var(env, "int", "@ugen", ae_flag_member, NULL);
  CHECK_BB(o_object_ugen)

  dl_func_init(&fun, "UGen", "chan", (m_uint)ugen_channel);
  dl_func_add_arg(&fun, "int", "arg0");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "op", (m_uint)ugen_get_op);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "op", (m_uint)ugen_set_op);
  dl_func_add_arg(&fun, "int", "arg0");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "float", "last", (m_uint)ugen_get_last);
  CHECK_BB(import_fun(env, &fun, 0))

  CHECK_BB(import_op(env, op_chuck,   "UGen", "UGen", "UGen", ugen_connect, 1))
  CHECK_BB(import_op(env, op_unchuck, "UGen", "UGen", "UGen", ugen_disconnect, 1))
  CHECK_BB(import_op(env, op_trig,    "UGen", "UGen", "UGen", trig_connect, 1))
  CHECK_BB(import_op(env, op_untrig,  "UGen", "UGen", "UGen", trig_disconnect, 1))

  CHECK_BB(import_class_end(env))
  return 1;
}
