#ifndef __TUPLE
#define __TUPLE
typedef struct TupleForm_ *TupleForm;
struct TupleForm_ {
  struct Vector_ contains;
  struct Vector_ types;
  struct Vector_ offset;
  uint64_t       start;
};

ANN void tuple_contains(const Env, const Value);
ANN void tuple_info(const Env, const Value);
ANN2(1) TupleForm new_tupleform(MemPool p, const Type parent_type);
ANN void free_tupleform(const TupleForm tuple, const struct Gwion_ *gwion);

#endif
