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
#include "sema.h"

#undef insert_symbol

#define describe_logical(name, op)                                             \
  static INSTR(name##Object) {                                                 \
    POP_REG(shred, SZ_INT);                                                    \
    const M_Object lhs      = *(M_Object *)REG(-SZ_INT);                       \
    const M_Object rhs      = *(M_Object *)REG(0);                             \
    *(m_uint *)REG(-SZ_INT) = (lhs op rhs);                                    \
  }

describe_logical(Eq, ==) describe_logical(Neq, !=);

static OP_CHECK(opck_object_at) {
  const Exp_Binary *bin = (Exp_Binary *)data;
  if (opck_rassign(env, data) == env->gwion->type[et_error])
    return env->gwion->type[et_error];
  if (bin->rhs->exp_type == ae_exp_decl) {
    Var_Decl vd = bin->rhs->d.exp_decl.var.vd;
    SET_FLAG(vd.value, late);
  }
  exp_setvar(bin->rhs, 1);
  CHECK_O(isa(bin->lhs->type, bin->rhs->type));
  bin->rhs->ref = bin->lhs;
  return bin->rhs->type;
}

static OP_CHECK(opck_object_instance) {
  Exp_Binary *bin = (Exp_Binary*)data;
  Exp* rhs = bin->rhs;
  if (rhs->exp_type != ae_exp_decl)
    return NULL;
  if (rhs->d.exp_decl.var.td->array)
    return NULL;
  Exp* lhs = bin->lhs;
  Exp* e = exp_self(bin);
  Exp_Decl *const decl = &e->d.exp_decl;
  e->exp_type = ae_exp_decl;
  decl->var.td = cpy_type_decl(env->gwion->mp, rhs->d.exp_decl.var.td);
  decl->var.vd = rhs->d.exp_decl.var.vd;
  decl->type = rhs->type;
  decl->args = lhs;
  free_exp(env->gwion->mp, rhs);
  CHECK_ON(check_exp(env, e));
  return e->type;
}

ANN void unset_local(const Emitter emit, void *const l);
static OP_EMIT(opem_object_at) {
  const Exp_Binary *bin = (Exp_Binary *)data;
  if(!bin->rhs->data) {
    const Instr addref = emit_add_instr(emit, RegAddRef);
    addref->m_val      = -SZ_INT * 2;
  } else if(bin->rhs->data != (void*)-1)
    unset_local(emit, bin->rhs->data);
  if (bin->rhs->exp_type != ae_exp_decl)
    (void)emit_add_instr(emit, ObjectAssign);
  else
    (void)emit_add_instr(emit, Assign);
  return true;
}

static OP_CHECK(opck_object_cast) {
  const Exp_Cast *cast = (Exp_Cast *)data;
  const Type      to   = known_type(env, cast->td);
  if (!isa(cast->exp->type, to)) {
    if (isa(to, cast->exp->type))
      ERR_N(exp_self(cast)->loc, _("can't upcast '%s' to '%s'"),
            cast->exp->type->name, to->name);
    ERR_N(exp_self(cast)->loc, _("can't cast '%s' to '%s'"),
          cast->exp->type->name, to->name);
  }
  return exp_self(cast)->type;
}

ANN /*static*/ Type scan_class(const Env env, const Type t,
                               const Type_Decl *td);

static OP_CHECK(opck_struct_scan) {
  struct TemplateScan *ts = (struct TemplateScan *)data;
  const Tmpl *tmpl = get_tmpl(ts->t);
  if(tmpl->call) return ts->t;
  return scan_class(env, ts->t, ts->td) ?: env->gwion->type[et_error];
}

ANN static void emit_dot_static_data(const Emitter emit, const Value v,
                                     const bool emit_addr) {
  const m_uint size  = v->type->size;
  const m_uint data = (m_uint)(v->from->owner->class_data + v->from->offset);
  emit_dotstatic(emit, data, size, emit_addr);
}

ANN static void emit_dot_static_import_data(const Emitter emit, const Value v,
                                            const bool emit_addr) {
  if (vflag(v, vflag_builtin) /*&& GET_FLAG(v, const)*/) {
    const m_uint size  = v->type->size;
    emit_regpushimm(emit, (m_uint)v->d.ptr, size, emit_addr);
  } else emit_dot_static_data(emit, v, emit_addr);
}

