#ifndef __SHREDULER
#define __SHREDULER
struct Shreduler_ {
  struct BBQ_ *      bbq;
  struct ShredTick_ *list;
  struct ShredTick_ *curr;
  struct Vector_     active_shreds;
  MUTEX_TYPE         mutex;
  size_t             shred_ids;
  bool               loop;
};

ANN Shreduler new_shreduler(const MemPool mp);
ANN void free_shreduler(const MemPool mp, const Shreduler s);
#endif
