
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
    Var_Decl vd = bin->rhs->d.exp_decl.vd;
    SET_FLAG(vd.value, late);
  }
  exp_setvar(bin->rhs, 1);
  CHECK_BO(isa(bin->lhs->type, bin->rhs->type));
  bin->rhs->ref = bin->lhs;
  return bin->rhs->type;
}

static OP_CHECK(opck_object_instance) {
  Exp_Binary *bin = (Exp_Binary*)data;
  Exp rhs = bin->rhs;
  if (rhs->exp_type != ae_exp_decl)
    return NULL;
  if (rhs->d.exp_decl.td->array)
    return NULL;
  Exp lhs = bin->lhs;
  Exp e = exp_self(bin);
  Exp_Decl *const decl = &e->d.exp_decl;
  e->exp_type = ae_exp_decl;
  decl->td = cpy_type_decl(env->gwion->mp, rhs->d.exp_decl.td);
  decl->vd = rhs->d.exp_decl.vd;
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
  return GW_OK;
}

static OP_CHECK(opck_object_cast) {
  const Exp_Cast *cast = (Exp_Cast *)data;
  const Type      to   = known_type(env, cast->td);
  if (isa(cast->exp->type, to) < 0) {
    if (isa(to, cast->exp->type) > 0)
      ERR_N(exp_self(cast)->pos, _("can't upcast '%s' to '%s'"),
            cast->exp->type->name, to->name);
    ERR_N(exp_self(cast)->pos, _("can't cast '%s' to '%s'"),
          cast->exp->type->name, to->name);
  }
  return exp_self(cast)->type;
}

ANN /*static*/ Type scan_class(const Env env, const Type t,
                               const Type_Decl *td);

static OP_CHECK(opck_struct_scan) {
  struct TemplateScan *ts = (struct TemplateScan *)data;
  if(ts->t->info->cdef->base.tmpl->call) return ts->t;
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
  dt->tmpl_name = tl2str(emit->gwion, f->def->base->tmpl->call, f->def->base->pos);
  instr->m_val2 = (m_uint)dt;
}

