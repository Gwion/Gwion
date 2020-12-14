#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "emit.h"
#include "traverse.h"
#include "template.h"
#include "parse.h"
#include "specialid.h"
#include "gwi.h"
#include "tmpl_info.h"

#undef insert_symbol

#define describe_logical(name, op)               \
static INSTR(name##Object) {                     \
  POP_REG(shred, SZ_INT);                        \
  const M_Object lhs = *(M_Object*)REG(-SZ_INT); \
  const M_Object rhs = *(M_Object*)REG(0);       \
  *(m_uint*)REG(-SZ_INT) = (lhs op rhs);         \
  release(lhs, shred);                           \
  release(rhs, shred);                           \
}

describe_logical(Eq,  ==)
describe_logical(Neq, !=)

static OP_CHECK(at_object) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  if(opck_rassign(env, data, mut) == env->gwion->type[et_error])
    return env->gwion->type[et_error];
  if(bin->rhs->exp_type == ae_exp_decl)
    SET_FLAG(bin->rhs->d.exp_decl.td, ref);
  exp_setvar(bin->rhs, 1);
  CHECK_BO(isa(bin->lhs->info->type , bin->rhs->info->type))
  return bin->rhs->info->type;
}

static OP_CHECK(opck_object_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  const Type to = known_type(env, cast->td);
  if(isa(cast->exp->info->type, to) < 0) {
    if(isa(to, cast->exp->info->type) > 0)
      ERR_N(exp_self(cast)->pos, _("can't upcast '%s' to '%s'"), cast->exp->info->type->name, to->name)
    ERR_N(exp_self(cast)->pos, _("can't cast '%s' to '%s'"), cast->exp->info->type->name, to->name)
  }
  return exp_self(cast)->info->type;
}

static OP_CHECK(opck_implicit_null2obj) {
  const struct Implicit* imp = (struct Implicit*)data;
  return imp->t;
}

static OP_EMIT(opem_implicit_null2obj) {
  return (Instr)GW_OK;
}

ANN /*static*/ Type scan_class(const Env env, const Type t, const Type_Decl * td);

static Type opck_object_scan(const Env env, const struct TemplateScan *ts) {
  if(ts->td->types)
    return scan_class(env, ts->t, ts->td) ?: env->gwion->type[et_error];
  ERR_N(td_pos(ts->td), _("you must provide template types for type '%s'"), ts->t->name)
}

static OP_CHECK(opck_struct_scan) {
  struct TemplateScan *ts = (struct TemplateScan*)data;
  return opck_object_scan(env, ts);
}

static const f_instr dotstatic[]  = { DotStatic, DotStatic2, DotStatic3, RegPushImm };
static const f_instr structmember[]  = { StructMember, StructMemberFloat, StructMemberOther, StructMemberAddr };

ANN Instr emit_kind(Emitter emit, const m_uint size, const uint addr, const f_instr func[]);
ANN static void emit_dot_static_data(const Emitter emit, const Value v, const uint emit_var) {
  const m_uint size = v->type->size;
  const Instr instr = emit_kind(emit, size, emit_var, dotstatic);
  instr->m_val = (m_uint)(v->from->owner->info->class_data + v->from->offset);
  instr->m_val2 = size;
}

static const f_instr regpushimm[] = { RegPushImm, RegPushImm2, RegPushImm3, RegPushImm4 };
ANN static void emit_dot_static_import_data(const Emitter emit, const Value v, const uint emit_addr) {
  if(v->d.ptr && vflag(v, vflag_builtin) && GET_FLAG(v, const)) {
    const m_uint size = v->type->size;
    const Instr instr = emit_kind(emit, size, emit_addr, regpushimm);
    instr->m_val = (m_uint)v->d.ptr;
    instr->m_val2 = size;
  } else
    emit_dot_static_data(emit, v, emit_addr);
}
static const f_instr dotmember[]  = { DotMember, DotMember2, DotMember3, DotMember4 };

