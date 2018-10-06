#include "jit/jit.h"
#include "gwjit_common.h"
#include "jitter.h"
#include "cc.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"

JIT_CODE(ArrayAppend) {
  push_reg(cc, -instr->m_val);
  CJval o = JLOADR(cc->reg, -SZ_INT, void_ptr);
  cc_check(cc, o, "NullPtrException");
  CJval data = JLOADR(o, JOFF(M_Object, data), void_ptr);
  CJval array = JLOADR(data, 0, void_ptr);
  CJval arg0[] = { array, cc->reg };
  CALL_NATIVE2(m_vector_add, "vpp", arg0);
  cc_release(cc, o);
  JSTORER(cc->reg, -SZ_INT, o);
}

JIT_CODE(ArrayTop) {
  INIT_LABEL(lbl0);
  INIT_LABEL(lbl1);
  CJval idx = JLOADR(cc->reg, -SZ_INT*2, nuint);
  CJval len = JLOADR(cc->reg, -SZ_INT, nuint);
  CJval cond = JINSN(lt, idx, len);
  JINSN(branch_if_not, cond, &lbl0);
  CJval type = JCONST(void_ptr, instr->m_val2);
  Jval arg[] = { cc->shred, type };
  CALL_NATIVE2(instantiate_object, "vpp", arg);
  JINSN(branch, &lbl1);
  JINSN(label, &lbl0);
  next_pc(cc, instr->m_val);
  JINSN(label, &lbl1);
}

JIT_CODE(ArrayBottom) {
  push_reg(cc, -SZ_INT);
  CJval arr = JLOADR(cc->reg, -SZ_INT*3, void_ptr);
  CJval idx = JLOADR(cc->reg, -SZ_INT*2, nuint);
  CJval obj = JLOADR(cc->reg, 0, void_ptr);
  CJval ptr = JLOADE(arr, idx, void_ptr);
  JSTORER(ptr, 0, obj);
  CJval sum  = JINSN(add, idx, JCONST(nuint, 1));
  JSTORER(cc->reg, -SZ_INT*2, sum);
  next_pc(cc, instr->m_val);
}

JIT_CODE(ArrayPost) {
  push_reg(cc, -SZ_INT*3);
  CJval arr = JLOADR(cc->reg, 0, void_ptr);
  CALL_NATIVE2(cc_free, "vp", &arr);
}

ANN void jit_type_add_ref(CC cc, CJval type) {
  CJval ref = JLOADR(type, JOFF(Type, obj), void_ptr);
  CJval _ref = JINSN(add, ref, JCONST(nuint, 1));
  JSTORER(type, JOFF(Type, obj), _ref);
}

void add2gc(const VM_Shred shred , const M_Object obj) {
  vector_add(&shred->gc, (vtype)obj);
}

JIT_CODE(ArrayInit) {
  const m_uint off = instr->m_val * instr->m_val2;
  const Type t = *(Type*)instr->ptr;
  push_reg(cc, -off + SZ_INT);
  CJval type   = JCONST(void_ptr, t);
  CJval size   = JCONST(nuint, instr->m_val2);
  CJval len    = JCONST(nuint, instr->m_val);
  CJval depth  = JCONST(nuint, t->array_depth);
  CJval arg0[] = { type, size, len , depth };
  CJval obj = CALL_NATIVE2(new_array, "ppUUU", arg0);
  JSTORER(cc->reg, -SZ_INT, obj);
  CJval gc = JADDR(cc->shred, JOFF(VM_Shred, gc));
  jit_vector_add(cc, gc, obj);
  CJval data  = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval array = JLOADR(data, 0, void_ptr);
  CJval _ptr = JLOADR(array, JOFF(M_Vector, ptr), void_ptr);
  CJval ptr = JADDR(_ptr, ARRAY_OFFSET);
  CJval data_size = JCONST(nuint, off);
  CJval _reg = JADDR(cc->reg , -SZ_INT);
  JINSN(memcpy, ptr, _reg, data_size);
}

ANN static void jit_init_array(CC cc, m_int curr,  CJval num_obj) {
  while(curr <= TOP) {
    CJval val = JLOADR(cc->reg, curr * SZ_INT, nint);
    CJval mul = JINSN(mul, JLOADR(num_obj, 0, nint), val);
    JSTORER(num_obj, 0, mul);
    ++curr;
  }
}

