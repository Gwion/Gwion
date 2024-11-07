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
  emit_regmove(emit, -SZ_INT);
  return true;
}

static INSTR(UnionIndex) {
  // probs exosts already
  *(m_uint*)REG(-SZ_INT) = **(m_uint**)REG(-SZ_INT);
}

static bool needs_reset(const Env env, const Exp *base) {
  return base->cast_to
//    && isa(base->cast_t
    && strcmp(*(m_str*)base->cast_to->nspc->class_data, env->curr->name);
}

// TODO: put in header, check other uses
OP_CHECK(opck_object_dot);

OP_EMIT(opem_setunion_implicit) { exit(19);}

OP_CHECK(opck_setunion_class) {
  struct TemplateScan *ts      = (struct TemplateScan *)data;
  const Type base = known_type(env, mp_vector_at(ts->td->types, TmplArg, 0)->d.td);
  const m_str name = mp_vector_at(ts->td->types, TmplArg, 1)->d.exp->d.prim.d.string.data;
  char buf[256];
  snprintf(buf, 256, "FlowType:[%s,\"%s\",\"%s\"]",
           base->name, name, env->curr->name); // vector_at 1
  const Type t = type_copy(env->gwion->mp, base);
  t->name = s_name(insert_symbol(env->gwion->st, buf));
  t->info->parent = ts->t; // check if not set by copy
  t->info->base_type = base;
  t->nspc = new_nspc(env->gwion->mp, t->name);
  t->nspc->class_data_size = 16;
  nspc_allocdata(env->gwion->mp, t->nspc);
  *(m_str*)t->nspc->class_data = s_name(insert_symbol(env->gwion->st, name));
  *(m_str*)(t->nspc->class_data+SZ_INT) = s_name(insert_symbol(env->gwion->st, env->name));
  return t;
}

OP_CHECK(opck_setunion_implicit) {
  exit(87);
}

static Value find_dot_value(const Env env, const Exp *exp) {
//  if(exp->exp_type == ae_exp_dot)
//    return find_dot_value(
  if(exp->exp_type == ae_exp_primary && exp->d.prim.prim_type == ae_prim_id)
    return nspc_lookup_value1(env->curr, exp->d.prim.d.var);
  return NULL;
}

static OP_CHECK(opck_setunion_dot) { 
  const Exp_Dot *member = (Exp_Dot *)data;
  const Type set_t = member->base->type;
  const Type t = member->base->type->info->base_type;
  member->base->type = t;
  const Type ret = opck_object_dot(env, data);
  member->base->type = set_t;
  return ret;
}

static OP_CHECK(opck_setunion_dot2) {
  puts(__func__);
  const Exp_Dot *member = (Exp_Dot *)data;
  puts(member->base->type->name);
  const m_str name = *(m_str*)member->base->type->nspc->class_data;
  if(!exp_getvar(exp_self(member)) && strcmp(s_name(member->tag.sym), name)) {
    // change member->base->loc to member->tag.loc
    char buf[256];
    sprintf(buf, "expected {G}%s{0}", name);
    gwlog_error("invalid union access", buf, env->name, member->base->loc, 0);
    return env->gwion->type[et_error];
  }
  member->base->type = member->base->type->info->base_type;
  return exp_self(member)->type;
}

static OP_CHECK(opck_union_dot) {
  const Exp_Dot *member = (Exp_Dot *)data;
  find_dot_value(env, member->base);
  DECL_NN(const Type, ret, = opck_object_dot(env, data));
/*
  char buf[256];
  snprintf(buf, 256, "FlowType:[%s,\"%s\",\"%s\"]",
           member->base->type->name, s_name(member->xid), env->curr->name);
  const Value v = find_dot_value(env, member->base);
  if(v)
    v->type = str2type(env->gwion, buf, member->base->loc);
*/
  return ret;
}

