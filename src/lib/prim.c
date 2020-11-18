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

GWION_IMPORT(int_op) {
  GWI_BB(gwi_oper_ini(gwi, "int", "int", "int"))
  GWI_BB(gwi_oper_end(gwi, "+", int_plus))
  GWI_BB(gwi_oper_end(gwi, "-", int_minus))
  GWI_BB(gwi_oper_end(gwi, "*", int_mul))
  GWI_BB(gwi_oper_end(gwi, "/", int_div))
  return gwi_oper_end(gwi, "%", int_modulo);
}

static GWION_IMPORT(int_logical) {
  GWI_BB(gwi_oper_end(gwi, "&&",  int_and))
  GWI_BB(gwi_oper_end(gwi, "||",   int_or))
  GWI_BB(gwi_oper_end(gwi, "==",   int_eq))
  GWI_BB(gwi_oper_end(gwi, "!=",   int_neq))
  GWI_BB(gwi_oper_end(gwi, ">",   int_gt))
  GWI_BB(gwi_oper_end(gwi, ">=",   int_ge))
  GWI_BB(gwi_oper_end(gwi, "<",   int_lt))
  GWI_BB(gwi_oper_end(gwi, "<=",   int_le))
  GWI_BB(gwi_oper_end(gwi, ">>",  int_sr))
  GWI_BB(gwi_oper_end(gwi, "<<",  int_sl))
  GWI_BB(gwi_oper_end(gwi, "&", int_sand))
  GWI_BB(gwi_oper_end(gwi, "|",  int_sor))
  return   gwi_oper_end(gwi, "^", int_xor);
}

static GWION_IMPORT(int_r) {
  CHECK_OP("=>", rassign, r_assign)
  CHECK_OP("+=>",  rassign, r_plus)
  CHECK_OP("-=>",  rassign, r_minus)
  CHECK_OP("*=>",  rassign, r_mul)
  CHECK_OP("/=>",  rassign, r_div)
  CHECK_OP("%=>",  rassign, r_modulo)
  CHECK_OP("<<=>",   rassign, r_sl)
  CHECK_OP(">>=>",   rassign, r_sr)
  CHECK_OP("&=>", rassign, r_sand)
  CHECK_OP("|=>",  rassign, r_sor)
  CHECK_OP("^=>", rassign, r_sxor)
  return GW_OK;
}

static INSTR(IntRange) {
  shred->reg -= SZ_INT *2;
  const m_int start = *(m_int*)REG(0);
  const m_int end   = *(m_int*)REG(SZ_INT);
  const m_int op    = start < end ? 1 : -1;
  const m_uint sz    = op > 0 ? end - start : start - end;
  const M_Object array = new_array(shred->info->vm->gwion->mp, (Type)instr->m_val, sz);
  for(m_int i = start, j = 0; i != end; i += op, ++j)
    m_vector_set(ARRAY(array), j, &i);
  *(M_Object*)REG(0) = array;
  PUSH_REG(shred, SZ_INT);
}

static OP_CHECK(opck_int_range) {
  const Exp exp = (Exp)data;
  const Range *range = exp->d.prim.d.range;
  const Exp e = range->start ?: range->end;
  return array_type(env, e->info->type, 1);
}

static OP_EMIT(opem_int_range) {
  const Exp exp = (Exp)data;
  const Instr instr = emit_add_instr(emit, IntRange);
  instr->m_val = (m_uint)exp->info->type;
  return instr;
}

static GWION_IMPORT(int_unary) {
  GWI_BB(gwi_oper_ini(gwi, NULL, "int", "int"))
  GWI_BB(gwi_oper_add(gwi, opck_unary_meta))
  GWI_BB(gwi_oper_end(gwi, "-", int_negate))
  CHECK_OP("++", unary, pre_inc)
  CHECK_OP("--", unary, pre_dec)
  GWI_BB(gwi_oper_end(gwi,  "~", int_cmp))
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
  INTERP_PRINTF("%s", *(m_uint*)VALUE ? "true" : "false");
}