ANN static void emit_member_func(const Emitter emit, const Exp_Dot *member) {
  const Func f = exp_self(member)->type->info->func;

  if(is_new(f->def)) {
    if(f != emit->env->func) emit_pushfunc(emit, f);
    return;
  }
  if (f->def->base->tmpl) {
    if(member->is_call) emit_dottmpl(emit, f);
    else {
      if(vflag(f->value_ref, vflag_member))
        emit_regmove(emit, -SZ_INT);
      emit_pushimm(emit, (m_uint)f);
      return;
    }
  } else if (is_static_call(emit->gwion, exp_self(member))) {
    if (member->is_call && f == emit->env->func && !is_new(f->def)) return;
    if(!member->is_call && vflag(f->value_ref, vflag_member)) emit_regmove(emit, -SZ_INT);
    return emit_pushfunc(emit, f);
  } else {
    if (tflag(member->base->type, tflag_struct))
      return emit_pushfunc(emit, f);
    const Instr instr = emit_add_instr(emit, DotFunc);
    instr->m_val      = f->def->vt_index;
    if (!vflag(f->value_ref, vflag_member))
      instr->m_val2 = -SZ_INT;
    else {
      if(member->is_call) emit_regmove(emit, SZ_INT);
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

ANN m_bool not_from_owner_class(const Env env, const Type t, const Value v,
                                const loc_t pos);

ANN static inline Value get_value(const Env env, const Exp_Dot *member,
                                  const Type t) {
  const Value value = find_value(t, member->xid);
  if (value)
    return value;
  if (env->func && env->func->def->base->values)
    return upvalues_lookup(env->func->def->base->values, member->xid);
  if(t->info->values)
    return (Value)scope_lookup1(t->info->values, (m_uint)member->xid);
  return NULL;
}

ANN static Type class_type(const Env env, const Exp_Dot *member, const Type base) {
  const Type parent = actual_type(env->gwion, base);
  if(!tflag(parent, tflag_tmpl)) return parent;
  Type_Decl td = {
       .xid=insert_symbol(env->gwion->st, parent->name),
       .types = member->is_call->tmpl ? member->is_call->tmpl->call : NULL
  };
  return known_type(env, &td);
}

OP_CHECK(opck_object_dot) {
  Exp_Dot *const member      = (Exp_Dot *)data;
  const m_str    str         = s_name(member->xid);
  const m_bool   base_static = is_class(env->gwion, member->base->type);
  const Type     the_base =
      base_static ? _class_base(member->base->type) : member->base->type;
  if (member->xid == insert_symbol(env->gwion->st, "this") && base_static)
    ERR_N(exp_self(member)->pos,
          _("keyword 'this' must be associated with object instance..."));
//  if (member->base->exp_type == ae_exp_decl) {
//puts(env->name);
//return env->gwion->type[et_error];
//    exit(12);
//  }
  const Value value = get_value(env, member, the_base);
  if (!value) {
    const Value v = nspc_lookup_value1(env->curr, member->xid);
    if(v) {
      if (member->is_call) {
        if (is_func(env->gwion, v->type) && (!v->from->owner_class || isa(the_base, v->from->owner_class) > 0)) // is_callable needs type
          return v->type;
        if (is_class(env->gwion, v->type)) {
          DECL_OO(const Type, parent, = class_type(env, member, v->type));
          if (the_base == parent) {
            const Symbol sym = insert_symbol(env->gwion->st, "new");
            const Value ret = nspc_lookup_value1(parent->nspc, sym);
            member->xid = sym;
            if(ret) return ret->type;
          }
          if (the_base->info->parent == parent && parent->nspc) {
            const Symbol sym = insert_symbol(env->gwion->st, "new");
            if(!env->func || env->func->def->base->xid != sym)
              ERR_N(exp_self(member)->pos, "calling a parent constructor is only allowed in `new` definition");
            // is this
            if(member->base->exp_type != ae_exp_primary ||
               member->base->d.prim.prim_type != ae_prim_id ||
               strcmp(s_name(member->base->d.prim.d.var), "this"))
              ERR_N(exp_self(member)->pos, "calling a parent constructor is only allowed with `this`");
            if(GET_FLAG(env->func, const))
              ERR_N(exp_self(member)->pos, "parent constructor already called");
            SET_FLAG(env->func, const); // mark the function as ctor_ok
            const Value ret = nspc_lookup_value1(parent->nspc, sym);
            member->xid = sym;
            if(ret) return ret->type;
          }
        }
      }
    }
    env_err(env, exp_self(member)->pos, _("class '%s' has no member '%s'"),
            the_base->name, str);
    if (member->base->type->nspc) did_you_mean_type(the_base, str);
    return env->gwion->type[et_error];
  }
  CHECK_BN(not_from_owner_class(env, the_base, value, exp_self(member)->pos));
  if (!env->class_def || isa(env->class_def, value->from->owner_class) < 0) {
    if (GET_FLAG(value, private)) {
      gwerr_basic("invalid variable access", "is private", NULL, env->name,
                  exp_self(member)->pos, 0);
      env_error_footer(env);
      defined_here(value);
      env_set_error(env, true);
    } else if (GET_FLAG(value, protect))
      exp_setprot(exp_self(member), 1);
  }
  if (base_static && vflag(value, vflag_member))
    ERR_N(exp_self(member)->pos,
          _("cannot access member '%s.%s' without object instance..."),
          the_base->name, str);
  if (GET_FLAG(value, const)) exp_setmeta(exp_self(member), 1);
  exp_self(member)->acquire = 1;
  return value->type;
}

ANN static Type member_type(const Gwion gwion, const Type base) {
  const Type t = actual_type(gwion, base);
  return !tflag(t, tflag_ref) ? t: (Type)vector_front(&t->info->tuple->contains);
}

OP_EMIT(opem_object_dot) {
  const Exp_Dot *member = (Exp_Dot *)data;
  const Type     t_base = member_type(emit->gwion, member->base->type);
  const Value    value  = find_value(t_base, member->xid);
//  if(!tflag(t_base, tflag_emit) /*&& emit->env->class_def != t_base*/)
//      ensure_emit(emit, t_base);
  if (is_class(emit->gwion, value->type)) {
    emit_pushimm(emit, (m_uint)value->type);
    return GW_OK;
  }
  if (tflag(t_base, tflag_struct) && !GET_FLAG(value, static)) {
    exp_setvar(member->base, true);
    CHECK_BB(emit_exp(emit, member->base));
  }
  if (!is_class(emit->gwion, member->base->type) &&
      (vflag(value, vflag_member) ||
       (is_func(emit->gwion, exp_self(member)->type)))) {
    if (!tflag(t_base, tflag_struct))
      CHECK_BB(emit_exp(emit, member->base));
  }
  if (is_func(emit->gwion, exp_self(member)->type) &&
      !fflag(exp_self(member)->type->info->func, fflag_fptr))
    emit_member_func(emit, member);
  else if (vflag(value, vflag_member)) {
    if (!tflag(t_base, tflag_struct))
      emit_member(emit, value, exp_getvar(exp_self(member)));
    else
      emit_struct_data(emit, value, exp_getvar(exp_self(member)));
  } else if (GET_FLAG(value, static))
    emit_dot_static_import_data(emit, value, exp_getvar(exp_self(member)));
  else exit(3); //emit_pushimm(emit, (m_uint)value->type);
  if(isa(value->type, emit->gwion->type[et_object]) > 0 &&
     !exp_getvar(exp_self(member)) &&
    (GET_FLAG(value, static) || GET_FLAG(value, late)))
    emit_fast_except(emit, value->from, exp_self(member)->pos);
  return GW_OK;
}

ANN static m_bool scantmpl_class_def(const Env env, struct tmpl_info *info) {
  const Class_Def c    = info->base->info->cdef;
  const Class_Def cdef = new_class_def(
      env->gwion->mp, c->flag, info->name,
      c->base.ext ? cpy_type_decl(env->gwion->mp, c->base.ext) : NULL,
      NULL, c->pos);
  if(c->body) cdef->body = cpy_ast(env->gwion->mp, c->body);
  cdef->cflag      = c->cflag;
  cdef->base.tmpl  = mk_tmpl(env, c->base.tmpl, info->td->types);
  const m_bool ret = scan0_class_def(env, cdef);
  if ((info->ret = cdef->base.type)) {
    info->ret->info->cdef = cdef;
    set_tflag(info->ret, tflag_cdef);
  } else
    free_class_def(env->gwion->mp, cdef);
  return ret;
}

ANN static m_bool scantmpl_union_def(const Env env, struct tmpl_info *info) {
  const Union_Def u    = info->base->info->udef;
  const Union_Def udef = new_union_def(
      env->gwion->mp, cpy_union_list(env->gwion->mp, u->l), u->pos);
  udef->xid  = info->name;
  udef->tmpl = mk_tmpl(env, u->tmpl, info->td->types);
  // resolve the template here
  if (GET_FLAG(info->base, global)) SET_FLAG(udef, global);
  const m_bool ret = scan0_union_def(env, udef);
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
    CHECK_BO(scantmpl_class_def(env, info));
  else
    CHECK_BO(scantmpl_union_def(env, info));
  return info->ret;
}

ANN Type scan_class(const Env env, const Type t, const Type_Decl *td) {
  struct tmpl_info info = {
      .base = t, .td = td, .list = t->info->cdef->base.tmpl->list};
  const Type exists = tmpl_exists(env, &info);
  if (exists) return exists != env->gwion->type[et_error] ? exists : NULL;
  struct EnvSet es    = {.env   = env,
                      .data  = env,
                      .func  = (_exp_func)scan0_cdef,
                      .scope = env->scope->depth,
                      .flag  = tflag_check};
  const Type    owner = t->info->value->from->owner_class;
  CHECK_BO(envset_pushv(&es, t->info->value));
  const bool local = !owner && !tmpl_global(env, td->types) && from_global_nspc(env, env->curr);
  if(local && env->context) env_push(env, NULL, env->context->nspc);
  const Type ret = _scan_class(env, &info);
  if(local && env->context)env_pop(env, es.scope);
  envset_pop(&es, owner);
  return ret;
}

static OP_EMIT(opem_not_object) {
  const Vector v    = &emit->code->instr;
  const Instr  back = (Instr)vector_back(v);
  if (back->opcode == eGWOP_EXCEPT) {
    back->opcode = eIntNot;
    return GW_OK;
  } else if (back->opcode == eOP_MAX && back->execute == fast_except) {
    back->opcode = eIntNot;
    return GW_OK;
  }
  const Instr instr = emit_add_instr(emit, RegSetImm);
  instr->m_val2     = -SZ_INT;
  return GW_OK;
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
  return GW_OK;
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
  return GW_OK;
}

GWION_IMPORT(object_op) {
  const Type t_error         = gwi_mk_type(gwi, "@error", 0, NULL);
  gwi->gwion->type[et_error] = t_error;
  GWI_BB(gwi_set_global_type(gwi, t_error, et_error))
  GWI_BB(gwi_oper_ini(gwi, "Object", "Object", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_object_at))
  GWI_BB(gwi_oper_emi(gwi, opem_object_at))
  GWI_BB(gwi_oper_end(gwi, ":=>", NULL))
  GWI_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "@Compound", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_object_instance))
  GWI_BB(gwi_oper_end(gwi, "=>", NULL))
  GWI_BB(gwi_oper_ini(gwi, "Object", "Object", "bool"))
  GWI_BB(gwi_oper_end(gwi, "==", EqObject))
  GWI_BB(gwi_oper_end(gwi, "!=", NeqObject))
  GWI_BB(gwi_oper_add(gwi, opck_object_cast))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
  GWI_BB(gwi_oper_ini(gwi, NULL, "Object", "bool"))
  GWI_BB(gwi_oper_emi(gwi, opem_uncond_object))
  GWI_BB(gwi_oper_end(gwi, "@unconditional", NULL))
  GWI_BB(gwi_oper_emi(gwi, opem_cond_object))
  GWI_BB(gwi_oper_end(gwi, "@conditional", NULL))
  GWI_BB(gwi_oper_emi(gwi, opem_not_object))
  GWI_BB(gwi_oper_end(gwi, "!", NULL))
  GWI_BB(gwi_oper_ini(gwi, "@Compound", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_struct_scan))
  GWI_BB(gwi_oper_end(gwi, "class", NULL))
  return GW_OK;
}