static OP_EMIT(opem_union_dot) {
  const Exp_Dot *member = (Exp_Dot *)data;
  exp_setvar(member->base, true);
  CHECK_B(emit_exp(emit, member->base));
  if (is_func(emit->gwion, exp_self(member)->type)) { // is_callable? can only be a func
    emit_pushimm(emit, (m_uint)exp_self(member)->type->info->func->code);
    return true;
  }
  if (!strcmp(s_name(member->tag.sym), "index")) {
    emit_add_instr(emit, UnionIndex);
    return true;
  }
  const Map      map    = &member->base->type->nspc->info->value->map;
  for (m_uint i = 0; i < map_size(map); ++i) {
    if (VKEY(map, i) == (m_uint)member->tag.sym) {
      const Value v         = (Value)VVAL(map, i);
      const uint  emit_addr = exp_getvar(exp_self(member));
      emit_unionmember(emit, i, v->type->size, emit_addr);
      return true;
    }
  }
  return false;
}

ANN void union_release(const VM_Shred shred, const Type t, const m_bit *data) {
  const m_uint idx = *(m_uint *)data;
  if (idx) {
    const Map   map = &t->nspc->info->value->map;
    const Value v   = (Value)map_at(map, idx - 1);
    if (tflag(v->type, tflag_compound))
      compound_release(shred, v->type, data + SZ_INT);
    // what if there is an union instead
  }
}

