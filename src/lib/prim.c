#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "emit.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "driver.h"
#include "traverse.h"
#include "parse.h"
#include "specialid.h"
#include "array.h"
#include "gack.h"

#define CHECK_OP(op, check, func) _CHECK_OP(op, check, int_##func)

static inline bool is_prim(const Exp e) { return e->exp_type == ae_exp_primary; }

static inline bool is_prim_int(const Exp e) {
  return (is_prim(e) && e->d.prim.prim_type == ae_prim_num);
}

static inline bool is_prim_float(const Exp e) {
  return (is_prim(e) && e->d.prim.prim_type == ae_prim_float);
}

static inline uint pot(const m_int x) { return (x > 0) && ((x & (x - 1)) == 0); }

#define POWEROF2_OPT(name, OP)                                                 \
  if (is_prim_int(bin->rhs) && pot(bin->rhs->d.prim.d.num)) {                  \
    bin->op                = insert_symbol(#OP);                               \
    bin->rhs->d.prim.d.num = sqrt(bin->rhs->d.prim.d.num);                     \
    return check_exp(env, exp_self(bin));                                      \
  }

#define BINARY_FOLD(ntype, name, TYPE, OP, pre, funcl, funcr, ctype,     \
                    exptype, lmember, rmember, retmember)                      \
  static OP_CHECK(opck_##ntype##_##name) {                                     \
    /*const*/ Exp_Binary *bin = (Exp_Binary *)data;                            \
    const Type            t   = env->gwion->type[TYPE];                        \
    if (!exp_self(bin)->pos.first.line) return t;                              \
    pre if (!funcl(bin->lhs) || !funcr(bin->rhs)) return t;                    \
    const ctype                  num =                                    \
        bin->lhs->d.prim.d.lmember OP bin->rhs->d.prim.d.rmember;              \
    free_exp(env->gwion->mp, bin->lhs);                                        \
    free_exp(env->gwion->mp, bin->rhs);                                        \
    exp_self(bin)->exp_type           = ae_exp_primary;                        \
    exp_self(bin)->d.prim.prim_type   = exptype;                               \
    exp_self(bin)->d.prim.d.retmember = num;                                   \
    return t;                                                                  \
  }

#define BINARY_FOLD_Z(ntype, name, TYPE, OP, pre, funcl, funcr, ctype,   \
                    exptype, lmember, rmember, retmember)                      \
  static OP_CHECK(opck_##ntype##_##name) {                                     \
    /*const*/ Exp_Binary *bin = (Exp_Binary *)data;                            \
    const Type            t   = env->gwion->type[TYPE];                        \
    if (!exp_self(bin)->pos.first.line) return t;                              \
    const bool rconst = funcr(bin->rhs);                                       \
    if(rconst && !bin->rhs->d.prim.d.retmember)                                \
      ERR_N(bin->rhs->pos, _("ZeroDivideException"));                          \
    pre if (!funcl(bin->lhs) || !rconst) return t;                             \
    const ctype                  num =                                    \
        bin->lhs->d.prim.d.lmember OP bin->rhs->d.prim.d.rmember;              \
    free_exp(env->gwion->mp, bin->lhs);                                        \
    free_exp(env->gwion->mp, bin->rhs);                                        \
    exp_self(bin)->exp_type           = ae_exp_primary;                        \
    exp_self(bin)->d.prim.prim_type   = exptype;                               \
    exp_self(bin)->d.prim.d.retmember = num;                                   \
    return t;                                                                  \
  }

#define BINARY_INT_FOLD(name, TYPE, OP, pre)                             \
  BINARY_FOLD(int, name, TYPE, OP, pre, is_prim_int, is_prim_int, m_int, \
              ae_prim_num, num, num, num)

#define BINARY_INT_FOLD_Z(name, TYPE, OP, pre)                             \
  BINARY_FOLD_Z(int, name, TYPE, OP, pre, is_prim_int, is_prim_int, m_int, \
              ae_prim_num, num, num, num)

