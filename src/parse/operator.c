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


ANN static M_Operator* new_mo(MemPool p, const struct Op_Import* opi) {
  M_Operator* mo = mp_calloc(p, M_Operator);
  mo->lhs       = opi->lhs;
  mo->rhs       = opi->rhs;
  mo->ret       = opi->ret;
  mo->instr     = (f_instr)opi->data;
  mo->ck     = opi->ck;
  mo->em     = opi->em;
  return mo;
}

struct OpChecker {
  const Env env;
  const Map map;
  const struct Op_Import* opi;
  m_bool mut;
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
  if(!v || !operator_find(v, ock->opi->lhs, ock->opi->rhs))
    return GW_OK;
  env_err(ock->env, ock->opi->pos, _("operator '%s', for type '%s' and '%s' already imported"),
        s_name(ock->opi->op), ock->opi->lhs ? ock->opi->lhs->name : NULL,
        ock->opi->rhs ? ock->opi->rhs->name : NULL);
  return GW_ERROR;
}

ANN static m_bool op_exist(const struct OpChecker* ock, const Nspc n) {
  return n->info->op_map.ptr ? _op_exist(ock, n) : GW_OK;
}

ANN m_bool add_op(const Gwion gwion, const struct Op_Import* opi) {
  Nspc n = gwion->env->curr;
  do {
    struct OpChecker ock = { gwion->env, &n->info->op_map, opi, 0 };
    CHECK_BB(op_exist(&ock, n))
  } while((n = n->parent));
  if(!gwion->env->curr->info->op_map.ptr)
    map_init(&gwion->env->curr->info->op_map);
  struct OpChecker ock = { gwion->env, &gwion->env->curr->info->op_map, opi, 0 };
  const Vector v = op_vector(gwion->mp, &ock);
  const M_Operator* mo = new_mo(gwion->mp, opi);
  vector_add(v, (vtype)mo);
  return GW_OK;
}

ANN static void set_nspc(struct OpChecker* ock, const Nspc nspc) {
  if(ock->opi->op == insert_symbol(ock->env->gwion->st, "@implicit") ||
     ock->opi->op == insert_symbol(ock->env->gwion->st, "@access") ||
     ock->opi->op == insert_symbol(ock->env->gwion->st, "@repeat")) {
    struct Implicit* imp = (struct Implicit*)ock->opi->data;
    imp->e->nspc = nspc;
    return;
  }
  if(ock->opi->op == insert_symbol(ock->env->gwion->st, "@conditionnal") ||
     ock->opi->op == insert_symbol(ock->env->gwion->st, "@unconditionnal")) {
    ((Exp)ock->opi->data)->nspc = nspc;
    return;
  }
  exp_self((union exp_data*)ock->opi->data)->nspc = nspc;
}

ANN static Type op_check_inner(struct OpChecker* ock) {
  Type t, r = ock->opi->rhs;
  do {
    const M_Operator* mo;
    const Vector v = (Vector)map_get(ock->map, (vtype)ock->opi->op);
    if(v && (mo = operator_find(v, ock->opi->lhs, r))) {
      if((mo->ck && (t = mo->ck(ock->env, (void*)ock->opi->data, &ock->mut))))
        return t;
      else
        return mo->ret;
    }
  } while(r && (r = op_parent(ock->env, r)));
  return NULL;
}

static m_str type_name(const Type t) {
  return t ? t == OP_ANY_TYPE ? "any" : t->name : "";
}

ANN Type op_check(const Env env, struct Op_Import* opi) {
  Nspc nspc = env->curr;
  do {
    if(nspc->info->op_map.ptr) {
      Type l = opi->lhs;
      do {
        struct Op_Import opi2 = { .op=opi->op, .lhs=l, .rhs=opi->rhs, .data=opi->data };
        struct OpChecker ock = { env, &nspc->info->op_map, &opi2, 0 };
        const Type ret = op_check_inner(&ock);
        if(ret) {
          if(ret == env->gwion->type[et_null])
            break;
          if(!ock.mut)
            set_nspc(&ock, nspc);
          return ret;
        }
      } while(l && (l = op_parent(env, l)));
    }
  } while((nspc = nspc->parent));
  if(opi->op == insert_symbol(env->gwion->st, "$") && opi->rhs == opi->lhs)
    return opi->rhs;
  if(opi->op != insert_symbol(env->gwion->st, "@implicit"))
    env_err(env, opi->pos, _("%s %s %s: no match found for operator"),
    type_name(opi->lhs), s_name(opi->op), type_name(opi->rhs));
  return NULL;
}

ANN m_bool operator_set_func(const struct Op_Import* opi) {
  const Nspc nspc = ((Func)opi->data)->value_ref->from->owner;
  const Vector v = (Vector)map_get(&nspc->info->op_map, (vtype)opi->op);
  DECL_OB(M_Operator*, mo, = operator_find(v, opi->lhs, opi->rhs))
  mo->func = (Func)opi->data;
  return GW_OK;
}

ANN static Instr handle_instr(const Emitter emit, const M_Operator* mo) {
  if(mo->func) {
    const Instr push = emit_add_instr(emit, mo->func->code ? RegPushImm : PushStaticCode);
    push->m_val = ((m_uint)mo->func->code ?:(m_uint)mo->func);
    const Instr instr = emit_exp_call1(emit, mo->func);
    if(mo->func->def->base->xid == insert_symbol(emit->gwion->st, "@conditionnal"))
      return emit_add_instr(emit, BranchEqInt);
    if(mo->func->def->base->xid == insert_symbol(emit->gwion->st, "@unconditionnal"))
      return emit_add_instr(emit, BranchNeqInt);
    return instr;
  }
  return emit_add_instr(emit, mo->instr);
}

ANN static Nspc get_nspc(SymTable *st, const struct Op_Import* opi) {
  if(opi->op == insert_symbol(st, "@implicit")) {
    struct Implicit* imp = (struct Implicit*)opi->data;
    return imp->e->nspc;
  }
  if(opi->op == insert_symbol(st, "@conditionnal") ||
     opi->op == insert_symbol(st, "@unconditionnal"))
    return ((Exp)opi->data)->nspc;
  return exp_self((union exp_data*)opi->data)->nspc;
}

ANN static inline Nspc ensure_nspc(SymTable *st, const struct Op_Import* opi) {
  DECL_OO(Nspc, nspc, = get_nspc(st, opi))
  while(!nspc->info->op_map.ptr)
    nspc = nspc->parent;
  return nspc;
}

ANN Instr op_emit(const Emitter emit, const struct Op_Import* opi) {
  DECL_OO(Nspc, nspc, = ensure_nspc(emit->gwion->st, opi))
  Type l = opi->lhs;
  do {
    Type r = opi->rhs;
    do {
      const Vector v = (Vector)map_get(&nspc->info->op_map, (vtype)opi->op);
if(!v)continue;
assert(v);
      const M_Operator* mo = operator_find(v, l, r);
      if(mo) {
        if(mo->em)
          return mo->em(emit, (void*)opi->data);
        return handle_instr(emit, mo);
      }
    } while(r && (r = op_parent(emit->env, r)));
  } while(l && (l = op_parent(emit->env, l)));
  return NULL;
}
