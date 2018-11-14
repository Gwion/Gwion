#ifndef __SHREDULER
#define __SHREDULER
struct  Shreduler_ {
  VM* vm;
  VM_Shred list;
  VM_Shred curr;
  m_bool   loop;
};
#endif
