ANN Jval cc_get_flag(JitCC*const, CJval , const ae_flag);
ANN void cc_release(JitCC *const , CJval);
ANN void cc_shredule(JitCC *const, CJval);
ANN void cc_remove(JitCC *const, const m_bool);
ANN void cc_except(JitCC *const, const m_str);
ANN Jval push_reg(JitCC* const j, const m_uint i);
ANN void next_pc(JitCC* const j, const m_uint i);
m_str safe_string(const M_Object);
