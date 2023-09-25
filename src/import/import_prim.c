#include <limits.h>
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
#include "array.h"
#include "gack.h"
#include "traverse.h"

static GACK(gack_prim) {
  for(m_uint i = 0; i < t->actual_size; i++) {
    const m_bit *value = VALUE + i;
    for(unsigned char mask = 1U << (CHAR_BIT-1); mask; mask >>= 1)
      INTERP_PRINTF("%c", *(uint8_t*)value & mask ? '1' : '0');
    if(i < t->actual_size - 1)
      INTERP_PRINTF(" ");
  }
}

static INSTR(bit_set) {
  POP_REG(shred, SZ_INT);
  const m_int offset = *(m_int*)REG(0);
  const m_uint byte = offset / CHAR_BIT;
  const m_uint idx = offset % CHAR_BIT;
  uint8_t *val = *(uint8_t**)REG(-SZ_INT);
  val += byte;
  const m_int key = *(m_int*)REG(-SZ_INT*2);
  if(key) *val = ((1 << idx) | (*val));
  else **(m_bit**)REG(-SZ_INT) = ((*val) & (~(1 << (idx))));
  *(m_bit**)REG(-SZ_INT) = REG(0);
}

static INSTR(bit_set_fast) {
  uint8_t *val = *(uint8_t**)REG(-SZ_INT);
  val += instr->m_val;
  const m_uint idx = instr->m_val2;
  const m_int key = *(m_int*)REG(-SZ_INT*2);
  if(key) *val = ((1 << idx) | (*val));
  else **(m_bit**)REG(-SZ_INT) = ((*val) & (~(1 << (idx))));
  *(m_bit**)REG(-SZ_INT) = REG(0);
}

static INSTR(bit_check) {
  const m_int offset = *(m_int*)REG(-SZ_INT);
  if(offset < 0 || offset >= (m_int)instr->m_val)
    handle(shred, "OutOfBoundByteAccess");
}

ANN static bool _bit_get(const uint8_t val, const m_uint idx) {
  const int mask =  1 << idx;
  const int masked_n = val & mask;
  return masked_n >> idx;
}

static INSTR(bit_get) {
  POP_REG(shred, instr->m_val);
  const m_int offset = *(m_int*)REG(0);
  const m_uint byte = offset / CHAR_BIT;
  const uint8_t val = *(uint8_t*)REG(byte - SZ_INT);
  *(m_int*)REG(-SZ_INT) = _bit_get(val, offset % CHAR_BIT);
}

static INSTR(bit_get_fast) {
  const uint8_t val = *(uint8_t*)REG((int16_t)instr->udata.two);
  *(m_int*)REG(-instr->m_val) = _bit_get(val, instr->udata.one);
}

static OP_EMIT(opem_bit_access) {
  struct ArrayAccessInfo *const info = (struct ArrayAccessInfo *)data;
  if(!is_prim_int(info->array.exp)) {
    CHECK_BB(emit_exp(emit, info->array.exp));
    const Instr check = emit_add_instr(emit, bit_check);
    check->m_val = info->array.type->actual_size * CHAR_BIT;
    if(!info->is_var) {
      const Instr instr = emit_add_instr(emit, bit_get);
      instr->m_val = info->array.type->size;
    } else emit_add_instr(emit, bit_set);
  } else {
    const m_uint offset = info->array.exp->d.prim.d.gwint.num;
    if(!info->is_var) {
      const Instr instr = emit_add_instr(emit, bit_get_fast);
      instr->m_val  = info->type->size;
      instr->udata.one  = offset % CHAR_BIT;
      instr->udata.two = (m_int)((offset / CHAR_BIT) - SZ_INT);
    } else {
      const Instr instr = emit_add_instr(emit, bit_set_fast);
      instr->m_val2  = offset % CHAR_BIT;
      instr->m_val = (offset / CHAR_BIT);
    }
  }
  return GW_OK;
}

static OP_EMIT(opem_bit_exp) {
  bool *var = data;
  var[1] = var[0];
  return GW_OK;
}

static OP_CHECK(opck_bit_access) {
  Array_Sub array = data;
  const Exp e = array->exp;
  if(e->next) ERR_N(e->next->pos, "too many expressions for bit access");
  if(is_prim_int(e)) {
    m_int idx = e->d.prim.d.gwint.num;
    if(idx < 0 || idx >= (m_int)array->type->size * CHAR_BIT)
      ERR_N(e->pos, "bit access out of bound");
  }
  return env->gwion->type[et_bool];
}

