#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "gwion.h"
#include "type.h"
#include "instr.h"
#include "emit.h"
#include "value.h"
#include "func.h"
#include "nspc.h"
#include "operator.h"

typedef Type (*f_type)(const Env env, const Exp exp);

typedef struct M_Operator_{
  Type lhs, rhs, ret;
  f_instr instr;
  Func func;
  opck ck;
  opem em;
} M_Operator;

ANN void free_op_map(Map map, struct Gwion_ *gwion) {
  LOOP_OPTIM
  for(m_uint i = map_size(map) + 1; --i;) {
    const restrict Vector v = (Vector)map_at(map, (vtype)i - 1);
    LOOP_OPTIM
    for(m_uint j = vector_size(v) + 1; --j;)
      mp_free(gwion->mp, M_Operator, (M_Operator*)vector_at(v, j - 1));
    free_vector(gwion->mp, v);
  }
  map_release(map);
}

ANN static Type op_parent(const Env env, const Type t) {
  if(GET_FLAG(t, template) && GET_FLAG(t, ref)) {
    const Type type = typedef_base(t);
    const m_str post = strstr(type->name, "<");
    size_t len = strlen(type->name) - strlen(post);
    char c[len + 1];
    for(size_t i = 0; i < len; i++)
      c[i] = type->name[i];
    c[len] = 0;
    return nspc_lookup_type1(env->curr, insert_symbol(env->gwion->st, c));
  }
  return t->e->parent;
}

static m_bool op_match(const restrict Type t, const restrict Type mo) {
  if(t == OP_ANY_TYPE || mo == OP_ANY_TYPE)
    return GW_OK;
  if((t && mo && mo->xid == t->xid) || (!t && !mo))
    return GW_OK;
  return 0;
}

ANN2(1) static M_Operator* operator_find(const Vector v, const restrict Type lhs, const restrict Type rhs) {
  for(m_uint i = vector_size(v) + 1; --i;) {
    M_Operator* mo = (M_Operator*)vector_at(v, i - 1);
    if(op_match(lhs, mo->lhs) && op_match(rhs, mo->rhs))
      return mo;
  }
  return NULL;
}

ANN m_bool add_op(const Gwion gwion, const Nspc nspc, const struct Op_Import* opi) {
  M_Operator* mo;
  Nspc n = nspc;
  do {
    if(!n->info->op_map.ptr)
      continue;
    const Vector v = (Vector)map_get(&n->info->op_map, (vtype)opi->op);
    if(v && (mo = operator_find(v, opi->lhs, opi->rhs))) {
      env_err(gwion->env, opi->pos, _("operator '%s', for type '%s' and '%s' already imported"),
            op2str(opi->op), opi->lhs ? opi->lhs->name : NULL,
            opi->rhs ? opi->rhs->name : NULL);
      return GW_ERROR;
    }
  } while((n = n->parent));
  Vector v = (Vector)map_get(&nspc->info->op_map, (vtype)opi->op);
  if(!v) {
    v = new_vector(gwion->mp);
    map_set(&nspc->info->op_map, (vtype)opi->op, (vtype)v);
  }
// new mo
  mo = mp_calloc(gwion->mp, M_Operator);
  mo->lhs       = opi->lhs;
  mo->rhs       = opi->rhs;
  mo->ret       = opi->ret;
  mo->instr     = (f_instr)opi->data;
  mo->ck     = opi->ck;
  mo->em     = opi->em;
  vector_add(v, (vtype)mo);
  return GW_OK;
}

ANN static void set_nspc(struct Op_Import* opi, const Nspc nspc) {
  if(opi->op == op_impl)return;
  if(opi->op == op_cast)
    ((Exp_Cast*)opi->data)->nspc = nspc;
  if(opi->lhs) {
    if(opi->rhs)
      ((Exp_Binary*)opi->data)->nspc = nspc;
    else
      ((Exp_Postfix*)opi->data)->nspc = nspc;
  } else
    ((Exp_Unary*)opi->data)->nspc = nspc;
}

