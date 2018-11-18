#ifndef __SHREDULER
#define __SHREDULER
struct  Shreduler_ {
  VM* vm;
  VM_Shred list;
  VM_Shred curr;
  m_uint shred_ids;
  struct Vector_ shreds;
  m_bool   loop;
};
#endif