ANN static void scan_prim_op(const Env env, const Type t){
  struct Op_Func   opfunc = { .ck = opck_bit_access, .em = opem_bit_access };
  struct Op_Import opi    = {
      .op = insert_symbol(env->gwion->st, "[]"),
      .lhs = env->gwion->type[et_int],
      .rhs = t,
      .func = &opfunc
  };
  add_op(env->gwion, &opi);
}

ANN static void scan_prim_op2(const Env env, const Type t){
  struct Op_Func   opfunc = {.em = opem_bit_exp };
  struct Op_Import opi    = {
      .op = insert_symbol(env->gwion->st, "@array_init"),
      .lhs = t,
      .ret = env->gwion->type[et_bool],
      .func = &opfunc
  };
  add_op(env->gwion, &opi);
}

static INSTR(bitset) {
  POP_REG(shred,  SZ_INT);
  memcpy(*(uint8_t**)REG(0), REG(-SZ_INT), instr->m_val);
}

static OP_EMIT(opem_bitset) {
  Exp_Binary *bin = data;
  const Instr instr = emit_add_instr(emit, bitset);
  const Type t = isa(bin->rhs->type, emit->gwion->type[et_int]) > 0
     ? bin->lhs->type
     : bin->rhs->type;
  instr->m_val = t->actual_size;
  return GW_OK;
}

static INSTR(bitcast) {
  memset(REG((m_int)instr->m_val), 0, instr->m_val2);
}

static OP_EMIT(opem_bitcast) {
  Exp_Cast *cast = data;
  const Type t = isa(cast->exp->type, emit->gwion->type[et_int]) > 0
     ? known_type(emit->env, cast->td)
     : cast->exp->type;
  const Instr instr = emit_add_instr(emit, bitcast);
  instr->m_val  = -SZ_INT + t->actual_size;
  instr->m_val2 = SZ_INT - t->actual_size;
  return GW_OK;
}

ANN2(1,2,3,5) static void prim_op(const Env env, const Type t, const m_str op, const opck ck, const opem em){
  struct Op_Func   opfunc = { .ck = ck, .em = em };
  struct Op_Import opi    = {
      .op = insert_symbol(env->gwion->st, op),
      .lhs = env->gwion->type[et_int],
      .rhs = t,
      .ret = t,
      .func = &opfunc
  };
  add_op(env->gwion, &opi);
  opi.lhs = t;
  opi.rhs = opi.ret = env->gwion->type[et_int];
  add_op(env->gwion, &opi);
}

ANN static void prim_implicit(const Env env, const Type t){
  struct Op_Import opi    = {
      .op = insert_symbol(env->gwion->st, "@implicit"),
      .lhs = t,
      .rhs = env->gwion->type[et_int],
      .ret = env->gwion->type[et_int],
  };
  add_op(env->gwion, &opi);
}

ANN Type mk_primitive(const Env env, const m_str name, const m_uint size) {
  m_uint sz = SZ_INT;
  while(sz < size) sz += SZ_INT;
  const Type t = new_type(env->gwion->mp, name, NULL);
  t->size = sz;
  t->actual_size = size;
  set_tflag(t, tflag_primitive);
  scan_prim_op(env, t);
  scan_prim_op2(env, t);
  if(size < SZ_INT) {
    prim_op(env, t, ":=>", opck_rassign, opem_bitset);
    prim_op(env, t, "$", NULL, opem_bitcast);
    prim_implicit(env, t);
  } else if(size == SZ_INT) {
    prim_op(env, t, ":=>", opck_rassign, (opem)dummy_func);
    prim_op(env, t, "$", NULL, (opem)dummy_func);
    prim_implicit(env, t);
  } // else provide function to get slices
  CHECK_BO(mk_gack(env->gwion->mp, t, gack_prim));
  return t;
}

ANN m_bool gwi_primitive(const Gwi gwi, const m_str name, const m_uint size, const ae_flag flag) {
  const Env env = gwi->gwion->env;
  const Prim_Def pdef = new_prim_def(gwi->gwion->mp, insert_symbol(gwi->gwion->st, name), size, gwi->loc, flag);
  if(gwi->gwion->data->cdoc) gwfmt_prim_def(gwi->gwfmt, pdef);
  if(!env->class_def || !tflag(env->class_def, tflag_tmpl)) {
    const m_bool ret = scan0_prim_def(gwi->gwion->env, pdef);
    free_prim_def(gwi->gwion->mp, pdef);
    return ret;
  }
  Section section = MK_SECTION(primitive, prim_def, pdef);
  gwi_body(gwi, &section);
  return GW_OK;
}
