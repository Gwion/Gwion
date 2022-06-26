#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "traverse.h"

// fallbacks
#define deep_any(_name, _data, action, ACTION, _test, _t, _op) \
static OP_##ACTION(op##action##_deep_##_t##_any) {             \
  Exp_Binary *bin = data;                                      \
  struct Op_Import opi = {                                     \
    .lhs  = bin->lhs->type,                                    \
    .rhs  = bin->rhs->type,                                    \
    .op   = insert_symbol(_data->gwion->st, #_op),             \
    .data = (m_uint)bin,                                       \
    .pos  = exp_self(bin)->pos                                 \
  };                                                           \
  return op_##_name(_data, &opi);                              \
}

static OP_CHECK(opck_deep_eq_any) {
  Exp_Binary *bin = data;
  bin->op = insert_symbol(env->gwion->st, "==");
  DECL_ON(const Type, t, = check_exp(env, exp_self(bin)));
  return t;
}

static OP_CHECK(opck_deep_ne_any) {
  Exp_Binary *bin = data;
  bin->op = insert_symbol(env->gwion->st, "!=");
  DECL_ON(const Type, t, = check_exp(env, exp_self(bin)));
  return t;
}

deep_any(emit, emit, em, EMIT,  CHECK_BB, eq, ==);
deep_any(emit, emit, em, EMIT,  CHECK_BB, ne, !=);

// get members of a specific type
static void type_get_member(const Gwion gwion, const Type t, const Vector v) {
  const Map m = &t->nspc->info->value->map;
  for(m_uint i = 0; i < map_size(m); i++) {
    const Value value = (Value)map_at(m, i);
    if(!vflag(value, vflag_member)) continue;
    if(is_func(gwion, value->type)) continue;
    vector_add(v, (m_uint)value);
  }
}

// get members of a type, starting from parents
static void compound_get_member(const Env env, const Type t, const Vector v) {
  if(t->info->parent && t->info->parent->nspc)
    compound_get_member(env, t->info->parent, v);
  type_get_member(env->gwion, t, v);
}

ANN static inline void check_deep_equal_exp(const Env env, const Exp e, const Vector v) {
  vector_init(v);
  compound_get_member(env, e->type, v);
  if(tflag(e->type, tflag_struct))
    exp_setvar(e, true);
}

#define MK_DOT(_data, _exp, _value)                           \
  {                                                           \
    .d = {                                                    \
      .exp_dot = {                                            \
        .base = _exp,                                         \
        .xid = insert_symbol(_data->gwion->st, _value->name)  \
      }                                                       \
    },                                                        \
    .type = _value->type,                                     \
    .exp_type = ae_exp_dot,                                   \
    .pos = _exp->pos                                          \
  }

#define MK_BIN(_lhs, _rhs, _bin)                         \
  {                                                      \
    .d = {                                               \
      .exp_binary = {                                    \
        .lhs = &_lhs,                                    \
        .rhs = &_rhs,                                    \
        .op = _bin->op                                   \
      }                                                  \
    },                                                   \
    .exp_type = ae_exp_binary,                           \
    .pos = exp_self(_bin)->pos                           \
  }

static bool deep_check(const Env env, const Exp_Binary *bin,
                       const Vector l, const Vector r) {
  const m_uint lsz = vector_size(l),
               rsz = vector_size(r);
//  if(lsz && rsz >= lsz) {
  if(rsz >= lsz) {
    for(m_uint i = 0; i < lsz; i++) {
      const Value lval = (Value)vector_at(l, i),
                  rval = (Value)vector_at(r, i);
      struct Exp_ lexp = MK_DOT(env, bin->lhs, lval),
                  rexp = MK_DOT(env, bin->rhs, rval),
                  temp = MK_BIN(lexp, rexp, bin);
      if(!check_exp(env, &temp))
        return false;
    }
  } else return false;
  return true;
}

static OP_CHECK(opck_deep_equal) {
  Exp_Binary *const bin = data;
  struct Vector_ l, r;
  check_deep_equal_exp(env, bin->lhs, &l);
  check_deep_equal_exp(env, bin->rhs, &r);
  const bool ret = deep_check(env, bin, &l, &r);
  vector_release(&l);
  vector_release(&r);
  if(ret) return env->gwion->type[et_bool];
  const Symbol op = bin->op;
  bin->op = !strcmp(s_name(bin->op), "?=")
    ? insert_symbol(env->gwion->st, "==") : insert_symbol(env->gwion->st, "!=");
  env_set_error(env,  true);
  const Type ret_type = check_exp(env, exp_self(bin));
  env_set_error(env,  false);
  if(ret_type) return env->gwion->type[et_bool];
  ERR_N(exp_self(bin)->pos, "no deep operation for: {G+/}%s{0} {+}%s{0} {G+/}%s{0}",
      bin->lhs->type->name, s_name(op), bin->rhs->type->name);
}

struct DeepEmit {
  Exp exp;
  Value val;
  Exp tmp;
  struct Vector_ vec;
};

ANN static inline Type deep_type(const Gwion gwion, const Type t) {
  if(!tflag(t, tflag_struct))
    return t;
  return ref_type(gwion, t, t->info->value->from->loc);
}

