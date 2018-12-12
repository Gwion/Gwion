#ifndef __SHREDULER
#define __SHREDULER
struct  Shreduler_ {
  VM* vm;
  VM_Shred list;
  VM_Shred curr;
  struct Vector_ shreds;
  size_t shred_ids;
  m_bool   loop;
};
#endif