static GWION_IMPORT(int_values) {
  GWI_BB(gwi_enum_ini(gwi, "bool"))
  GWI_BB(gwi_enum_add(gwi, "false", 0))
  GWI_BB(gwi_enum_add(gwi, "true", 1))
  const Type t_bool = gwi_enum_end(gwi);
  GWI_BB(gwi_gack(gwi, t_bool, gack_bool))
  gwi->gwion->type[et_bool] = t_bool;
  GWI_BB(gwi_oper_ini(gwi, NULL, "int", "bool"))
  GWI_BB(gwi_oper_add(gwi, opck_unary_meta)) // should return bool
  GWI_BB(gwi_oper_end(gwi,  "!", IntNot))
  struct SpecialId_ spid = { .type=t_bool, .exec=RegPushMaybe, .is_const=1 };
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

static OP_CHECK(opck_cast_f2i) {
  return env->gwion->type[et_int];
}

static OP_CHECK(opck_implicit_f2i) {
  return env->gwion->type[et_null];
}

static OP_CHECK(opck_cast_i2f) {
  return env->gwion->type[et_float];
}

static OP_CHECK(opck_implicit_i2f) {
  struct Implicit* imp = (struct Implicit*)data;
  return imp->e->info->cast_to = env->gwion->type[et_float];
}

#define CHECK_FF(op, check, func) _CHECK_OP(op, check, float_##func)
#define CHECK_IF(op, check, func) _CHECK_OP(op, check, int_float_##func)
#define CHECK_FI(op, check, func) _CHECK_OP(op, check, float_int_##func)

static GWION_IMPORT(intfloat) {
  GWI_BB(gwi_oper_ini(gwi, "int", "float", "int"))
  GWI_BB(gwi_oper_end(gwi, "&&",           int_float_and))
  GWI_BB(gwi_oper_end(gwi, "||",            int_float_or))
  GWI_BB(gwi_oper_end(gwi, "==", 			 int_float_eq))
  GWI_BB(gwi_oper_end(gwi, "!=", 			 int_float_neq))
  GWI_BB(gwi_oper_end(gwi, ">", 			 int_float_gt))
  GWI_BB(gwi_oper_end(gwi, ">=", 			 int_float_ge))
  GWI_BB(gwi_oper_end(gwi, "<", 			 int_float_lt))
  GWI_BB(gwi_oper_end(gwi, "<=", 			 int_float_le))
  GWI_BB(gwi_oper_ini(gwi, "int", "float", "float"))
  GWI_BB(gwi_oper_end(gwi, "+",          int_float_plus))
  GWI_BB(gwi_oper_end(gwi, "*",         int_float_mul))
  GWI_BB(gwi_oper_end(gwi, "-",         int_float_minus))
  GWI_BB(gwi_oper_end(gwi, "/",        int_float_div))
  CHECK_IF("=>", rassign, r_assign)
  CHECK_IF("+=>", rassign, r_plus)
  CHECK_IF("-=>", rassign, r_minus)
  CHECK_IF("*=>", rassign, r_mul)
  CHECK_IF("/=>", rassign, r_div)
  _CHECK_OP("$", cast_i2f, CastI2F)
  _CHECK_OP("@implicit", implicit_i2f, CastI2F)
  return GW_OK;
}

static GWION_IMPORT(floatint) {
  GWI_BB(gwi_oper_ini(gwi, "float", "int", "float"))
  GWI_BB(gwi_oper_end(gwi, "+",         float_int_plus))
  GWI_BB(gwi_oper_end(gwi, "-",        float_int_minus))
  GWI_BB(gwi_oper_end(gwi, "*",        float_int_mul))
  GWI_BB(gwi_oper_end(gwi, "/",       float_int_div))
  GWI_BB(gwi_oper_ini(gwi, "float", "int", "int"))
  GWI_BB(gwi_oper_end(gwi, "&&",          float_int_and))
  GWI_BB(gwi_oper_end(gwi, "||",           float_int_or))
  GWI_BB(gwi_oper_end(gwi, "==", 			float_int_eq))
  GWI_BB(gwi_oper_end(gwi, "!=", 			float_int_neq))
  GWI_BB(gwi_oper_end(gwi, ">", 			float_int_gt))
  GWI_BB(gwi_oper_end(gwi, ">=", 			float_int_ge))
  GWI_BB(gwi_oper_end(gwi, "<", 			float_int_lt))
  GWI_BB(gwi_oper_end(gwi, "<=", 			float_int_le))
  CHECK_FI("=>", rassign, r_assign)
  CHECK_FI("+=>", rassign, r_plus)
  CHECK_FI("-=>", rassign, r_minus)
  CHECK_FI("*=>", rassign, r_mul)
  CHECK_FI("/=>", rassign, r_div)
  _CHECK_OP("$", cast_f2i, CastF2I)
  _CHECK_OP("@implicit", implicit_f2i, CastF2I)
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
  GWI_BB(gwi_oper_end(gwi, "+",         FloatPlus))
  GWI_BB(gwi_oper_end(gwi, "-",        FloatMinus))
  GWI_BB(gwi_oper_end(gwi, "*",        FloatTimes))
  GWI_BB(gwi_oper_ini(gwi, "dur", "dur", "float"))
  GWI_BB(gwi_oper_end(gwi, "/",       FloatDivide))

  GWI_BB(gwi_oper_ini(gwi, "dur", "float", "dur"))
  GWI_BB(gwi_oper_end(gwi, "/",       FloatDivide))

  GWI_BB(gwi_oper_ini(gwi, "dur", "dur", "int"))
  GWI_BB(gwi_oper_end(gwi, ">",           float_gt))
  GWI_BB(gwi_oper_end(gwi, ">=",           float_ge))
  GWI_BB(gwi_oper_end(gwi, "<",           float_lt))
  return gwi_oper_end(gwi, "<=",           float_le);
}


static inline int is_now(const Env env, const Exp exp) {
  return exp->exp_type == ae_exp_primary &&
    exp->d.prim.prim_type == ae_prim_id &&
    exp->d.prim.d.var == insert_symbol("now");
}

static OP_CHECK(opck_now) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  if(!is_now(env, bin->rhs))
    CHECK_NN(opck_const_rhs(env, data, mut))
  exp_setvar(bin->rhs, 1);
  return bin->rhs->info->type;
}