ANN static void emit_dottmpl(const Emitter emit, const Func f) {
  const Instr instr = emit_add_instr(emit, DotTmpl);
  instr->m_val = (m_uint)f->def;
  struct dottmpl_ *dt = mp_malloc(emit->gwion->mp, dottmpl);
  dt->nspc = emit->env->curr;
  dt->type = emit->env->class_def;
  dt->tmpl_name = tl2str(emit->gwion, f->def->base->tmpl->call, f->def->base->tag.loc);
  instr->m_val2 = (m_uint)dt;
}

ANN static void emit_member_func(const Emitter emit, const Exp_Dot *member) {
  Exp* self = exp_self(member);
  const Func f = self->type->info->func;

  if(is_new(f->def)) {
    if(f != emit->env->func) emit_pushfunc(emit, f);
    return;
  }
  if (f->def->base->tmpl) {
    if(self->is_call) emit_dottmpl(emit, f);
    else {
      if(vflag(f->value_ref, vflag_member))
        emit_regmove(emit, -SZ_INT);
      emit_pushimm(emit, (m_uint)f);
      return;
    }
  } else if (is_static_call(emit->gwion, exp_self(member))) {
    if (self->is_call && f == emit->env->func && !is_new(f->def)) return;
    if(!self->is_call && vflag(f->value_ref, vflag_member)) emit_regmove(emit, -SZ_INT);
    return emit_pushfunc(emit, f);
  } else {
    if (tflag(member->base->type, tflag_struct))
      return emit_pushfunc(emit, f);
    const Instr instr = emit_add_instr(emit, DotFunc);
    instr->m_val      = f->def->vt_index;
    if (!vflag(f->value_ref, vflag_member))
      instr->m_val2 = -SZ_INT;
    else {
      if(self->is_call) emit_regmove(emit, SZ_INT);
      else instr->m_val2 = -SZ_INT;
    }
  }
  return;
}

ANN static inline void emit_member(const Emitter emit, const Value v,
                                   const uint emit_addr) {
  const m_uint size  = v->type->size;
  emit_dotmember(emit, v->from->offset, size, emit_addr);
}

ANN static inline void emit_struct_data(const Emitter emit, const Value v,
                                        const bool emit_addr) {
  emit_structmember(emit, v->from->offset, v->type->size, emit_addr);
  if (!emit_addr) emit_regmove(emit, v->type->size - SZ_INT);
}

ANN static inline Value get_value(const Env env, const Exp_Dot *member,
                                  const Type t) {
  const Value value = find_value(t, member->tag.sym);
  if (value)
    return value;
  if (env->func && env->func->def->base->values)
    return upvalues_lookup(env->func->def->base->values, member->tag.sym);
  if(t->info->values)
    return (Value)scope_lookup1(t->info->values, (m_uint)member->tag.sym);
  return NULL;
}

ANN static bool member_access(const Env env, Exp* exp, const Value value) {
  if (!env->class_def || !isa(env->class_def, value->from->owner_class)) {
    if (GET_FLAG(value, private)) {
      gwlog_error("invalid variable access", "is private", env->name,
                  exp->loc, 0);
      defined_here(value);
      env_set_error(env, true);
      return false;
    } else if (GET_FLAG(value, protect))
      exp_setprot(exp, 1);
  }
  return true;
}

OP_CHECK(opck_object_dot) {
  Exp_Dot *const member      = (Exp_Dot *)data;
  Exp* self = exp_self(member);
  const m_str  str         = s_name(member->tag.sym);
  const bool   base_static = is_class(env->gwion, member->base->type);
  const Type   the_base =
      base_static ? _class_base(member->base->type) : member->base->type;
  const Value value = get_value(env, member, the_base);
  if (!value) {
    const Value v = nspc_lookup_value1(env->curr, member->tag.sym);
    if(v) {
      if (self->is_call) {
        if (is_func(env->gwion, v->type) && (!v->from->owner_class || isa(the_base, v->from->owner_class))) // is_callable needs type
          return v->type;
      }
    }
    env_err(env, member->tag.loc, _("class '%s' has no member '%s'"),
            the_base->name, str);
    if (member->base->type->nspc) did_you_mean_type(the_base, str);
    return env->gwion->type[et_error];
  }
  CHECK_ON(not_from_owner_class(env, the_base, value, self->loc));
  CHECK_ON(member_access(env, self, value));
  if ((base_static && vflag(value, vflag_member)) ||
      (value->from->owner_class != env->class_def && isa(value->from->owner_class, env->class_def)))
    ERR_N(self->loc,
          _("cannot access member '%s.%s' without object instance..."),
          the_base->name, str);
  if (GET_FLAG(value, const)) exp_setmeta(self, true);
  return value->type;
}

