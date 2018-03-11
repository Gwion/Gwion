#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"

struct Type_ t_array  = { "Array", SZ_INT, &t_object };

m_int o_object_array;

struct M_Vector_ {
  char*  ptr;   // data
  m_uint len;   // number of elements * size
  m_uint size;  // size of objects
  m_uint depth;
  m_uint cap;
};

ANN m_uint m_vector_size(M_Vector v) {
  return v->len;
}

static DTOR(array_dtor) {
  Type base = array_base(o->type_ref);
  if(ARRAY(o)->depth > 1 || isa(base, &t_object) > 0) {
    m_uint i;
    for(i = 0; i < ARRAY(o)->len * SZ_INT; i += SZ_INT)
      release(*(M_Object*)(ARRAY(o)->ptr + i), shred);
  }
  free(ARRAY(o)->ptr);
  free(ARRAY(o));
  REM_REF(o->type_ref)
}

ANN M_Object new_M_Array(const Type t, const m_uint size,
    const m_uint length, const m_uint depth) {
  m_uint cap = 1;
  M_Object a = new_M_Object(NULL);
  initialize_object(a, t);
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

ANN void m_vector_get(M_Vector v, const m_uint i, void* c) {
  memcpy(c, v->ptr + i * v->size, v->size);
}

ANN void m_vector_add(M_Vector v, const void* data) {
  if(++v->len >= v->cap) {
    v->cap *=2;
    v->ptr = realloc(v->ptr, v->cap * v->size);
  }
  memcpy((v->ptr + (v->len - 1)*v->size), data,v->size);
}

ANN void m_vector_set(M_Vector v, const m_uint i, const void* data) {
  memcpy(v->ptr + i * v->size, data,  v->size);
}

ANN void m_vector_rem(M_Vector v, m_uint index) {
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

static MFUN(vm_vector_rem) {
  m_int index = *(m_int*)(shred + SZ_INT);
  M_Vector v = ARRAY(o);
  if(index < 0 || index >= v->len)
    return;
  if(isa(o->type_ref, &t_object) > 0) {
    M_Object obj;
    m_vector_get(v, index, &obj);
    release(obj,shred);
  }
  m_vector_rem(v, index);
}

ANN char* m_vector_addr(M_Vector v, m_uint i) {
  return &*(char*)(v->ptr + i * v->size);
}

static MFUN(vm_vector_size) {
  *(m_uint*)RETURN = ARRAY(o)->len;
}

static MFUN(vm_vector_depth) {
  *(m_uint*)RETURN = ARRAY(o)->depth;
}

static MFUN(vm_vector_cap) {
  *(m_uint*)RETURN = ARRAY(o)->cap;
}

INSTR(Array_Append) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + instr->m_val);
  M_Object o = *(M_Object*)REG(0);
  m_vector_add(ARRAY(o), REG(SZ_INT));
  release(o, shred);
  *(M_Object*)REG(0) = o;
  PUSH_REG(shred, SZ_INT);
}

ANN static Type get_array_type(Type t) {
  while(t->d.base_type)
    t = t->d.base_type;
  return t;
}

static OP_CHECK(opck_array_at) {
  Exp_Binary* bin = (Exp_Binary*)data;
  Type l = get_array_type(bin->lhs->type);
  Type r = get_array_type(bin->rhs->type);

  if(isa(l, r) < 0) {
    REM_REF(bin->lhs->type)
    err_msg(TYPE_, bin->pos, "array types do not match.");
    return &t_null;
  }
  if(bin->lhs->type->array_depth != bin->rhs->type->array_depth) {
    REM_REF(bin->lhs->type)
    err_msg(TYPE_, bin->pos, "array depths do not match.");
    return &t_null;
  }
  bin->rhs->emit_var = 1;
  return bin->rhs->type;
}

static OP_CHECK(opck_array_shift) {
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

static OP_EMIT(opem_array_shift) {
  Exp_Binary* bin = (Exp_Binary*)data;
  Type type = bin->rhs->type;
  Instr instr = emitter_add_instr(emit, Array_Append);
  instr->m_val = type->size;
  return 1;
}

static OP_CHECK(opck_array_cast) {
  Exp_Cast* cast = (Exp_Cast*)data;
  Type l = cast->exp->type;
  Type r = cast->self->type;
  if(!l->d.base_type) l = l->parent;
  if(!r->d.base_type) l = r->parent;
  if(l->array_depth == r->array_depth || isa(l->d.base_type, r->d.base_type) > 0)
    return l;
  return &t_null;
}

m_bool import_array(const Gwi gwi) {
  SET_FLAG((&t_array), ae_flag_abstract);
  CHECK_BB(gwi_class_ini(gwi,  &t_array, NULL, array_dtor))

	gwi_item_ini(gwi, "int", "@array");
  o_object_array = gwi_item_end(gwi, ae_flag_member, NULL);
  CHECK_BB(o_object_array)

  gwi_func_ini(gwi, "int", "size", vm_vector_size);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "depth", vm_vector_depth);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "cap", vm_vector_cap);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "remove", vm_vector_rem);
  gwi_func_arg(gwi, "int", "index");
  CHECK_BB(gwi_func_end(gwi, 0))

  CHECK_BB(gwi_class_end(gwi))
  CHECK_BB(gwi_oper_ini(gwi, "Array", (m_str)OP_ANY_TYPE, NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_array_at))
  CHECK_BB(gwi_oper_end(gwi, op_at_chuck, Assign_Object))
  CHECK_BB(gwi_oper_add(gwi, opck_array_shift))
  CHECK_BB(gwi_oper_emi(gwi, opem_array_shift))
  CHECK_BB(gwi_oper_end(gwi, op_shift_left, Array_Append))
  CHECK_BB(gwi_oper_ini(gwi, "Array", "Array", NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_array_cast))
  CHECK_BB(gwi_oper_emi(gwi, opem_basic_cast))
  CHECK_BB(gwi_oper_end(gwi, op_dollar, NULL))
  return 1;
}