ANN static void emit_member_func(const Emitter emit, const Exp_Dot* member) {
  const Func f = exp_self(member)->info->type->info->func;
  if(f->def->base->tmpl)
    emit_add_instr(emit, DotTmplVal);
else
  if(is_class(emit->gwion, member->t_base) || member->base->exp_type == ae_exp_cast) {
    const Instr func_i = emit_add_instr(emit, f->code ? RegPushImm : SetFunc);
    func_i->m_val = (m_uint)f->code ?: (m_uint)f;
    return;
  }
//  if(f->def->base->tmpl)
//    emit_add_instr(emit, DotTmplVal);
  else {
    if(tflag(member->t_base, tflag_struct)) {
      if(!GET_FLAG(f->def->base, static)) {
        exp_setvar(member->base, 1);
        emit_exp(emit, member->base);
      }
    const Instr func_i = emit_add_instr(emit, f->code ? RegPushImm : SetFunc);
    func_i->m_val = (m_uint)f->code ?: (m_uint)f;
      return;
    }
    const Instr instr = emit_add_instr(emit, vflag(f->value_ref, vflag_member) ? DotFunc : DotStaticFunc);
    instr->m_val = f->vt_index;
  }
  return;
}

ANN static inline void emit_member(const Emitter emit, const Value v, const uint emit_addr) {
  const m_uint size = v->type->size;
  const Instr instr = emit_kind(emit, size, emit_addr, dotmember);
  instr->m_val = v->from->offset;
  instr->m_val2 = size;
}

ANN static inline void emit_struct_data(const Emitter emit, const Value v, const uint emit_addr) {
  const Instr instr = emit_kind(emit, v->type->size, emit_addr, structmember);
  instr->m_val = v->from->offset;
  if(!emit_addr) {
    const Instr instr = emit_add_instr(emit, RegPush);
    instr->m_val = v->type->size -SZ_INT;
  }
}

ANN m_bool not_from_owner_class(const Env env, const Type t, const Value v, const loc_t pos);
OP_CHECK(opck_object_dot) {
  const Exp_Dot *member = (Exp_Dot*)data;
  const m_str str = s_name(member->xid);
  const m_bool base_static = is_class(env->gwion, member->t_base);
  const Type the_base = base_static ? member->t_base->info->base_type : member->t_base;
//  if(!the_base->nspc)
//    ERR_N(member->base->pos,
//          _("type '%s' does not have members - invalid use in dot expression of %s"),
//          the_base->name, str)
  if(member->xid ==  insert_symbol(env->gwion->st, "this") && base_static)
    ERR_N(exp_self(member)->pos,
          _("keyword 'this' must be associated with object instance..."))
  const Value value = find_value(the_base, member->xid);
  if(!value) {
    env_err(env, exp_self(member)->pos,
          _("class '%s' has no member '%s'"), the_base->name, str);
    if(member->t_base->nspc)
      did_you_mean_type(the_base, str);
    return env->gwion->type[et_error];
  }
  CHECK_BN(not_from_owner_class(env, the_base, value, exp_self(member)->pos))
  if(!env->class_def || isa(env->class_def, value->from->owner_class) < 0) {
    if(GET_FLAG(value, private))
      ERR_N(exp_self(member)->pos,
          _("can't access private '%s' outside of class..."), value->name)
    else if(GET_FLAG(value, protect))
      exp_setprot(exp_self(member), 1);
  }
  if(base_static && vflag(value, vflag_member))
    ERR_N(exp_self(member)->pos,
          _("cannot access member '%s.%s' without object instance..."),
          the_base->name, str)
  if(GET_FLAG(value, const))
    exp_setmeta(exp_self(member), 1);
  return value->type;
}

OP_EMIT(opem_object_dot) {
  const Exp_Dot *member = (Exp_Dot*)data;
  const Type t_base = actual_type(emit->gwion, member->t_base);
  const Value value = find_value(t_base, member->xid);
  if(!is_class(emit->gwion, member->t_base) && (vflag(value, vflag_member) ||
       (isa(exp_self(member)->info->type, emit->gwion->type[et_function]) > 0 &&
       !is_fptr(emit->gwion, exp_self(member)->info->type)))) {
    if(!tflag(t_base, tflag_struct))
      CHECK_BO(emit_exp(emit, member->base))
  }
  if(isa(exp_self(member)->info->type, emit->gwion->type[et_function]) > 0 && !is_fptr(emit->gwion, exp_self(member)->info->type))
	  emit_member_func(emit, member);
  else if(vflag(value, vflag_member)) {
    if(!tflag(t_base, tflag_struct))
      emit_member(emit, value, exp_getvar(exp_self(member)));
    else {
      exp_setvar(member->base, 1);
      CHECK_BO(emit_exp(emit, member->base))
      emit_struct_data(emit, value, exp_getvar(exp_self(member)));
    }
  } else if(GET_FLAG(value, static))
    emit_dot_static_import_data(emit, value, exp_getvar(exp_self(member)));
  else { // member type
    const Instr instr = emit_add_instr(emit, RegPushImm);
    instr->m_val = (m_uint)value->type;
  }
  return (Instr)GW_OK;
}

