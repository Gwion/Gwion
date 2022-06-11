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
#include "specialid.h"
#include "gack.h"
#include "traverse.h"

static GACK(gack_none) { INTERP_PRINTF("None") }

static OP_CHECK(opck_none) {
  Exp_Binary *bin = (Exp_Binary *)data;
  CHECK_NN(opck_rassign(env, data));
  exp_setvar(bin->rhs, 1);
  return bin->rhs->type;
}

static OP_EMIT(opem_none) {
  const Instr instr = emit_add_instr(emit, RegMove);
  instr->m_val      = -SZ_INT;
  return GW_OK;
}

static OP_EMIT(opem_union_dot) {
  const Exp_Dot *member = (Exp_Dot *)data;
  const Map      map    = &member->base->type->nspc->info->value->map;
  CHECK_BB(emit_exp(emit, member->base));
  if (is_func(emit->gwion, exp_self(member)->type)) { // is_callable? can only be a func
    const Instr instr = emit_add_instr(emit, RegPushImm);
    const Func  f =
        (Func)vector_front(&member->base->type->info->parent->nspc->vtable);
    instr->m_val = (m_uint)f->code;
    return GW_OK;
  }
  if (!strcmp(s_name(member->xid), "index")) {
    emit_add_instr(emit, DotMember);
    return GW_OK;
  }
  for (m_uint i = 0; i < map_size(map); ++i) {
    if (VKEY(map, i) == (m_uint)member->xid) {
      const Value v         = (Value)VVAL(map, i);
      const uint  emit_addr = exp_getvar(exp_self(member));
      const Instr instr     = emit_unionmember(emit, v->type->size, emit_addr);
      instr->m_val          = i; // + 1;
      instr->m_val2         = v->type->size;
      return GW_OK;
    }
  }
  return GW_ERROR;
}

static DTOR(UnionDtor) {
  const m_uint idx = *(m_uint *)o->data;
  if (idx) {
    const Map   map = &o->type_ref->nspc->info->value->map;
    const Value v   = (Value)map_at(map, idx - 1);
    if (isa(v->type, shred->info->vm->gwion->type[et_compound]) > 0)
      compound_release(shred, v->type, (o->data + SZ_INT));
  }
}

static OP_CHECK(opck_union_is) {
  const Exp       e    = (Exp)data;
  const Exp_Call *call = &e->d.exp_call;
  const Exp       exp  = call->args;
  if (exp->exp_type != ae_exp_primary && exp->d.prim.prim_type != ae_prim_id)
    ERR_N(exp->pos, "Union.is() argument must be of form id");
  const Type  t = call->func->d.exp_dot.base->type;
  const Value v = find_value(t, exp->d.prim.d.var);
  if (!v)
    ERR_N(exp->pos, "'%s' has no member '%s'", t->name,
          s_name(exp->d.prim.d.var));
  const Map map = &t->nspc->info->value->map;
  for (m_uint i = 0; i < map_size(map); ++i) {
    const Value v = (Value)VVAL(map, i);
    if (!strcmp(s_name(exp->d.prim.d.var), v->name)) {
      const Exp exp_func  = call->func;
      const Exp exp_args  = call->args;
      e->exp_type         = ae_exp_binary;
      e->d.exp_binary.lhs = cpy_exp(env->gwion->mp, exp_func);
      e->d.exp_binary.lhs->d.exp_dot.xid =
          insert_symbol(env->gwion->st, "index");
      //      e->d.exp_binary.rhs = new_prim_int(env->gwion->mp, i+1, e->pos);
      e->d.exp_binary.rhs = new_prim_int(env->gwion->mp, i, e->pos);
      free_exp(env->gwion->mp, exp_func);
      free_exp(env->gwion->mp, exp_args);
      e->d.exp_binary.op = insert_symbol(env->gwion->st, "==");
      CHECK_ON(check_exp(env, e));
      return e->type;
    }
  }
  return env->gwion->type[et_error];
}

static MFUN(union_is) {
  *(m_uint *)RETURN = *(m_uint *)MEM(SZ_INT) == *(m_uint *)o->data;
}

static MFUN(union_new) {
  memcpy(o->data, MEM(SZ_INT*2), *(m_uint*)MEM(SZ_INT));
  *(M_Object *)RETURN = o;
}

