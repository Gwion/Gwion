#include "err_msg.h"
#include "type.h"
#include "func.h"
#include "instr.h"
#include "operator_private.h"

m_int name2op(m_str name) {
  m_uint i = 0;
  while(op_name[i]) {
    if(!strcmp(name, op_name[i]))
      return operators[i];
    i++;
  }
  return -1;
}

const m_str op2str(Operator op) {
  if(op >= (sizeof(operators) / sizeof(Operator)))
    return NULL;
  return op_str[op];
}

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

static M_Operator* operator_find(Vector v, Type lhs, Type rhs) {
  m_uint i;
  for(i = vector_size(v) + 1; --i;) {
    M_Operator* mo = (M_Operator*)vector_at(v, i - 1);
    if(((lhs && mo->lhs && mo->lhs->xid == lhs->xid) || (!lhs && !mo->lhs)) &&
        ((rhs && mo->rhs && mo->rhs->xid == rhs->xid) || (!rhs && !mo->rhs)))
      return mo;
  }
  return NULL;
}

m_bool env_add_op(Env env, Operator op, Type lhs, Type rhs, Type ret, f_instr f, m_bool global) {
  Nspc nspc = env->curr;
  Vector v;
  M_Operator* mo;

  if(global)
    nspc = env->global_nspc;

  if(!nspc->op_map.ptr)
    map_init(&nspc->op_map);

  if(!(v = (Vector)map_get(&nspc->op_map, (vtype)op))) {
    if(!op2str(op))
      CHECK_BB(err_msg(TYPE_, 0, "failed to import operator '%s', for type '%s' and '%s'. reason: no such operator",
                       op2str(op), lhs ? lhs->name : NULL, rhs ? rhs->name : NULL))
      v = new_vector();
    map_set(&nspc->op_map, (vtype)op, (vtype)v);
  }
  if((mo = operator_find(v, lhs, rhs))) {
    err_msg(TYPE_, 0, "operator '%s', for type '%s' and '%s' already imported",
            op2str(op), lhs ? lhs->name : NULL, rhs ? rhs->name : NULL);
    return -1;
  }

  mo = malloc(sizeof(M_Operator));
  mo->lhs       = lhs;
  mo->rhs       = rhs;
  mo->ret       = ret;
  mo->instr     = f;
  mo->func      = NULL;
  vector_add(v, (vtype)mo);
  if(lhs)
    ADD_REF(lhs)
    if(rhs)
      ADD_REF(rhs)
      ADD_REF(ret)
      return 1;
}

Type get_return_type(Env env, Operator op, Type lhs, Type rhs) {
  Nspc nspc = env->curr;

  while(nspc) {
    if(nspc->op_map.ptr) {
      Type l = lhs;
      do {
        Type r = rhs;
        do {
          M_Operator* mo;
          Vector v = (Vector)map_get(&nspc->op_map, (vtype)op);
          if((mo = operator_find(v, l, r)))
            return mo->ret;
        } while(r && (r = r->parent));
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

Instr get_instr(Emitter emit, Operator op, Type lhs, Type rhs) {
  Nspc nspc = emit->env->curr;

  while(nspc) {
    Type l = lhs;
    do {
      Type r = rhs;
      do {
        M_Operator* mo;
        Vector v = (Vector)map_get(&nspc->op_map, (vtype)op);
        if((mo = operator_find(v, l, r))) {
          Instr instr;
          if(mo->func) {
            instr = add_instr(emit, Reg_Push_Imm); //do we need to set offset ?
            CHECK_BO(emit_exp_call1(emit, mo->func, mo->func->def->ret_type, 0))
          } else if(mo->instr) {
            instr = add_instr(emit, mo->instr);
          } else {
            err_msg(EMIT_, 0, "Trying to call non emitted operator.");
            instr = NULL;
          }
          return instr;
        }
      } while(r && (r = r->parent));
    } while(l && (l = l->parent));
    nspc = nspc->parent;
  }
  return NULL;
}
