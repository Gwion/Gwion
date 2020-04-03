#ifndef __TUPLE
#define __TUPLE
typedef struct TupleForm_* TupleForm;
struct TupleForm_ {
  struct Vector_ types;
  struct Vector_ offset;
//  Type_List list;
  m_uint start;
};

ANN Type tuple_type(const Env, const Vector, const loc_t);
ANN void tuple_info(const Env, Type_Decl*, const Var_Decl);
ANN2(1) TupleForm new_tupleform(MemPool p, const Type parent_type);
ANN void free_tupleform(MemPool p, const TupleForm tuple);

#ifdef __INSTR
INSTR(TupleCtor);
INSTR(TupleMember);
#endif
#endif
