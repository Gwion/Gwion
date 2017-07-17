struct  Shreduler_ {
  VM* vm;
  VM_Shred list;
//  VM_Shred curr;
  m_uint n_shred;
  m_bool loop;
};