static GWION_IMPORT(time) {
  GWI_BB(gwi_oper_cond(gwi, "time", BranchEqFloat, BranchNeqFloat))
  GWI_BB(gwi_oper_ini(gwi, "time", "time", "time"))
  CHECK_FF("=>", rassign, r_assign)
  GWI_BB(gwi_oper_ini(gwi, "time", "dur", "time"))
  GWI_BB(gwi_oper_end(gwi, "+",         FloatPlus))
  GWI_BB(gwi_oper_ini(gwi, "time", "time", "dur"))
  GWI_BB(gwi_oper_end(gwi, "-",         FloatMinus))
  GWI_BB(gwi_oper_ini(gwi, "dur", "time", "time"))
  CHECK_FF("=>", rassign, r_assign)
  GWI_BB(gwi_oper_end(gwi, "+",         FloatPlus))
  GWI_BB(gwi_oper_ini(gwi,  "dur",  "@now", "time"))
  _CHECK_OP("=>", now, Time_Advance)
  GWI_BB(gwi_oper_ini(gwi, "time", "time", "int"))
  GWI_BB(gwi_oper_end(gwi, ">",           float_gt))
  GWI_BB(gwi_oper_end(gwi, ">=", 	      float_ge))
  GWI_BB(gwi_oper_end(gwi, "<", 		  float_lt))
  return gwi_oper_end(gwi, "<=",           float_le);
}

static GWION_IMPORT(float) {
  GWI_BB(gwi_oper_cond(gwi, "float", BranchEqFloat, BranchNeqFloat))
  GWI_BB(gwi_oper_ini(gwi, "float", "float", "float"))
  GWI_BB(gwi_oper_end(gwi, "+",          FloatPlus))
  GWI_BB(gwi_oper_end(gwi, "-",         FloatMinus))
  GWI_BB(gwi_oper_end(gwi, "*",         FloatTimes))
  GWI_BB(gwi_oper_end(gwi, "/",        FloatDivide))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
  CHECK_FF("=>", rassign, r_assign)
  CHECK_FF("+=>", rassign, r_plus)
  CHECK_FF("-=>", rassign, r_minus)
  CHECK_FF("*=>", rassign, r_mul)
  CHECK_FF("/=>", rassign, r_div)
  GWI_BB(gwi_oper_ini(gwi, "float", "float", "int"))
  GWI_BB(gwi_oper_end(gwi, "&&",           float_and))
  GWI_BB(gwi_oper_end(gwi, "||",            float_or))
  GWI_BB(gwi_oper_end(gwi, "==", 			 float_eq))
  GWI_BB(gwi_oper_end(gwi, "!=", 			 float_neq))
  GWI_BB(gwi_oper_end(gwi, ">", 			 float_gt))
  GWI_BB(gwi_oper_end(gwi, ">=", 			 float_ge))
  GWI_BB(gwi_oper_end(gwi, "<", 			 float_lt))
  GWI_BB(gwi_oper_end(gwi, "<=", 			 float_le))
  GWI_BB(gwi_oper_ini(gwi, NULL,   "float", "float"))
  CHECK_FF("-", unary_meta, negate)
  GWI_BB(gwi_oper_ini(gwi, NULL,   "float", "int"))
  GWI_BB(gwi_oper_ini(gwi, NULL,   "time", "int"))
  GWI_BB(gwi_oper_ini(gwi, NULL,   "dur", "int"))
  GWI_BB(gwi_oper_ini(gwi, "int", "dur", "dur"))
  GWI_BB(gwi_oper_end(gwi, "::",         int_float_mul))
  GWI_BB(gwi_oper_ini(gwi, "float", "dur", "dur"))
  GWI_BB(gwi_oper_end(gwi, "::",         FloatTimes))
  GWI_BB(gwi_oper_ini(gwi, NULL,   "float", "bool"))
  GWI_BB(gwi_oper_add(gwi, opck_unary_meta2)) // should return bool
  GWI_BB(gwi_oper_end(gwi,  "!", float_not))
  return GW_OK;
}

GWION_IMPORT(prim) {
  GWI_BB(import_int(gwi))
  GWI_BB(import_float(gwi))
  GWI_BB(import_intfloat(gwi))
  GWI_BB(import_floatint(gwi))
  GWI_BB(import_dur(gwi))
  return import_time(gwi);
}
