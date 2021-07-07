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
#include "import.h"
#include "traverse.h"
#include "clean.h"

typedef Type (*f_type)(const Env env, const Exp exp);

typedef struct M_Operator_ {
  Type           lhs, rhs, ret;
  f_instr        instr;
  Func           func;
  opck           ck;
  opem           em;
  struct Vector_ effect;
} M_Operator;

ANN void free_op_map(Map map, struct Gwion_ *gwion) {
  LOOP_OPTIM
  for (m_uint i = map_size(map) + 1; --i;) {
    const restrict Vector v = (Vector)&VVAL(map, (vtype)i - 1);
    LOOP_OPTIM
    for (m_uint j = vector_size(v) + 1; --j;) {
      M_Operator *const mop = (M_Operator *)vector_at(v, j - 1);
      if (mop->effect.ptr) vector_release(&mop->effect);
      mp_free(gwion->mp, M_Operator, mop);
    }
    vector_release(v);
  }
  map_release(map);
}

ANN void free_op_tmpl(Vector v, struct Gwion_ *gwion) {
  LOOP_OPTIM
  for (m_uint i = vector_size(v) + 1; --i;) {
    const Func_Def fdef = (Func_Def)vector_at(v, i - 1);
    free_func_def(gwion->mp, fdef);
  }
  vector_release(v);
}

static m_str type_name(const Type t) {
  return t ? t == OP_ANY_TYPE ? "any" : t->name : "";
}

static m_bool op_match(const restrict Type t, const restrict Type mo) {
  if (t == OP_ANY_TYPE || mo == OP_ANY_TYPE) return GW_OK;
  return t == mo;
}

ANN2(1)
static M_Operator *operator_find(const Vector v, const restrict Type lhs,
                                 const restrict Type rhs) {
  for (m_uint i = vector_size(v) + 1; --i;) {
    M_Operator *mo = (M_Operator *)vector_at(v, i - 1);
    if (!mo) continue;
    if (op_match(lhs, mo->lhs) && op_match(rhs, mo->rhs)) return mo;
  }
  return NULL;
}

ANN2(1)
static M_Operator *operator_find2(const Vector v, const restrict Type lhs,
                                  const restrict Type rhs) {
  for (m_uint i = vector_size(v) + 1; --i;) {
    M_Operator *mo = (M_Operator *)vector_at(v, i - 1);
    if (mo && lhs == mo->lhs && rhs == mo->rhs) return mo;
  }
  return NULL;
}

ANN void operator_suspend(const Nspc n, struct Op_Import *opi) {
  const m_int  idx = map_index(&n->info->op_map, (vtype)opi->op);
  const Vector v   = (Vector)&VVAL(&n->info->op_map, idx);
  for (m_uint i = vector_size(v) + 1; --i;) {
    M_Operator *mo = (M_Operator *)vector_at(v, i - 1);
    if (opi->lhs == mo->lhs && opi->rhs == mo->rhs) {
      opi->data      = (uintptr_t)mo;
      opi->ret       = (Type)&VPTR(v, i - 1);
      VPTR(v, i - 1) = 0;
      break;
    }
  }
}

ANN static M_Operator *new_mo(MemPool p, const struct Op_Import *opi) {
  M_Operator *mo = mp_calloc(p, M_Operator);
  mo->lhs        = opi->lhs;
  mo->rhs        = opi->rhs;
  mo->ret        = opi->ret;
  mo->instr      = (f_instr)opi->data;
  if (opi->func) {
    mo->ck         = opi->func->ck;
    mo->em         = opi->func->em;
    mo->effect.ptr = opi->func->effect.ptr;
  }
  return mo;
}

struct OpChecker {
  const Env               env;
  const Map               map;
  const struct Op_Import *opi;
  struct Vector_          effect;
};

__attribute__((returns_nonnull)) ANN static Vector
op_vector(const struct OpChecker *ock) {
  const m_int idx = map_index(ock->map, (vtype)ock->opi->op);
  if (idx > -1) return (Vector)&VVAL(ock->map, idx);
  map_set(ock->map, (vtype)ock->opi->op, 0);
  const Vector create = (Vector)&VVAL(ock->map, VLEN(ock->map) - 1);
  vector_init(create);
  return create;
}