static OP_CHECK(opck_union_new) {
  Exp_Call *call = (Exp_Call *)data;
  const Exp name = call->args;
  if (!name || !name->next || name->next->next)
    ERR_N(call->func->pos, "Union constructor takes two arguments, "
                           "'id' and 'value'");
  if (name->exp_type != ae_exp_primary || name->d.prim.prim_type != ae_prim_id)
    return NULL;
  const Exp  val  = name->next;
  const Type base = call->func->d.exp_dot.base->type;
  const Map  map  = &base->nspc->info->value->map;
  for (m_uint i = 0; i < map_size(map); ++i) {
    if (VKEY(map, i) == (m_uint)name->d.prim.d.var) {
      const Value v          = (Value)VVAL(map, i);
      name->d.prim.prim_type = ae_prim_num;
      name->d.prim.d.num     = i;
      name->type             = env->gwion->type[et_int];
      DECL_ON(const Type, t, = check_exp(env, val));
      if (isa(t, v->type) < 0) {
        ERR_N(val->pos, "Invalid type '%s' for '%s', should be '%s'", t->name,
              v->name, v->type->name);
      }
      const Exp e = new_prim_int(env->gwion->mp, t->size + SZ_INT, val->pos);
      e->next = name;
      e->type = env->gwion->type[et_int];
      call->args = e;
      return base;
    }
  }
  return NULL;
}

ANN GWION_IMPORT(union) {
  const Type t_none = gwi_mk_type(gwi, "None", 0, NULL);
  GWI_BB(gwi_set_global_type(gwi, t_none, et_none))
  GWI_BB(gwi_gack(gwi, t_none, gack_none))
  gwi_add_type(gwi, t_none);
  struct SpecialId_ spid = {
      .type = gwi->gwion->type[et_none], .exec = NoOp, .is_const = 1};
  gwi_specialid(gwi, "None", &spid);

  GWI_BB(gwi_oper_ini(gwi, "None", "None", "None"))
  GWI_BB(gwi_oper_add(gwi, opck_none))
  GWI_BB(gwi_oper_emi(gwi, opem_none))
  GWI_BB(gwi_oper_end(gwi, "=>", NoOp))

  const Type t_union = gwi_class_ini(gwi, "union", "Object");
  gwi_class_xtor(gwi, NULL, UnionDtor);
  gwi->gwion->type[et_union] = t_union;

  GWI_BB(gwi_item_ini(gwi, "int", "index"))
  GWI_BB(gwi_item_end(gwi, ae_flag_const, num, 0))
  GWI_BB(gwi_func_ini(gwi, "bool", "is"))
  GWI_BB(gwi_func_arg(gwi, "int", "member"))
  GWI_BB(gwi_func_end(gwi, union_is, ae_flag_none))
  GWI_BB(gwi_func_ini(gwi, "auto", "new:[T]"))
  GWI_BB(gwi_func_arg(gwi, "int", "size"))
  GWI_BB(gwi_func_arg(gwi, "int", "id"))
  GWI_BB(gwi_func_arg(gwi, "T", "value"))
  GWI_BB(gwi_func_end(gwi, union_new, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

  const Func             f      = (Func)vector_front(&t_union->nspc->vtable);
  const struct Op_Func   opfunc = {.ck = opck_union_is};
  const struct Op_Import opi    = {
      .rhs  = f->value_ref->type,
      .func = &opfunc,
      .data = (uintptr_t)f,
      .pos  = gwi->loc,
      .op   = insert_symbol(gwi->gwion->st, "@func_check")};
  CHECK_BB(add_op(gwi->gwion, &opi));

  const Func             f1      = (Func)vector_at(&t_union->nspc->vtable, 1);
  const struct Op_Func   opfunc1 = {.ck = opck_union_new};
  const struct Op_Import opi1    = {
      .rhs  = f1->value_ref->type,
      .func = &opfunc1,
      .data = (uintptr_t)f1,
      .pos  = gwi->loc,
      .op   = insert_symbol(gwi->gwion->st, "@func_check")};
  CHECK_BB(add_op(gwi->gwion, &opi1));

builtin_func(gwi->gwion->mp, f1, union_new);
  gwi->gwion->type[et_union] = t_union;

  GWI_BB(gwi_oper_ini(gwi, "union", (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_emi(gwi, opem_union_dot))
  GWI_BB(gwi_oper_end(gwi, "@dot", NULL))

  GWI_BB(gwi_union_ini(gwi, "Option:[A]"))
  GWI_BB(gwi_union_add(gwi, "None", "none"))
  GWI_BB(gwi_union_add(gwi, "A", "val"))
  GWI_BB(gwi_union_end(gwi, ae_flag_none))

  return GW_OK;
}
