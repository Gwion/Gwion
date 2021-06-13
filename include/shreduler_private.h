#ifndef __SHREDULER
#define __SHREDULER
struct Shreduler_ {
  struct BBQ_ *      bbq;
  struct ShredTick_ *list;
  struct ShredTick_ *curr;
  struct Vector_     shreds;
  MUTEX_TYPE         mutex;
  size_t             shred_ids;
  bool               loop;
};
#endif