ANN static m_bool _op_exist(const struct OpChecker *ock, const Nspc n) {
  const m_int idx = map_index(&n->info->op_map, (vtype)ock->opi->op);
  if (idx == -1 || !operator_find2((Vector)&VVAL(ock->map, idx), ock->opi->lhs,
                                   ock->opi->rhs))
    return GW_OK;
  env_err(ock->env, ock->opi->pos,
          _("operator '%s', for type '%s' and '%s' already imported"),
          s_name(ock->opi->op), type_name(ock->opi->lhs),
          type_name(ock->opi->rhs));
  return GW_ERROR;
}

ANN static m_bool op_exist(const struct OpChecker *ock, const Nspc n) {
  return n->info->op_map.ptr ? _op_exist(ock, n) : GW_OK;
}

ANN m_bool add_op(const Gwion gwion, const struct Op_Import *opi) {
  Nspc n = gwion->env->curr;
  do {
    struct OpChecker ock = {
        .env = gwion->env, .map = &n->info->op_map, .opi = opi};
    CHECK_BB(op_exist(&ock, n));
  } while ((n = n->parent));
  if (!gwion->env->curr->info->op_map.ptr)
    map_init(&gwion->env->curr->info->op_map);
  struct OpChecker ock = {
      .env = gwion->env, .map = &gwion->env->curr->info->op_map, .opi = opi};
  const Vector      v  = op_vector(&ock);
  const M_Operator *mo = new_mo(gwion->mp, opi);
  vector_add(v, (vtype)mo);
  return GW_OK;
}

ANN static inline Type op_parent(const Env env, const Type t) {
  const size_t depth = t->array_depth;
  return !depth || !array_base(t)->info->parent
             ? t->info->parent
             : array_type(env, array_base(t)->info->parent, depth);
}

ANN static Type op_check_inner(const Env env, struct OpChecker *ock,
                               const uint i) {
  Type t, r = ock->opi->rhs;
  do {
    const M_Operator *mo;
    const m_int       idx = map_index(ock->map, (vtype)ock->opi->op);
    if (idx > -1 && (mo = !i ? operator_find2((Vector)&VVAL(ock->map, idx),
                                              ock->opi->lhs, r)
                             : operator_find((Vector)&VVAL(ock->map, idx),
                                             ock->opi->lhs, r))) {
      if ((mo->ck && (t = mo->ck(ock->env, (void *)ock->opi->data)))) {
        ock->effect.ptr = mo->effect.ptr;
        return t;
      } else
        return mo->ret;
    }
  } while (r && (r = op_parent(env, r)));
  return NULL;
}

//! check if type matches for template operator
ANN bool _tmpl_match(const Env env, const Type t, Type_Decl *const td,
                     Specialized_List *slp) {
  const Specialized_List sl = *slp;
  if (sl && !td->next && !td->types && td->xid == sl->xid) {
    *slp = sl->next;
    return true;
  }
  const Type base = known_type(env, td);
  return base ? isa(t, base) > 0 : false;
}

//! check Func_Base matches for template operator
// usage of `is_class` is supicious rn
ANN bool tmpl_match(const Env env, const struct Op_Import *opi,
                    Func_Base *const base) {
  Specialized_List sl  = base->tmpl->list;
  const Arg_List   arg = base->args;
  if (opi->lhs) {
    if (!_tmpl_match(env, opi->lhs, arg->td, &sl)) return false;
    if (fbflag(base, fbflag_postfix)) return !!opi->rhs;
    if (!fbflag(base, fbflag_unary)) {
      if (!opi->rhs) return false;
      if (!_tmpl_match(env, opi->rhs, arg->next->td, &sl)) return false;
    } else if (opi->rhs)
      return false;
  } else {
    if (!fbflag(base, fbflag_unary) ||
        !_tmpl_match(env, opi->rhs, arg->td, &sl))
      return false;
  }
  return true;
}

//! make template operator Type_List
ANN2(1, 2)
static Type_List op_type_list(const Env env, const Type t, const Type_List next,
                              const loc_t loc) {
  Type_Decl *const td0 = type2td(env->gwion, t, loc);
  return new_type_list(env->gwion->mp, td0, next);
}

//! make template operator Func_def
ANN Type op_def(const Env env, struct Op_Import *const opi,
                const Func_Def fdef) {
  const Func_Def tmpl_fdef    = cpy_func_def(env->gwion->mp, fdef);
  tmpl_fdef->base->tmpl->base = 0;
  if (opi->lhs) {
    Type_List next =
        opi->rhs ? op_type_list(env, opi->rhs, NULL, opi->pos) : NULL;
    tmpl_fdef->base->tmpl->call = op_type_list(env, opi->lhs, next, opi->pos);
  } else
    tmpl_fdef->base->tmpl->call = op_type_list(env, opi->rhs, NULL, opi->pos);
  if (traverse_func_def(env, tmpl_fdef) < 0) {
    if (!tmpl_fdef->base->func) func_def_cleaner(env->gwion, tmpl_fdef);
    return NULL;
  }
  return op_check(env, opi);
}

