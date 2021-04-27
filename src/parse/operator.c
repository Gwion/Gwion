#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "emit.h"
#include "operator.h"
#include "object.h"
#include "array.h"

typedef Type (*f_type)(const Env env, const Exp exp);

typedef struct M_Operator_{
  Type lhs, rhs, ret;
  f_instr instr;
  Func func;
  opck ck;
  opem em;
  struct Vector_ effect;
} M_Operator;

ANN void free_op_map(Map map, struct Gwion_ *gwion) {
  LOOP_OPTIM
  for(m_uint i = map_size(map) + 1; --i;) {
    const restrict Vector v = (Vector)map_at(map, (vtype)i - 1);
    LOOP_OPTIM
    for(m_uint j = vector_size(v) + 1; --j;) {
      M_Operator *const mop = (M_Operator*)vector_at(v, j - 1);
      if(mop->effect.ptr)
        vector_release(&mop->effect);
      mp_free(gwion->mp, M_Operator, mop);

    }
    free_vector(gwion->mp, v);
  }
  map_release(map);
}

static m_str type_name(const Type t) {
  return t ? t == OP_ANY_TYPE ? "any" : t->name : "";
}

static m_bool op_match(const restrict Type t, const restrict Type mo) {
  if(t == OP_ANY_TYPE || mo == OP_ANY_TYPE)
    return GW_OK;
  return t == mo;
}

ANN2(1) static M_Operator* operator_find(const Vector v, const restrict Type lhs, const restrict Type rhs) {
  for(m_uint i = vector_size(v) + 1; --i;) {
    M_Operator* mo = (M_Operator*)vector_at(v, i - 1);
    if(!mo)
      continue;
    if(op_match(lhs, mo->lhs) && op_match(rhs, mo->rhs))
      return mo;
  }
  return NULL;
}

ANN2(1) static M_Operator* operator_find2(const Vector v, const restrict Type lhs, const restrict Type rhs) {
  for(m_uint i = vector_size(v) + 1; --i;) {
    M_Operator* mo = (M_Operator*)vector_at(v, i - 1);
    if(mo && lhs == mo->lhs && rhs == mo->rhs)
      return mo;
  }
  return NULL;
}

ANN void operator_suspend(const Nspc n, struct Op_Import *opi) {
  const Vector v = (Vector)map_get(&n->info->op_map, (vtype)opi->op);
  for(m_uint i = vector_size(v) + 1; --i;) {
    M_Operator* mo = (M_Operator*)vector_at(v, i - 1);
    if(opi->lhs == mo->lhs && opi->rhs == mo->rhs) {
      opi->data = (uintptr_t)mo;
      opi->ret = (Type)&VPTR(v, i-1);
      VPTR(v, i-1) = 0;
      break;
    }
  }
}

ANN static M_Operator* new_mo(MemPool p, const struct Op_Import* opi) {
  M_Operator* mo = mp_calloc(p, M_Operator);
  mo->lhs       = opi->lhs;
  mo->rhs       = opi->rhs;
  mo->ret       = opi->ret;
  mo->instr     = (f_instr)opi->data;
  if(opi->func) {
    mo->ck     = opi->func->ck;
    mo->em     = opi->func->em;
    mo->effect.ptr = opi->func->effect.ptr;
  }
  return mo;
}

struct OpChecker {
  const Env env;
  const Map map;
  const struct Op_Import* opi;
  struct Vector_ effect;
};

__attribute__((returns_nonnull))
ANN static Vector op_vector(MemPool p, const struct OpChecker *ock) {
  const Vector exist = (Vector)map_get(ock->map, (vtype)ock->opi->op);
  if(exist)
    return exist;
  const Vector create = new_vector(p);
  map_set(ock->map, (vtype)ock->opi->op, (vtype)create);
  return create;
}

ANN static m_bool _op_exist(const struct OpChecker* ock, const Nspc n) {
  const Vector v = (Vector)map_get(&n->info->op_map, (vtype)ock->opi->op);
  if(!v || !operator_find2(v, ock->opi->lhs, ock->opi->rhs))
    return GW_OK;
  env_err(ock->env, ock->opi->pos, _("operator '%s', for type '%s' and '%s' already imported"),
        s_name(ock->opi->op), type_name(ock->opi->lhs), type_name(ock->opi->rhs));
  return GW_ERROR;
}

ANN static m_bool op_exist(const struct OpChecker* ock, const Nspc n) {
  return n->info->op_map.ptr ? _op_exist(ock, n) : GW_OK;
}

ANN m_bool add_op(const Gwion gwion, const struct Op_Import* opi) {
  Nspc n = gwion->env->curr;
  do {
    struct OpChecker ock = { .env=gwion->env, .map=&n->info->op_map, .opi=opi };
    CHECK_BB(op_exist(&ock, n));
  } while((n = n->parent));
  if(!gwion->env->curr->info->op_map.ptr)
    map_init(&gwion->env->curr->info->op_map);
  struct OpChecker ock = { .env=gwion->env, .map=&gwion->env->curr->info->op_map, .opi=opi };
  const Vector v = op_vector(gwion->mp, &ock);
  const M_Operator* mo = new_mo(gwion->mp, opi);
  vector_add(v, (vtype)mo);
  return GW_OK;
}