ANN static m_bool scantmpl_class_def(const Env env, struct tmpl_info *info) {
  const Class_Def c = info->base->info->cdef;
  const Class_Def cdef = new_class_def(env->gwion->mp, c->flag, info->name, c->base.ext ? cpy_type_decl(env->gwion->mp, c->base.ext) : NULL,
      c->body ?cpy_ast(env->gwion->mp, c->body) : NULL,
    loc_cpy(env->gwion->mp, c->pos));
  cdef->cflag = c->cflag;
  cdef->base.tmpl = mk_tmpl(env, c->base.tmpl, info->td->types);
  const m_bool ret = scan0_class_def(env, cdef);
  if((info->ret = cdef->base.type)) {
    info->ret->info->cdef = cdef;
    set_tflag(info->ret, tflag_cdef);
    set_tflag(info->ret, tflag_ctmpl);
  } else
    free_class_def(env->gwion->mp, cdef);
  return ret;
}
/*
ANN static OP_CHECK(opck_option_get) {
  Exp_Binary *bin = (Exp_Binary*)data;
  exp_setvar(bin->rhs, 1);
  return bin->rhs->info->type;
}

static INSTR(OptionCheck) {
  const M_Object o = *(M_Object*)REG(-SZ_INT*2);
  if(*(m_uint*)(o->data) != instr->m_val2)
    Except(shred, "invalid union access");
  memcpy(*(m_bit**)REG(-SZ_INT), o->data + SZ_INT, instr->m_val);
  POP_REG(shred, SZ_INT*2 - instr->m_val);
  memcpy(&*(m_bit*)REG(-instr->m_val), o->data + SZ_INT, instr->m_val);
}

ANN static OP_EMIT(opem_option_get) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Instr instr = emit_add_instr(emit, OptionCheck);
  instr->m_val = bin->rhs->info->type->size;
  instr->m_val2 = 1;
  return instr;
}

ANN static OP_CHECK(opck_option_set) {
  Exp_Binary *bin = (Exp_Binary*)data;
  CHECK_NN(opck_rassign(env, data, mut)) // check those two lines
  exp_setvar(bin->rhs, 0);
  const Type rhs = bin->rhs->info->type;
  const Nspc nspc = bin->lhs->info->type->nspc;
  for(m_uint i = 0; i < nspc->info->class_data_size; i += SZ_INT) {
    if(rhs == *(Type*)(nspc->info->class_data + i))
      return rhs;
  }
//  ERR_N(exp_self(data)->pos, _(
//  const Value v = nspc_lookup_value0(bin->rhs->info->type->nspc, insert_symbol(env->gwion->st, "@val"));
//  return v->type;
}

static INSTR(OptionSet) {
  M_Object o = *(M_Object*)REG(-SZ_INT);
  *(m_uint*)(o->data) = instr->m_val2;
  *(m_bit**)(o->data + SZ_INT) = *(void**)REG(-instr->m_val);
}

ANN static OP_EMIT(opem_option_set) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Value v = nspc_lookup_value0(bin->rhs->info->type->nspc, insert_symbol(emit->gwion->st, "@val"));
  const Instr set = emit_add_instr(emit, OptionSet);
  set->m_val = SZ_INT + v->type->size;
  set->m_val2 = 1;
  const Instr pop = emit_add_instr(emit, RegPop);
  pop->m_val = SZ_INT;
  return set;
}

ANN static OP_CHECK(opck_option_setn) {
  Exp_Binary *bin = (Exp_Binary*)data;
  CHECK_NN(opck_rassign(env, data, mut))
  exp_setvar(bin->rhs, 1);
  return env->gwion->type[et_void];
}

ANN static OP_EMIT(opem_option_setn) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Instr instr = emit_add_instr(emit, OptionSet);
  instr->m_val = bin->rhs->info->type->nspc->info->offset;
  const Instr  pop2 = emit_add_instr(emit, RegPop);
  pop2->m_val = SZ_INT * 2;
  return instr;
}

ANN static OP_EMIT(opem_option_not) {
  Exp_Unary *unary = (Exp_Unary*)data;
  const Instr pop = emit_add_instr(emit, RegPop);
  pop->m_val = unary->exp->info->type->size - SZ_INT;
  return emit_add_instr(emit, IntNot);
}

ANN static OP_EMIT(opem_option_cond) {
  Exp exp = (Exp)data;
  const Instr pop = emit_add_instr(emit, RegPop);
  pop->m_val = exp->info->type->size - SZ_INT;
  return emit_add_instr(emit, BranchEqInt);
}

ANN static OP_EMIT(opem_option_uncond) {
  Exp exp = (Exp)data;
  const Instr pop = emit_add_instr(emit, RegPop);
  pop->m_val = exp->info->type->size - SZ_INT;
  return emit_add_instr(emit, BranchNeqInt);
}
*/