//! find template operator
ANN static Type op_check_tmpl(const Env env, struct Op_Import *opi) {
  Nspc nspc = env->curr;
  do {
    if (!nspc->info->op_tmpl.ptr) continue;
    const Vector v = &nspc->info->op_tmpl;
    for (m_uint i = vector_size(v) + 1; --i;) {
      const Func_Def fdef = (Func_Def)vector_at(v, i - 1);
      if (opi->op != fdef->base->xid) continue;
      if (!tmpl_match(env, opi, fdef->base)) continue;
      return op_def(env, opi, fdef);
    }
  } while ((nspc = nspc->parent));
  return NULL;
}

ANN void* op_get(const Env env, struct Op_Import *opi) {
  for (int i = 0; i < 2; ++i) {
    Nspc nspc = env->curr;
    do {
      Type l = opi->lhs;
      if (!nspc->info->op_map.ptr) continue;
      const Map map = &nspc->info->op_map;
      do {
        Type r = opi->rhs;
        do {
           const m_int idx = map_index(map, (vtype)opi->op);
           if(idx != -1) {
             M_Operator *const mo = !i
                    ? operator_find2((Vector)&VVAL(map, idx), l, r)
                    : operator_find((Vector)&VVAL(map, idx), l, r);
            if (mo)
              return mo;
          }
        } while (r && (r = op_parent(env, r)));
      } while (l && (l = op_parent(env, l)));
    } while ((nspc = nspc->parent));
  }
  return NULL;
}

ANN Type op_check(const Env env, struct Op_Import *opi) {
  for (int i = 0; i < 2; ++i) {
    Nspc nspc = env->curr;
    do {
      if (!nspc->info->op_map.ptr) continue;
      Type l = opi->lhs;
      do {
        struct Op_Import opi2 = {
            .op = opi->op, .lhs = l, .rhs = opi->rhs, .data = opi->data};
        struct OpChecker ock = {
            .env = env, .map = &nspc->info->op_map, .opi = &opi2};
        const Type ret = op_check_inner(env, &ock, i);
        if (ret) {
          if (ret == env->gwion->type[et_error]) return NULL;
          if (ock.effect.ptr) {
            const Vector base = &ock.effect;
            for (m_uint i = 0; i < vector_size(base); i++)
              env_add_effect(env, (Symbol)vector_at(base, i), opi->pos);
          }
          opi->nspc = nspc;
          return ret;
        }
      } while (l && (l = op_parent(env, l)));
    } while ((nspc = nspc->parent));
  }
  const Type try_tmpl = op_check_tmpl(env, opi);
  if (try_tmpl) return try_tmpl;
  // this should be an any case
  if (opi->op == insert_symbol(env->gwion->st, "$") && opi->rhs == opi->lhs)
    return opi->rhs;
  if (opi->op == insert_symbol(env->gwion->st, "@func_check")) return NULL;
  if (opi->op == insert_symbol(env->gwion->st, "@class_check"))
    return env->gwion->type[et_error];
  if (opi->op != insert_symbol(env->gwion->st, "@implicit"))
    env_err(env, opi->pos, _("%s %s %s: no match found for operator"),
            type_name(opi->lhs), s_name(opi->op), type_name(opi->rhs));
  return NULL;
}

ANN m_bool operator_set_func(const struct Op_Import *opi) {
  const Nspc   nspc = ((Func)opi->data)->value_ref->from->owner;
  const m_int  idx  = map_index(&nspc->info->op_map, (vtype)opi->op);
  const Vector v    = (Vector)&VVAL(&nspc->info->op_map, idx);
  DECL_OB(M_Operator *, mo, = operator_find(v, opi->lhs, opi->rhs));
  mo->func = (Func)opi->data;
  return GW_OK;
}

