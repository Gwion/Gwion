#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "emit.h"
#include "vm.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "traverse.h"
#include "object.h"
#include "parse.h"
#include "array.h"

ANN void tuple_info(const Env env, Type_Decl *base, const Var_Decl var) {
  const Value v = var->value;
  const m_uint offset = vector_back(&env->class_def->e->tuple->offset);
  vector_add(&env->class_def->e->tuple->types, (vtype)v->type);
  vector_add(&env->class_def->e->tuple->offset, offset + v->type->size);
  Type_Decl *td = cpy_type_decl(env->gwion->mp, base);
  if(var->array)
    td->array = cpy_array_sub(env->gwion->mp, var->array);
}

ANN2(1) TupleForm new_tupleform(MemPool p, const Type parent_type) {
  TupleForm tuple = mp_malloc(p, TupleForm);
  vector_init(&tuple->types);
  vector_init(&tuple->offset);
  if(parent_type && parent_type->e->tuple) {
    const TupleForm parent = parent_type->e->tuple;
    const m_uint sz = vector_size(&parent->types);
    tuple->start = parent->start + sz;
    if(sz) {
      const Type last = (Type)vector_back(&parent->types);
      const m_uint offset = vector_back(&parent->offset);
      vector_add(&tuple->offset, offset + last->size);
    } else {
      vector_add(&tuple->offset, 0);
    }
  } else {
    vector_add(&tuple->offset, 0);
    tuple->start = 0;
  }
  return tuple;
}

ANN void free_tupleform(MemPool p, const TupleForm tuple) {
  vector_release(&tuple->types);
  vector_release(&tuple->offset);
}
