struct  Shreduler_ {
  VM* vm;
  VM_Shred list;
  VM_Shred curr;
  m_bool loop;
};