ANN static inline Type op_parent(const Env env, const Type t) {
  const size_t depth = t->array_depth;
  return !depth || !array_base(t)->info->parent ?
      t->info->parent :
      array_type(env, array_base(t)->info->parent, depth);
}

ANN static Type op_check_inner(struct OpChecker* ock, const uint i) {
  Type t, r = ock->opi->rhs;
  do {
    const M_Operator* mo;
    const Vector v = (Vector)map_get(ock->map, (vtype)ock->opi->op);
    if(v && (mo = !i ? operator_find2(v, ock->opi->lhs, r) : operator_find(v, ock->opi->lhs, r))) {
      if((mo->ck && (t = mo->ck(ock->env, (void*)ock->opi->data)))) {
        ock->effect.ptr = mo->effect.ptr;
        return t;
      } else
        return mo->ret;
    }
  } while(r && (r = r->info->parent));
  return NULL;
}

ANN Type op_check(const Env env, struct Op_Import* opi) {
for(int i = 0; i < 2; ++i) {
  Nspc nspc = env->curr;
  do {
    if(!nspc->info->op_map.ptr)
      continue;
    Type l = opi->lhs;
    do {
      struct Op_Import opi2 = { .op=opi->op, .lhs=l, .rhs=opi->rhs, .data=opi->data };
      struct OpChecker ock = { .env=env, .map=&nspc->info->op_map, .opi=&opi2 };
      const Type ret = op_check_inner(&ock, i);
      if(ret) {
        if(ret == env->gwion->type[et_error])
          return NULL;
        if(ock.effect.ptr) {
          const Vector base = &ock.effect;
          for(m_uint i = 0; i < vector_size(base); i++)
            env_add_effect(env, (Symbol)vector_at(base, i), opi->pos);
        }
        opi->nspc = nspc;
        return ret;
      }
    } while(l && (l = op_parent(env, l)));
  } while((nspc = nspc->parent));
}
  // this should be an any case
  if(opi->op == insert_symbol(env->gwion->st, "$") && opi->rhs == opi->lhs)
    return opi->rhs;
  if(opi->op == insert_symbol(env->gwion->st, "@func_check"))
    return NULL;
  if(opi->op == insert_symbol(env->gwion->st, "@class_check"))
    return env->gwion->type[et_error];
  if(opi->op != insert_symbol(env->gwion->st, "@implicit"))
    env_err(env, opi->pos, _("%s %s %s: no match found for operator"),
    type_name(opi->lhs), s_name(opi->op), type_name(opi->rhs));
  return NULL;
}

ANN m_bool operator_set_func(const struct Op_Import* opi) {
  const Nspc nspc = ((Func)opi->data)->value_ref->from->owner;
  const Vector v = (Vector)map_get(&nspc->info->op_map, (vtype)opi->op);
  DECL_OB(M_Operator*, mo, = operator_find(v, opi->lhs, opi->rhs));
  mo->func = (Func)opi->data;
  return GW_OK;
}

ANN static m_bool handle_instr(const Emitter emit, const M_Operator* mo) {
  if(mo->func) {
    const Instr push = emit_add_instr(emit, mo->func->code ? RegPushImm : SetFunc);
    push->m_val = ((m_uint)mo->func->code ?:(m_uint)mo->func);
    CHECK_BB(emit_exp_call1(emit, mo->func));
    if(mo->func->def->base->xid == insert_symbol(emit->gwion->st, "@conditionnal"))
      emit_add_instr(emit, BranchEqInt);
    else if(mo->func->def->base->xid == insert_symbol(emit->gwion->st, "@unconditionnal"))
      emit_add_instr(emit, BranchNeqInt);
    return GW_OK;
  }
  (void)emit_add_instr(emit, mo->instr);
  return GW_OK;
}

ANN m_bool op_emit(const Emitter emit, const struct Op_Import* opi) {
  for(int i = 0; i < 2; ++i) {
  Nspc nspc = emit->env->curr;
  do {
    if(!nspc->info->op_map.ptr)continue;
    Type l = opi->lhs;
    do {
      Type r = opi->rhs;
      do {
        const Vector v = (Vector)map_get(&nspc->info->op_map, (vtype)opi->op);
        if(!v)
          continue;
        const M_Operator* mo = !i ? operator_find2(v, l, r) :operator_find(v, l, r);
        if(mo) {
          if(mo->em) {
            const m_bool ret = mo->em(emit, (void*)opi->data);
            if(ret)
              return ret;
          } else if(mo->func || mo->instr)
            return handle_instr(emit, mo);
        }
      } while(r && (r = op_parent(emit->env, r)));
    } while(l && (l = op_parent(emit->env, l)));
  } while((nspc = nspc->parent));
  }
  return GW_ERROR;
}
