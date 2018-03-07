#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "value.h"
#include "func.h"
#include "instr.h"

typedef Type (*f_type)(Env env, Exp exp);

typedef struct {
  Type lhs, rhs, ret;
  f_instr instr;
  Func func;
  Type (*ck)(Env, void*);
  m_bool (*em)(Emitter, void*);
} M_Operator;

static void free_op(M_Operator* a) {
  if(a->lhs && a->lhs != OP_ANY_TYPE)
    REM_REF(a->lhs)
  if(a->rhs && a->rhs != OP_ANY_TYPE)
    REM_REF(a->rhs)
  if(a->ret)
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

static const Type op_parent(const Env env, const Type t) {
  if(GET_FLAG(t, ae_flag_template) && GET_FLAG(t, ae_flag_ref)) {
    m_str post = strstr(t->name, "<");
    char c[strlen(t->name) - strlen(post) + 1];
    memset(c, 0, strlen(t->name) - strlen(post) + 1);
    strncpy(c, t->name, strlen(t->name) - strlen(post));
    return nspc_lookup_type1(env->curr, insert_symbol(c));
  }
  return t->parent;
}

static const m_bool op_match(const Type t, const Type mo) {
  if(t == OP_ANY_TYPE || mo == OP_ANY_TYPE)
    return 1;
  if((t && mo && mo->xid == t->xid) || (!t && !mo))
    return 1;
  return 0;
}

static M_Operator* operator_find(const Vector v, const Type lhs, const Type rhs) {
  m_uint i;
  for(i = vector_size(v) + 1; --i;) {
    M_Operator* mo = (M_Operator*)vector_at(v, i - 1);
    if(op_match(lhs, mo->lhs) && op_match(rhs, mo->rhs))
      return mo;
  }
  return NULL;
}

const m_bool add_op(Nspc nspc, const struct Op_Import* opi) {
  Vector v = (Vector)map_get(&nspc->op_map, (vtype)opi->op);
  M_Operator* mo;
  if(!v) {
    v = new_vector();
    map_set(&nspc->op_map, (vtype)opi->op, (vtype)v);
  }
  if((mo = operator_find(v, opi->lhs, opi->rhs)))
    CHECK_BB((err_msg(TYPE_, 0, "operator '%s', for type '%s' and '%s' already imported",
            op2str(opi->op), opi->lhs ? opi->lhs->name : NULL,
            opi->rhs ? opi->rhs->name : NULL)))
  mo = calloc(1, sizeof(M_Operator));
  mo->lhs       = opi->lhs;
  mo->rhs       = opi->rhs;
  mo->ret       = opi->ret;
  mo->instr     = (f_instr)opi->data;
  mo->ck     = opi->ck;
  mo->em     = opi->em;
  vector_add(v, (vtype)mo);
  if(opi->lhs && opi->lhs != OP_ANY_TYPE)
    ADD_REF(opi->lhs)
  if(opi->rhs && opi->rhs != OP_ANY_TYPE)
    ADD_REF(opi->rhs)
  if(opi->ret)
  ADD_REF(opi->ret)
  return 1;
}

static void set_nspc(struct Op_Import* opi, const Nspc nspc) {
  if(opi->op == op_implicit)return;
  if(opi->op == op_dollar)
    ((Exp_Cast*)opi->data)->nspc = nspc;
  if(opi->lhs) {
    if(opi->rhs)
      ((Exp_Binary*)opi->data)->nspc = nspc;
    else
      ((Exp_Postfix*)opi->data)->nspc = nspc;
  } else
    ((Exp_Unary*)opi->data)->nspc = nspc;
}

static const Type op_check_inner(const Env env, const Map map, const struct Op_Import* opi) {
  Type t, r = opi->rhs;
  do {
    M_Operator* mo;
    Vector v = (Vector)map_get(map, (vtype)opi->op);

    if(v && (mo = operator_find(v, opi->lhs, r))) {
      if((mo->ck && (t = mo->ck(env, (void*)opi->data))))
        return t;
      else
        return mo->ret;
    }
  } while(r && (r = op_parent(env, r)));
  return NULL;
}

static const m_str type_name(const Type t) {
  return t ? t == OP_ANY_TYPE ? "any" : t->name : "";
}

const Type op_check(const Env env, struct Op_Import* opi) {
  Nspc nspc = env->curr;
  while(nspc) {
    if(nspc->op_map.ptr) {
      Type l = opi->lhs;
      do {
        struct Op_Import opi2 = { opi->op, l, opi->rhs, NULL,
          NULL, NULL, opi->data };
        Type ret = op_check_inner(env, &nspc->op_map, &opi2);
        if(ret) {
          if(ret == &t_null)
            break;
          set_nspc(opi, nspc);
          return ret;
        }
      } while(l && (l = op_parent(env, l)));
    }
    nspc = nspc->parent;
  }
  if(opi->op != op_implicit)
  (void)err_msg(TYPE_, 0, "%s %s %s: no match found for operator",
    type_name(opi->lhs), op2str(opi->op), type_name(opi->rhs));
  return NULL;
}

const m_bool operator_set_func(const Env env, const struct Op_Import* opi) {
  Nspc nspc = ((Func)opi->data)->value_ref->owner;
  M_Operator* mo;
  Vector v = (Vector)map_get(&nspc->op_map, opi->op);
  mo = operator_find(v, opi->lhs, opi->rhs);
  mo->func = ((Func)opi->data);
  return 1;
}

static const m_bool handle_instr(Emitter emit, const M_Operator* mo) {
  if(mo->func) {
    Instr instr = emitter_add_instr(emit, Reg_PushImm);
    instr->m_val = SZ_INT;
    CHECK_BB(emit_exp_call1(emit, mo->func, mo->func->def->ret_type))
    return 1;
  }
  if(mo->instr)
    return emitter_add_instr(emit, mo->instr) ? 1 : -1;
  CHECK_BB(err_msg(EMIT_, 0, "Trying to call non emitted operator."))
  return -1;
}

static const Nspc get_nspc(const struct Op_Import* opi) {
  if(opi->op == op_implicit)
    return opi->rhs->owner;
  if(opi->op == op_dollar)
    return ((Exp_Cast*)opi->data)->nspc;
  if(opi->lhs) {
    if(opi->rhs)
      return ((Exp_Binary*)opi->data)->nspc;
    else
      return ((Exp_Postfix*)opi->data)->nspc;
  }
  return ((Exp_Unary*)opi->data)->nspc;
}

const m_bool op_emit(Emitter emit, const struct Op_Import* opi) {
  Nspc nspc = get_nspc(opi);

  Type l = opi->lhs;
  do {
    Type r = opi->rhs;
    do {
      M_Operator* mo;
      Vector v;
      if(!nspc->op_map.ptr)
        continue;
      v = (Vector)map_get(&nspc->op_map, (vtype)opi->op);
      if((mo = operator_find(v, l, r))) {
        if(mo->em)
          return mo->em(emit, (void*)opi->data);
        return  handle_instr(emit, mo);
      }
    } while(r && (r = op_parent(emit->env, r)));
  } while(l && (l = op_parent(emit->env, l)));
  return -1;
}