INSTR(Instr_Pre_Ctor_Array_Top) { GWDEBUG_EXE
  if(*(m_uint*)REG(-SZ_INT * 2) >= *(m_uint*)REG(-SZ_INT))
    shred->next_pc = instr->m_val;
  else
    instantiate_object(vm, shred, *(Type*)instr->ptr);
}

INSTR(Instr_Pre_Ctor_Array_Bottom) { GWDEBUG_EXE
  POP_REG(shred,  SZ_INT);
  M_Object obj = *(M_Object*)REG(0);
  m_uint * array = *(m_uint**)REG(-SZ_INT * 3);
  m_int i = *(m_int*)REG(-SZ_INT * 2);
  *(m_uint*)array[i] = (m_uint)obj;
  (*(m_int*)REG(-SZ_INT * 2))++;
  shred->next_pc = instr->m_val;
}

INSTR(Instr_Pre_Ctor_Array_Post) { GWDEBUG_EXE
  POP_REG(shred,  SZ_INT * 3);
  m_uint* array = *(m_uint**)REG(0);
  if(array && array != (m_uint*)1) // shit happens (array pluin extend related)
    free(array);
}

struct ArrayAllocInfo {
  m_int capacity;
  const m_int top;
  Type type, base;
  m_uint* objs;
  m_int* index;
  m_bool is_obj;
};

static M_Object do_alloc_array_object(struct ArrayAllocInfo* info, m_int cap) {
  M_Object base;
  if(cap < 0) {
    gw_err("[gwion](VM): NegativeArraySize: while allocating arrays...\n");
    return NULL;
  }
  base = new_M_Array(info->type, info->capacity >= info->top ?
      info->base->size : SZ_INT, cap, -info->capacity);
  if(!base) {
    gw_err("[gwion](VM): OutOfMemory: while allocating arrays...\n");
    return NULL;
  }
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
      info->base, info->objs, info->index, info->is_obj };
    M_Object next = do_alloc_array(shred, &aai);
    if(!next) {
      release(base, shred);
      return NULL;
    }
    m_vector_set(ARRAY(base), i, &next);
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

INSTR(Instr_Array_Init) { GWDEBUG_EXE // for litteral array
  m_uint i;
  VM_Array_Info* info = *(VM_Array_Info**)instr->ptr;
  M_Object obj;
  POP_REG(shred, instr->m_val2 * info->length);
  obj = new_M_Array(info->type, info->base->size, info->length, info->depth);
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

INSTR(Instr_Array_Alloc) { GWDEBUG_EXE
  VM_Array_Info* info = *(VM_Array_Info**)instr->ptr;
  M_Object ref;
  m_uint num_obj = 0;
  m_int index = 0;
  struct ArrayAllocInfo aai = { -info->depth, -1, info->type, info->base,
         NULL, &index, info->is_obj};
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
  gw_err("[Gwion](VM): OutOfMemory: while allocating arrays...\n"); // LCOV_EXCL_LINE
error:
  gw_err("[Gwion](VM): (note: in shred[id=%" UINT_F ":%s])\n", shred->xid, shred->name);
  vm_shred_exit(shred);
}

static void array_push(VM_Shred shred, M_Vector a, m_uint i, m_uint size, m_bool emit_var) {
  if(emit_var)
      *(char**)REG(0) = m_vector_addr(a, i);
  else
    m_vector_get(a, i, REG(0));
  PUSH_REG(shred,  size);
}

static void oob(M_Object obj, VM_Shred shred, m_int i) {
  gw_err("[Gwion](VM): ArrayOutofBounds: in shred[id=%" UINT_F ":%s], PC=[%" UINT_F "], index=[%" UINT_F "]\n",
         shred->xid, shred->name, shred->pc, i);
  release(obj, shred);
  vm_shred_exit(shred);
}

#define OOB(shred, obj, i)  if(i < 0 || i >=  ARRAY(obj)->len) { \
  oob(obj, shred, i); return; }

INSTR(Instr_Array_Access) { GWDEBUG_EXE
  m_int i = 0;
  M_Object obj;
  POP_REG(shred,  SZ_INT * 2);
  if(!(obj = *(M_Object*)REG(0)))
    Except(shred, "NullPtrException");
  i = *(m_int*)REG(SZ_INT);
  OOB(shred, obj, i)
  array_push(shred, ARRAY(obj), i, instr->m_val2, instr->m_val);
}

INSTR(Instr_Array_Access_Multi) { GWDEBUG_EXE
  m_int i, j;
  POP_REG(shred,  SZ_INT * (instr->m_val + 1));
  M_Object obj, *base = (M_Object*)REG(0);
  if(!(obj = *base))
    Except(shred, "NullPtrException");
  for(j = 0; j < instr->m_val - 1; j++) {
    i = *(m_int*)REG(SZ_INT * (j + 1));
    OOB(shred, *base, *(m_int*)REG(SZ_INT * (j + 1)))
    m_vector_get(ARRAY(obj), i, &obj);
    if(!obj) {
      release(*base, shred);
      Except(shred, "NullPtrException");
    }
  }
  i = *(m_int*)REG(SZ_INT * (j + 1));
  OOB(shred, *base,*(m_int*)REG(SZ_INT * (j + 1)))
  array_push(shred, ARRAY(obj), i, instr->m_val2, *(m_uint*)instr->ptr);
}
