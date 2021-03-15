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
#include "gwi.h"
#include "tmpl_info.h"

#undef insert_symbol

#define describe_logical(name, op)               \
static INSTR(name##Object) {                     \
  POP_REG(shred, SZ_INT);                        \
  const M_Object lhs = *(M_Object*)REG(-SZ_INT); \
  const M_Object rhs = *(M_Object*)REG(0);       \
  *(m_uint*)REG(-SZ_INT) = (lhs op rhs);         \
}

describe_logical(Eq,  ==)
describe_logical(Neq, !=)

static OP_CHECK(opck_object_at) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  if(opck_rassign(env, data) == env->gwion->type[et_error])
    return env->gwion->type[et_error];
  if(bin->rhs->exp_type == ae_exp_decl)
    SET_FLAG(bin->rhs->d.exp_decl.list->self->value, late);
  exp_setvar(bin->rhs, 1);
  CHECK_BN(isa(bin->lhs->type , bin->rhs->type))
  return bin->rhs->type;
}

static OP_EMIT(opem_object_at) {
  const Instr addref = emit_add_instr(emit, RegAddRef);
  addref->m_val = -SZ_INT*2;
  (void)emit_add_instr(emit, ObjectAssign);
  return GW_OK;
}

static OP_CHECK(opck_object_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  const Type to = known_type(env, cast->td);
  if(isa(cast->exp->type, to) < 0) {
    if(isa(to, cast->exp->type) > 0)
      ERR_N(exp_self(cast)->pos, _("can't upcast '%s' to '%s'"), cast->exp->type->name, to->name)
    ERR_N(exp_self(cast)->pos, _("can't cast '%s' to '%s'"), cast->exp->type->name, to->name)
  }
  return exp_self(cast)->type;
}

ANN /*static*/ Type scan_class(const Env env, const Type t, const Type_Decl * td);

static Type opck_object_scan(const Env env, const struct TemplateScan *ts) {
  if(ts->td->types)
    return scan_class(env, ts->t, ts->td) ?: env->gwion->type[et_error];
  Type_Decl *td = (Type_Decl*)ts->td;
  ERR_N(td->pos, _("you must provide template types for type '%s'"), ts->t->name)
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
//  if(v->d.ptr && vflag(v, vflag_builtin) && GET_FLAG(v, const)) {
  if(vflag(v, vflag_builtin) && GET_FLAG(v, const)) {
    const m_uint size = v->type->size;
    const Instr instr = emit_kind(emit, size, emit_addr, regpushimm);
    instr->m_val = (m_uint)v->d.ptr;
    instr->m_val2 = size;
  } else
    emit_dot_static_data(emit, v, emit_addr);
}
static const f_instr dotmember[]  = { DotMember, DotMember2, DotMember3, DotMember4 };

ANN static void emit_member_func(const Emitter emit, const Exp_Dot* member) {
  const Func f = exp_self(member)->type->info->func;
  if(f->def->base->tmpl)
    emit_add_instr(emit, DotTmplVal);
  else if(GET_FLAG(member->base->type, final) || is_class(emit->gwion, member->base->type) || member->base->exp_type == ae_exp_cast) {
    const Instr func_i = emit_add_instr(emit, f->code ? RegPushImm : SetFunc);
    func_i->m_val = (m_uint)f->code ?: (m_uint)f;
    return;
  }
//  if(f->def->base->tmpl)
//    emit_add_instr(emit, DotTmplVal);
  else {
    if(tflag(member->base->type, tflag_struct)) {
      if(!GET_FLAG(f->def->base, static)) {
        exp_setvar(member->base, 1);
        emit_exp(emit, member->base);
      }
      const Instr func_i = emit_add_instr(emit, f->code ? RegPushImm : SetFunc);
      func_i->m_val = (m_uint)f->code ?: (m_uint)f;
      return;
    }
    const Instr instr = emit_add_instr(emit, DotFunc);
    instr->m_val = f->vt_index;
    if(!vflag(f->value_ref, vflag_member))
      instr->m_val2 = -SZ_INT;
    else {
      const Instr instr = emit_add_instr(emit, RegMove);
      instr->m_val = SZ_INT;
    }
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
    const Instr instr = emit_add_instr(emit, RegMove);
    instr->m_val = v->type->size -SZ_INT;
  }
}

ANN m_bool not_from_owner_class(const Env env, const Type t, const Value v, const loc_t pos);

ANN static inline Value get_value(const Env env, const Exp_Dot *member, const Type t) {
  const Value value = find_value(t, member->xid);
  if(value)
    return value;
  if(env->func && env->func->def->base->values)
    return (Value)scope_lookup1(env->func->def->base->values, (m_uint)member->xid);
  return NULL;
}