JIT_CODE(ArrayAlloc) {
  const ArrayInfo* info = *(ArrayInfo**)instr->ptr;
  const m_bool is_obj = info->is_obj && !info->is_ref;
  CJval size = JCONST(nuint, sizeof(struct ArrayInfo_));
  CJval aai = JINSN(alloca, size);
  CJval depth = JCONST(nint, -info->depth);
  JSTORER(aai, __builtin_offsetof(struct ArrayInfo_, depth), depth);
  CJval type = JCONST(void_ptr, info->type);
  JSTORER(aai, __builtin_offsetof(struct ArrayInfo_, type), type);
  CJval base = JCONST(void_ptr, info->base);
  JSTORER(aai, __builtin_offsetof(struct ArrayInfo_, base), base);
  Jval index = jit_value_create(cc->f, jit_type_nint);
  JINSN(store, index, JCONST(nuint, 0));
  CJval aindex = JINSN(address_of, index);
  JSTORER(aai, __builtin_offsetof(struct ArrayInfo_, d), aindex);
  CJval jobj = JCONST(int, info->is_obj);
  JSTORER(aai, __builtin_offsetof(struct ArrayInfo_, is_obj), jobj);

  if(is_obj) {
    Jval num_obj = jit_value_create(cc->f, jit_type_nuint);
    JINSN(store, num_obj, JCONST(nuint, 1));
    CJval anum_obj = JINSN(address_of, num_obj);
    jit_init_array(cc, -info->depth, anum_obj);
    INIT_LABEL(lbl);
    Jval data = JCONST(void_ptr, NULL);
    CJval cond = JINSN(gt, num_obj, JCONST(nint, 0));
    JINSN(branch_if_not, cond, &lbl);
    CJval arg0[] = { num_obj, JCONST(nuint, SZ_INT) };
    data = CALL_NATIVE2(xcalloc, "pUU", arg0);
    JINSN(label, &lbl);
    JSTORER(aai, __builtin_offsetof(struct ArrayInfo_, data), data);
    CJval arg[] = { cc->shred, aai };
    CJval ref = CALL_NATIVE(do_alloc_array, "ppp", arg);
    push_reg(cc, -SZ_INT * (info->depth - 4));
    JSTORER(cc->reg, -SZ_INT*4, ref);
    JSTORER(cc->reg, -SZ_INT*3, data);
    JSTORER(cc->reg, -SZ_INT*2, JCONST(nuint, 0));
    JSTORER(cc->reg, -SZ_INT, num_obj);
  } else {
    JSTORER(aai, __builtin_offsetof(struct ArrayInfo_, data), JCONST(void_ptr, NULL));
    CJval arg[] = { cc->shred, aai };
    CJval ref = CALL_NATIVE(do_alloc_array, "ppp", arg);
    if(info->depth > 1)
      push_reg(cc, -SZ_INT * (info->depth - 1));
    JSTORER(cc->reg, -SZ_INT, ref);
  }
}

static void jit_oob(const CC cc, CJval obj,
  CJval dim, CJval idx, CJval base) {(void)dim;
  CJval zero = JCONST(nint, 0);
  CJval cond = JINSN(lt, idx, zero);
  CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval arr  = JLOADR(data, 0, void_ptr);
  CJval ptr = JLOADR(arr, 0, nuint);
  CJval len = JLOADR(ptr, 0, nuint);
  INIT_LABEL(lbl);
  INIT_LABEL(lbl2);
  JINSN(branch_if, cond, &lbl);
  CJval cond1 = JINSN(ge, idx, len);
  JINSN(branch_if_not, cond1, &lbl2);
  JINSN(label, &lbl);
  cc_release(cc, base);
//CJval arg[] = { JCONST(void_ptr, "idx %li\n"), idx };
//CALL_NATIVE2(printf, "ipI", arg);
  cc_except(cc, "ArrayOutofBounds");
  JINSN(label, &lbl2);
}

ANN static inline void jit_array_push(CC cc, CJval a , CJval idx, const Instr instr) {
  if(instr->m_val) {
    CJval arg[] = { a, idx };
    CJval addr = CALL_NATIVE2(m_vector_addr, "ppU", arg);
    JSTORER(cc->reg, 0, addr);
  } else {
    CJval arg[] = { a, idx, cc->reg };
    CALL_NATIVE2(m_vector_get, "vpUp", arg);
  }
  push_reg(cc, instr->m_val2);
}

JIT_CODE(ArrayAccess) {
  push_reg(cc, -SZ_INT*2);
  CJval obj = JLOADR(cc->reg, 0, void_ptr);
  CJval dim  = JCONST(nuint, 0);
  cc_check(cc, obj, "NullPtrException");
  CJval idx = JLOADR(cc->reg, SZ_INT, nint);
  jit_oob(cc, obj, dim, idx, obj);
  CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval arr  = JLOADR(data, 0, void_ptr);
  jit_array_push(cc, arr, idx, instr);
}

JIT_CODE(ArrayAccessMulti) {
  const m_uint depth = *(m_uint*)instr->ptr;
  push_reg(cc, -SZ_INT * (depth + 1));
  CJval base = JLOADR(cc->reg, 0, void_ptr);
  cc_check(cc, base, "NullPtrException");
  CJval dim = JCONST(nuint, depth);
  Jval  obj = JINSN(dup, base); // ???
  for(m_uint i = 1; i < depth; ++i) {
    CJval idx = JLOADR(cc->reg, SZ_INT * i, nint);
    jit_oob(cc, obj, dim, idx, base);
    CJval _obj = JINSN(address_of, obj);
    CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
    CJval array  = JLOADR(data, 0, void_ptr);
    CJval arg[] = { array, idx, _obj };
    CALL_NATIVE2(m_vector_get, "vpUp", arg);
    cc_check(cc, obj, "NullPtrException");
  }
  CJval idx = JLOADR(cc->reg, SZ_INT * depth, nint);
  jit_oob(cc, obj, dim, idx, base);
  CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval arr  = JLOADR(data, 0, void_ptr);
  jit_array_push(cc, arr, idx, instr);
}

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_array(struct Jit* j) {
  JIT_IMPORT(ArrayInit)
  JIT_IMPORT(ArrayAlloc)
  JIT_IMPORT(ArrayAppend)
  JIT_IMPORT(ArrayTop)
  JIT_IMPORT(ArrayBottom)
  JIT_IMPORT(ArrayPost)
  JIT_IMPORT(ArrayAccess)
  JIT_IMPORT(ArrayAccessMulti)
}