ANN static void deep_emit_init(const Emitter emit, struct DeepEmit *d, const m_int offset) {
  char name[256];
  sprintf(name, "@%u:%u", d->exp->pos.first.line, d->exp->pos.first.column);
  d->val = new_value(emit->env, deep_type(emit->gwion, d->exp->type), name, d->exp->pos);
  d->tmp = new_prim_id(emit->gwion->mp, insert_symbol(emit->gwion->st, d->val->name), d->exp->pos);
  d->tmp->d.prim.value = d->val;
  d->tmp->type = d->val->type;
  check_deep_equal_exp(emit->env, d->exp, &d->vec);
  const Instr instr = emit_add_instr(emit, Reg2Mem);
  instr->m_val2 = offset;
  d->val->from->offset = instr->m_val = emit_localn(emit, d->val->type);
}

ANN static void deep_emit_release(const Emitter emit, struct DeepEmit *d) {
  free_exp(emit->gwion->mp, d->tmp);
  value_remref(d->val, emit->gwion);
  vector_release(&d->vec);
}

struct DeepEmits {
  struct DeepEmit *lhs;
  struct DeepEmit *rhs;
  struct Vector_   acc;
  Exp_Binary *bin;
};

static void deep_emits_init(const Emitter emit, struct DeepEmits *ds) {
  emit_add_instr(emit, RegMove)->m_val = -SZ_INT;
  deep_emit_init(emit, ds->lhs, -SZ_INT);
  deep_emit_init(emit, ds->rhs, 0);
  vector_init(&ds->acc);
}

ANN static void deep_emits_release(const Emitter emit, struct DeepEmits *ds) {
  deep_emit_release(emit, ds->lhs);
  deep_emit_release(emit, ds->rhs);
  vector_release(&ds->acc);
}

ANN static void emit_deep_fail(const Emitter emit, const Vector v) {
  const m_uint sz = emit_code_size(emit);
  for(m_uint i = 0; i < vector_size(v); i++) {
    const Instr branch = (Instr)vector_at(v, i);
    branch->m_val = sz;
  }
  emit_add_instr(emit, RegSetImm)->m_val2 = -SZ_INT;
}

ANN static bool deep_emit(const Emitter emit, struct DeepEmits *ds) {
  for(m_uint i = 0; i < vector_size(&ds->lhs->vec); i++) {
    const Value lhs = (Value)vector_at(&ds->lhs->vec, i),
                rhs = (Value)vector_at(&ds->rhs->vec, i);
    struct Exp_ lexp = MK_DOT(emit, ds->lhs->tmp, lhs);
    struct Exp_ rexp = MK_DOT(emit, ds->rhs->tmp, rhs);
    struct Exp_ temp = MK_BIN(lexp, rexp, ds->bin);
    temp.type=emit->gwion->type[et_bool];
    if(tflag(lexp.type, tflag_struct))
      exp_setvar(&lexp, true);
    if(tflag(rexp.type, tflag_struct))
      exp_setvar(&rexp, true);
    if(emit_exp(emit, &temp) < 0) return false;
    vector_add(&ds->acc, (m_uint)emit_add_instr(emit, BranchEqInt));
  }
  const Instr jmp = emit_add_instr(emit, Goto);
  emit_deep_fail(emit, &ds->acc);
  jmp->m_val = emit_code_size(emit);
  return true;
}

static OP_EMIT(opem_deep_equal) {
  emit_push_scope(emit);
  Exp_Binary *const bin = data;
  struct DeepEmit  dl = { .exp=bin->lhs };
  struct DeepEmit  dr = { .exp=bin->rhs };
  struct DeepEmits ds = { .lhs=&dl, .rhs=&dr, .bin = bin };
  deep_emits_init(emit, &ds);
  const bool ret  = deep_emit(emit, &ds);
  deep_emits_release(emit, &ds);
  emit_pop_scope(emit);
  return ret ? GW_OK: GW_ERROR;
}

GWION_IMPORT(deep_equal) {

  GWI_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, (m_str)OP_ANY_TYPE, "bool"))

    gwidoc(gwi, "Deep Equality fallback");
    GWI_BB(gwi_oper_add(gwi, opck_deep_eq_any))
    GWI_BB(gwi_oper_emi(gwi, opem_deep_eq_any))
    GWI_BB(gwi_oper_end(gwi, "?=", NULL))

    gwidoc(gwi, "Deep Inequality fallback");
    GWI_BB(gwi_oper_add(gwi, opck_deep_ne_any))
    GWI_BB(gwi_oper_emi(gwi, opem_deep_ne_any))
    GWI_BB(gwi_oper_end(gwi, "<>", NULL))

  GWI_BB(gwi_oper_ini(gwi, "@Compound", "@Compound", "bool"))

    gwidoc(gwi, "Deep Equality");
    GWI_BB(gwi_oper_add(gwi, opck_deep_equal))
    GWI_BB(gwi_oper_emi(gwi, opem_deep_equal))
    GWI_BB(gwi_oper_end(gwi, "?=", NULL))

    gwidoc(gwi, "Deep Inequality");
    GWI_BB(gwi_oper_add(gwi, opck_deep_equal))
    GWI_BB(gwi_oper_emi(gwi, opem_deep_equal))
    GWI_BB(gwi_oper_end(gwi, "<>", NULL))

  return GW_OK;
}