OP_CHECK(opck_object_dot) {
  const Exp_Dot *member = (Exp_Dot*)data;
  const m_str str = s_name(member->xid);
  const m_bool base_static = is_class(env->gwion, member->base->type);
  const Type the_base = base_static ? _class_base(member->base->type) : member->base->type;
//  if(!the_base->nspc)
//    ERR_N(&member->base->pos,
//          _("type '%s' does not have members - invalid use in dot expression of %s"),
//          the_base->name, str)
  if(member->xid ==  insert_symbol(env->gwion->st, "this") && base_static)
    ERR_N(exp_self(member)->pos,
          _("keyword 'this' must be associated with object instance..."))
  const Value value = get_value(env, member, the_base);
  if(!value) {
    const Value v = nspc_lookup_value1(env->curr, member->xid);
    if(v && isa(v->type, env->gwion->type[et_function]) > 0)
      return v->type;
    env_err(env, exp_self(member)->pos,
          _("class '%s' has no member '%s'"), the_base->name, str);
    if(member->base->type->nspc)
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
  const Type t_base = actual_type(emit->gwion, member->base->type);
  const Value value = find_value(t_base, member->xid);
  if(!is_class(emit->gwion, member->base->type) && (vflag(value, vflag_member) ||
       (isa(exp_self(member)->type, emit->gwion->type[et_function]) > 0 &&
       !is_fptr(emit->gwion, exp_self(member)->type)))) {
    if(!tflag(t_base, tflag_struct))
      CHECK_BB(emit_exp(emit, member->base))
  }
  if(isa(exp_self(member)->type, emit->gwion->type[et_function]) > 0 && !is_fptr(emit->gwion, exp_self(member)->type))
	  emit_member_func(emit, member);
  else if(vflag(value, vflag_member)) {
    if(!tflag(t_base, tflag_struct))
      emit_member(emit, value, exp_getvar(exp_self(member)));
    else {
      exp_setvar(member->base, 1);
      CHECK_BB(emit_exp(emit, member->base))
      emit_struct_data(emit, value, exp_getvar(exp_self(member)));
    }
  } else if(GET_FLAG(value, static))
    emit_dot_static_import_data(emit, value, exp_getvar(exp_self(member)));
  else { // member type
    const Instr instr = emit_add_instr(emit, RegPushImm);
    instr->m_val = (m_uint)value->type;
  }
  if(GET_FLAG(value, late) && !exp_getvar(exp_self(member))) {
    const Instr instr = emit_add_instr(emit, GWOP_EXCEPT);
    instr->m_val = -SZ_INT;
  }
  return GW_OK;
}

ANN static m_bool scantmpl_class_def(const Env env, struct tmpl_info *info) {
  const Class_Def c = info->base->info->cdef;
  const Class_Def cdef = new_class_def(env->gwion->mp, c->flag, info->name, c->base.ext ? cpy_type_decl(env->gwion->mp, c->base.ext) : NULL,
      c->body ? cpy_ast(env->gwion->mp, c->body) : NULL, c->pos);
  cdef->cflag = c->cflag;
  cdef->base.tmpl = mk_tmpl(env, c->base.tmpl, info->td->types);
  const m_bool ret = scan0_class_def(env, cdef);
  if((info->ret = cdef->base.type)) {
    info->ret->info->cdef = cdef;
    set_tflag(info->ret, tflag_cdef);
  } else
    free_class_def(env->gwion->mp, cdef);
  return ret;
}

ANN static m_bool scantmpl_union_def(const Env env, struct tmpl_info *info) {
  const Union_Def u = info->base->info->udef;
  const Union_Def udef = new_union_def(env->gwion->mp, cpy_union_list(env->gwion->mp, u->l), u->pos);
  udef->xid = info->name;
  udef->tmpl = mk_tmpl(env, u->tmpl, info->td->types);
  if(GET_FLAG(info->base, global))
    SET_FLAG(udef, global);
  const m_bool ret = scan0_union_def(env, udef);
  if(udef->type) {
    udef->type->info->udef = udef;// mark as udef
    info->ret = udef->type;
    set_tflag(info->ret, tflag_udef);
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
  struct tmpl_info info = { .base=t, .td=td, .list=t->info->cdef->base.tmpl->list };
  const Type exists = tmpl_exists(env, &info);
  if(exists)
    return exists != env->gwion->type[et_error] ? exists : NULL;
  struct EnvSet es = { .env=env, .data=env, .func=(_exp_func)scan0_cdef,
    .scope=env->scope->depth, .flag=tflag_scan0 };
  const Type _t = known_type(env, td->types->td);
  CHECK_BO(envset_push(&es, _t->info->owner_class, _t->info->owner))
  const Type ret = _scan_class(env, &info);
  if(es.run)
    envset_pop(&es, _t->info->owner_class);
  return ret;
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

static OP_EMIT(opem_not_object) {
  const Vector v = &emit->code->instr;
  const Instr back = (Instr)vector_back(v);
  if(back->opcode == eGWOP_EXCEPT) {
    vector_pop(v);
    mp_free(emit->gwion->mp, Instr, back);
    emit_add_instr(emit, IntNot);
    return GW_OK;
  }
  const Instr instr = emit_add_instr(emit, RegSetImm);
  instr->m_val2 = -SZ_INT;
  return GW_OK;
}

GWION_IMPORT(object_op) {
  const Type t_error  = gwi_mk_type(gwi, "@error",  0, NULL);
  gwi->gwion->type[et_error] = t_error;
  GWI_BB(gwi_set_global_type(gwi, t_error, et_error))
  GWI_BB(gwi_oper_ini(gwi, "Object", "Object", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_object_at))
  GWI_BB(gwi_oper_emi(gwi, opem_object_at))
  GWI_BB(gwi_oper_end(gwi, "=>", NULL))
  GWI_BB(gwi_oper_ini(gwi, "Object", "Object", "bool"))
  GWI_BB(gwi_oper_end(gwi, "==",  EqObject))
  GWI_BB(gwi_oper_end(gwi, "!=", NeqObject))
  GWI_BB(gwi_oper_add(gwi, opck_object_cast))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
//.  GWI_BB(gwi_oper_add(gwi, opck_implicit_null2obj))
//  GWI_BB(gwi_oper_emi(gwi, opem_implicit_null2obj))
//  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
  GWI_BB(gwi_oper_ini(gwi, NULL, "Object", "bool"))
  GWI_BB(gwi_oper_emi(gwi, opem_not_object))
  GWI_BB(gwi_oper_end(gwi, "!", IntNot))
  GWI_BB(gwi_oper_ini(gwi, "@Compound", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_struct_scan))
  GWI_BB(gwi_oper_end(gwi, "@scan", NULL))

  return GW_OK;
}