ANN static Type member_type(const Gwion gwion, const Type base) {
  const Type t = actual_type(gwion, base);
  return !tflag(t, tflag_ref) ? t: (Type)vector_front(&t->info->tuple->contains);
}

OP_EMIT(opem_object_dot) {
  const Exp_Dot *member = (Exp_Dot *)data;
  const Type     t_base = member_type(emit->gwion, member->base->type);
  const Value    value  = find_value(t_base, member->tag.sym);
  if (is_class(emit->gwion, value->type)) {
    emit_pushimm(emit, (m_uint)value->type);
    return true;
  }
  if (tflag(t_base, tflag_struct) && !GET_FLAG(value, static)) {
    exp_setvar(member->base, true);
    CHECK_B(emit_exp(emit, member->base));
  }
  if (!is_class(emit->gwion, member->base->type) &&
      (vflag(value, vflag_member) ||
       (is_func(emit->gwion, exp_self(member)->type)))) {
    if (!tflag(t_base, tflag_struct))
      CHECK_B(emit_exp(emit, member->base));
  }
  if (is_func(emit->gwion, exp_self(member)->type) &&
      !fflag(exp_self(member)->type->info->func, fflag_fptr))
    emit_member_func(emit, member);
  else if (vflag(value, vflag_member)) {
    if (!tflag(t_base, tflag_struct))
      emit_member(emit, value, exp_getvar(exp_self(member)));
    else
      emit_struct_data(emit, value, exp_getvar(exp_self(member)));
  } else {
    assert(GET_FLAG(value, static));
    emit_dot_static_import_data(emit, value, exp_getvar(exp_self(member)));
  }
  if(is_object(emit->gwion, value->type) &&
     !exp_getvar(exp_self(member)) &&
    (GET_FLAG(value, static) || GET_FLAG(value, late)))
    emit_fast_except(emit, value->from, exp_self(member)->loc);
  return true;
}

ANN static bool scantmpl_class_def(const Env env, struct tmpl_info *info) {
  const Class_Def c    = info->base->info->cdef;
  const Class_Def cdef = new_class_def(
      env->gwion->mp, c->flag, MK_TAG(info->name,c->base.tag.loc),
      c->base.ext ? cpy_type_decl(env->gwion->mp, c->base.ext) : NULL,
      NULL);
  if(c->body) cdef->body = cpy_ast(env->gwion->mp, c->body);
  cdef->cflag      = c->cflag;
  cdef->base.tmpl  = mk_tmpl(env, c->base.tmpl, info->td->types);
  const bool ret = (!is_spread_tmpl(cdef->base.tmpl) || sema_variadic_class(env, cdef)) &&
                 scan0_class_def(env, cdef);
  if ((info->ret = cdef->base.type)) {
    info->ret->info->cdef = cdef;
    set_tflag(info->ret, tflag_cdef);
  } else
    free_class_def(env->gwion->mp, cdef);
  return ret;
}

ANN static bool scantmpl_union_def(const Env env, struct tmpl_info *info) {
  const Union_Def u    = info->base->info->udef;
  const Union_Def udef = new_union_def(
      env->gwion->mp, cpy_variable_list(env->gwion->mp, u->l), u->tag.loc);
  udef->tag.sym  = info->name;
  udef->tmpl = mk_tmpl(env, u->tmpl, info->td->types);
  // resolve the template here
  if (GET_FLAG(info->base, global)) SET_FLAG(udef, global);
  const bool ret = scan0_union_def(env, udef);
  if (udef->type) {
    udef->type->info->udef = udef;       // mark as udef
    info->ret              = udef->type; // is info->ret necessary?
    set_tflag(info->ret, tflag_udef);
  } else
    free_union_def(env->gwion->mp, udef);
  return ret;
}

ANN static Type _scan_class(const Env env, struct tmpl_info *info) {
  if (info->base->info->parent != env->gwion->type[et_union])
    CHECK_O(scantmpl_class_def(env, info));
  else
    CHECK_O(scantmpl_union_def(env, info));
  return info->ret;
}

