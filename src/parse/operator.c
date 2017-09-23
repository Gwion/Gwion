#include "err_msg.h"
#include "type.h"
#include "func.h"
#include "instr.h"

typedef Type (*f_type)(Env env, Exp exp);

typedef struct {
  Type lhs, rhs, ret;
  f_instr instr;
  Func func;
} M_Operator;

static void free_op(M_Operator* a) {
  if(a->func && a->func->def) {
//    REM_REF(a->func);
    REM_REF(a->func->value_ref->m_type);
    REM_REF(a->func->value_ref);
    REM_REF(a->func);
//    free_func_def(a->func->def);
  }
  if(a->lhs)
    REM_REF(a->lhs)
    if(a->rhs)
      REM_REF(a->rhs)
      REM_REF(a->ret)
      free(a);
}
void free_op_map(Map map) {
  m_uint i;
  Vector v;
  for(i = map_size(map) + 1; --i;) {
    m_uint j;
    v = (Vector)map_at(map, (vtype)i - 1);
    for(j = vector_size(v) + 1; --j;)
      free_op((M_Operator*)vector_at(v, j - 1));
    free_vector(v);
  }
  map_release(map);
}

static m_bool op_match(Type t, Type mo) {
  if((t && mo && mo->xid == t->xid) || (!t && !mo))
    return 1;
  return 0;
}

static M_Operator* operator_find(Vector v, Type lhs, Type rhs) {
  m_uint i;
  for(i = vector_size(v) + 1; --i;) {
    M_Operator* mo = (M_Operator*)vector_at(v, i - 1);
    if(op_match(lhs, mo->lhs) && op_match(rhs, mo->rhs))
      return mo;
  }
  return NULL;
}

m_bool env_add_op(Env env, struct Op_Import* opi) {
  Nspc nspc = env->curr;
  Vector v;
  M_Operator* mo;

  if(opi->global)
    nspc = env->global_nspc;

  if(!nspc->op_map.ptr)
    map_init(&nspc->op_map);

  if(!(v = (Vector)map_get(&nspc->op_map, (vtype)opi->op))) {
    if(!op2str(opi->op))
      CHECK_BB(err_msg(TYPE_, 0, "failed to import operator '%s', for type '%s' and '%s'. reason: no such operator",
            op2str(opi->op), opi->lhs ? opi->lhs->name : NULL,
            opi->rhs ? opi->rhs->name : NULL))
      v = new_vector();
    map_set(&nspc->op_map, (vtype)opi->op, (vtype)v);
  }
  if((mo = operator_find(v, opi->lhs, opi->rhs))) {
    if((err_msg(TYPE_, 0, "operator '%s', for type '%s' and '%s' already imported",
            op2str(opi->op), opi->lhs ? opi->lhs->name : NULL,
            opi->rhs ? opi->rhs->name : NULL) < 0))
       return -1;
  }
  mo = malloc(sizeof(M_Operator));
  mo->lhs       = opi->lhs;
  mo->rhs       = opi->rhs;
  mo->ret       = opi->ret;
  mo->instr     = opi->f;
  mo->func      = NULL;
  vector_add(v, (vtype)mo);
  if(opi->lhs)
    ADD_REF(opi->lhs)
    if(opi->rhs)
      ADD_REF(opi->rhs)
      ADD_REF(opi->ret)
      return 1;
}

static Type get_return_type_inner(Map map, struct Op_Import* opi) {
  Type r = opi->rhs;
  do {
    M_Operator* mo;
    Vector v = (Vector)map_get(map, (vtype)opi->op);
    if((mo = operator_find(v, opi->lhs, r)))
      return mo->ret;
  } while(r && (r = r->parent));
  return NULL;
}

Type get_return_type(Env env, struct Op_Import* opi) {
  Nspc nspc = env->curr;

  while(nspc) {
    if(nspc->op_map.ptr) {
      Type l = opi->lhs;
      do {
        struct Op_Import opi2 = { opi->op, l, opi->rhs, NULL, NULL, NULL, 0 };
        Type ret = get_return_type_inner(&nspc->op_map, &opi2);
        if(ret)
          return ret;
      } while(l && (l = l->parent));
    }
    nspc = nspc->parent;
  }
  return NULL;
}

m_bool operator_set_func(Env env, Func f, Type lhs, Type rhs) {
  Nspc nspc = env->curr;
  M_Operator* mo;
  Vector v = (Vector)map_get(&nspc->op_map, (vtype)name2op(s_name(f->def->name)));
  mo = operator_find(v, lhs, rhs);
  mo->func = f;
  return 1;
}

static Instr handle_instr(Emitter emit, M_Operator* mo) {
  if(mo->func) {
    Instr instr = emitter_add_instr(emit, Reg_Push_Imm); //do we need to set offset ?
    CHECK_BO(emit_exp_call1(emit, mo->func, mo->func->def->ret_type, 0))
    return instr;
  }
  if(mo->instr)
    return emitter_add_instr(emit, mo->instr);
  CHECK_BO(err_msg(EMIT_, 0, "Trying to call non emitted operator."))
  return NULL;
}
Instr get_instr(Emitter emit, struct Op_Import* opi) {
  Nspc nspc = emit->env->curr;

  while(nspc) {
    Type l = opi->lhs;
    do {
      Type r = opi->rhs;
      do {
        M_Operator* mo;
        Vector v;
        if(!nspc->op_map.ptr)
          continue;
        v = (Vector)map_get(&nspc->op_map, (vtype)opi->op);
        if((mo = operator_find(v, l, r)))
          return  handle_instr(emit, mo);
      } while(r && (r = r->parent));
    } while(l && (l = l->parent));
    nspc = nspc->parent;
  }
  return NULL;
}
