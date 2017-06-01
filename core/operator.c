#include "err_msg.h"
#include "instr.h"
#include "operator_private.h"

m_int name2op(m_str name) {
  m_uint i = 0;
  while(op_name[i] && op_str[i]) {
    if(!strcmp(op_name[i], name)) {
      return operators[i];
    }
    if(!op_name[i + 1]) {
      return -1;
    }
    i++;
  }
  return -1;
}

const m_str op2str(Operator op) {
  if(op >= (sizeof(operators) / sizeof(Operator)))
    return NULL;
  return op_str[op];
}

static Map new_Operator_Map() {
  m_uint i;
  Map map = new_Map();
  for(i = 0; i < (sizeof(operators) / sizeof(Operator)); i++)
    map_set(map, (vtype)operators[i], (vtype)new_vector());
  return map;
}

void free_Operator_Map(Map map) {
  m_uint i;
  Vector v;
  for(i = 0; i < (sizeof(operators) / sizeof(Operator)); i++) {
    m_uint j;
    v = (Vector)map_get(map, (vtype)operators[i]);
    for(j = 0; j < vector_size(v); j++)
      free((M_Operator*)vector_at(v, j));
    free_vector(v);
  }
  free_Map(map);
}

static M_Operator* operator_find(Vector v, Type lhs, Type rhs) {
  m_uint i;
  for(i = 0; i < vector_size(v); i++) {
    M_Operator* mo = (M_Operator*)vector_at(v, i);
    if((lhs && mo->lhs && mo->lhs->xid == lhs->xid || (!lhs && ! mo->lhs)) &&
        ((mo->rhs && mo->rhs->xid == rhs->xid) || (!rhs && !mo->rhs)))
      return mo;
  }
  return NULL;
}

m_bool add_binary_op(Env env, Operator op, Type lhs, Type rhs, Type ret, f_instr f, m_bool global) {
#ifdef DEBUG_OPERATOR
  debug_msg(" op  ", "import operator '%s' for type '%s' and '%s', in  '%s'",
            op2str(op), lhs ? lhs->name : NULL, rhs ? rhs->name : NULL, env->curr->name);
#endif

  NameSpace nspc = env->curr;
  Vector v;
  M_Operator* mo;

  if(global && nspc->parent)
    /*		nspc = nspc->parent;*/
    nspc = env->global_nspc;

if(!nspc->operator)
nspc->operator = new_Operator_Map();

  v = (Vector)map_get(nspc->operator, (vtype)op);

  if(!v) {
    err_msg(TYPE_, 0, "failed to import operator '%s', for type '%s' and '%s'. reason: no such operator",
            op2str(op), lhs ? lhs->name : NULL, rhs ? rhs->name : NULL);
    return -1;
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
  vector_append(v, (vtype)mo);
  return 1;
}

Type get_return_type(Env env, Operator op, Type lhs, Type rhs) {
#ifdef DEBUG_OPERATOR
  debug_msg(" op", "get return type for operator '%s', for type '%s' and '%s'",
            op2str(op), lhs ? lhs->name : NULL, rhs ? rhs->name : NULL);
#endif
  Type t, l = lhs, r = lhs;
  NameSpace nspc = env->curr;
  while(nspc) {
    if(!nspc->operator)
		goto next;
    Vector v = (Vector)map_get(nspc->operator, (vtype)op);
    M_Operator* mo;
    if((mo = operator_find(v, lhs, rhs)))
      return mo->ret;
    l = l ? l->parent : NULL;
    while(l) {
      if((t = get_return_type(env, op, l, rhs)))
        return t;
      r = rhs;
      while(r) {
        if((t = get_return_type(env, op, l, r)))
          return t;
        r = r->parent;
      }
      l = l->parent;
    }
next:
    nspc = nspc->parent;
  }
  nspc = env->curr;
  while(nspc) {
    r = rhs ? rhs->parent : NULL;
    while(r) {
      if((t = get_return_type(env, op, lhs, r)))
        return t;
      l = lhs;
      while(l) {
        if((t = get_return_type(env, op, l, r)))
          return t;
        l = l->parent;
      }
      r = r->parent;
    }
    nspc = nspc->parent;
  }
  return NULL;
}

m_bool operator_set_func(Env env, Func f, Type lhs, Type rhs) {
#ifdef DEBUG_OPERATOR
  debug_msg(" op", 0, "set func'");
#endif
  NameSpace nspc = env->curr;
  M_Operator* mo;
  Vector v = (Vector)map_get(nspc->operator, (vtype)name2op(S_name(f->def->name)));
  mo = operator_find(v, lhs, rhs);
  mo->func = f;
  return 1;
}

m_bool get_instr(Emitter emit, Operator op, Type lhs, Type rhs) {
#ifdef DEBUG_OPERATOR
  debug_msg(" op", "get instr for operator '%s', for type '%s' and '%s'",
            op2str(op), lhs->name, rhs->name);
#endif
  Type l = lhs, r = rhs;
  Vector v;
  NameSpace nspc = emit->env->curr;
  while(nspc) {
    M_Operator* mo;
    v = (Vector)map_get(nspc->operator, (vtype)op);
    if((mo = operator_find(v, lhs, rhs))) {
      if(mo->func) {
        add_instr(emit, Reg_Push_Imm); //do we need to set offset ?
        if(emit_func_call1(emit, mo->func, mo->func->def->ret_type, 0) < 0)
          return -1;
        return 1;
      } else {
        add_instr(emit, mo->instr);
      }
      return 1;
    }
    if(l && l->parent)
      if(get_instr(emit, op, l->parent, rhs) > 0)
        return 1;

    if(r && r->parent)
      if(get_instr(emit, op, lhs, r->parent) > 0)
        return 1;
    nspc = nspc->parent;
  }
  return -1;
}
