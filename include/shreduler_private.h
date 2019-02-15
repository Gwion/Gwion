#ifndef __SHREDULER
#define __SHREDULER
struct  Shreduler_ {
  VM* vm;
  struct ShredTick_ *list;
  struct ShredTick_ *curr;
  struct Vector_ shreds;
  size_t shred_ids;
  m_bool   loop;
};
#endif