BINARY_INT_FOLD(add, et_int, +,)
BINARY_INT_FOLD(sub, et_int, -,)
BINARY_INT_FOLD(mul, et_int, *, POWEROF2_OPT(name, <<))
BINARY_INT_FOLD_Z(div, et_int, /, POWEROF2_OPT(name, >>))
BINARY_INT_FOLD_Z(mod, et_int, %,)
BINARY_INT_FOLD(sl, et_int, <<,)
BINARY_INT_FOLD(sr, et_int, >>,)
BINARY_INT_FOLD(sand, et_int, &,)
BINARY_INT_FOLD(sor, et_int, |,)
BINARY_INT_FOLD(xor, et_int, ^,)
BINARY_INT_FOLD(gt, et_bool, >,)
BINARY_INT_FOLD(lt, et_bool, <,)
BINARY_INT_FOLD(ge, et_bool, >=,)
BINARY_INT_FOLD(le, et_bool, <=,)
BINARY_INT_FOLD(and, et_bool, &&,)
BINARY_INT_FOLD(or, et_bool, ||,)
BINARY_INT_FOLD(eq, et_bool, ==,)
BINARY_INT_FOLD(neq, et_bool, !=,)

#define BINARY_OP_EMIT(name, type, member) \
static OP_EMIT(opem_##type##_##name) { \
  Exp_Binary *const bin = (Exp_Binary *)data; \
  if(!is_prim_##type(bin->rhs)) \
    (void)emit_add_instr(emit, type##_##name); \
  else { \
    const Instr instr = (Instr)vector_back(&emit->code->instr); \
    instr->opcode = e##type##_##name##_imm; \
    instr->m_val = bin->rhs->d.prim.d.member; \
  } \
  return GW_OK; \
}

#define BINARY_INT_EMIT(name) BINARY_OP_EMIT(name, int, num)
BINARY_INT_EMIT(add)
BINARY_INT_EMIT(sub)
BINARY_INT_EMIT(mul)
BINARY_INT_EMIT(div)
BINARY_INT_EMIT(mod)
BINARY_INT_EMIT(lt)
BINARY_INT_EMIT(le)
BINARY_INT_EMIT(gt)
BINARY_INT_EMIT(ge)

#define IMPORT_BINARY_INT(name, op)          \
  GWI_BB(gwi_oper_add(gwi, opck_int_##name)) \
  GWI_BB(gwi_oper_emi(gwi, opem_int_##name)) \
  GWI_BB(gwi_oper_end(gwi, #op, NULL))

GWION_IMPORT(int_op) {
  GWI_BB(gwi_oper_ini(gwi, "int", "int", "int"))
  IMPORT_BINARY_INT(add, +)
  IMPORT_BINARY_INT(sub, -)
  IMPORT_BINARY_INT(mul, *)
  IMPORT_BINARY_INT(div, /)
  IMPORT_BINARY_INT(mod, %)
  return GW_OK;
}

static GWION_IMPORT(int_logical) {
  GWI_BB(gwi_oper_ini(gwi, "int", "int", "int"))
  IMPORT_BINARY_INT(gt, >)
  IMPORT_BINARY_INT(ge, >=)
  IMPORT_BINARY_INT(lt, <)
  IMPORT_BINARY_INT(le, <=)
  GWI_BB(gwi_oper_add(gwi, opck_int_sr))
  GWI_BB(gwi_oper_end(gwi, ">>", int_sr))
  GWI_BB(gwi_oper_add(gwi, opck_int_sl))
  GWI_BB(gwi_oper_end(gwi, "<<", int_sl))
  GWI_BB(gwi_oper_add(gwi, opck_int_sand))
  GWI_BB(gwi_oper_end(gwi, "&", int_sand))
  GWI_BB(gwi_oper_add(gwi, opck_int_sor))
  GWI_BB(gwi_oper_end(gwi, "|", int_sor))
  GWI_BB(gwi_oper_add(gwi, opck_int_xor))
  GWI_BB(gwi_oper_end(gwi, "^", int_xor))
  GWI_BB(gwi_oper_ini(gwi, "int", "int", "bool"))
  GWI_BB(gwi_oper_add(gwi, opck_int_and))
  GWI_BB(gwi_oper_end(gwi, "&&", int_and))
  GWI_BB(gwi_oper_add(gwi, opck_int_or))
  GWI_BB(gwi_oper_end(gwi, "||", int_or))
  GWI_BB(gwi_oper_add(gwi, opck_int_eq))
  GWI_BB(gwi_oper_end(gwi, "==", int_eq))
  GWI_BB(gwi_oper_add(gwi, opck_int_neq))
  return gwi_oper_end(gwi, "!=", int_neq);
}

static GWION_IMPORT(int_r) {
  GWI_BB(gwi_oper_ini(gwi, "int", "int", "int"))
  CHECK_OP("=>", rassign, r_assign)
  CHECK_OP("+=>", rassign, r_plus)
  CHECK_OP("-=>", rassign, r_minus)
  CHECK_OP("*=>", rassign, r_mul)
  CHECK_OP("/=>", rassign, r_div)
  CHECK_OP("%=>", rassign, r_modulo)
  CHECK_OP("<<=>", rassign, r_sl)
  CHECK_OP(">>=>", rassign, r_sr)
  CHECK_OP("&=>", rassign, r_sand)
  CHECK_OP("|=>", rassign, r_sor)
  CHECK_OP("^=>", rassign, r_sxor)
  return GW_OK;
}

static INSTR(IntRange) {
  shred->reg -= SZ_INT;
  const m_int    start = *(m_int *)REG(-SZ_INT);
  const m_int    end   = *(m_int *)REG(0);
  const m_int    op    = start < end ? 1 : -1;
  const m_uint   sz    = op > 0 ? end - start : start - end;
  const M_Object array = new_array(shred->info->mp, (Type)instr->m_val, sz);
  for (m_int i = start, j = 0; i != end; i += op, ++j)
    m_vector_set(ARRAY(array), j, &i);
  *(M_Object *)REG(-SZ_INT) = array;
}

static OP_CHECK(opck_int_range) {
  const Exp    exp   = (Exp)data;
  const Range *range = exp->d.prim.d.range;
  const Exp    e     = range->start ?: range->end;
  return array_type(env, e->type, 1);
}

static OP_EMIT(opem_int_range) {
  const Exp   exp   = (Exp)data;
  const Instr instr = emit_add_instr(emit, IntRange);
  instr->m_val      = (m_uint)exp->type;
  return GW_OK;
}

#define UNARY_FOLD(ntype, name, TYPE, OP, func, ctype, exptype, member)        \
  static OP_CHECK(opck_##ntype##_##name) {                                     \
    /*const*/ Exp_Unary *unary = (Exp_Unary *)data;                            \
    const Type           t     = env->gwion->type[TYPE];                       \
    CHECK_NN(opck_unary_meta(env, data));                                      \
    if (!func(unary->exp)) return t;                                           \
    const ctype num                   = OP unary->exp->d.prim.d.member;        \
    exp_self(unary)->exp_type         = ae_exp_primary;                        \
    exp_self(unary)->d.prim.prim_type = exptype;                               \
    exp_self(unary)->d.prim.d.num     = num;                                   \
    return t;                                                                  \
  }
#define UNARY_INT_FOLD(name, TYPE, OP)                                         \
  UNARY_FOLD(int, name, TYPE, OP, is_prim_int, m_int, ae_prim_num, num)
UNARY_INT_FOLD(negate, et_int, -)
UNARY_INT_FOLD(cmp, et_int, ~)
UNARY_INT_FOLD(not, et_bool, !)

static GWION_IMPORT(int_unary) {
  GWI_BB(gwi_oper_ini(gwi, NULL, "int", "int"))
  GWI_BB(gwi_oper_add(gwi, opck_int_negate))
  GWI_BB(gwi_oper_end(gwi, "-", int_negate))
  CHECK_OP("++", unary, pre_inc)
  CHECK_OP("--", unary, pre_dec)
  GWI_BB(gwi_oper_add(gwi, opck_int_cmp))
  GWI_BB(gwi_oper_end(gwi, "~", int_cmp))
  GWI_BB(gwi_oper_ini(gwi, NULL, "int", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_int_range))
  GWI_BB(gwi_oper_emi(gwi, opem_int_range))
  GWI_BB(gwi_oper_end(gwi, "@range", NULL))
  GWI_BB(gwi_oper_ini(gwi, "int", NULL, "int"))
  CHECK_OP("++", post, post_inc)
  GWI_BB(gwi_oper_add(gwi, opck_post))
  GWI_BB(gwi_oper_end(gwi, "--", int_post_dec))
  return GW_OK;
}
static GACK(gack_bool) {
  //  gw_out("%s", *(m_uint*)VALUE ? "true" : "false");
  INTERP_PRINTF("%s", *(m_uint *)VALUE ? "true" : "false");
}

static GWION_IMPORT(int_values) {
  DECL_OB(const Type, t_bool, = gwi_mk_type(gwi, "bool", SZ_INT, "int"));
  GWI_BB(gwi_set_global_type(gwi, t_bool, et_bool))
  GWI_BB(gwi_gack(gwi, t_bool, gack_bool))
  gwi_item_ini(gwi, "bool", "true");
  gwi_item_end(gwi, ae_flag_const, num, 1);
  gwi_item_ini(gwi, "bool", "false");
  gwi_item_end(gwi, ae_flag_const, num, 0);
  GWI_BB(gwi_oper_ini(gwi, NULL, "int", "bool"))
  GWI_BB(gwi_oper_add(gwi, opck_unary_meta))
  GWI_BB(gwi_oper_add(gwi, opck_int_not))
  GWI_BB(gwi_oper_end(gwi, "!", IntNot))
  struct SpecialId_ spid = {
      .type = t_bool, .exec = RegPushMaybe, .is_const = 1};
  gwi_specialid(gwi, "maybe", &spid);
  return GW_OK;
}

static GWION_IMPORT(int) {
  GWI_BB(import_int_values(gwi))
  GWI_BB(gwi_oper_cond(gwi, "int", BranchEqInt, BranchNeqInt))
  GWI_BB(gwi_oper_ini(gwi, "int", "int", "int"))
  GWI_BB(import_int_op(gwi))
  GWI_BB(import_int_logical(gwi))
  GWI_BB(import_int_r(gwi))
  GWI_BB(import_int_unary(gwi))
  return GW_OK;
}

static OP_CHECK(opck_cast_f2i) { return env->gwion->type[et_int]; }

static OP_CHECK(opck_implicit_f2i) { return env->gwion->type[et_error]; }

static OP_CHECK(opck_cast_i2f) { return env->gwion->type[et_float]; }

static OP_CHECK(opck_implicit_i2f) { return env->gwion->type[et_float]; }

#define CHECK_FF(op, check, func) _CHECK_OP(op, check, float_##func)
#define CHECK_IF(op, check, func) _CHECK_OP(op, check, int_float_##func)
#define CHECK_FI(op, check, func) _CHECK_OP(op, check, float_int_##func)

#define BINARY_INT_FLOAT_FOLD(name, TYPE, OP, pre)                       \
  BINARY_FOLD(int_float, name, TYPE, OP, pre, is_prim_int,               \
              is_prim_float, m_float, ae_prim_float, num, fnum, fnum)
#define BINARY_INT_FLOAT_FOLD_Z(name, TYPE, OP, pre)                       \
  BINARY_FOLD_Z(int_float, name, TYPE, OP, pre, is_prim_int,               \
              is_prim_float, m_float, ae_prim_float, num, fnum, fnum)
#define BINARY_INT_FLOAT_FOLD2(name, TYPE, OP, pre)                      \
  BINARY_FOLD(int_float, name, TYPE, OP, pre, is_prim_int,               \
              is_prim_float, m_float, ae_prim_num, num, fnum, num)

BINARY_INT_FLOAT_FOLD(add, et_float, +,)
BINARY_INT_FLOAT_FOLD(sub, et_float, -,)
BINARY_INT_FLOAT_FOLD(mul, et_float, *, /*POWEROF2_OPT(name, <<)*/)
BINARY_INT_FLOAT_FOLD_Z(div, et_float, /, /*POWEROF2_OPT(name, >>)*/)
BINARY_INT_FLOAT_FOLD2(gt, et_bool, >,)
BINARY_INT_FLOAT_FOLD2(ge, et_bool, >=,)
BINARY_INT_FLOAT_FOLD2(lt, et_bool, <=,)
BINARY_INT_FLOAT_FOLD2(le, et_bool, <=,)
BINARY_INT_FLOAT_FOLD2(and, et_bool, &&,)
BINARY_INT_FLOAT_FOLD2(or, et_bool, ||,)
BINARY_INT_FLOAT_FOLD2(eq, et_bool, ==,)
BINARY_INT_FLOAT_FOLD2(neq, et_bool, !=,)

static GWION_IMPORT(intfloat) {
  GWI_BB(gwi_oper_ini(gwi, "int", "float", "int"))
  GWI_BB(gwi_oper_add(gwi, opck_int_float_gt))
  GWI_BB(gwi_oper_end(gwi, ">", int_float_gt))
  GWI_BB(gwi_oper_add(gwi, opck_int_float_ge))
  GWI_BB(gwi_oper_end(gwi, ">=", int_float_ge))
  GWI_BB(gwi_oper_add(gwi, opck_int_float_lt))
  GWI_BB(gwi_oper_end(gwi, "<", int_float_lt))
  GWI_BB(gwi_oper_add(gwi, opck_int_float_le))
  GWI_BB(gwi_oper_end(gwi, "<=", int_float_le))
  GWI_BB(gwi_oper_ini(gwi, "int", "float", "float"))
  GWI_BB(gwi_oper_add(gwi, opck_int_float_add))
  GWI_BB(gwi_oper_end(gwi, "+", int_float_plus))
  GWI_BB(gwi_oper_add(gwi, opck_int_float_mul))
  GWI_BB(gwi_oper_end(gwi, "*", int_float_mul))
  GWI_BB(gwi_oper_add(gwi, opck_int_float_sub))
  GWI_BB(gwi_oper_end(gwi, "-", int_float_minus))
  GWI_BB(gwi_oper_add(gwi, opck_int_float_div))
  GWI_BB(gwi_oper_end(gwi, "/", int_float_div))
  CHECK_IF("=>", rassign, r_assign)
  CHECK_IF("+=>", rassign, r_plus)
  CHECK_IF("-=>", rassign, r_minus)
  CHECK_IF("*=>", rassign, r_mul)
  CHECK_IF("/=>", rassign, r_div)
  _CHECK_OP("$", cast_i2f, CastI2F)
  _CHECK_OP("@implicit", implicit_i2f, CastI2F)
  GWI_BB(gwi_oper_ini(gwi, "int", "float", "bool"))
  GWI_BB(gwi_oper_add(gwi, opck_int_float_and))
  GWI_BB(gwi_oper_end(gwi, "&&", int_float_and))
  GWI_BB(gwi_oper_add(gwi, opck_int_float_or))
  GWI_BB(gwi_oper_end(gwi, "||", int_float_or))
  GWI_BB(gwi_oper_add(gwi, opck_int_float_eq))
  GWI_BB(gwi_oper_end(gwi, "==", int_float_eq))
  GWI_BB(gwi_oper_add(gwi, opck_int_float_neq))
  GWI_BB(gwi_oper_end(gwi, "!=", int_float_neq))
  return GW_OK;
}

#define BINARY_FLOAT_INT_FOLD(name, TYPE, OP, pre)                       \
  BINARY_FOLD(float_int, name, TYPE, OP, pre, is_prim_float,             \
              is_prim_int, m_float, ae_prim_float, fnum, num, fnum)

#define BINARY_FLOAT_INT_FOLD_Z(name, TYPE, OP, pre)                       \
  BINARY_FOLD_Z(float_int, name, TYPE, OP, pre, is_prim_float,             \
              is_prim_int, m_float, ae_prim_float, fnum, num, fnum)

#define BINARY_FLOAT_INT_FOLD2(name, TYPE, OP, pre)                      \
  BINARY_FOLD(float_int, name, TYPE, OP, pre, is_prim_float,             \
              is_prim_int, m_int, ae_prim_num, fnum, num, num)

BINARY_FLOAT_INT_FOLD(add, et_float, +,)
BINARY_FLOAT_INT_FOLD(sub, et_float, -,)
BINARY_FLOAT_INT_FOLD(mul, et_float, *, /*POWEROF2_OPT(name, <<)*/)
BINARY_FLOAT_INT_FOLD_Z(div, et_float, /, /*POWEROF2_OPT(name, >>)*/)

BINARY_FLOAT_INT_FOLD2(gt, et_bool, >,)
BINARY_FLOAT_INT_FOLD2(ge, et_bool, >=,)
BINARY_FLOAT_INT_FOLD2(lt, et_bool, <=,)
BINARY_FLOAT_INT_FOLD2(le, et_bool, <=,)
BINARY_FLOAT_INT_FOLD2(and, et_bool, &&,)
BINARY_FLOAT_INT_FOLD2(or, et_bool, ||,)
BINARY_FLOAT_INT_FOLD2(eq, et_bool, ==,)
BINARY_FLOAT_INT_FOLD2(neq, et_bool, !=,)

static GWION_IMPORT(floatint) {
  GWI_BB(gwi_oper_ini(gwi, "float", "int", "float"))
  GWI_BB(gwi_oper_add(gwi, opck_float_int_add))
  GWI_BB(gwi_oper_end(gwi, "+", float_int_plus))
  GWI_BB(gwi_oper_add(gwi, opck_float_int_sub))
  GWI_BB(gwi_oper_end(gwi, "-", float_int_minus))
  GWI_BB(gwi_oper_add(gwi, opck_float_int_mul))
  GWI_BB(gwi_oper_end(gwi, "*", float_int_mul))
  GWI_BB(gwi_oper_add(gwi, opck_float_int_div))
  GWI_BB(gwi_oper_end(gwi, "/", float_int_div))
  CHECK_FI("=>", rassign, r_assign)
  CHECK_FI("+=>", rassign, r_plus)
  CHECK_FI("-=>", rassign, r_minus)
  CHECK_FI("*=>", rassign, r_mul)
  CHECK_FI("/=>", rassign, r_div)
  _CHECK_OP("$", cast_f2i, CastF2I)
  _CHECK_OP("@implicit", implicit_f2i, CastF2I)
  GWI_BB(gwi_oper_ini(gwi, "float", "int", "bool"))
  GWI_BB(gwi_oper_add(gwi, opck_float_int_and))
  GWI_BB(gwi_oper_end(gwi, "&&", float_int_and))
  GWI_BB(gwi_oper_add(gwi, opck_float_int_or))
  GWI_BB(gwi_oper_end(gwi, "||", float_int_or))
  GWI_BB(gwi_oper_add(gwi, opck_float_int_eq))
  GWI_BB(gwi_oper_end(gwi, "==", float_int_eq))
  GWI_BB(gwi_oper_add(gwi, opck_float_int_neq))
  GWI_BB(gwi_oper_end(gwi, "!=", float_int_neq))
  GWI_BB(gwi_oper_add(gwi, opck_float_int_gt))
  GWI_BB(gwi_oper_end(gwi, ">", float_int_gt))
  GWI_BB(gwi_oper_add(gwi, opck_float_int_ge))
  GWI_BB(gwi_oper_end(gwi, ">=", float_int_ge))
  GWI_BB(gwi_oper_add(gwi, opck_float_int_lt))
  GWI_BB(gwi_oper_end(gwi, "<", float_int_lt))
  GWI_BB(gwi_oper_add(gwi, opck_float_int_le))
  GWI_BB(gwi_oper_end(gwi, "<=", float_int_le))
  return GW_OK;
}

static GWION_IMPORT(dur) {
  GWI_BB(gwi_oper_cond(gwi, "dur", BranchEqFloat, BranchNeqFloat))
  GWI_BB(gwi_oper_ini(gwi, "dur", "dur", "dur"))
  CHECK_FF("=>", rassign, r_assign)
  CHECK_FF("+=>", rassign, r_plus)
  CHECK_FF("-=>", rassign, r_minus)
  CHECK_FF("*=>", rassign, r_mul)
  CHECK_FF("/=>", rassign, r_div)
  GWI_BB(gwi_oper_end(gwi, "+", float_add))
  GWI_BB(gwi_oper_end(gwi, "-", float_sub))
  GWI_BB(gwi_oper_end(gwi, "*", float_mul))
  GWI_BB(gwi_oper_ini(gwi, "dur", "dur", "float"))
  GWI_BB(gwi_oper_eff(gwi, "ZeroDivideException"))
  GWI_BB(gwi_oper_end(gwi, "/", float_div))

  GWI_BB(gwi_oper_ini(gwi, "dur", "float", "dur"))
  GWI_BB(gwi_oper_eff(gwi, "ZeroDivideException"))
  GWI_BB(gwi_oper_end(gwi, "/", float_div))

  GWI_BB(gwi_oper_ini(gwi, "float", "dur", "dur"))
  CHECK_FF("*=>", rassign, r_mul)
  CHECK_FF("/=>", rassign, r_div)

  GWI_BB(gwi_oper_ini(gwi, "dur", "dur", "bool"))
  GWI_BB(gwi_oper_end(gwi, "==", float_eq))
  GWI_BB(gwi_oper_end(gwi, "!=", float_neq))
  GWI_BB(gwi_oper_end(gwi, ">", float_gt))
  GWI_BB(gwi_oper_end(gwi, ">=", float_ge))
  GWI_BB(gwi_oper_end(gwi, "<", float_lt))
  return gwi_oper_end(gwi, "<=", float_le);
}

static inline int is_now(const Env env, const Exp exp) {
  return exp->exp_type == ae_exp_primary &&
         exp->d.prim.prim_type == ae_prim_id &&
         exp->d.prim.d.var == insert_symbol("now");
}

static OP_CHECK(opck_now) {
  const Exp_Binary *bin = (Exp_Binary *)data;
  if (!is_now(env, bin->rhs)) CHECK_NN(opck_const_rhs(env, data));
  exp_setvar(bin->rhs, 1);
  return bin->rhs->type;
}

static GWION_IMPORT(time) {
  GWI_BB(gwi_oper_cond(gwi, "time", BranchEqFloat, BranchNeqFloat))
  GWI_BB(gwi_oper_ini(gwi, "time", "time", "time"))
  CHECK_FF("=>", rassign, r_assign)
  GWI_BB(gwi_oper_ini(gwi, "time", "dur", "time"))
  GWI_BB(gwi_oper_end(gwi, "+", float_add))
  GWI_BB(gwi_oper_end(gwi, "*", float_mul))
  GWI_BB(gwi_oper_eff(gwi, "ZeroDivideException"))
  GWI_BB(gwi_oper_end(gwi, "/", float_div))
  GWI_BB(gwi_oper_ini(gwi, "time", "time", "dur"))
  GWI_BB(gwi_oper_end(gwi, "-", float_sub))
  GWI_BB(gwi_oper_ini(gwi, "float", "time", "time"))
  CHECK_FF("*=>", rassign, r_mul)
  CHECK_FF("/=>", rassign, r_div)
  GWI_BB(gwi_oper_ini(gwi, "dur", "time", "time"))
  CHECK_FF("=>", rassign, r_assign)
  GWI_BB(gwi_oper_end(gwi, "+", float_add))
  GWI_BB(gwi_oper_ini(gwi, "dur", "@now", "time"))
  _CHECK_OP("=>", now, Time_Advance)
  GWI_BB(gwi_oper_ini(gwi, "time", "time", "bool"))
  GWI_BB(gwi_oper_end(gwi, ">", float_gt))
  GWI_BB(gwi_oper_end(gwi, ">=", float_ge))
  GWI_BB(gwi_oper_end(gwi, "<", float_lt))
  return gwi_oper_end(gwi, "<=", float_le);
}

#define BINARY_FLOAT_FOLD(name, TYPE, OP, pre)                           \
  BINARY_FOLD(float, name, TYPE, OP, pre, is_prim_float, is_prim_float,  \
              m_float, ae_prim_float, fnum, fnum, fnum)

#define BINARY_FLOAT_FOLD_Z(name, TYPE, OP, pre)                           \
  BINARY_FOLD_Z(float, name, TYPE, OP, pre, is_prim_float, is_prim_float,  \
              m_float, ae_prim_float, fnum, fnum, fnum)

#define BINARY_FLOAT_FOLD2(name, TYPE, OP, pre)                          \
  BINARY_FOLD(float, name, TYPE, OP, pre, is_prim_float, is_prim_float,  \
              m_int, ae_prim_num, fnum, fnum, num)

BINARY_FLOAT_FOLD(add, et_float, +,)
BINARY_FLOAT_FOLD(sub, et_float, -,)
BINARY_FLOAT_FOLD(mul, et_float, *, /*POWEROF2_OPT(name, <<)*/)
BINARY_FLOAT_FOLD_Z(div, et_float, /, /*POWEROF2_OPT(name, >>)*/)

BINARY_FLOAT_FOLD2(and, et_bool, &&,)
BINARY_FLOAT_FOLD2(or, et_bool, ||,)
BINARY_FLOAT_FOLD2(eq, et_bool, ==,)
BINARY_FLOAT_FOLD2(neq, et_bool, !=,)
BINARY_FLOAT_FOLD2(gt, et_bool, >,)
BINARY_FLOAT_FOLD2(ge, et_bool, >=,)
BINARY_FLOAT_FOLD2(lt, et_bool, <,)
BINARY_FLOAT_FOLD2(le, et_bool, <=,)

#define BINARY_FLOAT_EMIT(name) BINARY_OP_EMIT(name, float, fnum)
BINARY_FLOAT_EMIT(add)
BINARY_FLOAT_EMIT(sub)
BINARY_FLOAT_EMIT(mul)
BINARY_FLOAT_EMIT(div)
BINARY_FLOAT_EMIT(ge)
BINARY_FLOAT_EMIT(gt)
BINARY_FLOAT_EMIT(le)
BINARY_FLOAT_EMIT(lt)

#define UNARY_FLOAT_FOLD(name, TYPE, OP)                                       \
  UNARY_FOLD(float, name, TYPE, OP, is_prim_int, m_float, ae_prim_float, fnum)
UNARY_FLOAT_FOLD(negate, et_float, -)
// UNARY_INT_FOLD(cmp, et_float, ~)
UNARY_FLOAT_FOLD(not, et_bool, !)

#define IMPORT_BINARY_FLOAT(name, op)          \
  GWI_BB(gwi_oper_add(gwi, opck_float_##name)) \
  GWI_BB(gwi_oper_emi(gwi, opem_float_##name)) \
  GWI_BB(gwi_oper_end(gwi, #op, NULL))

static GWION_IMPORT(float) {
  GWI_BB(gwi_oper_cond(gwi, "float", BranchEqFloat, BranchNeqFloat))
  GWI_BB(gwi_oper_ini(gwi, "float", "float", "float"))
  IMPORT_BINARY_FLOAT(add, +);
  IMPORT_BINARY_FLOAT(sub, -);
  IMPORT_BINARY_FLOAT(mul, *);
  IMPORT_BINARY_FLOAT(div, /);
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
  CHECK_FF("=>", rassign, r_assign)
  CHECK_FF("+=>", rassign, r_plus)
  CHECK_FF("-=>", rassign, r_minus)
  CHECK_FF("*=>", rassign, r_mul)
  CHECK_FF("/=>", rassign, r_div)
  GWI_BB(gwi_oper_ini(gwi, "float", "float", "bool"))
  GWI_BB(gwi_oper_add(gwi, opck_float_and))
  GWI_BB(gwi_oper_end(gwi, "&&", float_and))
  GWI_BB(gwi_oper_add(gwi, opck_float_or))
  GWI_BB(gwi_oper_end(gwi, "||", float_or))
  GWI_BB(gwi_oper_add(gwi, opck_float_eq))
  GWI_BB(gwi_oper_end(gwi, "==", float_eq))
  GWI_BB(gwi_oper_add(gwi, opck_float_neq))
  GWI_BB(gwi_oper_end(gwi, "!=", float_neq))
  IMPORT_BINARY_FLOAT(gt, >);
  IMPORT_BINARY_FLOAT(ge, >=);
  IMPORT_BINARY_FLOAT(lt, <);
  IMPORT_BINARY_FLOAT(le, <=);
  GWI_BB(gwi_oper_ini(gwi, NULL, "float", "float"))
  //  CHECK_FF("-", unary_meta, negate)
  GWI_BB(gwi_oper_add(gwi, opck_float_negate))
  GWI_BB(gwi_oper_end(gwi, "-", float_negate))
  GWI_BB(gwi_oper_ini(gwi, "int", "dur", "dur"))
  GWI_BB(gwi_oper_end(gwi, "::", int_float_mul))
  GWI_BB(gwi_oper_ini(gwi, "float", "dur", "dur"))
  GWI_BB(gwi_oper_end(gwi, "::", float_mul))
  GWI_BB(gwi_oper_ini(gwi, NULL, "float", "bool"))
  //  GWI_BB(gwi_oper_add(gwi, opck_unary_meta2))
  GWI_BB(gwi_oper_add(gwi, opck_float_not))
  GWI_BB(gwi_oper_end(gwi, "!", float_not))
  return GW_OK;
}

GWION_IMPORT(prim) {
  GWI_BB(import_int(gwi))      // const folded + imm optimized
  GWI_BB(import_float(gwi))    // const folded
  GWI_BB(import_intfloat(gwi)) // const folded
  GWI_BB(import_floatint(gwi)) // const folded
  GWI_BB(import_dur(gwi))
  return import_time(gwi);
}
