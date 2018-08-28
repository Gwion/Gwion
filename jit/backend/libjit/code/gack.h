#include "jit/jit.h"
#include "gwjit_common.h"
#include "jitter.h"
#include "cc.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"

JIT_CODE(Gack) {
  const Vector v = *(Vector*)instr->ptr;
  const m_uint size = vector_size(v);
  m_uint offset = instr->m_val;
  for(m_uint i = size + 1; --i;) {
    const Type type = (Type)vector_at(v, size - i);
    if(size == 1) {
      CJval arg[] = { JCONST(void_ptr, type) };
      CALL_NATIVE(print_type, "vp", arg);
    }
    if(isa(type, t_object) > 0) {
      CJval obj = JLOADR(cc->reg, -offset, void_ptr);
      CJval jtype = JCONST(void_ptr, type);
      CJval arg[] = { jtype, obj };
      CALL_NATIVE(print_object, "vpp", arg);
    } else if(isa(type, t_function) > 0) {
      CJval jtype = JCONST(void_ptr, type);
      CJval ptr   = JADDR(cc->reg, -offset);
      CJval arg[] = { jtype, ptr };
      CALL_NATIVE(print_func, "vpp", arg);
    } else if(isa(type, t_class) > 0) {
      CJval arg[] = { JCONST(void_ptr, type->d.base_type) };
      CALL_NATIVE(print_type, "vp", arg);
    } else if(isa(type, t_void) > 0) {
      CJval arg[] = { JCONST(void_ptr, "void") };
      CALL_NATIVE(print_string1, "vp", arg);
    } else {
      CJval jtype = JCONST(void_ptr, type);
      CJval ptr   = JADDR(cc->reg, -offset);
      CJval arg[] = { jtype, ptr };
      CALL_NATIVE(print_prim, "vpp", arg);
    }
    offset -= type->size;
  }
  CJval fmt = JCONST(void_ptr, "\n");
  CJval out_arg[] = { fmt };
  CALL_NATIVE(gw_out, "vp", out_arg);
}

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_gack(struct Jit* j) {
  JIT_IMPORT(Gack)
}
