ANN Jval cc_get_flag(CC const, CJval , const ae_flag);
ANN void cc_release(CC const , CJval);
ANN void cc_release2(CC const , CJval);
ANN void cc_shredule(CC const, CJval);
ANN void cc_remove(CC const, const m_bool);
ANN void cc_except(CC const, const m_str);
ANN void cc_check(CC const, CJval, const m_str);
ANN Jval push_reg(CC const j, const m_int i);
ANN void push_reg2(CC const j, CJval i);
ANN void next_pc(CC const j, const m_uint i);
void cc_add2gc(CC, CJval);
#include "type.h"
ANN static inline void type_add_ref(Type t) { ++t->obj.ref_count; }


ANN void jit_vector_add(CC, CJval, CJval);
ANN void jit_type_add_ref(CC cc, CJval type);

ANN void cc_free(void*);
Jval jit_mp_alloc(CC cc, const m_uint size);
jit_function_t get_jit_func(CC cc, const m_str s);
//ANN Jval cc_inc(CC cc, Jval val);
//ANN Jval cc_dec(CC cc, Jval val);
