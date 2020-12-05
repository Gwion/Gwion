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
static inline m_bool nonnull_check(const Type l, const Type r) {
  return !tflag(l, tflag_nonnull) && tflag(r, tflag_nonnull);
}

ANN static inline Type check_nonnull(const Env env, const Type l, const Type r,
      const m_str action, const loc_t pos) {
  if(tflag(r, tflag_nonnull)) {
    if(isa(l, env->gwion->type[et_null]) > 0)
      ERR_N(pos, _("can't %s '%s' to '%s'"), action, l->name, r->name);
    if(isa(l, unflag_type(r)) < 0)
      ERR_N(pos, _("can't %s '%s' to '%s'"), action, l->name, r->name);
    return r->info->parent;
  }
  if(l != env->gwion->type[et_null] && isa(l, r) < 0)
    ERR_N(pos, _("can't %s '%s' to '%s'"), action, l->name, r->name);
  return r;
}

static OP_CHECK(at_object) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  const Type l = bin->lhs->info->type;
  const Type r = bin->rhs->info->type;
  if(opck_rassign(env, data, mut) == env->gwion->type[et_null])
    return env->gwion->type[et_null];
  if(check_nonnull(env, l, r, "assign", exp_self(bin)->pos) == env->gwion->type[et_null])
    return env->gwion->type[et_null];
  if(bin->rhs->exp_type == ae_exp_decl)
    SET_FLAG(bin->rhs->d.exp_decl.td, ref);
  exp_setvar(bin->rhs, 1);
  return r;
}

/*static*/ OP_EMIT(opem_at_object) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  const Type l = bin->lhs->info->type;
  const Type r = bin->rhs->info->type;

  const uint is_new = bin->lhs->exp_type == ae_exp_unary && bin->lhs->d.exp_unary.op == insert_symbol(emit->gwion->st, "new");
  if(bin->lhs->exp_type == ae_exp_unary && bin->lhs->d.exp_unary.op == insert_symbol(emit->gwion->st, "new")) {
    for(m_uint i = vector_size(&emit->code->instr); --i;) {
      const Instr instr = (Instr)vector_at(&emit->code->instr, i - 1);
      if(instr->opcode == GcAdd) {
        vector_rem(&emit->code->instr, i - 1);
        mp_free(emit->gwion->mp, Instr, instr);
      }
    }
  } else if(bin->lhs->exp_type != ae_exp_primary || bin->lhs->d.prim.prim_type != ae_prim_array) {
    const Instr instr = emit_add_instr(emit, RegAddRef);
    instr->m_val = -SZ_INT*2;
  }

  if(nonnull_check(l, r)) {
    const Instr instr = emit_add_instr(emit, GWOP_EXCEPT);
    instr->m_val = SZ_INT;
  }
  return emit_add_instr(emit, ObjectAssign);
}

static OP_CHECK(opck_object_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  const Type l = cast->exp->info->type;
  const Type r = exp_self(cast)->info->type;
  if(check_nonnull(env, l, r, "cast", exp_self(cast)->pos) == env->gwion->type[et_null])
    return env->gwion->type[et_null];
  return force_type(env, r);
}

static OP_EMIT(opem_object_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  const Type l = cast->exp->info->type;
  const Type r = exp_self(cast)->info->type;
  if(nonnull_check(l, r))
    emit_add_instr(emit, GWOP_EXCEPT);
  return (Instr)GW_OK;
}

static OP_CHECK(opck_implicit_null2obj) {
  const struct Implicit* imp = (struct Implicit*)data;
  const Type l = imp->e->info->type;
  const Type r = imp->t;
  if(check_nonnull(env, l, r, "implicitly cast", imp->e->pos) == env->gwion->type[et_null])
    return env->gwion->type[et_null];
  return imp->t;
}

static OP_EMIT(opem_implicit_null2obj) {
  const struct Implicit* imp = (struct Implicit*)data;
  const Type l = imp->e->info->type;
  const Type r = imp->t;
  if(nonnull_check(l, r))
    emit_add_instr(emit, GWOP_EXCEPT);
  return (Instr)GW_OK;
}

ANN /*static*/ Type scan_class(const Env env, const Type t, const Type_Decl* td);