static OP_CHECK(opck_union_is) {
  Exp*       e    = (Exp*)data;
  const Exp_Call *call = &e->d.exp_call;
  Exp*       exp  = call->args;
  if(!exp)
    ERR_N(e->loc, "Union.is() takes one argument of form id");
  if (exp->exp_type != ae_exp_primary || exp->d.prim.prim_type != ae_prim_id)
    ERR_N(exp->loc, "Union.is() argument must be of form id");
  const Type  t = call->func->d.exp_dot.base->type;
  const Value v = find_value(t, exp->d.prim.d.var);
  if (!v)
    ERR_N(exp->loc, "'%s' has no member '%s'", t->name,
          s_name(exp->d.prim.d.var));
  const Map map = &t->nspc->info->value->map;
  for (m_uint i = 0; i < map_size(map); ++i) {
    const Value v = (Value)VVAL(map, i);
    if (!strcmp(s_name(exp->d.prim.d.var), v->name)) {
      Exp* exp_func  = call->func;
      Exp* exp_args  = call->args;
      e->exp_type         = ae_exp_binary;
      e->d.exp_binary.lhs = cpy_exp(env->gwion->mp, exp_func);
      e->d.exp_binary.lhs->d.exp_dot.tag.sym =
          insert_symbol(env->gwion->st, "index");
      //      e->d.exp_binary.rhs = new_prim_int(env->gwion->mp, i+1, e->loc);
      e->d.exp_binary.rhs = new_prim_int(env->gwion->mp, i, e->loc);
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
  m_bit *data = *(m_bit**)MEM(0);
  memcpy(data, MEM(SZ_INT*2), *(m_uint*)MEM(SZ_INT));
  memcpy((m_bit*)RETURN, data, *(m_uint*)MEM(SZ_INT));
}
#include "parse.h"
#undef insert_symbol
static OP_CHECK(opck_union_new) {
  Exp_Call *call = (Exp_Call *)data;
  Exp* name = call->args;
  if (!name)
    ERR_N(call->func->loc, "Union constructor takes one or two arguments, "
                           "'id' and 'value'");
  if (name->exp_type != ae_exp_primary || name->d.prim.prim_type != ae_prim_id)
    ERR_N(call->func->loc, "Union constructor first argument me be an identifier");
  Exp*  val  = name->next;
  const Type base = call->func->d.exp_dot.base->type;
  const Map  map  = &base->nspc->info->value->map;

  for (m_uint i = 0; i < map_size(map); ++i) {
    if (VKEY(map, i) == (m_uint)name->d.prim.d.var) {
      const Value v            = (Value)VVAL(map, i);
      name->d.prim.prim_type   = ae_prim_num;
      name->d.prim.d.gwint.num = i;
      name->type               = env->gwion->type[et_int];
      if(!val && v->type == env->gwion->type[et_none]) {
        Exp* e = new_prim_int(env->gwion->mp, SZ_INT, name->loc);
        e->next = name;
        e->type = env->gwion->type[et_int];
        name->next = new_prim_int(env->gwion->mp, 0, name->loc);
        name->next->type = env->gwion->type[et_int];
        call->args = e;
      } else {
        if (val->next)
          ERR_N(call->func->loc, "too many arguments for union constructor");
        DECL_ON(const Type, t, = check_exp(env, val));
        if (!check_implicit(env, val, v->type)) {
          ERR_N(val->loc, "Invalid type '%s' for '%s', should be '%s'", t->name,
                v->name, v->type->name);
        }
        Exp* e = new_prim_int(env->gwion->mp, t->size + SZ_INT, val->loc);
        e->next = name;
        e->type = env->gwion->type[et_int];
        call->args = e;
      }
      return base;
    }
  }
  ERR_N(name->loc, "%s has no member %s\n", base->name, s_name(name->d.prim.d.var));
}

ANN GWION_IMPORT(union) {
  const Type t_none = gwi_mk_type(gwi, "None", 0, NULL);
  GWI_B(gwi_set_global_type(gwi, t_none, et_none))
  GWI_B(gwi_gack(gwi, t_none, gack_none))
  gwi_add_type(gwi, t_none);
  struct SpecialId_ spid = {
      .type = gwi->gwion->type[et_none], .exec = NoOp, .is_const = 1};
  gwi_specialid(gwi, "None", &spid);

   GWI_B(gwi_oper_ini(gwi, "None", "None", "None"))
   GWI_B(gwi_oper_add(gwi, opck_none))
   GWI_B(gwi_oper_emi(gwi, opem_none))
   GWI_B(gwi_oper_end(gwi, ":=>", NoOp))

  const Type t_union = gwi_struct_ini(gwi, "Union");
  //gwi_class_xtor(gwi, NULL, UnionDtor);
  gwi->gwion->type[et_union] = t_union;

  GWI_B(gwi_item_ini(gwi, "int", "index"))
  GWI_B(gwi_item_end(gwi, ae_flag_const, num, 0))
  /*
  GWI_B(gwi_func_ini(gwi, "void", "@ctor"))
  GWI_B(gwi_func_end(gwi, union_ctor, ae_flag_none))
  */
  GWI_B(gwi_func_ini(gwi, "bool", "is"))
  GWI_B(gwi_func_arg(gwi, "int", "member"))
  GWI_B(gwi_func_end(gwi, union_is, ae_flag_none))
  GWI_B(gwi_func_ini(gwi, "auto", "new:[T]"))
  GWI_B(gwi_func_arg(gwi, "int", "size"))
  GWI_B(gwi_func_arg(gwi, "int", "id"))
  GWI_B(gwi_func_arg(gwi, "T", "value"))
  GWI_B(gwi_func_end(gwi, union_new, ae_flag_none))

  GWI_B(gwi_class_end(gwi))

  const struct Op_Func   opfunc0 = {.ck = opck_union_is};
  CHECK_B(add_op_func_check(gwi->gwion->env, t_union, &opfunc0, 0));

  const struct Op_Func   opfunc1 = {.ck = opck_union_new};
  CHECK_B(add_op_func_check(gwi->gwion->env, t_union, &opfunc1, 1));

   GWI_B(gwi_oper_ini(gwi, "Union", (m_str)OP_ANY_TYPE, NULL))
   GWI_B(gwi_oper_emi(gwi, opem_union_dot))
   GWI_B(gwi_oper_end(gwi, ".", NULL))

  GWI_B(gwi_union_ini(gwi, "Option:[A]"))
  GWI_B(gwi_union_add(gwi, "None", "none"))
  GWI_B(gwi_union_add(gwi, "A", "val"))
  GWI_B(gwi_union_end(gwi, ae_flag_none))

  return true;
}
