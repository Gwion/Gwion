ANN Jval cc_get_flag(JitCC*const, CJval , const ae_flag);
ANN void cc_release(JitCC *const , CJval);
ANN void cc_release2(JitCC *const , CJval);
ANN void cc_shredule(JitCC *const, CJval);
ANN void cc_remove(JitCC *const, const m_bool);
ANN void cc_except(JitCC *const, const m_str);
ANN void cc_check(JitCC *const, CJval, const m_str);
ANN Jval push_reg(JitCC* const j, const m_int i);
ANN void push_reg2(JitCC* const j, CJval i);
ANN void next_pc(JitCC* const j, const m_uint i);
void add_to_gc(VM_Shred shred, M_Object obj); // move from array to func
#include "type.h"
ANN static inline void type_add_ref(Type t) { ++t->obj.ref_count; }


ANN void jit_vector_add(CC, CJval, CJval);
ANN void jit_type_add_ref(CC cc, CJval type);

ANN void cc_free(void*);