static Type opck_object_scan(const Env env, const struct TemplateScan *ts) {
  if(ts->td->types)
    return scan_class(env, ts->t, ts->td) ?: env->gwion->type[et_null];
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
  if(is_class(emit->gwion, member->t_base) || tflag(member->base->info->type, tflag_force)) {
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
  if(!the_base->nspc)
    ERR_N(member->base->pos,
          _("type '%s' does not have members - invalid use in dot expression of %s"),
          the_base->name, str)
  if(member->xid ==  insert_symbol(env->gwion->st, "this") && base_static)
    ERR_N(exp_self(member)->pos,
          _("keyword 'this' must be associated with object instance..."))
  const Value value = find_value(the_base, member->xid);
  if(!value) {
    env_err(env, exp_self(member)->pos,
          _("class '%s' has no member '%s'"), the_base->name, str);
    if(member->t_base->nspc)
      did_you_mean_type(the_base, str);
    return env->gwion->type[et_null];
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
    if(isa(member->t_base, emit->env->gwion->type[et_object]) > 0)
      emit_except(emit, member->t_base);
  }
  if(isa(exp_self(member)->info->type, emit->gwion->type[et_function]) > 0 && !is_fptr(emit->gwion, exp_self(member)->info->type))
	  emit_member_func(emit, member);
  else if(vflag(value, vflag_member)) {
    if(!tflag(t_base, tflag_struct))
      emit_member(emit, value, exp_getvar(exp_self(member)));
    else {
//      exp_setvar(member->base, exp_getvar(exp_self(member)));
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

struct tmpl_info {
  Symbol           name;
  ID_List          list;
  Type_List        call;
  Type             ret;
  Type             base;
  struct Vector_   type;
  struct Vector_   size;
  uint8_t index;
};

ANN static inline size_t tmpl_set(struct tmpl_info* info, const Type t) {
  vector_add(&info->type, (vtype)t);
  const size_t len = strlen(t->name);
  vector_add(&info->size, len);
  return len;
}

ANN static ssize_t template_size(const Env env, struct tmpl_info* info) {
  ID_List base = info->list; // ???
  Type_List call = info->call;
  size_t size = 0;
  do {
    DECL_OB(const Type, t, = known_type(env, call->td))
    size += tmpl_set(info, t);
  } while((call = call->next) && (base = base->next) && ++size);
//  } while((call = call->next) && ++size);
  size += tmpl_set(info, info->base);
  return size + 4;
}

ANN static inline m_str tmpl_get(struct tmpl_info* info, m_str str) {
  const Type t = (Type)vector_at(&info->type, info->index);
  strcpy(str, t->name);
  return str += vector_at(&info->size, info->index);
}

ANN static void template_name(struct tmpl_info* info, m_str s) {
  m_str str = s;
  const m_uint size = info->index = vector_size(&info->type) -1;
  str = tmpl_get(info, str);
  *str++ = ':';
  *str++ = '[';
  for(info->index = 0; info->index < size; ++info->index) {
    str = tmpl_get(info, str);
    if(info->index < size - 1)
      *str++ = ',';
    else {
      *str++ = ']';
    }
  }
  *str = '\0';
}

ANEW ANN static Symbol template_id(const Env env, struct tmpl_info* info) {
  vector_init(&info->type);
  vector_init(&info->size);
  ssize_t sz = template_size(env, info);
  char name[sz];
  if(sz > GW_ERROR)
    template_name(info, name);
  vector_release(&info->type);
  vector_release(&info->size);
  return sz > GW_ERROR ? insert_symbol(env->gwion->st, name) : NULL;
}

ANN static m_bool template_match(ID_List base, Type_List call) {
  while((call = call->next) && (base = base->next));
  return !call ? GW_OK : GW_ERROR;
}

ANN static Type tmpl_exists(const Env env, const Symbol name) {
  if(env->class_def && name == insert_symbol(env->gwion->st, env->class_def->name))
     return env->class_def;
  return nspc_lookup_type1(env->curr, name);
}

ANN static m_bool scantmpl_class_def(const Env env, struct tmpl_info *info) {
  const Class_Def c = info->base->info->cdef;
  const Class_Def cdef = new_class_def(env->gwion->mp, c->flag, info->name, c->base.ext ? cpy_type_decl(env->gwion->mp, c->base.ext) : NULL,
      c->body ?cpy_ast(env->gwion->mp, c->body) : NULL,
    loc_cpy(env->gwion->mp, c->pos));
  cdef->cflag = c->cflag;
  cdef->base.tmpl = mk_tmpl(env, c->base.tmpl, info->call);
  const m_bool ret = scan0_class_def(env, cdef);
  if((info->ret = cdef->base.type)) {
    info->ret->info->cdef = cdef;
    set_tflag(info->ret, tflag_cdef);
    set_tflag(info->ret, tflag_ctmpl);
  } else
    free_class_def(env->gwion->mp, cdef);
  return ret;
}

ANN static m_bool scantmpl_union_def(const Env env, struct tmpl_info *info) {
  const Union_Def u = info->base->info->udef;
  const Union_Def udef = new_union_def(env->gwion->mp, cpy_decl_list(env->gwion->mp, u->l),
    loc_cpy(env->gwion->mp, u->pos));
  udef->type_xid = info->name;
  udef->tmpl = mk_tmpl(env, u->tmpl, info->call);
  if(GET_FLAG(info->base, global))
    SET_FLAG(udef, global);
  const m_bool ret = scan0_union_def(env, udef);
  if(udef->type) {
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

ANN Type scan_class(const Env env, const Type t, const Type_Decl* td) {
  if(template_match(t->info->cdef->base.tmpl->list, td->types) < 0) // invalid template
    ERR_O(td->pos, _("invalid template types number"))
  struct tmpl_info info = { .base=t, .call=td->types, .list=t->info->cdef->base.tmpl->list  };
  DECL_OO(const Symbol, name, = info.name = template_id(env, &info))
  const Type exists = tmpl_exists(env, name);
  if(exists)
    return exists;
  struct EnvSet es = { .env=env, .data=env, .func=(_exp_func)scan0_cdef,
    .scope=env->scope->depth, .flag=tflag_scan0 };
  CHECK_BO(envset_push(&es, t->info->owner_class, t->info->ctx ? t->info->ctx->nspc : env->curr))
  const Type ret = _scan_class(env, &info);
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
  const Type t_null  = gwi_mk_type(gwi, "@null",  SZ_INT, NULL);
  gwi->gwion->type[et_null] = t_null;
  GWI_BB(gwi_set_global_type(gwi, t_null, et_null))
  GWI_BB(gwi_oper_cond(gwi, "Object", BranchEqInt, BranchNeqInt))
  GWI_BB(gwi_oper_ini(gwi, "Object", "Object", NULL))
  GWI_BB(gwi_oper_add(gwi, at_object))
  GWI_BB(gwi_oper_emi(gwi, opem_at_object))
  GWI_BB(gwi_oper_end(gwi, "@=>", NULL))
  GWI_BB(gwi_oper_ini(gwi, "@null", "Object", NULL))
  GWI_BB(gwi_oper_add(gwi, at_object))
  GWI_BB(gwi_oper_emi(gwi, opem_at_object))
  GWI_BB(gwi_oper_end(gwi, "@=>", NULL))
  GWI_BB(gwi_oper_ini(gwi, "Object", "Object", "int"))
  GWI_BB(gwi_oper_end(gwi, "==",  EqObject))
  GWI_BB(gwi_oper_end(gwi, "!=", NeqObject))
  GWI_BB(gwi_oper_add(gwi, opck_object_cast))
  GWI_BB(gwi_oper_emi(gwi, opem_object_cast))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_implicit_null2obj))
  GWI_BB(gwi_oper_emi(gwi, opem_implicit_null2obj))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
  GWI_BB(gwi_oper_ini(gwi, "@null", "Object", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_implicit_null2obj))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
  GWI_BB(gwi_oper_ini(gwi, "@null", "Object", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_object_cast))
  GWI_BB(gwi_oper_emi(gwi, opem_object_cast))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
  GWI_BB(gwi_oper_ini(gwi, NULL, "Object", "bool"))
  GWI_BB(gwi_oper_add(gwi, opck_unary_meta2))
  GWI_BB(gwi_oper_end(gwi, "!", IntNot))
  GWI_BB(gwi_oper_ini(gwi, "@Compound", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_struct_scan))
  GWI_BB(gwi_oper_end(gwi, "@scan", NULL))
  gwi_item_ini(gwi, "@null", "null");
  gwi_item_end(gwi, 0, NULL);
  return GW_OK;
}