ANN Type scan_class(const Env env, const Type t, const Type_Decl *td) {
  struct tmpl_info info = {
      .base = t, .td = td, .list = get_tmpl(t)->list};
  const Type exists = tmpl_exists(env, &info);
  if (exists) return exists != env->gwion->type[et_error] ? exists : NULL;
  struct EnvSet es    = {.env   = env,
                      .data  = env,
                      .func  = (_envset_func)scan0_cdef,
                      .scope = env->scope->depth,
                      .flag  = tflag_check};
  CHECK_O(envset_pushv(&es, t->info->value));
  const Type ret = _scan_class(env, &info);
  envset_popv(&es, t->info->value);
  return ret;
}

static OP_EMIT(opem_not_object) {
  const Vector v    = &emit->code->instr;
  const Instr  back = (Instr)vector_back(v);
  if (back->opcode == eGWOP_EXCEPT) {
    back->opcode = eIntNot;
    return true;
  } else if (back->opcode == eOP_MAX && back->execute == fast_except) {
    back->opcode = eIntNot;
    return true;
  }
  const Instr instr = emit_add_instr(emit, RegSetImm);
  instr->m_val2     = -SZ_INT;
  return true;
}

static OP_EMIT(opem_uncond_object) {
  const Vector v    = &emit->code->instr;
  if(vector_size(v) >= 2) {
    const Instr  back = (Instr)vector_at(v, vector_size(v) -2);
    if (back->opcode == eGWOP_EXCEPT || (back->opcode == eOP_MAX && back->execute == fast_except)) {
      free_instr(emit->gwion, back);
      vector_rem(v, vector_size(v) - 2);
    }
  }
  emit_add_instr(emit, BranchNeqInt);
  return true;
}

static OP_EMIT(opem_cond_object) {
  const Vector v    = &emit->code->instr;
  if(vector_size(v) >= 2) {
    const Instr  back = (Instr)vector_at(v, vector_size(v) -2);
    if (back->opcode == eGWOP_EXCEPT || (back->opcode == eOP_MAX && back->execute == fast_except)) {
      free_instr(emit->gwion, back);
      vector_rem(v, vector_size(v) - 2);
    }
  }
  emit_add_instr(emit, BranchEqInt);
  return true;
}

GWION_IMPORT(object_op) {
  const Type t_error         = gwi_mk_type(gwi, "@error", 0, NULL);
  gwi->gwion->type[et_error] = t_error;
  gwi_set_global_type(gwi, t_error, et_error);
  GWI_B(gwi_oper_ini(gwi, "Object", "Object", NULL))
  GWI_B(gwi_oper_add(gwi, opck_object_at))
  GWI_B(gwi_oper_emi(gwi, opem_object_at))
  GWI_B(gwi_oper_end(gwi, ":=>", NULL))
  //GWI_B(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "@Compound", NULL))
  GWI_B(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "Compound", NULL))
  GWI_B(gwi_oper_add(gwi, opck_object_instance))
  GWI_B(gwi_oper_end(gwi, "=>", NULL))
  GWI_B(gwi_oper_ini(gwi, "Object", "Object", "bool"))
  GWI_B(gwi_oper_end(gwi, "==", EqObject))
  GWI_B(gwi_oper_end(gwi, "!=", NeqObject))
  GWI_B(gwi_oper_add(gwi, opck_object_cast))
  GWI_B(gwi_oper_end(gwi, "$", NULL))
  GWI_B(gwi_oper_ini(gwi, NULL, "Object", "bool"))
  GWI_B(gwi_oper_emi(gwi, opem_uncond_object))
  GWI_B(gwi_oper_end(gwi, "@unconditional", NULL))
  GWI_B(gwi_oper_emi(gwi, opem_cond_object))
  GWI_B(gwi_oper_end(gwi, "@conditional", NULL))
  GWI_B(gwi_oper_add(gwi, opck_unary_meta2))
  GWI_B(gwi_oper_emi(gwi, opem_not_object))
  GWI_B(gwi_oper_end(gwi, "!", NULL))
  //GWI_B(gwi_oper_ini(gwi, "@Compound", NULL, NULL))
  GWI_B(gwi_oper_ini(gwi, "Compound", NULL, NULL))
  GWI_B(gwi_oper_add(gwi, opck_struct_scan))
  GWI_B(gwi_oper_end(gwi, "class", NULL))
  return true;
}