ANN static Type op_check_inner(const Env env, const Map map, struct Op_Import* opi, 
  m_bool* mut) {
  Type t, r = opi->rhs;
  do {
    const M_Operator* mo;
    const Vector v = (Vector)map_get(map, (vtype)opi->op);
    if(v && (mo = operator_find(v, opi->lhs, r))) {
      if((mo->ck && (t = mo->ck(env, (void*)opi->data, mut))))
        return t;
      else
        return mo->ret;
    }
  } while(r && (r = op_parent(env, r)));
  return NULL;
}

static m_str type_name(const Type t) {
  return t ? t == OP_ANY_TYPE ? "any" : t->name : "";
}

ANN Type op_check(const Env env, struct Op_Import* opi) {
  Type ret = NULL;
  Nspc nspc = env->curr;
  do {
    if(nspc->info->op_map.ptr) {
      Type l = opi->lhs;
      do {
        m_bool mut = 0;
        struct Op_Import opi2 = { .op=opi->op, .lhs=l, .rhs=opi->rhs, .data=opi->data };
        ret = op_check_inner(env, &nspc->info->op_map, &opi2, &mut);
        if(ret) {
          if(ret == t_null)
            break;
          if(!mut)
            set_nspc(opi, nspc);
          return ret;
        }
      } while(l && (l = op_parent(env, l)));
    }
    nspc = nspc->parent;
  } while(nspc);
  if(opi->op == op_cast || (ret != t_null && opi->op != op_impl))
    env_err(env, opi->pos, _("%s %s %s: no match found for operator"),
    type_name(opi->lhs), op2str(opi->op), type_name(opi->rhs));
  return NULL;
}

ANN m_bool operator_set_func(const struct Op_Import* opi) {
  const Nspc nspc = ((Func)opi->data)->value_ref->owner;
  const Vector v = (Vector)map_get(&nspc->info->op_map, opi->op);
  DECL_OB(M_Operator*, mo, = operator_find(v, opi->lhs, opi->rhs))
  mo->func = (Func)opi->data;
  return GW_OK;
}

ANN static m_bool handle_instr(const Emitter emit, const M_Operator* mo) {
  if(mo->func) {
    const Instr instr = emit_add_instr(emit, mo->func->code ? RegPushImm : PushStaticCode);
    instr->m_val = ((m_uint)mo->func->code ?:(m_uint)mo->func);
    return emit_exp_call1(emit, mo->func);
  }
  emit_add_instr(emit, mo->instr);
  return GW_OK;
}

ANN static Nspc get_nspc(const struct Op_Import* opi) {
  if(opi->op == op_impl)
    return opi->rhs->e->owner;
  if(opi->op == op_cast)
    return ((Exp_Cast*)opi->data)->nspc;
  if(opi->lhs) {
    if(opi->rhs)
      return ((Exp_Binary*)opi->data)->nspc;
    else
      return ((Exp_Postfix*)opi->data)->nspc;
  }
  return ((Exp_Unary*)opi->data)->nspc;
}

ANN m_bool op_emit(const Emitter emit, const struct Op_Import* opi) {
  Nspc nspc = get_nspc(opi);
  do {
    Type l = opi->lhs;
    do {
      Type r = opi->rhs;
      do {
        if(!nspc->info->op_map.ptr)
          continue;
        const Vector v = (Vector)map_get(&nspc->info->op_map, (vtype)opi->op);
        const M_Operator* mo = operator_find(v, l, r);
        if(mo) {
          if(mo->em)
            return mo->em(emit, (void*)opi->data);
          return handle_instr(emit, mo);
        }
      } while(r && (r = op_parent(emit->env, r)));
    } while(l && (l = op_parent(emit->env, l)));
  } while((nspc = nspc->parent));
// probably deserves err_msg here
  return GW_ERROR;
}
