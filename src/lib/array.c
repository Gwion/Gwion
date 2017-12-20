#include <math.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"

struct Type_ t_array  = { "Array", SZ_INT, &t_object, te_array };

m_int o_object_array;

struct M_Vector_ {
  char*  ptr;   // data
  m_uint len;   // number of elements * size
  m_uint size;  // size of objects
  m_uint depth;
  m_uint cap;
};

DTOR(array_dtor) {
  if(o->type_ref->d.array_type) {// maybe unnecessary. preferably check array depth
    if(ARRAY(o)->depth > 1 || isa(o->type_ref->d.array_type, &t_object) > 0) {
      m_uint i;
      for(i = 0; i < ARRAY(o)->len * SZ_INT; i += SZ_INT)
        release(*(M_Object*)(ARRAY(o)->ptr + i), shred);
    }
  }
  free(ARRAY(o)->ptr);
  free(ARRAY(o));
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

void m_vector_get(M_Vector v, m_uint i, char* c) {
  memcpy(c, v->ptr + i * v->size, v->size);
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

void m_vector_set(M_Vector v, m_uint i, char* data) {
  memcpy(v->ptr + i * v->size, data,  v->size);
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

MFUN(vm_vector_rem) {
  m_int index = *(m_int*)(shred + SZ_INT);
  M_Vector v = ARRAY(o);
  if(index < 0 || index >= v->len)
    return;
  if(isa(o->type_ref, &t_object) > 0)
    release((M_Object)i_vector_at(v, index), shred);
  m_vector_rem(v, index);
}

char* m_vector_addr(M_Vector v, m_uint i) {
  return &*(char*)(v->ptr + i * v->size);
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

static Type get_array_type(Type t) {
  while(t->d.array_type)
    t = t->d.array_type;
  return t;
}

static Type at_array(Env env, void* data) {
  Exp_Binary* bin = (Exp_Binary*)data;
  Type l = get_array_type(bin->lhs->type);
  Type r = get_array_type(bin->rhs->type);
  
  if(isa(l, r) < 0) {
    err_msg(TYPE_, bin->pos, "array types do not match.");
    return &t_null;
  }
  if(bin->lhs->type->array_depth != bin->rhs->type->array_depth) {
    err_msg(TYPE_, bin->pos, "array depths do not match.");
    return &t_null;
  }
  bin->rhs->emit_var = 1;
  return bin->rhs->type;
}

static Type shift_array(Env env, void* data) {
  exit(2);
  Exp_Binary* bin = (Exp_Binary*)data;
  Type l = get_array_type(bin->lhs->type);
  Type r = get_array_type(bin->rhs->type);
  if(isa(l, r) < 0) {
    err_msg(TYPE_, bin->pos, "array types do not match.");
    return &t_null;
  }
  if(bin->lhs->type->array_depth != bin->rhs->type->array_depth + 1)
    return &t_null;
  return bin->lhs->type;
}

m_bool import_array(Importer importer) {
  SET_FLAG((&t_array), ae_flag_abstract);
  CHECK_BB(importer_class_ini(importer,  &t_array, NULL, array_dtor))

	importer_item_ini(importer, "int", "@array");
  o_object_array = importer_item_end(importer, ae_flag_member, NULL);
  CHECK_BB(o_object_array)

  importer_func_ini(importer, "int", "size", (m_uint)vm_vector_size);
  CHECK_BB(importer_func_end(importer, 0))

  importer_func_ini(importer, "int", "depth", (m_uint)vm_vector_depth);
  CHECK_BB(importer_func_end(importer, 0))

  importer_func_ini(importer, "int", "cap", (m_uint)vm_vector_cap);
  CHECK_BB(importer_func_end(importer, 0))

  importer_func_ini(importer, "int", "remove", (m_uint)vm_vector_rem);
  importer_func_arg(importer, "int", "index");
  CHECK_BB(importer_func_end(importer, 0))

  CHECK_BB(importer_class_end(importer))
  CHECK_BB(importer_oper_ini(importer, "Array", "Array", NULL))
  CHECK_BB(importer_oper_add(importer, at_array))
  CHECK_BB(importer_oper_end(importer, op_at_chuck, Assign_Object, 0))
  CHECK_BB(importer_oper_add(importer, shift_array))
  CHECK_BB(importer_oper_end(importer, op_shift_left, Array_Append, 0))
  return 1;
}

INSTR(Instr_Pre_Ctor_Array_Top) {
  if(*(m_uint*)REG(-SZ_INT * 2) >= *(m_uint*)REG(-SZ_INT))
    shred->next_pc = instr->m_val;
  else
    instantiate_object(vm, shred, *(Type*)instr->ptr);
}

INSTR(Instr_Pre_Ctor_Array_Bottom) {
  POP_REG(shred,  SZ_INT);
  M_Object obj = *(M_Object*)REG(0);
  m_uint * array = *(m_uint**)REG(-SZ_INT * 3);
  m_int i = *(m_int*)REG(-SZ_INT * 2);
  *(m_uint*)array[i] = (m_uint)obj;
  (*(m_int*)REG(-SZ_INT * 2))++;
  shred->next_pc = instr->m_val;
}

INSTR(Instr_Pre_Ctor_Array_Post) {
  POP_REG(shred,  SZ_INT * 3);
  m_uint* array = *(m_uint**)REG(0);
  free(array);
}

struct ArrayAllocInfo {
  m_int capacity;
  const m_int top;
  Type type;
  m_bool is_obj;
  m_uint* objs;
  m_int* index;
};

static M_Object do_alloc_array_object(struct ArrayAllocInfo* info, m_int cap) {
  M_Object base;
  if(cap < 0) {
    fprintf(stderr, "[gwion](VM): NegativeArraySize: while allocating arrays...\n");
    return NULL;
  }
  base = new_M_Array(info->capacity >= info->top ?
      info->type->d.array_type->size : SZ_INT, cap, -info->capacity);
  if(!base) {
    fprintf(stderr, "[gwion](VM): OutOfMemory: while allocating arrays...\n");
    return NULL;
  }
  base->type_ref = info->type; // /13/03/17 
  ADD_REF(info->type);
  return base;
}

static M_Object do_alloc_array_init(struct ArrayAllocInfo* info, m_int cap,
    M_Object base) {
  if(info->is_obj && info->objs) {
    m_int i;
    for(i = 0; i < cap; i++) {
      info->objs[*info->index] = (m_uint)m_vector_addr(ARRAY(base), i);
      (*info->index)++;
    }
  }
  return base;
}

static M_Object do_alloc_array(VM_Shred shred, struct ArrayAllocInfo* info);
static M_Object do_alloc_array_loop(VM_Shred shred, struct ArrayAllocInfo* info,
    m_int cap, M_Object base) {
  m_int i;
  for(i = 0; i < cap; i++) {
    struct ArrayAllocInfo aai = { info->capacity + 1, info->top, info->type,
      info->is_obj, info->objs, info->index };
    M_Object next = do_alloc_array(shred, &aai);
    if(!next) {
      release(base, shred);
      return NULL;
    }
    i_vector_set(ARRAY(base), i, (m_uint)next);
  }
  return base;
}

static M_Object do_alloc_array(VM_Shred shred, struct ArrayAllocInfo* info) {
  M_Object base;
  m_int cap = *(m_int*)REG(info->capacity * SZ_INT);

  if(!(base = do_alloc_array_object(info, cap)))
    return NULL;
  if(info->capacity >= info->top)
    return do_alloc_array_init(info, cap, base);
  else
    return do_alloc_array_loop(shred, info, cap, base);
  return base;
}

INSTR(Instr_Array_Init) { // for litteral array
  m_uint i;
  VM_Array_Info* info = *(VM_Array_Info**)instr->ptr;
  M_Object obj;
  POP_REG(shred, instr->m_val2 * info->length);
  obj = new_M_Array(info->type->d.array_type->size, info->length, info->depth);
  obj->type_ref = info->type;
  vector_add(&shred->gc, (vtype) obj);
  for(i = 0; i < info->length; i++)
    m_vector_set(ARRAY(obj), i, REG(instr->m_val2 * i));
  *(M_Object*)REG(0) = obj;
  PUSH_REG(shred,  SZ_INT);
  instr->m_val = 1;
}

static m_uint* init_array(VM_Shred shred, VM_Array_Info* info, m_uint* num_obj) {
  m_int curr = -info->depth;
  m_int top = - 1;
  m_int tmp;
  *num_obj = 1;
  while(curr <= top) {
    tmp = *(m_int*)REG(SZ_INT * curr);
    *num_obj *= tmp;
    curr++;
  }
  if(*num_obj > 0)
    return (m_uint*)calloc(*num_obj, sizeof(m_uint));
  return (m_uint*)1;
}

INSTR(Instr_Array_Alloc) {
  VM_Array_Info* info = *(VM_Array_Info**)instr->ptr;
  M_Object ref;
  m_uint num_obj = 0;
  m_int index = 0;
  struct ArrayAllocInfo aai = { -info->depth, -1, info->type,  info->is_obj, NULL, &index};
  if(info->is_obj && !info->is_ref &&
      !(aai.objs = init_array(shred, info, &num_obj)))
      goto out_of_memory;
  if(!(ref = do_alloc_array(shred, &aai)))
    goto error;
  POP_REG(shred, SZ_INT * info->depth);
  *(M_Object*)REG(0) = ref;
  PUSH_REG(shred,  SZ_INT);
  if(info->is_obj && !info->is_ref) {
    *(m_uint**)REG(0) = aai.objs;
    PUSH_REG(shred,  SZ_INT);
    *(m_uint*) REG(0) = 0;
    PUSH_REG(shred,  SZ_INT);
    *(m_uint*) REG(0) = num_obj;
    PUSH_REG(shred,  SZ_INT);
  }
  REM_REF(info->type);
  instr->m_val = 1;
  return;

out_of_memory:
  fprintf(stderr, "[Gwion](VM): OutOfMemory: while allocating arrays...\n"); // LCOV_EXCL_LINE
  goto error;                                                                  // LCOV_EXCL_LINE
error:
  fprintf(stderr, "[Gwion](VM): (note: in shred[id=%" UINT_F ":%s])\n", shred->xid, shred->name);
  release(shred->me, shred);
  shred->me = NULL;
}

static void array_push(VM_Shred shred, M_Vector a, m_uint i, Kindof kind, m_bool emit_var) {
  if(emit_var)
      *(char**)REG(0) = m_vector_addr(a, i);
  else
    m_vector_get(a, i, REG(0));
  PUSH_REG(shred,  kind);
}

static void oob(M_Object obj, VM_Shred shred, m_int i) {
  fprintf(stderr,
          "[Gwion](VM): ArrayOutofBounds: in shred[id=%" UINT_F ":%s], PC=[%" UINT_F "], index=[%" UINT_F "]\n",
          shred->xid, shred->name, shred->pc, i);
  release(obj, shred);
  release(shred->me, shred);
  shred->me = NULL;
}

#define OOB(shred, obj, i)  if(i < 0 || i >=  m_vector_size(ARRAY(obj))) { \
  oob(obj, shred, i); return; }

INSTR(Instr_Array_Access) {
  m_int i = 0;
  M_Object obj;
  POP_REG(shred,  SZ_INT * 2);
  if(!(obj = *(M_Object*)REG(0)))
    Except(shred, "NullPtrException");
  i = *(m_int*)REG(SZ_INT);
  OOB(shred, obj, i)
  array_push(shred, ARRAY(obj), i, instr->m_val2, instr->m_val);
}

INSTR(Instr_Array_Access_Multi) {
  m_int i, j;
  POP_REG(shred,  SZ_INT * (instr->m_val + 1));
  M_Object obj, *base = (M_Object*)REG(0);
  if(!(obj = *base))
    Except(shred, "NullPtrException");
  for(j = 0; j < instr->m_val - 1; j++) {
    i = *(m_int*)REG(SZ_INT * (j + 1));
    OOB(shred, *base, *(m_int*)REG(SZ_INT * (j + 1)))
    if(!(obj = (M_Object)i_vector_at(ARRAY(obj), i))) {
      release(*base, shred);
      Except(shred, "NullPtrException");
    }
  }
  i = *(m_int*)REG(SZ_INT * (j + 1));
  OOB(shred, *base,*(m_int*)REG(SZ_INT * (j + 1)))
  array_push(shred, ARRAY(obj), i, instr->m_val2, *(m_uint*)instr->ptr);
}