ANN static m_bool handle_instr(const Emitter emit, const M_Operator *mo) {
  if (mo->func) {
    const Instr push =
        emit_add_instr(emit, mo->func->code ? RegPushImm : SetFunc);
    push->m_val = ((m_uint)mo->func->code ?: (m_uint)mo->func);
    CHECK_BB(emit_exp_call1(emit, mo->func, true));
    if (mo->func->def->base->xid ==
        insert_symbol(emit->gwion->st, "@conditional"))
      emit_add_instr(emit, BranchEqInt);
    else if (mo->func->def->base->xid ==
             insert_symbol(emit->gwion->st, "@unconditional"))
      emit_add_instr(emit, BranchNeqInt);
    return GW_OK;
  }
  (void)emit_add_instr(emit, mo->instr);
  return GW_OK;
}

ANN m_bool op_emit(const Emitter emit, const struct Op_Import *opi) {
  for (int i = 0; i < 2; ++i) {
    Nspc nspc = emit->env->curr;
    do {
      if (!nspc->info->op_map.ptr) continue;
      Type l = opi->lhs;
      do {
        Type r = opi->rhs;
        do {
          const m_int idx = map_index(&nspc->info->op_map, (vtype)opi->op);
          if (idx == -1) continue;
          const Vector      v = (Vector)&VVAL(&nspc->info->op_map, idx);
          const M_Operator *mo =
              !i ? operator_find2(v, l, r) : operator_find(v, l, r);
          if (mo) {
            if (mo->em) {
              const m_bool ret = mo->em(emit, (void *)opi->data);
              if (ret) return ret;
            } else if (mo->func || mo->instr)
              return handle_instr(emit, mo);
          }
        } while (r && (r = op_parent(emit->env, r)));
      } while (l && (l = op_parent(emit->env, l)));
    } while ((nspc = nspc->parent));
  }
  return GW_ERROR;
}

#define CONVERT(t) t != from ? t : to
ANN static M_Operator *cpy_mo(MemPool p, M_Operator *const base,
                              const Type from, const Type to) {
  M_Operator *mo = mp_calloc(p, M_Operator);
  mo->lhs        = CONVERT(base->lhs);
  mo->rhs        = CONVERT(base->rhs);
  mo->ret        = CONVERT(base->ret);
  mo->instr      = base->instr;
  mo->func       = base->func;
  mo->ck         = base->ck;
  mo->em         = base->em;
  if (base->effect.ptr) {
    vector_init(&mo->effect);
    vector_copy2(&base->effect, &mo->effect);
  }
  return mo;
}
#undef CONVERT

ANN static inline Map ensure_map(const Nspc nspc) {
  const Map map = &nspc->info->op_map;
  if (!map->ptr) map_init(map);
  return map;
}

ANN static inline Vector ensure_vec(const Map map, const m_uint key) {
  const m_int idx = map_index(map, key);
  if (idx > -1) return (Vector)&VVAL(map, idx);
  map_set(map, key, 0);
  const Vector v = (Vector)&VVAL(map, VLEN(map) - 1);
  vector_init(v);
  return v;
}

ANN static void op_visit(const MemPool mp, const Nspc nspc,
                         const struct Op_Import *opi, const Vector visited) {
  if (vector_find(visited, (m_uint)nspc) != -1) return;
  vector_add(visited, (m_uint)nspc);
  if (nspc->info->op_map.ptr) {
    const Map map      = &nspc->info->op_map;
    const Map base_map = ensure_map(opi->rhs->info->value->from->owner);
    for (m_uint i = 0; i < map_size(map); i++) {
      const Vector v  = (Vector)&VVAL(map, i);
      const m_uint sz = vector_size(v);
      for (m_uint j = 0; j < sz; j++) {
        M_Operator *const mo = (M_Operator *)vector_at(v, j);
        if (opi->lhs == mo->lhs || opi->lhs == mo->rhs || opi->lhs == mo->ret) {
          const M_Operator *tmp    = cpy_mo(mp, mo, opi->lhs, opi->rhs);
          const Vector      target = ensure_vec(base_map, VKEY(map, i));
          vector_add(target, (vtype)tmp);
        }
      }
    }
  }
  if (nspc->parent) op_visit(mp, nspc->parent, opi, visited);
}

ANN void op_cpy(const Env env, const struct Op_Import *opi) {
  struct Vector_ visited;
  vector_init(&visited);
  op_visit(env->gwion->mp, opi->rhs->info->value->from->owner, opi, &visited);
  op_visit(env->gwion->mp, opi->lhs->info->value->from->owner, opi, &visited);
  op_visit(env->gwion->mp, env->curr, opi, &visited);
  vector_release(&visited);
}
