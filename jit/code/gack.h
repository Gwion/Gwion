#include "jit/jit.h"
#include "gwjit_common.h"
#include "cc.h"
#include "q.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"
/*
JIT_CODE(Gack) {
  CJval reg = push_reg(j, -instr->m_val);
  m_uint offset = 0;
  const Vector v = *(Vector*)instr->ptr;
  const m_uint size = instr->m_val2;
  for(m_uint i = size + 1; --i;) {
    const Type type = (Type)vector_at(v, size - i);
    if(isa(type, t_object) > 0) {
      CJval obj = JLOADR(reg, offset, void_ptr);
      CJval t   = JCONST(void_ptr, type);
      CJval arg[] = { t, obj };
      CALL_NATIVE(print_object, "vpp", arg);
    } else if(isa(type, t_function) > 0) {
      CJval obj = JADDR(reg, offset);
      CJval t   = JCONST(void_ptr, type);
      CJval arg[] = { t, obj };
      CALL_NATIVE(print_func, "vpp", arg);
    } else if(isa(type, t_class) > 0) {
      CJval t   = JCONST(void_ptr, type->d.base_type);
      CJval arg[] = { t };
      CALL_NATIVE(print_type, "vp", arg);
    } else if(isa(type, t_void) > 0) {
      CJval str = JCONST(void_ptr, "void");
      CJval arg[] = { str };
      CALL_NATIVE(print_string1, "vp", arg);
    } else {
      CJval t   = JCONST(void_ptr, type);
      CJval ptr = JADDR(reg, offset);
      CJval arg[] = { t, ptr };
      CALL_NATIVE(print_prim, "vpp", arg);
    }
    CJval clear = JCONST(void_ptr, " "CLEAR);
    CJval earg[] = { clear };
    CALL_NATIVE(gw_out, "vp", earg)
    offset += type->size;
  }
  if(instr->m_val2 == 1) {
    CJval colon = JCONST(void_ptr, " :");
    CJval carg[] = { colon };
    CALL_NATIVE(gw_out, "vp", carg)
    CJval type = JCONST(void_ptr, vector_front(*(Vector*)instr->ptr));
    CJval parg[] = { type };
    CALL_NATIVE(print_type, "vp", parg)
  }
  CJval clear = JCONST(void_ptr, CLEAR"\n");
  CJval earg[] = { clear };
  CALL_NATIVE(gw_out, "vp", earg)
}
*/
