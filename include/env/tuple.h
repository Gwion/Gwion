#ifndef __TUPLE
#define __TUPLE
typedef struct TupleForm_ *TupleForm;
struct TupleForm_ {
  struct Vector_ contains;
  struct Vector_ types;
  struct Vector_ offset;
  m_uint         start;
};

ANN Type tuple_type(const Env, const Vector, const loc_t);
ANN void tuple_contains(const Env, const Value);
ANN void tuple_info(const Env, const Value);
ANN2(1) TupleForm new_tupleform(MemPool p, const Type parent_type);
ANN void free_tupleform(const TupleForm tuple, const struct Gwion_ *gwion);

#ifdef __INSTR
INSTR(TupleCtor);
INSTR(TupleMember);
#endif
#endif
