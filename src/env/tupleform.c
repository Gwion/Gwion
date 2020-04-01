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
#include "tuple.h"
#include "array.h"

ANN void tuple_info(const Env env, Type_Decl *base, const Var_Decl var) {
  const Value v = var->value;
  const m_uint offset = vector_back(&env->class_def->e->tuple->offset);
  vector_add(&env->class_def->e->tuple->types, (vtype)v->type);
  vector_add(&env->class_def->e->tuple->offset, offset + v->type->size);
  Type_Decl *td = cpy_type_decl(env->gwion->mp, base);
  if(var->array)
    td->array = cpy_array_sub(env->gwion->mp, var->array);
  if(env->class_def->e->tuple->list) {
    Type_List tl = env->class_def->e->tuple->list;
    while(tl->next)
      tl = tl->next;
    tl->next = new_type_list(env->gwion->mp, td, NULL);
  } else
    env->class_def->e->tuple->list = new_type_list(env->gwion->mp, td, NULL);
}

ANN TupleForm new_tupleform(MemPool p) {
  TupleForm tuple = mp_malloc(p, TupleForm);
  vector_init(&tuple->types);
  vector_init(&tuple->offset);
  vector_add(&tuple->offset, 0);
  tuple->list = NULL;
  return tuple;
}

ANN void free_tupleform(MemPool p, const TupleForm tuple) {
  vector_release(&tuple->types);
  vector_release(&tuple->offset);
  if(tuple->list)
    free_type_list(p, tuple->list);
}