ANN static m_bool scantmpl_union_def(const Env env, struct tmpl_info *info) {
  const Union_Def u = info->base->info->udef;
  const Union_Def udef = new_union_def(env->gwion->mp, cpy_union_list(env->gwion->mp, u->l),
    loc_cpy(env->gwion->mp, u->pos));
  udef->xid = info->name;
  udef->tmpl = mk_tmpl(env, u->tmpl, info->td->types);
  if(GET_FLAG(info->base, global))
    SET_FLAG(udef, global);
  const m_bool ret = scan0_union_def(env, udef);
  if(udef->type) {
/*
    if(!strcmp(info->base->name, "Option")) {
      const Type base = known_type(env, info->td->types->td);
      const m_uint scope = env_push(env, udef->type->info->parent->info->owner_class, udef->type->info->parent->info->owner);
      const struct Op_Func opfunc0 = { .ck=opck_option_get, .em=opem_option_get };
      struct Op_Import opi0 = { .lhs=udef->type, .rhs=base, .ret=base,
       .op=insert_symbol(env->gwion->st, "?=>"), .pos=info->td->pos, .func=&opfunc0 };
      CHECK_BB(add_op(env->gwion, &opi0))
      const struct Op_Func opfunc1 = { .ck=opck_option_set, .em=opem_option_set };
      struct Op_Import opi1 = { .lhs=base, .rhs=udef->type, .ret=udef->type, .func=&opfunc1,
       .op=insert_symbol(env->gwion->st, "@=>"), .pos=info->td->pos };
      CHECK_BB(add_op(env->gwion, &opi1))
      const struct Op_Func opfunc2 = { .ck=opck_option_setn, .em=opem_option_setn };
      struct Op_Import opi2 = { .lhs=env->gwion->type[et_error], .rhs=udef->type, .ret=udef->type, .func=&opfunc2,
       .op=insert_symbol(env->gwion->st, "@=>"), .pos=info->td->pos };
      CHECK_BB(add_op(env->gwion, &opi2))
      const struct Op_Func opfunc3 = { .em=opem_option_not };
      struct Op_Import opi3 = { .rhs=udef->type, .ret=env->gwion->type[et_bool], .func=&opfunc3,
       .op=insert_symbol(env->gwion->st, "!"), .pos=info->td->pos };
      CHECK_BB(add_op(env->gwion, &opi3))
      const struct Op_Func opfunc4 = { .em=opem_option_cond };
      struct Op_Import opi4 = { .rhs=udef->type, .ret=env->gwion->type[et_bool], .func=&opfunc4,
       .op=insert_symbol(env->gwion->st, "@conditionnal"), .pos=info->td->pos };
      CHECK_BB(add_op(env->gwion, &opi4))
      const struct Op_Func opfunc5 = { .em=opem_option_uncond };
      struct Op_Import opi5 = { .rhs=udef->type, .ret=env->gwion->type[et_bool], .func=&opfunc5,
       .op=insert_symbol(env->gwion->st, "@unconditionnal"), .pos=info->td->pos };
      CHECK_BB(add_op(env->gwion, &opi5))
      env_pop(env, scope);
    }
*/
    udef->type->info->udef = udef;// mark as udef
    info->ret = udef->type;
    set_tflag(info->ret, tflag_udef);
//    set_tflag(info->ret, tflag_tmpl);
  } else
    free_union_def(env->gwion->mp, udef);
  return ret;
}

ANN static Type _scan_class(const Env env, struct tmpl_info *info) {
  if(info->base->info->parent !=  env->gwion->type[et_union])
    CHECK_BO(scantmpl_class_def(env, info))
  else
    CHECK_BO(scantmpl_union_def(env, info))
  return info->ret;
}

ANN Type tmpl_exists(const Env env, struct tmpl_info *const info);
ANN Type scan_class(const Env env, const Type t, const Type_Decl *td) {
  struct tmpl_info info = { .base=t, .td=td, .list=t->info->cdef->base.tmpl->list  };
  const Type exists = tmpl_exists(env, &info);
  if(exists)
    return exists != env->gwion->type[et_error] ? exists : NULL;
  struct EnvSet es = { .env=env, .data=env, .func=(_exp_func)scan0_cdef,
    .scope=env->scope->depth, .flag=tflag_scan0 };
//  CHECK_BO(envset_push(&es, t->info->owner_class, t->info->ctx ? t->info->ctx->nspc : env->curr))
//const Nspc former = env->curr;
//if(env->class_def)
//const Type cdef = env->class_def;
  CHECK_BO(envset_push(&es, t->info->owner_class, t->info->ctx ? t->info->ctx->nspc : env->curr))
//  CHECK_BO(envset_push(&es, t->info->owner_class, t->info->owner))
/*{
nspc_push_type(env->gwion->mp, env->curr);
  Type_List tl = td->types;
  do {
    nspc_add_type(env->curr, tl->td->xid, nspc_lookup_type0(former, tl->td->xid));
  } while((tl = tl->next));
}*/
  const Type ret = _scan_class(env, &info);
//nspc_pop_type(env->gwion->mp, env->curr);
  if(es.run)
    envset_pop(&es, t->info->owner_class);
  return ret;
}

ANN static inline Symbol dot_symbol(SymTable *st, const Value v) {
  const m_str name = !GET_FLAG(v, static) ? "this" : v->from->owner_class->name;
  return insert_symbol(st, name);
}

ANN Exp symbol_owned_exp(const Gwion gwion, const Symbol *data) {
  const Value v = prim_self(data)->value;
  const Exp base = new_prim_id(gwion->mp, dot_symbol(gwion->st, v), loc_cpy(gwion->mp, prim_pos(data)));
  const Exp dot = new_exp_dot(gwion->mp, base, *data, loc_cpy(gwion->mp, prim_pos(data)));
  const Type owner = v->from->owner_class;
  dot->d.exp_dot.t_base = dot->d.exp_dot.base->info->type = !GET_FLAG(v, static) ?
    owner : type_class(gwion, owner);
  dot->info->type = prim_exp(data)->info->type;
  exp_setvar(dot, exp_getvar(prim_exp(data)));
  return dot;
}

ANN void struct_release(const VM_Shred shred, const Type base, const m_bit *ptr) {
  const Vector types   = &base->info->tuple->types;
  const Vector offsets = &base->info->tuple->offset;
  for(m_uint i = 0; i < vector_size(types); ++i) {
    const Type t = (Type)vector_at(types, i);
    if(isa(t, shred->info->vm->gwion->type[et_compound]) < 0)
      continue;
    const m_uint offset = vector_at(offsets, i);
    if(!tflag(t, tflag_struct))
      release(*(M_Object*)(ptr + offset), shred);
    else
      struct_release(shred, t, *(m_bit**)(ptr + offset));
  }
}

GWION_IMPORT(object_op) {
  const Type t_error  = gwi_mk_type(gwi, "@error",  0, NULL);
  gwi->gwion->type[et_error] = t_error;
  GWI_BB(gwi_set_global_type(gwi, t_error, et_error))
  GWI_BB(gwi_oper_ini(gwi, "Object", "Object", NULL))
  GWI_BB(gwi_oper_add(gwi, at_object))
  GWI_BB(gwi_oper_end(gwi, "@=>", ObjectAssign))
  GWI_BB(gwi_oper_ini(gwi, "Object", "Object", "int"))
  GWI_BB(gwi_oper_end(gwi, "==",  EqObject))
  GWI_BB(gwi_oper_end(gwi, "!=", NeqObject))
  GWI_BB(gwi_oper_add(gwi, opck_object_cast))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_implicit_null2obj))
  GWI_BB(gwi_oper_emi(gwi, opem_implicit_null2obj))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
  GWI_BB(gwi_oper_ini(gwi, "@Compound", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_struct_scan))
  GWI_BB(gwi_oper_end(gwi, "@scan", NULL))
  return GW_OK;
}
